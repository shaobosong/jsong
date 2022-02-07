#include "lib/bd_xjson_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define COUNT 1000
#define TEST_EXPECT(__val, __cmpr) \
do \
{ \
    if(__val == __cmpr) {;} \
    else { MY_ASSERT(__val == __cmpr); } \
} while (0)

static int numcmp(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

/* this method only for test */
bd_xjson_node* get_json_array_list_head(bd_xjson_array* arr)
{
    return ((bd_xjson_list*)(arr->data))->head;
}

/* this method only for test */
bd_xjson_node* get_json_array_list_tail(bd_xjson_array* arr)
{
    return ((bd_xjson_list*)(arr->data))->tail;
}

/* this method only for test */
int get_json_array_list_size(bd_xjson_array* arr)
{
    return ((bd_xjson_list*)(arr->data))->size;
}

/* this method only for test */
int get_json_object_htab_size(bd_xjson_object* obj)
{
    return ((bd_xjson_htab*)(obj->data))->size;
}

/* this method only for test */
bd_xjson_type get_json_type(bd_xjson* json)
{
    return json->type;
}

void test_json_array_create_and_remove(void)
{
    /* create a array */
    JSON_ARRAY(json);
    TEST_EXPECT(get_json_array_list_head(json), NULL);
    TEST_EXPECT(get_json_array_list_tail(json), NULL);
    TEST_EXPECT(get_json_array_list_size(json), 0);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_ARRAY);
    /* remove a array */
    FREE_JSON(json);
}

void test_json_array_set_and_get_json_string(void)
{
    char *str, *str_get;

    /* create a json string */
    JSON_STRING(json_str, "this is first test");
    JSON_STRING(json_str_get, "");

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* set a json string into json array if empty in this position */
    json_arr->set(json_arr, 0, json_str);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json string in this position */
    json_str_get = json_arr->get(json_arr, 0, json_str_get);
    str = json_str->get(json_str);
    str_get = json_str_get->get(json_str_get);
    TEST_EXPECT(strcmp(str, str_get), 0);
    TEST_EXPECT(json_str_get->type, BD_XJSON_STRING);
    free(str);
    free(str_get);

    /* get a string directly in this position */
    str = json_str->get(json_str);
    str_get = json_arr->get_str(json_arr, 0);
    TEST_EXPECT(strcmp(str, str_get), 0);
    free(str);
    free(str_get);

    /* set a string directly into json array if empty in this position */
    json_arr->set_str(json_arr, 0, "this is second test");
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json string in this position */
    json_str_get = json_arr->get(json_arr, 0, json_str_get);
    str_get = json_str_get->get(json_str_get);
    TEST_EXPECT(strcmp("this is second test", str_get), 0);
    TEST_EXPECT(json_str_get->type, BD_XJSON_STRING);
    free(str_get);

    FREE_JSON(json_str);
    FREE_JSON(json_str_get);
    FREE_JSON(json_arr);
}

void test_json_array_set_and_get_json_number(void)
{
    int num, num_get;

    /* create a json number */
    JSON_NUMBER(json_num, 2022);
    JSON_NUMBER(json_num_get, 0);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* set a json number into json array if empty in this position */
    json_arr->set(json_arr, 0, json_num);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json number in this position */
    json_num_get = json_arr->get(json_arr, 0, json_num_get);
    num = json_num->get(json_num);
    num_get = json_num_get->get(json_num_get);
    TEST_EXPECT(num, num_get);
    TEST_EXPECT(json_num_get->type, BD_XJSON_NUMBER);

    /* get a number directly in this position */
    num = json_num->get(json_num);
    num_get = json_arr->get_num(json_arr, 0);
    TEST_EXPECT(num, num_get);

    /* set a number directly into json array if empty in this position */
    json_arr->set_num(json_arr, 0, 2099);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json number in this position */
    json_num_get = json_arr->get(json_arr, 0, json_num_get);
    num_get = json_num_get->get(json_num_get);
    TEST_EXPECT(2099, num_get);
    TEST_EXPECT(json_num_get->type, BD_XJSON_NUMBER);

    FREE_JSON(json_num);
    FREE_JSON(json_num_get);
    FREE_JSON(json_arr);
}

void test_json_array_set_and_get_json_true(void)
{
    /* create a json true */
    JSON_TRUE(json_true);
    JSON_TRUE(json_true_get);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* set a json true into json array if empty in this position */
    json_arr->set(json_arr, 0, json_true);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json true in this position */
    json_true_get = json_arr->get(json_arr, 0, json_true_get);
    TEST_EXPECT(json_true_get->data, 0);
    TEST_EXPECT(json_true_get->type, BD_XJSON_TRUE);

    FREE_JSON(json_true);
    FREE_JSON(json_true_get);
    FREE_JSON(json_arr);
}

void test_json_array_set_and_get_json_false(void)
{
    /* create a json false */
    JSON_FALSE(json_false);
    JSON_FALSE(json_false_get);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* set a json false into json array if empty in this position */
    json_arr->set(json_arr, 0, json_false);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json false in this position */
    json_false_get = json_arr->get(json_arr, 0, json_false_get);
    TEST_EXPECT(json_false_get->data, 0);
    TEST_EXPECT(json_false_get->type, BD_XJSON_FALSE);

    FREE_JSON(json_false);
    FREE_JSON(json_false_get);
    FREE_JSON(json_arr);
}

void test_json_array_set_and_get_json_null(void)
{
    /* create a json null */
    JSON_NULL(json_null);
    JSON_NULL(json_null_get);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* set a json null into json array if empty in this position */
    json_arr->set(json_arr, 0, json_null);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json null in this position */
    json_null_get = json_arr->get(json_arr, 0, json_null_get);
    TEST_EXPECT(json_null_get->data, 0);
    TEST_EXPECT(json_null_get->type, BD_XJSON_NULL);

    FREE_JSON(json_null);
    FREE_JSON(json_null_get);
    FREE_JSON(json_arr);
}

void test_json_array_set_and_get_json_object(void)
{
    char stack[256], compr[256];
    bzero(stack, 256);
    bzero(compr, 256);

    /* create a json object */
    JSON_OBJECT(json_obj);
    /* add some <key>-<value> for test in json_obj */
    json_obj->set_str(json_obj, "name", "butdraw");
    json_obj->set_str(json_obj, "age", "29");
    json_obj->set_str(json_obj, "id", "a0000001234");
    JSON_OBJECT(json_obj_get); /* {} */
    JSON_OBJECT_ITER iter, end;

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* set(add) a json object into json array if empty in this position */
    json_arr->set(json_arr, 0, json_obj);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json object in this position */
    json_obj_get = json_arr->get(json_arr, 0, json_obj_get);
    TEST_EXPECT(get_json_object_htab_size(json_obj_get), 3);
    /* traverse all elements in json_obj */
    iter = json_obj->begin(json_obj);
    end = json_obj->end(json_obj);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(iter);
        printf("value: %s\n", get);
        strcat(stack, iter.data.key);
        strcat(stack, get);
        free(get);
    }
    /* traverse all elements in json_obj_get */
    iter = json_obj_get->begin(json_obj_get);
    end = json_obj_get->end(json_obj_get);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(iter);
        printf("value: %s\n", get);
        strcat(compr, iter.data.key);
        strcat(compr, get);
        free(get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(json_obj_get->type, BD_XJSON_OBJECT);
    printf("-------------------\n");

    FREE_JSON(json_obj);
    FREE_JSON(json_obj_get);
    FREE_JSON(json_arr);
}

void test_json_array_set_and_get_json_array(void)
{
    int i;
    char* values[3] =
    {
        "butdraw",
        "29",
        "a0000001234"
    };

    /* create a sub json array */
    JSON_ARRAY(sub_json_arr);
    /* add some <value> for test in sub_json_arr */
    sub_json_arr->set_str(sub_json_arr, 0, values[0]);
    sub_json_arr->set_str(sub_json_arr, 1, values[1]);
    sub_json_arr->set_str(sub_json_arr, 2, values[2]);
    JSON_ARRAY(json_arr_get); /* [] */
    JSON_ARRAY_ITER iter, end;

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* set a json array into json array in head */
    json_arr->set(json_arr, 0, sub_json_arr);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json array in [0] */
    json_arr_get = json_arr->get(json_arr, 0, json_arr_get);
    TEST_EXPECT(get_json_array_list_size(sub_json_arr), 3);
    iter = json_arr_get->begin(json_arr_get);
    end = json_arr_get->end(json_arr_get);
    i = 0;
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = JSON_ARRAY_ITER_GET_STR(iter);
        TEST_EXPECT(strcmp(get, values[i]), 0);
        free(get);
        i++;
    }
    TEST_EXPECT(json_arr_get->type, BD_XJSON_ARRAY);

    FREE_JSON(json_arr);
    FREE_JSON(sub_json_arr);
    FREE_JSON(json_arr_get);
}

void test_json_array_add_json_string(void)
{
    char *str, *str_get;

    /* create a json string */
    JSON_STRING(json_str, "this is first test");
    JSON_STRING(json_str_get, "");

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add a json string into json array in head */
    json_arr->add(json_arr, 0, json_str);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* add a string directly into json array in head */
    json_arr->add_str(json_arr, 0, "this is second test");
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a string directly in [0] */
    str_get = json_arr->get_str(json_arr, 0);
    TEST_EXPECT(strcmp("this is second test", str_get), 0);
    free(str_get);

    /* get a string directly in [1] */
    str_get = json_arr->get_str(json_arr, 1);
    str = json_str->get(json_str);
    TEST_EXPECT(strcmp(str, str_get), 0);
    free(str);
    free(str_get);

    FREE_JSON(json_str);
    FREE_JSON(json_str_get);
    FREE_JSON(json_arr);
}

void test_json_array_add_json_number(void)
{
    int num, num_get;

    /* create a json number */
    JSON_NUMBER(json_num, 2022);
    JSON_NUMBER(json_num_get, 0);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add a json number into json array in head */
    json_arr->add(json_arr, 0, json_num);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* add a number directly into json array in head */
    json_arr->add_num(json_arr, 0, 2099);
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a number directly in [0] */
    num_get = json_arr->get_num(json_arr, 0);
    TEST_EXPECT(2099, num_get);

    /* get a number directly in [1] */
    num = json_num->get(json_num);
    num_get = json_arr->get_num(json_arr, 1);
    TEST_EXPECT(num, num_get);

    FREE_JSON(json_num);
    FREE_JSON(json_num_get);
    FREE_JSON(json_arr);
}

void test_json_array_add_json_true(void)
{
    /* create a json true */
    JSON_TRUE(json_true);
    JSON_TRUE(json_true_get);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add a json true into json array in head */
    json_arr->add(json_arr, 0, json_true);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* add a true directly into json array in head */
    json_arr->add_true(json_arr, 0);
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a json true in [0] */
    json_true_get = json_arr->get(json_arr, 0, json_true_get);
    TEST_EXPECT(json_true_get->data, 0);
    TEST_EXPECT(json_true_get->type, BD_XJSON_TRUE);

    /* get a json true in [1] */
    json_true_get = json_arr->get(json_arr, 1, json_true_get);
    TEST_EXPECT(json_true_get->data, 0);
    TEST_EXPECT(json_true_get->type, BD_XJSON_TRUE);

    FREE_JSON(json_true);
    FREE_JSON(json_true_get);
    FREE_JSON(json_arr);
}

void test_json_array_add_json_false(void)
{
    /* create a json false */
    JSON_TRUE(json_false);
    JSON_TRUE(json_false_get);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add a json false into json array in head */
    json_arr->add(json_arr, 0, json_false);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* add a false directly into json array in head */
    json_arr->add_true(json_arr, 0);
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a json false in [0] */
    json_false_get = json_arr->get(json_arr, 0, json_false_get);
    TEST_EXPECT(json_false_get->data, 0);
    TEST_EXPECT(json_false_get->type, BD_XJSON_TRUE);

    /* get a json false in [1] */
    json_false_get = json_arr->get(json_arr, 1, json_false_get);
    TEST_EXPECT(json_false_get->data, 0);
    TEST_EXPECT(json_false_get->type, BD_XJSON_TRUE);

    FREE_JSON(json_false);
    FREE_JSON(json_false_get);
    FREE_JSON(json_arr);
}

void test_json_array_add_json_null(void)
{
    /* create a json null */
    JSON_TRUE(json_null);
    JSON_TRUE(json_null_get);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add a json null into json array in head */
    json_arr->add(json_arr, 0, json_null);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* add a null directly into json array in head */
    json_arr->add_true(json_arr, 0);
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a json null in [0] */
    json_null_get = json_arr->get(json_arr, 0, json_null_get);
    TEST_EXPECT(json_null_get->data, 0);
    TEST_EXPECT(json_null_get->type, BD_XJSON_TRUE);

    /* get a json null in [1] */
    json_null_get = json_arr->get(json_arr, 1, json_null_get);
    TEST_EXPECT(json_null_get->data, 0);
    TEST_EXPECT(json_null_get->type, BD_XJSON_TRUE);

    FREE_JSON(json_null);
    FREE_JSON(json_null_get);
    FREE_JSON(json_arr);
}

void test_json_array_add_json_object(void)
{
    char stack[256], compr[256];
    bzero(stack, 256);
    bzero(compr, 256);

    /* create a json object */
    JSON_OBJECT(json_obj);
    /* add some <key>-<value> for test in json_obj */
    json_obj->set_str(json_obj, "name", "butdraw");
    json_obj->set_str(json_obj, "age", "29");
    json_obj->set_str(json_obj, "id", "a0000001234");
    JSON_OBJECT_COPY(json_obj_copy, json_obj);
    json_obj_copy->set_str(json_obj_copy, "height", "181");
    JSON_OBJECT(json_obj_get); /* {} */
    JSON_OBJECT_ITER iter, end;

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add a json object into json array in head */
    json_arr->add(json_arr, 0, json_obj);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* add a json object into json array in head */
    json_arr->add(json_arr, 0, json_obj_copy);
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a json object in [0] */
    json_obj_get = json_arr->get(json_arr, 0, json_obj_get);
    /* traverse all elements in json_obj_copy */
    iter = json_obj_copy->begin(json_obj_copy);
    end = json_obj_copy->end(json_obj_copy);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(iter);
        printf("value: %s\n", get);
        strcat(stack, iter.data.key);
        strcat(stack, get);
        free(get);
    }
    /* traverse all elements in json_obj_get */
    iter = json_obj_get->begin(json_obj_get);
    end = json_obj_get->end(json_obj_get);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(iter);
        printf("value: %s\n", get);
        strcat(compr, iter.data.key);
        strcat(compr, get);
        free(get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    bzero(stack, 256);
    bzero(compr, 256);
    TEST_EXPECT(json_obj_get->type, BD_XJSON_OBJECT);
    printf("-------------------\n");

    /* get a json object in [1] */
    json_obj_get = json_arr->get(json_arr, 1, json_obj_get);
    /* traverse all elements in json_obj */
    iter = json_obj->begin(json_obj);
    end = json_obj->end(json_obj);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(iter);
        printf("value: %s\n", get);
        strcat(stack, iter.data.key);
        strcat(stack, get);
        free(get);
    }
    /* traverse all elements in json_obj_get */
    iter = json_obj_get->begin(json_obj_get);
    end = json_obj_get->end(json_obj_get);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(iter);
        printf("value: %s\n", get);
        strcat(compr, iter.data.key);
        strcat(compr, get);
        free(get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    bzero(stack, 256);
    bzero(compr, 256);
    TEST_EXPECT(json_obj_get->type, BD_XJSON_OBJECT);
    printf("-------------------\n");

    FREE_JSON(json_obj);
    FREE_JSON(json_obj_get);
    FREE_JSON(json_obj_copy);
    FREE_JSON(json_arr);
}

void test_json_array_add_json_array(void)
{
    int i = 0;
    char* values[4] =
    {
        "butdraw",
        "29",
        "a0000001234",
        "181"
    };

    /* create a sub json array */
    JSON_ARRAY(sub_json_arr);
    /* add some <value> for test in sub_json_arr */
    sub_json_arr->set_str(sub_json_arr, 0, values[0]);
    sub_json_arr->set_str(sub_json_arr, 1, values[1]);
    sub_json_arr->set_str(sub_json_arr, 2, values[2]);
    JSON_ARRAY_COPY(sub_json_arr_copy, sub_json_arr);
    sub_json_arr_copy->set_str(sub_json_arr_copy, 3, values[3]);
    JSON_ARRAY(json_arr_get); /* [] */
    JSON_ARRAY_ITER iter, end;

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add a json array into json array in head */
    json_arr->add(json_arr, 0, sub_json_arr);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* add a json array into json array in head */
    json_arr->add(json_arr, 0, sub_json_arr_copy);
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a json array in [0] */
    json_arr_get = json_arr->get(json_arr, 0, json_arr_get);
    i = 0;
    /* traverse all elements in json_arr_get */
    iter = json_arr_get->begin(json_arr_get);
    end = json_arr_get->end(json_arr_get);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = JSON_ARRAY_ITER_GET_STR(iter);
        TEST_EXPECT(strcmp(get, values[i]), 0);
        free(get);
        i++;
    }
    TEST_EXPECT(json_arr_get->type, BD_XJSON_ARRAY);

    /* get a json array in [1] */
    json_arr_get = json_arr->get(json_arr, 1, json_arr_get);

    /* traverse all elements in json_arr_get */
    iter = json_arr_get->begin(json_arr_get);
    end = json_arr_get->end(json_arr_get);
    i = 0;
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = JSON_ARRAY_ITER_GET_STR(iter);
        TEST_EXPECT(strcmp(get, values[i]), 0);
        free(get);
        i++;
    }
    TEST_EXPECT(json_arr_get->type, BD_XJSON_ARRAY);

    FREE_JSON(json_arr);
    FREE_JSON(sub_json_arr);
    FREE_JSON(sub_json_arr_copy);
    FREE_JSON(json_arr_get);
}

void test_json_array_delete_json(void)
{
    char *str;
    int num;

    char stack[256], compr[256];
    bzero(stack, 256);
    bzero(compr, 256);

    int i;
    char* values[3] =
    {
        "butdraw",
        "29",
        "a0000001234"
    };

    /* create a json object */
    JSON_OBJECT(json_obj);
    /* add some <key>-<value> for test in json_obj */
    json_obj->set_str(json_obj, "name", values[0]);
    json_obj->set_str(json_obj, "age", values[1]);
    json_obj->set_str(json_obj, "id", values[2]);
    JSON_OBJECT(json_obj_get); /* {} */
    JSON_OBJECT_ITER oiter, oend;

    /* create a sub json array */
    JSON_ARRAY(sub_json_arr);
    sub_json_arr->add_str(sub_json_arr, -1, values[0]);
    sub_json_arr->add_str(sub_json_arr, -1, values[1]);
    sub_json_arr->add_str(sub_json_arr, -1, values[2]);
    JSON_ARRAY(sub_json_arr_get); /* [] */
    JSON_ARRAY_ITER aiter, aend;

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add some strings directly into json array in head */
    json_arr->add_str(json_arr, 0, "this is first test");
    json_arr->add_num(json_arr, 0, 2022);
    json_arr->add_true(json_arr, 0);
    json_arr->add_false(json_arr, 0);
    json_arr->add_null(json_arr, 0);
    json_arr->add(json_arr, 0, json_obj);
    json_arr->add(json_arr, 0, sub_json_arr);
    TEST_EXPECT(get_json_array_list_size(json_arr), 7);

    /* get a string directly in [-1] */
    str = json_arr->get_str(json_arr, -1);
    TEST_EXPECT(strcmp("this is first test", str), 0);
    free(str);

    /* delete a element in [-1] */
    json_arr->delete(json_arr, -1);
    TEST_EXPECT(get_json_array_list_size(json_arr), 6);

    /* get a number directly in [-1] */
    num = json_arr->get_num(json_arr, -1);
    TEST_EXPECT(num, 2022);

    /* delete a element in [-1] */
    json_arr->delete(json_arr, -1);
    TEST_EXPECT(get_json_array_list_size(json_arr), 5);

    /* delete a element in [-1] */
    json_arr->delete(json_arr, -1);
    TEST_EXPECT(get_json_array_list_size(json_arr), 4);

    /* delete a element in [-1] */
    json_arr->delete(json_arr, -1);
    TEST_EXPECT(get_json_array_list_size(json_arr), 3);

    /* delete a element in [-1] */
    json_arr->delete(json_arr, -1);
    TEST_EXPECT(get_json_array_list_size(json_arr), 2);

    /* get a json object in [-1] */
    json_obj_get = json_arr->get(json_arr, -1, json_obj_get);
    /* traverse all elements in json_obj */
    oiter = json_obj->begin(json_obj);
    oend = json_obj->end(json_obj);
    JSON_OBJECT_FOREACH(oiter, oend)
    {
        char* get;
        printf("key: %s, ", oiter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(oiter);
        printf("value: %s\n", get);
        strcat(stack, oiter.data.key);
        strcat(stack, get);
        free(get);
        i++;
    }
    /* traverse all elements in json_obj_get */
    oiter = json_obj_get->begin(json_obj_get);
    oend = json_obj_get->end(json_obj_get);
    JSON_OBJECT_FOREACH(oiter, oend)
    {
        char* get;
        printf("key: %s, ", oiter.data.key);
        get = JSON_OBJECT_ITER_GET_STR(oiter);
        printf("value: %s\n", get);
        strcat(compr, oiter.data.key);
        strcat(compr, get);
        free(get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    bzero(stack, 256);
    bzero(compr, 256);
    TEST_EXPECT(json_obj_get->type, BD_XJSON_OBJECT);
    printf("-------------------\n");

    /* delete a element in [-1] */
    json_arr->delete(json_arr, -1);
    TEST_EXPECT(get_json_array_list_size(json_arr), 1);

    /* get a json array in [-1] */
    sub_json_arr_get = json_arr->get(json_arr, -1, sub_json_arr_get);

    /* traverse all elements in sub_json_arr_get */
    aiter = sub_json_arr_get->begin(sub_json_arr_get);
    aend = sub_json_arr_get->end(sub_json_arr_get);
    i = 0;
    JSON_ARRAY_FOREACH(aiter, aend)
    {
        char* get;
        get = JSON_ARRAY_ITER_GET_STR(aiter);
        TEST_EXPECT(strcmp(get, values[i]), 0);
        free(get);
        i++;
    }
    TEST_EXPECT(sub_json_arr_get->type, BD_XJSON_ARRAY);

    /* delete a array in [-1] */
    json_arr->delete(json_arr, -1);
    TEST_EXPECT(get_json_array_list_size(json_arr), 0);
    TEST_EXPECT(get_json_array_list_head(json_arr), NULL);
    TEST_EXPECT(get_json_array_list_tail(json_arr), NULL);

    FREE_JSON(json_arr);
    FREE_JSON(sub_json_arr);
    FREE_JSON(sub_json_arr_get);
    FREE_JSON(json_obj);
    FREE_JSON(json_obj_get);
}

void test_json_array_quick_sort(void)
{
    int num;
    int arr[COUNT];
    JSON_ARRAY(json);

    /* add elements into json array in loop */
    for(int i = 0; i < COUNT; i++)
    {
        num = rand();
        arr[i] = num;
        json->add_num(json, -1, num);
    }

    /* quick sort for array and json array respectively */
    qsort(arr, COUNT, sizeof(int), numcmp);
    json->sort(json, numcmp);

    /* check result */
    for(int i = 0; i < COUNT; i++)
    {
        TEST_EXPECT(json->get_num(json, i), arr[i]);
    }

    FREE_JSON(json);
}

void test_json_array_traverse_all_elements(void)
{
    JSON_ARRAY_ITER iter, end;
    char* pchars[3] =
    {
        "this is a 1st test",
        "this is a 2nd test",
        "this is a 3rd test"
    };

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* add some elements into json array */
    json_arr->add_str(json_arr, -1, pchars[0]);
    json_arr->add_str(json_arr, -1, pchars[1]);
    json_arr->add_str(json_arr, -1, pchars[2]);

    /* traverse */
    iter = json_arr->begin(json_arr);
    end = json_arr->end(json_arr);
    int i =  0;
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = JSON_ARRAY_ITER_GET_STR(iter);
        TEST_EXPECT(strcmp(pchars[i], get), 0);
        free(get);
        i++;
    }

    /* traverse in reverse */
    iter = json_arr->rbegin(json_arr);
    end = json_arr->rend(json_arr);
    i = 2;
    JSON_ARRAY_REVERSE_FOREACH(iter, end)
    {
        char* get;
        get = JSON_ARRAY_ITER_GET_STR(iter);
        TEST_EXPECT(strcmp(pchars[i], get), 0);
        free(get);
        i--;
    }

    FREE_JSON(json_arr);
}

void test_json_array_stringify(void)
{
    char* str_json = NULL;
    int len;
    char* str = "[\"this is a test\",2022,true,false,null,{\"key\":\"value\"},"
        "[\"this is a test\",2022,true,false,null,{\"key\":\"value\"}]]";
    printf("%s\n", str);

    /* create a json array */
    JSON_ARRAY(json_arr);

    /* create a sub json object */
    JSON_OBJECT(json_obj);
    json_obj->add_str(json_obj, "key", "value");

    /* add some elements into json array */
    json_arr->add_str(json_arr, -1, "this is a test");
    json_arr->add_num(json_arr, -1, 2022);
    json_arr->add_true(json_arr, -1);
    json_arr->add_false(json_arr, -1);
    json_arr->add_null(json_arr, -1);
    json_arr->add(json_arr, -1, json_obj);
    json_arr->add(json_arr, -1, json_arr);

    /* stringify a json array */
    bd_xjson_stringify(json_arr, &str_json, &len);
    printf("%s\n", str_json);
    TEST_EXPECT(strcmp(str_json, str), 0 );
    free(str_json);

    FREE_JSON(json_arr);
    FREE_JSON(json_obj);
}

void test_parse_json_array(void )
{
    int i;
    bd_xjson_type types[7] =
    {
        BD_XJSON_STRING,
        BD_XJSON_NUMBER,
        BD_XJSON_TRUE,
        BD_XJSON_FALSE,
        BD_XJSON_NULL,
        BD_XJSON_OBJECT,
        BD_XJSON_ARRAY
    };
    char* str = "[\"this is a test\",2022,true,false,null,{\"key\":\"value\"},"
        "[\"this is a test\",2022,true,false,null,{\"key\":\"value\"}]]";
    JSON_STRING(json_str, "");
    JSON_NUMBER(json_num, 0);
    JSON_OBJECT(sub_json_obj);
    JSON_ARRAY(sub_json_arr);
    JSON_ARRAY_ITER aiter, aend;
    JSON_OBJECT_ITER oiter, oend;

    /* create a json array */
    JSON_ARRAY(json_arr);

    bd_xjson_parse(str, json_arr);
    /* check types */
    aiter = json_arr->begin(json_arr);
    aend = json_arr->end(json_arr);
    i = 0;
    JSON_ARRAY_FOREACH(aiter, aend)
    {
        TEST_EXPECT(types[i], aiter.data.value.type);
        i++;
    }
    /* check data */
    char* str_json = json_arr->get_str(json_arr, 0);
    TEST_EXPECT(strcmp("this is a test", str_json), 0);
    free(str_json);

    int num = json_arr->get_num(json_arr, 1);
    TEST_EXPECT(num, 2022);

    sub_json_obj = json_arr->get(json_arr, 5, sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj), 1);
    char stack[256];
    bzero(stack, 256);
    oiter = sub_json_obj->begin(sub_json_obj);
    oend = sub_json_obj->end(sub_json_obj);
    JSON_OBJECT_FOREACH(oiter, oend)
    {
        printf("key: %s, ", oiter.data.key);
        strcat(stack, oiter.data.key);
        char* get = JSON_OBJECT_ITER_GET_STR(oiter);
        printf("value: %s\n", get);
        strcat(stack, get);
        free(get);
    }
    TEST_EXPECT(strcmp(stack, "keyvalue"), 0);

    sub_json_arr = json_arr->get(json_arr, 6, sub_json_arr);
    TEST_EXPECT(get_json_array_list_size(sub_json_arr), 6);
    aiter = sub_json_arr->begin(sub_json_arr);
    aend = sub_json_arr->end(sub_json_arr);
    i = 0;
    /* check sub json array types */
    JSON_ARRAY_FOREACH(aiter, aend)
    {
        TEST_EXPECT(types[i], aiter.data.value.type);
        i++;
    }
    /* check sub json array data */
    str_json = json_arr->get_str(json_arr, 0);
    TEST_EXPECT(strcmp("this is a test", str_json), 0);
    free(str_json);

    num = json_arr->get_num(json_arr, 1);
    TEST_EXPECT(num, 2022);

    sub_json_obj = json_arr->get(json_arr, 5, sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj), 1);
    bzero(stack, 256);
    oiter = sub_json_obj->begin(sub_json_obj);
    oend = sub_json_obj->end(sub_json_obj);
    JSON_OBJECT_FOREACH(oiter, oend)
    {
        printf("key: %s, ", oiter.data.key);
        strcat(stack, oiter.data.key);
        char* get = JSON_OBJECT_ITER_GET_STR(oiter);
        printf("value: %s\n", get);
        strcat(stack, get);
        free(get);
    }
    TEST_EXPECT(strcmp(stack, "keyvalue"), 0);
    printf("-------------------\n");

    FREE_JSON(json_arr);
    FREE_JSON(json_str);
    FREE_JSON(json_num);
    FREE_JSON(sub_json_arr);
    FREE_JSON(sub_json_obj);
}

int main(int argc, char* argv[])
{
    test_json_array_create_and_remove();
    test_json_array_set_and_get_json_string();
    test_json_array_set_and_get_json_number();
    test_json_array_set_and_get_json_true();
    test_json_array_set_and_get_json_false();
    test_json_array_set_and_get_json_null();
    test_json_array_set_and_get_json_object();
    test_json_array_set_and_get_json_array();
    test_json_array_add_json_string();
    test_json_array_add_json_number();
    test_json_array_add_json_true();
    test_json_array_add_json_false();
    test_json_array_add_json_null();
    test_json_array_add_json_object();
    test_json_array_add_json_array();
    test_json_array_delete_json();
    test_json_array_quick_sort();
    test_json_array_traverse_all_elements();
    test_json_array_stringify();
    test_parse_json_array();
    THROW_WARNING("All tests pass");
    return 0;
}
