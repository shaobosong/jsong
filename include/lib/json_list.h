#ifndef JSON_LIST_H
#define JSON_LIST_H

#include "lib/json.h"

typedef struct JSONNode JSONNode;
typedef struct JSONLinkedList JSONLinkedList;

struct JSONNode {
    JSON value;
    JSONNode *next;
    JSONNode *prev;
};

struct JSONLinkedList {
    JSONNode *head, *tail, *nil;
    int size;
};

JSONLinkedList *list_create();
JSONLinkedList *list_create_copy(const JSONLinkedList *src);
void list_free(JSONLinkedList *list);
int list_insert_tail(JSONLinkedList *list, const JSON *val);
int list_insert(JSONLinkedList *list, int pos, const JSON *val);
int list_erase(JSONLinkedList *list, int pos);
int list_find(const JSONLinkedList *list, int pos, JSON *val);
int list_update(JSONLinkedList *list, int pos, const JSON *val);
int list_set(JSONLinkedList *list, int pos, const JSON *val);
void list_qsort(JSONLinkedList *list, int (*compare_fn)(const void *, const void *));

typedef struct JSONLinkedListIter {
    void *index;
    JSON value;
} JSONLinkedListIter;

typedef struct JSONLinkedListIter JSONLinkedListIter;
typedef struct JSONLinkedListIter jsong_list_iter;

JSONLinkedListIter list_begin(const JSONLinkedList *list);
JSONLinkedListIter list_end(const JSONLinkedList *list);
JSONLinkedListIter list_iterate(JSONLinkedListIter iter);
JSONLinkedListIter list_rbegin(const JSONLinkedList *list);
JSONLinkedListIter list_rend(const JSONLinkedList *list);
JSONLinkedListIter list_riterate(JSONLinkedListIter iter);

#define jsong_list_foreach(__iter, __end) \
    for (; \
        __iter.index != __end.index; \
        __iter = list_iterate(__iter))
#define jsong_list_reverse_foreach(__iter, __end) \
    for (; \
        __iter.index != __end.index; \
        __iter = list_riterate(__iter))

int list_iter_get(JSONLinkedListIter iter, JSON *val);

#endif