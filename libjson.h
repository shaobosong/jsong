/*
 *  json
 *
 *  Copyright (c) 2022-2024 ShaoBo Song

 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

/* JSON Type */
enum {
    JSON_TYPE_OBJECT = 1,
    JSON_TYPE_STRING,
    JSON_TYPE_NUMBER,
    JSON_TYPE_ARRAY,
    JSON_TYPE_TRUE,
    JSON_TYPE_FALSE,
    JSON_TYPE_NULL
};

/* JSON */
typedef struct JSON JSON;

/* JSON Impl */
typedef struct JSONObject JSONObject;
typedef struct JSONString JSONString;
typedef struct JSONNumber JSONNumber;
typedef struct JSONArray JSONArray;
typedef struct JSON JSONTrue;
typedef struct JSON JSONFalse;
typedef struct JSON JSONNull;

/* JSON Object Iter */
typedef struct JSONObjectIter JSONObjectIter;

/* JSON Array Iter */
typedef struct JSONArrayIter JSONArrayIter;

/*
 *  JSON class
 *  All classes in json_impl.h are based on it.
 *
 *  @data: data of json
 *    object:  JSONHashTable *
 *    string:  char *
 *    number:  int *
 *    array:   JSONList *
 *  @type: type of json
 */
#define JSONClass(klass) \
struct klass { \
    void* data; \
    int type; \
}
JSONClass(JSON);


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

/*
 *  JSONString class
 *
 *  @set: set
 *  @get: get
 */
#define JSONStringClass(klass) \
struct klass { \
/* parent class */ \
    JSONClass(); \
    void (*set)(JSONString *this, const char *val); \
    char *(*get)(const JSONString *this); \
}
JSONStringClass(JSONString);

/*
 *  JSONNumber class
 *
 *  @set: set
 *  @get: get
 */
#define JSONNumberClass(klass) \
struct klass { \
/* parent class */ \
    JSONClass(); \
    void (*set)(JSONNumber *this, int val); \
    int (*get)(const JSONNumber *this); \
}
JSONNumberClass(JSONNumber);

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


struct JSONObjectIter {
    void* index;
    char* key;
    JSON value;
    void* __entries;
};

struct JSONArrayIter {
    void* index;
    JSON value;
};


/* the following functions are not called directly, only for MACRO */
JSONObject *obj_default_cstr();
JSONObject obj_default();
JSONObject *obj_data_cstr(void* data);
JSONObject obj_data(void* data);
JSONObject *obj_copy_cstr(const void* src);
JSONObject obj_copy(const void* src);
JSONString *str_assign_cstr(char* val);
JSONString str_assign(char* val);
JSONString *str_data_cstr(void* data);
JSONString str_data(void* data);
JSONString *str_copy_cstr(const void* src);
JSONString str_copy(const void* src);
JSONNumber *num_assign_cstr(int val);
JSONNumber num_assign(int val);
JSONNumber *num_data_cstr(void *data);
JSONNumber num_data(void *data);
JSONNumber *num_copy_cstr(const void* src);
JSONNumber num_copy(const void* src);
JSONArray *arr_default_cstr();
JSONArray arr_default();
JSONArray *arr_data_cstr(void *data);
JSONArray arr_data(void *data);
JSONArray *arr_copy_cstr(const void* src);
JSONArray arr_copy(const void* src);
JSONTrue* true_default_cstr();
JSONTrue true_default();
JSONFalse* false_default_cstr();
JSONFalse false_default();
JSONNull* null_default_cstr();
JSONNull null_default();
int json_reassign(void* dst, const void* src);
int json_free(void* val);
void json_free_data(JSON* json);

void json_stringify(const void* json, char** pstr, int* plen);
int json_parse(const char* str, void* json);

JSONObjectIter obj_iterate(JSONObjectIter iter);
JSONArrayIter arr_iterate(JSONArrayIter iter);
JSONArrayIter arr_riterate(JSONArrayIter iter);


/*
 *  User Interface
 */
#define JSON_OBJECT_PTR()                     obj_default_cstr()
#define JSON_OBJECT()                         obj_default()
#define JSON_OBJECT_DATA_PTR(data)            obj_data_cstr(data)
#define JSON_OBJECT_DATA(data)                obj_data(data)
#define JSON_OBJECT_COPY_PTR(obj)             obj_copy_cstr(obj)
#define JSON_OBJECT_COPY(obj)                 obj_copy(obj)
#define JSON_OBJECT_FOREACH(iter, end)        for(;iter.index != end.index;iter = obj_iterate(iter))
#define JSON_STRING_PTR(str)                  str_assign_cstr(str)
#define JSON_STRING(str)                      str_assign(str)
#define JSON_STRING_DATA_PTR(data)            str_data_cstr(data)
#define JSON_STRING_DATA(data)                str_data(data)
#define JSON_STRING_COPY_PTR(str)             str_copy_cstr(str)
#define JSON_STRING_COPY(str)                 str_copy(str)
#define JSON_NUMBER_PTR(num)                  num_assign_cstr(num)
#define JSON_NUMBER(num)                      num_assign(num)
#define JSON_NUMBER_DATA_PTR(data)            num_data_cstr(data)
#define JSON_NUMBER_DATA(data)                num_data(data)
#define JSON_NUMBER_COPY_PTR(num)             num_copy_cstr(num)
#define JSON_NUMBER_COPY(num)                 num_copy(num)
#define JSON_ARRAY_PTR()                      arr_default_cstr()
#define JSON_ARRAY()                          arr_default()
#define JSON_ARRAY_DATA_PTR(data)             arr_data_cstr(data)
#define JSON_ARRAY_DATA(data)                 arr_data(data)
#define JSON_ARRAY_COPY_PTR(arr)              arr_copy_cstr(arr)
#define JSON_ARRAY_COPY(arr)                  arr_copy(arr)
#define JSON_ARRAY_FOREACH(iter, end)         for(;iter.index != end.index;iter = arr_iterate(iter))
#define JSON_ARRAY_REVERSE_FOREACH(iter, end) for(;iter.index != end.index;iter = arr_riterate(iter))
#define JSON_TRUE_PTR()                       true_default_cstr()
#define JSON_TRUE()                           true_default()
#define JSON_FALSE_PTR()                      false_default_cstr()
#define JSON_FALSE()                          false_default()
#define JSON_NULL_PTR()                       null_default_cstr()
#define JSON_NULL()                           null_default()
#define JSON_REASSIGN(json, val)              json_reassign(json, val)
#define FREE_JSON(json)                       json_free(json)
#define FREE_JSON_DATA(json)                  json_free_data(json)

#define JSON_STRINGIFY(json, pstr, plen)      json_stringify(json, pstr, plen)
#define JSON_PARSE(str, json)                 json_parse(str, json)