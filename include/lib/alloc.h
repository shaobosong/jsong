#ifndef lib_alloc_h
#define lib_alloc_h

#include "lib/error.h"

void* xzmalloc(int size);
void* xrealloc(void* ptr, int size);
void xfree(void* ptr);

#endif