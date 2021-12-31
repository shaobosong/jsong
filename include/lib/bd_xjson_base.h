#ifndef bd_xjson_base_h
#define bd_xjson_base_h

#include "lib/bd_xjson_type.h"

typedef enum bd_xjson_type bd_xjson_type;
typedef struct bd_xjson bd_xjson;

/*
 *  bd_xjson class
 *      All classes are based on it and can be cast to it.
 *
 *  @data: data of json
 *  @type: type of json
 */
struct bd_xjson
{
/* protected */
    void* data;
    bd_xjson_type type;
};
int bd_xjson_copy(bd_xjson* dest, bd_xjson* src);
int bd_xjson_free(bd_xjson* json);

#endif