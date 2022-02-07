#include "lib/bd_xjson_alloc.h"
#include "lib/bd_xjson_impl.h"

bd_xjson_object* for_json_object(void)
{
    bd_xjson_object* obj = xzmalloc(sizeof *obj);
    BD_XJSON_OBJECT_CLASS(obj);
    obj_default_cstr(obj);
    return obj;
}
bd_xjson_object* for_json_object_copy(bd_xjson_object* copy)
{
    bd_xjson_object* obj = xzmalloc(sizeof *obj);
    BD_XJSON_OBJECT_CLASS(obj);
    obj_copy_cstr(obj, copy);
    return obj;
}

bd_xjson_string* for_json_string(char* value)
{
    bd_xjson_string* str = xzmalloc(sizeof *str);
    BD_XJSON_STRING_CLASS(str);
    str_assign_cstr(str, value);
    return str;
}

bd_xjson_number* for_json_number(int value)
{
    bd_xjson_number* num = xzmalloc(sizeof *num);
    BD_XJSON_NUMBER_CLASS(num);
    num_assign_cstr(num, value);
    return num;
}

bd_xjson_array* for_json_array(void)
{
    bd_xjson_array* arr = xzmalloc(sizeof *arr);
    BD_XJSON_ARRAY_CLASS(arr);
    arr_default_cstr(arr);
    return arr;
}
bd_xjson_array* for_json_array_copy(bd_xjson_array* copy)
{
    bd_xjson_array* arr = xzmalloc(sizeof *arr);
    BD_XJSON_ARRAY_CLASS(arr);
    arr_copy_cstr(arr, copy);
    return arr;
}

bd_xjson* for_json_literal(bd_xjson_type type)
{
    bd_xjson* literal = xzmalloc(sizeof *literal);
    literal->type = type;
    literal->data = NULL;
    return literal;
}