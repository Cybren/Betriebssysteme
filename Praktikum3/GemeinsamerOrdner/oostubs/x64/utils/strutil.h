#pragma once

#include "types.h"

extern "C" {

char *strchrnul(const char *s, int c);
char *strchr(const char *s, int c);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);

}

