#pragma once

/*! \file
 *
 *  \brief Enthält die Klasse Scheduler
 */

#include "thread/dispatcher.h"
#include "thread/thread.h"
#include "object/queue.h"
#include "machine/apicsystem.h"
#include "meeting/waitingroom.h"

/*! \brief Der Scheduler implementiert die Ablaufplanung und somit die Auswahl des nächsten Threads.
 *  \ingroup thread
 *
 *
 *  \note Der Scheduler verwaltet die Ready-Liste (ein privates Queue Objekt der
 *  Klasse), also die Liste der lauffähigen Threads. Die
 *  Liste wird von vorne nach hinten abgearbeitet. Dabei werden Threads, die
 *  neu im System sind oder den Prozessor abgeben, stets an das Ende der Liste
 *  angefügt.
 */
class Scheduler
	: public Dispatcher
{
	// Verhindere Kopien und Zuweisungen
	Scheduler(const Scheduler&)            = delete;
	Scheduler& operator=(const Scheduler&) = delete;
	void dispatchNext();
private:
	Queue<Thread, &Thread::queue_link> readylist;
public:
	/*! \brief Konstruktor
	 *
	 */
	Scheduler() {}
	/*! \brief Starten des Schedulings
	 *
	 *  Diese Methode setzt das Scheduling in Gang, indem der erste
	 *  Thread von der Ready-Liste entfernt und aktiviert wird. In MPStuBS muss
	 *  diese Methode auf jeder CPU einmal aufgerufen werden, um auf dem
	 *  jeweiligen Prozessor den ersten Thread einzulasten.
	 */
	void schedule();
	/*! \brief Anmelden eines Threads zum Scheduling
	 *
	 *  Mit dieser Methode wird der Thread that beim Scheduler angemeldet. Er
	 *  wird an das Ende der Ready-Liste angefügt.
	 *  \param that Thread, der angemeldet werden soll.
	 */
	void ready(Thread *that);
	/*! \brief Selbstbeenden des aktuellen Threads.
	 *
	 *  Hiermit kann sich ein Thread selbst beenden. Er wird nun nicht wieder
	 *  an das Ende der Ready-Liste angefügt. Statt dessen wird nur der erste
	 *  Thread von der Ready-Liste heruntergenommen und aktiviert.
	 */
	void exit();
	/*! \brief Beenden eines beliebigen Threads
	 *
	 *  Mit dieser Methode kann ein Thread einen anderen (\b that) beenden.
	 *  In OOStuBS genügt es einfach den Thread \b that von der Ready-Liste
	 *  zu entfernen. Er wird somit nie wieder vom Prozessor ausgeführt werden.
	 *  In MPStuBS ist die Implementierung etwas kniffliger, da der Thread
	 *  \b that auch während der Ausführung von kill auf anderen Prozessoren
	 *  laufen kann. Wird \b that nicht in der Ready-Liste gefunden, so muss
	 *  bei ihm vermerkt werden, dass der
	 *  Thread beendet werden soll. Dies muss in resume überprüft werden,
	 *  bevor ein Thread wieder in die Ready-Liste eingetragen wird.
	 *  \param that Thread, der beendet werden soll.
	 */
	void kill(Thread *that);
	/*! \brief Auslösen eines Threadwechsels
	 *
	 *  Hiermit kann ein Threadwechsel ausgelöst werden, ohne dass der
	 *  aufrufende Thread wissen muss, welche anderen Thread Objekte im System
	 *  existieren und welcher davon sinnvollerweise aktiviert werden sollte.
	 *  Diese Entscheidung trifft der Scheduler anhand der Einträge seiner
	 *  Ready-Liste. Der Scheduler soll den gerade laufenden Thread
	 *  an das Ende der Ready-Liste anfügen und den ersten Thread in der
	 *  Ready-Liste aktivieren.
	 */
	void resume();

	Thread* idlethread[CPU_MAX];
public:
	/*! \brief Gibt an, ob die Ready-Liste leer ist
	 *
	 *
	 *  \todo Methode implementieren
	 *
	 */
	bool isEmpty() {
		return readylist.first() == 0;
	}
	/*! \brief Setzt einen dedizierten Idle-Thread für die entsprechende CPU
	 *
	 *  \param cpuid ID der CPU, für die ein Idlethread registriert werden soll.
	 *  \param thread Zeiger auf das Thread-Objekt des Idlethreads.
	 *
	 *  \todo Methode implementieren
	 *
	 */
	void set_idle_thread(int cpuid, Thread* thread) {
		idlethread[cpuid] = thread;
	}
public:
	/*! \brief Mit dieser Methode kann sich der laufende Thread selbst
	 *  blockieren.
	 *
	 *  Er gibt also den Prozessor an den nächsten lauffähigen Thread ab ohne
	 *  sich selbst auf die Ready-Liste zu setzen.
	 *  Stattdessen wird er der Liste des übergebenen Waitingroom hinzugefügt
	 *  und es wird vermerkt, in welchem Waitingroom-Objekt er wartet.
	 *  \param waitingroom Wartezimmer, in welchem der Thread warten möchte.
	 *
	 *  \todo Methode implementieren
	 *
	 */
	void block(Waitingroom *waitingroom);

	/*! \brief Mit dieser Methode kann ein schlafender Thread geweckt werden.
	 *
	 *  Das heißt, dass er wieder auf die Ready-Liste gesetzt wird. Dabei sollte
	 *  auch markiert werden, dass der Thread nun auf kein Ereignis mehr
	 *  wartet.
	 *  \param customer Schlafender Thread, der geweckt werden soll.
	 *
	 *  \todo Methode implementieren
	 *
	 */
	void wakeup(Thread *customer);

};

extern Scheduler scheduler;
