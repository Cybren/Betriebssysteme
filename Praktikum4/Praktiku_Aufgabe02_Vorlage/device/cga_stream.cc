#include "device/cga_stream.h"

void CGA_Stream::flush()
{
	print(buffer, pos);
	pos = 0;
}
