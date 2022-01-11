#ifndef BD_XJSON_STACK_H
#define BD_XJSON_STACK_H

#define bd_xjson_stack(__type) \
struct \
{ \
    uint64_t top; \
    uint64_t capacity; \
    uint64_t size; \
    __type* data; \
}

#define bd_xjson_stack_init(__stk, __capacity) \
do{ \
    __stk.top = __capacity; \
    __stk.capacity = __capacity; \
    __stk.size = 0; \
    __stk.data = xzmalloc((__capacity)*sizeof(*(__stk.data))); \
} while(0)

#define bd_xjson_stack_top(__stk) \
    (__stk.data[__stk.top])

#define bd_xjson_stack_push(__stk, __ele) \
do{ \
    __stk.top = (__stk.top + 1) % (__stk.capacity + 1); \
    if(__stk.top > __stk.capacity >> 1) \
    { \
        __stk.data = xrealloc(__stk.data, (__stk.capacity<<1)*sizeof(*(__stk.data))); \
        __stk.capacity = __stk.capacity<<1; \
    } \
    __stk.data[__stk.top] = __ele; \
    __stk.size += 1; \
} while(0)

#define bd_xjson_stack_pop(__stk) \
do{ \
    __stk.top = __stk.capacity - ((__stk.capacity - __stk.top + 1) % (__stk.capacity + 1)); \
    __stk.size -= 1; \
} while(0)

#define bd_xjson_stack_pop_all(__stk) \
do{ \
    memset(__stk.data, 0, (__stk.capacity)*sizeof(*(__stk.data))); \
    __stk.top = __stk.capacity; \
    __stk.size = 0; \
} while(0)

#define bd_xjson_stack_clear(__stk) \
do{ \
    __stk.top = 0; \
    __stk.capacity = 0; \
    __stk.size = 0; \
    xfree(__stk.data); \
} while(0)

#define bd_xjson_stack_empty(__stk) \
    (__stk.top == __stk.capacity)

#endif