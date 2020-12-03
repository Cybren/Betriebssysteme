/*! \file
 *  \brief Enthält die Klasse zur Terminalausgabe über den seriellen Anschluss
 */

#pragma once

#include "object/o_stream.h"
#include "machine/serial.h"
#include "guard/gate.h"
#include "object/bbuffer.h"

/*! \brief Konsole (VT100) über serielle Schnittstelle.
 *  \ingroup io
 *
 *  Mit Hilfe dieser Klasse kann ein VT100-kompatibles Anzeigeterminal über
 *  die serielle Schnittstelle verbunden werden.
 *
 *  Eine Anwendung, welche komfortabel eine Verbindung erstellt (mit der
 *  Möglichkeit die Geschwindigkeit sowie Übertragungsformat einzustellen),
 *  ist `/proj/i4bs/tools/serial`.
 *
 *  Beispielaufruf für eine Verbindung mit dem Rechner `StuBS4` bei 19200 Baud
 *  mit 7 Datenbits, geradem Paritätsbit und 2 Stoppbits:
 *
 *      /proj/i4bs/tools/serial 4 19200 7e2
 *
 *
 *  Durch <a href="http://ascii-table.com/ansi-escape-sequences-vt-100.php">Escape-Codes</a>
 *  kann die Darstellung (u.a. Farbe und Position) beeinflusst werden.
 */

class Serial_Stream
	: public O_Stream, public Serial
	, public Gate
{
private:
	/*! \brief Mehrstellige Zahl als ASCII schreiben
	 */
	void write_number(int x);
protected:
	/*! \brief Buffer für im Prolog ausgelesene Zeichen */
	BBuffer<char, 16> buffer_prologue;

public:
	/*! \brief Attribtue
	 *
	 *  Mittels Attribute kann die Darstellung der Ausgabe beeinflusst werden.
	 */
	enum Attrib {
		RESET = 0,
		BRIGHT = 1,
		DIM = 2,
		UNDERSCORE = 4,
		BLINK = 5,
		REVERSE = 7,
		HIDDEN = 8,
	};

	/*! \brief Farbcodes
	 *
	 *  Für Vorder- und Hintergrund sind acht Farbattribute vorhanden.
	 *  Die tatsächliche Farbe wird von den anderen Attributen beeinflusst und
	 *  kann je nach Terminalemulator deutlich unterschiedlich aussehen.
	 */
	enum Color {
		BLACK = 0,
		RED = 1,
		GREEN = 2,
		YELLOW = 3,
		BLUE = 4,
		MAGENTA = 5,
		CYAN = 6,
		WHITE = 7
	};

	/*! \brief Konstruktor für die VT100 kompatible Konsole
	 *
	 * beinhaltet die serielle Verbindung
	 */
	Serial_Stream(Serial::comPort port = COM1, Serial::baudRate baudrate = BAUD_115200, Serial::dataBits databits = DATA_8BIT, Serial::stopBits stopbits = STOP_1BIT, Serial::parity parity = PARITY_NONE);

	/*! \brief Destruktor
	 */
	~Serial_Stream() {}

	/*! \brief Methode zur Ausgabe des Pufferinhalts der Basisklasse Stringbuffer.
	 *
	 *  Die Methode wird implizit aufgerufen, sobald der Puffer voll ist,
	 *  kann aber auch explizit verwendet werden, um eine Ausgabe zu erzwingen.
	 */
	void flush();

	/*! \brief Vordergrundfarbe (für alle nachfolgenden Ausgaben) ändern
	 *
	 *  \param c Farbe
	 */
	void setForeground(Serial_Stream::Color c);

	/*! \brief Hintergrundfarbe (für alle nachfolgenden Ausgaben) ändern
	 *
	 *  \param c Farbe
	 */
	void setBackground(Serial_Stream::Color c);

	/*! \brief Ausgabeformatierung (für alle nachfolgenden Ausgaben) ändern
	 *
	 *  \param a Ausgabettribut
	 */
	void setAttribute(Serial_Stream::Attrib a);

	/*! \brief Terminal zurücksetzen
	 *
	 * Bildschirm löschen, Cursor auf den Anfang setzen und Farben sowie
	 * Attribute auf den Standardwert zurücksetzen
	 */
	void reset();

	/*! \brief Setzen des Cursors in Spalte \p x und Zeile \p y.
	 *
	 *  \param x Spalte
	 *  \param y Zeile
	 */
	void setpos (int x, int y);

	/*! \brief Abfragen der Cursorposition
	 *
	 * Es ist möglich die aktuelle Cursorposition über einen speziellen Escape-
	 * code zu empfangen. Allerdings kann es abhängig von der Implementierung
	 * sein, dass aufgrund eines nicht angeschlossenen Terminals oder
	 * Datenübertragungsfehlers endlos gewartet wird.
	 *
	 *  \param x Spalte
	 *  \param y Zeile
	 *  \return \c true , falls die Position erfolgreich empfangen wurde
	 */
	bool getpos (int &x, int &y);

	/*! \brief Anzeige mehrerer Zeichen ab der aktuellen Cursorposition
	 *
	 *  Mit dieser Methode kann eine Zeichenkette \p string ausgegeben werden, wobei
	 *  an der aktuellen Position des Cursors begonnen wird. Da die Zeichenkette
	 *  anders als sonst bei C üblich keine Nullterminierung zu enthalten braucht,
	 *  wird der Parameter \p length benötigt, der angeben muss, aus wievielen Zeichen
	 *  string besteht.
	 *
	 *  \param string Auszugebende Zeichenkette
	 *  \param length Länge der Zeichenkette
	 */
	void print (char* string, int length);

	/*! \brief Initialisiere unterbrechungsgesteuerte serielle Verbindung
	 *
	 * Wir beschränken uns nur auf Interrupts beim Empfang, allerdings
	 * könnte auf die gleiche Art und Weise auch das Senden umgesetzt werden
	 * (Unterbrechung sobald Leitung frei ist).
	 */
	void listen();

	/*! \brief Enthält den Prolog der Unterbrechungsbehandlung.
	 *
	 * Das empfangene Zeichen wird in einen lokalen Puffer (z.B. BBuffer )
	 * gelegt und (bei Erfolg) ein Epilog angefordert.
	 *
	 *  \return Gibt `true` zurück, falls ein Epilog notwendig ist,
	 *          ansonsten `false`.
	 */
	bool prologue();

	/*! \brief Verarbeitung des empfangenen Zeichens
	 *
	 *  In dieser Methode soll das ein Empfangenes Zeichen einem eventuell
	 *  wartenden Thread signalisiert werden. Dazu wird ein Semaphor verwendet,
	 *  dessen Zähler angibt, wieviele Zeichen im Empfangspuffer zur
	 *  Abholung bereitstehen. Wenn ein Zeichen nicht in den Puffer geschrieben
	 *  werden konnte (da der Puffer voll ist) darf der Semaphor also auch
	 *  nicht hochgezählt werden.
	 *
	 *  \opt Methode anpassen
	 */
	void epilogue();

	/*! \brief Schnittstelle für Anwendungen, um Daten von der seriellen
	 *  Schnittstelle zu empfangen.
	 *
	 *  Diese Methode gibt das zuletzt empfangene Byte zurück.
	 *  Wenn noch kein unabgerufenes Byte empfangen wurde, wird der
	 *  aufrufende Anwendungsthread solange blockiert. Dies wird durch die
	 *  Verwendung eines Semaphors erreicht.
	 *
	 *  \opt Methode implementieren
	 *
	 * \return empfangenes Zeichen
	 */
	 char getchar();

private:
	/// Puffer für Zeichen abholbereite Zeichen
	BBuffer<char, 16> buffer_epilogue;

	// Semaphore
	Semaphore sem;

};
