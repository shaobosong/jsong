#ifndef bd_xjson_api_h
#define bd_xjson_api_h

#include "lib/bd_xjson_alloc.h"

#define FREE_JSON(p)   \
do                     \
{                      \
    FREE_JSON_DATA(p); \
    xfree(p);          \
} while (0)


/*
 *  JSON_OBJECT
 */
#define JSON_OBJECT(__id) \
    bd_xjson_object* __id = for_json_object()
#define JSON_OBJECT_COPY(__id, __copy) \
    bd_xjson_object* __id = for_json_object_copy(__copy)


/*
 *  JSON_STRING
 */
#define JSON_STRING(__id, __chars) \
    bd_xjson_string* __id = for_json_string(__chars)


/*
 *  JSON_NUMBER
 */
#define JSON_NUMBER(__id, __int) \
    bd_xjson_number* __id = for_json_number(__int)


/*
 *  JSON_ARRAY
 */

#define JSON_ARRAY(__id) \
    bd_xjson_array* __id = for_json_array()
#define JSON_ARRAY_COPY(__id, __copy) \
    bd_xjson_array* __id = for_json_array_copy(__copy)


/*
 *  JSON_TRUE
 */
#define JSON_TRUE(__id) \
    bd_xjson_true* __id = (bd_xjson_true*)for_json_literal(BD_XJSON_TRUE)


/*
 *  JSON_FALSE
 */
#define JSON_FALSE(__id) \
    bd_xjson_false* __id = (bd_xjson_false*)for_json_literal(BD_XJSON_FALSE)


/*
 *  JSON_NULL
 */
#define JSON_NULL(__id) \
    bd_xjson_null* __id = (bd_xjson_null*)for_json_literal(BD_XJSON_NULL)

#endif