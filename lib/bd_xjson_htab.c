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
    for (const char* p = key; *p; p++)
    {
        hash ^= (uint64_t)*(unsigned char*)p;
        hash *= FNV_PRIME;
    }
    return hash;
}

static int entry_free(bd_xjson_entry* entry)
{
    if(NULL == entry)
    {
        THROW_WARNING("ENTRY was initialized");
        return -1;
    }
    xfree(entry->key);
    if(bd_xjson_free(&entry->value))
    {
        THROW_WARNING("VALUE of ENTRY free failed");
        return -1;
    }
    return 0;
}

static int entry_copy(bd_xjson_entry* dest, bd_xjson_entry* src)
{
    if(NULL == dest)
    {
        THROW_WARNING("DEST is not initialized");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("SRC is not initialized");
        return -1;
    }
    /* copy key */
    int s = strlen(src->key) + 1;
    dest->key = xzmalloc(s);
    strcat(dest->key, src->key);
    /* copy value */
    if(bd_xjson_copy(&(dest->value), &(src->value)))
    {
        THROW_WARNING("copy value of SRC to value field of DEST failed");
        return -1;
    }
    return 0;
}

static int entries_clear(bd_xjson_entry** entries, size_t capacity)
{
    bd_xjson_entry* e = *entries;
    for(int i = 0; i< capacity; i++)
    {
        if(NULL == e[i].key)
        {
            continue;
        }
        if(entry_free(&e[i]))
        {
            THROW_WARNING("one of ENTRIES free failed");
            return -1;
        }
    }
    xfree(e);
    return 0;
}

/* unsafe if length of src is greater than length of dest */
static int entries_deep_copy(bd_xjson_entry* dest, bd_xjson_entry* src, size_t capacity)
{
    if(NULL == dest)
    {
        THROW_WARNING("DEST is not initialized");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("SRC is not initialized");
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
            THROW_WARNING("copy data of SRC to data field of DEST failed");
            return -1;
        }
    }
    return 0;
}

static int entries_grow_copy(bd_xjson_entry* dest, bd_xjson_entry* src, size_t ncap, size_t ocap)
{
    if(NULL == dest)
    {
        THROW_WARNING("DEST is not initialized");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("SRC is not initialized");
        return -1;
    }
    for(int i = 0; i < ocap; i++)
    {
        if(NULL == src[i].key)
        {
            continue;
        }
        uint64_t hash = hash_key(src[i].key);
        uint64_t j = (uint64_t)(hash & (ncap - 1));
        /* unsafe */
        while(dest[j].key)
        {
            j = (j + 1) & (ncap - 1);
        }
        dest[j] = src[i];
    }
    return 0;
}

int htab_create(bd_xjson_htab** htab, size_t capacity)
{
    if(*htab)
    {
        THROW_WARNING("HTAB was initialized");
        return -1;
    }
    if(0 == capacity)
    {
        THROW_WARNING("try to initialize CAPACITY of HTAB by zero");
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

int htab_free(bd_xjson_htab** htab)
{
    if(NULL == *htab)
    {
        THROW_WARNING("HTAB is not initialized");
        return -1;
    }
    bd_xjson_htab* h = *htab;
    entries_clear(&(h->entries), h->capacity);
    xfree(h);
    return 0;
}

static int htab_grow(bd_xjson_htab* htab)
{
    if(NULL == htab)
    {
        THROW_WARNING("HTAB is not initialized");
        return -1;
    }
    if(0 == htab->capacity)
    {
        THROW_WARNING("illegal CAPACITY of HTAB try to grow");
        return -1;
    }
    /* calculate new capacity */
    size_t old_capacity = htab->capacity;
    size_t new_capacity = htab->capacity << 1;
    if(new_capacity < old_capacity)
    {
        THROW_WARNING("new CAPACITY of HTAB is overflow");
        return -1;
    }
    bd_xjson_entry* old_entries = htab->entries;
    bd_xjson_entry* new_entries = xzmalloc(new_capacity*sizeof(bd_xjson_entry));
    /* apply shallow copy but not deep copy to improve performance */
    if(entries_grow_copy(new_entries, old_entries, new_capacity, old_capacity))
    {
        THROW_WARNING("shallow copy old entries to the new failed");
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
        THROW_WARNING("DEST is not initialized");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("SRC is not initialized");
        return -1;
    }
    if(dest->capacity != src->capacity)
    {
        THROW_WARNING("the CAPACITY of both DEST and SRC is inconsistent");
        return -1;
    }
    if(entries_deep_copy(dest->entries, src->entries, src->capacity))
    {
        THROW_WARNING("deep copy entries of SRC to entries field of DEST failed");
        return -1;
    }
    dest->capacity = src->capacity;
    dest->size = src->size;

    return 0;
}

static uint64_t htab_find_id(bd_xjson_htab* htab, const char* key)
{
    uint64_t hash = hash_key(key);
    uint64_t i = (uint64_t)(hash & (htab->capacity - 1));
    uint64_t ct = 0;
    for(;;)
    {
        if(ct >= htab->size) /* capacity is equal to 1 */
        {
            return htab->size;
        }
        if(NULL == htab->entries[i].key ||
            0 == strcmp(htab->entries[i].key, key))
        {
            break;
        }
        i = (i + 1) & (htab->capacity - 1);
        ct++;
    }
    return i;
}

int htab_insert(bd_xjson_htab* htab, const char* key, bd_xjson* val)
{
    if(NULL == htab)
    {
        THROW_WARNING("HTAB is not initialized");
        return -1;
    }
    if(NULL == key)
    {
        THROW_WARNING("KEY is not initialized");
        return -1;
    }
    if(NULL == val)
    {
        THROW_WARNING("VAL is not initialized");
        return -1;
    }
    /* if size of hash table will exceed half of capacity, grow it */
    if(htab->size > (htab->capacity >> 1))
    {
        if(htab_grow(htab))
        {
            THROW_WARNING("hash table grow failed");
            return -1;
        }
    }

    uint64_t i = htab_find_id(htab, key);
    if(htab->entries[i].key)
    {
        THROW_WARNING("hash table try to insert <value> by existed <key>");
        return -1;
    }
    /* insert a key */
    int s = strlen(key) + 1;
    htab->entries[i].key = xzmalloc(s);
    strcat(htab->entries[i].key, key);
    /* insert a value */
    if(bd_xjson_copy(&(htab->entries[i].value), val))
    {
        THROW_WARNING("copy VAL to entries field of HTAB failed");
        return -1;
    }
    /* plus 1 in size */
    htab->size += 1;

    return 0;
}

int htab_erase(bd_xjson_htab* htab, const char* key)
{
    if(NULL == htab)
    {
        THROW_WARNING("HTAB is not initialized");
        return -1;
    }
    if(NULL == key)
    {
        THROW_WARNING("KEY is not initialized");
        return -1;
    }

    uint64_t i = htab_find_id(htab, key);
    if(i == htab->size || NULL == htab->entries[i].key)
    {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }

    if(entry_free(&htab->entries[i]))
    {
        THROW_WARNING("entry free failed");
        return -1;
    }
    /* must not ignore next statement */
    memset(&htab->entries[i], 0, sizeof(bd_xjson_entry));
    htab->size -= 1;

    return 0;
}

/* check if type of val matches type of found element */
int htab_find(bd_xjson_htab* htab, const char* key, bd_xjson* val)
{
    if(NULL == htab)
    {
        THROW_WARNING("HTAB is not initialized");
        return -1;
    }
    if(NULL == key)
    {
        THROW_WARNING("KEY is not initialized");
        return -1;
    }
    if(NULL == val)
    {
        THROW_WARNING("VAL is not initialized");
        return -1;
    }

    uint64_t i = htab_find_id(htab, key);
    if(i == htab->size || NULL == htab->entries[i].key)
    {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }

    if(val->type != htab->entries[i].value.type)
    {
        THROW_WARNING("type of VAL can't match type of found element");
        return -1;
    }
    /* free exist data */
    if(val->data)
    {
        if(bd_xjson_free(val))
        {
            THROW_WARNING("VAL free failed");
            return -1;
        }
    }
    if(bd_xjson_copy(val, &(htab->entries[i].value)))
    {
        THROW_WARNING("copy failed");
        return -1;
    }

    return 0;
}

int htab_update(bd_xjson_htab* htab, const char* key, bd_xjson* val)
{
    if(NULL == htab)
    {
        THROW_WARNING("HTAB is not initialized");
        return -1;
    }
    if(NULL == key)
    {
        THROW_WARNING("KEY is not initialized");
        return -1;
    }
    if(NULL == val)
    {
        THROW_WARNING("VAL is not initialized");
        return -1;
    }

    uint64_t i = htab_find_id(htab, key);
    if(i == htab->size || NULL == htab->entries[i].key)
    {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }

    /* free old entry data */
    if(bd_xjson_free(&(htab->entries[i].value)))
    {
        THROW_WARNING("value of entry free failed");
        return -1;
    }
    /* update type and value */
    if(bd_xjson_copy(&(htab->entries[i].value), val))
    {
        THROW_WARNING("copy VAL to value field of entry failed");
        return -1;
    }

    return 0;
}