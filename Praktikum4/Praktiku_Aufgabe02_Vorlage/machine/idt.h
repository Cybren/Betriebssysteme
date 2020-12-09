#pragma once

#include "types.h"
namespace IDT {


	enum Gate {
		GATE_TASK = 0x5,
		GATE_INT  = 0x6,
		GATE_TRAP = 0x7,
	};


	enum GateSize {
		GATE_SIZE_16 = 0,
		GATE_SIZE_32 = 1,
	};


	enum DPL {
		DPL_KERNEL = 0,
		DPL_USER = 3,
	};


	void init(uint8_t max = 255);


	void handle(uint8_t vector, void * handler, enum Gate type = Gate::GATE_INT, enum GateSize size = GateSize::GATE_SIZE_32, enum DPL dpl = DPL::DPL_KERNEL, bool present = true);
}
