#pragma once

/*! \file
 *  \brief Enthält die Klasse Bell.
 */

#include "meeting/waitingroom.h"
#include "object/queuelink.h"
class Bellringer;

/*! \brief Synchronisationsobjekt zum Schlafenlegen für eine bestimmte
 *  Zeitspanne
 *  \ingroup ipc
 *
 *
 *  Ein "Wecker" ist ein Synchronisationsobjekt, mit dem ein oder mehrere
 *  Threads sich für eine bestimmte Zeit schlafen legen können.
 */
class Bell
	: public Waitingroom
{
	// Verhindere Kopien und Zuweisungen
	Bell(const Bell&)            = delete;
	Bell& operator=(const Bell&) = delete;

private:

	/* Erlaube Klasse Bellringer Zugriff auf die interne Variable counter und
	 * bellringer_link. */
	friend class Bellringer;

	/*! \brief Verkettungszeiger
	 *
	 * Mittels dieses Verkettungszeigers kann der Bellringer mehrere Bells verwalten.
	 */
	QueueLink<Bell> bellringer_link;

	/*! \brief Interner Zähler zur Verwaltung der Bell-Objekte im Bellringer.
	 *
	 */
	unsigned int counter;
public:
	/*! \brief Konstruktor.
	 *
	 *  Der Wecker ist zunächst noch abgeschaltet.
	 *
	 *  \todo Konstruktor implementieren
	 *
	 */
	Bell()
		: counter(0)
	{}

	/*! \brief Läuten der Glocke
	 *
	 *  Wird von Bellringer aufgerufen, wenn die Wartezeit abgelaufen ist und
	 *  weckt den schlafenden Thread.
	 *
	 *  \todo Methode implementieren
	 *
	 */
	void ring();

	/*! \brief Abfragen der verbleibenden Zeit bis die Glocke geläutet wird
	 *
	 * Unter Verwendung der Methode Bellringer::residual()
	 *
	 * \return Zeit in ms bis die Glocke geläutet wird
	 *
	 *  \opt Methode implementieren
	 *
	 */
	unsigned int residual();

	/*! \brief Stellen des Weckers.
	 *
	 * Mit dieser Methode kann der Wecker auf die gewünschte Wartezeit gestellt
	 * werden.
	 * \param ms Zeit in Millisekunden, die zur Umrechnung an Bellringer::job()
	 * weitergegeben wird.
	 *
	 *  \opt Methode implementieren
	 *
	 */
	void set(unsigned int ms);

	/*! \brief Schlafen, bis der Wecker klingelt.
	 *
	 * Der Wecker muss zuvor mittels Bell::set() konfiguriert worden sein, und
	 * sollte noch nicht gekläutet worden sein (siehe Bell::residual() )
	 *
	 *  \opt Methode implementieren
	 *
	 */
	 void sleep();

	/*! \brief Temporäres Bell-Objekt erzeugen und Thread schlafen legen, bis
	 * der Wecker klingelt.
	 *  \param ms Zeit in Millisekunden
	 *
	 *  \todo Methode implementieren
	 *
	 */
	static void sleep(unsigned int ms);
};
