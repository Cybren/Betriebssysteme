/*! \file
 * \brief Enthält die Klasse LAPIC zum Zugriff auf den Local APIC
 *
 *  Local APIC referenziert in Intel System Programming Guide 3A
 */

#pragma once

#include "types.h"

/*! \brief Abstraktion des in der CPU integrierten local APICs.
 *  \ingroup interrupts
 *
 *
 *  In modernen PCs besitzt jede CPU einen sogenannten "local APIC". Dieser
 *  vermittelt zwischen dem I/O APIC, an den die externen Interruptquellen
 *  angeschlossen sind, und der CPU. Interruptnachrichten, welche den
 *  lokalen APIC von aussen erreichen, werden an den zugeordneten Prozessorkern
 *  weitergereicht, um dort die Interruptbearbeitung anzustoßen.
 *
 *  In Multiprozessorsystem ist es darüberhinaus noch möglich mit Hilfe des
 *  lokalen APICs Nachrichten in Form von Interprozessorinterrupts an andere
 *  CPUs zu schicken bzw. zu empfangen.
 */
namespace LAPIC {
	/*! \brief Initialisiert den local APIC der jeweiligen CPU und setzt die
	 * lokale APIC ID im LDR Register
	 * \param logicalID zu setzende APIC ID
	 */
	void init(uint8_t logicalID);

	/*! \brief Signalisiert EOI (End of interrupt)
	 *
	 *  Teilt dem local APIC mit, dass die aktuelle Interruptbehandlung
	 *  abgeschlossen ist. Diese Funktion muss gegen Ende der
	 *  Unterbrechungsbehandlung aufgerufen werden und zwar bevor
	 *  prozessorseitig die Unterbrechungen wieder zugelassen werden.
	 */
	void endOfInterrupt();

	/*! \brief Liefert die ID des in der aktuellen CPU integrieren APICs
	 *  \return lAPIC ID
	 */
	uint8_t getID();

	uint8_t getLogicalID();
	void setLogicalID(uint8_t id);
	/*! \brief Liefert Versionsnummer des local APICs
	 *  \return Versionsnummer
	 */
	uint8_t getVersion();

namespace IPI {

	/*! \brief Kehrt mit true zurück, falls zum Zeitpunkt des Aufrufs kein IPI
	 *  aktiv ist.
	 *
	 *  \return false, falls der letzte gesendete IPI noch nicht vom Zielprozessor akzeptiert wurde
	 */
	bool isDelivered();

	void send(uint8_t destination, uint8_t vector);

	void sendGroup(uint8_t logical_destination, uint8_t vector);

	void sendAll(uint8_t vector);

	void sendOthers(uint8_t vector);

	void sendInit(bool assert = true);

	void sendStartup(uint8_t vector);

}  // namespace IPI

namespace Timer {

	/*! \brief Ermittelt die Frequenz des LAPIC-Timers.
	 *  \return Anzahl der Timerticks pro Millisekunde
	 *
	 */
	uint32_t ticks();

	/*! \brief Stellt den LAPIC-Timer ein.
	 *  \param counter Startwert des Zählers, der bei jedem Bus-Takt dekrementiert wird.
	 *  \param divide Zweierpotenz, durch die der Bustakt geteilt werden soll (wird als numerischer Wert (1, 2, 4, 8, ...) übergeben, zur Umrechnung in die passende Bitmaske siehe LAPIC::timer_div)
	 *  \param vector Nummer des auszulösenden Interrupts.
	 *  \param periodic Gibt an, ob die Unterbrechung periodisch kommen soll
	 *  \param masked Unterdrückt Unterbrechungen bei Zählerablauf
	 */
	void set(uint32_t counter, uint8_t divide, uint8_t vector, bool periodic, bool masked = false);

	/*! \brief Setzt die LAPIC-Timer Interrupt Make
	 *  \param masked Unterdrückt Unterbrechungen bei Zählerablauf
	 *  \opt Für Tickless Kernel implementieren
	 */
	void setMasked(bool masked);
}  // namespace Timer
}  // namespace LAPIC
