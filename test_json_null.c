#include "libjson.h"
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
    JSONNull* json = JSON_NULL();
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_NULL);
    /* remove a json null */
    JSON_FREE(json);
}

void test_json_null_stringify()
{
    char* str = NULL;
    int len;
    JSONNull* json = JSON_NULL();

    /* stringify a json null */
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "null"), 0);
    free(str);

    JSON_FREE(json);
}

void test_parse_json_null()
{
    int res;
    char* str = NULL;
    JSONNull* json = JSON_NULL();

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
    printf("All tests pass\n");
    return 0;
}
