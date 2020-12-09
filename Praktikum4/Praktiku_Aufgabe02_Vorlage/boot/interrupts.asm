[EXTERN guardian]

[SECTION .text]

%macro IRQ 2
align 8
irq_handler_%1:
	%if %2 == 0
		push 0
	%endif

	push rax
	push rcx
	push rdx
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11

	cld

	mov rdi, %1

	mov rsi, rsp

	call guardian

	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rax

	add rsp, 8

	iretq
%endmacro

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

%assign i 18
%rep 238
IRQ i, 0
%assign i i+1
%endrep

[SECTION .data]

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
