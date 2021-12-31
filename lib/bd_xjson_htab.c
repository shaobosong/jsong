#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "lib/bd_xjson_htab.h"
#include "lib/bd_xjson_list.h"
#include "lib/error.h"
#include "lib/alloc.h"

#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

/* Return 64-bit FNV-1a hash for key (NULL-terminated). See description: */
/* https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function */
static uint64_t hash_key(const char* key)
{
    uint64_t hash = FNV_OFFSET_BASIS;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)*(unsigned char*)p;
        hash *= FNV_PRIME;
    }
    return hash;
}

static int entry_free(bd_xjson_entry* entry)
{
    xfree(entry->key);
    if(bd_xjson_free(&entry->value))
    {
        THROW_WARNING("bd_xjson free failed");
        return -1;
    }
    return 0;
}

static int entry_copy(bd_xjson_entry* dest, bd_xjson_entry* src)
{
    if(NULL == dest)
    {
        THROW_WARNING("uninitialized entries try to copy");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("uninitialized entries try to be the copied");
        return -1;
    }
    /* copy key */
    int s = strlen(src->key) + 1;
    dest->key = xzmalloc(s);
    strcat(dest->key, src->key);
    /* copy value */
    if(bd_xjson_copy(&(dest->value), &(src->value)))
    {
        THROW_WARNING("bd_xjson copy failed");
        return -1;
    }
    return 0;
}

static int entries_clear(bd_xjson_entry** entryies)
{
    return 0;
}

/* unsafe if length of src is greater than length of dest */
static int entries_copy(bd_xjson_entry* dest, bd_xjson_entry* src, size_t capacity)
{
    if(NULL == dest)
    {
        THROW_WARNING("uninitialized entries try to copy");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("uninitialized entries try to be the copied");
        return -1;
    }
    for(int i = 0; i < capacity; i++)
    {
        if(NULL == src[i].key)
        {
            continue;
        }
        if(entry_copy(&dest[i], &src[i]))
        {
            THROW_WARNING("entry copy failed");
            return -1;
        }
    }
    return 0;
}

int htab_init(bd_xjson_htab** htab, size_t capacity)
{
    if(*htab)
    {
        THROW_WARNING("duplicated initialization already done");
        return -1;
    }
    if(0 == capacity)
    {
        THROW_WARNING("try to initialize zero as capacity");
        return -1;
    }
    bd_xjson_htab* h;
    h = xzmalloc(sizeof *h);
    h->capacity = capacity;
    h->entries = xzmalloc(capacity*sizeof(bd_xjson_entry));
    h->size = 0;

    *htab = h;
    return 0;
}

int htab_grow(bd_xjson_htab* htab)
{
    if(NULL == htab)
    {
        THROW_WARNING("unitialized htash table try to grow");
        return -1;
    }
    if(0 == htab->capacity)
    {
        THROW_WARNING("illegal capacity of hash table try to grow");
        return -1;
    }
    /* calculate new capacity */
    size_t old_capacity = htab->capacity;
    size_t new_capacity = htab->capacity << 1;
    if(new_capacity < old_capacity)
    {
        THROW_WARNING("new capacity of hash table is overflow");
        return -1;
    }
    bd_xjson_entry* old_entries = htab->entries;
    bd_xjson_entry* new_entries = xzmalloc(new_capacity*sizeof(bd_xjson_entry));
    if(entries_copy(new_entries, old_entries, old_capacity))
    {
        THROW_WARNING("entries copy failed");
        return -1;
    }
    /* free old entries */
    xfree(old_entries);
    /* get new htab */
    htab->capacity = new_capacity;
    htab->entries = new_entries;

    return 0;
}

int htab_copy(bd_xjson_htab* dest, bd_xjson_htab* src)
{
    if(NULL == dest)
    {
        THROW_WARNING("unitialized htash table try to accept the copied");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("uninitialized hash table try to be the copied");
        return -1;
    }
    if(dest->capacity != src->capacity)
    {
        THROW_WARNING("the capaticy of the two hash tables is inconsistent");
        return -1;
    }
    if(entries_copy(dest->entries, src->entries, src->capacity))
    {
        THROW_WARNING("entries copy failed");
        return -1;
    }
    dest->capacity = src->capacity;
    dest->size = src->size;

    return 0;
}

int htab_insert(bd_xjson_htab* htab, const char* key, bd_xjson* val)
{
    if(NULL == htab)
    {
        THROW_WARNING("unitialized htash table try to insert");
        return -1;
    }
    if(NULL == key)
    {
        THROW_WARNING("hash table try to insert unitialized key");
        return -1;
    }
    if(NULL == val)
    {
        THROW_WARNING("hash table try to insert uninitialized value");
        return -1;
    }
    /* if size of hash table will exceed half of capacity, grow it */
    if(htab->size > (htab->capacity >> 1))
    {
        htab_grow(htab);
    }
    uint64_t hash = hash_key(key);
    uint64_t id = (uint64_t)(hash & (htab->capacity - 1));

    for(uint64_t i = id ;;)
    {
        if(NULL == htab->entries[i].key)
        {
            /* insert a key */
            int s = strlen(key) + 1;
            htab->entries[i].key = xzmalloc(s);
            strcat(htab->entries[i].key, key);
            /* insert a value */
            if(bd_xjson_copy(&(htab->entries[i].value), val))
            {
                THROW_WARNING("bd_xjson copy failed");
                return -1;
            }
            /* plus 1 in size */
            htab->size += 1;
            break;
        }
        if(0 == strcmp(htab->entries[i].key, key))
        {
            THROW_WARNING("hash table try to insert exist <key>-<value>");
            return -1;
        }
        i = (i + 1) & htab->capacity;
    }
    return 0;
}

int htab_delete(bd_xjson_htab* htab, const char* key)
{
    if(NULL == htab)
    {
        THROW_WARNING("unitialized htash table try to delete");
        return -1;
    }
    if(NULL == key)
    {
        THROW_WARNING("hash table try to delete unitialized key");
        return -1;
    }

    uint64_t hash = hash_key(key);
    uint64_t id = (uint64_t)(hash & (htab->capacity - 1));
    uint64_t ct = 0;
    for(uint64_t i = id ;;)
    {
        if(NULL == htab->entries[i].key || htab->size > ct)
        {
            THROW_WARNING("hash table try to delete value by unitialized key");
            return -1;
        }
        if(0 == strcmp(htab->entries[i].key, key))
        {
            entry_free(&htab->entries[i]);
            /* must not ignore next statement */
            htab->entries[i] = *(bd_xjson_entry*)0;
            break;
        }
        i = (i + 1) & htab->capacity;
        ct++;
    }
    return 0;
}

int htab_search(bd_xjson_htab* htab, const char* key, bd_xjson* val)
{
    return 0;
}