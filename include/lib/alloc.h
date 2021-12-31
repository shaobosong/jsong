#ifndef lib_alloc_h
#define lib_alloc_h

#include "lib/error.h"

#define THROW_XZMALLOC_EXCEPTION \
    THROW_EXCEPTION("allocation failed")
#define THROW_XREALLOC_EXCEPTION \
    THROW_EXCEPTION("reallocation failed")
#define THROW_XFREE_EXCEPTION \
    THROW_EXCEPTION("try to free uninitialized memory or double free")

void* xzmalloc(int size);
void* xrealloc(void* ptr, int size);
void xfree(void* ptr);

#endif