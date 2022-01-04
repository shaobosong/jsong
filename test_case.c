#include <mcheck.h>
#include <stdlib.h>
#include "lib/bd_xjson_api.h"
#include <string.h>

int main(int argc, char* argv[])
{
    setenv("MALLOC_TRACE", "./log", 1);
    mtrace();
    // JSON_STRING_ASSIGN(str, "this is a test");
    // JSON_NUMBER_ASSIGN(num, 2021);
    // JSON_FALSE(false);

    // JSON_ARRAY(arr_sub0);
    // arr_sub0->add(arr_sub0, -1, str);
    // arr_sub0->add(arr_sub0,  -1, false);

    // JSON_ARRAY(arr_sub1);
    // arr_sub0->add(arr_sub1, 0, false);
    // arr_sub0->add(arr_sub1,  -1, num);

    // JSON_ARRAY(arr_sub2);
    // arr_sub2->add(arr_sub2, -1, num);

    // JSON_ARRAY(arr);
    // arr->add(arr, -1, arr_sub0);
    // arr->add(arr, -1, arr_sub1);
    // arr->add(arr, -1, str);
    // arr->add(arr, -1, arr_sub2);
    // JSON_ARRAY_COPY(copy, arr);
    // arr->delete(arr, -4);
    // arr->delete(arr, -3);
    // arr->delete(arr, -2);
    // arr->delete(arr, -1);

    // FREE_JSON(copy);
    // FREE_JSON(arr);
    // FREE_JSON(arr_sub2);
    // FREE_JSON(arr_sub1);
    // FREE_JSON(arr_sub0);
    // FREE_JSON(false);
    // FREE_JSON(num);
    // FREE_JSON(str);

    JSON_OBJECT(obj);
    JSON_FALSE(var_false);
    obj->add(obj, "false", var_false);
    JSON_NUMBER_ASSIGN(number, 6);
    obj->add(obj, "number", number);
    obj->delete(obj, "number");
    JSON_OBJECT_COPY(objcpy, obj);
    FREE_JSON(number);
    FREE_JSON(var_false);
    FREE_JSON(obj);
    FREE_JSON(objcpy);
    muntrace();

    return 0;
}
