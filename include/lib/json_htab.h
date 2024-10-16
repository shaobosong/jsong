#ifndef JSON_HTAB_H
#define JSON_HTAB_H

#include <stddef.h>
#include <stdint.h>
#include "lib/json.h"

#define DEFAULT_CAPACITY 16

typedef struct JSONEntry JSONEntry;
typedef struct JSONHashTable JSONHashTable;

struct JSONEntry {
    char *key;
    JSON value;
    uint64_t prev, next;
};

struct JSONHashTable {
    JSONEntry *entries;
    uint64_t capacity, size;
    uint64_t first, last;
};

JSONHashTable *htab_create(uint64_t capacity);
JSONHashTable *htab_create_copy(const JSONHashTable *src);
void htab_free(JSONHashTable *htab);
int htab_insert(JSONHashTable *htab, const char *key, const JSON *val);
int htab_insert_ref(JSONHashTable *htab, const char *key, const JSON *val);
int htab_erase(JSONHashTable *htab, const char *key);
int htab_find(const JSONHashTable *htab, const char *key, JSON *val);
int htab_find_ref(const JSONHashTable *htab, const char *key, JSON *val);
int htab_update(JSONHashTable *htab, const char *key, const JSON *val);
int htab_set(JSONHashTable *htab, const char *key, const JSON *val);
int htab_set_ref(JSONHashTable *htab, const char *key, const JSON *val);

/* define struct of iterator by type and name of data */
typedef struct JSONHashTableIter {
    void *index;
    char *key;
    JSON value;
    void *__entries;
} JSONHashTableIter;

JSONHashTableIter htab_begin(const JSONHashTable *htab);
JSONHashTableIter htab_end(const JSONHashTable *htab);
JSONHashTableIter htab_iterate(JSONHashTableIter iter);

#define json_htab_foreach(__iter, __end) \
    for (; \
        __iter.index != __end.index; \
        __iter = htab_iterate(__iter))

int htab_iter_get(JSONHashTableIter iter, JSON *val);

#endif