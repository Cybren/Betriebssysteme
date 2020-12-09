

#pragma once

#include "strbuf.h"



class O_Stream
	: public Stringbuffer
{
	O_Stream(const O_Stream&)            = delete;
	O_Stream& operator=(const O_Stream&) = delete;
public:

	int base;


	O_Stream ()
		: Stringbuffer (), base(10)
	{}


	virtual ~O_Stream () {}


	virtual void flush () = 0;


	O_Stream& operator << (char c);


	O_Stream& operator << (unsigned char c);

	O_Stream& operator << (const char* string);

	O_Stream& operator << (bool b);

	O_Stream& operator << (short ival);

	O_Stream& operator << (unsigned short ival);

	O_Stream& operator << (int ival);

	O_Stream& operator << (unsigned int ival);

	O_Stream& operator << (long ival);

	O_Stream& operator << (unsigned long ival);

	O_Stream& operator << (long long ival);

	O_Stream& operator << (unsigned long long ival);

	O_Stream& operator << (const void* ptr);

	O_Stream& operator << (O_Stream& (*f) (O_Stream&));

};


O_Stream& flush(O_Stream& os);


O_Stream& endl(O_Stream& os);


O_Stream& bin(O_Stream& os);


O_Stream& oct(O_Stream& os);


O_Stream& dec(O_Stream& os);


O_Stream& hex(O_Stream& os);
