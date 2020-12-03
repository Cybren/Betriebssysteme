/*! \file
 *  \brief Enthält die für Remotedebugging notwendigen Klassen und Funktionen
 */

/*! \defgroup debug Debugging Tools
 *    \brief Unterstützung bei der Entkäferung des Betriebssystems
*/

#pragma once

#include "handler.h"
#include "machine/serial.h"

/*! \brief Setze einen manuellen Debug Breakpoint.
 *
 *  Es wird ein Breakpoint-Trap ausgelöst
 */
inline void breakpoint(void){
	asm volatile("int $3;\r\n" ::: "memory");
}

/*! \brief GDB Remote Stub Klasse
 *  \ingroup debug
 *
 *  Diese Klasse ermöglicht ein entferntes Debuggen des Betriebssystems mit GDB
 *  auf echter Hardware (aber auch im Emulator), in dem sie einen Teil des
 *  <a href="https://sourceware.org/gdb/onlinedocs/gdb/Remote-Protocol.html">GDB Remote Serial Protocols (RSP)</a>
 *  implementiert, eigene Unterbrechungsbehandlungsroutinen für Traps
 *  installiert und über die serielle Schnittstelle kommuniziert.
 *
 *  Für den Einsatz muss GDB mit der selben Binärdatei wie das Betriebssystem
 *  auf der Hardware gestartet werden -- idealerweise im Quelltextverzeichnis,
 *  denn dann kann der Quelltext in die Ausgabe eingebettet werden.
 *  Zudem müssen die Einstellungen für die seriellen Übertragung sowohl auf
 *  der Hardware als auch im GDB identisch sein.
 *
 *  Beispiel:
 *
 *      ~> ssh faui04a
 *      faui04a:~> cd mpstubs
 *      faui04a:~/mpstubs> make netboot
 *      faui04a:~/mpstubs> gdb .build/system64
 *      GNU gdb (Debian 7.12-6) 7.12.0.20161007-git
 *      [...]
 *      Reading symbols from /proj/i4bs/student/uj66ojab/kernel...done.
 *      (gdb) set arch i386:x86-64

 *      (gdb) set serial baud 9600
 *      (gdb) target remote /dev/ttyBS1
 *      Remote debugging using /dev/ttyBS1
 *      main () at main.cc:87
 *
 *  \note GDB liegt bereits eine <a href="https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/stubs/i386-stub.c;hb=HEAD">i386-stub.c</a>
 *        bei, welche jedoch hässlich, schlecht gewartet und nicht sonderlich
 *        gut in unser objektorientiertes Betriebssystem integrierbar ist.
 *        Deshalb verwenden wir eine überarbeitete Version von <a href="https://github.com/mborgerson/gdbstub">Matt Borgersons gdbstub</a>
 *        (veröffentlicht 2016 unter der GPL v2 Lizenz).
 */
class GDB_Stub
	: public Serial
{
	// Verhindere Kopien und Zuweisungen
	GDB_Stub(const GDB_Stub&)            = delete;
	GDB_Stub& operator=(const GDB_Stub&) = delete;

	/*! \brief zusätzliche Debug-Ausgaben auf dem Zielrechner
	 * (zum debuggen des Debuggerstubs)
	 */
	const bool debug;

public:
	/*! \brief Konstruktor
	 *
	 * Konfiguriert die serielle Schnittstelle (als \c 8N1 ) sowie
	 * Unterbrechungsbehandlungen
	 *
	 * \param wait        Warte nach der Konfiguration auf eine
	 *                    GDB-Verbindung
	 * \param debugOutput Debugge den GDB-Stub durch Bildschirmausgaben der
	 *                    Kommunikation (hilfreich bei Erweiterung des RSP)
	 * \param port        COM-Port für die serielle Verbindung
	 * \param baudrate    Baud Rate, Standard bei GDB ist \c 9600
	 *                    (kann aber zu Engpässen führen)
	 */
	GDB_Stub(bool wait = false, bool debugOutput = false, Serial::comPort port = COM1, Serial::baudRate baudrate = BAUD_9600);

protected:
	/*! \brief Behandlung eines Traps
	 *
	 * Diese Funktion wird nach der Speicherung des aktuellen CPU Zustandes von
	 * der generischen Debug-Unterbrechungsbehandlungsroutine aufgerufen.
	 * Sie ist das Herzstück und übernimmt die Kommunikation mit dem GDB Host.
	 */
	void handle(void);

	/*! \brief Erlaube der generischen Unterbrechungsbehandlung Zugriff auf die
	 * geschützten Methoden dieser Klasse
	 */
	friend void debug_handler(struct debug_context * context);

	/*! \brief Sende eine Zeichenkette über die serielle Schnittstelle
	 *
	 * \param buf Zeiger auf den Puffer
	 * \param len Größe des Puffers
	 * \retval  0 falls erfolgreich,
	 * \retval -1 falls kein/nicht alle Bytes gesendet werden konnten
	 */
	int writeString(const char *buf, size_t len);

	/*! \brief Empfange eine Zeichenkette über die serielle Schnittstelle
	 *
	 * \param buf Zeiger auf den Puffer
	 * \param buf_len Größe des Puffers
	 * \param len Anzahl der zu empfangenden Bytes
	 * \retval  0 falls erfolgreich,
	 * \retval -1 falls kein/nicht alle Bytes empfangen werden konnten
	 */
	int readString(char *buf, size_t buf_len, size_t len);

	/*! \brief Sende ein Datenpaket
	 *
	 * Datenpakete haben die Form
	 * \code{.gdb}
	 *    $<Daten>#<Prüfsumme>
	 * \endcode
	 *
	 * \param pkt Zeiger auf den Paketpuffer
	 * \param pkt_len Größe des Paketpuffers
	 * \retval  0 falls Paket erfolgreich gesendet und bestätigt wurde,
	 * \retval  1 falls Paket zwar gesendet aber nicht bestätigt wurde,
	 * \retval -1 andernfalls
	 */
	int send_packet(const char *pkt, size_t pkt_len);

	/*! \brief Empfange ein Datenpaket
	 *
	 * Bedingt eine fehlerfreie Verbindung zur Übertragung von 7-Bit ANSI Zeichen
	 *
	 * \param pkt_buf Zeiger auf den Paketpuffer
	 * \param pkt_buf_len Größe des Paketpuffers
	 * \param pkt_len Größe des zu empfangenden Pakets
	 * \retval  0 falls Paket erfolgreich empfangen wurde,
	 * \retval -1 andernfalls
	 */
	int recv_packet(char *pkt_buf, size_t pkt_buf_len, size_t *pkt_len);

	/*! \brief Berechne die Prüfsumme
	 *
	 * \param buf Zeiger auf den Puffer
	 * \param len Größe des Puffers
	 * \return 8-Bit Prüfsumme des Puffers
	 */
	int checksum(const char *buf, size_t len);

	/*! \brief Empfange ein Bestätigung für ein Paket
	 *
	 * \retval  0 eine [positive] Bestätigung (ACK, \c + ) wurde empfangen
	 * \retval  1 eine negative Bestätigung (NACK, \c - ) wurde empfangen
	 * \retval -1 andernfalls
	 */
	int recv_ack(void);

 	/*! \brief Erstelle und sende ein \c OK Paket
	 *
	 * \return Statuscode von send_packet
	 */
	int send_ok_packet();

	/*! \brief Erstelle und sende ein Signalpaket
	 *
	 * beinhaltet aktuellen Interruptvektor (\c S Vektor)
	 *
	 * \return Statuscode von send_packet
	 */
	int send_signal_packet(char *buf, size_t buf_len, char signal);

	/*! \brief Erstelle und sende ein Fehlerpaket (\c E Errorcode)
	 *
	 * \return Statuscode von send_packet
	 */
	int send_error_packet(char *buf, size_t buf_len, char error);

	/*! \brief Lese den Speicherinhalt einer Adresse in den Puffer
	 *
	 * \param buf Zeiger auf den Puffer
	 * \param buf_len Größe des puffers
	 * \param addr Startadresse des zu lesenden Speichers
	 * \param len Größe des zu lesenden Speichers
	 * \param hex Speichere als Hexadezimal (\c true) oder binär (\c false) in den Puffer
	 * \return Anzahl der gelsenen Bytes oder \c -1 falls der Puffer zu klein
	 */
	int mem_read(char *buf, size_t buf_len, uintptr_t addr, size_t len, bool hex);

	/*! \brief Schreibe den Pufferinhalt an eine Adresse
	 *
	 * \param buf Zeiger auf den Puffer
	 * \param buf_len Größe des puffers
	 * \param addr Startadresse des zu schreibenden Speichers
	 * \param len Größe des zu schreibenden Speichers
	 * \param hex Interpretiere den Pufferinhalt als Hexadezimal (\c true) oder binär (\c false)
	 * \return Anzahl der geschrieben Bytes oder \c -1 falls der Puffer zu gross
	 */
	int mem_write(const char *buf, size_t buf_len, uintptr_t addr, size_t len, bool hex);

	/*! \brief Setze das Programm am aktuellen Instruktionszeiger fort
	 */
	void sys_continue(void);

	/*! \brief Springe zur nächsten Instruktion
	 */
	void sys_step(void);
};
