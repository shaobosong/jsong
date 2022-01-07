#ifndef bd_xjson_htab_h
#define bd_xjson_htab_h

#include <stddef.h>
#include <stdint.h>
#include "lib/bd_xjson_base.h"

#define DEFAULT_CAPACITY 16

typedef struct bd_xjson_entry bd_xjson_entry;
typedef struct bd_xjson_htab bd_xjson_htab;
typedef struct bd_xjson_htab_iter bd_xjson_htab_iter;

struct bd_xjson_entry
{
    char* key;
    bd_xjson value;
};
struct bd_xjson_htab
{
    bd_xjson_entry* entries;
    uint64_t capacity;
    uint64_t size;
};
int htab_create(bd_xjson_htab** htab, uint64_t capacity);
int htab_copy(bd_xjson_htab* dest, bd_xjson_htab* src);
int htab_free(bd_xjson_htab* htab);
int htab_insert(bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_erase(bd_xjson_htab* htab, const char* key);
int htab_find(bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_update(bd_xjson_htab* htab, const char* key, bd_xjson* val);

struct bd_xjson_htab_iter
{
/* private */
    int64_t __index;
    uint64_t __count;
/* public */
    char* key;
    bd_xjson value;
};
bd_xjson_htab_iter htab_iterate(bd_xjson_htab* htab, bd_xjson_htab_iter it);

#define bd_xjson_htab_iter_init(it) \
do \
{ \
    it.__index = -1; \
    it.__count = 0; \
    it.key = NULL; \
    memset(&(it.value), 0, sizeof(it.value)); \
} while (0)

#define bd_xjson_htab_foreach(htab, it) \
    for(it = htab_iterate(htab, it); \
        it.key; \
        it = htab_iterate(htab, it))

#endif