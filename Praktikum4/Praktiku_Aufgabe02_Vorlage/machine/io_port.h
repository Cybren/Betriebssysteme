

#pragma once

#include "types.h"





class IO_Port
{

	uint16_t address;
public:

	IO_Port(uint16_t a) : address(a) {}

	void outb(uint8_t val) const {
		asm volatile(
			"out %%al, %%dx\n\t"
			:
			:"a"(val), "d"(address)
			:
		);
	};

	void outw(uint16_t val) const {
		asm volatile(
			"out %%ax, %%dx\n\t"
			:
			:"a"(val), "d"(address)
			:
		);
	};

	uint8_t inb() const {
		uint8_t out = 0;

		asm volatile(
			"in %%dx, %%al\n\t"
			:"=a"(out)
			:"d"(address)
			:
		);

		return out;
	};

	uint16_t inw() const {
		uint16_t out = 0;

		asm volatile(
			"inw %%dx, %%ax\n\t"
			:"=a"(out)
			:"d"(address)
			:
		);

		return out;
	};
};
