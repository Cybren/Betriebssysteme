

#pragma once

#ifndef STRINGIFY

#define STRINGIFY(S) #S
#endif


#define assert_size(TYPE, SIZE) \
	static_assert(sizeof(TYPE) == (SIZE), "Wrong size for " STRINGIFY(TYPE))


#ifdef NDEBUG
#define assert(EXP)
#else
#define assert(EXP) \
	do { \
		if (__builtin_expect(!(EXP), 0)) { \
			assertion_failed(STRINGIFY(EXP), __func__, __FILE__, __LINE__); \
		} \
	} while(0)


[[noreturn]] void assertion_failed(const char * exp, const char * func, const char * file, int line);
#endif
