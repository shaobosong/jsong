#ifndef bd_xjson_type_h
#define bd_xjson_type_h

typedef enum bd_xjson_type bd_xjson_type;
enum bd_xjson_type
{
    BD_XJSON_OBJECT = 1,
    BD_XJSON_STRING,
    BD_XJSON_NUMBER,
    BD_XJSON_ARRAY,
    BD_XJSON_TRUE,
    BD_XJSON_FALSE,
    BD_XJSON_NULL

};

#endif