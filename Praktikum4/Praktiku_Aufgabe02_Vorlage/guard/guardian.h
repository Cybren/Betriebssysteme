#pragma once




struct irq_context {
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rax;
	uint64_t error_code : 32;
	uint64_t            : 0;
	uint64_t rip;
	uint64_t cs : 16;
	uint64_t    : 0;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss : 16;
	uint64_t    : 0;
} __attribute__((packed));


extern "C" void guardian(uint32_t vector, irq_context *context);
