#include "machine/lapic.h"
#include "machine/lapic_registers.h"
#include "machine/io_port.h"

namespace LAPIC {
namespace Timer {



enum DeliveryStatus {
	Idle        = 0,
	SendPending = 1
};

enum TimerMode {
	OneShot  = 0,
	Periodic = 1,
	Deadline = 2

};

enum Mask {
	NotMasked = 0,
	Masked = 1
};


union TimerControlRegister {
	struct {
		uint32_t  vector : 8;
		uint32_t reserved_1 : 4;
		DeliveryStatus delivery_status :1;
		uint32_t reserved_2:3;
		Mask masked:1;
		TimerMode timer_mode: 2;
		uint32_t reserved_3:13;
	};
	Register value;
} __attribute__((packed));


static IO_Port ctrl(0x43);
static IO_Port data(0x42);
static IO_Port help(0x61);


static const Register div_masks[] = {
	0xb,
	0x0,
	0x1,
	0x2,
	0x3,
	0x8,
	0x9,
	0xa
};

uint32_t ticks(){


	set(0xffffffff, 1, 42, false, true);



	help.outb(0x01);
	ctrl.outb(0xB0);
	data.outb(0xFF);
	data.outb(0xFF);


	Register ticks = LAPIC::read(TimerCurrentCounter);


	while(!(help.inb() & 0x20));


	ticks = ticks - read(TimerCurrentCounter);


	set(0, 1, 42, false, true);

	uint32_t freq = (uint32_t)(((uint64_t) ticks * 1000 * 1000) / (838 * 65535));
	return freq;
}



Register getClockDiv(uint8_t div){

	if(!((div != 0) && !(div & (div - 1)))) {
		return 0xff;
	}

	int trail = __builtin_ctz(div);
	if (trail > 7) {
		return 0xff;
	}

	return div_masks[trail];
}

void set(uint32_t counter, uint8_t divide, uint8_t vector, bool periodic, bool masked)
{

	(void) counter;
	(void) divide;
	(void) vector;
	(void) periodic;
	(void) masked;
}
}
}
