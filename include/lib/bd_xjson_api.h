#ifndef bd_xjson_api_h
#define bd_xjson_api_h

#include "lib/bd_xjson.h"

#define FREE_JSON(p)                                        \
do                                                          \
{                                                           \
    FREE_JSON_DATA(p);                                      \
    xfree(p);                                               \
} while (0)


/*
 *  JSON_OBJECT
 */
#define JSON_OBJECT(p)          \
    BD_XJSON_OBJECT_CLASS(p);   \
    obj_default_cstr(p)
#define JSON_OBJECT_COPY(p,c)   \
    BD_XJSON_OBJECT_CLASS(p);   \
    obj_copy_cstr(p,c)


/*
 *  JSON_STRING
 */
#define JSON_STRING(p)          \
    BD_XJSON_STRING_CLASS(p);   \
    str_default_cstr(p)
#define JSON_STRING_COPY(p,c)   \
    BD_XJSON_STRING_CLASS(p);   \
    str_copy_cstr(p,c)
#define JSON_STRING_ASSIGN(p,c) \
    BD_XJSON_STRING_CLASS(p);   \
    str_assign_cstr(p,c)


/*
 *  JSON_NUMBER
 */
#define JSON_NUMBER(p)          \
    BD_XJSON_NUMBER_CLASS(p);   \
    num_default_cstr(p)
#define JSON_NUMBER_COPY(p,c)   \
    BD_XJSON_NUMBER_CLASS(p);   \
    num_copy_cstr(p,c)
#define JSON_NUMBER_ASSIGN(p,c) \
    BD_XJSON_NUMBER_CLASS(p);   \
    num_assign_cstr(p,c)


/*
 *  JSON_ARRAY
 */
#define JSON_ARRAY(p)          \
    BD_XJSON_ARRAY_CLASS(p);   \
    arr_default_cstr(p)
#define JSON_ARRAY_COPY(p,c)   \
    BD_XJSON_ARRAY_CLASS(p);   \
    arr_copy_cstr(p,c)


/*
 *  JSON_TRUE
 */
#define JSON_TRUE(p)           \
    BD_XJSON_TRUE_CLASS(p)


/*
 *  JSON_FALSE
 */
#define JSON_FALSE(p)          \
    BD_XJSON_FALSE_CLASS(p)


/*
 *  JSON_NULL
 */
#define JSON_NULL(p)           \
    BD_XJSON_NULL_CLASS(p)

#endif