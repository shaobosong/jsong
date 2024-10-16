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

/* only for test */
void* get_json_data(JSONFalse* json)
{
    return json->data;
}

/* only for test */
int get_json_type(JSONFalse* json)
{
    return json->type;
}

void test_json_false_create_and_remove(void)
{
    /* create a json false */
    JSONFalse* json = JSON_FALSE_PTR();
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), JSON_TYPE_FALSE);
    /* remove a json false */
    FREE_JSON(json);
}

void test_json_false_stringify()
{
    char* str = NULL;
    int len;
    JSONFalse* json = JSON_FALSE_PTR();

    /* stringify a json false */
    json_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "false"), 0);
    free(str);

    FREE_JSON(json);
}

void test_parse_json_false()
{
    int res;
    char* str = NULL;
    JSONFalse* json = JSON_FALSE_PTR();

    /* parse a json false */
    str = "false";
    res = json_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), JSON_TYPE_FALSE);

    str = " \t\n\rfalse \t\n\r";
    res = json_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), JSON_TYPE_FALSE);

    str = " f alse";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw false";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "fabutdrawlse";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "false butdraw";
    res = json_parse(str, json);
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
