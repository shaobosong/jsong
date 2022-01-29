#include "lib/bd_xjson_api.h"
#include <stdlib.h>
#include <string.h>

#define TEST_EXPECT(__val, __cmpr) \
do \
{ \
    if(__val == __cmpr) {;} \
    else { MY_ASSERT(__val == __cmpr); } \
} while (0)

/* this method only for test */
void* get_json_data(bd_xjson_null* json)
{
    return json->data;
}

/* this method only for test */
bd_xjson_type get_json_type(bd_xjson_null* json)
{
    return json->type;
}

void test_json_null_create_and_remove(void)
{
    /* create a json null */
    JSON_NULL(json);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_NULL);
    /* remove a json null */
    FREE_JSON(json);
}

void test_json_null_stringify()
{
    char* str = NULL;
    int len;
    JSON_NULL(json);

    /* stringify a json null */
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "null"), 0);
    free(str);

    FREE_JSON(json);
}

void test_parse_json_null()
{
    int res;
    char* str = NULL;
    JSON_NULL(json);

    /* parse a json null */
    str = "null";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_NULL);

    str = " \t\n\rnull \t\n\r";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_NULL);

    str = " n ull";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw null";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "nubutdrawll";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "null butdraw";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);
}

int main(int argc, char* argv[])
{
    test_json_null_create_and_remove();
    test_json_null_stringify();
    test_parse_json_null();
    THROW_WARNING("All tests pass");
    return 0;
}
