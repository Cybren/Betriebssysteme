; vim: set et ts=4 sw=4:

%include "machine/toc.inc"

; EXPORTIERTE FUNKTIONEN
[GLOBAL toc_switch]
[GLOBAL toc_go]

[SECTION .text]

; IMPLEMENTIERUNG DER FUNKTIONEN

; TOC_GO : Startet den ersten Thread ueberhaupt.
;
; C Prototyp: void toc_go (struct toc *regs);

align 8
toc_go:
    mov eax, [esp + 4]          ; Parameter regs ins eax Register
    mov ebx, [ebx_offset + eax] ; Register wieder herstellen
    mov esi, [esi_offset + eax]
    mov edi, [edi_offset + eax]
    mov ebp, [ebp_offset + eax]
    mov esp, [esp_offset + eax] ; Stack Pointer umsetzen
    ret                         ; Sprung zu 'kickoff'

; TOC_SWITCH : Threadumschaltung. Der aktuelle Registersatz wird     
;              gesichert und der Registersatz des neuen "thread of control"
;              wird in den Prozessor eingelesen.  
;
; C Prototyp: void toc_switch (struct toc *regs_now,
;                              struct toc *reg_then);

align 8
toc_switch:
    mov eax, [esp + 4]          ; regs_now
    mov [ebx_offset + eax], ebx ; nicht-fluechtige Register speichern
    mov [esi_offset + eax], esi
    mov [edi_offset + eax], edi
    mov [ebp_offset + eax], ebp
    mov [esp_offset + eax], esp

    mov eax, [esp + 8]          ; regs_then
    mov ebx, [ebx_offset + eax] ; Register wieder herstellen
    mov esi, [esi_offset + eax]
    mov edi, [edi_offset + eax]
    mov ebp, [ebp_offset + eax]
    mov esp, [esp_offset + eax]

    ret ; Threadwechsel
