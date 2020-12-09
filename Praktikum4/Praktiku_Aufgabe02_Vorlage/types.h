#pragma once

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned long long uintptr_t;

typedef __SIZE_TYPE__      size_t;

typedef char               int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef long long          intptr_t;

typedef long int           ssize_t;
#define SSIZE_MAX 9223372036854775807ULL

typedef __PTRDIFF_TYPE__   ptrdiff_t;

#ifndef NULL
#define NULL 0
#endif


#include "debug/assert.h"
assert_size(int8_t, 1);
assert_size(int16_t, 2);
assert_size(int32_t, 4);
assert_size(int64_t, 8);
assert_size(intptr_t, sizeof(void*));
assert_size(uint8_t, 1);
assert_size(uint16_t, 2);
assert_size(uint32_t, 4);
assert_size(uint64_t, 8);
assert_size(uintptr_t, sizeof(void*));
