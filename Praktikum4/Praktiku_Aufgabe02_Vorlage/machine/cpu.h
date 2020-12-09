



#pragma once

#include "types.h"


namespace CPU
{

	namespace Interrupt {

		inline void enable() {
			asm volatile("sti\n\t nop\n\t" : : : "memory");
		}


		inline bool disable() {
			uintptr_t out;
			asm volatile (
				"pushf\n\t"
				"pop %0\n\t"
				"cli\n\t"
				: "=r"(out)
				:
				: "memory"
			);

			bool enabled = (out & 0x200) != 0;
			return enabled;
		}


		inline void restore(bool val) {
			if(val)
				enable();
		}
	}


	inline void pause() {
		asm volatile("pause\n\t" : : : "memory");
	}


	inline void idle() {
		asm volatile("sti\n\t hlt\n\t" : : : "memory");
	}

	inline void die() {
		asm volatile("cli\n\t hlt\n\t" : : : "memory");
	}


	inline void halt() {
		asm volatile("hlt\n\t" : : : "memory");
	}


	template<uint32_t id>
	class MSR {
	public:
		static uint64_t read(void) {
			uint64_t retval;
			asm volatile("rdmsr" : "=A"(retval) : "c"(id) : "memory");
			return retval;
		}

		static void write(uint64_t val) {
			asm volatile("wrmsr" : : "A"(val), "c"(id) : "memory");
		}
	};


	template<uint8_t id>
	class CR {
	public:
		static uintptr_t read(void) {
			uintptr_t val;
			asm volatile("mov %%cr%c1, %0" : "=r"(val) : "n"(id));
			return val;
		}

		static void write(uintptr_t value) {
			asm volatile("mov %0, %%cr%c1" : : "r"(value), "n"(id));
		}
	};


	namespace TSC {
		inline uint64_t read(void) {
			uint64_t val;
			asm volatile("rdtsc" : "=A"(val));
			return val;
		}
	}


	template<uint8_t id>
	class PMC {
	public:
		static uint64_t read(void) {
			uint64_t val;
			asm volatile("rdpmc" : "=A"(val) : "c"(id) : "memory" );
			return val;
		}
	};
};
