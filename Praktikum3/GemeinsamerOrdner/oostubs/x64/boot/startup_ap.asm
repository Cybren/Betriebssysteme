; Start der restlichen Prozessoren (im Real Mode)
; Umschaltung in den 'Protected-Mode' mit temporäre GDT
; Dieser Code wird von APICSystem::copySetupAPtoLowMem() reloziert!

[SECTION .setup_ap_seg]

; Wir befinden uns beim Start der Prozessoren (anders als bei BSP)
; zuerst im Real Mode (16 bit) und muessen haendisch Umschalten.
USE16

setup_ap:
	; Segmentregister initialisieren
	mov ax,cs ; Daten- und Codesegment sollen
	mov ds,ax ; hierher zeigen. Stack brauchen wir hier nicht.

	; Unterbrechungen sperren
	cli
	; NMI verbieten
	mov al, 0x80
	out 0x70, al

	; vorrübergehende GDT setzen
	lgdt [ap_gdtd - setup_ap]

	; Umschalten in den Protected Mode
	mov eax,cr0 ; Setze PM bit im Kontrollregister 1
	or  eax,1
	mov cr0,eax
	; Weiter Sprung in die 32 bit Funktion
	jmp dword 0x08:startup_ap

; temporäre GDT
align 4
ap_gdt:
	dw 0,0,0,0   ; NULL Deskriptor
	; Codesegment von 0-4GB
	dw 0xFFFF    ; 4Gb - (0x100000*0x1000 = 4Gb)
	dw 0x0000    ; base address=0
	dw 0x9A00    ; code read/exec
	dw 0x00CF    ; granularity=4096, 386 (+5th nibble of limit)
; Datensegment von 0-4GB
	dw 0xFFFF    ; 4Gb - (0x100000*0x1000 = 4Gb)
	dw 0x0000    ; base address=0
	dw 0x9200    ; data read/write
	dw 0x00CF    ; granularity=4096, 386 (+5th nibble of limit)

; temporärer GDT-Deskriptor
ap_gdtd:
	dw $ - ap_gdt - 1 ; Limit
	dd 0x40000 + ap_gdt - setup_ap ; Physikalische Adresse der ap_gdt

[SECTION .text]

USE32

; 32 bit Startup-Code für die APs
startup_ap:
	; Alle Segmentselektoren neu laden, die zeigen noch auf die ap_gdt
	; Nach Intel Manual Kapitel 9.9.1 ist das nötig
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Jetzt verwenden wir die gleiche Initialisierung wie der BSP
	[EXTERN segment_init]
	jmp segment_init
