bits 32

[EXTERN cpu_stack_size]
[EXTERN cpu_stack_pointer]
[EXTERN kernel_init]
[EXTERN gdt_desc_global]
[EXTERN long_mode]
[EXTERN multiboot_addr]

%include "boot/multiboot/config.inc"

[SECTION .text]

[GLOBAL startup_bsp]
startup_bsp:
	cmp eax, MULTIBOOT_HEADER_MAGIC_LOADER
	jne skip_multiboot
	mov [multiboot_addr], ebx

skip_multiboot:
	cli
	mov al, 0x80
	out 0x70, al

	jmp load_cs

[GLOBAL segment_init]
segment_init:
	lgdt [gdt_desc_global]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x8:load_cs

load_cs:
	mov eax, [cpu_stack_size]
	lock xadd [cpu_stack_pointer], eax
	add eax, [cpu_stack_size]
	mov esp, eax

	cld

	jmp long_mode

	call kernel_init
