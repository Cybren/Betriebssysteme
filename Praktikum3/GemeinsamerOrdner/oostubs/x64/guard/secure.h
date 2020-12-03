/*! \file
 *  \brief Diese Datei enthält die Klasse Secure.
 */

#pragma once

/* INCLUDES */

#include "guard/guard.h"

/*! \brief Die Klasse Secure dient dem bequemen Schutz kritischer Abschnitte.
 *  \ingroup interrupts
 *
 *
 *  Dabei wird die Tatsache ausgenutzt, dass der C++ Compiler für jedes Objekt
 *  automatisch Konstruktor- und Destruktoraufrufe in den Code einbaut und dass
 *  ein Objekt seine Gültigkeit verliert, sobald der Bereich (Scope), in dem es
 *  deklariert wurde, verlassen wird.
 *
 *  Wenn im Konstruktor von Secure also ein kritischer Abschnitt betreten und
 *  im Destruktor wieder verlassen wird, kann die Markierung kritischer
 *  Codebereiche ganz einfach folgendermaßen erfolgen:
 *
 *  \verbatim
	// unkritisch
	...
	{ Secure section;
	   // hier kommen die kritischen Anweisungen
	   ...
	}
	// Ende des kritischen Abschnitts
	\endverbatim
 */
class Secure
{
	// Verhindere Kopien und Zuweisungen
	Secure(const Secure&) = delete;
	Secure& operator=(const Secure&) = delete;
public:
	/*! \brief Konstruktor; Setzt die Sperrvariable
	 */
	Secure() {
		Guard::enter();
	}

	/*! \brief Destruktor; Gibt die Sperrvariable frei
	 */
	~Secure() {
		Guard::leave();
	}

};
