#pragma once

/*! \defgroup interrupts Unterbrechungsbehandlung
 *  \brief Das Interrupt Subsystem
 *
 * Das Interrupt Subsystem von OO/MPStubBS beinhaltet alle
 * Funktionalität um Unterbrechungen von der Hardware anzunehmen und
 * diese abzuarbeiten. In späteren Übungen werden dann die Interrupts
 * mit der Anwendungs-aktivierten Kernausführung (Systemaufrufe)
 * synchronisiert. Der Eintrittspunkt für das Interrupt Subsystem ist die
 * Funktion \ref guardian.
 */

/*! \brief Gesicherter Unterbrechungskontext falls ein Fehlercode vorhanden ist.
 *
 *  Bei Unterbrechungen sichert zuerst die Hardware, und dann der
 *  Assemblerteil der Unterbrechungbehandlung den Kontext der CPU.
 *  Die irq_entry-Funktionen in boot/interrupts.inc legen dabei die flüchtigen
 *  Register auf den Stapel, bevor die guardian() Funktion betreten wird.
 *  Dieser Kontext beinhaltet alles um den Zustand geordnet wieder
 *  herstellen zu können, wenn die Unterbrechung behandelt ist. Diese
 *  Struktur kann für Unterbrechungen verwendet werden, falls ein
 *  Fehlercode von der Hardware auf den Stack gelegt wurde.
 *
 *  \see guardian
 */
struct irq_context {
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rax;
	uint64_t error_code : 32; // Fehlercode (falls vorhanden)
	uint64_t            : 0;  // Auf 64bit ausrichten (wegen 32bit Errorcode)
	uint64_t rip; // Intruktionszeiger
	uint64_t cs : 16; // Code Segmnet
	uint64_t    : 0;  // Auf 64bit ausrichten (wegen 16 bit Code Segment)
	uint64_t rflags; // Flags
	uint64_t rsp; // Stackpointer (vor Interrupt)
	uint64_t ss : 16; // Stacksegment
	uint64_t    : 0;  // Auf 64bit ausrichten (wegen 16 bit Stacksegment)
} __attribute__((packed));

/*! \brief Low-Level Interrupt-Behandlung.
 *  \ingroup interrupts
 *
 *
 *  Zentrale Unterbrechungsbehandlungsroutine des Systems. Diese
 *  Funktion wird von dem Assembler Teil der Unterbrechungsbehandlung
 *  aufgerufen. Beim Betreten der Funktion sind die Unterbrechungen
 *  auf dieser CPU gesperrt.
 *
 *  \param vector gibt die Nummer des aufgetretenen Interrupts an.
 *  \param context beinhaltet einen Pointer auf den eigenen Stack, über
 *                 den auf den Unterbrechungskontext zugegriffen werden kann.
 */
extern "C" void guardian(uint32_t vector, irq_context *context);
