


#pragma once


#ifdef VERBOSE
#define DBG_VERBOSE DBG
#else
#define DBG_VERBOSE nullstream

#include "debug/null_stream.h"

#endif


#define DBG dout

#include "device/cgastr.h"


extern CGA_Stream dout;

