; Dies ist der Eintrittspunkt des eigentlichen Systems.
; Die Umschaltung in den 'Protected Mode' ist bereits (durch den Bootloader)
; erfolgt. Es wird alles vorbereitet, damit so schnell wie möglich die weitere
; Ausführung durch C/C++-Code erfolgen kann.

bits 32

; externe Funktionen und Variablen, die hier benötigt werden
[EXTERN cpu_stack_size]    ; Initiale Stackgröße eines Prozessor
[EXTERN cpu_stack_pointer] ; Zeiger auf Speicher für CPU Stack
[EXTERN kernel_init]       ; Wird zur Interruptbehandlung aufgerufen
[EXTERN gdt_desc_global]   ; Der "Pointer" zur globalen Deskriptortabelle (machine/gdt.cc)
[EXTERN long_mode]         ; Einsprung in den long mode
[EXTERN multiboot_addr]    ; Zeiger, welcher die Adresse auf die Multiboot Datenstruktur bekommen soll

; Konfiguration für Multiboot laden
%include "boot/multiboot/config.inc"

[SECTION .text]

; Startroutine des Bootstrapprozessors (CPU0)
[GLOBAL startup_bsp]
startup_bsp:
	; Prüfen, ob Loader Multiboot (1) Struktur verwendet hat
	cmp eax, MULTIBOOT_HEADER_MAGIC_LOADER
	jne skip_multiboot
	; Adresse der Multiboot-Struktur liegt magisch in einem Register,
	; wir speichern uns das für die Zukunft mal präventiv weg.
	mov [multiboot_addr], ebx

skip_multiboot:
	; Unterbrechungen sperren
	cli
	; NMI verbieten
	mov al, 0x80
	out 0x70, al

	jmp load_cs

; Initialisierung des Segments
; (wird sowohl von Betriebsystem- als auch Anwendungsprozessor verwendet)
[GLOBAL segment_init]
segment_init:
	; GDT setzen
	lgdt [gdt_desc_global]

	; Datensegmentregister initialisieren
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; cs Segment Register neu laden
	jmp 0x8:load_cs

load_cs:
	; Stackpointer initialisieren:
	; cpu_stack_pointer atomar um cpu_stack_size erhöhen (damit gleichzeitig startende APs kein Problem haben)
	mov eax, [cpu_stack_size]
	lock xadd [cpu_stack_pointer], eax
	; Aufgrund der Richtung, in der der Stack wächst muss nochmal cpu_stack_size addieren werden
	add eax, [cpu_stack_size]
	; Stack pointer verwenden
	mov esp, eax

	; Richtung für String-Operationen festlegen
	cld

	; Longmode (64 bit) initialisieren)
	jmp long_mode

	; Wechsel in C/C++
	call kernel_init
