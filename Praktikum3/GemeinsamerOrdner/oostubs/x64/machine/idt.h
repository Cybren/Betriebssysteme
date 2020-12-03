#pragma once

#include "types.h"
namespace IDT {

	// Gate types
	enum Gate {
		GATE_TASK = 0x5,
		GATE_INT  = 0x6,
		GATE_TRAP = 0x7,
	};

	// Segment type (16 oder 32bit)
	enum GateSize {
		GATE_SIZE_16 = 0,
		GATE_SIZE_32 = 1,
	};

	// Descriptor Privilege Level
	enum DPL {
		DPL_KERNEL = 0,
		DPL_USER = 3,
	};

	/*! \brief Initialisiere die IDT mit den Standardeinsprungsfunktion für die
	 * Interruptbehandlung
	 *
	 * Es werden die 'irq_handler_VECTOR'-Behandlungsroutinen aus der 'interrupt.asm'
	 * in die Interrupt Description Table eingetragen, welche den Kontext sichern
	 * und zu guardian() springen.
	 *
	 * \param max Höchster Interruptvektor (inklusive)
	 */
	void init(uint8_t max = 255);

	/*! \brief Einsprungsfunktion für Interruptbehandlung
	 *
	 * Die gewählte Einsprungsfunktion muss zuerst die ganze Sicherung der Register
	 * übernehmen.
	 *
	 * \param vector Interruptvektor zu der die Behandlung gesetzt/geändert werden soll
	 * \param handler (lowlevel) Einsrpungsfunktion für die Behandlung
	 * \param type Gatetyp (Interrupt, Trap oder Task)
	 * \param size 16- oder 32-bit
	 * \param dpl Berechtigung für die Interruptauslösung (Kernel oder Userspace)
	 * \param present Ist die Behandlung Verfügbar?
	 */
	void handle(uint8_t vector, void * handler, enum Gate type = Gate::GATE_INT, enum GateSize size = GateSize::GATE_SIZE_32, enum DPL dpl = DPL::DPL_KERNEL, bool present = true);
}
