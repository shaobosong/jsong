#ifndef LIB_UTILS_H
#define LIB_UTILS_H

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

char* m_strcat(char* str, ...);

#endif