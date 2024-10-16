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
void* get_json_data(JSONNull* json)
{
    return json->data;
}

/* only for test */
int get_json_type(JSONNull* json)
{
    return json->type;
}

void test_json_null_create_and_remove(void)
{
    /* create a json null */
    JSONNull* json = JSON_NULL_PTR();
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), JSON_TYPE_NULL);
    /* remove a json null */
    FREE_JSON(json);
}

void test_json_null_stringify()
{
    char* str = NULL;
    int len;
    JSONNull* json = JSON_NULL_PTR();

    /* stringify a json null */
    json_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "null"), 0);
    free(str);

    FREE_JSON(json);
}

void test_parse_json_null()
{
    int res;
    char* str = NULL;
    JSONNull* json = JSON_NULL_PTR();

    /* parse a json null */
    str = "null";
    res = json_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), JSON_TYPE_NULL);

    str = " \t\n\rnull \t\n\r";
    res = json_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), JSON_TYPE_NULL);

    str = " n ull";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw null";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "nubutdrawll";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "null butdraw";
    res = json_parse(str, json);
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
