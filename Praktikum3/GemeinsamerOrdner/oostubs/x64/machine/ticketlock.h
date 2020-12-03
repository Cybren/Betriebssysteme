/*! \file
 *  \brief Enthält die Klasse Ticketlock
 */

#pragma once

#include "cpu.h"

/*! \brief Mit Hilfe eines Ticketlocks kann man Codeabschnitte serialisieren,
 *  die echt nebenläufig auf mehreren CPUs laufen.
 *
 *  Die Synchronisation läuft dabei über eine Sperr- und eine Ticket-Variable.
 *  Sobald jemand den kritischen Abschnitt betreten will, holt er sich sein
 *  Ticket durch atomares Erhöhen der Ticket-Variable und wartet darauf, dass
 *  die Sperr-Variable seinen Ticket-Zählerstand erreicht. Verlässt er den
 *  kritischen Abschnitt, so erhöht er die Sperr-Variable und der nächste ist
 *  an der Reihe.
 *
 *  <a href="http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html">Eintrag im GCC Manual über Atomic Builtins</a>
 */
class Ticketlock
{
private:
	Ticketlock(const Ticketlock& copy); //verhindert Kopieren

public:

	/*! \brief Konstruktor; Initialisierung des Ticketlocks.
	 */
	Ticketlock()
	{}

	/*! \brief Betritt den gesperrten Abschnitt. Ist dieser besetzt, so wird
	 *  solange aktiv gewartet, bis er betreten werden kann.
	 */
	void lock() {
	}

	/*! \brief Gibt den gesperrten Abschnitt wieder frei.
	 */
	void unlock() {
	}
};
