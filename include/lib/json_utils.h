#ifndef LIB_UTILS_H
#define LIB_UTILS_H

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#ifndef extract32
#define extract32(value, start, length) \
    (((value) >> (start)) & (~0U >> (32 - (length))))
#endif

/* Apend multi strings onto dest */
char* json_xmstrcat(char* str, ...);

/* log */
void __attribute__((format(printf, 1, 2), unused)) json_log_printf(const char* fmt, ...);
#define THROW_WARNING(msg) \
    json_log_printf("%s: %d: %s: "msg, \
        __FILE__, __LINE__, __func__)

/* Memory allocation and free */
void *json_xmallocz(int size);
void *json_xreallocz(void *ptr, int old_size, int new_size);
void json_xfree(void *ptr);

#endif