/*! \file
 *  \brief Enthält die Klasse Application
 */

#pragma once

#include "types.h"
#include "thread/thread.h"

/*! \brief Die Klasse Application definiert eine Anwendung für OO-Stubs.
 */
class Application
	: public Thread
{
	// Verhindere Kopien und Zuweisungen
	Application(const Application&)            = delete;
	Application& operator=(const Application&) = delete;

private:
	int id;
	uint8_t appstack[4096];
public:
	/*! \brief Konstruktor
	 *
	 * \param i Instanz-ID
	 */
	Application(int i = 0) : Thread(appstack + 4096), id(i) {}

	/*! \brief Setzt eine Instanz-ID
	 *
	 */
	void setID(int i) {
		id = i;
	}

	/*! \brief Enthält den Code der Anwendung
	 *
	 */
	void action ();
};
