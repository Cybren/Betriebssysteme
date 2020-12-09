

#pragma once
#include <types.h>

const unsigned CPU_MAX = 8;

namespace System {
	enum Device {
		timer = 0,
		keyboard = 1,
		com1 = 4,
		com2 = 3,
		com3 = 4,
		com4 = 3,
		floppy = 6,
		lpt1 = 7,
		rtc = 8,
		ps2mouse = 12,
		ide1 = 14,
		ide2 = 15
	};


	bool init();


	void bootAPs();


	int getCPUID();


	void* getIOAPICAddress();


	uint8_t getIOAPICID();


	uint8_t getIOAPICSlot(System::Device device);


	unsigned getNumberOfCPUs();


	unsigned getNumberOfOnlineCPUs();


	bool isOnlineCPU(uint8_t cpu);

	void setupThisProcessor();


	uint8_t getLogicalLAPICID(uint8_t cpu);

}
