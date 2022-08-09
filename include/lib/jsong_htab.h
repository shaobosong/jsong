#ifndef JSONG_HTAB_H
#define JSONG_HTAB_H

#include <stddef.h>
#include <stdint.h>
#include "lib/jsong.h"

#define DEFAULT_CAPACITY 16

typedef struct jsong_entry jsong_entry;
typedef struct jsong_htab jsong_htab;

struct jsong_entry
{
    char* key;
    jsong value;
    uint64_t prev, next;
};

struct jsong_htab
{
    jsong_entry* entries;
    uint64_t capacity, size;
    uint64_t first, last;
};

jsong_htab* htab_create(uint64_t capacity);
jsong_htab* htab_create_copy(const jsong_htab* src);
void htab_free(jsong_htab* htab);
int htab_insert(jsong_htab* htab, const char* key, const jsong* val);
int htab_insert_ref(jsong_htab* htab, const char* key, const jsong* val);
int htab_erase(jsong_htab* htab, const char* key);
int htab_find(const jsong_htab* htab, const char* key, jsong* val);
int htab_find_ref(const jsong_htab* htab, const char* key, jsong* val);
int htab_update(jsong_htab* htab, const char* key, const jsong* val);
int htab_set(jsong_htab* htab, const char* key, const jsong* val);
int htab_set_ref(jsong_htab* htab, const char* key, const jsong* val);

/* define struct of iterator by type and name of data */
typedef struct jsong_htab_iter
{
    void* index;
    char* key;
    jsong value;
    void* __entries;
} jsong_htab_iter;
jsong_htab_iter htab_begin(const jsong_htab* htab);
jsong_htab_iter htab_end(const jsong_htab* htab);
jsong_htab_iter htab_iterate(jsong_htab_iter iter);
#define jsong_htab_foreach(__iter, __end) \
    for(; \
        __iter.index != __end.index; \
        __iter = htab_iterate(__iter))
int htab_iter_get(jsong_htab_iter iter, jsong* val);

#endif