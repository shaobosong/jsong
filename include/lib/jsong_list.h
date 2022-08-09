#ifndef JSONG_LIST_H
#define JSONG_LIST_H

#include "lib/jsong.h"

typedef struct jsong_node jsong_node;
typedef struct jsong_list jsong_list;

struct jsong_node
{
    jsong value;
    jsong_node *next;
    jsong_node *prev;
};

struct jsong_list
{
    jsong_node* head, * tail, * nil;
    int size;
};
jsong_list* list_create();
jsong_list* list_create_copy(const jsong_list* src);
void list_free(jsong_list* list);
int list_insert_tail(jsong_list* list, const jsong* val);
int list_insert(jsong_list* list, int pos, const jsong* val);
int list_erase(jsong_list* list, int pos);
int list_find(const jsong_list* list, int pos, jsong* val);
int list_update(jsong_list* list, int pos, const jsong* val);
int list_set(jsong_list* list, int pos, const jsong* val);
void list_qsort(jsong_list* list, int (*compare_fn)(const void*, const void*));
typedef struct jsong_list_iter
{
    void* index;
    jsong value;
} jsong_list_iter;
jsong_list_iter list_begin(const jsong_list* list);
jsong_list_iter list_end(const jsong_list* list);
jsong_list_iter list_iterate(jsong_list_iter iter);
jsong_list_iter list_rbegin(const jsong_list* list);
jsong_list_iter list_rend(const jsong_list* list);
jsong_list_iter list_riterate(jsong_list_iter iter);
#define jsong_list_foreach(__iter, __end) \
    for(; \
        __iter.index != __end.index; \
        __iter = list_iterate(__iter))
#define jsong_list_reverse_foreach(__iter, __end) \
    for(; \
        __iter.index != __end.index; \
        __iter = list_riterate(__iter))
int list_iter_get(jsong_list_iter iter, jsong* val);

#endif