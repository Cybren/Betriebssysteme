/*! \file
 *  \brief Enthält die Klasse Gate
 */

#pragma once

#include "object/queuelink.h"
/*! \brief Klasse von Objekten, die in der Lage sind, Unterbrechungen zu behandeln.
 *  \ingroup interrupts
 *
 *
 * Die Klassen aller Objekte, die in der Plugbox eingetragen werden sollen,
 * müssen von Gate abgeleitet werden.
 *
 * Alle diese Objekte müssen nun die virtuelle Methode epilogue() definieren.
 * Für die virtuelle Methode epilogue() kann in Gate eine leere
 * Implementierung angegeben werden, so dass abgeleitete Treiberklassen nicht
 * unbedingt einen Epilog definieren müssen. Um feststellen, ob ein Gate-Objekt
 * schon in eine Epilog-Warteschlange einhängt wurde, soll ein entsprechendes
 * boolesches Attribut in die Klasse aufgenommen werden, das durch
 * Zugriffsfunktionen gesetzt und abgefragt werden kann.
 */
class Gate {
	// Verhindere Kopien und Zuweisungen
	Gate(const Gate&) = delete;
	Gate& operator=(const Gate&) = delete;
	/*! \brief  Gibt an, ob der Epilog auf seine Abarbeitung wartet.
	 */
	int epilogue_queued;

public:
	/*! \brief Konstruktor
	 *
	 */
	Gate() : epilogue_queued (0) {}

	/*! \brief  Verkettungszeiger für Epilog Queue
	 */
	QueueLink<Gate> queue_link;
	/*! \brief Destruktor
	 *
	 *  Klassen mit virtuellen Methoden sollten grundsätzlich einen virtuellen
	 *  Destruktor besitzen, der auch leer sein darf. Bei OO-Stubs beruhigt
	 *  das den Compiler, bei anderen Systemen garantiert das, dass die
	 *  Speicherfreigabe mit delete für Objekte der abgeleiteten Klassen
	 *  korrekt funktioniert.
	 */
	virtual ~Gate() {}
	/*! \brief Unterbrechungsbehandlungsroutine, die sofort nach Auftreten
	 *  der Unterbrechung asynchron zu anderen Kernaktivitäten ausgeführt wird.
	 *  \return gibt an, ob der entsprechende Epilog auszuführen ist.
	 */
	virtual bool prologue() = 0;
	/*! \brief Eine gegebenenfalls verzögerte, synchronisiert ausgeführte
	 *  Unterbrechungsbehandlung.
	 */
	virtual void epilogue() { }

	/*! \brief Setzt atomar ein Flag um zu markieren, dass sich das Objekt
	 *  gerade in einer Epilog-Warteschlange befindet.
	 *  \return Gibt \b false zurück, falls das Flag vorher bereits gesetzt war,
	 *  andernfalls \b true.
	 */
	bool set_queued() {
		int old = __sync_lock_test_and_set(&epilogue_queued, 1);
		return old == 0;
	};
	/*! \brief Setzt das in set_queued() gesetzte Flag zurück.
	 *
	 */
	void set_dequeued() {
		__sync_lock_release(&epilogue_queued);
	};
};
