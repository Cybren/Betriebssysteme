#pragma once

#include "object/o_stream.h"
#include "machine/cga_screen.h"


class CGA_Stream
	: public O_Stream, public CGA_Screen
{

	CGA_Stream(const CGA_Stream&)            = delete;
	CGA_Stream& operator=(const CGA_Stream&) = delete;
public:

	CGA_Stream(int from_col, int to_col, int from_row, int to_row,
			   bool use_cursor = false)
		: O_Stream(), CGA_Screen (from_col, to_col, from_row, to_row, use_cursor)
	{
		flush();
	}


	void flush();
};
