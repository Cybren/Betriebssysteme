

#pragma once

#include "types.h"

namespace LAPIC {
	typedef uint32_t Register;


	enum Index : uint16_t {
		Identification               = 0x020,
		Version                      = 0x030,
		TaskPriority                 = 0x080,
		EOI                          = 0x0b0,
		LogicalDestination           = 0x0d0,
		DestinationFormat            = 0x0e0,
		SpuriousInterruptVector      = 0x0f0,
		InterruptCommandRegisterLow  = 0x300,
		InterruptCommandRegisterHigh = 0x310,
		TimerControl                 = 0x320,
		TimerInitialCounter          = 0x380,
		TimerCurrentCounter          = 0x390,
		TimerDivideConfiguration     = 0x3e0
	};

	Register read(Index idx);
	void write(Index idx, Register value);
}
