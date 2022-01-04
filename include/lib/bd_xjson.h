#ifndef bd_xjson_h
#define bd_xjson_h

#include <stddef.h>

#include "lib/bd_xjson_base.h"
#include "lib/alloc.h"

typedef struct bd_xjson_object bd_xjson_object;
typedef struct bd_xjson_string bd_xjson_string;
typedef struct bd_xjson_number bd_xjson_number;
typedef struct bd_xjson_array bd_xjson_array;
typedef struct bd_xjson_true bd_xjson_true;
typedef struct bd_xjson_false bd_xjson_false;
typedef struct bd_xjson_null bd_xjson_null;

#define BD_XJSON(type, p) \
    type* p = xzmalloc(sizeof *p)

/*
 *  bd_xjson_object class
 *
 *  @add: add a <key>-<val@bd_json_*> pair by a given <key> and <val@bd_json_*>
 *  @delete: delete a <key>-<val@bd_json_*> pair by a given <key>
 *  @search: search <val@bd_json_*> by a given <key>
 *  @update: update <val@bd_json_*> by a given <key>
 */
/* constructor */
void obj_default_cstr(bd_xjson_object* this);
void obj_copy_cstr(bd_xjson_object* this, bd_xjson_object* obj);
/* destructor */
void obj_default_dstr(bd_xjson_object* this);
/* member functions */
void obj_add(bd_xjson_object* obj, const char* key, void* val);
void obj_delete(bd_xjson_object* obj, const char* key);
void obj_search(bd_xjson_object* obj, const char* key, void* val);
void obj_update(bd_xjson_object* obj, const char* key, void* val);
struct bd_xjson_object
{
/* parent class */
    bd_xjson base;
/* public */
    /* member functions */
    void (*add)(bd_xjson_object* this, const char* key, void* val);
    void (*delete)(bd_xjson_object* this, const char* key);
    void (*search)(bd_xjson_object* this, const char* key, void* val);
    void (*update)(bd_xjson_object* this, const char* key, void* val);
};
#define BD_XJSON_OBJECT_CLASS(p)    \
    BD_XJSON(bd_xjson_object, p);   \
    p->base.type = BD_XJSON_OBJECT; \
    p->base.data = NULL;            \
    p->add = obj_add;               \
    p->delete = obj_delete;         \
    p->search = obj_search;         \
    p->update = obj_update


/*
 *  bd_xjson_string class
 */
/* constructor */
void str_default_cstr(bd_xjson_string* this);
void str_copy_cstr(bd_xjson_string* this, bd_xjson_string* str);
void str_assign_cstr(bd_xjson_string* this, const char* chars);
/* destructor */
void str_default_dstr(bd_xjson_string* this);
struct bd_xjson_string
{
/* parent class */
    bd_xjson base;
};
#define BD_XJSON_STRING_CLASS(p)    \
    BD_XJSON(bd_xjson_string, p);   \
    p->base.type = BD_XJSON_STRING; \
    p->base.data = NULL


/*
 *  bd_xjson_number class
 */
/* constructor */
void num_default_cstr(bd_xjson_number* this);
void num_copy_cstr(bd_xjson_number* this, bd_xjson_number* num);
void num_assign_cstr(bd_xjson_number* this, int val);
/* destructor */
void num_default_dstr(bd_xjson_number* this);
struct bd_xjson_number
{
/* parent class */
    bd_xjson base;
};
#define BD_XJSON_NUMBER_CLASS(p)    \
    BD_XJSON(bd_xjson_number, p);   \
    p->base.type = BD_XJSON_NUMBER; \
    p->base.data = NULL


/*
 *  bd_xjson_array class
 *
 *  @add: add a <val@bd_json_*> by a given <pos>
 *  @delete: delete a <val@bd_json_*> by a given <pos>
 *  @search: search <val@bd_json_*> by a given <pos>
 *  @update: update <val@bd_json_*> by a given <pos>
 */
/* constructor */
void arr_default_cstr(bd_xjson_array* this);
void arr_copy_cstr(bd_xjson_array* this, bd_xjson_array* arr);
/* desctrctor */
void arr_default_dstr(bd_xjson_array* this);
/* member functions */
void arr_add(bd_xjson_array* arr, int pos, void* val);
void arr_delete(bd_xjson_array* arr, int pos);
void arr_search(bd_xjson_array* arr, int pos, void* val);
void arr_update(bd_xjson_array* arr, int pos, void* val);
struct bd_xjson_array
{
/* parent class */
    bd_xjson base; /* a linked list */
/* public */
    /* member functions */
    void (*add)(bd_xjson_array* this, int pos, void* val);
    void (*delete)(bd_xjson_array* this, int pos);
    void (*search)(bd_xjson_array* this, int pos, void* val);
    void (*update)(bd_xjson_array* this, int pos, void* val);
};
#define BD_XJSON_ARRAY_CLASS(p)    \
    BD_XJSON(bd_xjson_array, p);   \
    p->base.type = BD_XJSON_ARRAY; \
    p->base.data = NULL;           \
    p->add = arr_add;              \
    p->delete = arr_delete;        \
    p->search = arr_search;        \
    p->update = arr_update


/*
 *  bd_xjson_true class
 */
struct bd_xjson_true
{
/* parent class */
    bd_xjson base;
};
#define BD_XJSON_TRUE_CLASS(p)     \
    BD_XJSON(bd_xjson_true, p);    \
    P->base.type = BD_XJSON_TRUE;  \
    p->base.data = NULL


/*
 *  bd_xjson_false class
 */
struct bd_xjson_false
{
/* parent class */
    bd_xjson base;
};
#define BD_XJSON_FALSE_CLASS(p)    \
    BD_XJSON(bd_xjson_false, p);   \
    p->base.type = BD_XJSON_FALSE; \
    p->base.data = NULL


/*
 *  bd_xjson_null class
 */
struct bd_xjson_null
{
/* parent class */
    bd_xjson base;
};
#define BD_XJSON_NULL_CLASS(p)     \
    BD_XJSON(bd_xjson_null, p);    \
    P->base.type = BD_XJSON_NULL;  \
    p->base.data = NULL


#endif
