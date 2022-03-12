#ifndef BD_XJSON_IMPL_H
#define BD_XJSON_IMPL_H

#include <stddef.h>

#include "lib/bd_xjson.h"
#include "lib/bd_xjson_htab.h"
#include "lib/bd_xjson_list.h"

typedef struct bd_xjson_object bd_xjson_object;
typedef struct bd_xjson_string bd_xjson_string;
typedef struct bd_xjson_number bd_xjson_number;
typedef struct bd_xjson_array bd_xjson_array;
typedef struct bd_xjson bd_xjson_true;
typedef struct bd_xjson bd_xjson_false;
typedef struct bd_xjson bd_xjson_null;

typedef bd_xjson_htab_iter bd_xjson_object_iter;
typedef bd_xjson_list_iter bd_xjson_array_iter;

/*
 *  bd_xjson_object class
 *
 *  @add: add a <key-val> pair
 *  @set: set a <key-val> pair
 *  @delete: delete a <key-val> pair
 *  @get: get a <val>
 *  @begin: return a iterator to the 1st element
 *  @end: return a past-the-end iterator that points to the element
 *        following the last element of the bd_xjson_object
 */
/* constructor */
bd_xjson_object* obj_default_cstr();
bd_xjson_object obj_default();
bd_xjson_object* obj_data_cstr(void* data);
bd_xjson_object obj_data(void* data);
bd_xjson_object* obj_copy_cstr(const void* src);
bd_xjson_object obj_copy(const void* src);
/* member functions */
void obj_add(bd_xjson_object* obj, const char* key, const void* val);
void obj_add_ref(bd_xjson_object* obj, const char* key, const void* val);
void obj_add_str(bd_xjson_object* obj, const char* key, const char* val);
void obj_add_num(bd_xjson_object* obj, const char* key, int val);
void obj_add_true(bd_xjson_object* obj, const char* key);
void obj_add_false(bd_xjson_object* obj, const char* key);
void obj_add_null(bd_xjson_object* obj, const char* key);
void obj_del(bd_xjson_object* obj, const char* key);
void obj_set(bd_xjson_object* obj, const char* key, const void* val);
void obj_set_ref(bd_xjson_object* obj, const char* key, const void* val);
void obj_set_str(bd_xjson_object* obj, const char* key, const char* val);
void obj_set_num(bd_xjson_object* obj, const char* key, int val);
void obj_set_true(bd_xjson_object* obj, const char* key);
void obj_set_false(bd_xjson_object* obj, const char* key);
void obj_set_null(bd_xjson_object* obj, const char* key);
void* obj_get(const bd_xjson_object* obj, const char* key, void* val);
void* obj_get_ref(const bd_xjson_object* obj, const char* key, void* val);
char* obj_get_str(const bd_xjson_object* obj, const char* key);
char* obj_get_str_ref(const bd_xjson_object* obj, const char* key);
int obj_get_num(const bd_xjson_object* obj, const char* key);
bd_xjson_object_iter obj_begin(const bd_xjson_object* obj);
bd_xjson_object_iter obj_end(const bd_xjson_object* obj);
bd_xjson_object_iter obj_iterate(bd_xjson_object_iter iter);
#define bd_xjson_object(klass) \
struct klass \
{ \
/* parent class */ \
    bd_xjson(); \
/* public */ \
    /* member functions */ \
    void (*add)(bd_xjson_object* this, const char* key, const void* val); \
    void (*add_ref)(bd_xjson_object* this, const char* key, const void* val); \
    void (*add_str)(bd_xjson_object* this, const char* key, const char* val); \
    void (*add_num)(bd_xjson_object* this, const char* key, int val); \
    void (*add_true)(bd_xjson_object* this, const char* key); \
    void (*add_false)(bd_xjson_object* this, const char* key); \
    void (*add_null)(bd_xjson_object* this, const char* key); \
    void (*del)(bd_xjson_object* this, const char* key); \
    void (*set)(bd_xjson_object* this, const char* key, const void* val); \
    void (*set_ref)(bd_xjson_object* this, const char* key, const void* val); \
    void (*set_str)(bd_xjson_object* this, const char* key, const char* val); \
    void (*set_num)(bd_xjson_object* this, const char* key, int val); \
    void (*set_true)(bd_xjson_object* this, const char* key); \
    void (*set_false)(bd_xjson_object* this, const char* key); \
    void (*set_null)(bd_xjson_object* this, const char* key); \
    void* (*get)(const bd_xjson_object* this, const char* key, void* val); \
    void* (*get_ref)(const bd_xjson_object* this, const char* key, void* val); \
    char* (*get_str)(const bd_xjson_object* this, const char* key); \
    char* (*get_str_ref)(const bd_xjson_object* this, const char* key); \
    int (*get_num)(const bd_xjson_object* this, const char* key); \
    bd_xjson_object_iter (*begin)(const bd_xjson_object* this); \
    bd_xjson_object_iter (*end)(const bd_xjson_object* this); \
}
bd_xjson_object(bd_xjson_object);
#define BD_XJSON_OBJECT_CLASS(__ptr, __data) \
do                                           \
{                                            \
    (__ptr)->type = BD_XJSON_OBJECT;         \
    (__ptr)->data = __data;                  \
    (__ptr)->add = obj_add;                  \
    (__ptr)->add_ref = obj_add_ref;          \
    (__ptr)->add_str = obj_add_str;          \
    (__ptr)->add_num = obj_add_num;          \
    (__ptr)->add_true = obj_add_true;        \
    (__ptr)->add_false = obj_add_false;      \
    (__ptr)->add_null = obj_add_null;        \
    (__ptr)->del = obj_del;                  \
    (__ptr)->set = obj_set;                  \
    (__ptr)->set_ref = obj_set_ref;          \
    (__ptr)->set_str = obj_set_str;          \
    (__ptr)->set_num = obj_set_num;          \
    (__ptr)->set_true = obj_set_true;        \
    (__ptr)->set_false = obj_set_false;      \
    (__ptr)->set_null = obj_set_null;        \
    (__ptr)->get = obj_get;                  \
    (__ptr)->get_ref = obj_get_ref;          \
    (__ptr)->get_str = obj_get_str;          \
    (__ptr)->get_str_ref = obj_get_str_ref;  \
    (__ptr)->get_num = obj_get_num;          \
    (__ptr)->begin = obj_begin;              \
    (__ptr)->end = obj_end;                  \
} while(0)


/*
 *  bd_xjson_string class
 *
 *  @set: set
 *  @get: get
 */
/* constructor */
bd_xjson_string* str_assign_cstr(char* val);
bd_xjson_string str_assign(char* val);
bd_xjson_string* str_data_cstr(void* data);
bd_xjson_string str_data(void* data);
bd_xjson_string* str_copy_cstr(const void* src);
bd_xjson_string str_copy(const void* src);
/* member functions */
void str_set(bd_xjson_string* str, const char* val);
char* str_get(const bd_xjson_string* str);
#define bd_xjson_string(klass) \
struct klass \
{ \
/* parent class */ \
    bd_xjson(); \
    void (*set)(bd_xjson_string* this, const char* val); \
    char* (*get)(const bd_xjson_string* this); \
}
bd_xjson_string(bd_xjson_string);
#define BD_XJSON_STRING_CLASS(__ptr, __data) \
do                                           \
{                                            \
    (__ptr)->type = BD_XJSON_STRING;         \
    (__ptr)->data = __data;                  \
    (__ptr)->set = str_set;                  \
    (__ptr)->get = str_get;                  \
} while(0)


/*
 *  bd_xjson_number class
 *
 *  @set: set
 *  @get: get
 */
/* constructor */
bd_xjson_number* num_assign_cstr(int val);
bd_xjson_number num_assign(int val);
bd_xjson_number* num_data_cstr(void *data);
bd_xjson_number num_data(void *data);
bd_xjson_number* num_copy_cstr(const void* src);
bd_xjson_number num_copy(const void* src);
/* member functions */
void num_set(bd_xjson_number* num, int val);
int num_get(const bd_xjson_number* num);
#define bd_xjson_number(klass) \
struct klass \
{ \
/* parent class */ \
    bd_xjson(); \
    void (*set)(bd_xjson_number* this, int val); \
    int (*get)(const bd_xjson_number* this); \
}
bd_xjson_number(bd_xjson_number);
#define BD_XJSON_NUMBER_CLASS(__ptr, __data) \
do                                           \
{                                            \
    (__ptr)->type = BD_XJSON_NUMBER;         \
    (__ptr)->data = __data;                  \
    (__ptr)->set = num_set;                  \
    (__ptr)->get = num_get;                  \
} while(0)


/*
 *  bd_xjson_array class
 *
 *  @add: add a <val>
 *  @set: set a <val>
 *  @delete: delete a <val>
 *  @get: get a <val>
 *  @sort: sort all items by quick sort
 *  @begin: return a iterator to the first element
 *  @end: return a past-the-end iterator that points to the element following
 *        the last element of the bd_xjson_array
 *  @rbegin: return a iterator to the first element
 *  @rend: return a past-the-end reverse iterator that points to the element
 *         following the last element of the reversed bd_xjson_array
 */
/* constructor */
bd_xjson_array* arr_default_cstr();
bd_xjson_array arr_default();
bd_xjson_array* arr_data_cstr(void *data);
bd_xjson_array arr_data(void *data);
bd_xjson_array* arr_copy_cstr(const void* src);
bd_xjson_array arr_copy(const void* src);
/* member functions */
void arr_add(bd_xjson_array* arr, int pos, const void* val);
void arr_add_str(bd_xjson_array* arr, int pos, const char* val);
void arr_add_num(bd_xjson_array* arr, int pos, int val);
void arr_add_true(bd_xjson_array* arr, int pos);
void arr_add_false(bd_xjson_array* arr, int pos);
void arr_add_null(bd_xjson_array* arr, int pos);
void arr_del(bd_xjson_array* arr, int pos);
void arr_set(bd_xjson_array* arr, int pos, const void* val);
void arr_set_str(bd_xjson_array* arr, int pos, const char* val);
void arr_set_num(bd_xjson_array* arr, int pos, int val);
void arr_set_true(bd_xjson_array* arr, int pos);
void arr_set_false(bd_xjson_array* arr, int pos);
void arr_set_null(bd_xjson_array* arr, int pos);
void* arr_get(const bd_xjson_array* arr, int pos, void* val);
char* arr_get_str(const bd_xjson_array* arr, int pos);
int arr_get_num(const bd_xjson_array* arr, int pos);
void arr_qsort(bd_xjson_array* arr, int (*compare_fn)(const void*, const void*));
bd_xjson_array_iter arr_begin(const bd_xjson_array* arr);
bd_xjson_array_iter arr_end(const bd_xjson_array* arr);
bd_xjson_array_iter arr_iterate(bd_xjson_array_iter iter);
bd_xjson_array_iter arr_rbegin(const bd_xjson_array* arr);
bd_xjson_array_iter arr_rend(const bd_xjson_array* arr);
bd_xjson_array_iter arr_riterate(bd_xjson_array_iter iter);
#define bd_xjson_array(klass) \
struct klass \
{ \
/* parent class */ \
    bd_xjson(); /* a linked list */ \
/* public */ \
    /* member functions */ \
    void (*add)(bd_xjson_array* this, int pos, const void* val); \
    void (*add_str)(bd_xjson_array* this, int pos, const char* val); \
    void (*add_num)(bd_xjson_array* this, int pos, int val); \
    void (*add_true)(bd_xjson_array* this, int pos); \
    void (*add_false)(bd_xjson_array* this, int pos); \
    void (*add_null)(bd_xjson_array* this, int pos); \
    void (*del)(bd_xjson_array* this, int pos); \
    void (*set)(bd_xjson_array* this, int pos, const void* val); \
    void (*set_str)(bd_xjson_array* this, int pos, const char* val); \
    void (*set_num)(bd_xjson_array* this, int pos, int val); \
    void (*set_true)(bd_xjson_array* this, int pos); \
    void (*set_false)(bd_xjson_array* this, int pos); \
    void (*set_null)(bd_xjson_array* this, int pos); \
    void* (*get)(const bd_xjson_array* this, int pos, void* val); \
    char* (*get_str)(const bd_xjson_array* this, int pos); \
    int (*get_num)(const bd_xjson_array* this, int pos); \
    void (*sort)(bd_xjson_array* this, int (*compare_fn)(const void*, const void*)); \
    bd_xjson_array_iter (*begin)(const bd_xjson_array* this); \
    bd_xjson_array_iter (*end)(const bd_xjson_array* this); \
    bd_xjson_array_iter (*rbegin)(const bd_xjson_array* this); \
    bd_xjson_array_iter (*rend)(const bd_xjson_array* this); \
}
bd_xjson_array(bd_xjson_array);
#define BD_XJSON_ARRAY_CLASS(__ptr, __data) \
do                                          \
{                                           \
    (__ptr)->type = BD_XJSON_ARRAY;         \
    (__ptr)->data = __data;                 \
    (__ptr)->add = arr_add;                 \
    (__ptr)->add_str = arr_add_str;         \
    (__ptr)->add_num = arr_add_num;         \
    (__ptr)->add_true = arr_add_true;       \
    (__ptr)->add_false = arr_add_false;     \
    (__ptr)->add_null = arr_add_null;       \
    (__ptr)->del = arr_del;                 \
    (__ptr)->set = arr_set;                 \
    (__ptr)->set_str = arr_set_str;         \
    (__ptr)->set_num = arr_set_num;         \
    (__ptr)->set_true = arr_set_true;       \
    (__ptr)->set_false = arr_set_false;     \
    (__ptr)->set_null = arr_set_null;       \
    (__ptr)->get = arr_get;                 \
    (__ptr)->get_str = arr_get_str;         \
    (__ptr)->get_num = arr_get_num;         \
    (__ptr)->sort = arr_qsort;              \
    (__ptr)->begin = arr_begin;             \
    (__ptr)->end = arr_end;                 \
    (__ptr)->rbegin = arr_rbegin;           \
    (__ptr)->rend = arr_rend;               \
} while(0)


/*
 *  bd_xjson_true class
 */
/* constructor */
bd_xjson_true* true_default_cstr();
bd_xjson_true true_default();
#define BD_XJSON_TRUE_CLASS(__ptr)  \
do                                  \
{                                   \
    (__ptr)->type = BD_XJSON_TRUE;  \
    (__ptr)->data = NULL;           \
} while(0)


/*
 *  bd_xjson_false class
 */
/* constructor */
bd_xjson_false* false_default_cstr();
bd_xjson_false false_default();
#define BD_XJSON_FALSE_CLASS(__ptr) \
do                                  \
{                                   \
    (__ptr)->type = BD_XJSON_FALSE; \
    (__ptr)->data = NULL;           \
} while(0)


/*
 *  bd_xjson_null class
 */
/* constructor */
bd_xjson_null* null_default_cstr();
bd_xjson_null null_default();
#define BD_XJSON_NULL_CLASS(__ptr)  \
do                                  \
{                                   \
    (__ptr)->type = BD_XJSON_NULL;  \
    (__ptr)->data = NULL;           \
} while(0)


#endif
