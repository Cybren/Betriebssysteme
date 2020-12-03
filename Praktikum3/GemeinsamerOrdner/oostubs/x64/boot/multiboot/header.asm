[SECTION .multiboot_header]

; Konfiguration laden
%include 'boot/multiboot/config.inc'

; Multiboot Header
align 4
multiboot_header:
	; Magischer Header
	dd MULTIBOOT_HEADER_MAGIC_OS
	; Flags (bedingen auch die nachfolgenden Felder)
	dd MULTIBOOT_HEADER_FLAGS
	; Prüfsumme über den Header
	dd MULTIBOOT_HEADER_CHKSUM

	; Falls Flag A_OUT KLUDGE gesetzt:
	; Header Addresse
	dd 0
	; Beginn und Ende der load Addresse
	dd 0
	dd 0
	; Ende des BSS Segments
	dd 0
	; Einsprungsaddresse
	dd 0

	; Falls Flag MULTIBOOT_VIDEO_MODE gesetzt:
	; Modus: 0 für Grafik, 1 für Text (beeinflusst Bedeutung der nachfolgende Felder)
	dd 0
	; Weite (Pixel bei Grafik, Zeichen bei Text)
	dd MULTIBOOT_VIDEO_WIDTH
	; Höhe (Pixel bei Grafik, Zeilen bei Text)
	dd MULTIBOOT_VIDEO_HEIGHT
	; Farbtiefe in Bit bei Grafik, Anzahl der Farben bei Text
	dd MULTIBOOT_VIDEO_BITDEPTH
