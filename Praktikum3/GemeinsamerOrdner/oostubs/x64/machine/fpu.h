/*! \file
 *  \brief Fließkommazahleneinheit
 */
#pragma once

#include "machine/cpu.h"

namespace FPU {

	/*! \brief 512 Byte Struktur zum speichern der FPU Register (via 'FXSAVE')
	 *
	 * \see intel_manual_vol1.pdf 10.5.1
	 */
	struct State {
		uint16_t fcw; // FPU Control Word
		uint16_t fsw; // FPU Status Word
		uint8_t ftw;  // FPU Tag Word
		uint16_t : 0; // (16 bit ausrichten)
		uint16_t fop; // FPU Opcode

		// FPU Instruction Pointer
		union {
			uint64_t ip;
			struct {
				uint32_t ip_off;
				uint32_t ip_seg;
			} __attribute__((packed));
		};

		// FPU Data Pointer Offset
		union {
			uint64_t dp;
			struct {
				uint32_t dp_off;
				uint32_t dp_seg;
			} __attribute__((packed));
		};

		// MXCSR Control and Status
		uint32_t mxcsr;
		uint32_t mxcsr_mask;

		/*! \brief 80-Bit Register für FPU-Stack/MMX
		 */
		struct ST {
			uint64_t fraction : 63,
			         integer : 1;
			uint16_t exponent : 15,
			         sign : 1;
		} __attribute__((packed));
		struct {
			ST value;
			uint64_t : 48; // (Ausrichten auf 64 Bit, somit 128 Bit im Speicher)
		} st[8];

		/*! \brief 128-Bit Register der SSE Erweiterung
		 */
		struct XMM {
			uint64_t low;
			uint64_t high;
		} __attribute__((packed));
		XMM xmm[16]; // SSE State

		// Reserviert
		uint64_t reserved[12];
	}  __attribute__((packed)) __attribute__((aligned(16)));

	assert_size(State, 512);

	// Fehlt: Test auf CPUID.01h:EDX.SSE
	inline bool init(){
		uint16_t temp;
		uintptr_t cr0 = CPU::CR<0>::read();
		// Clear EM and TS
		cr0 &= ~0x6;
		// Set MP
		cr0 |= 0x2;
		CPU::CR<0>::write(cr0);
		// Init FPU, store status word
		asm volatile(
			"fninit\n\t"
			"fnstsw %0\n\t"
			: "=m"(temp)
		);
		// Check status word
		if (temp != 0) {
			return false;
		}

		// Store control word
		asm volatile(
			"fnstcw %0"
			: "=m"(temp)
		);
		// Check control word
		if ((temp & 0x103f) != 0x3f) {
			return false;
		}
		// Set CR4.{OSFXSR, OSXMMEXCPT
		// TODO: really handle interrupt 19?
		CPU::CR<4>::write(CPU::CR<4>::read() | 0x600);
		return true;
	}

	/*! \brief Sichere den FPU State
	 * \param target Zeiger auf den Speicher, in den der FPU State geschrieben
	 * werden soll
	 */
	inline void save(State * target){
		asm volatile(
			"fxsave %0\n\t"
			:
			: "m"(target)
		);
	}

	/*! \brief Wiederherstellen eines zuvor gespeicherten FPU States
	 * \param source Zeiger auf den Speicher, von dem der FPU State
	 * wiederhergestellt werden soll
	 */
	inline void restore(const State * source){
		asm volatile(
			"fxrstor %0\n\t"
			:
			: "m"(source)
		);
	}
}
