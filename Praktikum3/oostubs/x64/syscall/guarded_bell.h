#pragma once

/*! \file
 *  \brief Enthält die Klasse Guarded_Bell
 */

#include "meeting/bell.h"
#include "guard/secure.h"

/*! \brief Schnittstelle von Anwendungsthreads zu Bell-Objekten.
 *
 *  Die Klasse Guarded_Bell implementiert die Systemaufrufschnittstelle zur
 *  Bell Klasse. Die von Guarded_Bell angebotenen Methoden werden direkt auf
 *  die Methoden der Basisklasse abgebildet, nur dass ihre Ausführung jeweils
 *  mit Hilfe eines Objekts der Klasse Secure geschützt wird.
 */
class Guarded_Bell
	: public Bell
{
	// Verhindere Kopien und Zuweisungen
	Guarded_Bell(const Guarded_Bell&)            = delete;
	Guarded_Bell& operator=(const Guarded_Bell&) = delete;

public:
	Guarded_Bell() {}

	/*! \copydoc Bell::sleep()
	 * \note Diese Methode entspricht der gleichnamigen Methode der
	 *  Basisklasse Bell, nur dass sie mit Hilfe eines Secure Objekts
	 *  geschützt wird.
	 *
	 *  \todo Methode implementieren
	 */
	static void sleep(unsigned int ms){
		(void)ms;
		Secure section;
		Bell::sleep(ms);
	};

	/*! \copydoc Bell::residual()
	 * \note Diese Methode entspricht der gleichnamigen Methode der
	 *  Basisklasse Bell, nur dass sie mit Hilfe eines Secure Objekts
	 *  geschützt wird.
	 *
	 *  \opt Methode implementieren
	 */
	unsigned int residual(){
		Secure section;
		return Bell::residual();
	};

	/*! \copydoc Bell::set()
	 * \note Diese Methode entspricht der gleichnamigen Methode der
	 *  Basisklasse Bell, nur dass sie mit Hilfe eines Secure Objekts
	 *  geschützt wird.
	 *
	 *  \opt Methode implementieren
	 */
	void set(unsigned int ms){
		(void)ms;
		Secure section;
		Bell::set(ms);
	}

	/*! \copydoc Bell::sleep()
	 * \note Diese Methode entspricht der gleichnamigen Methode der
	 *  Basisklasse Bell, nur dass sie mit Hilfe eines Secure Objekts
	 *  geschützt wird.
	 *
	 *  \opt Methode implementieren
	 */
	 void sleep(){
 		Secure section;
 		Bell::sleep();
 	}
};
