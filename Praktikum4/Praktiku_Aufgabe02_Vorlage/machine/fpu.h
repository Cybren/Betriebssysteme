
#pragma once

#include "machine/cpu.h"

namespace FPU {


	struct State {
		uint16_t fcw;
		uint16_t fsw;
		uint8_t ftw;
		uint16_t : 0;
		uint16_t fop;


		union {
			uint64_t ip;
			struct {
				uint32_t ip_off;
				uint32_t ip_seg;
			} __attribute__((packed));
		};


		union {
			uint64_t dp;
			struct {
				uint32_t dp_off;
				uint32_t dp_seg;
			} __attribute__((packed));
		};


		uint32_t mxcsr;
		uint32_t mxcsr_mask;


		struct ST {
			uint64_t fraction : 63,
			         integer : 1;
			uint16_t exponent : 15,
			         sign : 1;
		} __attribute__((packed));
		struct {
			ST value;
			uint64_t : 48;
		} st[8];


		struct XMM {
			uint64_t low;
			uint64_t high;
		} __attribute__((packed));
		XMM xmm[16];


		uint64_t reserved[12];
	}  __attribute__((packed)) __attribute__((aligned(16)));

	assert_size(State, 512);


	inline bool init(){
		uint16_t temp;
		uintptr_t cr0 = CPU::CR<0>::read();

		cr0 &= ~0x6;

		cr0 |= 0x2;
		CPU::CR<0>::write(cr0);

		asm volatile(
			"fninit\n\t"
			"fnstsw %0\n\t"
			: "=m"(temp)
		);

		if (temp != 0) {
			return false;
		}


		asm volatile(
			"fnstcw %0"
			: "=m"(temp)
		);

		if ((temp & 0x103f) != 0x3f) {
			return false;
		}


		CPU::CR<4>::write(CPU::CR<4>::read() | 0x600);
		return true;
	}


	inline void save(State * target){
		asm volatile(
			"fxsave %0\n\t"
			:
			: "m"(target)
		);
	}


	inline void restore(const State * source){
		asm volatile(
			"fxrstor %0\n\t"
			:
			: "m"(source)
		);
	}
}
