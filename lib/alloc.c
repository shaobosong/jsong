#include <stdlib.h>
#include <string.h>

#include "lib/alloc.h"

void* xzmalloc(int s)
{
    void* p;
    p = malloc(s);
    if(!p && s != 0)
    {
        THROW_XZMALLOC_EXCEPTION;
    }
    memset(p, 0, s);
    return (p);
}

void* xrealloc(void* p, int s)
{
    p = realloc(p, s);
    if(!p && s != 0)
    {
        THROW_XREALLOC_EXCEPTION;
    }
    return (p);
}

void xfree(void* p)
{
    if(!p)
    {
        THROW_XFREE_EXCEPTION;
    }
    free(p);
    return ;
}