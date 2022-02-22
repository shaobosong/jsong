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
int get_json_data(bd_xjson_number* json)
{
    return *(int*)json->data;
}

/* this method only for test */
bd_xjson_type get_json_type(bd_xjson* json)
{
    return json->type;
}

void test_json_number_create_and_remove(void)
{
    /* create a json number */
    JSONNumber* json = JSON_NUMBER(2022);
    TEST_EXPECT(get_json_data(json), 2022);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_NUMBER);
    /* remove a json number */
    JSON_FREE(json);
}

void test_json_number_set_and_get(void)
{
    JSONNumber* json = JSON_NUMBER(2022);

    /* set */
    json->set(json, 1993);
    TEST_EXPECT(get_json_data(json), 1993);

    /* get */
    TEST_EXPECT(json->get(json), 1993);

    /* set as threshold */
    json->set(json, 2147483647);
    json->set(json, -2147483648);

    /* get this threshold */
    TEST_EXPECT(json->get(json), -2147483648);

    JSON_FREE(json);
}

void test_json_number_stringify()
{
    char* str = NULL;
    int len;
    JSONNumber* json = JSON_NUMBER(2022);

    /* stringify a json number */
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "2022"), 0);
    free(str);

    /* stringify a json number */
    json->set(json, -2147483648);
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "-2147483648"), 0);
    free(str);

    JSON_FREE(json);
}

void test_parse_json_number()
{
    int res;
    char* str = NULL;
    JSONNumber* json = JSON_NUMBER(0);

    /* parse a json number */
    str = "2022";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(json->get(json), 2022);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_NUMBER);

    str = " \t\n\r-2147483648 \t\n\r";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(json->get(json), -2147483648);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_NUMBER);

    str = " 2 022";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw 2022";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "20butdraw22";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "2022 butdraw";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);
}

int main(int argc, char* argv[])
{
    test_json_number_create_and_remove();
    test_json_number_set_and_get();
    test_json_number_stringify();
    test_parse_json_number();
    printf("All tests pass\n");
    return 0;
}
