bits 32

[GLOBAL long_mode]
long_mode:

CPUID_BIT_MASK equ 1 << 21

check_cpuid:
	pushfd
	mov eax, [esp]
	xor eax, CPUID_BIT_MASK
	push eax
	popfd
	pushfd
	pop eax
	xor eax, [esp]
	popfd
	and eax, CPUID_BIT_MASK
	jnz check_long_mode
	mov dword [0xb8000], 0xcf6fcf4e
	mov dword [0xb8004], 0xcf43cf20
	mov dword [0xb8008], 0xcf55cf50
	mov dword [0xb800c], 0xcf44cf49
	hlt

CPUID_GET_LARGEST_EXTENDED_FUNCTION_NUMBER equ 0x80000000
CPUID_GET_EXTENDED_PROCESSOR_FEATURES equ 0x80000001

check_long_mode:
	mov eax, CPUID_GET_LARGEST_EXTENDED_FUNCTION_NUMBER
	cpuid
	cmp eax, CPUID_GET_EXTENDED_PROCESSOR_FEATURES
	jb no_long_mode
	mov eax, CPUID_GET_EXTENDED_PROCESSOR_FEATURES
	cpuid
	test edx, 1 << 29
	jnz setup_paging
	mov dword [0xb8010], 0xcf44cf49

no_long_mode:
	mov dword [0xb8000], 0xcf6fcf4e
	mov dword [0xb8004], 0xcf36cf20
	mov dword [0xb8008], 0xcf62cf34
	mov dword [0xb800c], 0xcf74cf69
	hlt

PAGE_SIZE equ 4096
PAGE_FLAGS_PRESENT equ 1 << 0
PAGE_FLAGS_WRITEABLE equ 1 << 1
PAGE_FLAGS_HUGE equ 1 << 7

setup_paging:
	mov ecx, 0

.identitymap_level2:
	mov eax, 0x200000
	mul ecx
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE | PAGE_FLAGS_HUGE
	mov [paging_level2_tables + ecx * 8], eax
	inc ecx
	cmp ecx, 512 * 4
	jne .identitymap_level2
	mov ecx, 0

.identitymap_level3:
	mov eax, ecx
	mov edx, PAGE_SIZE
	mul edx
	add eax, paging_level2_tables
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE
	mov [paging_level3_table + ecx * 8], eax
	inc ecx
	cmp ecx, 4
	jne .identitymap_level3

	mov eax, paging_level2_tables
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE
	mov [paging_level3_table], eax
	mov eax, paging_level3_table
	or eax, PAGE_FLAGS_PRESENT | PAGE_FLAGS_WRITEABLE
	mov [paging_level4_table], eax

paging_enable:
	mov eax, paging_level4_table
	mov cr3, eax
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	lgdt [gdt64.pointer]

	jmp gdt64.code:long_mode_start

	mov dword [0xb8000], 0x3f4b3f4f
	hlt

[SECTION .rodata]
gdt64:
    dq 0
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

[SECTION .bss]

align 4096

[GLOBAL paging_level4_table]
[GLOBAL paging_level3_table]
[GLOBAL paging_level2_tables]

paging_level4_table:
	resb PAGE_SIZE

paging_level3_table:
	resb PAGE_SIZE

paging_level2_tables:
	resb PAGE_SIZE * 4

[SECTION .text]
[EXTERN kernel_init]
bits 64

long_mode_start:
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	extern kernel_init
	call kernel_init

	mov rax, 0x2f592f412f4b4f4f
	mov qword [0xb8000], rax
	hlt
