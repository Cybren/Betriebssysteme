;
; Copyright (C) 2016  Matt Borgerson
; Copyright (C) 2019  Bernhard Heinloth
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along
; with this program; if not, write to the Free Software Foundation, Inc.,
; 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
;

%define NUM_HANDLERS 40

[SECTION .data]

; Erstelle eine Tabelle (Array) mit den Adressen der spezifischen Unterbrechungsbehandlungen
[GLOBAL dbg_irq_handlers]

dbg_irq_handlers:
%macro dbg_irq_addr 1
	dq dbg_irq_handler_%1
%endmacro
%assign i 0
%rep NUM_HANDLERS
	dbg_irq_addr i
	%assign i i+1
%endrep

; Speichere die Anzahl der Unterbrechungsbehandlungsroutinen in einer Variable
[GLOBAL dbg_irq_handlers_num]

dbg_irq_handlers_num:
	dd NUM_HANDLERS

[SECTION .text]
; Die generische Unterbrechungsbehandlung ist (analog zum guardian) eine C Funktion
[EXTERN debug_handler]

; Debug-Unterbrechungsbehandlungen
; Analog zu den normalen Unterbrechungsbehandlungen in der startup.asm,
; jedoch müssen hier mehr (alle) Register gesichert werden.
;; DBGIRQ <irq-num> <error-code?>
%macro DBGIRQ 2
align 8
dbg_irq_handler_%1:
	; CPU sichert
	; - FLAGS
	; - CS
	; - IP
	; - ERROR CODE, falls vorhanden (bei Vektor 8, 10-14 und 17)
	%if %2 == 0
		; Falls es keinen ERROR CODE gibt, setzen wir einen Platzhalter
		; (damit der Stack identisch aufgebaut ist)
		push 0
	%endif

	; - VECTOR (Interrupt-Nummer)
	push %1

	; sichern aller General-Purpose Register:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	; Segment-Register
	push fs
	push gs

	; Kontext (Stackpointer als Zeiger) als erstes Argument
	mov rdi, rsp

	; statt guardian rufen wir unseren debug-Interrupthandler auf
	call debug_handler

	; CPU Kontext vom Stack nehmen
	pop gs
	pop fs
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	add rsp, 16 ; wegen ERROR CODE und VECTOR
	iretq
%endmacro

; Macro für jeden Trap aufrufen (kann aber auch händisch gemacht werden, wie in der startup.asm)
%assign i 0
%rep NUM_HANDLERS
	%if (i == 8) || ((i >= 10) && (i <= 14)) || (i == 17)
		; mit ERROR CODE
		DBGIRQ i, 1
	%else
		; ohne ERROR CODE
		DBGIRQ i, 0
	%endif
	%assign i i+1
%endrep
