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
void* get_json_data(bd_xjson_true* json)
{
    return json->data;
}

/* this method only for test */
bd_xjson_type get_json_type(bd_xjson_true* json)
{
    return json->type;
}

void test_json_true_create_and_remove(void)
{
    /* create a json true */
    JSON_TRUE(json);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_TRUE);
    /* remove a json true */
    FREE_JSON(json);
}

void test_json_true_stringify()
{
    char* str = NULL;
    int len;
    JSON_TRUE(json);

    /* stringify a json true */
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "true"), 0);
    free(str);

    FREE_JSON(json);
}

void test_parse_json_true()
{
    int res;
    char* str = NULL;
    JSON_TRUE(json);

    /* parse a json true */
    str = "true";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_TRUE);

    str = " \t\n\rtrue \t\n\r";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    TEST_EXPECT(get_json_data(json), 0);
    TEST_EXPECT(get_json_type(json), BD_XJSON_TRUE);

    str = " t rue";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw true";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "trbutdrawue";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "true butdraw";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);
}

int main(int argc, char* argv[])
{
    test_json_true_create_and_remove();
    test_json_true_stringify();
    test_parse_json_true();
    THROW_WARNING("All tests pass");
    return 0;
}
