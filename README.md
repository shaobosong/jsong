# bd_xjson
A simple json library for c language
# compile
./compile.h
# usage
```
#include "lib/bd_xjson_api.h"
int main()
{
    JSON_OBJECT(obj);
    JSON_ARRAY(arr);
    JSON_NUMBER(num, 2022);
    JSON_NUMBER(max, 2147483647);
    JSON_STRING(str, "");

    /* add a key-value in json OBJECT */
    obj->add(obj, "key0", num);
    obj->add_str(obj, "key1", "this is a `string` message");
    obj->add_num(obj, "key2", 1993);
    obj->add_true(obj, "key3");
    obj->add_false(obj, "key4");
    obj->add_null(obj, "key5");
    /* update a key-value in json OBJECT */
    obj->update(obj, "key0", max);
    /* search a key-value in json OBJECT */
    obj->search(obj, "key1", str);
    /* delete a key-value in json OBJECT */
    obj->delete(obj, "key0");

    /* add a value in tail end of json ARRAY */
    arr->add(arr, -1, num);
    arr->add_str(arr, -1, "this is second `string` message");
    arr->add_num(arr, -1, 1993);
    arr->add_true(arr, -1);
    arr->add_false(arr, -1);
    arr->add_null(arr, -1);
    /* update a value in json ARRAY */
    arr->update(arr, 0, max);
    /* search a value in json ARRAY */
    arr->search(arr, 2, num);
    /* delete a value in json ARRAY */
    arr->delete(arr, -1);

    /* stringify a json object */
    char* json_str = bd_xjson_stringify(obj);

    /* parse json string to a json object */
    int res = bd_xjson_parse("{\"status\":0, \"list\":[{\"fullname\":\"/bin\", \"type\":4, \"mode\":\"drwx--x--x\"}]}", obj);
    /* you can print your result in stdout */
    /* printf("%s\n", bd_xjson_stringify(obj)); */

    /* free json obejct */
    FREE_JSON(obj);
    FREE_JSON(arr);
    FREE_JSON(num);
    FREE_JSON(str);
    FREE_JSON(max);

    return 0;
}
```
