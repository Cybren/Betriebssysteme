#include "machine/gdt.h"
#include "debug/output.h"


GDTDescriptor gdt_descriptors[] __attribute__ ((aligned (8))) = {

	{},


	{ 0x0,
	  0xFFFFFFFF,
	  true,
	  false},


	{ 0x0,
	  0xFFFFFFFF,
	  false,
	  false},

};


extern "C"
constexpr GlobalDescriptorTable gdt_desc_global __attribute__ ((aligned(8))) = {
	sizeof(gdt_descriptors) - 1,
	&gdt_descriptors[0]
};

