#include <mcheck.h>
#include <stdlib.h>
#include "lib/bd_xjson_api.h"

int main(int argc, char* argv[])
{
    setenv("MALLOC_TRACE", "./log", 1);
    mtrace();
    BD_XJSON_STRING_ASSIGN(str, "this is a test");
    BD_XJSON_NUMBER_ASSIGN(num, 2021);
    BD_XJSON_FALSE(false);

    BD_XJSON_ARRAY(arr_sub0);
    arr_sub0->add(arr_sub0, -1, str);
    arr_sub0->add(arr_sub0,  -1, false);

    BD_XJSON_ARRAY(arr_sub1);
    arr_sub0->add(arr_sub1, 0, false);
    arr_sub0->add(arr_sub1,  -1, num);

    BD_XJSON_ARRAY(arr_sub2);
    arr_sub2->add(arr_sub2, -1, num);

    BD_XJSON_ARRAY(arr);
    arr->add(arr, -1, arr_sub0);
    arr->add(arr, -1, arr_sub1);
    arr->add(arr, -1, str);
    arr->add(arr, -1, arr_sub2);
    BD_XJSON_ARRAY_COPY(copy, arr);
    arr->delete(arr, -4);
    arr->delete(arr, -3);
    arr->delete(arr, -2);
    arr->delete(arr, -1);

    FREE_BD_XJSON(copy);
    FREE_BD_XJSON(arr);
    FREE_BD_XJSON(arr_sub2);
    FREE_BD_XJSON(arr_sub1);
    FREE_BD_XJSON(arr_sub0);
    FREE_BD_XJSON(false);
    FREE_BD_XJSON(num);
    FREE_BD_XJSON(str);
    muntrace();

    return 0;
}
