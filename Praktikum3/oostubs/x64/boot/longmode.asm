; Wechsel in den Long Mode (64 bit)

bits 32

[GLOBAL long_mode]
long_mode:

; Wir können über den Befehl `cpuid` herausfinden, ob 64bit von der CPU
; unterstützt werden.
; Problem: Wir müssen zuerst herausfinden, ob der Befehl `cpuid` selbst
; unterstützt wird.
; Dazu müssen wir versuchen im EFLAGS Register das 21. Bit umzudrehen.
; Klappt es, dann gibt es die `cpuid` Instruktion

CPUID_BIT_MASK equ 1 << 21

check_cpuid:
	; EFLAGS zum späteren Wiederherstellen sichern
	pushfd

	; EFLAGS vom Stack nach EAX kopieren.
	mov eax, [esp]

	; Das 21. Bit (ID) in EAX umdrehen
	xor eax, CPUID_BIT_MASK

	; EAX (über den Stack) nach EFLAGS kopieren
	push eax
	popfd

	; Und wieder von EFLAGS (über den Stack) zurück nach EAX kopieren
	; (nun aber mit den umgedrehten Bit, falls unterstützt)
	pushfd
	pop eax

	; Exklusives Oder mit dem noch auf dem Stack liegenden EFLAGS, um alle
	; geänderten (umgedrehten) Bits in EAX zu speichern
	xor eax, [esp]

	; Die ursprünglichen EFLAGS wiederherstellen
	popfd

	; Prüfen ob das 21. Bit nun gesetzt ist -- falls ja, dann haben wir CPUID
	and eax, CPUID_BIT_MASK
	jnz check_long_mode

	; Zeige Fehlermeldung "No CPUID" und halte die CPU an
	mov dword [0xb8000], 0xcf6fcf4e
	mov dword [0xb8004], 0xcf43cf20
	mov dword [0xb8008], 0xcf55cf50
	mov dword [0xb800c], 0xcf44cf49
	hlt

; Nun können wir prüfen, ob Long Mode vorhanden ist. Also fast.
; Zuerst müssen wir prüfen, ob `cpuid` die Funktion, uns zusagen ob Long Mode
; vorhanden ist, überhaupt vorhanden ist. Erst danach können wir nach 64 Bit
; fragen.

CPUID_GET_LARGEST_EXTENDED_FUNCTION_NUMBER equ 0x80000000
CPUID_GET_EXTENDED_PROCESSOR_FEATURES equ 0x80000001

check_long_mode:
	; CPUID-Argument über das Borhandensein von erweiterten Funktionen
	; setzen und CPUID aufrufen
	mov eax, CPUID_GET_LARGEST_EXTENDED_FUNCTION_NUMBER
	cpuid
	; Rückgabe ist die maximale erweiterte Funktion, die unterstützt wird;
	; Wir brauchen die Funktion für die erweiteren Prozessormerkmale
	cmp eax, CPUID_GET_EXTENDED_PROCESSOR_FEATURES
	; Falls nicht vorhanden, so ist die CPU zu alt für den Long Mode
	jb no_long_mode

	; Endlich können wir nach der Verfügbarkeit des Long Modes fragen
	mov eax, CPUID_GET_EXTENDED_PROCESSOR_FEATURES
	cpuid
	; Das 29. Bit im EDX Register sagt uns nun, ob der Long Mode vorhanden ist
	test edx, 1 << 29
	jnz setup_paging
	mov dword [0xb8010], 0xcf44cf49

no_long_mode:
	; Zeige Fehlermeldung "No 64bit" und halte die CPU an
	mov dword [0xb8000], 0xcf6fcf4e
	mov dword [0xb8004], 0xcf36cf20
	mov dword [0xb8008], 0xcf62cf34
	mov dword [0xb800c], 0xcf74cf69
	hlt

; Für Long Mode brauchen wir Paging.
; Da das für den Anfang etwas kompliziert ist, machen wir zuerst einmal für die
; untersten 4 Gigabyte eine Identitätszuweisung -- jede virtuelle Adresse zeigt
; auch auf die gleiche physikalische Adresse. Die Größe ist wichtig für einige
; im Speicher eingeblendete Geräte (und wir wollen sie der Einfachheit halber
; nicht auf andere Adressen umbiegen, sorgt nur für Verwirrung)
; Im Fach BST wird das im Detail durchgenommen, wer einen schnellen Überblick
; will: https://wiki.osdev.org/Page_Tables#2_MiB_pages_2

PAGE_SIZE equ 4096
PAGE_FLAGS_PRESENT equ 1 << 0
PAGE_FLAGS_WRITEABLE equ 1 << 1
PAGE_FLAGS_HUGE equ 1 << 7

setup_paging:
	; Alle Einträge unserer Level 2 Tabellen sollen jeweils 2 MB umfassen (huge),
	; schreibbar sein und auf den entsprechenden physikalischen Speicher zeigen.
	; Da wir bei 64 bit 2048 Einträge (in vier Level 2 Tabellen) beschreiben,
	; bekommen wir dadurch 4 GB Speicher.

	; Wir machen das in einer Schleife und verwenden ECX als Zählervariable
	mov ecx, 0

.identitymap_level2:
	; Physikalische Adresse in EAX (2 MB mulitpliziert mit dem Zähler)
	mov eax, 0x200000
	mul ecx
	; Flags setzen
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE | PAGE_FLAGS_HUGE
	; Entsprechenden Eintrag in der Tabelle setzen
	mov [paging_level2_tables + ecx * 8], eax

	; Zähler erhöhen...
	inc ecx
	; ... bis alle vier Tabellen voll sind
	cmp ecx, 512 * 4
	jne .identitymap_level2

	; Die ersten vier Einträge unserer Level 3 Tabelle sollen auf die vier
	;  Level 2 Tabellen zeigen und schreibbar sein
	; Wir verwenden ECX als Zählervariable
	mov ecx, 0

.identitymap_level3:
	; Adressberechnung: ecx * PAGE_SIZE + paging_level2_tables
	mov eax, ecx
	; Die Größe einer Seite wird in edx gespeichert
	mov edx, PAGE_SIZE
	mul edx
	add eax, paging_level2_tables
	; Flags setzen
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE
	; Jeder Eintrag ist 8 bytes lang
	mov [paging_level3_table + ecx * 8], eax

	; Zähler erhöhen...
	inc ecx
	; ... bis 4, dann sind wir fertig
	cmp ecx, 4
	jne .identitymap_level3

	mov eax, paging_level2_tables
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE
	mov [paging_level3_table], eax

	; Der erste Eintrag unserer Level 4 Tabelle soll auf die Level 3 Tabelle
	; zeigen und schreibbar sein
	mov eax, paging_level3_table
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE
	mov [paging_level4_table], eax

paging_enable:
	; Nun wollen wir das Paging aktivieren.
	; Zuerst müssen wir einige Einstellungen in den control register tätigen:

	; Die oberste Tabelle (Level 4) in das cr3 Register gesteckt.
	mov eax, paging_level4_table
	mov cr3, eax

	; Physikalische Adresserweiterung starten (durch setzen des 5. Bits in cr4)
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; Setze Long Mode Enable Bit in den EFER MSR (model specific register)
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; Schlussendlich wird in cr0 das Bit um Paging zu aktivieren gesetzt
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	; GDT laden
	lgdt [gdt64.pointer]

	; weiter sprung in den Startcode
	jmp gdt64.code:long_mode_start

	; print `OK` to screen
	mov dword [0xb8000], 0x3f4b3f4f
	hlt

[SECTION .rodata]
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64 ; new
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

; Speicher für die Seitentabellen/-verzeichnisse
[SECTION .bss]

align 4096

[GLOBAL paging_level4_table]
[GLOBAL paging_level3_table]
[GLOBAL paging_level2_tables]
; Tabelle für Level 4: Page Map Level 4
paging_level4_table:
	resb PAGE_SIZE

; Tabelle Level 3: Page Directory Pointer Table
paging_level3_table:
	resb PAGE_SIZE

; Level 2: Page Directories (4 Stück)
paging_level2_tables:
	resb PAGE_SIZE * 4

; TODO: Identity mapping checken.

[SECTION .text]
[EXTERN kernel_init]
bits 64

long_mode_start:
	; Schreibe 0 in alle Segment Register
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Springe in die C Funktion
	extern kernel_init
	call kernel_init

	; print `OKAY` to screen
	mov rax, 0x2f592f412f4b4f4f
	mov qword [0xb8000], rax
	hlt
