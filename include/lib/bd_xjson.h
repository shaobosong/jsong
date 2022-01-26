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

#define FREE_JSON_DATA(p)                                   \
do                                                          \
{                                                           \
    switch((p)->type)                                       \
    {                                                       \
        case BD_XJSON_OBJECT:                               \
            obj_default_dstr((bd_xjson_object*)(p));        \
            break;                                          \
        case BD_XJSON_STRING:                               \
            str_default_dstr((bd_xjson_string*)(p));        \
            break;                                          \
        case BD_XJSON_NUMBER:                               \
            num_default_dstr((bd_xjson_number*)(p));        \
            break;                                          \
        case BD_XJSON_ARRAY:                                \
            arr_default_dstr((bd_xjson_array*)(p));         \
            break;                                          \
        case BD_XJSON_TRUE:                                 \
        case BD_XJSON_FALSE:                                \
        case BD_XJSON_NULL:                                 \
            break;                                          \
        default:                                            \
            THROW_EXCEPTION("illegal type from the freed"); \
    }                                                       \
} while (0)

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
void obj_add(bd_xjson_object* obj, const char* key, void* val); /* deprecated */
void obj_add_str(bd_xjson_object* obj, const char* key, char* val); /* deprecated */
void obj_add_num(bd_xjson_object* obj, const char* key, int val); /* deprecated */
void obj_add_true(bd_xjson_object* obj, const char* key); /* deprecated */
void obj_add_false(bd_xjson_object* obj, const char* key); /* deprecated */
void obj_add_null(bd_xjson_object* obj, const char* key); /* deprecated */
void obj_delete(bd_xjson_object* obj, const char* key);
void obj_search(bd_xjson_object* obj, const char* key, void* val); /* deprecated */
void obj_update(bd_xjson_object* obj, const char* key, void* val); /* deprecated */
void obj_set(bd_xjson_object* obj, const char* key, void* val);
void obj_set_str(bd_xjson_object* obj, const char* key, const char* val);
void obj_set_num(bd_xjson_object* obj, const char* key, int val);
void obj_set_true(bd_xjson_object* obj, const char* key);
void obj_set_false(bd_xjson_object* obj, const char* key);
void obj_set_null(bd_xjson_object* obj, const char* key);
void obj_get(bd_xjson_object* obj, const char* key, void* val);
char* obj_get_str(bd_xjson_object* obj, const char* key);
int obj_get_num(bd_xjson_object* obj, const char* key);
struct bd_xjson_object
{
/* parent class */
    bd_xjson_base;
/* public */
    /* member functions */
    void (*add)(bd_xjson_object* this, const char* key, void* val); /* deprecated */
    void (*add_str)(bd_xjson_object* this, const char* key, char* val); /* deprecated */
    void (*add_num)(bd_xjson_object* this, const char* key, int val); /* deprecated */
    void (*add_true)(bd_xjson_object* this, const char* key); /* deprecated */
    void (*add_false)(bd_xjson_object* this, const char* key); /* deprecated */
    void (*add_null)(bd_xjson_object* this, const char* key); /* deprecated */
    void (*delete)(bd_xjson_object* this, const char* key);
    void (*search)(bd_xjson_object* this, const char* key, void* val); /* deprecated */
    void (*update)(bd_xjson_object* this, const char* key, void* val); /* deprecated */
    void (*set)(bd_xjson_object* this, const char* key, void* val);
    void (*set_str)(bd_xjson_object* this, const char* key, const char* val);
    void (*set_num)(bd_xjson_object* this, const char* key, int val);
    void (*set_true)(bd_xjson_object* this, const char* key);
    void (*set_false)(bd_xjson_object* this, const char* key);
    void (*set_null)(bd_xjson_object* this, const char* key);
    void (*get)(bd_xjson_object* this, const char* key, void* val);
    char* (*get_str)(bd_xjson_object* this, const char* key);
    int (*get_num)(bd_xjson_object* this, const char* key);
};
#define BD_XJSON_OBJECT_CLASS(__ptr)    \
do                                      \
{                                       \
    (__ptr)->type = BD_XJSON_OBJECT;    \
    (__ptr)->data = NULL;               \
    (__ptr)->add = obj_add;             \
    (__ptr)->add_str = obj_add_str;     \
    (__ptr)->add_num = obj_add_num;     \
    (__ptr)->add_true = obj_add_true;   \
    (__ptr)->add_false = obj_add_false; \
    (__ptr)->add_null = obj_add_null;   \
    (__ptr)->delete = obj_delete;       \
    (__ptr)->search = obj_search;       \
    (__ptr)->update = obj_update;       \
    (__ptr)->set = obj_set;             \
    (__ptr)->set_str = obj_set_str;     \
    (__ptr)->set_num = obj_set_num;     \
    (__ptr)->set_true = obj_set_true;   \
    (__ptr)->set_false = obj_set_false; \
    (__ptr)->set_null = obj_set_null;   \
    (__ptr)->get = obj_get;             \
    (__ptr)->get_str = obj_get_str;     \
    (__ptr)->get_num = obj_get_num;     \
} while(0)


/*
 *  bd_xjson_string class
 */
/* constructor */
void str_default_cstr(bd_xjson_string* this); /* deprecated */
void str_copy_cstr(bd_xjson_string* this, bd_xjson_string* str); /* deprecated */
void str_assign_cstr(bd_xjson_string* this, const char* val);
/* destructor */
void str_default_dstr(bd_xjson_string* this);
/* member functions */
void str_set(bd_xjson_string* str, char* val);
char* str_get(bd_xjson_string* str);
struct bd_xjson_string
{
/* parent class */
    bd_xjson_base;
    void (*set)(bd_xjson_string* this, char* val);
    char* (*get)(bd_xjson_string* this);
};
#define BD_XJSON_STRING_CLASS(__ptr) \
do                                   \
{                                    \
    (__ptr)->type = BD_XJSON_STRING; \
    (__ptr)->data = NULL;            \
    (__ptr)->set = str_set;          \
    (__ptr)->get = str_get;          \
} while(0)


/*
 *  bd_xjson_number class
 */
/* constructor */
void num_default_cstr(bd_xjson_number* this); /* deprecated */
void num_copy_cstr(bd_xjson_number* this, bd_xjson_number* num); /* deprecated */
void num_assign_cstr(bd_xjson_number* this, int val);
/* destructor */
void num_default_dstr(bd_xjson_number* this);
/* member functions */
void num_set(bd_xjson_number* num, int val);
int num_get(bd_xjson_number* num);
struct bd_xjson_number
{
/* parent class */
    bd_xjson_base;
    void (*set)(bd_xjson_number* this, int val);
    int (*get)(bd_xjson_number* this);
};
#define BD_XJSON_NUMBER_CLASS(__ptr) \
do                                   \
{                                    \
    (__ptr)->type = BD_XJSON_NUMBER; \
    (__ptr)->data = NULL;            \
    (__ptr)->set = num_set;          \
    (__ptr)->get = num_get;          \
} while(0)


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
void arr_add(bd_xjson_array* arr, int pos, void* val); /* deprecated */
void arr_add_str(bd_xjson_array* arr, int pos, char* val); /* deprecated */
void arr_add_num(bd_xjson_array* arr, int pos, int val); /* deprecated */
void arr_add_true(bd_xjson_array* arr, int pos); /* deprecated */
void arr_add_false(bd_xjson_array* arr, int pos); /* deprecated */
void arr_add_null(bd_xjson_array* arr, int pos); /* deprecated */
void arr_delete(bd_xjson_array* arr, int pos);
void arr_search(bd_xjson_array* arr, int pos, void* val); /* deprecated */
void arr_update(bd_xjson_array* arr, int pos, void* val); /* deprecated */
void arr_set(bd_xjson_array* arr, int pos, void* val);
void arr_set_str(bd_xjson_array* arr, int pos, char* val);
void arr_set_num(bd_xjson_array* arr, int pos, int val);
void arr_set_true(bd_xjson_array* arr, int pos);
void arr_set_false(bd_xjson_array* arr, int pos);
void arr_set_null(bd_xjson_array* arr, int pos);
void arr_get(bd_xjson_array* arr, int pos, void* val);
char* arr_get_str(bd_xjson_array* arr, int pos);
int arr_get_num(bd_xjson_array* arr, int pos);
void arr_qsort(bd_xjson_array* arr, int (*compare_fn)(const void*, const void*));
struct bd_xjson_array
{
/* parent class */
    bd_xjson_base; /* a linked list */
/* public */
    /* member functions */
    void (*add)(bd_xjson_array* this, int pos, void* val); /* deprecated */
    void (*add_str)(bd_xjson_array* this, int pos, char* val); /* deprecated */
    void (*add_num)(bd_xjson_array* this, int pos, int val); /* deprecated */
    void (*add_true)(bd_xjson_array* this, int pos); /* deprecated */
    void (*add_false)(bd_xjson_array* this, int pos); /* deprecated */
    void (*add_null)(bd_xjson_array* this, int pos); /* deprecated */
    void (*delete)(bd_xjson_array* this, int pos);
    void (*search)(bd_xjson_array* this, int pos, void* val); /* deprecated */
    void (*update)(bd_xjson_array* this, int pos, void* val); /* deprecated */
    void (*set)(bd_xjson_array* this, int pos, void* val);
    void (*set_str)(bd_xjson_array* this, int pos, char* val);
    void (*set_num)(bd_xjson_array* this, int pos, int val);
    void (*set_true)(bd_xjson_array* this, int pos);
    void (*set_false)(bd_xjson_array* this, int pos);
    void (*set_null)(bd_xjson_array* this, int pos);
    void (*get)(bd_xjson_array* this, int pos, void* val);
    char* (*get_str)(bd_xjson_array* this, int pos);
    int (*get_num)(bd_xjson_array* this, int pos);
    void (*sort)(bd_xjson_array* this, int (*compare_fn)(const void*, const void*));
};
#define BD_XJSON_ARRAY_CLASS(__ptr)      \
do                                       \
{                                        \
    (__ptr)->type = BD_XJSON_ARRAY;      \
    (__ptr)->data = NULL;                \
    (__ptr)->add = arr_add;              \
    (__ptr)->add_str = arr_add_str;      \
    (__ptr)->add_num = arr_add_num;      \
    (__ptr)->add_true = arr_add_true;    \
    (__ptr)->add_false = arr_add_false;  \
    (__ptr)->add_null = arr_add_null;    \
    (__ptr)->delete = arr_delete;        \
    (__ptr)->search = arr_search;        \
    (__ptr)->update = arr_update;        \
    (__ptr)->set = arr_set;              \
    (__ptr)->set_str = arr_set_str;      \
    (__ptr)->set_num = arr_set_num;      \
    (__ptr)->set_true = arr_set_true;    \
    (__ptr)->set_false = arr_set_false;  \
    (__ptr)->set_null = arr_set_null;    \
    (__ptr)->get = arr_get;              \
    (__ptr)->get_str = arr_get_str;      \
    (__ptr)->get_num = arr_get_num;      \
    (__ptr)->sort = arr_qsort;           \
} while(0)


/*
 *  bd_xjson_true class
 */
struct bd_xjson_true
{
/* parent class */
    bd_xjson_base;
};
#define BD_XJSON_TRUE_CLASS(__ptr)  \
do                                  \
{                                   \
    (__ptr)->type = BD_XJSON_TRUE;  \
    (__ptr)->data = NULL;           \
} while(0)


/*
 *  bd_xjson_false class
 */
struct bd_xjson_false
{
/* parent class */
    bd_xjson_base;
};
#define BD_XJSON_FALSE_CLASS(__ptr) \
do                                  \
{                                   \
    (__ptr)->type = BD_XJSON_FALSE; \
    (__ptr)->data = NULL;           \
} while(0)


/*
 *  bd_xjson_null class
 */
struct bd_xjson_null
{
/* parent class */
    bd_xjson_base;
};
#define BD_XJSON_NULL_CLASS(__ptr)  \
do                                  \
{                                   \
    (__ptr)->type = BD_XJSON_NULL;  \
    (__ptr)->data = NULL;           \
} while(0)


#endif
