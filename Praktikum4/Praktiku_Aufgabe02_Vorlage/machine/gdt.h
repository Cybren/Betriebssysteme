#pragma once

#include "types.h"

enum {
	KERNEL_CODE_SEGMENT = 1,
	KERNEL_DATA_SEGMENT = 2,
	KERNEL_16DATA_SEGMENT = 3,
	KERNEL_16CODE_SEGMENT = 4,
};


class GDTDescriptor {
public:
	union {
		struct
		{
			unsigned int limit_low:16;
			unsigned int base_low : 24;

			unsigned int accessed :1;
			unsigned int read_write :1;
			unsigned int conforming_expand_down :1;
			unsigned int code :1;
			unsigned int always_1 :1;
			unsigned int DPL :2;
			unsigned int present :1;
			unsigned int limit_high :4;
			unsigned int available :1;
			unsigned int always_0 :1;
			unsigned int big :1;
			unsigned int granularity :1;
			unsigned int base_high :8;
		} __attribute__((packed));

		uint64_t value;
	} __attribute__((packed));


	constexpr GDTDescriptor() : value(0) {};


	constexpr GDTDescriptor(uint64_t val) : value(val) {};


	constexpr GDTDescriptor(uint32_t base, uint32_t limit,
							bool code, bool user, bool _32_bit = true)
		: limit_low((limit > 0xffff)
					? ((limit >> 12) & 0x0000FFFF)
					: (limit & 0x0000FFFF)),
		base_low(base & 0x00FFFFFF),
		accessed(0),
		read_write(1),
		conforming_expand_down(0),
		code(code ? 1 : 0),
		always_1(1),
		DPL(user ? 3 : 0),
		present(1),
		limit_high((limit > 0xffff)
				   ? ((limit >> 28) & 0xF)
				   : ((limit >> 16) & 0xF)),
		available(0),
		always_0(0),
		big(_32_bit),
		granularity(limit > 0xffff),
		base_high((base & 0xFF000000) >> 24)
	{ };

} __attribute__((packed));


struct GlobalDescriptorTable {
	uint16_t limit;
	const GDTDescriptor* base;
} __attribute__((packed));

extern "C"  GDTDescriptor gdt_descriptors[];
