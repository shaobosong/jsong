/*
 *  bd_xjson
 *
 *  Copyright (c) 2022 ShaoBo Song

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
typedef enum bd_xjson_type bd_xjson_type;
enum bd_xjson_type
{
    BD_XJSON_OBJECT = 1,
    BD_XJSON_STRING,
    BD_XJSON_NUMBER,
    BD_XJSON_ARRAY,
    BD_XJSON_TRUE,
    BD_XJSON_FALSE,
    BD_XJSON_NULL

};

/* JSON */
typedef struct bd_xjson bd_xjson;

/* JSON Impl */
typedef struct bd_xjson_object bd_xjson_object;
typedef struct bd_xjson_string bd_xjson_string;
typedef struct bd_xjson_number bd_xjson_number;
typedef struct bd_xjson_array bd_xjson_array;
typedef struct bd_xjson bd_xjson_true;
typedef struct bd_xjson bd_xjson_false;
typedef struct bd_xjson bd_xjson_null;

/* JSON Object Iter */
typedef struct bd_xjson_object_iter bd_xjson_object_iter;

/* JSON Array Iter */
typedef struct bd_xjson_array_iter bd_xjson_array_iter;

/*
 *  bd_xjson class
 *      All classes in bd_xjson_impl.h are based on it.
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
#define bd_xjson(klass)                                                              \
struct klass                                                                         \
{                                                                                    \
    void* data;                                                                      \
    bd_xjson_type type;                                                              \
}
bd_xjson(bd_xjson);


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
#define bd_xjson_object(klass)                                                       \
struct klass                                                                         \
{                                                                                    \
    /* parent class */                                                               \
    bd_xjson();                                                                      \
    /* member functions */                                                           \
    void (*add)(bd_xjson_object* this, const char* key, const void* val);            \
    void (*add_ref)(bd_xjson_object* this, const char* key, const void* val);        \
    void (*add_str)(bd_xjson_object* this, const char* key, const char* val);        \
    void (*add_num)(bd_xjson_object* this, const char* key, int val);                \
    void (*add_true)(bd_xjson_object* this, const char* key);                        \
    void (*add_false)(bd_xjson_object* this, const char* key);                       \
    void (*add_null)(bd_xjson_object* this, const char* key);                        \
    void (*del)(bd_xjson_object* this, const char* key);                             \
    void (*set)(bd_xjson_object* this, const char* key, const void* val);            \
    void (*set_ref)(bd_xjson_object* this, const char* key, const void* val);        \
    void (*set_str)(bd_xjson_object* this, const char* key, const char* val);        \
    void (*set_num)(bd_xjson_object* this, const char* key, int val);                \
    void (*set_true)(bd_xjson_object* this, const char* key);                        \
    void (*set_false)(bd_xjson_object* this, const char* key);                       \
    void (*set_null)(bd_xjson_object* this, const char* key);                        \
    void* (*get)(const bd_xjson_object* this, const char* key, void* val);           \
    void* (*get_ref)(const bd_xjson_object* this, const char* key, void* val);       \
    char* (*get_str)(const bd_xjson_object* this, const char* key);                  \
    char* (*get_str_ref)(const bd_xjson_object* this, const char* key);              \
    int (*get_num)(const bd_xjson_object* this, const char* key);                    \
    bd_xjson_object_iter (*begin)(const bd_xjson_object* this);                      \
    bd_xjson_object_iter (*end)(const bd_xjson_object* this);                        \
}
bd_xjson_object(bd_xjson_object);

/*
 *  bd_xjson_string class
 *
 *  @set: set
 *  @get: get
 */
#define bd_xjson_string(klass)                                                       \
struct klass                                                                         \
{                                                                                    \
    /* parent class */                                                               \
    bd_xjson();                                                                      \
    /* member functions */                                                           \
    void (*set)(bd_xjson_string* this, const char* val);                             \
    char* (*get)(const bd_xjson_string* this);                                       \
}
bd_xjson_string(bd_xjson_string);

/*
 *  bd_xjson_number class
 *
 *  @set: set
 *  @get: get
 */
#define bd_xjson_number(klass)                                                       \
struct klass                                                                         \
{                                                                                    \
    /* parent class */                                                               \
    bd_xjson();                                                                      \
    /* member functions */                                                           \
    void (*set)(bd_xjson_number* this, int val);                                     \
    int (*get)(const bd_xjson_number* this);                                         \
}
bd_xjson_number(bd_xjson_number);

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
#define bd_xjson_array(klass)                                                        \
struct klass                                                                         \
{                                                                                    \
    /* parent class */                                                               \
    bd_xjson();                                                                      \
    /* member functions */                                                           \
    void (*add)(bd_xjson_array* this, int pos, const void* val);                     \
    void (*add_str)(bd_xjson_array* this, int pos, const char* val);                 \
    void (*add_num)(bd_xjson_array* this, int pos, int val);                         \
    void (*add_true)(bd_xjson_array* this, int pos);                                 \
    void (*add_false)(bd_xjson_array* this, int pos);                                \
    void (*add_null)(bd_xjson_array* this, int pos);                                 \
    void (*del)(bd_xjson_array* this, int pos);                                      \
    void (*set)(bd_xjson_array* this, int pos, const void* val);                     \
    void (*set_str)(bd_xjson_array* this, int pos, const char* val);                 \
    void (*set_num)(bd_xjson_array* this, int pos, int val);                         \
    void (*set_true)(bd_xjson_array* this, int pos);                                 \
    void (*set_false)(bd_xjson_array* this, int pos);                                \
    void (*set_null)(bd_xjson_array* this, int pos);                                 \
    void* (*get)(const bd_xjson_array* this, int pos, void* val);                    \
    char* (*get_str)(const bd_xjson_array* this, int pos);                           \
    int (*get_num)(const bd_xjson_array* this, int pos);                             \
    void (*sort)(bd_xjson_array* this, int (*compare_fn)(const void*, const void*)); \
    bd_xjson_array_iter (*begin)(const bd_xjson_array* this);                        \
    bd_xjson_array_iter (*end)(const bd_xjson_array* this);                          \
    bd_xjson_array_iter (*rbegin)(const bd_xjson_array* this);                       \
    bd_xjson_array_iter (*rend)(const bd_xjson_array* this);                         \
}
bd_xjson_array(bd_xjson_array);


struct bd_xjson_object_iter
{
    void* index;
    char* key;
    bd_xjson value;
    void* __entries;
};

struct bd_xjson_array_iter
{
    void* index;
    bd_xjson value;
};


/* the following functions are not called directly, only for MACRO */
bd_xjson_object* obj_default_cstr();
bd_xjson_object obj_default();
bd_xjson_object* obj_data_cstr(void* data);
bd_xjson_object obj_data(void* data);
bd_xjson_object* obj_copy_cstr(const void* src);
bd_xjson_object obj_copy(const void* src);
bd_xjson_string* str_assign_cstr(char* val);
bd_xjson_string str_assign(char* val);
bd_xjson_string* str_data_cstr(void* data);
bd_xjson_string str_data(void* data);
bd_xjson_string* str_copy_cstr(const void* src);
bd_xjson_string str_copy(const void* src);
bd_xjson_number* num_assign_cstr(int val);
bd_xjson_number num_assign(int val);
bd_xjson_number* num_data_cstr(void *data);
bd_xjson_number num_data(void *data);
bd_xjson_number* num_copy_cstr(const void* src);
bd_xjson_number num_copy(const void* src);
bd_xjson_array* arr_default_cstr();
bd_xjson_array arr_default();
bd_xjson_array* arr_data_cstr(void *data);
bd_xjson_array arr_data(void *data);
bd_xjson_array* arr_copy_cstr(const void* src);
bd_xjson_array arr_copy(const void* src);
bd_xjson_true* true_default_cstr();
bd_xjson_true true_default();
bd_xjson_false* false_default_cstr();
bd_xjson_false false_default();
bd_xjson_null* null_default_cstr();
bd_xjson_null null_default();
int bd_xjson_reassign(void* dst, const void* src);
int bd_xjson_free(void* val);
void bd_xjson_free_data(bd_xjson* json);

void bd_xjson_stringify(const void* json, char** pstr, int* plen);
int bd_xjson_parse(const char* str, void* json);

bd_xjson_object_iter obj_iterate(bd_xjson_object_iter iter);
bd_xjson_array_iter arr_iterate(bd_xjson_array_iter iter);
bd_xjson_array_iter arr_riterate(bd_xjson_array_iter iter);


/*
 *  User Interface
 */
typedef bd_xjson JSON;
typedef bd_xjson_object JSONObject;
typedef bd_xjson_object_iter JSONObjectIter;
typedef bd_xjson_string JSONString;
typedef bd_xjson_number JSONNumber;
typedef bd_xjson_array JSONArray;
typedef bd_xjson_array_iter JSONArrayIter;
typedef bd_xjson_true JSONTrue;
typedef bd_xjson_false JSONFalse;
typedef bd_xjson_null JSONNull;

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
#define JSON_REASSIGN(json, val)              bd_xjson_reassign(json, val)
#define FREE_JSON(json)                       bd_xjson_free(json)
#define FREE_JSON_DATA(json)                  bd_xjson_free_data(json)

#define JSON_STRINGIFY(json, pstr, plen)      bd_xjson_stringify(json, pstr, plen)
#define JSON_PARSE(str, json)                 bd_xjson_parse(str, json)