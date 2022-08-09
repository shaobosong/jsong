/*
 *  jsong
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
typedef enum jsong_type jsong_type;
enum jsong_type
{
    JSONG_OBJECT = 1,
    JSONG_STRING,
    JSONG_NUMBER,
    JSONG_ARRAY,
    JSONG_TRUE,
    JSONG_FALSE,
    JSONG_NULL

};

/* JSON */
typedef struct jsong jsong;

/* JSON Impl */
typedef struct jsong_object jsong_object;
typedef struct jsong_string jsong_string;
typedef struct jsong_number jsong_number;
typedef struct jsong_array jsong_array;
typedef struct jsong jsong_true;
typedef struct jsong jsong_false;
typedef struct jsong jsong_null;

/* JSON Object Iter */
typedef struct jsong_object_iter jsong_object_iter;

/* JSON Array Iter */
typedef struct jsong_array_iter jsong_array_iter;

/*
 *  jsong class
 *      All classes in jsong_impl.h are based on it.
 *
 *  @data: data of json
 *      object:    jsong_htab*
 *      string:    char*
 *      number:    int*
 *      array:     jsong_list*
 *      true:      void*(0)
 *      false:     void*(0)
 *      null:      void*(0)
 *  @type: type of json
 *      object:    JSONG_OBJECT
 *      string:    JSONG_STRING
 *      number:    JSONG_NUMBER
 *      array:     JSONG_ARRAY
 *      true:      JSONG_TRUE
 *      false:     JSONG_FALSE
 *      null:      JSONG_NULL
 */
#define jsong(klass)                                                              \
struct klass                                                                      \
{                                                                                 \
    void* data;                                                                   \
    jsong_type type;                                                              \
}
jsong(jsong);


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
#define jsong_object(klass)                                                       \
struct klass                                                                      \
{                                                                                 \
    /* parent class */                                                            \
    jsong();                                                                      \
    /* member functions */                                                        \
    void (*add)(jsong_object* this, const char* key, const void* val);            \
    void (*add_ref)(jsong_object* this, const char* key, const void* val);        \
    void (*add_str)(jsong_object* this, const char* key, const char* val);        \
    void (*add_num)(jsong_object* this, const char* key, int val);                \
    void (*add_true)(jsong_object* this, const char* key);                        \
    void (*add_false)(jsong_object* this, const char* key);                       \
    void (*add_null)(jsong_object* this, const char* key);                        \
    void (*del)(jsong_object* this, const char* key);                             \
    void (*set)(jsong_object* this, const char* key, const void* val);            \
    void (*set_ref)(jsong_object* this, const char* key, const void* val);        \
    void (*set_str)(jsong_object* this, const char* key, const char* val);        \
    void (*set_num)(jsong_object* this, const char* key, int val);                \
    void (*set_true)(jsong_object* this, const char* key);                        \
    void (*set_false)(jsong_object* this, const char* key);                       \
    void (*set_null)(jsong_object* this, const char* key);                        \
    void* (*get)(const jsong_object* this, const char* key, void* val);           \
    void* (*get_ref)(const jsong_object* this, const char* key, void* val);       \
    char* (*get_str)(const jsong_object* this, const char* key);                  \
    char* (*get_str_ref)(const jsong_object* this, const char* key);              \
    int (*get_num)(const jsong_object* this, const char* key);                    \
    jsong_object_iter (*begin)(const jsong_object* this);                         \
    jsong_object_iter (*end)(const jsong_object* this);                           \
}
jsong_object(jsong_object);

/*
 *  jsong_string class
 *
 *  @set: set
 *  @get: get
 */
#define jsong_string(klass)                                                       \
struct klass                                                                      \
{                                                                                 \
    /* parent class */                                                            \
    jsong();                                                                      \
    /* member functions */                                                        \
    void (*set)(jsong_string* this, const char* val);                             \
    char* (*get)(const jsong_string* this);                                       \
}
jsong_string(jsong_string);

/*
 *  jsong_number class
 *
 *  @set: set
 *  @get: get
 */
#define jsong_number(klass)                                                       \
struct klass                                                                      \
{                                                                                 \
    /* parent class */                                                            \
    jsong();                                                                      \
    /* member functions */                                                        \
    void (*set)(jsong_number* this, int val);                                     \
    int (*get)(const jsong_number* this);                                         \
}
jsong_number(jsong_number);

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
#define jsong_array(klass)                                                        \
struct klass                                                                      \
{                                                                                 \
    /* parent class */                                                            \
    jsong();                                                                      \
    /* member functions */                                                        \
    void (*add)(jsong_array* this, int pos, const void* val);                     \
    void (*add_str)(jsong_array* this, int pos, const char* val);                 \
    void (*add_num)(jsong_array* this, int pos, int val);                         \
    void (*add_true)(jsong_array* this, int pos);                                 \
    void (*add_false)(jsong_array* this, int pos);                                \
    void (*add_null)(jsong_array* this, int pos);                                 \
    void (*del)(jsong_array* this, int pos);                                      \
    void (*set)(jsong_array* this, int pos, const void* val);                     \
    void (*set_str)(jsong_array* this, int pos, const char* val);                 \
    void (*set_num)(jsong_array* this, int pos, int val);                         \
    void (*set_true)(jsong_array* this, int pos);                                 \
    void (*set_false)(jsong_array* this, int pos);                                \
    void (*set_null)(jsong_array* this, int pos);                                 \
    void* (*get)(const jsong_array* this, int pos, void* val);                    \
    char* (*get_str)(const jsong_array* this, int pos);                           \
    int (*get_num)(const jsong_array* this, int pos);                             \
    void (*sort)(jsong_array* this, int (*compare_fn)(const void*, const void*)); \
    jsong_array_iter (*begin)(const jsong_array* this);                           \
    jsong_array_iter (*end)(const jsong_array* this);                             \
    jsong_array_iter (*rbegin)(const jsong_array* this);                          \
    jsong_array_iter (*rend)(const jsong_array* this);                            \
}
jsong_array(jsong_array);


struct jsong_object_iter
{
    void* index;
    char* key;
    jsong value;
    void* __entries;
};

struct jsong_array_iter
{
    void* index;
    jsong value;
};


/* the following functions are not called directly, only for MACRO */
jsong_object* obj_default_cstr();
jsong_object obj_default();
jsong_object* obj_data_cstr(void* data);
jsong_object obj_data(void* data);
jsong_object* obj_copy_cstr(const void* src);
jsong_object obj_copy(const void* src);
jsong_string* str_assign_cstr(char* val);
jsong_string str_assign(char* val);
jsong_string* str_data_cstr(void* data);
jsong_string str_data(void* data);
jsong_string* str_copy_cstr(const void* src);
jsong_string str_copy(const void* src);
jsong_number* num_assign_cstr(int val);
jsong_number num_assign(int val);
jsong_number* num_data_cstr(void *data);
jsong_number num_data(void *data);
jsong_number* num_copy_cstr(const void* src);
jsong_number num_copy(const void* src);
jsong_array* arr_default_cstr();
jsong_array arr_default();
jsong_array* arr_data_cstr(void *data);
jsong_array arr_data(void *data);
jsong_array* arr_copy_cstr(const void* src);
jsong_array arr_copy(const void* src);
jsong_true* true_default_cstr();
jsong_true true_default();
jsong_false* false_default_cstr();
jsong_false false_default();
jsong_null* null_default_cstr();
jsong_null null_default();
int jsong_reassign(void* dst, const void* src);
int jsong_free(void* val);
void jsong_free_data(jsong* json);

void jsong_stringify(const void* json, char** pstr, int* plen);
int jsong_parse(const char* str, void* json);

jsong_object_iter obj_iterate(jsong_object_iter iter);
jsong_array_iter arr_iterate(jsong_array_iter iter);
jsong_array_iter arr_riterate(jsong_array_iter iter);


/*
 *  User Interface
 */
typedef jsong JSON;
typedef jsong_object JSONObject;
typedef jsong_object_iter JSONObjectIter;
typedef jsong_string JSONString;
typedef jsong_number JSONNumber;
typedef jsong_array JSONArray;
typedef jsong_array_iter JSONArrayIter;
typedef jsong_true JSONTrue;
typedef jsong_false JSONFalse;
typedef jsong_null JSONNull;

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
#define JSON_REASSIGN(json, val)              jsong_reassign(json, val)
#define FREE_JSON(json)                       jsong_free(json)
#define FREE_JSON_DATA(json)                  jsong_free_data(json)

#define JSON_STRINGIFY(json, pstr, plen)      jsong_stringify(json, pstr, plen)
#define JSON_PARSE(str, json)                 jsong_parse(str, json)