/*! \file
 *  \brief Enthält die Klasse Plugbox
 */

#pragma once

#include "guard/gate.h"

/*! \brief Abstraktion einer Interruptvektortabelle.
 *  \ingroup interrupts
 *
 *
 *  Damit kann man die Adresse der Behandlungsroutine für jeden Hardware-
 *  und Softwareinterrupt und jede Prozessorexception festlegen. Jede
 *  Unterbrechungsquelle wird durch ein Gate-Objekt repräsentiert. Diese
 *  liegen in einem Feld (256 Elemente). Der Index in dieses Feld ist dabei
 *  die Vektornummer.
 */
namespace Plugbox {
	/*! \brief Liste der verwendeten Interruptvektoren
	 *
	 *  In OOStuBS benötigen wir lediglich Vektornummern für den Timerbaustein
	 *  und die Tastatur, in MPStuBS noch weitere zur Synchronisation zwischen
	 *  CPUs.
	 */
	enum Vector {
		timer = 32,         ///< Interruptvektornummer für den Timerinterrupt.
		keyboard = 33,      ///< Interruptvektornummer für den Keyboardinterrupt.
		serial = 36,        ///< Interruptvektornummer für den Empfangsinterrupt der seriellen Schnittstelle.
		panic = 99,         ///<  Interruptvektornummer für Fehlkonfigurationen
		assassin = 100,     ///< Interruptvektornummer für den Assassin; nur in MPStuBS benötigt.
		wakeup = 101        ///< Interruptvektornummer zum Aufwecken von schlafenden CPUs; nur in MPStubs benötigt.
	};

	/*! \brief Einstöpseln einer Behandlungsroutine, die in Form eines
	 *  Gate-Objekts vorhanden sein muss.
	 *  \param vector Interruptvektor, für den die Behandlungsroutine registriert
	 *  werden soll.
	 *  \param gate Objekt mit Behandlungsroutine
	 */
	void assign(unsigned int vector, Gate *gate);
	/*! \brief Abfrage eines eingetragenen Gate Objekts.
	 *
	 *  \param vector Abzufragende Interruptvektornummer
	 *  \return Referenz auf das mit \b vector assoziierte Gate Objekt
	 */
	Gate* report(unsigned int vector);
};
