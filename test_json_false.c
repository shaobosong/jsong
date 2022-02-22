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
void* get_json_data(bd_xjson_false* json)
{
    return json->data;
}

/* this method only for test */
bd_xjson_type get_json_type(bd_xjson_false* json)
{
    return json->type;
}

void test_json_false_create_and_remove(void)
{
    /* create a json false */
    JSONFalse* json = JSON_FALSE();
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_FALSE);
    /* remove a json false */
    JSON_FREE(json);
}

void test_json_false_stringify()
{
    char* str = NULL;
    int len;
    JSONFalse* json = JSON_FALSE();

    /* stringify a json false */
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "false"), 0);
    free(str);

    JSON_FREE(json);
}

void test_parse_json_false()
{
    int res;
    char* str = NULL;
    JSONFalse* json = JSON_FALSE();

    /* parse a json false */
    str = "false";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_FALSE);

    str = " \t\n\rfalse \t\n\r";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_FALSE);

    str = " f alse";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw false";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "fabutdrawlse";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "false butdraw";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);
}

int main(int argc, char* argv[])
{
    test_json_false_create_and_remove();
    test_json_false_stringify();
    test_parse_json_false();
    printf("All tests pass\n");
    return 0;
}