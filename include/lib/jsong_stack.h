#ifndef JSONG_STACK_H
#define JSONG_STACK_H

#if 1
#define jsong_stack(__type) \
struct \
{ \
    int top; \
    int capacity; \
    int size; \
    __type* data; \
}

#define jsong_stack_init(__stk, __capacity) \
do{ \
    (__stk).capacity = __capacity; \
    (__stk).top = -1; \
    (__stk).size = 0; \
    (__stk).data = xmallocz((__capacity)*sizeof(*((__stk).data))); \
} while(0)

#define jsong_stack_top(__stk) \
    ((__stk).data[(__stk).top])

#define jsong_stack_push(__stk, __ele) \
do{ \
    (__stk).top += 1; \
    if((__stk).top > (__stk).capacity >> 1) \
    { \
        (__stk).data = xreallocz( \
            (__stk).data, \
            ((__stk).capacity)*sizeof(*((__stk).data)), \
            ((__stk).capacity<<1)*sizeof(*((__stk).data)) \
        ); \
        (__stk).capacity = (__stk).capacity<<1; \
    } \
    (__stk).data[(__stk).top] = (__ele); \
    (__stk).size += 1; \
} while(0)

#define jsong_stack_pop(__stk) \
do{ \
    memset(&(jsong_stack_top(__stk)), 0, sizeof(*((__stk).data))); \
    (__stk).top -= 1; \
    (__stk).size -= 1; \
} while(0)

#define jsong_stack_pop_block(__stk, __count) \
do{ \
    memset(&((__stk).data[(__stk).top + 1 - (__count)]), 0, (__count)*sizeof(*((__stk).data))); \
    (__stk).top -= (__count); \
    (__stk).size -= (__count); \
} while(0)

#define jsong_stack_pop2_old_top(__stk, __old_top) \
do{ \
    memset(&((__stk).data[(__stk).top + 1 - ((__stk).top - (__old_top))]), \
        0, \
        ((__stk).top - (__old_top))*sizeof(*((__stk).data))); \
    (__stk).top -= ((__stk).top - (__old_top)); \
    (__stk).size -= ((__stk).top - (__old_top)); \
} while(0)

#define jsong_stack_pop_all(__stk) \
do{ \
    memset((__stk).data, 0, ((__stk).capacity)*sizeof(*((__stk).data))); \
    (__stk).top = -1; \
    (__stk).size = 0; \
} while(0)

#define jsong_stack_clear(__stk) \
do{ \
    (__stk).top = -1; \
    (__stk).capacity = 0; \
    (__stk).size = 0; \
    xfree((__stk).data); \
} while(0)

#define jsong_stack_empty(__stk) \
    ((__stk).top == -1)

#elif
#define jsong_stack(__type) \
struct \
{ \
    uint64_t top; \
    uint64_t capacity; \
    uint64_t size; \
    __type* data; \
}

#define jsong_stack_init(__stk, __capacity) \
do{ \
    (__stk).top = __capacity; \
    (__stk).capacity = __capacity; \
    (__stk).size = 0; \
    (__stk).data = xmallocz((__capacity + 1)*sizeof(*((__stk).data))); \
} while(0)

#define jsong_stack_top(__stk) \
    ((__stk).data[(__stk).top])

#define jsong_stack_next_index(__stk, __index) \
    ((__index + 1)%((__stk).capacity + 1))

#define jsong_stack_push(__stk, __ele) \
do{ \
    (__stk).top = ((__stk).top + 1) % ((__stk).capacity + 1); \
    if((__stk).top > (__stk).capacity >> 1) \
    { \
        (__stk).data = xrealloc((__stk).data, (((__stk).capacity<<1) + 1)*sizeof(*((__stk).data))); \
        (__stk).capacity = (__stk).capacity<<1; \
    } \
    (__stk).data[(__stk).top] = __ele; \
    (__stk).size += 1; \
} while(0)

#define jsong_stack_pop(__stk) \
do{ \
    memset(&(jsong_stack_top(__stk)), 0, sizeof(*((__stk).data))); \
    (__stk).top = (__stk).capacity - (((__stk).capacity - (__stk).top + 1) % ((__stk).capacity + 1)); \
    __stk.size -= 1; \
} while(0)

#define jsong_stack_pop_block(__stk, __count) \
do{ \
    memset(&((__stk).data[(__stk).top + 1 - __count]), 0, __count*sizeof(*((__stk).data))); \
    (__stk).top = (__stk).capacity - (((__stk).capacity - (__stk).top + __count) % ((__stk).capacity + 1)); \
    (__stk).size -= __count; \
} while(0)

#define jsong_stack_pop_restore(__stk, __old_stk_top) \
do{ \
    uint64_t __count = (__stk).top + 1 - (__old_stk_top + 1) % ((__stk).capacity + 1); \
    memset(&((__stk).data[(__stk).top + 1 - __count]), 0, __count*sizeof(*((__stk).data))); \
    (__stk).top = (__stk).capacity - (((__stk).capacity - (__stk).top + __count) % ((__stk).capacity + 1)); \
    (__stk).size -= __count; \
} while(0)

#define jsong_stack_pop_all(__stk) \
do{ \
    memset((__stk).data, 0, ((__stk).capacity)*sizeof(*((__stk).data))); \
    (__stk).top = (__stk).capacity; \
    (__stk).size = 0; \
} while(0)

#define jsong_stack_clear(__stk) \
do{ \
    (__stk).top = 0; \
    (__stk).capacity = 0; \
    (__stk).size = 0; \
    xfree((__stk).data); \
} while(0)

#define jsong_stack_empty(__stk) \
    ((__stk).top == (__stk).capacity)

#endif

#endif