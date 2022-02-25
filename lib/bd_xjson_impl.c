#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/bd_xjson_impl.h"
#include "lib/bd_xjson_list.h"
#include "lib/bd_xjson_htab.h"
#include "lib/utils.h"

bd_xjson_object* obj_default_cstr()
{
    bd_xjson_object *d;

    d = xzmalloc(sizeof *d);
    BD_XJSON_OBJECT_CLASS(d, htab_create(1));

    return d;
}

/* TYPE of VAL maybe bd_xjson or bd_xjson_object */
bd_xjson_object* obj_copy_cstr(const void* val)
{
    const bd_xjson_object *s;
    bd_xjson_object *d;

    s = val;
    assert(s->type == BD_XJSON_OBJECT);
    assert(s->data);

    d = xzmalloc(sizeof *d);
    BD_XJSON_OBJECT_CLASS(d, htab_create_copy(s->data));

    return d;
}

void obj_add(bd_xjson_object* obj, const char* key, const void* val)
{
    assert(obj->data && key && val);
    htab_insert(obj->data, key, val);
}

void obj_add_str(bd_xjson_object* obj, const char* key, const char* val)
{
    bd_xjson json = {
        .type = BD_XJSON_STRING,
        .data = (char*)val
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_num(bd_xjson_object* obj, const char* key, int val)
{
    bd_xjson json = {
        .type = BD_XJSON_NUMBER,
        .data = &val
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_true(bd_xjson_object* obj, const char* key)
{
    bd_xjson json = {
        .type = BD_XJSON_TRUE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_false(bd_xjson_object* obj, const char* key)
{
    bd_xjson json = {
        .type = BD_XJSON_FALSE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_null(bd_xjson_object* obj, const char* key)
{
    bd_xjson json = {
        .type = BD_XJSON_NULL,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_del(bd_xjson_object* obj, const char* key)
{
    assert(obj->data && key);
    htab_erase(obj->data, key);
}

void obj_set(bd_xjson_object* obj, const char* key, const void* val)
{
    assert(obj->data && key && val);
    htab_set(obj->data, key, val);
}

void obj_set_str(bd_xjson_object* obj, const char* key, const char* val)
{
    bd_xjson json = {
        .type = BD_XJSON_STRING,
        .data = (char*)val
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_num(bd_xjson_object* obj, const char* key, int val)
{
    bd_xjson json = {
        .type = BD_XJSON_NUMBER,
        .data = &val
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_true(bd_xjson_object* obj, const char* key)
{
    bd_xjson json = {
        .type = BD_XJSON_TRUE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_false(bd_xjson_object* obj, const char* key)
{
    bd_xjson json = {
        .type = BD_XJSON_FALSE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_null(bd_xjson_object* obj, const char* key)
{
    bd_xjson json = {
        .type = BD_XJSON_NULL,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void* obj_get(const bd_xjson_object* obj, const char* key, void* val)
{
    assert(obj->data && key && val);
    htab_find(obj->data, key, val);

    return val;
}

char* obj_get_str(const bd_xjson_object* obj, const char* key)
{
    bd_xjson json = {
        .type = BD_XJSON_STRING,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find(obj->data, key, &json);

    return json.data;
}

int obj_get_num(const bd_xjson_object* obj, const char* key)
{
    int num;
    bd_xjson json = {
        .type = BD_XJSON_NUMBER,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find(obj->data, key, &json);
    num = *(int*)json.data;

    xfree(json.data);
    return num;
}

bd_xjson_object_iter obj_begin(const bd_xjson_object* obj)
{
    assert(obj->data);
    return htab_begin(obj->data);
}

bd_xjson_object_iter obj_end(const bd_xjson_object* obj)
{
    assert(obj->data);
    return htab_end(obj->data);
}

bd_xjson_object_iter obj_iterate(bd_xjson_object_iter iter)
{
    return htab_iterate(iter);
}

bd_xjson_array* arr_default_cstr()
{
    bd_xjson_array *d;

    d = xzmalloc(sizeof *d);
    BD_XJSON_ARRAY_CLASS(d, list_create());

    return d;
}

/* TYPE of VAL maybe bd_xjson or bd_xjson_array */
bd_xjson_array* arr_copy_cstr(const void* val)
{
    const bd_xjson_array *s;
    bd_xjson_array *d;

    s = val;
    assert(s->type == BD_XJSON_ARRAY);
    assert(s->data);

    d = xzmalloc(sizeof *d);
    BD_XJSON_ARRAY_CLASS(d, list_create_copy(s->data));

    return d;
}

void arr_add(bd_xjson_array* arr, int pos, const void* val)
{
    assert(arr->data && val);
    list_insert(arr->data, pos, val);
}

void arr_add_str(bd_xjson_array* arr, int pos, const char* val)
{
    bd_xjson json = {
        .type = BD_XJSON_STRING,
        .data = (char*)val
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_num(bd_xjson_array* arr, int pos, int val)
{
    bd_xjson json = {
        .type = BD_XJSON_NUMBER,
        .data = &val
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_true(bd_xjson_array* arr, int pos)
{
    bd_xjson json = {
        .type = BD_XJSON_TRUE,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_false(bd_xjson_array* arr, int pos)
{
    bd_xjson json = {
        .type = BD_XJSON_FALSE,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_null(bd_xjson_array* arr, int pos)
{
    bd_xjson json = {
        .type = BD_XJSON_NULL,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_del(bd_xjson_array* arr, int pos)
{
    assert(arr->data);
    list_erase(arr->data, pos);
}

void arr_set(bd_xjson_array* arr, int pos, const void* val)
{
    assert(arr->data && val);
    list_set(arr->data, pos, val);
}

void arr_set_str(bd_xjson_array* arr, int pos, const char* val)
{
    bd_xjson json = {
        .type = BD_XJSON_STRING,
        .data = (char*)val
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_num(bd_xjson_array* arr, int pos, int val)
{
    bd_xjson json = {
        .type = BD_XJSON_NUMBER,
        .data = &val
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}
void arr_set_true(bd_xjson_array* arr, int pos)
{
    bd_xjson json = {
        .type = BD_XJSON_TRUE,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_false(bd_xjson_array* arr, int pos)
{
    bd_xjson json = {
        .type = BD_XJSON_FALSE,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_null(bd_xjson_array* arr, int pos)
{
    bd_xjson json = {
        .type = BD_XJSON_NULL,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void* arr_get(const bd_xjson_array* arr, int pos, void* val)
{
    assert(arr->data && val);
    list_find(arr->data, pos, val);

    return val;
}

char* arr_get_str(const bd_xjson_array* arr, int pos)
{
    bd_xjson json = {
        .type = BD_XJSON_STRING,
        .data = NULL
    };

    assert(arr->data);
    list_find(arr->data, pos, &json);

    return json.data;
}

int arr_get_num(const bd_xjson_array* arr, int pos)
{
    int num;
    bd_xjson json = {
        .type = BD_XJSON_NUMBER,
        .data = NULL
    };

    assert(arr->data);
    list_find(arr->data, pos, &json);
    num = *(int*)json.data;

    xfree(json.data);
    return num;
}

void arr_qsort(bd_xjson_array* arr, int (*compare_fn)(const void*, const void*))
{
    assert(arr->data);
    list_qsort(arr->data, compare_fn);
}

bd_xjson_array_iter arr_begin(const bd_xjson_array* arr)
{
    assert(arr->data);
    return list_begin(arr->data);
}

bd_xjson_array_iter arr_end(const bd_xjson_array* arr)
{
    assert(arr->data);
    return list_end(arr->data);
}

bd_xjson_array_iter arr_iterate(bd_xjson_array_iter iter)
{
    return list_iterate(iter);
}

bd_xjson_array_iter arr_rbegin(const bd_xjson_array* arr)
{
    assert(arr->data);
    return list_rbegin(arr->data);
}

bd_xjson_array_iter arr_rend(const bd_xjson_array* arr)
{
    assert(arr->data);
    return list_rend(arr->data);
}

bd_xjson_array_iter arr_riterate(bd_xjson_array_iter iter)
{
    return list_riterate(iter);
}

bd_xjson_string* str_assign_cstr(char* val)
{
    bd_xjson_string *d;

    d = xzmalloc(sizeof *d);
    BD_XJSON_STRING_CLASS(d, xzmalloc(strlen(val) + 1));

    strcat(d->data, val);

    return d;
}

/* TYPE of VAL maybe bd_xjson or bd_xjson_string */
bd_xjson_string* str_copy_cstr(const void* val)
{
    const bd_xjson_string *s;
    bd_xjson_string *d;

    s = val;
    d = xzmalloc(sizeof *d);
    BD_XJSON_STRING_CLASS(d, xzmalloc(strlen(s->data) + 1));

    assert(s->data);
    assert(s->type == d->type);
    strcat(d->data, s->data);

    return d;
}

void str_set(bd_xjson_string* str, const char* val)
{
    assert(str->data);
    xfree(str->data);

    str->data = xzmalloc(strlen(val) + 1);
    strcat(str->data, val);
}

char* str_get(const bd_xjson_string* str)
{
    assert(str->data);
    char* chars = xzmalloc(strlen(str->data) + 1);
    strcat(chars, str->data);

    return chars;
}

bd_xjson_number* num_assign_cstr(int val)
{
    bd_xjson_number *d;

    d = xzmalloc(sizeof *d);
    BD_XJSON_NUMBER_CLASS(d, xzmalloc(sizeof (int)));

    *(int*)d->data = val;

    return d;
}

/* TYPE of VAL maybe bd_xjson or bd_xjson_number */
bd_xjson_number* num_copy_cstr(const void* val)
{
    const bd_xjson_number *s;
    bd_xjson_number *d;

    s = val;
    d = xzmalloc(sizeof *d);
    BD_XJSON_NUMBER_CLASS(d, xzmalloc(sizeof (int)));

    assert(s->data);
    assert(s->type == d->type);
    *(int*)d->data = *(int*)s->data;

    return d;
}

void num_set(bd_xjson_number* num, int val)
{
    assert(num->data);
    *(int*)num->data = val;
}

int num_get(const bd_xjson_number* num)
{
    assert(num->data);
    return *(int*)num->data;
}

bd_xjson_true* true_default_cstr()
{
    bd_xjson_true *d;

    d = xzmalloc(sizeof *d);
    BD_XJSON_TRUE_CLASS(d);

    return d;
}

bd_xjson_false* false_default_cstr()
{
    bd_xjson_false *d;

    d = xzmalloc(sizeof *d);
    BD_XJSON_FALSE_CLASS(d);

    return d;
}

bd_xjson_null* null_default_cstr()
{
    bd_xjson_null *d;

    d = xzmalloc(sizeof *d);
    BD_XJSON_NULL_CLASS(d);

    return d;
}