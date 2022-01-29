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
char* get_json_data(bd_xjson_string* json)
{
    return (char*)json->data;
}

/* this method only for test */
bd_xjson_type get_json_type(bd_xjson* json)
{
    return json->type;
}

void test_json_string_create_and_remove(void)
{
    /* create a json string */
    JSON_STRING(json, "2022,i love you");
    TEST_EXPECT(strcmp(get_json_data(json), "2022,i love you"), 0);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_STRING);
    /* remove a json string */
    FREE_JSON(json);
}

void test_json_string_set_and_get(void)
{
    char* get = NULL;
    JSON_STRING(json, "some characters");

    /* set */
    json->set(json, "another characters");
    TEST_EXPECT(strcmp(get_json_data(json), "another characters"), 0);

    /* get */
    get = json->get(json);
    TEST_EXPECT(strcmp(get, "another characters"), 0);
    free(get);

    /* set NULL-Terminated as string */
    json->set(json, "");
    TEST_EXPECT(strcmp(get_json_data(json), ""), 0);

    /* get this NULL-Terminated string */
    get = json->get(json);
    TEST_EXPECT(strcmp(get, ""), 0);
    free(get);

    FREE_JSON(json);
}

void test_json_string_stringify()
{
    char* str = NULL;
    int len;
    JSON_STRING(json, "some characters");

    /* stringify a json string */
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "\"some characters\""), 0);
    free(str);

    /* stringify a json string */
    json->set(json, "");
    bd_xjson_stringify(json, &str, &len);
    TEST_EXPECT(strcmp(str, "\"\""), 0);
    free(str);

    FREE_JSON(json);
}

void test_parse_json_string()
{
    int res;
    char* str = NULL;
    char* get = NULL;
    JSON_STRING(json, "");

    str = "\"some characters\"";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    get = json->get(json);
    TEST_EXPECT(strcmp(get, "some characters"), 0);
    free(get);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_STRING);

    str = "\"\"";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    get = json->get(json);
    TEST_EXPECT(strcmp(get, ""), 0);
    free(get);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_STRING);

    str = " \t\n\r\"some characters\" \t\n\r";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, 0);
    get = json->get(json);
    TEST_EXPECT(strcmp(get, "some characters"), 0);
    free(get);
    TEST_EXPECT(get_json_type((bd_xjson*)json), BD_XJSON_STRING);

    str = "\"some characters";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    /* unsupport NULL-Terminated in middle */
    str = "\"some\0characters\"";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "some characters\"";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "\"some characters\" butdraw";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);

    str = "butdraw \"some characters\"";
    res = bd_xjson_parse(str, json);
    TEST_EXPECT(res, -1);
}

int main(int argc, char* argv[])
{
    test_json_string_create_and_remove();
    test_json_string_set_and_get();
    test_json_string_stringify();
    test_parse_json_string();
    THROW_WARNING("All tests pass");
    return 0;
}
