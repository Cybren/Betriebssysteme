#include "strbuf.h"

void Stringbuffer::put(char c) {
	buffer[pos] = c;
	pos++;
	if (pos == sizeof(buffer))
		flush();
}
