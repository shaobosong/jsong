#ifndef JSON_IMPL_H
#define JSON_IMPL_H

#include <stddef.h>

#include "lib/json.h"
#include "lib/json_htab.h"
#include "lib/json_list.h"

typedef struct JSONObject JSONObject;
typedef struct JSONString JSONString;
typedef struct JSONNumber JSONNumber;
typedef struct JSONArray JSONArray;
typedef struct JSON JSONTrue;
typedef struct JSON JSONFalse;
typedef struct JSON JSONNull;

typedef JSONHashTableIter JSONObjectIter;
typedef JSONLinkedListIter JSONArrayIter;

/*
 *  JSONObject class
 *
 *  @add: add a <key-val> pair
 *  @set: set a <key-val> pair
 *  @delete: delete a <key-val> pair
 *  @get: get a <val>
 *  @begin: return a iterator to the 1st element
 *  @end: return a past-the-end iterator that points to the element
 *        following the last element of the JSONObject
 */
/* constructor */
JSONObject *obj_default_cstr();
JSONObject obj_default();
JSONObject *obj_data_cstr(void *data);
JSONObject obj_data(void *data);
JSONObject *obj_copy_cstr(const void *src);
JSONObject obj_copy(const void *src);

/* member functions */
void obj_add(JSONObject *obj, const char *key, const void *val);
void obj_add_ref(JSONObject *obj, const char *key, const void *val);
void obj_add_str(JSONObject *obj, const char *key, const char *val);
void obj_add_num(JSONObject *obj, const char *key, int val);
void obj_add_true(JSONObject *obj, const char *key);
void obj_add_false(JSONObject *obj, const char *key);
void obj_add_null(JSONObject *obj, const char *key);
void obj_del(JSONObject *obj, const char *key);
void obj_set(JSONObject *obj, const char *key, const void *val);
void obj_set_ref(JSONObject *obj, const char *key, const void *val);
void obj_set_str(JSONObject *obj, const char *key, const char *val);
void obj_set_num(JSONObject *obj, const char *key, int val);
void obj_set_true(JSONObject *obj, const char *key);
void obj_set_false(JSONObject *obj, const char *key);
void obj_set_null(JSONObject *obj, const char *key);
void *obj_get(const JSONObject *obj, const char *key, void *val);
void *obj_get_ref(const JSONObject *obj, const char *key, void *val);
char *obj_get_str(const JSONObject *obj, const char *key);
char *obj_get_str_ref(const JSONObject *obj, const char *key);
int obj_get_num(const JSONObject *obj, const char *key);
JSONObjectIter obj_begin(const JSONObject *obj);
JSONObjectIter obj_end(const JSONObject *obj);
JSONObjectIter obj_iterate(JSONObjectIter iter);

#define JSONObjectClass(klass) \
struct klass { \
/* parent class */ \
    JSONClass(); \
/* public */ \
    /* member functions */ \
    void (*add)(JSONObject *this, const char *key, const void *val); \
    void (*add_ref)(JSONObject *this, const char *key, const void *val); \
    void (*add_str)(JSONObject *this, const char *key, const char *val); \
    void (*add_num)(JSONObject *this, const char *key, int val); \
    void (*add_true)(JSONObject *this, const char *key); \
    void (*add_false)(JSONObject *this, const char *key); \
    void (*add_null)(JSONObject *this, const char *key); \
    void (*del)(JSONObject *this, const char *key); \
    void (*set)(JSONObject *this, const char *key, const void *val); \
    void (*set_ref)(JSONObject *this, const char *key, const void *val); \
    void (*set_str)(JSONObject *this, const char *key, const char *val); \
    void (*set_num)(JSONObject *this, const char *key, int val); \
    void (*set_true)(JSONObject *this, const char *key); \
    void (*set_false)(JSONObject *this, const char *key); \
    void (*set_null)(JSONObject *this, const char *key); \
    void *(*get)(const JSONObject *this, const char *key, void *val); \
    void *(*get_ref)(const JSONObject *this, const char *key, void *val); \
    char *(*get_str)(const JSONObject *this, const char *key); \
    char *(*get_str_ref)(const JSONObject *this, const char *key); \
    int (*get_num)(const JSONObject *this, const char *key); \
    JSONObjectIter (*begin)(const JSONObject *this); \
    JSONObjectIter (*end)(const JSONObject *this); \
}
JSONObjectClass(JSONObject);

#define JSON_OBJECT_CLASS(__ptr, __data)     \
do {                                         \
    (__ptr)->type = JSON_TYPE_OBJECT;        \
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
 *  JSONString class
 *
 *  @set: set
 *  @get: get
 */
/* constructor */
JSONString *str_assign_cstr(char *val);
JSONString str_assign(char *val);
JSONString *str_data_cstr(void *data);
JSONString str_data(void *data);
JSONString *str_copy_cstr(const void *src);
JSONString str_copy(const void *src);

/* member functions */
void str_set(JSONString *str, const char *val);
char *str_get(const JSONString *str);

#define JSONStringClass(klass) \
struct klass { \
/* parent class */ \
    JSONClass(); \
    void (*set)(JSONString *this, const char *val); \
    char *(*get)(const JSONString *this); \
}
JSONStringClass(JSONString);

#define JSON_STRING_CLASS(__ptr, __data)     \
do {                                         \
    (__ptr)->type = JSON_TYPE_STRING;        \
    (__ptr)->data = __data;                  \
    (__ptr)->set = str_set;                  \
    (__ptr)->get = str_get;                  \
} while(0)


/*
 *  JSONNumber class
 *
 *  @set: set
 *  @get: get
 */
/* constructor */
JSONNumber *num_assign_cstr(int val);
JSONNumber num_assign(int val);
JSONNumber *num_data_cstr(void *data);
JSONNumber num_data(void *data);
JSONNumber *num_copy_cstr(const void *src);
JSONNumber num_copy(const void *src);

/* member functions */
void num_set(JSONNumber *num, int val);
int num_get(const JSONNumber *num);

#define JSONNumberClass(klass) \
struct klass { \
/* parent class */ \
    JSONClass(); \
    void (*set)(JSONNumber *this, int val); \
    int (*get)(const JSONNumber *this); \
}
JSONNumberClass(JSONNumber);

#define JSON_NUMBER_CLASS(__ptr, __data)     \
do {                                         \
    (__ptr)->type = JSON_TYPE_NUMBER;        \
    (__ptr)->data = __data;                  \
    (__ptr)->set = num_set;                  \
    (__ptr)->get = num_get;                  \
} while(0)


/*
 *  JSONArray class
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
 *         following the last element of the reversed JSONArray
 */
/* constructor */
JSONArray *arr_default_cstr();
JSONArray arr_default();
JSONArray *arr_data_cstr(void *data);
JSONArray arr_data(void *data);
JSONArray *arr_copy_cstr(const void *src);
JSONArray arr_copy(const void *src);

/* member functions */
void arr_add(JSONArray *arr, int pos, const void *val);
void arr_add_str(JSONArray *arr, int pos, const char *val);
void arr_add_num(JSONArray *arr, int pos, int val);
void arr_add_true(JSONArray *arr, int pos);
void arr_add_false(JSONArray *arr, int pos);
void arr_add_null(JSONArray *arr, int pos);
void arr_del(JSONArray *arr, int pos);
void arr_set(JSONArray *arr, int pos, const void *val);
void arr_set_str(JSONArray *arr, int pos, const char *val);
void arr_set_num(JSONArray *arr, int pos, int val);
void arr_set_true(JSONArray *arr, int pos);
void arr_set_false(JSONArray *arr, int pos);
void arr_set_null(JSONArray *arr, int pos);
void *arr_get(const JSONArray *arr, int pos, void *val);
char *arr_get_str(const JSONArray *arr, int pos);
int arr_get_num(const JSONArray *arr, int pos);
void arr_qsort(JSONArray *arr, int (*compare_fn)(const void*, const void*));
JSONArrayIter arr_begin(const JSONArray *arr);
JSONArrayIter arr_end(const JSONArray *arr);
JSONArrayIter arr_iterate(JSONArrayIter iter);
JSONArrayIter arr_rbegin(const JSONArray *arr);
JSONArrayIter arr_rend(const JSONArray *arr);
JSONArrayIter arr_riterate(JSONArrayIter iter);

#define JSONArrayClass(klass) \
struct klass { \
/* parent class */ \
    JSONClass(); /* a linked list */ \
/* public */ \
    /* member functions */ \
    void (*add)(JSONArray *this, int pos, const void *val); \
    void (*add_str)(JSONArray *this, int pos, const char *val); \
    void (*add_num)(JSONArray *this, int pos, int val); \
    void (*add_true)(JSONArray *this, int pos); \
    void (*add_false)(JSONArray *this, int pos); \
    void (*add_null)(JSONArray *this, int pos); \
    void (*del)(JSONArray *this, int pos); \
    void (*set)(JSONArray *this, int pos, const void *val); \
    void (*set_str)(JSONArray *this, int pos, const char *val); \
    void (*set_num)(JSONArray *this, int pos, int val); \
    void (*set_true)(JSONArray *this, int pos); \
    void (*set_false)(JSONArray *this, int pos); \
    void (*set_null)(JSONArray *this, int pos); \
    void *(*get)(const JSONArray *this, int pos, void *val); \
    char *(*get_str)(const JSONArray *this, int pos); \
    int (*get_num)(const JSONArray *this, int pos); \
    void (*sort)(JSONArray *this, int (*compare_fn)(const void*, const void*)); \
    JSONArrayIter (*begin)(const JSONArray *this); \
    JSONArrayIter (*end)(const JSONArray *this); \
    JSONArrayIter (*rbegin)(const JSONArray *this); \
    JSONArrayIter (*rend)(const JSONArray *this); \
}
JSONArrayClass(JSONArray);

#define JSON_ARRAY_CLASS(__ptr, __data)     \
do {                                        \
    (__ptr)->type = JSON_TYPE_ARRAY;        \
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
 *  JSONTrue class
 */
/* constructor */
JSONTrue *true_default_cstr();
JSONTrue true_default();
#define JSON_TRUE_CLASS(__ptr)       \
do {                                 \
    (__ptr)->type = JSON_TYPE_TRUE;  \
    (__ptr)->data = NULL;            \
} while(0)


/*
 *  JSONFalse class
 */
/* constructor */
JSONFalse *false_default_cstr();
JSONFalse false_default();
#define JSON_FALSE_CLASS(__ptr)      \
do {                                 \
    (__ptr)->type = JSON_TYPE_FALSE; \
    (__ptr)->data = NULL;            \
} while(0)


/*
 *  JSONNull class
 */
/* constructor */
JSONNull *null_default_cstr();
JSONNull null_default();
#define JSON_NULL_CLASS(__ptr)       \
do {                                 \
    (__ptr)->type = JSON_TYPE_NULL;  \
    (__ptr)->data = NULL;            \
} while(0)


#endif
