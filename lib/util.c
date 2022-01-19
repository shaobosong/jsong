#include <stdarg.h>
#include "lib/util.h"

char* mstrcat(char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    const char* a;
    int t = 0;
    while(s[t])
    {
        ++t;
    }
    while(*(a = va_arg(ap, const char*)))
    {
        int i = 0;
        while(a[i])
        {
            s[t] = a[i];
            ++t;
            ++i;
        }
    }
    s[t] = '\0';
    va_end(ap);
    return s;
}