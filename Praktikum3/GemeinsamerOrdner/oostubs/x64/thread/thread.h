/*! \file
 *  \brief Enthält die Klasse Thread
 */

/*! \defgroup thread Multithreading
 *  \brief Das Multithreading Subsystem
 *
 * Die Multithreading Gruppe enthält alle Elemente die Grundlegend für
 * das CPU Multiplexing benötigt werden. Ziel dieses Modules ist es
 * die Abstraktion des Threads an zu bieten, die für die Anwendung
 * eine virtualisierte CPU bereit stellt.
 */

#pragma once

#include "machine/context.h"

// Forwärtsdeklaration
class Waitingroom;
#include "object/queuelink.h"

/*! \brief Der Thread ist das Objekt der Ablaufplanung.
 *  \ingroup thread
 */
class Thread
{
public:
	/*! \brief Konstruktor.
	 *
	 *  Initialisiert den Kontext mit Hilfe von toc_settle
	 *  \param tos Top of Stack, also die höchste Adresse des Speicherbereiches,
	 *  der als Stack für diesen Thread fungieren soll.
	 */
	Thread(void* tos);

	/*! \brief Verkettungszeiger für Scheduler und Waitingroom */
	QueueLink<Thread> queue_link;
private:
	struct stack context;
public:
	/*! \brief Aktiviert den ersten Thread auf einem Prozessor.
	 *
	 *  Durch den Aufruf von Thread::go() wird der erste Thread auf dem
	 *  jeweiligen Prozessor gestartet. Alle weiteren Threadwechsel sollen
	 *  dann mit Hilfe der Methode Thread::resume() ausgeführt werden.
	 */
	void go();
	/*! \brief Wechsel von dem aktuell laufenden Thread zum nächsten.
	 *
	 *  Die aktuelle Belegung der nicht-flüchtigen Register wird in dem toc
	 *  Element gesichert und durch die Werte von next (dem toc Element des
	 *  nächsten Threads) ersetzt.
	 *  \param next Zeiger auf den nächsten Thread, der laufen soll.
	 */
	void resume(Thread *next);
	/*! \brief Methode, die als Thread ausgeführt werden soll.
	 *
	 *  Abgeleitete Klassen können durch Überschreiben dieser Methode den Code
	 *  angeben, der als Thread ausgeführt werden soll.
	 */
	virtual void action() = 0;
private:
	volatile bool kill_flag;
public:
	/*! \brief Setzen des Kill-Flags. Thread wechselt in den Zustand sterbend.
	 *  Wird nur für MPStuBS benötigt.
	 *
	 */
	void set_kill_flag() {
		kill_flag = true;
	}
	/*! \brief Zurücksetzen des Kill-Flags. Thread ist damit gestorben. Wird
	 *  nur für MPStuBS benötigt.
	 *
	 */
	void reset_kill_flag() {
		kill_flag = false;
	}
	/*! \brief Abfragen des Kill-Flags. Nachschauen, ob der Thread gerade
	 *  stirbt. Wird nur für MPStuBS benötigt.
	 *
	 */
	bool dying() {
		return kill_flag;
	}
public:
	Waitingroom *waitingroom;
	/*! \brief Hiermit kann abgefragt werden, in welchem Waitingroom Objekt der
	 *  Thread eingetragen ist.
	 *
	 *  Ein Rückgabewert von 0 soll bedeuten, dass der Thread auf überhaupt
	 *  kein Ereignis wartet, also entweder der aktuell laufende Thread ist,
	 *  auf der Ready-Liste des Schedulers steht, bereits terminiert ist oder
	 *  noch gar nicht beim Scheduler angemeldet wurde.
	 *  \return Zeiger auf das Waitingroom, in dem dieser Thread wartet, oder 0.
	 *
	 *  \todo Methode implementieren
	 */
	Waitingroom* waiting_in();

	/*! \brief Mit dieser Methode wird im Thread vermerkt, dass dieser derzeit
	 *  im Waitingroom \b w auf ein Ereignis wartet.
	 *  \param w Waitingroom, in dem dieser Thread wartet.
	 *
	 *  \todo Methode implementieren
	 */
	void waiting_in(Waitingroom* w);
};
