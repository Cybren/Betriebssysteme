#pragma once
#include "machine/cpu.h"

extern "C" void guardian(uint32_t vector, irq_context *context);
