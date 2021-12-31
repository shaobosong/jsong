#ifndef bd_xjson_api_h
#define bd_xjson_api_h

#include "lib/bd_xjson.h"

/* private */
#define BD_XJSON(type, p)                          \
    type* p = xzmalloc(sizeof *p)
/* public */
#define FREE_BD_XJSON(p)                           \
    switch(p->data.type)                           \
    {                                              \
        case BD_XJSON_OBJECT:                      \
        /* to do */                                \
            break;                                 \
        case BD_XJSON_STRING:                      \
            str_default_dstr((bd_xjson_string*)p); \
            break;                                 \
        case BD_XJSON_NUMBER:                      \
            num_default_dstr((bd_xjson_number*)p); \
            break;                                 \
        case BD_XJSON_ARRAY:                       \
            arr_default_dstr((bd_xjson_array*)p);  \
            break;                                 \
        default:;                                  \
    }                                              \
    xfree(p)


/*
 *  bd_xjson_object class
 */
/* to do */


/*
 *  bd_xjson_string class
 */
/* private */
#define BD_XJSON_STRING_CLASS(p)    \
    BD_XJSON(bd_xjson_string, p);   \
    p->data.type = BD_XJSON_STRING; \
    p->data.data = NULL
/* public */
#define BD_XJSON_STRING(p)          \
    BD_XJSON_STRING_CLASS(p);       \
    str_default_cstr(p)
#define BD_XJSON_STRING_COPY(p,c)   \
    BD_XJSON_STRING_CLASS(p);       \
    str_copy_cstr(p,c)
#define BD_XJSON_STRING_ASSIGN(p,c) \
    BD_XJSON_STRING_CLASS(p);       \
    str_assign_cstr(p,c)


/*
 *  bd_xjson_number class
 */
/* private */
#define BD_XJSON_NUMBER_CLASS(p)    \
    BD_XJSON(bd_xjson_number, p);   \
    p->data.type = BD_XJSON_NUMBER; \
    p->data.data = NULL
/* public */
#define BD_XJSON_NUMBER(p)          \
    BD_XJSON_NUMBER_CLASS(p);       \
    num_default_cstr(p)
#define BD_XJSON_NUMBER_COPY(p,c)   \
    BD_XJSON_NUMBER_CLASS(p);       \
    num_copy_cstr(p,c)
#define BD_XJSON_NUMBER_ASSIGN(p,c) \
    BD_XJSON_NUMBER_CLASS(p);       \
    num_assign_cstr(p,c)


/*
 *  bd_xjson_array class
 */
/* private */
#define BD_XJSON_ARRAY_CLASS(p)    \
    BD_XJSON(bd_xjson_array, p);   \
    p->data.type = BD_XJSON_ARRAY; \
    p->data.data = NULL;           \
    p->add = arr_add;              \
    p->delete = arr_delete;        \
    p->search = arr_search;        \
    p->update = arr_update
/* public */
#define BD_XJSON_ARRAY(p)          \
    BD_XJSON_ARRAY_CLASS(p);       \
    arr_default_cstr(p)
#define BD_XJSON_ARRAY_COPY(p,c)   \
    BD_XJSON_ARRAY_CLASS(p);       \
    arr_copy_cstr(p,c)


/*
 *  bd_xjson_true class
 */
/* private */
#define BD_XJSON_TRUE_CLASS(p)     \
    BD_XJSON(bd_xjson_true, p);    \
    P->data.type = BD_XJSON_TRUE;  \
    p->data.data = NULL
/* public */
#define BD_XJSON_TRUE(p)           \
    BD_XJSON_TRUE_CLASS(p)


/*
 *  bd_xjson_false class
 */
/* private */
#define BD_XJSON_FALSE_CLASS(p)    \
    BD_XJSON(bd_xjson_false, p);   \
    p->data.type = BD_XJSON_FALSE; \
    p->data.data = NULL
/* public */
#define BD_XJSON_FALSE(p)          \
    BD_XJSON_FALSE_CLASS(p)


/*
 *  bd_xjson_null class
 */
/* private */
#define BD_XJSON_NULL_CLASS(p)     \
    BD_XJSON(bd_xjson_null, p);    \
    P->data.type = BD_XJSON_NULL;  \
    p->data.data = NULL
/* public */
#define BD_XJSON_NULL(p)           \
    BD_XJSON_NULL_CLASS(p)

#endif