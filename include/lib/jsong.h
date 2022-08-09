#ifndef JSONG_H
#define JSONG_H

typedef enum jsong_type jsong_type;
typedef struct jsong jsong;

enum jsong_type
{
    JSONG_OBJECT = 1,
    JSONG_STRING,
    JSONG_NUMBER,
    JSONG_ARRAY,
    JSONG_TRUE,
    JSONG_FALSE,
    JSONG_NULL
};

/*
 *  jsong class
 *      All classes in jsong_impl.h are based on it.
 *
 *  @data: data of json
 *      object:    jsong_htab*
 *      string:    char*
 *      number:    int*
 *      array:     jsong_list*
 *      true:      void*(0)
 *      false:     void*(0)
 *      null:      void*(0)
 *  @type: type of json
 *      object:    JSONG_OBJECT
 *      string:    JSONG_STRING
 *      number:    JSONG_NUMBER
 *      array:     JSONG_ARRAY
 *      true:      JSONG_TRUE
 *      false:     JSONG_FALSE
 *      null:      JSONG_NULL
 */
#define jsong(klass)    \
struct klass            \
{                       \
    void* data;         \
    jsong_type type;    \
}
jsong(jsong);

/* private */
void jsong_copy(jsong* dst, const jsong* src);

/* public */
int jsong_reassign(void* dst, const void* src);
int jsong_free(void* json);
void jsong_free_data(jsong* json);
int jsong_stringify(const void* json, char** pstr, int* plen);
int jsong_parse(const char* str, void* json);

#endif