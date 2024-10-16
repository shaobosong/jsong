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
int get_json_data(JSONNumber* json)
{
    return *(int*)json->data;
}

/* only for test */
int get_json_type(JSON *json)
{
    return json->type;
}

void test_json_number_create_and_remove(void)
{
    /* create a json number */
    JSONNumber* json = JSON_NUMBER_PTR(2022);
    TEST_EXPECT(get_json_data(json), 2022);
    TEST_EXPECT(get_json_type((JSON *)json), JSON_TYPE_NUMBER);
    /* remove a json number */
    FREE_JSON(json);
}

void test_json_number_set_and_get(void)
{
    JSONNumber* json = JSON_NUMBER_PTR(2022);

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

    FREE_JSON(json);
}

void test_json_number_stringify()
{
    char* str = NULL;
    int len;
    JSONNumber* json = JSON_NUMBER_PTR(2022);

    /* stringify a json number */
    json_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "2022"), 0);
    free(str);

    /* stringify a json number */
    json->set(json, -2147483648);
    json_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "-2147483648"), 0);
    free(str);

    FREE_JSON(json);
}

void test_parse_json_number()
{
    int res;
    char* str = NULL;
    JSONNumber* json = JSON_NUMBER_PTR(0);

    /* parse a json number */
    str = "2022";
    res = json_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(json->get(json), 2022);
    TEST_EXPECT(get_json_type((JSON *)json), JSON_TYPE_NUMBER);

    str = " \t\n\r-2147483648 \t\n\r";
    res = json_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(json->get(json), -2147483648);
    TEST_EXPECT(get_json_type((JSON *)json), JSON_TYPE_NUMBER);

    str = " 2 022";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw 2022";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "20butdraw22";
    res = json_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "2022 butdraw";
    res = json_parse(str, json);
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
