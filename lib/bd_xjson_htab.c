#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "lib/bd_xjson_htab.h"
#include "lib/utils.h"


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

static void entry_clear(bd_xjson_entry* e)
{
    assert(e);
    xfree(e->key);
    bd_xjson_free_data(&e->value);
    memset(e, 0, sizeof(*e));
}

static void entry_copy(bd_xjson_entry* d, const bd_xjson_entry* s)
{
    /* copy key */
    d->key = xmallocz(strlen(s->key) + 1);
    strcat(d->key, s->key);
    /* copy value */
    bd_xjson_copy(&(d->value), &(s->value));
    /* copy index */
    d->prev = s->prev;
    d->next = s->next;
}

static void entry_placed_in(bd_xjson_htab* h, uint64_t p)
{
    uint64_t i;

    if(h->first == h->capacity) {
        h->entries[p].prev = h->capacity;
        h->entries[p].next = h->capacity;
        h->first = p;
        h->last = p;
        return ;
    }
    if(p < h->first) {
        /* new p of begin in hash table */
        h->entries[h->first].prev = p;
        h->entries[p].prev = h->capacity;
        h->entries[p].next = h->first;
        h->first = p;
        return ;
    }
    if(h->last < p) {
        /* new p of end in hash table */
        h->entries[h->last].next = p;
        h->entries[p].prev = h->last;
        h->entries[p].next = h->capacity;
        h->last = p;
        return ;
    }
    /* h->begin < p < h->end */
    i = h->first;
    for(; i < p; i = h->entries[i].next);
    bd_xjson_entry* prev = &h->entries[h->entries[i].prev];
    bd_xjson_entry* next = &h->entries[i];
    h->entries[p].prev = next->prev;
    h->entries[p].next = prev->next;
    prev->next = p;
    next->prev = p;
    return ;
}

static void entry_removed_in(bd_xjson_htab* h, uint64_t p)
{
    /* removed the last in entries */
    if(h->entries[p].prev == h->entries[p].next) {
        h->first = h->capacity;
        h->last = h->capacity;
        return ;
    }
    /* removed in start */
    if(p == h->first) {
        h->first = h->entries[p].next;
        h->entries[h->first].prev = h->capacity;
        return ;
    }
    /* removed in end */
    if(p == h->last) {
        h->last = h->entries[p].prev;
        h->entries[h->last].next = h->capacity;
        return ;
    }
    /* removed in middle */
    h->entries[h->entries[p].prev].next = h->entries[p].next;
    h->entries[h->entries[p].next].prev = h->entries[p].prev;
    return ;
}

bd_xjson_htab* htab_create(uint64_t c)
{
    bd_xjson_htab* h;
    h = xmallocz(sizeof *h);
    h->entries = xmallocz((c + 1)*sizeof(bd_xjson_entry));
    h->size = 0;
    h->capacity = c;
    h->first = c;
    h->last = c;

    return h;
}

bd_xjson_htab* htab_create_copy(const bd_xjson_htab* s)
{
    bd_xjson_htab* d;
    uint64_t i;
    bd_xjson_htab_iter iter, end;

    assert(s->capacity != 0);
    d = htab_create(s->capacity);

    iter = htab_begin(s);
    end = htab_end(s);
    bd_xjson_htab_foreach(iter, end)
    {
        i = (iter.index - iter.__entries) /
            sizeof(bd_xjson_entry);
        entry_copy(&d->entries[i], &s->entries[i]);
    }

    d->capacity = s->capacity;
    d->size = s->size;
    d->first = s->first;
    d->last = s->last;

    return d;
}

void htab_free(bd_xjson_htab* h)
{
    bd_xjson_entry *curr, *end, *next;

    assert(h);
    /* entries clear */
    curr = &h->entries[h->first];
    end = &h->entries[h->capacity];
    next = NULL;
    for(; curr != end; curr = next)
    {
        next = &h->entries[curr->next];
        entry_clear(curr);
    }

    /* entries free */
    xfree(h->entries);

    /* hash table free */
    xfree(h);
}

static void htab_grow(bd_xjson_htab* h, uint64_t c)
{
    uint64_t i;
    bd_xjson_htab old = *h;
    bd_xjson_htab_iter iter, end;

    /* if capacity is up to max valid value, stop growing */
    if(c < h->capacity) {
        c = h->capacity;
        return ;
    }

    /* set new htab properities */
    h->entries = xmallocz((c + 1)*sizeof(bd_xjson_entry));
    h->capacity = c;
    h->first = c;
    h->last = c;

    /* shallow copy to improve performance */
    iter = htab_begin(&old);
    end = htab_end(&old);
    bd_xjson_htab_foreach(iter, end)
    {
        i = (uint64_t)(hash_key(iter.key) & (c - 1));
        /* unsafe */
        while(h->entries[i].key)
        {
            i = (i + 1) & (c - 1);
        }
        h->entries[i] = *(bd_xjson_entry*)iter.index;
        entry_placed_in(h, i);
    }

    /* free old entries */
    xfree(old.entries);
}

static uint64_t htab_find_id(const bd_xjson_htab* h, const char* k)
{
    uint64_t i = hash_key(k) & (h->capacity - 1);
    uint64_t n = 0;
    for(;;)
    {
        if(n >= h->capacity) { /* capacity is equal to 1 */
            return h->capacity;
        }
        if(NULL == h->entries[i].key ||
            0 == strcmp(h->entries[i].key, k)) {
            break;
        }
        i = (i + 1) & (h->capacity - 1);
        n++;
    }
    return i;
}

int htab_insert_ref(bd_xjson_htab* h, const char* k, const bd_xjson* v)
{
    /* if size of hash table will exceed half of capacity, grow it */
    if(h->size > (h->capacity >> 1)) {
        htab_grow(h, h->capacity << 1);
    }

    uint64_t i = htab_find_id(h, k);
    if(h->entries[i].key) {
        THROW_WARNING("hash table try to insert <value> by existed <key>");
        return -1;
    }
    /* insert a key */
    h->entries[i].key = xmallocz(strlen(k) + 1);
    strcat(h->entries[i].key, k);
    /* insert a value */
    h->entries[i].value = *v;
    /* plus 1 in size */
    h->size += 1;
    /* alter index of begin and end iterator */
    entry_placed_in(h, i);

    return 0;
}

int htab_insert(bd_xjson_htab* h, const char* k, const bd_xjson* v)
{
    uint64_t i;

    if(NULL == v) {
        THROW_WARNING("VAL is not initialized");
        return -1;
    }
    /* if size of hash table will exceed half of capacity, grow it */
    if(h->size > (h->capacity >> 1)) {
        htab_grow(h, h->capacity << 1);
    }

    i = htab_find_id(h, k);
    if(h->entries[i].key) {
        THROW_WARNING("hash table try to insert <value> by existed <key>");
        return -1;
    }
    /* insert a key */
    h->entries[i].key = xmallocz(strlen(k) + 1);
    strcat(h->entries[i].key, k);
    /* insert a value */
    bd_xjson_copy(&(h->entries[i].value), v);
    /* plus 1 in size */
    h->size += 1;
    /* alter index of begin and end iterator */
    entry_placed_in(h, i);

    return 0;
}

static void entries_reorder(bd_xjson_htab* htab, uint64_t start, uint64_t middle, uint64_t end)
{
    uint64_t e2m, e2m_id, k;

    /* the last element had been moved 'forward' */
    if(middle == end)
        return ;

    for(e2m = end; e2m != middle; e2m = (e2m - 1) & (htab->capacity - 1))
    {
        e2m_id = hash_key(htab->entries[e2m].key) & (htab->capacity - 1);
        for(k = e2m_id; k != e2m;k = (k + 1) & (htab->capacity - 1))
        {
            if(k == middle) {
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
    uint64_t i, s, e;

    /* remove a element */
    i = htab_find_id(htab, key);
    if(i == htab->capacity || NULL == htab->entries[i].key) {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }
    entry_removed_in(htab, i);
    entry_clear(&htab->entries[i]);
    htab->size -= 1;

    /* reorder */
    s = hash_key(key) & (htab->capacity - 1);
    e = (i + 1) & (htab->capacity - 1);
    for(;;)
    {
        if(NULL == htab->entries[e].key) {
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
    uint64_t i = htab_find_id(htab, key);
    if(i == htab->capacity || NULL == htab->entries[i].key) {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }

    if(val->type != htab->entries[i].value.type) {
        THROW_WARNING("type of VAL can't match type of found element");
        return -1;
    }
    /* free exist data */
    if(val->data) {
        bd_xjson_free_data(val);
    }
    bd_xjson_copy(val, &(htab->entries[i].value));

    return 0;
}

int htab_find_ref(const bd_xjson_htab* htab, const char* key, bd_xjson* val)
{
    uint64_t i = htab_find_id(htab, key);
    if(i == htab->capacity || NULL == htab->entries[i].key) {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }

    if(val->type != htab->entries[i].value.type) {
        THROW_WARNING("type of VAL can't match type of found element");
        return -1;
    }
    /* free exist data */
    if(val->data) {
        bd_xjson_free_data(val);
    }
    *val = htab->entries[i].value;

    return 0;
}

int htab_update(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
{
    uint64_t i = htab_find_id(htab, key);
    if(i == htab->capacity || NULL == htab->entries[i].key) {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }

    /* free old entry data */
    bd_xjson_free_data(&(htab->entries[i].value));
    /* update type and value */
    bd_xjson_copy(&(htab->entries[i].value), val);

    return 0;
}

int htab_update_ref(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
{
    uint64_t i = htab_find_id(htab, key);
    if(i == htab->capacity || NULL == htab->entries[i].key) {
        THROW_WARNING("hash table try to find index by non-existent key");
        return -1;
    }

    /* free old entry data */
    bd_xjson_free_data(&(htab->entries[i].value));
    /* update type and value */
    htab->entries[i].value = *val;

    return 0;
}

int htab_set(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
{
    uint64_t i = htab_find_id(htab, key);

    /* free old entry data if exist */
    if(htab->entries[i].key) {
        if(htab_update(htab, key, val)) {
            THROW_WARNING("HTAB set using update method error");
            return -1;
        }
    }
    else {
        if(htab_insert(htab, key, val)) {
            THROW_WARNING("HTAB set using insert method error");
            return -1;
        }
    }

    return 0;
}

int htab_set_ref(bd_xjson_htab* htab, const char* key, const bd_xjson* val)
{
    uint64_t i = htab_find_id(htab, key);

    /* free old entry data if exist */
    if(htab->entries[i].key) {
        if(htab_update_ref(htab, key, val)) {
            THROW_WARNING("HTAB set using update method error");
            return -1;
        }
    }
    else {
        if(htab_insert_ref(htab, key, val)) {
            THROW_WARNING("HTAB set using insert method error");
            return -1;
        }
    }

    return 0;
}

bd_xjson_htab_iter htab_begin(const bd_xjson_htab* h)
{
    bd_xjson_htab_iter iter = {
        .index = &h->entries[h->first],
        .key = h->entries[h->first].key,
        .value = h->entries[h->first].value,
        .__entries = h->entries
    };
    return iter;
}

bd_xjson_htab_iter htab_end(const bd_xjson_htab* h)
{
    bd_xjson_htab_iter iter = {
        .index = &h->entries[h->capacity]
    };
    return iter;
}

bd_xjson_htab_iter htab_iterate(bd_xjson_htab_iter iter)
{
    bd_xjson_entry* index = iter.index;
    bd_xjson_entry* entries = iter.__entries;

    iter.index = &entries[index->next];
    iter.key = entries[index->next].key;
    iter.value = entries[index->next].value;

    return iter;
}

int htab_iter_get(bd_xjson_htab_iter iter, bd_xjson* val)
{
    if(val->type != iter.value.type) {
        THROW_WARNING("unmatched JSON type to get value in iterator");
        return -1;
    }
    /* free old val data if exist */
    if(val->data) {
        bd_xjson_free_data(val);
    }
    /* copy from iter data */
    bd_xjson_copy(val, &(iter.value));
    return 0;
}