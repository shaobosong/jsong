#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "lib/utils.h"
#include "lib/config.h"

/* must place a NULL-Terminated trailing */
char* xmstrcat(char *str, ...)
{
    const char *el;
    va_list ap;

    va_start(ap, str);
    while(!!(el = va_arg(ap, const char*)))
        strcat(str, el);
    va_end(ap);

    return str;
}

#ifdef CONFIG_LOG
static FILE *log_file;
static void log_vprintf(const char *fmt, va_list ap)
{
    if(!log_file)
        log_file = fopen("/tmp/bd_xjson.log", "wb");
    vfprintf(log_file, fmt, ap);
}
#else
static void log_vprintf(const char *fmt, va_list ap)
{
    vfprintf(stderr, fmt, ap);
}
#endif

void __attribute__((format(printf, 1, 2), unused))
log_printf(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log_vprintf(fmt, ap);
    va_end(ap);
}

/* zeroing each element */
void *xmallocz(int size)
{
    void *ptr;
    ptr = malloc(size);
    if(size != 0)
    {
        assert(ptr != NULL);
    }
    memset(ptr, 0, size);
    return (ptr);
}

/* zeroing new elements */
void *xreallocz(void *ptr, int old_size, int new_size)
{
    ptr = realloc(ptr, new_size);
    if(new_size != 0)
    {
        assert(ptr != NULL);
    }
    if(new_size > old_size)
        memset(ptr + old_size, 0, new_size - old_size);
    return (ptr);
}

void xfree(void *ptr)
{
    assert(ptr != NULL);
    free(ptr);
    return ;
}