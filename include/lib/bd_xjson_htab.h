#ifndef BD_XJSON_HTAB_H
#define BD_XJSON_HTAB_H

#include <stddef.h>
#include <stdint.h>
#include "lib/bd_xjson.h"

#define DEFAULT_CAPACITY 16

typedef struct bd_xjson_entry bd_xjson_entry;
typedef struct bd_xjson_htab bd_xjson_htab;

struct bd_xjson_entry
{
    char* key;
    bd_xjson value;
    uint64_t prev, next;
};

struct bd_xjson_htab
{
    bd_xjson_entry* entries;
    uint64_t capacity, size;
    uint64_t first, last;
};

bd_xjson_htab* htab_create(uint64_t capacity);
bd_xjson_htab* htab_create_copy(const bd_xjson_htab* src);
void htab_free(bd_xjson_htab* htab);
int htab_insert(bd_xjson_htab* htab, const char* key, const bd_xjson* val);
int htab_insert_ref(bd_xjson_htab* htab, const char* key, const bd_xjson* val);
int htab_erase(bd_xjson_htab* htab, const char* key);
int htab_find(const bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_find_ref(const bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_update(bd_xjson_htab* htab, const char* key, const bd_xjson* val);
int htab_set(bd_xjson_htab* htab, const char* key, const bd_xjson* val);
int htab_set_ref(bd_xjson_htab* htab, const char* key, const bd_xjson* val);

/* define struct of iterator by type and name of data */
typedef struct bd_xjson_htab_iter
{
    void* index;
    char* key;
    bd_xjson value;
    void* __entries;
} bd_xjson_htab_iter;
bd_xjson_htab_iter htab_begin(const bd_xjson_htab* htab);
bd_xjson_htab_iter htab_end(const bd_xjson_htab* htab);
bd_xjson_htab_iter htab_iterate(bd_xjson_htab_iter iter);
#define bd_xjson_htab_foreach(__iter, __end) \
    for(; \
        __iter.index != __end.index; \
        __iter = htab_iterate(__iter))
int htab_iter_get(bd_xjson_htab_iter iter, bd_xjson* val);

#endif