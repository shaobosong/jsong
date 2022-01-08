#ifndef BD_XJSON_ITER_H
#define BD_XJSON_ITER_H

#define bd_xjson_iter(__type) \
struct \
{ \
    uint64_t index; \
    __type data; \
}

#endif