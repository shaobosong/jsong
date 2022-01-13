# bd_xjson
A simple json library for c language
# compile
./compile.h
# usage
#include "lib/bd_xjson_api.h"
int main()
{
    JSON_OBJECT(obj);
    JSON_ARRAY(arr);
    JSON_NUMBER_ASSIGN(number, 2022);
    JSON_NUMBER_ASSIGN(maxint, 2147483647);
    JSON_STRING_ASSIGN(string, "good morning");

    /* add a key-value in json OBJECT */
    obj->add(obj, "key0", number);
    /* update a key-value in json OBJECT */
    obj->update(obj, "key0", maxint);
    /* search a key-value in json OBJECT */
    obj->search(obj, "key0", number);
    /* delete a key-value in json OBJECT */
    obj->delete(obj, "key0");

    /* add a value in tail end of json ARRAY */
    arr->add(arr, -1, number);
    /* update a value in json ARRAY */
    arr->update(arr, 0, maxint);
    /* search a value in json ARRAY */
    arr->search(arr, -1, number);
    /* delete a value in json ARRAY */
    arr->delete(arr, -1);

    /* stringify a json object */
    char* json_str = bd_xjson_stringify(obj);

    /* parse json string to a json object */
    int res = bd_xjson_parse("{\"status\":0, \"list\":[{\"fullname\":\"/bin\", \"type\":4, \"mode\":\"drwx--x--x\"}]}", obj);
    /* you can print your result in stdout */
    /* printf("%s\n", bd_xjson_stringify(obj)); */
    
    return 0;
}
