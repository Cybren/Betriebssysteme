/*! \file
 *  \brief Enthält die Low-Level Unterbrechungsbehandlung für den Debugger
 */

#pragma once

#include "types.h"
#include "debug/assert.h"

/*! \brief Gesicherter Debug-Unterbrechungskontext (wie er auf dem Stack liegt)
 *
 * \see debug_handler
 * \see irq_context
 */
struct debug_context {
	uint64_t gs : 16;
	uint64_t : 0;
	uint64_t fs : 16;
	uint64_t : 0;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t vector;
	uint64_t error_code : 32;
	uint64_t : 0;
	uint64_t rip;
	uint64_t cs : 16;
	uint64_t : 0;
	uint64_t eflags : 32;
	uint64_t : 0;
	uint64_t rsp;
	uint64_t ss : 16;
	uint64_t : 0;
} __attribute__((packed));

assert_size(debug_context, 24 * 8);

/*! \brief Low-Level Debug-Interrupt-Behandlung.
 *
 *  Debug-Unterbrechungsbehandlungsroutine des Systems, analog zu \ref guardian .
 *  Aufruf dieser Funktion durch den Assembler Teil der Debug-
 *  Unterbrechungsbehandlung (\c dbg_irq_entry_* in der `debug/gbd/handler.asm`)
 *   -- diese Routinen müssen zuvor mittels GDB_Stub::install_handler
 *  installiert werden.
 *  Nach dem vorbereiten der Daten wird GDB_Stub::handle aufgerufen, welches
 *  mit dem Host über die serielle Schnittstelle kommuniziert.
 *
 *  \param context beinhaltet einen Pointer auf den eigenen Stack, über den auf
 *                 den Debug-Unterbrechungskontext zugegriffen werden kann.
 */
extern "C" void debug_handler(struct debug_context * context);
