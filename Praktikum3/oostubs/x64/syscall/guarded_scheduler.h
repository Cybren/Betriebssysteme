/*! \file
 *  \brief Enthält die Klasse Guarded_Scheduler
 */

#pragma once

#include "thread/thread.h"
#include "guard/secure.h"
#include "thread/scheduler.h"

/*! \brief Systemaufrufschnittstelle zum Scheduler
 *
 *  Der Guarded_Scheduler implementiert die Systemaufrufschnittstelle zum
 *  Scheduler. Die von ihm angebotenen Methoden werden direkt auf die Methoden
 *  des globalen Scheduler-Objekts abgebildet, nur dass ihre Ausführung jeweils
 *  mit Hilfe eines Objekts der Klasse Secure geschützt wird.
 */
class Guarded_Scheduler {
 public:
	/*! \brief Anmelden des Threads \b that beim Scheduler.
	 *  \param that Anzumeldender Thread.
	 */
	static void ready(Thread *that) {
		Secure section;
		scheduler.ready(that);
	}

	/*! \brief Beenden des aktuell laufenden Threads.
	 *
	 */
	static void exit() {
		Secure section;
		scheduler.exit();
	}

	/*! \brief Bricht einen anderen Thread ab.
	 *
	 *  In OOStuBS genügt es den Thread von der Ready-Liste zu entfernen.
	 */
	static void kill(Thread *that) {
		Secure section;
		scheduler.kill(that);
	}

	/*! \brief Auslösen eines Threadwechsels
	 *
	 */
	static void resume() {
		Secure section;
		scheduler.resume();
	}
};
