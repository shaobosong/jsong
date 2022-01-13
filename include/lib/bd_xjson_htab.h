#ifndef bd_xjson_htab_h
#define bd_xjson_htab_h

#include <stddef.h>
#include <stdint.h>
#include "lib/bd_xjson_base.h"
#include "lib/bd_xjson_iter.h"

#define DEFAULT_CAPACITY 16

typedef struct bd_xjson_entry bd_xjson_entry;
typedef struct bd_xjson_htab bd_xjson_htab;

struct bd_xjson_entry
{
    char* key;
    bd_xjson value;
    uint64_t prev;
    uint64_t next;
};
struct bd_xjson_htab
{
    bd_xjson_entry* entries;
    uint64_t capacity;
    uint64_t size;
    uint64_t first;
    uint64_t last;
};
int htab_create(bd_xjson_htab** htab, uint64_t capacity);
int htab_copy(bd_xjson_htab* dest, bd_xjson_htab* src);
int htab_free(bd_xjson_htab* htab);
int htab_insert_direct(bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_insert(bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_erase(bd_xjson_htab* htab, const char* key);
int htab_find(bd_xjson_htab* htab, const char* key, bd_xjson* val);
int htab_update(bd_xjson_htab* htab, const char* key, bd_xjson* val);

/* define struct of iterator by type and name of data */
typedef bd_xjson_iter(bd_xjson_entry, entry) bd_xjson_htab_iter;
bd_xjson_htab_iter htab_begin(bd_xjson_htab* htab);
bd_xjson_htab_iter htab_end(bd_xjson_htab* htab);
bd_xjson_htab_iter htab_iterate(bd_xjson_htab* htab, bd_xjson_htab_iter iter);
#define bd_xjson_htab_foreach(__htab, __iter, __end) \
    for(bd_xjson_htab_iter __iter = htab_begin(__htab), \
        __end = htab_end(__htab); \
        __iter.index != __end.index; \
        __iter = htab_iterate(__htab, __iter))

#endif