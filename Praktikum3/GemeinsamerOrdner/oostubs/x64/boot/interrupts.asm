;Unterbrechungsbehandlungen
; Unsere eigentliche Unterbrechungsbehandlung ist in der C-Funktion "guardian"
[EXTERN guardian]

[SECTION .text]

; Einsprungsfunktion für die Interruptbehandlung eines jeden Vektors

; Die Interruptbehandlung muss in Assembler gestartet werden, um die
; caller-saved Register (vgl Aufrufkonventionen) zu sichern, bevor die
; eigentliche C-Funktion angesprungen werden kann.
; Wir verwenden hierfür ein Makro, dass als ersten Parameter die Interruptnummer
; erhält. Als zweiter Parameter wird ein boolsches Flag übergebn, das angibt,
; ob bei dem entsprechenden Trap ein Fehlercode auf dem Stack gelegt wird.

;; Aufruf durch: IRQ <irq-num> <error-code?>
%macro IRQ 2
align 8
irq_handler_%1:
	%if %2 == 0
		; Der Interrupt hat keinen Errorcode - deswegen legen wir 0 auf den
		; Stack, damit das Stacklayout in beiden Fällen identisch ist
		push 0
	%endif
	; Sichere die caller-saved Register (= Teil des CPU Kontexts)
	push rax
	push rcx
	push rdx
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11

	; Zeichenkettenrichtung zurücksetzen
	cld

	; Die Interrupt-Nummer ist das erste Argument
	mov rdi, %1

	; Einen Pointer auf den CPU Kontext als zweites Argument pushen
	mov rsi, rsp

	; Aufruf der C-Funktion
	call guardian

	; Wiederherstellen der caller-saved Register
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rax

	; Errorcode verwerfen
	add rsp, 8

	; Rückkehr vom Interrupt
	iretq
%endmacro

; Für die Traps werden zum Teil Fehlercodes auf den Stack gelegt.
; Deshalb wird hier das Makro für die Spezialfälle händisch aufgerufen.
IRQ 0, 0
IRQ 1, 0
IRQ 2, 0
IRQ 3, 0
IRQ 4, 0
IRQ 5, 0
IRQ 6, 0
IRQ 7, 0
IRQ 8, 1
IRQ 9, 0
IRQ 10, 1
IRQ 11, 1
IRQ 12, 1
IRQ 13, 1
IRQ 14, 1
IRQ 15, 0
IRQ 16, 0
IRQ 17, 1

; Für alle weiteren Interrupts (18 - 255) gibt es keinen Fehlercode,
; das Makro wird in einer Schleife aufgerufen
%assign i 18
%rep 238
IRQ i, 0
%assign i i+1
%endrep

[SECTION .data]

; Erstelle eine Tabelle (Array) mit den Adressen der spezifischen Unterbrechungsbehandlungen
[GLOBAL irq_handlers]

irq_handlers:
%macro irq_addr 1
	dq irq_handler_%1
%endmacro
%assign i 0
%rep 256
	irq_addr i
	%assign i i+1
%endrep
