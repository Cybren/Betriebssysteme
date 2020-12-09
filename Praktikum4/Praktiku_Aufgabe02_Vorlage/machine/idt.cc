#include "idt.h"
#include "gdt.h"


extern void * const irq_handlers[];

namespace IDT {


struct Entry {
	uint16_t address_low;
	uint16_t selector;
	union {
		struct {
			uint8_t ist     : 3;
			uint8_t         : 5;
			Gate type       : 3;
			GateSize size   : 1;
			uint8_t         : 1;
			DPL dpl         : 2;
			uint8_t present : 1;
		} __attribute__((packed));
		uint16_t flags;
	};
	uint64_t address_high : 48;
	uint64_t : 0;
} __attribute__((packed));


static struct Entry idt[256];


struct Register {
	uint16_t limit;
	struct Entry * base;
	Register(uint8_t max = 255) : limit((max + (uint16_t)1)*8-1), base(idt) {};
} __attribute__((packed));

static_assert(sizeof(Entry) == 16, "IDT::Entry has wrong size");
static_assert(sizeof(Register) == 10, "IDT::Register has wrong size");

void init(uint8_t max){

	for (unsigned int i=0; i<=max ; i++)
		handle(i, irq_handlers[i]);

	Register idtr(max);

	asm volatile("lidt %0\n\t" :: "m"(idtr) );
}

void handle(uint8_t vector, void * handler, enum Gate type, enum GateSize size, enum DPL dpl, bool present){
	struct Entry &entry = idt[vector];
	entry.selector = 8;
	entry.type = type;
	entry.size = size;
	entry.dpl = dpl;
	entry.present = present ? 1 : 0;
	uintptr_t address = (uintptr_t)(handler);
	entry.address_low = address & 0xffff;
	entry.address_high = (address >> 16) & 0xffffffffffff;
}

}
