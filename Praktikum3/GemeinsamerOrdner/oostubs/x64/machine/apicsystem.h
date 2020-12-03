/*! \file
 *  \brief Enthält die Klasse APICSystem, die zum Hochfahren des
 *  Systems benötigt wird.
 */

#pragma once
#include <types.h>
/// maximum number of CPUs
const unsigned CPU_MAX = 8;

namespace System {
	enum Device {
		timer = 0,
		keyboard = 1,
		com1 = 4,
		com2 = 3,
		com3 = 4,
		com4 = 3,
		floppy = 6,
		lpt1 = 7,
		rtc = 8,
		ps2mouse = 12,
		ide1 = 14,
		ide2 = 15
	};

	/*! \brief Führt Systemerkennung aus.
	 *
	 *  Diese Funktion erkennt ein eventuell vorhandenes Mehrprozessorsystem.
	 *  Nach erfolgter Erkennung kann der Systemtyp mit Hilfe der Methode
	 *  getSystemType() abgefragt werden.
	 */
	bool init();

	/*! \brief Startet alle Applikationsprozessoren.
	 */
	void bootAPs();

	/*! \brief Liefert die CPUID der aktuellen CPU.
	 *
	 *  \return CPUID der aktuellen CPU.
	 */
	int getCPUID();

	/*! \brief Liefert die während des Bootvorgangs ausgelesene Addresse des IOAPICs
	 *
	 */
	void* getIOAPICAddress();

	/*! \brief Liefert die während des Bootvorgangs ausgelesene ID des IOAPICs
	 *
	 */
	uint8_t getIOAPICID();

	/*! \brief Liefert die Nummer des Pins, an dem das Gerät device hängt
	 *
	 */
	uint8_t getIOAPICSlot(System::Device device);

	/*! \brief Gibt die Anzahl der CPUs zurück
	 *
	 */
	unsigned getNumberOfCPUs();

	/*! \brief Gibt die Anzahl der erfolgreich gebooteten CPUs zurück
	 *
	 */
	unsigned getNumberOfOnlineCPUs();

	/*! \brief Gibt an ob die gegegebene CPU erfolgreich gebootetet ist
	 * \param cpu ID der abzufragenden CPU
	 * \return `true` falls erfolgreich gebootet
	 */
	bool isOnlineCPU(uint8_t cpu);

	void setupThisProcessor();

	/*! \brief Liefert die logische ID des LAPICs der übergebenen \a cpu.
	 *
	 *  Die logische ID der LAPICs wird von uns beim Boot-Vorgang so
	 *  initialisiert, dass genau ein Bit pro CPU gesetzt ist. Dabei ist in der
	 *  logischen ID des LAPICs von CPU 0 Bit 0 gesetzt, in der ID des LAPICs
	 *  von CPU 1 Bit 1, usw.
	 *
	 *  \param cpu Die abzufragende CPU
	 */
	uint8_t getLogicalLAPICID(uint8_t cpu);

}  // namespace System
