/*! \file
 *  \brief Diese Datei enthält die Klasse Keyboard
 */

#pragma once

#include "machine/keyctrl.h"
#include "guard/gate.h"
#include "machine/key.h"
#include "meeting/semaphore.h"

/*! \brief Die Klasse Keyboard stellt die Abstraktion der Tastatur dar.
 *  \ingroup io
 *
 *  Sie sorgt für die korrekte Initialisierung und vor allem für die
 *  Unterbrechungsbehandlung. Später wird Keyboard auch die Tastaturabfrage
 *  durch die Anwendung ermöglichen.
 */
class Keyboard
	: public Gate, public Keyboard_Controller
{
	// Verhindere Kopien und Zuweisungen
	Keyboard(const Keyboard&)            = delete;
	Keyboard& operator=(const Keyboard&) = delete;

private:
	Key lastkey;
	Key currentkey;
	Semaphore sema;
public:
	/*! \brief Konstruktor
	 *
	 */
	/*! \brief Konstruktor; Initialisiert Semaphore mit 0
	 *
	 *  \todo Konstruktor anpassen
	 *
	 *
	 */
	Keyboard() : sema(0) {}

	/*! \brief 'Anstöpseln' der Tastatur.
	 *
	 *  Initialisierung der Tastatur und Aktivieren der spezifischen
	 *  Interruptbehandlung. Dazu muss sich das Keyboard-Objekt bei der Plugbox
	 *  plugbox anmelden und dem IOAPIC Objekt ioapic mitteilen, dass
	 *  Unterbrechungen der Tastatur fortan dem Prozessor gemeldet werden
	 *  sollen -- bei levelgesteuerten Unterbrechungen (sonst könnte eine
	 *  weitere CPU einen IRQ erhalten bevor die erste Abarbeitung abgeschlossen
	 *  ist).
	 */
	void plugin();

	/*! \brief Prolog der Tastaturunterbrechung.
	*
	*  Mit dieser Methode wird auf die Unterbrechungen durch die Tastatur
	*  reagiert. Da bei jedem Tastendruck mehrere Unterbrechungen ausgelöst
	*  werden, ist nicht in jedem Fall mit einem verwertbaren ASCII Code zu
	*  rechnen. Nur wenn die Methode Keyboard_Controller::key_hit einen
	*  gültigen Key liefert, konnte ein vollständiger Scancode ermittelt
	*  werden und soll die Ausgabe des Zeichens angefordert werden.
	*
	*  Weiterhin muss sichergestellt werden, dass ein gültiger Key nicht durch
	*  einen nachfolgenden Prolog überschrieben wird, bevor er ausgegeben wurde.
	*/
	bool prologue();

	/*! \brief Epilog der Tastaturunterbrechung.
	 *
	 *  In dieser Methode soll das Auftreten des Tastendrucks einem eventuell
	 *  wartenden Thread signalisiert werden. Dazu wird ein Semaphor verwendet,
	 *  dessen Zähler angibt, wieviele Tastaturcodes im Keyboard Puffer zur
	 *  Abholung bereitstehen. Wenn ein Puffereintrag überschrieben werden muss,
	 *  ohne dass der alte Wert von einem Anwendungsthread abgefragt wurde,
	 *  darf der Semaphor also nicht hochgezählt werden.
	 *
	 *  \todo Methode anpassen
	 */
	void epilogue();

	/*! \brief Schnittstelle für Anwendungen, um Zeichen von der Tastatur zu
	 *  lesen.
	 *
	 *  Diese Methode liefert die zuletzt gedrückte Taste (als Objekt der
	 *  Klasse Key) zurück. Wenn gerade keine Taste gedrückt wurde, wird der
	 *  aufrufende Anwendungsthread solange blockiert. Dies wird durch die
	 *  Verwendung eines Semaphors erreicht. Sobald der Tastencode abgeholt
	 *  worden ist, sollte geeignet vermerkt werden, dass der verwendete
	 *  Puffer nun für den nächsten Tastencode zur Verfügung steht.
	 *
	 *  \todo Methode implementieren
	 *
	 */
	Key getkey();
};
