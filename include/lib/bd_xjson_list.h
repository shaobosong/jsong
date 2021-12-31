#ifndef bd_xjson_list_h
#define bd_xjson_list_h

#include "lib/bd_xjson_base.h"

typedef struct bd_xjson_node bd_xjson_node;
typedef struct bd_xjson_list bd_xjson_list;

struct bd_xjson_node
{
    bd_xjson data;
    bd_xjson_node *next;
    bd_xjson_node *prev;
};
int node_init(bd_xjson_node** node);
int node_copy(bd_xjson_node* dest, bd_xjson_node* src);
int node_free(bd_xjson_node* node);


struct bd_xjson_list
{
    bd_xjson_node* head, * tail;
    int size;
};
int list_init(bd_xjson_list** list);
int list_copy(bd_xjson_list* dest, bd_xjson_list* src);
int list_insert(bd_xjson_list* list, int pos, bd_xjson* val);
int list_remove(bd_xjson_list* list, int pos);
int list_clear(bd_xjson_list** list);
int list_find(bd_xjson_list* list, int pos, bd_xjson* val);
int list_update(bd_xjson_list* list, int pos, bd_xjson* val);

#endif