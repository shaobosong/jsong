#ifndef BD_XJSON_LIST_H
#define BD_XJSON_LIST_H

#include "lib/bd_xjson.h"

typedef struct bd_xjson_node bd_xjson_node;
typedef struct bd_xjson_list bd_xjson_list;

struct bd_xjson_node
{
    bd_xjson value;
    bd_xjson_node *next;
    bd_xjson_node *prev;
};

struct bd_xjson_list
{
    bd_xjson_node* head, * tail, * nil;
    int size;
};
bd_xjson_list* list_create();
bd_xjson_list* list_create_copy(const bd_xjson_list* src);
void list_free(bd_xjson_list* list);
int list_insert_tail(bd_xjson_list* list, const bd_xjson* val);
int list_insert(bd_xjson_list* list, int pos, const bd_xjson* val);
int list_erase(bd_xjson_list* list, int pos);
int list_find(const bd_xjson_list* list, int pos, bd_xjson* val);
int list_update(bd_xjson_list* list, int pos, const bd_xjson* val);
int list_set(bd_xjson_list* list, int pos, const bd_xjson* val);
void list_qsort(bd_xjson_list* list, int (*compare_fn)(const void*, const void*));
typedef struct bd_xjson_list_iter
{
    void* index;
    bd_xjson value;
} bd_xjson_list_iter;
bd_xjson_list_iter list_begin(const bd_xjson_list* list);
bd_xjson_list_iter list_end(const bd_xjson_list* list);
bd_xjson_list_iter list_iterate(bd_xjson_list_iter iter);
bd_xjson_list_iter list_rbegin(const bd_xjson_list* list);
bd_xjson_list_iter list_rend(const bd_xjson_list* list);
bd_xjson_list_iter list_riterate(bd_xjson_list_iter iter);
#define bd_xjson_list_foreach(__iter, __end) \
    for(; \
        __iter.index != __end.index; \
        __iter = list_iterate(__iter))
#define bd_xjson_list_reverse_foreach(__iter, __end) \
    for(; \
        __iter.index != __end.index; \
        __iter = list_riterate(__iter))
int list_iter_get(bd_xjson_list_iter iter, bd_xjson* val);

#endif