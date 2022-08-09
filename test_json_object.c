#include "libjson.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TEST_EXPECT(__val, __cmpr) \
do \
{ \
    if(__val == __cmpr) {;} \
    else { assert(__val == __cmpr); } \
} while (0)

/* only for test */
typedef struct jsong_node jsong_node;
typedef struct jsong_list
{
    jsong_node* head, * tail, * nil;
    int size;
} jsong_list;

/* only for test */
typedef struct jsong_entry jsong_entry;
typedef struct jsong_htab
{
    jsong_entry* entries;
    uint64_t capacity, size, first, last;
} jsong_htab;

/* only for test */
jsong_node* get_json_array_list_head(jsong_array* arr)
{
    return ((jsong_list*)(arr->data))->head;
}

/* only for test */
jsong_node* get_json_array_list_tail(jsong_array* arr)
{
    return ((jsong_list*)(arr->data))->tail;
}

/* only for test */
int get_json_array_list_size(jsong_array* arr)
{
    return ((jsong_list*)(arr->data))->size;
}

/* for test */
int get_json_object_htab_size(jsong_object* obj)
{
    return ((jsong_htab*)(obj->data))->size;
}

/* only for test */
jsong_type get_json_type(jsong* json)
{
    return json->type;
}

void test_json_object_create_and_remove(void)
{
     /* create a object */
    JSONObject* json = JSON_OBJECT_PTR();
    TEST_EXPECT(get_json_object_htab_size(json), 0);
    TEST_EXPECT(get_json_type((jsong*)json), JSONG_OBJECT);
    /* remove a object */
    FREE_JSON(json);
}

void test_json_object_set_and_get_json_string(void)
{
    char *str, *str_get;

    /* create a json string */
    JSONString* json_str = JSON_STRING_PTR("butdraw");
    JSONString* json_str_get = JSON_STRING_PTR("");

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* set a json string into json object */
    json_obj->set(json_obj, "name", json_str);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json string in this position */
    json_str_get = json_obj->get(json_obj, "name", json_str_get);
    str = json_str->get(json_str);
    str_get = json_str_get->get(json_str_get);
    TEST_EXPECT(strcmp(str, str_get), 0);
    TEST_EXPECT(json_str_get->type, JSONG_STRING);
    free(str);
    free(str_get);

    /* get a string directly in this position */
    str = json_str->get(json_str);
    str_get = json_obj->get_str(json_obj, "name");
    TEST_EXPECT(strcmp(str, str_get), 0);
    free(str);
    free(str_get);

    /* set a string directly into json object */
    json_obj->set_str(json_obj, "name", "butdraw butdraw");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json string by key */
    json_str_get = json_obj->get(json_obj, "name", json_str_get);
    str_get = json_str_get->get(json_str_get);
    TEST_EXPECT(strcmp("butdraw butdraw", str_get), 0);
    TEST_EXPECT(json_str_get->type, JSONG_STRING);
    free(str_get);

    FREE_JSON(json_str);
    FREE_JSON(json_str_get);
    FREE_JSON(json_obj);
}

void test_json_object_set_and_get_json_number(void)
{
    int num, num_get;

    /* create a json number */
    JSONNumber* json_num = JSON_NUMBER_PTR(2022);
    JSONNumber* json_num_get = JSON_NUMBER_PTR(0);

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* set a json number into json object */
    json_obj->set(json_obj, "age", json_num);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json number by key */
    json_num_get = json_obj->get(json_obj, "age", json_num_get);
    num = json_num->get(json_num);
    num_get = json_num_get->get(json_num_get);
    TEST_EXPECT(num, num_get);
    TEST_EXPECT(json_num_get->type, JSONG_NUMBER);

    /* get a number directly by key */
    num = json_num->get(json_num);
    num_get = json_obj->get_num(json_obj, "age");
    TEST_EXPECT(num, num_get);

    /* set a number directly into json object */
    json_obj->set_num(json_obj, "age", 2099);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json number by key */
    json_num_get = json_obj->get(json_obj, "age", json_num_get);
    num_get = json_num_get->get(json_num_get);
    TEST_EXPECT(2099, num_get);
    TEST_EXPECT(json_num_get->type, JSONG_NUMBER);

    FREE_JSON(json_num);
    FREE_JSON(json_num_get);
    FREE_JSON(json_obj);
}

void test_json_object_set_and_get_json_true(void)
{
    /* create a json true */
    JSONTrue* json_true = JSON_TRUE_PTR();
    JSONTrue* json_true_get = JSON_TRUE_PTR();

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* set a json true into json object */
    json_obj->set(json_obj, "isadult", json_true);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json true */
    json_true_get = json_obj->get(json_obj, "isadult", json_true_get);
    TEST_EXPECT(json_true_get->data, 0);
    TEST_EXPECT(json_true_get->type, JSONG_TRUE);

    FREE_JSON(json_true);
    FREE_JSON(json_true_get);
    FREE_JSON(json_obj);
}

void test_json_object_set_and_get_json_false(void)
{
    /* create a json false */
    JSONFalse* json_false = JSON_FALSE_PTR();
    JSONFalse* json_false_get = JSON_FALSE_PTR();

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* set a json false into json object */
    json_obj->set(json_obj, "isadult", json_false);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json false */
    json_false_get = json_obj->get(json_obj, "isadult", json_false_get);
    TEST_EXPECT(json_false_get->data, 0);
    TEST_EXPECT(json_false_get->type, JSONG_FALSE);

    FREE_JSON(json_false);
    FREE_JSON(json_false_get);
    FREE_JSON(json_obj);
}

void test_json_object_set_and_get_json_null(void)
{
    /* create a json null */
    JSONNull* json_null = JSON_NULL_PTR();
    JSONNull* json_null_get = JSON_NULL_PTR();

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* set a json null into json object */
    json_obj->set(json_obj, "other information", json_null);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json null */
    json_null_get = json_obj->get(json_obj, "other information", json_null_get);
    TEST_EXPECT(json_null_get->data, 0);
    TEST_EXPECT(json_null_get->type, JSONG_NULL);

    FREE_JSON(json_null);
    FREE_JSON(json_null_get);
    FREE_JSON(json_obj);
}

void test_json_object_set_and_get_json_object(void)
{
    char stack[256], compr[256];
    memset(stack, 0, 256);
    memset(compr, 0, 256);

    /* create a sub json object */
    JSONObject* sub_json_obj = JSON_OBJECT_PTR();
    /* add some <key>-<value> for test in sub_json_obj */
    sub_json_obj->set_str(sub_json_obj, "name", "butdraw");
    sub_json_obj->set_str(sub_json_obj, "age", "29");
    sub_json_obj->set_str(sub_json_obj, "id", "a0000001234");
    JSONObject* sub_json_obj_get = JSON_OBJECT_PTR(); /* {} */
    JSONObjectIter iter, end;

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* set a sub json object into json object */
    json_obj->set(json_obj, "student01", sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json object */
    sub_json_obj_get = json_obj->get(json_obj, "student01", sub_json_obj_get);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj_get), 3);
    /* traverse all elements in sub_json_obj */
    iter = sub_json_obj->begin(sub_json_obj);
    end = sub_json_obj->end(sub_json_obj);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, iter.key);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_obj_get */
    iter = sub_json_obj_get->begin(sub_json_obj_get);
    end = sub_json_obj_get->end(sub_json_obj_get);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, iter.key);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_obj_get->type, JSONG_OBJECT);
    printf("-------------------\n");

    sub_json_obj->set_str(sub_json_obj, "name", "yoyo");
    sub_json_obj->set_str(sub_json_obj, "age", "29");
    sub_json_obj->set_str(sub_json_obj, "id", "a0000005210");

    /* set a sub json object into json object */
    json_obj->set(json_obj, "student01", sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json object */
    sub_json_obj_get = json_obj->get(json_obj, "student01", sub_json_obj_get);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj_get), 3);
    /* traverse all elements in sub_json_obj */
    iter = sub_json_obj->begin(sub_json_obj);
    end = sub_json_obj->end(sub_json_obj);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, iter.key);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_obj_get */
    iter = sub_json_obj_get->begin(sub_json_obj_get);
    end = sub_json_obj_get->end(sub_json_obj_get);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, iter.key);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_obj_get->type, JSONG_OBJECT);
    printf("-------------------\n");

    FREE_JSON(json_obj);
    FREE_JSON(sub_json_obj);
    FREE_JSON(sub_json_obj_get);
}

void test_json_object_set_and_get_json_array(void)
{
    char stack[256], compr[256];
    memset(stack, 0, 256);
    memset(compr, 0, 256);

    /* create a sub json array */
    JSONArray* sub_json_arr = JSON_ARRAY_PTR();
    /* add some <value> for test in sub_json_arr */
    sub_json_arr->add_str(sub_json_arr, -1, "apple");
    sub_json_arr->add_str(sub_json_arr, -1, "watermelon");
    sub_json_arr->add_str(sub_json_arr, -1, "grape");
    JSONArray* sub_json_arr_get = JSON_ARRAY_PTR(); /* [] */
    JSONArrayIter iter, end;

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* set a sub json array into json object */
    json_obj->set(json_obj, "fruit", sub_json_arr);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json array */
    sub_json_arr_get = json_obj->get(json_obj, "fruit", sub_json_arr_get);
    TEST_EXPECT(get_json_array_list_size(sub_json_arr_get), 3);

    /* traverse all elements in sub_json_arr */
    iter = sub_json_arr->begin(sub_json_arr);
    end = sub_json_arr->end(sub_json_arr);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_obj_get */
    iter = sub_json_arr_get->begin(sub_json_arr_get);
    end = sub_json_arr_get->end(sub_json_arr_get);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_arr_get->type, JSONG_ARRAY);
    printf("-------------------\n");

    sub_json_arr->set_str(sub_json_arr, 0, "lemon");
    sub_json_arr->set_str(sub_json_arr, 1, "mango");
    sub_json_arr->set_str(sub_json_arr, 2, "pitaya");

    /* set a sub json array into json object */
    json_obj->set(json_obj, "fruit", sub_json_arr);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json array */
    sub_json_arr_get = json_obj->get(json_obj, "fruit", sub_json_arr_get);
    TEST_EXPECT(get_json_array_list_size(sub_json_arr_get), 3);

    /* traverse all elements in sub_json_arr */
    iter = sub_json_arr->begin(sub_json_arr);
    end = sub_json_arr->end(sub_json_arr);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_arr_get */
    iter = sub_json_arr_get->begin(sub_json_arr_get);
    end = sub_json_arr_get->end(sub_json_arr_get);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_arr_get->type, JSONG_ARRAY);
    printf("-------------------\n");

    FREE_JSON(json_obj);
    FREE_JSON(sub_json_arr);
    FREE_JSON(sub_json_arr_get);
}

void test_json_object_add_json_string(void)
{
    char *str, *str_get;

    /* create a json string */
    JSONString* json_str = JSON_STRING_PTR("butdraw");
    JSONString* json_str_get = JSON_STRING_PTR("");

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add a json string into json object */
    json_obj->add(json_obj, "name", json_str);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json string in this position */
    json_str_get = json_obj->get(json_obj, "name", json_str_get);
    str = json_str->get(json_str);
    str_get = json_str_get->get(json_str_get);
    TEST_EXPECT(strcmp(str, str_get), 0);
    TEST_EXPECT(json_str_get->type, JSONG_STRING);
    free(str);
    free(str_get);

    /* get a string directly in this position */
    str = json_str->get(json_str);
    str_get = json_obj->get_str(json_obj, "name");
    TEST_EXPECT(strcmp(str, str_get), 0);
    free(str);
    free(str_get);

    /* add a string directly into json object */
    json_obj->add_str(json_obj, "age", "29");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json string by key */
    json_str_get = json_obj->get(json_obj, "age", json_str_get);
    str_get = json_str_get->get(json_str_get);
    TEST_EXPECT(strcmp("29", str_get), 0);
    TEST_EXPECT(json_str_get->type, JSONG_STRING);
    free(str_get);

    FREE_JSON(json_str);
    FREE_JSON(json_str_get);
    FREE_JSON(json_obj);
}

void test_json_object_add_json_number(void)
{
    int num, num_get;

    /* create a json number */
    JSONNumber* json_num = JSON_NUMBER_PTR(2022);
    JSONNumber* json_num_get = JSON_NUMBER_PTR(0);

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add a json number into json object */
    json_obj->add(json_obj, "age", json_num);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json number by key */
    json_num_get = json_obj->get(json_obj, "age", json_num_get);
    num = json_num->get(json_num);
    num_get = json_num_get->get(json_num_get);
    TEST_EXPECT(num, num_get);
    TEST_EXPECT(json_num_get->type, JSONG_NUMBER);

    /* get a number directly by key */
    num = json_num->get(json_num);
    num_get = json_obj->get_num(json_obj, "age");
    TEST_EXPECT(num, num_get);

    /* add a number directly into json object */
    json_obj->add_num(json_obj, "level", 2099);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json number by key */
    json_num_get = json_obj->get(json_obj, "level", json_num_get);
    num_get = json_num_get->get(json_num_get);
    TEST_EXPECT(2099, num_get);
    TEST_EXPECT(json_num_get->type, JSONG_NUMBER);

    FREE_JSON(json_num);
    FREE_JSON(json_num_get);
    FREE_JSON(json_obj);
}

void test_json_object_add_json_true(void)
{
    /* create a json true */
    JSONTrue* json_true = JSON_TRUE_PTR();
    JSONTrue* json_true_get = JSON_TRUE_PTR();

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add a json true into json object */
    json_obj->add(json_obj, "isadult", json_true);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json true */
    json_true_get = json_obj->get(json_obj, "isadult", json_true_get);
    TEST_EXPECT(json_true_get->data, 0);
    TEST_EXPECT(json_true_get->type, JSONG_TRUE);

    /* add a json true into json object directly */
    json_obj->add_true(json_obj, "isstudent");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json true */
    json_true_get = json_obj->get(json_obj, "isstudent", json_true_get);
    TEST_EXPECT(json_true_get->data, 0);
    TEST_EXPECT(json_true_get->type, JSONG_TRUE);

    FREE_JSON(json_true);
    FREE_JSON(json_true_get);
    FREE_JSON(json_obj);
}

void test_json_object_add_json_false(void)
{
    /* create a json false */
    JSONFalse* json_false = JSON_FALSE_PTR();
    JSONFalse* json_false_get = JSON_FALSE_PTR();

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add a json false into json object */
    json_obj->add(json_obj, "isadult", json_false);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json false */
    json_false_get = json_obj->get(json_obj, "isadult", json_false_get);
    TEST_EXPECT(json_false_get->data, 0);
    TEST_EXPECT(json_false_get->type, JSONG_FALSE);

    /* add a json false into json object directly */
    json_obj->add_false(json_obj, "isstudent");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json flase */
    json_false_get = json_obj->get(json_obj, "isstudent", json_false_get);
    TEST_EXPECT(json_false_get->data, 0);
    TEST_EXPECT(json_false_get->type, JSONG_FALSE);

    FREE_JSON(json_false);
    FREE_JSON(json_false_get);
    FREE_JSON(json_obj);
}

void test_json_object_add_json_null(void)
{
    /* create a json null */
    JSONNull* json_null = JSON_NULL_PTR();
    JSONNull* json_null_get = JSON_NULL_PTR();

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add a json null into json object */
    json_obj->add(json_obj, "isadult", json_null);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json null */
    json_null_get = json_obj->get(json_obj, "isadult", json_null_get);
    TEST_EXPECT(json_null_get->data, 0);
    TEST_EXPECT(json_null_get->type, JSONG_NULL);

    /* add a json null into json object directly */
    json_obj->add_null(json_obj, "isstudent");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json null */
    json_null_get = json_obj->get(json_obj, "isstudent", json_null_get);
    TEST_EXPECT(json_null_get->data, 0);
    TEST_EXPECT(json_null_get->type, JSONG_NULL);

    FREE_JSON(json_null);
    FREE_JSON(json_null_get);
    FREE_JSON(json_obj);
}

void test_json_object_add_json_object(void)
{
    char stack[256], compr[256];
    memset(stack, 0, 256);
    memset(compr, 0, 256);

    /* create a sub json object */
    JSONObject* sub_json_obj = JSON_OBJECT_PTR();
    /* add some <key>-<value> for test in sub_json_obj */
    sub_json_obj->set_str(sub_json_obj, "name", "butdraw");
    sub_json_obj->set_str(sub_json_obj, "age", "29");
    sub_json_obj->set_str(sub_json_obj, "id", "a0000001234");
    JSONObject* sub_json_obj_get = JSON_OBJECT_PTR(); /* {} */
    JSONObjectIter iter, end;

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add a sub json object into json object */
    json_obj->add(json_obj, "student01", sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json object */
    sub_json_obj_get = json_obj->get(json_obj, "student01", sub_json_obj_get);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj_get), 3);
    /* traverse all elements in sub_json_obj */
    iter = sub_json_obj->begin(sub_json_obj);
    end = sub_json_obj->end(sub_json_obj);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, iter.key);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_obj_get */
    iter = sub_json_obj_get->begin(sub_json_obj_get);
    end = sub_json_obj_get->end(sub_json_obj_get);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, iter.key);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_obj_get->type, JSONG_OBJECT);
    printf("-------------------\n");

    sub_json_obj->set_str(sub_json_obj, "name", "yoyo");
    sub_json_obj->set_str(sub_json_obj, "age", "29");
    sub_json_obj->set_str(sub_json_obj, "id", "a0000005210");

    /* add a sub json object into json object */
    json_obj->add(json_obj, "student02", sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json object */
    sub_json_obj_get = json_obj->get(json_obj, "student02", sub_json_obj_get);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj_get), 3);
    /* traverse all elements in sub_json_obj */
    iter = sub_json_obj->begin(sub_json_obj);
    end = sub_json_obj->end(sub_json_obj);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, iter.key);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_obj_get */
    iter = sub_json_obj_get->begin(sub_json_obj_get);
    end = sub_json_obj_get->end(sub_json_obj_get);
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, iter.key);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_obj_get->type, JSONG_OBJECT);
    printf("-------------------\n");

    FREE_JSON(json_obj);
    FREE_JSON(sub_json_obj);
    FREE_JSON(sub_json_obj_get);
}

void test_json_object_add_json_array(void)
{
    char stack[256], compr[256];
    memset(stack, 0, 256);
    memset(compr, 0, 256);

    /* create a sub json array */
    JSONArray* sub_json_arr = JSON_ARRAY_PTR();
    /* add some <value> for test in sub_json_arr */
    sub_json_arr->add_str(sub_json_arr, -1, "apple");
    sub_json_arr->add_str(sub_json_arr, -1, "watermelon");
    sub_json_arr->add_str(sub_json_arr, -1, "grape");
    JSONArray* sub_json_arr_get = JSON_ARRAY_PTR(); /* [] */
    JSONArrayIter iter, end;

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add a sub json array into json object */
    json_obj->add(json_obj, "fruit", sub_json_arr);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json array */
    sub_json_arr_get = json_obj->get(json_obj, "fruit", sub_json_arr_get);
    TEST_EXPECT(get_json_array_list_size(sub_json_arr_get), 3);

    /* traverse all elements in sub_json_arr */
    iter = sub_json_arr->begin(sub_json_arr);
    end = sub_json_arr->end(sub_json_arr);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_obj_get */
    iter = sub_json_arr_get->begin(sub_json_arr_get);
    end = sub_json_arr_get->end(sub_json_arr_get);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_arr_get->type, JSONG_ARRAY);
    printf("-------------------\n");

    sub_json_arr->set_str(sub_json_arr, 0, "lemon");
    sub_json_arr->set_str(sub_json_arr, 1, "mango");
    sub_json_arr->set_str(sub_json_arr, 2, "pitaya");

    /* add a sub json array into json object */
    json_obj->add(json_obj, "fruit02", sub_json_arr);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json array */
    sub_json_arr_get = json_obj->get(json_obj, "fruit02", sub_json_arr_get);
    TEST_EXPECT(get_json_array_list_size(sub_json_arr_get), 3);

    /* traverse all elements in sub_json_arr */
    iter = sub_json_arr->begin(sub_json_arr);
    end = sub_json_arr->end(sub_json_arr);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(stack, get);
    }
    /* traverse all elements in sub_json_arr_get */
    iter = sub_json_arr_get->begin(sub_json_arr_get);
    end = sub_json_arr_get->end(sub_json_arr_get);
    JSON_ARRAY_FOREACH(iter, end)
    {
        char* get;
        get = iter.value.data;
        printf("value: %s\n", get);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    TEST_EXPECT(sub_json_arr_get->type, JSONG_ARRAY);
    printf("-------------------\n");

    FREE_JSON(json_obj);
    FREE_JSON(sub_json_arr);
    FREE_JSON(sub_json_arr_get);
}

void test_json_object_delete_json(void)
{
    char *str;
    int num;

    char stack[256], compr[256];
    memset(stack, 0, 256);
    memset(compr, 0, 256);

    int i;
    char* values[3] =
    {
        "butdraw",
        "29",
        "a0000001234"
    };

    /* create a json object */
    JSONObject* sub_json_obj = JSON_OBJECT_PTR();
    /* add some <key>-<value> for test in json_obj */
    sub_json_obj->set_str(sub_json_obj, "name", values[0]);
    sub_json_obj->set_str(sub_json_obj, "age", values[1]);
    sub_json_obj->set_str(sub_json_obj, "id", values[2]);
    JSONObject* sub_json_obj_get = JSON_OBJECT_PTR(); /* {} */
    JSONObjectIter oiter, oend;

    /* create a sub json array */
    JSONArray* sub_json_arr = JSON_ARRAY_PTR();
    sub_json_arr->add_str(sub_json_arr, -1, values[0]);
    sub_json_arr->add_str(sub_json_arr, -1, values[1]);
    sub_json_arr->add_str(sub_json_arr, -1, values[2]);
    JSONArray* sub_json_arr_get = JSON_ARRAY_PTR(); /* [] */
    JSONArrayIter aiter, aend;

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add some strings directly into json array in head */
    json_obj->add_str(json_obj, "string", "this is first test");
    json_obj->add_num(json_obj, "number", 2022);
    json_obj->add_true(json_obj, "true");
    json_obj->add_false(json_obj, "false");
    json_obj->add_null(json_obj, "null");
    json_obj->add(json_obj, "object", sub_json_obj);
    json_obj->add(json_obj, "array", sub_json_arr);
    TEST_EXPECT(get_json_object_htab_size(json_obj), 7);

    /* get a string directly */
    str = json_obj->get_str(json_obj, "string");
    TEST_EXPECT(strcmp("this is first test", str), 0);
    free(str);

    /* delete a element */
    json_obj->del(json_obj, "string");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 6);

    /* get a number directly */
    num = json_obj->get_num(json_obj, "number");
    TEST_EXPECT(num, 2022);

    /* delete a element */
    json_obj->del(json_obj, "number");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 5);

    /* delete a element */
    json_obj->del(json_obj, "false");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 4);

    /* delete a element */
    json_obj->del(json_obj, "null");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 3);

    /* delete a element */
    json_obj->del(json_obj, "true");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 2);

    /* get a json object */
    sub_json_obj_get = json_obj->get(json_obj, "object", sub_json_obj_get);
    /* traverse all elements in sub_json_obj */
    oiter = sub_json_obj->begin(sub_json_obj);
    oend = sub_json_obj->end(sub_json_obj);
    JSON_OBJECT_FOREACH(oiter, oend)
    {
        char* get;
        printf("key: %s, ", oiter.key);
        get = oiter.value.data;
        printf("value: %s\n", get);
        strcat(stack, oiter.key);
        strcat(stack, get);
        i++;
    }
    /* traverse all elements in sub_json_obj_get */
    oiter = sub_json_obj_get->begin(sub_json_obj_get);
    oend = sub_json_obj_get->end(sub_json_obj_get);
    JSON_OBJECT_FOREACH(oiter, oend)
    {
        char* get;
        printf("key: %s, ", oiter.key);
        get = oiter.value.data;
        printf("value: %s\n", get);
        strcat(compr, oiter.key);
        strcat(compr, get);
    }
    TEST_EXPECT(strcmp(stack, compr), 0);
    memset(stack, 0, 256);
    memset(compr, 0, 256);
    TEST_EXPECT(sub_json_obj_get->type, JSONG_OBJECT);
    printf("-------------------\n");

    /* delete a element */
    json_obj->del(json_obj, "object");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 1);

    /* get a json array */
    sub_json_arr_get = json_obj->get(json_obj, "array", sub_json_arr_get);

    /* traverse all elements in sub_json_arr_get */
    aiter = sub_json_arr_get->begin(sub_json_arr_get);
    aend = sub_json_arr_get->end(sub_json_arr_get);
    i = 0;
    JSON_ARRAY_FOREACH(aiter, aend)
    {
        char* get;
        get = aiter.value.data;
        TEST_EXPECT(strcmp(get, values[i]), 0);
        i++;
    }
    TEST_EXPECT(sub_json_arr_get->type, JSONG_ARRAY);

    /* delete a array */
    json_obj->del(json_obj, "array");
    TEST_EXPECT(get_json_object_htab_size(json_obj), 0);

    FREE_JSON(json_obj);
    FREE_JSON(sub_json_arr);
    FREE_JSON(sub_json_arr_get);
    FREE_JSON(sub_json_obj);
    FREE_JSON(sub_json_obj_get);
}

void test_json_object_traverse_all_elements(void)
{
    JSONObjectIter iter, end;
    char* pchars[3] =
    {
        "this is a 1st test",
        "this is a 2nd test",
        "this is a 3rd test"
    };

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* add some elements into json object */
    json_obj->add_str(json_obj, "str1", pchars[0]);
    json_obj->add_str(json_obj, "str2", pchars[1]);
    json_obj->add_str(json_obj, "str3", pchars[2]);

    /* traverse */
    iter = json_obj->begin(json_obj);
    end = json_obj->end(json_obj);
    int i =  2;
    JSON_OBJECT_FOREACH(iter, end)
    {
        char* get;
        printf("key: %s, ", iter.key);
        get = iter.value.data;
        TEST_EXPECT(strcmp(pchars[i], get), 0);
        printf("value: %s\n", get);
        i--;
    }

    FREE_JSON(json_obj);
}

void test_json_object_stringify(void)
{
    char* str_json = NULL;
    int len;
    char* str =
    "{"
        "\"object\":"
            "{"
                "\"false\":false,"
                "\"string\":\"this is a string\","
                "\"true\":true,"
                "\"null\":null,"
                "\"number\":2022"
            "},"
        "\"false\":false,"
        "\"string\":\"this is a string\","
        "\"array\":"
        "["
            "\"string\","
            "2022,"
            "true,"
            "false,"
            "null,"
            "{"
                "\"true\":true,"
                "\"null\":null,"
                "\"false\":false,"
                "\"number\":2022,"
                "\"string\":\"this is a string\""
            "},"
            "["
                "\"string\","
                "2022,"
                "true,"
                "false,"
                "null,"
                "{"
                    "\"true\":true,"
                    "\"null\":null,"
                    "\"false\":false,"
                    "\"number\":2022,"
                    "\"string\":\"this is a string\""
                "}"
            "]"
        "],"
        "\"true\":true,"
        "\"null\":null,"
        "\"number\":2022"
    "}";
    printf("%s\n", str);

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    /* create a sub json object */
    JSONArray* sub_json_arr = JSON_ARRAY_PTR();
    sub_json_arr->add_str(sub_json_arr, -1, "string");
    sub_json_arr->add_num(sub_json_arr, -1, 2022);
    sub_json_arr->add_true(sub_json_arr, -1);
    sub_json_arr->add_false(sub_json_arr, -1);
    sub_json_arr->add_null(sub_json_arr, -1);

    /* add some elements into json object */
    json_obj->add_str(json_obj, "string", "this is a string");
    json_obj->add_num(json_obj, "number", 2022);
    json_obj->add_true(json_obj, "true");
    json_obj->add_false(json_obj, "false");
    json_obj->add_null(json_obj, "null");
    sub_json_arr->add(sub_json_arr, -1, json_obj);
    sub_json_arr->add(sub_json_arr, -1, sub_json_arr);
    json_obj->add(json_obj, "object", json_obj);
    json_obj->add(json_obj, "array", sub_json_arr);

    /* stringify a json array */
    jsong_stringify(json_obj, &str_json, &len);
    printf("%s\n", str_json);
    TEST_EXPECT(strcmp(str_json, str), 0 );
    free(str_json);

    FREE_JSON(sub_json_arr);
    FREE_JSON(json_obj);
}

void test_parse_json_object(void )
{
    char stack[256];
    memset(stack, 0, 256);
    int i;
    jsong_type types[7] =
    {
        JSONG_STRING,
        JSONG_NUMBER,
        JSONG_TRUE,
        JSONG_FALSE,
        JSONG_NULL,
        JSONG_OBJECT,
        JSONG_ARRAY
    };
    char* str =
    "{"
        "\"object\":"
            "{"
                "\"false\":false,"
                "\"string\":\"this is a string\","
                "\"true\":true,"
                "\"null\":null,"
                "\"number\":2022"
            "},"
        "\"false\":false,"
        "\"string\":\"this is a string\","
        "\"array\":"
        "["
            "\"this is a string\","
            "2022,"
            "true,"
            "false,"
            "null,"
            "{"
                "\"true\":true,"
                "\"null\":null,"
                "\"false\":false,"
                "\"number\":2022,"
                "\"string\":\"this is a string\""
            "},"
            "["
                "\"this is a string\","
                "2022,"
                "true,"
                "false,"
                "null,"
                "{"
                    "\"true\":true,"
                    "\"null\":null,"
                    "\"false\":false,"
                    "\"number\":2022,"
                    "\"string\":\"this is a string\""
                "}"
            "]"
        "],"
        "\"true\":true,"
        "\"null\":null,"
        "\"number\":2022"
    "}";
    JSONString* json_str = JSON_STRING_PTR("");
    JSONNumber* json_num = JSON_NUMBER_PTR(0);
    JSONObject* sub_json_obj = JSON_OBJECT_PTR();
    JSONArray* sub_json_arr = JSON_ARRAY_PTR();
    JSONArrayIter aiter, aend;

    /* create a json object */
    JSONObject* json_obj = JSON_OBJECT_PTR();

    jsong_parse(str, json_obj);

    /* check data */
    char* str_json = json_obj->get_str(json_obj, "string");
    TEST_EXPECT(strcmp("this is a string", str_json), 0);
    free(str_json);

    int num = json_obj->get_num(json_obj, "number");
    TEST_EXPECT(num, 2022);

    sub_json_obj = json_obj->get(json_obj, "object", sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj), 5);

    sub_json_arr = json_obj->get(json_obj, "array", sub_json_arr);
    TEST_EXPECT(get_json_array_list_size(sub_json_arr), 7);
    aiter = sub_json_arr->begin(sub_json_arr);
    aend = sub_json_arr->end(sub_json_arr);
    i = 0;
    /* check sub json array types */
    JSON_ARRAY_FOREACH(aiter, aend)
    {
        TEST_EXPECT(types[i], aiter.value.type);
        i++;
    }
    /* check sub json array data */
    str_json = sub_json_arr->get_str(sub_json_arr, 0);
    TEST_EXPECT(strcmp("this is a string", str_json), 0);
    free(str_json);

    num = sub_json_arr->get_num(sub_json_arr, 1);
    TEST_EXPECT(num, 2022);

    sub_json_obj = sub_json_arr->get(sub_json_arr, 5, sub_json_obj);
    TEST_EXPECT(get_json_object_htab_size(sub_json_obj), 5);

    FREE_JSON(json_obj);
    FREE_JSON(json_str);
    FREE_JSON(json_num);
    FREE_JSON(sub_json_arr);
    FREE_JSON(sub_json_obj);
}

int main(int argc, char* argv[])
{
    test_json_object_create_and_remove();
    test_json_object_set_and_get_json_string();
    test_json_object_set_and_get_json_number();
    test_json_object_set_and_get_json_true();
    test_json_object_set_and_get_json_false();
    test_json_object_set_and_get_json_null();
    test_json_object_set_and_get_json_object();
    test_json_object_set_and_get_json_array();
    test_json_object_add_json_string();
    test_json_object_add_json_number();
    test_json_object_add_json_true();
    test_json_object_add_json_false();
    test_json_object_add_json_null();
    test_json_object_add_json_object();
    test_json_object_add_json_array();
    test_json_object_delete_json();
    test_json_object_traverse_all_elements();
    test_json_object_stringify();
    test_parse_json_object();
    printf("All tests pass\n");
    return 0;
}
