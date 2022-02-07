#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "lib/bd_xjson_htab.h"
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

static int entry_clear(bd_xjson_entry* entry)
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
    memset(entry, 0, sizeof(*entry));
    return 0;
}

static int entry_copy(bd_xjson_entry* dest, const bd_xjson_entry* src)
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
    dest->prev = src->prev;
    dest->next = src->next;
    return 0;
}

static void entry_placed_in(bd_xjson_htab* htab, uint64_t index)
{
    if(htab->first == htab->capacity)
    {
        htab->entries[index].prev = htab->capacity;
        htab->entries[index].next = htab->capacity;
        htab->first = index;
        htab->last = index;
        return ;
    }
    if(index < htab->first)
    {
        /* new index of begin in hash table */
        htab->entries[htab->first].prev = index;
        htab->entries[index].prev = htab->capacity;
        htab->entries[index].next = htab->first;
        htab->first = index;
        return ;
    }
    if(htab->last < index)
    {
        /* new index of end in hash table */
        htab->entries[htab->last].next = index;
        htab->entries[index].prev = htab->last;
        htab->entries[index].next = htab->capacity;
        htab->last = index;
        return ;
    }
    /* htab->begin < index < htab->end */
    int iter = htab->first;
    for(; iter < index; iter = htab->entries[iter].next);
    bd_xjson_entry* prev = &htab->entries[htab->entries[iter].prev];
    bd_xjson_entry* next = &htab->entries[iter];
    htab->entries[index].prev = next->prev;
    htab->entries[index].next = prev->next;
    prev->next = index;
    next->prev = index;
    return ;
}

static void entry_removed_in(bd_xjson_htab* htab, uint64_t index)
{
    /* removed the last in entries */
    if(htab->entries[index].prev == htab->entries[index].next)
    {
        htab->first = htab->capacity;
        htab->last = htab->capacity;
        return ;
    }
    /* removed in start */
    if(index == htab->first)
    {
        htab->first = htab->entries[index].next;
        htab->entries[htab->first].prev = htab->capacity;
        return ;
    }
    /* removed in end */
    if(index == htab->last)
    {
        htab->last = htab->entries[index].prev;
        htab->entries[htab->last].next = htab->capacity;
        return ;
    }
    /* removed in middle */
    htab->entries[htab->entries[index].prev].next = htab->entries[index].next;
    htab->entries[htab->entries[index].next].prev = htab->entries[index].prev;
    return ;
}

int htab_create(bd_xjson_htab** htab, uint64_t capacity)
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
    h->entries = xzmalloc((capacity + 1)*sizeof(bd_xjson_entry));
    h->size = 0;
    h->first = capacity;
    h->last = capacity;

    *htab = h;
    return 0;
}

int htab_free(bd_xjson_htab* htab)
{
    if(NULL == htab)
    {
        THROW_WARNING("HTAB is not initialized");
        return -1;
    }
    /* entries clear */
    bd_xjson_htab_iter iter = htab_begin(htab);
    bd_xjson_htab_iter end = htab_end(htab);
    bd_xjson_htab_foreach(iter, end)
    {
        if(entry_clear(&htab->entries[iter.index]))
        {
            THROW_WARNING("one of ENTRIES free failed");
            return -1;
        }
    }
    /* entries free */
    xfree(htab->entries);
    /* hash table free */
    xfree(htab);
    return 0;
}

static int htab_grow(bd_xjson_htab* htab, uint64_t new_capacity)
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
    bd_xjson_htab old_htab = *htab;
    if(new_capacity < old_htab.capacity)
    {
        THROW_WARNING("new CAPACITY of HTAB is overflow");
        return -1;
    }
    /* set new htab properities */
    htab->entries = xzmalloc((new_capacity + 1)*sizeof(bd_xjson_entry));
    htab->capacity = new_capacity;
    htab->first = new_capacity;
    htab->last = new_capacity;
    /* apply shallow copy but not deep copy to improve performance */
    bd_xjson_htab_iter iter = htab_begin(&old_htab);
    bd_xjson_htab_iter end = htab_end(&old_htab);
    bd_xjson_htab_foreach(iter, end)
    {
        uint64_t index = (uint64_t)(hash_key(iter.data.key) & (new_capacity - 1));
        /* unsafe */
        while(htab->entries[index].key)
        {
            index = (index + 1) & (new_capacity - 1);
        }
        htab->entries[index] = iter.data;
        entry_placed_in(htab, index);
    }
    /* free old entries */
    xfree(old_htab.entries);

    return 0;
}

int htab_copy(bd_xjson_htab* dest, const bd_xjson_htab* src)
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
    /* copy entries from src */
    bd_xjson_htab_iter iter = htab_begin(src);
    bd_xjson_htab_iter end = htab_end(src);
    bd_xjson_htab_foreach(iter, end)
    {
        if(entry_copy(&dest->entries[iter.index], &src->entries[iter.index]))
        {
            THROW_WARNING("copy data of SRC to data field of DEST failed");
            return -1;
        }
    }
    dest->capacity = src->capacity;
    dest->size = src->size;
    dest->first = src->first;
    dest->last = src->last;

    return 0;
}

static uint64_t htab_find_id(const bd_xjson_htab* htab, const char* key)
{
    uint64_t i = hash_key(key) & (htab->capacity - 1);
    uint64_t c = 0;
    for(;;)
    {
        if(c >= htab->capacity) /* capacity is equal to 1 */
        {
            return htab->capacity;
        }
        if(NULL == htab->entries[i].key ||
            0 == strcmp(htab->entries[i].key, key))
        {
            break;
        }
        i = (i + 1) & (htab->capacity - 1);
        c++;
    }
    return i;
}

int htab_insert_direct(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
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
        if(htab_grow(htab, htab->capacity << 1))
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
    htab->entries[i].value = *val;
    /* plus 1 in size */
    htab->size += 1;
    /* alter index of begin and end iterator */
    entry_placed_in(htab, i);

    return 0;
}

int htab_insert(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
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
        if(htab_grow(htab, htab->capacity << 1))
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
    /* alter index of begin and end iterator */
    entry_placed_in(htab, i);

    return 0;
}

static void entries_reorder(bd_xjson_htab* htab, uint64_t start, uint64_t middle, uint64_t end)
{
    /* the last element had been moved 'forward' */
    if(middle == end)
        return ;

    for(uint64_t e2m = end; e2m != middle; e2m = (e2m - 1) & (htab->capacity - 1))
    {
        uint64_t e2m_id = hash_key(htab->entries[e2m].key) & (htab->capacity - 1);
        for(uint64_t k = e2m_id; k != e2m;k = (k + 1) & (htab->capacity - 1))
        {
            if(k == middle)
            {
                entry_removed_in(htab, e2m);
                htab->entries[middle] = htab->entries[e2m];
                entry_placed_in(htab, middle);

                memset(&htab->entries[e2m], 0, sizeof(htab->entries[e2m]));
                entries_reorder(htab, start, e2m, end);
                return ;
            }
        }
    }

    /* there are no more elements to move 'forward' */
    return ;
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

    /* remove a element */
    uint64_t i = htab_find_id(htab, key);
    if(i == htab->capacity || NULL == htab->entries[i].key)
    {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }
    entry_removed_in(htab, i);
    if(entry_clear(&htab->entries[i]))
    {
        THROW_WARNING("one of ENTRIES free failed");
        return -1;
    }
    htab->size -= 1;

    /* reorder */
    uint64_t s = hash_key(key) & (htab->capacity - 1);
    uint64_t e = (i + 1) & (htab->capacity - 1);
    for(;;)
    {
        if(NULL == htab->entries[e].key)
        {
            e = (e - 1) & (htab->capacity - 1);
            break;
        }
        e = (e + 1) & (htab->capacity - 1);
    }
    entries_reorder(htab, s, i, e);

    return 0;
}

/* check if type of val matches type of found element */
int htab_find(const bd_xjson_htab* htab, const char* key, bd_xjson* val)
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
    if(i == htab->capacity || NULL == htab->entries[i].key)
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

int htab_update(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
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
    if(i == htab->capacity || NULL == htab->entries[i].key)
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

int htab_set(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
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

    /* free old entry data if exist */
    if(htab->entries[i].key)
    {
        if(htab_update(htab, key, val))
        {
            THROW_WARNING("HTAB set using update method error");
            return -1;
        }
    }
    else
    {
        if(htab_insert(htab, key, val))
        {
            THROW_WARNING("HTAB set using insert method error");
            return -1;
        }
    }

    return 0;
}

bd_xjson_htab_iter htab_begin(const bd_xjson_htab* htab)
{
    bd_xjson_htab_iter iter =
    {
        .index = htab->first,
        .data = htab->entries[htab->first],
        .entries = htab->entries
    };
    return iter;
}

bd_xjson_htab_iter htab_end(const bd_xjson_htab* htab)
{
    bd_xjson_htab_iter iter =
    {
        .index = htab->capacity
    };
    return iter;
}

bd_xjson_htab_iter
htab_iterate(bd_xjson_htab_iter iter)
{
    iter.index = iter.data.next;
    iter.data = iter.entries[iter.index];
    return iter;
}

int htab_iter_get(bd_xjson_htab_iter iter, bd_xjson* val)
{
    if(val->type != iter.data.value.type)
    {
        THROW_WARNING("unmatched JSON type to get value in iterator");
        return -1;
    }
    /* free old val data if exist */
    if(val->data)
    {
        if(bd_xjson_free(val))
        {
            THROW_WARNING("value free failed");
            return -1;
        }
    }
    /* copy from iter data */
    if(bd_xjson_copy(val, &(iter.data.value)))
    {
        THROW_WARNING("copy failed");
        return -1;
    }
    return 0;
}