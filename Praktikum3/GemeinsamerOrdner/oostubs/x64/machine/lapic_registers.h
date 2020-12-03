/*! \file
 *  \brief Strukturen und Makros zum Zugriff auf den Local APIC
 */

#pragma once

#include "types.h"

namespace LAPIC {
	typedef uint32_t Register;

	/// System Programming Guide 3A, p. 9-8 - 9-10
	enum Index : uint16_t {
		Identification               = 0x020, // Local APIC ID Register, R/W
		Version                      = 0x030, // Local APIC Version Register, RO
		TaskPriority                 = 0x080, // Task Priority Register, R/W
		EOI                          = 0x0b0, // EOI Register, WO
		LogicalDestination           = 0x0d0, // Logical Destination Register, R/W
		DestinationFormat            = 0x0e0, // Destination Format Register, bits 0-27 RO, bits 28-31 R/W
		SpuriousInterruptVector      = 0x0f0, // Spurious Interrupt Vector Register, bits 0-8 R/W, bits 9-1 R/W
		InterruptCommandRegisterLow  = 0x300, // Interrupt Command Register 1, R/W
		InterruptCommandRegisterHigh = 0x310, // Interrupt Command Register 2, R/W
		TimerControl                 = 0x320, // LAPIC timer control register, R/W
		TimerInitialCounter          = 0x380, // LAPIC timer initial counter register, R/W
		TimerCurrentCounter          = 0x390, // LAPIC timer current counter register, RO
		TimerDivideConfiguration     = 0x3e0  // LAPIC timer divide configuration register, RW
	};

	Register read(Index idx);
	void write(Index idx, Register value);
}
