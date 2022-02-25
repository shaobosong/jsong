#ifndef LIB_UTILS_H
#define LIB_UTILS_H

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

char* m_strcat(char* str, ...);
char* get_time();
/* log */
void __attribute__((format(printf, 1, 2), unused)) log_printf(const char* fmt, ...);
#define THROW_WARNING(msg) \
    log_printf("[%s] %s:%d: [----%s----]: "msg, \
        get_time(), __FILE__, __LINE__, __func__)

void* xzmalloc(int size);
void* xrealloc(void* ptr, int size);
void xfree(void* ptr);

#endif