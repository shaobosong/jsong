#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "lib/bd_xjson_list.h"
#include "lib/error.h"
#include "lib/alloc.h"

int node_create(bd_xjson_node** node)
{
    if(*node)
    {
        THROW_WARNING("NODE was initialized");
        return -1;
    }
    bd_xjson_node* n;
    n = xzmalloc(sizeof *n);
    n->next = n->prev = NULL;
    n->data.data = NULL;
    n->data.type = 0;

    *node = n;
    return 0;
}
/* only copy value and type */
int node_copy(bd_xjson_node* dest, bd_xjson_node* src)
{
    /* don't responsible for initializing node */
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
    /* only copy data */
    if(bd_xjson_copy(&(dest->data), &(src->data)))
    {
        THROW_WARNING("copy data of SRC to data field of DEST failed");
        return -1;
    }

    return 0;
}

int node_free(bd_xjson_node* node)
{
    if(NULL == node)
    {
        THROW_WARNING("NODE is not initialized");
        return -1;
    }
    if(bd_xjson_free(&(node->data)))
    {
        THROW_WARNING("data of NODE free failed");
        return -1;
    }
    xfree(node);
    return 0;
}

int list_create(bd_xjson_list** list)
{
    if(*list)
    {
        THROW_WARNING("LIST was initialized");
        return -1;
    }
    bd_xjson_list* l;
    l = xzmalloc(sizeof *l);
    l->head = l->tail = NULL;
    l->size = 0;

    *list = l;
    return 0;
}

int list_copy(bd_xjson_list* dest, bd_xjson_list* src)
{
    /* don't responsible for initializing list */
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
    bd_xjson_node* list_node = NULL;
    bd_xjson_node* copy_node = src->head;
    bd_xjson_node* list_node_prev = NULL;
    for(; copy_node; list_node = list_node->next, copy_node = copy_node->next)
    {
        if(node_create(&(list_node)))
        {
            THROW_WARNING("node initializaition failed");
            return -1;
        }
        if(node_copy(list_node, copy_node))
        {
            THROW_WARNING("copy node of SRC to area of DEST failed");
            return -1;
        }
        /* prev of node*/
        list_node->prev = list_node_prev;
        if(list_node_prev)
        {
            /* next of node */
            list_node_prev->next = list_node;
        }
        else
        {
            /* head of list */
            dest->head = list_node;
        }

        /* tail and size of list */
        dest->tail = list_node;
        dest->size += 1;
        /* save current node */
        list_node_prev = list_node;
    }
    return 0;
}

int list_insert(bd_xjson_list* list, int pos, bd_xjson* val)
{
    if(NULL == list)
    {
        THROW_WARNING("LIST is not initialized");
        return -1;
    }
    if(NULL == val)
    {
        THROW_WARNING("VAL is not initialized");
        return -1;
    }
    if(pos > list->size || pos < -list->size-1)
    {
        THROW_WARNING("try to insert in illegal POS");
        return -1;
    }

    bd_xjson_node* node = NULL;
    if(node_create(&(node)))
    {
        THROW_WARNING("node initializaition failed");
        return -1;
    }
    if(bd_xjson_copy(&(node->data), val))
    {
        THROW_WARNING("copy VAL to data field of node failed");
        return -1;
    }

    /* insert into head if list have no any nodes */
    if( 0 == list->size)
    {
        node->next = NULL;
        node->prev = NULL;
        list->head = node;
        list->tail = node;
        list->size += 1;
        return 0;
    }
    /* insert into head behind position */
    if( 0 == pos || -list->size-1 == pos)
    {
        node->next = list->head;
        node->prev = NULL;
        list->head->prev = node;
        list->head = node; /* new head node */
        list->size += 1;
        return 0;
    }
    /* insert into tail behind position */
    if(-1 == pos || list->size == pos)
    {
        node->next = NULL;
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
        list->size += 1;
        return 0;
    }
    /* insert from head behind position */
    if(pos > 0)
    {
        bd_xjson_node* curr;
        curr = list->head->next;
        for(int i = 1; i < pos; i++)
        {
            curr = curr->next;
        }
        node->next = curr;
        node->prev = curr->prev;
        curr->prev->next = node;
        curr->prev = node;
        list->size += 1;
        return 0;
    }
    /* insert from tail behind position */
    else if(pos < -1)
    {
        bd_xjson_node* curr;
        curr = list->tail->prev;
        for(int i = -2; i > pos; i--)
        {
            curr = curr->prev;
        }
        node->next = curr->next;
        node->prev = curr;
        curr->next->prev = node;
        curr->next = node;
        list->size += 1;
        return 0;
    }
    return -1;
}

int list_erase(bd_xjson_list* list, int pos)
{
    if(NULL == list)
    {
        THROW_WARNING("LIST is not initialized");
        return -1;
    }
    if( 0 == list->size)
    {
        THROW_WARNING("emptry LIST try to erase");
        return -1;
    }
    if(pos >= (int)list->size || pos < -list->size)
    {
        THROW_WARNING("try to erase in illegal POS");
        return -1;
    }
    /* remove head */
    if( 0 == pos || pos == -list->size)
    {
        bd_xjson_node* head = list->head;
        bd_xjson_node* next = list->head->next;
        node_free(head);
        if(next)
        {
            next->prev = NULL;
            list->head = next;
        }
        else
        {
            list->head = list->tail = NULL;
        }
        list->size -= 1;
        return 0;
    }
    /* remove tail */
    if(-1 == pos || pos == list->size - 1)
    {
        bd_xjson_node* tail = list->tail;
        bd_xjson_node* prev = list->tail->prev;
        node_free(tail);
        if(prev)
        {
            prev->next = NULL;
            list->tail = prev;
        }
        else
        {
            list->head = list->tail = NULL;
        }
        list->size -= 1;
        return 0;
    }
    /* remove from head by position */
    if(pos > 0)
    {
        bd_xjson_node* curr;
        curr = list->head->next;
        for(int i = 1; i < pos; i++){
            curr = curr->next;
        }
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        node_free(curr);
        list->size -= 1;
        return 0;
    }
    /* remove from tail by position */
    else if(pos < -1)
    {
        bd_xjson_node* curr;
        curr = list->tail->prev;
        for(int i = -2; i > pos; i--){
            curr = curr->prev;
        }
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        node_free(curr);
        list->size -= 1;
        return 0;
    }
    return -1;
}

int list_free(bd_xjson_list* list)
{
    if(NULL == list)
    {
        THROW_WARNING("LIST is not initialized");
        return -1;
    }
    unsigned size = list->size;
    for(int i = 0; i < size; i++)
    {
        list_erase(list, 0);
    }
    xfree(list);
    return 0;
}

/* val: deep copy */
/* you must initialize 'val->data' in your code */
int list_find(bd_xjson_list* list, int pos, bd_xjson* val)
{
    if( NULL == list)
    {
        THROW_WARNING("LIST is not initialized");
        return -1;
    }
    if( NULL == val)
    {
        THROW_WARNING("VAL is not initialized");
        return -1;
    }
    if( 0 == list->size)
    {
        THROW_WARNING("empty list try to find");
        return -1;
    }
    if( pos >= (int)list->size || pos < -list->size)
    {
        THROW_WARNING("try to find in illegal position");
        return -1;
    }
    bd_xjson_node* node;
    /* find from head by position */
    if(pos >= 0)
    {
        node = list->head;
        for(int i = 0; i < pos; i++){
            node = node->next;
        }
    }
    /* find from tail by position */
    if(pos <= -1)
    {
        node = list->tail;
        for(int i = -1; i > pos; i--){
            node = node->prev;
        }
    }
    if(val->type != node->data.type)
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
    if(bd_xjson_copy(val, &(node->data)))
    {
        THROW_WARNING("copy data of node to VAL failed");
        return -1;
    }
    return 0;
}

int list_update(bd_xjson_list* list, int pos, bd_xjson* val)
{
    if(NULL == list)
    {
        THROW_WARNING("LIST is not initialized");
        return -1;
    }
    if(NULL == val)
    {
        THROW_WARNING("VAL is not initialized");
        return -1;
    }
    if( 0 == list->size)
    {
        THROW_WARNING("empty list try to update");
        return -1;
    }
    if( pos >= (int)list->size || pos < -list->size)
    {
        THROW_WARNING("try to update in illegal position");
        return -1;
    }
    bd_xjson_node* node;
    /* find from head by position */
    if(pos >= 0)
    {
        node = list->head;
        for(int i = 0; i < pos; i++){
            node = node->next;
        }
    }
    /* find from tail by position */
    if(pos <= -1)
    {
        node = list->tail;
        for(int i = -1; i > pos; i--){
            node = node->prev;
        }
    }

    /* free old node data */
    if(bd_xjson_free(&(node->data)))
    {
        THROW_WARNING("data of node free failed");
        return -1;
    }
    /* update type and value */
    if(bd_xjson_copy(&(node->data), val))
    {
        THROW_WARNING("copy VAL to data field of node failed");
        return -1;
    }
    return 0;
}