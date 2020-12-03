#pragma once

/*! \file
 *  \brief Enthält die Klasse WakeUp
 */

#include "guard/gate.h"
#include "machine/plugbox.h"

/*! \brief Interruptbehandlungsobjekt, um in MPStuBS schlafende Prozessoren
 *  mit einem IPI zu wecken, falls neue Threads aktiv wurden. Diese Interruptbehandlung
 *  soll explizit keinen Epilog nach sich ziehen.
 *
 *  Nur in MPStuBS benötigt.
 */
class WakeUp
	: public Gate
{
public:
	/*! \brief Interruptbehandlung registrieren.
	 *
	 *
	 *  \todo Methode implementieren
	 *
	 */

	/*! \brief Interrupt soll Prozessor nur aufwecken und signalisieren, dass
	 *  neue Threads in der Bereitliste vorhanden sind.
	 *
	 *  \todo Methode implementieren
	 *
	 *
	 */
};
