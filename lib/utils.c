#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "lib/utils.h"
#include "lib/config.h"

char time_buff[64];

/* must place a NULL-Terminated trailing, i.e. '\0' */
char* m_strcat(char* str, ...)
{
    const char* el;
    va_list ap;

    va_start(ap, str);
    while(*(el = va_arg(ap, const char*)))
        strcat(str, el);
    va_end(ap);

    return str;
}

char* get_time()
{
    struct timespec ts;
    struct tm tm_time;

    /* get current time */
    time(&ts.tv_sec);

    /* get time string */
    localtime_r(&ts.tv_sec, &tm_time);
    strftime(time_buff, 64, TIME_FORMAT, &tm_time);

    return time_buff;
}

#ifdef CONFIG_LOG
static FILE *log_file;
static void log_vprintf(const char* fmt, va_list ap)
{

    if(!log_file)
        log_file = fopen("/tmp/bd_xjson.log", "wb");
    vfprintf(log_file, fmt, ap);
}
#else
static void log_vprintf(const char* fmt, va_list ap)
{
    vprintf(fmt, ap);
}
#endif

void __attribute__((format(printf, 1, 2), unused)) log_printf(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log_vprintf(fmt, ap);
    va_end(ap);
}

void* xzmalloc(int s)
{
    void* p;
    p = malloc(s);
    if(s != 0)
    {
        assert(p != NULL);
    }
    memset(p, 0, s);
    return (p);
}

void* xrealloc(void* p, int s)
{
    p = realloc(p, s);
    if(s != 0)
    {
        assert(p != NULL);
    }
    return (p);
}

void xfree(void* p)
{
    assert(p != NULL);
    free(p);
    return ;
}