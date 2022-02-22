#include <stdarg.h>
#include <string.h>
#include "lib/utils.h"

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