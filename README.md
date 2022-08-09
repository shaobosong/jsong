# jsong
A simple json library for c language
# compile
make
# usage
```
#include "libjson.h"
#include <stdlib.h>
#include <stdio.h>
int main()
{
    /* create a empty json OBJECT */
    JSONObject* json_obj = JSON_OBJECT_PTR();
    /* create a empty json ARRAY */
    JSONArray* json_arr = JSON_ARRAY_PTR();
    /* create a json NUMBER */
    JSONNumber* json_num =  JSON_NUMBER_PTR(2022);
    JSONNumber* json_max = JSON_NUMBER_PTR(2147483647);
    /* create a json STRING */
    JSONString* json_str = JSON_STRING_PTR("");

    /* add a key-value in json OBJECT */
    json_obj->add(json_obj, "key0", json_num);
    json_obj->add_str(json_obj, "key1", "this is a `string` message");
    json_obj->add_num(json_obj, "key2", 1993);
    json_obj->add_true(json_obj, "key3");
    json_obj->add_false(json_obj, "key4");
    json_obj->add_null(json_obj, "key5");
    /* set a key-value in json OBJECT */
    json_obj->set(json_obj, "key0", json_max);
    json_obj->set_num(json_obj, "key1", 2022);
    json_obj->set_str(json_obj, "key2", "this is a `set` test");
    json_obj->set_null(json_obj, "key3");
    json_obj->set_true(json_obj, "key4");
    json_obj->set_false(json_obj, "key5");
    /* get a key-value in json OBJECT */
    json_obj->get(json_obj, "key2", json_str);
    int num = json_obj->get_num(json_obj, "key1");
    printf("key1: %d\n", num);
    char* chars = json_obj->get_str(json_obj, "key2");
    printf("key2: %s\n", chars);
    free(chars);
    /* delete a key-value in json OBJECT */
    json_obj->del(json_obj, "key0");

    /* add a value in tail end of json ARRAY */
    json_arr->add(json_arr, -1, json_num);
    json_arr->add_str(json_arr, -1, "this is second `string` message");
    json_arr->add_num(json_arr, -1, 1993);
    json_arr->add_true(json_arr, -1);
    json_arr->add_false(json_arr, -1);
    json_arr->add_null(json_arr, -1);
    /* set a value in json ARRAY */
    json_arr->set(json_arr, 0, json_max);
    json_arr->set_num(json_arr, 1, 2022);
    json_arr->set_str(json_arr, 2, "this is a `set` test");
    json_arr->set_null(json_arr, 3);
    json_arr->set_true(json_arr, -2);
    json_arr->set_false(json_arr, -1);
    /* get a value in json ARRAY */
    json_arr->get(json_arr, 1, json_num);
    num = json_arr->get_num(json_arr, 1);
    chars = json_arr->get_str(json_arr, 2);
    free(chars);
    /* delete a value in json ARRAY */
    json_arr->del(json_arr, -1);

    /* stringify a json */
    char* str;
    int len;
    JSON_STRINGIFY(json_obj, &str, &len);
    printf("%s\n", str);
    free(str);

    /* parse json string to a json object */
    str =
    "{"
        "\"object\":"
            "{"
                "\"\":\"\""
            "},"
        "\"array\":"
            "[],"
        "\"false\":false,"
        "\"string\":\"this is a string\","
        "\"true\":true,"
        "\"null\":null,"
        "\"number\":2022"
    "}";
    printf("%s\n", str);
    if(JSON_PARSE(str, json_obj))
    {
        printf("json parse failed\n");
    }
    JSON_STRINGIFY(json_obj, &str, &len);
    printf("%s\n", str);

    /* free json obejct */
    FREE_JSON(json_obj);
    FREE_JSON(json_arr);
    FREE_JSON(json_num);
    FREE_JSON(json_str);
    FREE_JSON(json_max);

    return 0;
}
```
