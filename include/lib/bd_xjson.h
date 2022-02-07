#ifndef bd_xjson_base_h
#define bd_xjson_base_h

#include "lib/bd_xjson_type.h"

typedef struct bd_xjson bd_xjson;

/*
 *  bd_xjson class
 *      All classes in bd_xjson.h are based on it and can be cast to it.
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

int bd_xjson_copy(bd_xjson* dest, const bd_xjson* src);
int bd_xjson_free(bd_xjson* json);

void bd_xjson_stringify(const void* json, char** pstr, int* plen);
int bd_xjson_parse(const char* str, void* json);
#endif