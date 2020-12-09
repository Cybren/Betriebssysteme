
#include "types.h"
#include "guardian.h"

extern "C" void guardian(uint32_t vector, irq_context *context)
{
	(void) vector;
	(void) context;

}
