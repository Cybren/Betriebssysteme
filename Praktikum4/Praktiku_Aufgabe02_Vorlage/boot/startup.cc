#include "types.h"
#include "debug/output.h"
#include "machine/acpi.h"
#include "machine/apicsystem.h"
#include "machine/cpu.h"
#include "machine/idt.h"
#include "machine/io_port.h"


extern const unsigned long cpu_stack_size = 4096;

static unsigned char cpu_stack[CPU_MAX * cpu_stack_size] __attribute__((aligned(16)));
unsigned char * cpu_stack_pointer = cpu_stack;


static void delay() {
	asm volatile("nop\n\t":::"memory");
}


static void initialise_pics() {
	IO_Port p20(0x20);
	IO_Port p21(0x21);
	IO_Port pa0(0xa0);
	IO_Port pa1(0xa1);


	p20.outb(0x11);
	delay();
	pa0.outb(0x11);
	delay();


	p21.outb(0x20);
	delay();


	pa1.outb(0x28);
	delay();


	p21.outb(0x04);
	delay();


	pa1.outb(0x02);
	delay();


	p21.outb(0x03);
	delay();
	pa1.outb(0x03);
	delay();


	pa1.outb(0xff);
	delay();

	p21.outb(0xff);
}


static void csu_init() {
	extern void (*__init_array_start[])();
	extern void (*__init_array_end[])();

	const size_t size = __init_array_end - __init_array_start;
	for (size_t i = 0; i < size; i++) {
		(*__init_array_start[i])();
	}
}

extern "C" void _init();
extern "C" void _fini();
extern "C" int main_ap() __attribute__((weak));
extern "C" int main();

static bool isBootstrapProcessor = true;

extern "C" void kernel_init() {

	IDT::init();

	if(isBootstrapProcessor) {
		isBootstrapProcessor = false;


		initialise_pics();


		_init();
		csu_init();



		if(!System::init()) {
			DBG << "acpi init failed" << endl;
			CPU::die();
		}
		System::setupThisProcessor();


		main();


		_fini();
	} else {

		System::setupThisProcessor();


		main_ap();
	}


	DBG << "CPU quit." << endl;
	for(;;) {
		CPU::die();
	}
}
