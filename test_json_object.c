#include "lib/bd_xjson_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_EXPECT(__val, __cmpr) \
do \
{ \
    if(__val == __cmpr) {;} \
    else { MY_ASSERT(__val == __cmpr); } \
} while (0)

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

void test_json_object_create_and_remove(void)
{
     /* create a object */
    JSON_OBJECT(json);
    TEST_EXPECT(get_json_object_htab_size(json), 0);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_OBJECT);
    /* remove a object */
    FREE_JSON(json);
}

int main(int argc, char* argv[])
{

    test_json_object_create_and_remove();
    JSON_OBJECT(obj);
    obj->add_str(obj, "name", "songshaobo");
    obj->add_num(obj, "age", 25);
    obj->add_str(obj, "id", "e0004756");
    obj->add_str(obj, "phone", "18817174040");

    JSON_OBJECT_ITER iter = obj->begin(obj);
    JSON_OBJECT_ITER end = obj->end(obj);
    JSON_OBJECT_FOREACH(obj, iter, end)
    {
        printf("key: %s\n", iter.data.key);
    }
    FREE_JSON(obj);
    return 0;
}