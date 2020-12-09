[SECTION .setup_ap_seg]

USE16

setup_ap:
	mov ax,cs
	mov ds,ax

	cli
	mov al, 0x80
	out 0x70, al

	lgdt [ap_gdtd - setup_ap]

	mov eax,cr0
	or  eax,1
	mov cr0,eax
	jmp dword 0x08:startup_ap

align 4
ap_gdt:
	dw 0,0,0,0
	dw 0xFFFF
	dw 0x0000
	dw 0x9A00
	dw 0x00CF
	dw 0xFFFF
	dw 0x0000
	dw 0x9200
	dw 0x00CF

ap_gdtd:
	dw $ - ap_gdt - 1
	dd 0x40000 + ap_gdt - setup_ap

[SECTION .text]

USE32

startup_ap:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	[EXTERN segment_init]
	jmp segment_init
