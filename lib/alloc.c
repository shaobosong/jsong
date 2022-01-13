#include <stdlib.h>
#include <string.h>

#include "lib/alloc.h"

void* xzmalloc(int s)
{
    void* p;
    p = malloc(s);
    MY_ASSERT(s != 0 && p != NULL);
    memset(p, 0, s);
    return (p);
}

void* xrealloc(void* p, int s)
{
    p = realloc(p, s);
    MY_ASSERT(s != 0 && p != NULL);
    return (p);
}

void xfree(void* p)
{
    MY_ASSERT(p != NULL);
    free(p);
    return ;
}