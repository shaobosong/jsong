#ifndef BD_XJSON_H
#define BD_XJSON_H

#include "lib/bd_xjson_type.h"

typedef struct bd_xjson bd_xjson;

/*
 *  bd_xjson class
 *      All classes in bd_xjson_impl.h are based on it.
 *
 *  @data: data of json
 *      object:    bd_xjson_htab*
 *      string:    char*
 *      number:    int*
 *      array:     bd_xjson_list*
 *      true:      void*(0)
 *      false:     void*(0)
 *      null:      void*(0)
 *  @type: type of json
 *      object:    BD_XJSON_OBJECT
 *      string:    BD_XJSON_STRING
 *      number:    BD_XJSON_NUMBER
 *      array:     BD_XJSON_ARRAY
 *      true:      BD_XJSON_TRUE
 *      false:     BD_XJSON_FALSE
 *      null:      BD_XJSON_NULL
 */
#define bd_xjson(__class) \
struct __class            \
{                         \
    void* data;           \
    bd_xjson_type type;   \
}
bd_xjson(bd_xjson);

int bd_xjson_reassign(void* dst, const void* src);
int bd_xjson_copy(void* dst, const void* src);
int bd_xjson_free_data(bd_xjson* json);
int bd_xjson_free(void* json);

void bd_xjson_stringify(const void* json, char** pstr, int* plen);
int bd_xjson_parse(const char* str, void* json);
#endif