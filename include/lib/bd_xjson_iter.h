#ifndef BD_XJSON_ITER_H
#define BD_XJSON_ITER_H

#define bd_xjson_iter(__class, __index_type, __data_type) \
struct __class \
{ \
    __index_type index; \
    __data_type data; \
}

#endif