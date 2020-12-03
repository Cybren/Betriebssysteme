/*! \file
 *  \brief Hilfsstrukturen zum Zugriff auf den IOAPIC
 */

#pragma once

/*! \brief Einstellungsmöglichkeiten für den IOAPIC
 */
namespace IOAPIC {
	typedef uint32_t Index;
	typedef uint32_t Register;

	extern volatile Index *IOREGSEL_REG;
	extern volatile Register *IOWIN_REG;

	/*! \brief ID des IO-APICs
	 *
	 *  Das IOAPICID Register liegt an Registernummer 0x0. Die IOAPICID wird
	 *  während des Bootvorgangs aus den Systemkonfigurationstabellen des BIOS
	 *  gelesen. Sie ist über System::getIOAPICID() abfragbar. Diese Information
	 *  muss bei der Initialisierung des IOAPICs ins IOAPICID Register eingetragen
	 *  werden.
	 *
	 * \see [IO-APIC manual](intel_ioapic.pdf#page=9), p. 9
	 */
	union Identification {
		struct {
			uint32_t     : 24,  ///< Reserviert, nicht ändern
			         id  :  4,  ///< IOAPIC Identification
			             :  4;  ///< Reserviert, nicht ändern
		};
		Register value;
		Identification(Register value) : value(value) {}
	} __attribute__((packed));
	static_assert(sizeof(Identification) == 4, "IOAPIC Identification has wrong size");

	/*! \brief Reaktionstyp eines Prozessor auf einen Interrupt */
	enum DeliveryMode {
		Fixed          = 0,  ///< normaler Interrupt an alle Zielprozessoren aus Maske
		LowestPriority = 1,  ///< normaler Interrupt an Zielprozessor aus der Maske mit der niedrigsten Priorität
		SMI            = 2,  ///< Systemverwaltungsunterbrechung, Vektornummer muss 0 sein
		// Reserved
		NMI            = 4,  ///< Nicht-maskierbarer Interrupt, Vektornummer wird ignoriert, nur flankengesteuert
		INIT           = 5,  ///< Initialisierungssignal (nur flankengesteuert)
		// Reserved
		ExternInt      = 7   ///< externer Interrupt (nur flankengesteuert)
	};

	 /*! \brief Interpretationen der Zielprozessorangabe in einem
	  * Redirection Table Eintrag
 	 */
	enum DestinationMode {
		 Physical = 0,  ///< Sende an Ziel (APIC) ID
		 Logical  = 1   ///< Sende an Gruppe von Prozessoren
	};

	/*! \brief Interruptpolaritäten für den Eintrags in der Redirection Table
	 */
	enum Polarity {
		High = 0,  ///< Aktiv bei hohem Pegel
		Low  = 1   ///< Aktiv bei niedrigem Pegel
	};

	/*! \brief Interrupterkennung */
	enum TriggerMode {
		Edge  = 0,  ///< Flankengesteuert
		Level = 1   ///< Pegelgesteuert
	};
	/*! \brief Interruptstatus */
	enum DeliveryStatus {
		Idle  = 0,       ///< keine Aktivität für Interrupt
		SendPending = 1  ///< Interrupt wird versendet sobald Bus / LAPIC frei ist
	};

	/*! \brief Interruptmaske */
	enum InterruptMask {
		Unmasked = 0,  ///< Interrupteintrag ist aktiv (nicht maskiert)
		Masked = 1     ///< Interrupteintrag ist deaktiviert (maskiert)
	};

	/*! \brief Eintrag in der Redirection Tablle
	 *
	 * Die Redirection Tabelle beginnt bei IOAPIC-Register `0x10` und
	 * endet bei `0x3f`.
	 *
	 * Jeder Eintrag umfasst 64 bit, also 2 Register des IOAPICs. value_low ist
	 * das niederwertige Wort, value_high das höherwertige. Eintrag 0 in der
	 * Tabelle wären dann die Register `0x10` (entspricht value_low) und
	 * `0x11` (entspricht value_high).
	 *
	 * Durch die Überlagerung (union) kann der Eintrag in einer Bitfeldstruktur
	 * gesetzt werden und anschliessend die Werte für die beiden Register
	 * verwendet werden.
	 *
	 * \see [IO-APIC manual](intel_ioapic.pdf#page=11), p. 11-13
	 */
	union RedirectionTableEntry {
		struct {
			/*! \brief Der Wert gibt an, welcher Vektor in der
			 *  Interruptvektortabelle für den entsprechenden Interrupt
			 *  aktiviert werden soll.
			 */
			uint64_t vector : 8;

			/*! \brief Delivery Mode gibt an, wie die adressierten CPUs
			 * (bzw. ihre local APICs) auf das Interruptsignal reagieren
			 * sollen. In OO/MPStubs soll DeliveryMode::LowestPriority
			 * verwendet werden. Da die einzelnen CPUs alle dieselbe
			 * Priorität besitzen, werden so die auftretenden Interrupts
			 * gleich über alle CPUs verteilt.
			 */
			DeliveryMode delivery_mode : 3;

			/*! \brief Destination Mode legt fest, wie Logical Destination
			 *  interpretiert werden soll. In OO/MPStubs soll hier
			 *  `DestinationMode::Logical` verwendet werden.
			 */
			DestinationMode destination_mode : 1;

			/*! \brief Delivery Status gibt den aktuellen Status über den
			 * Versand des Interrupts an.
			 *
			 * \note Dieses Feld kann nur gelesen werden, Schreibzugriffe werden
			 * ignoriert
			 */
			DeliveryStatus delivery_status : 1;

			/*! \brief polarity gibt an, wann ein Interrupt signalisiert
			 * werden soll.
			 * OO/MPStubs soll hier `Polarity::High` verwenden.
			 */
			Polarity polarity : 1;

			/*! \brief Das Remote IRR Bit wird bei pegelgesteuerte Interrupts
			 * gesetzt, wenn der LAPIC den Interrupt akzeptiert.
			 *
			 * \note Dieses Feld darf nur gelesen werden.
			 */
			uint64_t remote_irr : 1;

			/*! \brief Trigger Mode gibt an, ob die Interruptsignalisierung
			 * pegel- oder flankengesteuert ist. OO/MPStubs verwendet hier
			 * TriggerMode::Edge für die Tastatur und den Timer, die (optionale)
			 * serielle Schnittstelle benötigt jedoch TriggerMode::Level
			 */
			TriggerMode trigger_mode : 1;

			/*! \brief Die Interruptmaske gibt an, ob für die entsprechende
			 * Quelle Interrupts zugelassen werden sollen oder nicht.
			 * Dabei gibt InterruptMask::Masked an, dass die Interruptquelle
			 * derzeit deaktiviert (maskiert) ist.
			 * Entsprechend gibt InterruptMask::Unmasked an, dass die Quelle
			 * aktiviert (demaskiert) ist.
			 */
			InterruptMask interrupt_mask : 1;

			/*! \brief Reserviert,
			 * Nicht ändern.
			 */
			uint64_t reserved : 39;

			/*! \brief Destination beschreibt im Logical Mode als Bitmaske eine
			 * Menge von Prozessoren, die potentiell als Empfänger der
			 * Interruptnachricht in Frage kommen. Für OOStuBS ist dies
			 * logischerweise `1`, für MPStuBS müssen in der Bitmaske die
			 * untersten `n` Bits auf `1` gesetzt sein, wobei `n` die
			 * Anzahl der im System vorhandenen CPUs ist (siehe
			 * System::getNumberOfCPUs()). Damit kommen alle CPUs als
			 * Empfänger infrage und die Interruptlast wird so gleichmäßig
			 * über alle CPUs verteilt.
			 *
			 * \note In QEMU funktioniert dies nicht. Dort bekommt immer
			 * `CPU 0` (also der BSP) alle Interrupts.
			 */
			uint64_t destination : 8;
		} __attribute__((packed));

		/*! \brief Eintrag in der IO Redirection Table
		 *
		 *  Jeder Eintrag in der IO Redirection Table symbolisiert eine externe
		 *  Interruptquelle und ist 64 Byte groß. Jedoch wird er wegen des
		 *  32bit-Zugriffs aufgeteilt in nieder- (low) und höherwertigen (high)
		 *  Eintrag aufgeteilt.
		 */
		struct {
			Register value_low;   ///< Erstes (niederwertiges) Register
			Register value_high;  ///< Zweites (höherwertiges) Register
		} __attribute__((packed));
		RedirectionTableEntry(Register value_low, Register value_high) : value_low(value_low), value_high(value_high) {}
	};

	static_assert(sizeof(RedirectionTableEntry) == 8, "Entry of Redirection Table has wrong size");
}  // namespace IOAPIC
