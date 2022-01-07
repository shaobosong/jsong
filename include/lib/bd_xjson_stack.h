#ifndef BD_XJSON_STACK_H
#define BD_XJSON_STACK_H

#define bd_xjson_stack(type) \
struct \
{ \
    int64_t top; \
    int64_t size; \
    type* data; \
}

#define bd_xjson_stack_init(__stk, __size) \
do{ \
    __stk.top = -1; \
    __stk.size = __size; \
    __stk.data = xzmalloc((__size)*sizeof(*__stk.data)); \
} while(0)

#define bd_xjson_stack_top(__stk) \
    (__stk.data[__stk.top])

#define bd_xjson_stack_push(__stk, __ele) \
do{ \
    __stk.data[__stk.top + 1] = __ele; \
    __stk.top += 1; \
} while(0)

#define bd_xjson_stack_pop(__stk) \
do{ \
    __stk.top -= 1; \
} while(0)

#define bd_xjson_stack_clear(__stk) \
do{ \
    __stk.top = -1; \
    __stk.size = 0; \
    xfree(__stk.data); \
} while(0)

#define bd_xjson_stack_empty(__stk) \
    (__stk.top == -1)

#endif