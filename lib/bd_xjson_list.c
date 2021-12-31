#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "lib/bd_xjson_list.h"
#include "lib/error.h"
#include "lib/alloc.h"

int node_init(bd_xjson_node** node)
{
    if(*node)
    {
        THROW_WARNING("duplicated initialization already done");
        return -1;
    }
    bd_xjson_node* n;
    n = xzmalloc(sizeof *n);
    n->next = n->prev = NULL;
    n->data.data = NULL;
    n->data.type = -1;

    *node = n;
    return 0;
}
/* only copy value and type */
int node_copy(bd_xjson_node* dest, bd_xjson_node* src)
{
    /* don't responsible for initializing node */
    if(NULL == dest)
    {
        THROW_WARNING("uninitialized node try to copy");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("uninitialized node try to be copied body");
        return -1;
    }
    bd_xjson* dd, * sd;
    dd = &(dest->data);
    sd = &(src->data);
    /* only copy type and value */
    dd->type = sd->type;
    switch(sd->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            dd->data = xzmalloc(strlen(sd->data) + 1);
            strcat(dd->data, sd->data);
            break;
        case BD_XJSON_NUMBER:
            dd->data = xzmalloc(sizeof (int));
            *(int*)(dd->data) = *(int*)(sd->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_init((bd_xjson_list**)&dd->data))
            {
                THROW_WARNING("list initializaition failed");
                return -1;
            }
            if(list_copy(dd->data, sd->data))
            {
                THROW_WARNING("list copy failed");
                return -1;
            }
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            THROW_WARNING("illegal type from the copied");
            return -1;
    }
    return 0;
}

int node_free(bd_xjson_node* node)
{
    bd_xjson* nd = &(node->data);
    switch(nd->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
        case BD_XJSON_NUMBER:
            xfree(nd->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_clear((bd_xjson_list**)&(nd->data)))
            {
                THROW_WARNING("list clear failed");
                return -1;
            }
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            THROW_WARNING("illegal type from the freed");
            return -1;
    }
    xfree(node);
    return 0;
}

int list_init(bd_xjson_list** list)
{
    if(*list)
    {
        THROW_WARNING("duplicated initialization already done");
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
        THROW_WARNING("uninitialized list try to copy");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("uninitialized list try to be copied body");
        return -1;
    }
    bd_xjson_node* list_node = NULL;
    bd_xjson_node* copy_node = src->head;
    bd_xjson_node* list_node_prev = NULL;
    for(; copy_node; list_node = list_node->next, copy_node = copy_node->next)
    {
        if(node_init(&(list_node)))
        {
            THROW_WARNING("node initializaition failed");
            return -1;
        }
        if(node_copy(list_node, copy_node))
        {
            THROW_WARNING("node copy failed");
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

/*
 * insert a bd_xjson from bd_xjson_node
 * val: deep copy
 * i = -1/size, insert in tail
 * i =  0, insert in head
 */
int list_insert(bd_xjson_list* list, int pos, bd_xjson* val)
{
    if(NULL == list)
    {
        THROW_WARNING("uninitialized list try to insert");
        return -1;
    }
    if(pos > list->size || pos < -list->size-1)
    {
        THROW_WARNING("try to insert in illegal position");
        return -1;
    }

    bd_xjson_node* node = NULL;
    node_init(&(node));
    bd_xjson* nd = &(node->data);
    nd->type = val->type;
    switch(val->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            nd->data = xzmalloc(strlen(val->data) + 1);
            strcat(nd->data, val->data);
            break;
        case BD_XJSON_NUMBER:
            nd->data = xzmalloc(sizeof (int));
            *(int*)(nd->data) = *(int*)(val->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_init((bd_xjson_list**)&(nd->data)))
            {
                THROW_WARNING("list initializaition failed");
                return -1;
            }
            if(list_copy(nd->data, val->data))
            {
                THROW_WARNING("list copy failed");
                return -1;
            }
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            THROW_WARNING("illegal type from the inserted");
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

int list_remove(bd_xjson_list* list, int pos)
{
    if(NULL == list)
    {
        THROW_WARNING("uninitialized list try to remove");
        return -1;
    }
    if( 0 == list->size)
    {
        THROW_WARNING("emptry list try to remove");
        return -1;
    }
    if(pos >= (int)list->size || pos < -list->size)
    {
        THROW_WARNING("try to remove in illegal position");
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

int list_clear(bd_xjson_list** list)
{
    if(NULL == *list)
    {
        THROW_WARNING("uninitialized list try to clear");
        return -1;
    }
    bd_xjson_list *l = *list;
    unsigned size = l->size;
    for(int i = 0; i < size; i++)
    {
        list_remove(l, 0);
    }
    xfree(l);
    *list = NULL; /* initialize again */
    return 0;
}

/* val: deep copy */
/* you must initialize 'val->data' in your code */
int list_find(bd_xjson_list* list, int pos, bd_xjson* val)
{
    if( NULL == list)
    {
        THROW_WARNING("uninitialized list try to find");
        return -1;
    }
    if( NULL == val)
    {
        THROW_WARNING("uninitialized class try to accept");
        return -1;
    }
    if( 0 == list->size)
    {
        THROW_WARNING("emptry list try to find");
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
    bd_xjson* nd = &(node->data);
    val->type = nd->type;
    switch(nd->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            if(val->data)
            {
                xfree(val->data);
            }
            unsigned s = strlen(nd->data) + 1;
            val->data = xzmalloc(s);
            strcat(nd->data, val->data);
            break;
        case BD_XJSON_NUMBER:
            if(val->data)
            {
                xfree(val->data);
            }
            val->data = xzmalloc(sizeof(int));
            *(int*)val->data = *(int*)nd->data;
            break;
        case BD_XJSON_ARRAY:
            if(val->data)
            {
                if(list_clear((bd_xjson_list**)&val->data))
                {
                    THROW_WARNING("list clear failed");
                    return -1;
                }
                if(list_init((bd_xjson_list**)&val->data))
                {
                    THROW_WARNING("list initializaition failed");
                    return -1;
                }
            }
            if(list_copy(val->data, nd->data))
            {
                THROW_WARNING("list copy failed");
                return -1;
            }
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            THROW_WARNING("illegal type from the found");
            return -1;
    }
    return 0;
}

int list_update(bd_xjson_list* list, int pos, bd_xjson* val)
{
    if(NULL == list)
    {
        THROW_WARNING("uninitialized list try to update");
        return -1;
    }
    if( 0 == list->size)
    {
        THROW_WARNING("emptry list try to update");
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
    bd_xjson* nd = &(node->data);
    switch(nd->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
        case BD_XJSON_NUMBER:
            xfree(nd->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_clear((bd_xjson_list**)&(nd->data)))
            {
                THROW_WARNING("list clear failed");
                return -1;
            }
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            THROW_WARNING("illegal type from the updated");
            return -1;
    }
    /* update type and value */
    nd->type = val->type;
    switch(val->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            ;unsigned s = strlen(val->data) + 1;
            nd->data = xzmalloc(s);
            strcat(nd->data, val->data);
            break;
        case BD_XJSON_NUMBER:
            nd->data = xzmalloc(sizeof(int));
            *(int*)(nd->data) = *(int*)val->data;
            break;
        case BD_XJSON_ARRAY:
            if(list_init((bd_xjson_list**)&(nd->data)))
            {
                THROW_WARNING("list initialization failed");
                return -1;
            }
            if(list_copy(nd->data, val->data))
            {
                THROW_WARNING("list copy failed");
                return -1;
            }
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            THROW_WARNING("try to update to be illegal type");
            return -1;
    }
    return 0;
}