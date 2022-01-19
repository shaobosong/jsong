#ifndef bd_xjson_alloc_h
#define bd_xjson_alloc_h

#include "lib/bd_xjson.h"

bd_xjson_object* for_json_object(void);
bd_xjson_object* for_json_object_copy(bd_xjson_object* copy);

bd_xjson_array* for_json_array(void);
bd_xjson_array* for_json_array_copy(bd_xjson_array* copy);

bd_xjson_string* for_json_string(char* value);

bd_xjson_number* for_json_number(int value);

bd_xjson* for_json_literal(bd_xjson_type type);

#endif