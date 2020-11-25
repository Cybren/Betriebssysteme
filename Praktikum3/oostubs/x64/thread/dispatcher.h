#pragma once

/*! \file
 *
 *  \brief Enthält die Klasse Dispatcher.
 */

#include "thread/thread.h"

/*! \brief Der Dispatcher lastet Threads ein und setzt damit die Entscheidungen der Ablaufplanung durch.
 *  \ingroup thread
 *
 *
 *  \note
 *  Der Dispatcher verwaltet den Life-Pointer, der die jeweils aktive Koroutine
 *  angibt und führt die eigentlichen Prozesswechsel durch. In der
 *  Uniprozessorvariante wird nur ein einziger Life-Pointer benötigt, da
 *  lediglich ein Prozess auf einmal aktiv sein kann. Für die
 *  Mehrprozessorvariante wird hingegen für jede CPU ein eigener Life-Pointer
 *  benötigt.
 *
 */
class Dispatcher
{
	// Verhindere Kopien und Zuweisungen
	Dispatcher(const Dispatcher&)            = delete;
	Dispatcher& operator=(const Dispatcher&) = delete;

protected:
	Thread* life;
	void setActive(Thread* c) {
		life = c;
	}
public:
	/*! \brief Konstruktor
	 *
	 *  Der Konstruktor initialisiert den oder die Life-Pointer mit Null,
	 *  um anzuzeigen, dass auf der jeweiligen CPU noch keine Koroutine bekannt
	 *  ist.
	 */
	Dispatcher()
		: life (0)
	{
	}

	/*! \brief Hiermit kann abgefragt werden, welche Koroutine gerade im Besitz
	 *  des aktuellen Prozessors ist.
	 *
	 */
	Thread* active() {
		return life;
	}

	/*! \brief Mit dieser Methode wird die Koroutine first im Life-Pointer des
	 *  aktuellen Prozessores vermerkt und gestartet.
	 *  \param first Erste auf dem aktuellen Prozessor zu startende Koroutine
	 *   im System.
	 */
	void go(Thread *first);
	/*! \brief Diese Methode setzt den Life-Pointer des aktuellen Prozessors auf
	 *  next und führt einen Koroutinenwechsel vom alten zum neuen Life-Pointer
	 *  durch.
	 *  \param next Nächste auszuführende Koroutine.
	 */
	void dispatch(Thread *next);
};
