/*! \file
 *  \brief Enthält Debugmacros, um Debugausgaben auf einen eigenen
 *  Screen umzuleiten.
 *
 *  Für den Uniprozessorfall reicht es ein CGA_Stream Objekt für Debugausgaben
 *  (\b dout) anzulegen.
 */

#pragma once

/*!
 *  \def DBG_VERBOSE
 *  \brief Stream, welche nur im Verbose-Modus im Debugfenster angezeigt wird
 *  \note Sofern eine serielle Konsole implementiert wurde, kann diese (durch
 *        Änderung des Makros) als Ausgabe verwendet werden - dadurch wird
 *        die ggf. sehr umfangreiche Ausgaben besser lesbar (Stichwort Scrollen)
 */
#ifdef VERBOSE
// Sofern VERBOSE gesetzt ist, leite alles an DBG (unten definiert) weiter
#define DBG_VERBOSE DBG
#else
// Andernfalls wird alles an den Null_Stream gesendet, der alles einfach verwirft
#define DBG_VERBOSE nullstream
// Wir müssen noch den Null_Stream inkludieren
#include "debug/null_stream.h"
#endif

/*!
 *  \def DBG
 *  \brief Ausgabestrom, welcher im Debugfenster angezeigt wird
 *  \note Es muss ein CGA_Stream Objekt für Debugausgaben (\b dout)
 *        angelegt werden.
 */
#define DBG dout

#include "device/cga_stream.h"

/*! \brief Debugausgabefenster
 *  Zeigt Ausgaben zur Fehlersuche wie
 *      `DBG << "Var = " << var << endl`
 *  in einem separaten Debugfenster an.
 *  Dieses sollte im Idealfall überschneidungsfrei unterhalb des normalen
 *  Ausgabefensters liegen und mindestens 4 Zeilen lang sein
 */
extern CGA_Stream dout;
