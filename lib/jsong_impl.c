#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/jsong_impl.h"
#include "lib/jsong_list.h"
#include "lib/jsong_htab.h"
#include "lib/utils.h"

jsong_object* obj_default_cstr()
{
    jsong_object *d;

    d = xmallocz(sizeof *d);
    JSONG_OBJECT_CLASS(d, htab_create(1));

    return d;
}

jsong_object obj_default()
{
    jsong_object d;
    JSONG_OBJECT_CLASS(&d, htab_create(1));

    return d;
}

jsong_object* obj_data_cstr(void *data)
{
    jsong_object *d;

    d = xmallocz(sizeof *d);
    JSONG_OBJECT_CLASS(d, data);

    return d;
}

jsong_object obj_data(void *data)
{
    jsong_object d;
    JSONG_OBJECT_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe jsong or jsong_object */
jsong_object* obj_copy_cstr(const void* val)
{
    const jsong_object *s;
    jsong_object *d;

    s = val;
    assert(s->type == JSONG_OBJECT);
    assert(s->data);

    d = xmallocz(sizeof *d);
    JSONG_OBJECT_CLASS(d, htab_create_copy(s->data));

    return d;
}

jsong_object obj_copy(const void* val)
{
    const jsong_object *s;
    jsong_object d;

    s = val;
    assert(s->type == JSONG_OBJECT);
    assert(s->data);

    JSONG_OBJECT_CLASS(&d, htab_create_copy(s->data));

    return d;
}

void obj_add(jsong_object* obj, const char* key, const void* val)
{
    assert(obj->data && key && val);
    htab_insert(obj->data, key, val);
}

void obj_add_ref(jsong_object* obj, const char* key, const void* val)
{
    assert(obj->data && key && val);
    htab_insert_ref(obj->data, key, val);
}

void obj_add_str(jsong_object* obj, const char* key, const char* val)
{
    jsong json = {
        .type = JSONG_STRING,
        .data = (char*)val
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_num(jsong_object* obj, const char* key, int val)
{
    jsong json = {
        .type = JSONG_NUMBER,
        .data = &val
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_true(jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_TRUE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_false(jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_FALSE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_null(jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_NULL,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_del(jsong_object* obj, const char* key)
{
    assert(obj->data && key);
    htab_erase(obj->data, key);
}

void obj_set(jsong_object* obj, const char* key, const void* val)
{
    assert(obj->data && key && val);
    htab_set(obj->data, key, val);
}

void obj_set_ref(jsong_object* obj, const char* key, const void* val)
{
    assert(obj->data && key && val);
    htab_set_ref(obj->data, key, val);
}

void obj_set_str(jsong_object* obj, const char* key, const char* val)
{
    jsong json = {
        .type = JSONG_STRING,
        .data = (char*)val
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_num(jsong_object* obj, const char* key, int val)
{
    jsong json = {
        .type = JSONG_NUMBER,
        .data = &val
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_true(jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_TRUE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_false(jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_FALSE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_null(jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_NULL,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void* obj_get(const jsong_object* obj, const char* key, void* val)
{
    assert(obj->data && key && val);
    htab_find(obj->data, key, val);

    return val;
}

void* obj_get_ref(const jsong_object* obj, const char* key, void* val)
{
    assert(obj->data && key && val);
    htab_find_ref(obj->data, key, val);

    return val;
}

char* obj_get_str(const jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_STRING,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find(obj->data, key, &json);

    return json.data;
}

char* obj_get_str_ref(const jsong_object* obj, const char* key)
{
    jsong json = {
        .type = JSONG_STRING,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find_ref(obj->data, key, &json);

    return json.data;
}

int obj_get_num(const jsong_object* obj, const char* key)
{
    int num;
    jsong json = {
        .type = JSONG_NUMBER,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find(obj->data, key, &json);
    num = *(int*)json.data;

    xfree(json.data);
    return num;
}

jsong_object_iter obj_begin(const jsong_object* obj)
{
    assert(obj->data);
    return htab_begin(obj->data);
}

jsong_object_iter obj_end(const jsong_object* obj)
{
    assert(obj->data);
    return htab_end(obj->data);
}

jsong_object_iter obj_iterate(jsong_object_iter iter)
{
    return htab_iterate(iter);
}

jsong_array* arr_default_cstr()
{
    jsong_array *d;

    d = xmallocz(sizeof *d);
    JSONG_ARRAY_CLASS(d, list_create());

    return d;
}

jsong_array arr_default()
{
    jsong_array d;

    JSONG_ARRAY_CLASS(&d, list_create());

    return d;
}

jsong_array* arr_data_cstr(void *data)
{
    jsong_array *d;

    d = xmallocz(sizeof *d);
    JSONG_ARRAY_CLASS(d, data);

    return d;
}

jsong_array arr_data(void *data)
{
    jsong_array d;
    JSONG_ARRAY_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe jsong or jsong_array */
jsong_array* arr_copy_cstr(const void* val)
{
    const jsong_array *s;
    jsong_array *d;

    s = val;
    assert(s->type == JSONG_ARRAY);
    assert(s->data);

    d = xmallocz(sizeof *d);
    JSONG_ARRAY_CLASS(d, list_create_copy(s->data));

    return d;
}

jsong_array arr_copy(const void* val)
{
    const jsong_array *s;
    jsong_array d;

    s = val;
    assert(s->type == JSONG_ARRAY);
    assert(s->data);

    JSONG_ARRAY_CLASS(&d, list_create_copy(s->data));

    return d;
}

void arr_add(jsong_array* arr, int pos, const void* val)
{
    assert(arr->data && val);
    list_insert(arr->data, pos, val);
}

void arr_add_str(jsong_array* arr, int pos, const char* val)
{
    jsong json = {
        .type = JSONG_STRING,
        .data = (char*)val
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_num(jsong_array* arr, int pos, int val)
{
    jsong json = {
        .type = JSONG_NUMBER,
        .data = &val
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_true(jsong_array* arr, int pos)
{
    jsong json = {
        .type = JSONG_TRUE,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_false(jsong_array* arr, int pos)
{
    jsong json = {
        .type = JSONG_FALSE,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_null(jsong_array* arr, int pos)
{
    jsong json = {
        .type = JSONG_NULL,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_del(jsong_array* arr, int pos)
{
    assert(arr->data);
    list_erase(arr->data, pos);
}

void arr_set(jsong_array* arr, int pos, const void* val)
{
    assert(arr->data && val);
    list_set(arr->data, pos, val);
}

void arr_set_str(jsong_array* arr, int pos, const char* val)
{
    jsong json = {
        .type = JSONG_STRING,
        .data = (char*)val
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_num(jsong_array* arr, int pos, int val)
{
    jsong json = {
        .type = JSONG_NUMBER,
        .data = &val
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}
void arr_set_true(jsong_array* arr, int pos)
{
    jsong json = {
        .type = JSONG_TRUE,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_false(jsong_array* arr, int pos)
{
    jsong json = {
        .type = JSONG_FALSE,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_null(jsong_array* arr, int pos)
{
    jsong json = {
        .type = JSONG_NULL,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void* arr_get(const jsong_array* arr, int pos, void* val)
{
    assert(arr->data && val);
    list_find(arr->data, pos, val);

    return val;
}

char* arr_get_str(const jsong_array* arr, int pos)
{
    jsong json = {
        .type = JSONG_STRING,
        .data = NULL
    };

    assert(arr->data);
    list_find(arr->data, pos, &json);

    return json.data;
}

int arr_get_num(const jsong_array* arr, int pos)
{
    int num;
    jsong json = {
        .type = JSONG_NUMBER,
        .data = NULL
    };

    assert(arr->data);
    list_find(arr->data, pos, &json);
    num = *(int*)json.data;

    xfree(json.data);
    return num;
}

void arr_qsort(jsong_array* arr, int (*compare_fn)(const void*, const void*))
{
    assert(arr->data);
    list_qsort(arr->data, compare_fn);
}

jsong_array_iter arr_begin(const jsong_array* arr)
{
    assert(arr->data);
    return list_begin(arr->data);
}

jsong_array_iter arr_end(const jsong_array* arr)
{
    assert(arr->data);
    return list_end(arr->data);
}

jsong_array_iter arr_iterate(jsong_array_iter iter)
{
    return list_iterate(iter);
}

jsong_array_iter arr_rbegin(const jsong_array* arr)
{
    assert(arr->data);
    return list_rbegin(arr->data);
}

jsong_array_iter arr_rend(const jsong_array* arr)
{
    assert(arr->data);
    return list_rend(arr->data);
}

jsong_array_iter arr_riterate(jsong_array_iter iter)
{
    return list_riterate(iter);
}

jsong_string* str_assign_cstr(char* val)
{
    jsong_string *d;

    d = xmallocz(sizeof *d);
    JSONG_STRING_CLASS(d, xmallocz(strlen(val) + 1));

    strcat(d->data, val);

    return d;
}

jsong_string str_assign(char* val)
{
    jsong_string d;

    JSONG_STRING_CLASS(&d, xmallocz(strlen(val) + 1));
    strcat(d.data, val);

    return d;
}

jsong_string* str_data_cstr(void* data)
{
    jsong_string *d;

    d = xmallocz(sizeof *d);
    JSONG_STRING_CLASS(d, data);

    return d;
}

jsong_string str_data(void* data)
{
    jsong_string d;
    JSONG_STRING_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe jsong or jsong_string */
jsong_string* str_copy_cstr(const void* val)
{
    const jsong_string *s;
    jsong_string *d;

    s = val;
    d = xmallocz(sizeof *d);
    JSONG_STRING_CLASS(d, xmallocz(strlen(s->data) + 1));

    assert(s->data);
    assert(s->type == d->type);
    strcat(d->data, s->data);

    return d;
}

jsong_string str_copy(const void* val)
{
    const jsong_string *s;
    jsong_string d;

    s = val;
    JSONG_STRING_CLASS(&d, xmallocz(strlen(s->data) + 1));

    assert(s->data);
    assert(s->type == d.type);
    strcat(d.data, s->data);

    return d;
}

void str_set(jsong_string* str, const char* val)
{
    assert(str->data);
    xfree(str->data);

    str->data = xmallocz(strlen(val) + 1);
    strcat(str->data, val);
}

char* str_get(const jsong_string* str)
{
    assert(str->data);
    char* chars = xmallocz(strlen(str->data) + 1);
    strcat(chars, str->data);

    return chars;
}

jsong_number* num_assign_cstr(int val)
{
    jsong_number *d;

    d = xmallocz(sizeof *d);
    JSONG_NUMBER_CLASS(d, xmallocz(sizeof (int)));

    *(int*)d->data = val;

    return d;
}

jsong_number num_assign(int val)
{
    jsong_number d;

    JSONG_NUMBER_CLASS(&d, xmallocz(sizeof (int)));
    *(int*)d.data = val;

    return d;
}

jsong_number* num_data_cstr(void* data)
{
    jsong_number *d;

    d = xmallocz(sizeof *d);
    JSONG_NUMBER_CLASS(d, data);

    return d;
}

jsong_number num_data(void* data)
{
    jsong_number d;
    JSONG_NUMBER_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe jsong or jsong_number */
jsong_number* num_copy_cstr(const void* val)
{
    const jsong_number *s;
    jsong_number *d;

    s = val;
    d = xmallocz(sizeof *d);
    JSONG_NUMBER_CLASS(d, xmallocz(sizeof (int)));

    assert(s->data);
    assert(s->type == d->type);
    *(int*)d->data = *(int*)s->data;

    return d;
}

void num_set(jsong_number* num, int val)
{
    assert(num->data);
    *(int*)num->data = val;
}

int num_get(const jsong_number* num)
{
    assert(num->data);
    return *(int*)num->data;
}

jsong_true* true_default_cstr()
{
    jsong_true *d;

    d = xmallocz(sizeof *d);
    JSONG_TRUE_CLASS(d);

    return d;
}

jsong_true true_default()
{
    jsong_true d;
    JSONG_TRUE_CLASS(&d);

    return d;
}

jsong_false* false_default_cstr()
{
    jsong_false *d;

    d = xmallocz(sizeof *d);
    JSONG_FALSE_CLASS(d);

    return d;
}

jsong_false false_default()
{
    jsong_false d;
    JSONG_FALSE_CLASS(&d);

    return d;
}

jsong_null* null_default_cstr()
{
    jsong_null *d;

    d = xmallocz(sizeof *d);
    JSONG_NULL_CLASS(d);

    return d;
}

jsong_null null_default()
{
    jsong_null d;
    JSONG_NULL_CLASS(&d);

    return d;
}