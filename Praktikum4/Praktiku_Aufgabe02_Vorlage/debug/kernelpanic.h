

#pragma once


#define kernelpanic(MSG) \
	do { \
			DBG << "PANIC: '" << (MSG) << "' in " << __func__ \
			    << " @ " << __FILE__ << ":" << __LINE__ << ") - CPU stopped." \
			    << endl; \
		CPU::die(); \
	} while(0)


#include "debug/output.h"
#include "machine/cpu.h"
