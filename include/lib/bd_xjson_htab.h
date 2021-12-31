#ifndef bd_xjson_htab_h
#define bd_xjson_htab_h

#include <stddef.h>
#include "lib/bd_xjson_base.h"

#define DEFAULT_CAPACITY 16

typedef struct bd_xjson_entry bd_xjson_entry;
typedef struct bd_xjson_htab bd_xjson_htab;

struct bd_xjson_entry
{
    char* key;
    bd_xjson value;
};
struct bd_xjson_htab
{
    bd_xjson_entry* entries;
    int capacity;
    size_t size;
};
int htab_init(bd_xjson_htab** htab, size_t capacity);
int htab_grow(bd_xjson_htab* htab);
int htab_copy(bd_xjson_htab* dest, bd_xjson_htab* src);
int htab_insert(bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_delete(bd_xjson_htab* htab, const char* key);
int htab_search(bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_update(bd_xjson_htab* htab, const char* key, bd_xjson* val);

#endif