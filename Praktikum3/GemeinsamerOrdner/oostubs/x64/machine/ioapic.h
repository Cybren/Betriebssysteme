/*! \file
 *  \brief Enthält die Klasse IOAPIC zum Zugriff auf den IO-APIC
 */

#pragma once

#include "types.h"
#include "machine/ioapic_registers.h"
#include "machine/plugbox.h"

/*! \brief Abstraktion des IO-APICs, der zur Verwaltung der externen Interrupts
 *  dient.
 *  \ingroup interrupts
 *
 *  Kernstück des IOAPICs ist die IO-Redirection Table. Dort lässt sich frei
 *  konfigurieren, welchem Interruptvektor eine bestimmte externe
 *  Unterbrechung zugeordnet werden soll. Ein Eintrag in dieser Tabelle ist
 *  64 Bit breit. Die Struktur IOAPIC::RedirectionTableEntry ist Bitfelder, das
 *  die einzelnen Einstellungen eines Eintrages zugänglich macht.
 */

namespace IOAPIC
{
	/*! \brief Initialisierung der IOAPICs
	 *
	 *  Dabei werden alle Einträge in der IO-Redirection Table mit einem
	 *  sinnvollen Wert vorbelegt. Das Feld, welches den auszulösenden
	 *  Interruptvektor angibt, sollte mit einer Vektornummer vorbelegt werden,
	 *  die so konfiguriert ist, dass sie den Panic-Handler auslöst.
	 *  Initial sollten auch alle externen Interrupts im IOAPIC deaktiviert
	 *  werden. Neben der Vektortabelle muss auch die APICID im
	 *  IOAPICID-Register auf den Wert gesetzt werden, der während des
	 *  Bootvorgangs aus den Systembeschreibungstabellen gelesen wurde.
	 *  (System::getIOAPICID())
	 */
	void init();

	/*! \brief Zuordnung eines Vektors in der Interruptvektortabelle zu einem
	 *  externen Interrupt.
	 *
	 *  \param slot Nummer des zu konfigurierenden Slots (und damit des
	 *  dazugehörigen externen Interrupts) in der IO-Redirection Table.
	 *  \param vector Nummer des Vektors, der durch den mit slot ausgewählten
	 *  externen Interrupt aktiviert werden soll.
	 *  \param trigger_mode Flanken- oder pegelgesteuerte Interruptsignalisierung
	 *  (letzteres wird bei der optionalen seriellen Schnittstelle benötigt)
	 *  \param polarity Art der Interruptsignalisierung
	 */
	void config(uint8_t slot, Plugbox::Vector vector, TriggerMode trigger_mode = TriggerMode::Edge, Polarity polarity = Polarity::High);

	/*! \brief Sorgt dafür, dass Unterbrechungen des zugeordneten Gerätes an
	 *  die CPU(s) weitergereicht werden.
	 *
	 *  Um eine Unterbrechungsbehandlung zu ermöglichen, muss zusätzlich
	 *  CPU::Interrupt::enable() in der main() aufgerufen werden.
	 *
	 * \param slot Nummer des freizuschaltenden Slots
	*/
	void allow(uint8_t slot);

	/*! \brief Ermöglicht einzelne Interrupts selektiv zu sperren.
	 *  \param slot Nummer des zu sperrenden Slots
	 *
	 *  \todo Funktion implementieren
	 *
	 */
	void forbid(uint8_t slot);

	/*! \brief Ermöglicht eine Abfrage des Maskierungsstatus für einzelne
	 *  Interrupts.
	 *  \param slot gibt an, für welchen Slot der Status abgefragt
	 *  werden soll.
	 *  \return gibt `true` zurück, falls der Interrupt zugelassen ist und
	 *  `false`, falls er ausmaskiert wurde.
	 */
	bool status(uint8_t slot);
};
