; Die folgenden Angaben belegen kernel_offset mit dem Wert 0 und user_offset
; mit 8. Dies entspricht den Abständen der Elemente kernel_offset
; und user_offset der Struktur stack zum Anfang der Struktur (in Bytes).
; Die Reihenfolge der Registerbezeichnungen muss unbedingt mit der von
; der Struktur stack übereinstimmen.

[ABSOLUTE 0]
kernel_offset:	resq 1
user_offset:	resq 1

[SECTION .text]

; IMPLEMENTIERUNG DER FUNKTIONEN

[GLOBAL context_switch]
; CONTEXT_SWITCH : Threadumschaltung. Der aktuelle Registersatz wird
;                  gesichert und der Registersatz des neuen Threadkontexts
;                  wird in den Prozessor eingelesen.
;
; C Prototyp: void context_switch (struct stack * current, struct stack * next);

align 8
context_switch:
	; nicht-fluechtige Register speichern
	push r15
	push r14
	push r13
	push r12
	push rbp
	push rbx
	; alten Stackpointer sichern
	mov [rdi + kernel_offset], rsp

	; Lade neuen Stackpointer
	mov rsp, [rsi + kernel_offset]
	; Register wieder herstellen
	pop rbx
	pop rbp
	pop r12
	pop r13
	pop r14
	pop r15
	; Threadwechsel
	ret

[EXTERN context_kickoff]

[GLOBAL context_kickoff_wrapper]
; CONTEXT_KICKOFF_WRAPPER: Präperiert die Argumente für context_kickoff und
;                          springt in eben jene Funktion
;
; C Prototyp: void context_kickoff_wrapper ();

align 8
context_kickoff_wrapper:
	; 1. Parameter (durch context_switch in r15) nach rdi kopieren
	mov rdi, r15
	; 2. Parameter von r14 nach rsi
	mov rsi, r14
	; 3. Parameter von r13 nach rdx
	mov rdx, r13
	; Sprung in eigentlichen Funktion
	jmp context_kickoff

