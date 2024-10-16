#ifndef JSON_H
#define JSON_H

typedef struct JSON JSON;

enum {
    JSON_TYPE_OBJECT = 1,
    JSON_TYPE_STRING,
    JSON_TYPE_NUMBER,
    JSON_TYPE_ARRAY,
    JSON_TYPE_TRUE,
    JSON_TYPE_FALSE,
    JSON_TYPE_NULL
};

/*
 *  JSON class
 *  All classes in json_impl.h are based on it.
 *
 *  @data: data of json
 *    object:  JSONHashTable *
 *    string:  char *
 *    number:  int *
 *    array:   JSONList *
 *  @type: type of json
 */
#define JSONClass(klass) \
struct klass {           \
    void *data;          \
    int type;            \
}
JSONClass(JSON);

/* private */
void json_copy(JSON *dst, const JSON *src);

/* public */
int json_reassign(void *dst, const void *src);
int json_free(void *json);
void json_free_data(JSON *json);
int json_stringify(const void *json, char **pstr, int *plen);
int json_parse(const char *str, void *json);

#endif