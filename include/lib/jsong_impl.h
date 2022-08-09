#ifndef JSONG_IMPL_H
#define JSONG_IMPL_H

#include <stddef.h>

#include "lib/jsong.h"
#include "lib/jsong_htab.h"
#include "lib/jsong_list.h"

typedef struct jsong_object jsong_object;
typedef struct jsong_string jsong_string;
typedef struct jsong_number jsong_number;
typedef struct jsong_array jsong_array;
typedef struct jsong jsong_true;
typedef struct jsong jsong_false;
typedef struct jsong jsong_null;

typedef jsong_htab_iter jsong_object_iter;
typedef jsong_list_iter jsong_array_iter;

/*
 *  jsong_object class
 *
 *  @add: add a <key-val> pair
 *  @set: set a <key-val> pair
 *  @delete: delete a <key-val> pair
 *  @get: get a <val>
 *  @begin: return a iterator to the 1st element
 *  @end: return a past-the-end iterator that points to the element
 *        following the last element of the jsong_object
 */
/* constructor */
jsong_object* obj_default_cstr();
jsong_object obj_default();
jsong_object* obj_data_cstr(void* data);
jsong_object obj_data(void* data);
jsong_object* obj_copy_cstr(const void* src);
jsong_object obj_copy(const void* src);
/* member functions */
void obj_add(jsong_object* obj, const char* key, const void* val);
void obj_add_ref(jsong_object* obj, const char* key, const void* val);
void obj_add_str(jsong_object* obj, const char* key, const char* val);
void obj_add_num(jsong_object* obj, const char* key, int val);
void obj_add_true(jsong_object* obj, const char* key);
void obj_add_false(jsong_object* obj, const char* key);
void obj_add_null(jsong_object* obj, const char* key);
void obj_del(jsong_object* obj, const char* key);
void obj_set(jsong_object* obj, const char* key, const void* val);
void obj_set_ref(jsong_object* obj, const char* key, const void* val);
void obj_set_str(jsong_object* obj, const char* key, const char* val);
void obj_set_num(jsong_object* obj, const char* key, int val);
void obj_set_true(jsong_object* obj, const char* key);
void obj_set_false(jsong_object* obj, const char* key);
void obj_set_null(jsong_object* obj, const char* key);
void* obj_get(const jsong_object* obj, const char* key, void* val);
void* obj_get_ref(const jsong_object* obj, const char* key, void* val);
char* obj_get_str(const jsong_object* obj, const char* key);
char* obj_get_str_ref(const jsong_object* obj, const char* key);
int obj_get_num(const jsong_object* obj, const char* key);
jsong_object_iter obj_begin(const jsong_object* obj);
jsong_object_iter obj_end(const jsong_object* obj);
jsong_object_iter obj_iterate(jsong_object_iter iter);
#define jsong_object(klass) \
struct klass \
{ \
/* parent class */ \
    jsong(); \
/* public */ \
    /* member functions */ \
    void (*add)(jsong_object* this, const char* key, const void* val); \
    void (*add_ref)(jsong_object* this, const char* key, const void* val); \
    void (*add_str)(jsong_object* this, const char* key, const char* val); \
    void (*add_num)(jsong_object* this, const char* key, int val); \
    void (*add_true)(jsong_object* this, const char* key); \
    void (*add_false)(jsong_object* this, const char* key); \
    void (*add_null)(jsong_object* this, const char* key); \
    void (*del)(jsong_object* this, const char* key); \
    void (*set)(jsong_object* this, const char* key, const void* val); \
    void (*set_ref)(jsong_object* this, const char* key, const void* val); \
    void (*set_str)(jsong_object* this, const char* key, const char* val); \
    void (*set_num)(jsong_object* this, const char* key, int val); \
    void (*set_true)(jsong_object* this, const char* key); \
    void (*set_false)(jsong_object* this, const char* key); \
    void (*set_null)(jsong_object* this, const char* key); \
    void* (*get)(const jsong_object* this, const char* key, void* val); \
    void* (*get_ref)(const jsong_object* this, const char* key, void* val); \
    char* (*get_str)(const jsong_object* this, const char* key); \
    char* (*get_str_ref)(const jsong_object* this, const char* key); \
    int (*get_num)(const jsong_object* this, const char* key); \
    jsong_object_iter (*begin)(const jsong_object* this); \
    jsong_object_iter (*end)(const jsong_object* this); \
}
jsong_object(jsong_object);
#define JSONG_OBJECT_CLASS(__ptr, __data)    \
do                                           \
{                                            \
    (__ptr)->type = JSONG_OBJECT;            \
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
 *  jsong_string class
 *
 *  @set: set
 *  @get: get
 */
/* constructor */
jsong_string* str_assign_cstr(char* val);
jsong_string str_assign(char* val);
jsong_string* str_data_cstr(void* data);
jsong_string str_data(void* data);
jsong_string* str_copy_cstr(const void* src);
jsong_string str_copy(const void* src);
/* member functions */
void str_set(jsong_string* str, const char* val);
char* str_get(const jsong_string* str);
#define jsong_string(klass) \
struct klass \
{ \
/* parent class */ \
    jsong(); \
    void (*set)(jsong_string* this, const char* val); \
    char* (*get)(const jsong_string* this); \
}
jsong_string(jsong_string);
#define JSONG_STRING_CLASS(__ptr, __data)    \
do                                           \
{                                            \
    (__ptr)->type = JSONG_STRING;            \
    (__ptr)->data = __data;                  \
    (__ptr)->set = str_set;                  \
    (__ptr)->get = str_get;                  \
} while(0)


/*
 *  jsong_number class
 *
 *  @set: set
 *  @get: get
 */
/* constructor */
jsong_number* num_assign_cstr(int val);
jsong_number num_assign(int val);
jsong_number* num_data_cstr(void *data);
jsong_number num_data(void *data);
jsong_number* num_copy_cstr(const void* src);
jsong_number num_copy(const void* src);
/* member functions */
void num_set(jsong_number* num, int val);
int num_get(const jsong_number* num);
#define jsong_number(klass) \
struct klass \
{ \
/* parent class */ \
    jsong(); \
    void (*set)(jsong_number* this, int val); \
    int (*get)(const jsong_number* this); \
}
jsong_number(jsong_number);
#define JSONG_NUMBER_CLASS(__ptr, __data)    \
do                                           \
{                                            \
    (__ptr)->type = JSONG_NUMBER;            \
    (__ptr)->data = __data;                  \
    (__ptr)->set = num_set;                  \
    (__ptr)->get = num_get;                  \
} while(0)


/*
 *  jsong_array class
 *
 *  @add: add a <val>
 *  @set: set a <val>
 *  @delete: delete a <val>
 *  @get: get a <val>
 *  @sort: sort all items by quick sort
 *  @begin: return a iterator to the first element
 *  @end: return a past-the-end iterator that points to the element following
 *        the last element of the jsong_array
 *  @rbegin: return a iterator to the first element
 *  @rend: return a past-the-end reverse iterator that points to the element
 *         following the last element of the reversed jsong_array
 */
/* constructor */
jsong_array* arr_default_cstr();
jsong_array arr_default();
jsong_array* arr_data_cstr(void *data);
jsong_array arr_data(void *data);
jsong_array* arr_copy_cstr(const void* src);
jsong_array arr_copy(const void* src);
/* member functions */
void arr_add(jsong_array* arr, int pos, const void* val);
void arr_add_str(jsong_array* arr, int pos, const char* val);
void arr_add_num(jsong_array* arr, int pos, int val);
void arr_add_true(jsong_array* arr, int pos);
void arr_add_false(jsong_array* arr, int pos);
void arr_add_null(jsong_array* arr, int pos);
void arr_del(jsong_array* arr, int pos);
void arr_set(jsong_array* arr, int pos, const void* val);
void arr_set_str(jsong_array* arr, int pos, const char* val);
void arr_set_num(jsong_array* arr, int pos, int val);
void arr_set_true(jsong_array* arr, int pos);
void arr_set_false(jsong_array* arr, int pos);
void arr_set_null(jsong_array* arr, int pos);
void* arr_get(const jsong_array* arr, int pos, void* val);
char* arr_get_str(const jsong_array* arr, int pos);
int arr_get_num(const jsong_array* arr, int pos);
void arr_qsort(jsong_array* arr, int (*compare_fn)(const void*, const void*));
jsong_array_iter arr_begin(const jsong_array* arr);
jsong_array_iter arr_end(const jsong_array* arr);
jsong_array_iter arr_iterate(jsong_array_iter iter);
jsong_array_iter arr_rbegin(const jsong_array* arr);
jsong_array_iter arr_rend(const jsong_array* arr);
jsong_array_iter arr_riterate(jsong_array_iter iter);
#define jsong_array(klass) \
struct klass \
{ \
/* parent class */ \
    jsong(); /* a linked list */ \
/* public */ \
    /* member functions */ \
    void (*add)(jsong_array* this, int pos, const void* val); \
    void (*add_str)(jsong_array* this, int pos, const char* val); \
    void (*add_num)(jsong_array* this, int pos, int val); \
    void (*add_true)(jsong_array* this, int pos); \
    void (*add_false)(jsong_array* this, int pos); \
    void (*add_null)(jsong_array* this, int pos); \
    void (*del)(jsong_array* this, int pos); \
    void (*set)(jsong_array* this, int pos, const void* val); \
    void (*set_str)(jsong_array* this, int pos, const char* val); \
    void (*set_num)(jsong_array* this, int pos, int val); \
    void (*set_true)(jsong_array* this, int pos); \
    void (*set_false)(jsong_array* this, int pos); \
    void (*set_null)(jsong_array* this, int pos); \
    void* (*get)(const jsong_array* this, int pos, void* val); \
    char* (*get_str)(const jsong_array* this, int pos); \
    int (*get_num)(const jsong_array* this, int pos); \
    void (*sort)(jsong_array* this, int (*compare_fn)(const void*, const void*)); \
    jsong_array_iter (*begin)(const jsong_array* this); \
    jsong_array_iter (*end)(const jsong_array* this); \
    jsong_array_iter (*rbegin)(const jsong_array* this); \
    jsong_array_iter (*rend)(const jsong_array* this); \
}
jsong_array(jsong_array);
#define JSONG_ARRAY_CLASS(__ptr, __data)    \
do                                          \
{                                           \
    (__ptr)->type = JSONG_ARRAY;            \
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
 *  jsong_true class
 */
/* constructor */
jsong_true* true_default_cstr();
jsong_true true_default();
#define JSONG_TRUE_CLASS(__ptr)     \
do                                  \
{                                   \
    (__ptr)->type = JSONG_TRUE;     \
    (__ptr)->data = NULL;           \
} while(0)


/*
 *  jsong_false class
 */
/* constructor */
jsong_false* false_default_cstr();
jsong_false false_default();
#define JSONG_FALSE_CLASS(__ptr)    \
do                                  \
{                                   \
    (__ptr)->type = JSONG_FALSE;    \
    (__ptr)->data = NULL;           \
} while(0)


/*
 *  jsong_null class
 */
/* constructor */
jsong_null* null_default_cstr();
jsong_null null_default();
#define JSONG_NULL_CLASS(__ptr)     \
do                                  \
{                                   \
    (__ptr)->type = JSONG_NULL;     \
    (__ptr)->data = NULL;           \
} while(0)


#endif
