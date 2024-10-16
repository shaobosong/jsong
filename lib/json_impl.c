#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/json_impl.h"
#include "lib/json_list.h"
#include "lib/json_htab.h"
#include "lib/json_utils.h"

JSONObject *obj_default_cstr()
{
    JSONObject *d;

    d = json_xmallocz(sizeof *d);
    JSON_OBJECT_CLASS(d, htab_create(1));

    return d;
}

JSONObject obj_default()
{
    JSONObject d;
    JSON_OBJECT_CLASS(&d, htab_create(1));

    return d;
}

JSONObject *obj_data_cstr(void *data)
{
    JSONObject *d;

    d = json_xmallocz(sizeof *d);
    JSON_OBJECT_CLASS(d, data);

    return d;
}

JSONObject obj_data(void *data)
{
    JSONObject d;
    JSON_OBJECT_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe JSON or JSONObject */
JSONObject *obj_copy_cstr(const void *val)
{
    const JSONObject *s;
    JSONObject *d;

    s = val;
    assert(s->type == JSON_TYPE_OBJECT);
    assert(s->data);

    d = json_xmallocz(sizeof *d);
    JSON_OBJECT_CLASS(d, htab_create_copy(s->data));

    return d;
}

JSONObject obj_copy(const void *val)
{
    const JSONObject *s;
    JSONObject d;

    s = val;
    assert(s->type == JSON_TYPE_OBJECT);
    assert(s->data);

    JSON_OBJECT_CLASS(&d, htab_create_copy(s->data));

    return d;
}

void obj_add(JSONObject *obj, const char *key, const void *val)
{
    assert(obj->data && key && val);
    htab_insert(obj->data, key, val);
}

void obj_add_ref(JSONObject *obj, const char *key, const void *val)
{
    assert(obj->data && key && val);
    htab_insert_ref(obj->data, key, val);
}

void obj_add_str(JSONObject *obj, const char *key, const char *val)
{
    JSON json = {
        .type = JSON_TYPE_STRING,
        .data = (char *)val
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_num(JSONObject *obj, const char *key, int val)
{
    JSON json = {
        .type = JSON_TYPE_NUMBER,
        .data = &val
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_true(JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_TRUE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_false(JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_FALSE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_add_null(JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_NULL,
        .data = NULL
    };

    assert(obj->data && key);
    htab_insert(obj->data, key, &json);
}

void obj_del(JSONObject *obj, const char *key)
{
    assert(obj->data && key);
    htab_erase(obj->data, key);
}

void obj_set(JSONObject *obj, const char *key, const void *val)
{
    assert(obj->data && key && val);
    htab_set(obj->data, key, val);
}

void obj_set_ref(JSONObject *obj, const char *key, const void *val)
{
    assert(obj->data && key && val);
    htab_set_ref(obj->data, key, val);
}

void obj_set_str(JSONObject *obj, const char *key, const char *val)
{
    JSON json = {
        .type = JSON_TYPE_STRING,
        .data = (char *)val
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_num(JSONObject *obj, const char *key, int val)
{
    JSON json = {
        .type = JSON_TYPE_NUMBER,
        .data = &val
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_true(JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_TRUE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_false(JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_FALSE,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void obj_set_null(JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_NULL,
        .data = NULL
    };

    assert(obj->data && key);
    htab_set(obj->data, key, &json);
}

void *obj_get(const JSONObject *obj, const char *key, void *val)
{
    assert(obj->data && key && val);
    htab_find(obj->data, key, val);

    return val;
}

void *obj_get_ref(const JSONObject *obj, const char *key, void *val)
{
    assert(obj->data && key && val);
    htab_find_ref(obj->data, key, val);

    return val;
}

char *obj_get_str(const JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_STRING,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find(obj->data, key, &json);

    return json.data;
}

char *obj_get_str_ref(const JSONObject *obj, const char *key)
{
    JSON json = {
        .type = JSON_TYPE_STRING,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find_ref(obj->data, key, &json);

    return json.data;
}

int obj_get_num(const JSONObject *obj, const char *key)
{
    int num;
    JSON json = {
        .type = JSON_TYPE_NUMBER,
        .data = NULL
    };

    assert(obj->data && key);
    htab_find(obj->data, key, &json);
    num = *(int *)json.data;

    json_xfree(json.data);
    return num;
}

JSONObjectIter obj_begin(const JSONObject *obj)
{
    assert(obj->data);
    return htab_begin(obj->data);
}

JSONObjectIter obj_end(const JSONObject *obj)
{
    assert(obj->data);
    return htab_end(obj->data);
}

JSONObjectIter obj_iterate(JSONObjectIter iter)
{
    return htab_iterate(iter);
}

JSONArray *arr_default_cstr()
{
    JSONArray *d;

    d = json_xmallocz(sizeof *d);
    JSON_ARRAY_CLASS(d, list_create());

    return d;
}

JSONArray arr_default()
{
    JSONArray d;

    JSON_ARRAY_CLASS(&d, list_create());

    return d;
}

JSONArray *arr_data_cstr(void *data)
{
    JSONArray *d;

    d = json_xmallocz(sizeof *d);
    JSON_ARRAY_CLASS(d, data);

    return d;
}

JSONArray arr_data(void *data)
{
    JSONArray d;
    JSON_ARRAY_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe JSON or JSONArray */
JSONArray *arr_copy_cstr(const void *val)
{
    const JSONArray *s;
    JSONArray *d;

    s = val;
    assert(s->type == JSON_TYPE_ARRAY);
    assert(s->data);

    d = json_xmallocz(sizeof *d);
    JSON_ARRAY_CLASS(d, list_create_copy(s->data));

    return d;
}

JSONArray arr_copy(const void *val)
{
    const JSONArray *s;
    JSONArray d;

    s = val;
    assert(s->type == JSON_TYPE_ARRAY);
    assert(s->data);

    JSON_ARRAY_CLASS(&d, list_create_copy(s->data));

    return d;
}

void arr_add(JSONArray *arr, int pos, const void *val)
{
    assert(arr->data && val);
    list_insert(arr->data, pos, val);
}

void arr_add_str(JSONArray *arr, int pos, const char *val)
{
    JSON json = {
        .type = JSON_TYPE_STRING,
        .data = (char *)val
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_num(JSONArray *arr, int pos, int val)
{
    JSON json = {
        .type = JSON_TYPE_NUMBER,
        .data = &val
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_true(JSONArray *arr, int pos)
{
    JSON json = {
        .type = JSON_TYPE_TRUE,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_false(JSONArray *arr, int pos)
{
    JSON json = {
        .type = JSON_TYPE_FALSE,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_add_null(JSONArray *arr, int pos)
{
    JSON json = {
        .type = JSON_TYPE_NULL,
        .data = NULL
    };

    assert(arr->data);
    list_insert(arr->data, pos, &json);
}

void arr_del(JSONArray *arr, int pos)
{
    assert(arr->data);
    list_erase(arr->data, pos);
}

void arr_set(JSONArray *arr, int pos, const void *val)
{
    assert(arr->data && val);
    list_set(arr->data, pos, val);
}

void arr_set_str(JSONArray *arr, int pos, const char *val)
{
    JSON json = {
        .type = JSON_TYPE_STRING,
        .data = (char *)val
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_num(JSONArray *arr, int pos, int val)
{
    JSON json = {
        .type = JSON_TYPE_NUMBER,
        .data = &val
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}
void arr_set_true(JSONArray *arr, int pos)
{
    JSON json = {
        .type = JSON_TYPE_TRUE,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_false(JSONArray *arr, int pos)
{
    JSON json = {
        .type = JSON_TYPE_FALSE,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void arr_set_null(JSONArray *arr, int pos)
{
    JSON json = {
        .type = JSON_TYPE_NULL,
        .data = NULL
    };

    assert(arr->data);
    list_set(arr->data, pos, &json);
}

void *arr_get(const JSONArray *arr, int pos, void *val)
{
    assert(arr->data && val);
    list_find(arr->data, pos, val);

    return val;
}

char *arr_get_str(const JSONArray *arr, int pos)
{
    JSON json = {
        .type = JSON_TYPE_STRING,
        .data = NULL
    };

    assert(arr->data);
    list_find(arr->data, pos, &json);

    return json.data;
}

int arr_get_num(const JSONArray *arr, int pos)
{
    int num;
    JSON json = {
        .type = JSON_TYPE_NUMBER,
        .data = NULL
    };

    assert(arr->data);
    list_find(arr->data, pos, &json);
    num = *(int *)json.data;

    json_xfree(json.data);
    return num;
}

void arr_qsort(JSONArray *arr, int (*compare_fn)(const void *, const void *))
{
    assert(arr->data);
    list_qsort(arr->data, compare_fn);
}

JSONArrayIter arr_begin(const JSONArray *arr)
{
    assert(arr->data);
    return list_begin(arr->data);
}

JSONArrayIter arr_end(const JSONArray *arr)
{
    assert(arr->data);
    return list_end(arr->data);
}

JSONArrayIter arr_iterate(JSONArrayIter iter)
{
    return list_iterate(iter);
}

JSONArrayIter arr_rbegin(const JSONArray *arr)
{
    assert(arr->data);
    return list_rbegin(arr->data);
}

JSONArrayIter arr_rend(const JSONArray *arr)
{
    assert(arr->data);
    return list_rend(arr->data);
}

JSONArrayIter arr_riterate(JSONArrayIter iter)
{
    return list_riterate(iter);
}

JSONString *str_assign_cstr(char *val)
{
    JSONString *d;

    d = json_xmallocz(sizeof *d);
    JSON_STRING_CLASS(d, json_xmallocz(strlen(val) + 1));

    strcat(d->data, val);

    return d;
}

JSONString str_assign(char *val)
{
    JSONString d;

    JSON_STRING_CLASS(&d, json_xmallocz(strlen(val) + 1));
    strcat(d.data, val);

    return d;
}

JSONString *str_data_cstr(void *data)
{
    JSONString *d;

    d = json_xmallocz(sizeof *d);
    JSON_STRING_CLASS(d, data);

    return d;
}

JSONString str_data(void *data)
{
    JSONString d;
    JSON_STRING_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe JSON or JSONString */
JSONString *str_copy_cstr(const void *val)
{
    const JSONString *s;
    JSONString *d;

    s = val;
    d = json_xmallocz(sizeof *d);
    JSON_STRING_CLASS(d, json_xmallocz(strlen(s->data) + 1));

    assert(s->data);
    assert(s->type == d->type);
    strcat(d->data, s->data);

    return d;
}

JSONString str_copy(const void *val)
{
    const JSONString *s;
    JSONString d;

    s = val;
    JSON_STRING_CLASS(&d, json_xmallocz(strlen(s->data) + 1));

    assert(s->data);
    assert(s->type == d.type);
    strcat(d.data, s->data);

    return d;
}

void str_set(JSONString *str, const char *val)
{
    assert(str->data);
    json_xfree(str->data);

    str->data = json_xmallocz(strlen(val) + 1);
    strcat(str->data, val);
}

char *str_get(const JSONString *str)
{
    assert(str->data);
    char *chars = json_xmallocz(strlen(str->data) + 1);
    strcat(chars, str->data);

    return chars;
}

JSONNumber *num_assign_cstr(int val)
{
    JSONNumber *d;

    d = json_xmallocz(sizeof *d);
    JSON_NUMBER_CLASS(d, json_xmallocz(sizeof (int)));

    *(int *)d->data = val;

    return d;
}

JSONNumber num_assign(int val)
{
    JSONNumber d;

    JSON_NUMBER_CLASS(&d, json_xmallocz(sizeof (int)));
    *(int *)d.data = val;

    return d;
}

JSONNumber *num_data_cstr(void *data)
{
    JSONNumber *d;

    d = json_xmallocz(sizeof *d);
    JSON_NUMBER_CLASS(d, data);

    return d;
}

JSONNumber num_data(void *data)
{
    JSONNumber d;
    JSON_NUMBER_CLASS(&d, data);

    return d;
}

/* TYPE of VAL maybe JSON or JSONNumber */
JSONNumber *num_copy_cstr(const void *val)
{
    const JSONNumber *s;
    JSONNumber *d;

    s = val;
    d = json_xmallocz(sizeof *d);
    JSON_NUMBER_CLASS(d, json_xmallocz(sizeof (int)));

    assert(s->data);
    assert(s->type == d->type);
    *(int *)d->data = *(int *)s->data;

    return d;
}

void num_set(JSONNumber *num, int val)
{
    assert(num->data);
    *(int *)num->data = val;
}

int num_get(const JSONNumber *num)
{
    assert(num->data);
    return *(int *)num->data;
}

JSONTrue *true_default_cstr()
{
    JSONTrue *d;

    d = json_xmallocz(sizeof *d);
    JSON_TRUE_CLASS(d);

    return d;
}

JSONTrue true_default()
{
    JSONTrue d;
    JSON_TRUE_CLASS(&d);

    return d;
}

JSONFalse *false_default_cstr()
{
    JSONFalse *d;

    d = json_xmallocz(sizeof *d);
    JSON_FALSE_CLASS(d);

    return d;
}

JSONFalse false_default()
{
    JSONFalse d;
    JSON_FALSE_CLASS(&d);

    return d;
}

JSONNull *null_default_cstr()
{
    JSONNull *d;

    d = json_xmallocz(sizeof *d);
    JSON_NULL_CLASS(d);

    return d;
}

JSONNull null_default()
{
    JSONNull d;
    JSON_NULL_CLASS(&d);

    return d;
}