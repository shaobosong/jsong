#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "lib/bd_xjson_list.h"
#include "lib/error.h"
#include "lib/alloc.h"

static int node_create(bd_xjson_node** node)
{
    if(*node)
    {
        THROW_WARNING("NODE was initialized");
        return -1;
    }
    bd_xjson_node* n;
    n = xzmalloc(sizeof *n);

    *node = n;
    return 0;
}

static int node_free(bd_xjson_node* node)
{
    if(NULL == node)
    {
        THROW_WARNING("NODE is not initialized");
        return -1;
    }
    if(bd_xjson_free(&(node->value)))
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

    /* create a dummy node */
    bd_xjson_node* n = NULL;
    node_create(&n);
    n->next = n;
    n->prev = n;

    l->nil = l->head = l->tail = n;
    l->size = 0;

    *list = l;
    return 0;
}

int list_copy(bd_xjson_list* dest, const bd_xjson_list* src)
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

    const bd_xjson_node* snode = src->head; /* start in head */
    const bd_xjson_node* enode = src->nil; /* end */
    for(; snode != enode; snode = snode->next)
    {
        bd_xjson_node* dnode = NULL;
        if(node_create(&dnode))
        {
            THROW_WARNING("node initializaition failed");
            return -1;
        }
        if(bd_xjson_copy(&dnode->value, &snode->value))
        {
            THROW_WARNING("copy VAL to data field of node failed");
            return -1;
        }
        /* old tail node */
        bd_xjson_node* tail = dest->nil->prev;
        /* insert */
        dnode->next = tail->next;
        dnode->prev = tail;
        tail->next->prev = dnode;
        tail->next = dnode;
    }
    /* new head and tail node */
    dest->head = dest->nil->next;
    dest->tail = dest->nil->prev;
    dest->size = src->size;
    return 0;
}

int list_insert_direct(bd_xjson_list* list, int pos, const bd_xjson* val)
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
    if(node_create(&node))
    {
        THROW_WARNING("node initializaition failed");
        return -1;
    }
    node->value = *val;

    if(pos >= 0)
    {
        bd_xjson_node* curr;
        /* head */
        curr = list->nil->next;
        /* find node */
        for(int i = 0; i < pos; i++)
        {
            curr = curr->next;
        }
        /* insert */
        node->next = curr;
        node->prev = curr->prev;
        curr->prev->next = node;
        curr->prev = node;
    } else
    {
        bd_xjson_node* curr;
        /* tail */
        curr = list->nil->prev;
        /* find node */
        for(int i = -1; i > pos; i--)
        {
            curr = curr->prev;
        }
        /* insert */
        node->next = curr->next;
        node->prev = curr;
        curr->next->prev = node;
        curr->next = node;
    }

    list->size += 1;
    list->head = list->nil->next;
    list->tail = list->nil->prev;
    return 0;
}

int list_insert(bd_xjson_list* list, int pos, const bd_xjson* val)
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
    if(node_create(&node))
    {
        THROW_WARNING("node initializaition failed");
        return -1;
    }
    if(bd_xjson_copy(&node->value, val))
    {
        THROW_WARNING("copy VAL to data field of node failed");
        return -1;
    }

    if(pos >= 0)
    {
        bd_xjson_node* curr;
        /* head */
        curr = list->nil->next;
        /* find node */
        for(int i = 0; i < pos; i++)
        {
            curr = curr->next;
        }
        /* insert */
        node->next = curr;
        node->prev = curr->prev;
        curr->prev->next = node;
        curr->prev = node;
    }
    else if(pos <= -1)
    {
        bd_xjson_node* curr;
        /* tail */
        curr = list->nil->prev;
        /* find node */
        for(int i = -1; i > pos; i--)
        {
            curr = curr->prev;
        }
        /* insert */
        node->next = curr->next;
        node->prev = curr;
        curr->next->prev = node;
        curr->next = node;
    }

    list->size += 1;
    list->head = list->nil->next;
    list->tail = list->nil->prev;
    return 0;
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
    if(pos >= list->size || pos < -list->size)
    {
        THROW_WARNING("try to erase in illegal POS");
        return -1;
    }

    /* remove from head by position */
    if(pos >= 0)
    {
        bd_xjson_node* curr;
        /* head */
        curr = list->nil->next;
        /* find node */
        for(int i = 0; i < pos; i++)
        {
            curr = curr->next;
        }
        /* erase node */
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        node_free(curr);
    }
    /* remove from tail by position */
    else if(pos <= -1)
    {
        bd_xjson_node* curr;
        /* tail */
        curr = list->nil->prev;
        /* find node */
        for(int i = -1; i > pos; i--)
        {
            curr = curr->prev;
        }
        /* erase node */
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        node_free(curr);
    }

    list->size -= 1;
    list->head = list->nil->next;
    list->tail = list->nil->prev;

    return 0;
}

int list_free(bd_xjson_list* list)
{
    if(NULL == list)
    {
        THROW_WARNING("LIST is not initialized");
        return -1;
    }
    bd_xjson_node* curr = list->head; /* start in head */
    bd_xjson_node* end = list->nil; /* end */
    bd_xjson_node* next = NULL;
    for(; curr != end; curr = next)
    {
        next = curr->next;
        node_free(curr);
    }
    xfree(list->nil);
    xfree(list);
    return 0;
}

/* val: deep copy */
/* you must initialize 'val->data' in your code */
int list_find(const bd_xjson_list* list, int pos, bd_xjson* val)
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
    if( pos >= list->size || pos < -list->size)
    {
        THROW_WARNING("try to find in illegal position");
        return -1;
    }
    bd_xjson_node* node = NULL;
    /* find from head by position */
    if(pos >= 0)
    {
        node = list->head;
        for(int i = 0; i < pos; i++){
            node = node->next;
        }
    }
    /* find from tail by position */
    else if(pos <= -1)
    {
        node = list->tail;
        for(int i = -1; i > pos; i--){
            node = node->prev;
        }
    }
    if(val->type != node->value.type)
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
    if(bd_xjson_copy(val, &node->value))
    {
        THROW_WARNING("copy data of node to VAL failed");
        return -1;
    }
    return 0;
}

int list_update(bd_xjson_list* list, int pos, const bd_xjson* val)
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
    if( pos >= list->size || pos <= -list->size - 1)
    {
        THROW_WARNING("try to update in illegal position");
        return -1;
    }
    bd_xjson_node* node = NULL;
    /* find from head by position */
    if(pos >= 0)
    {
        node = list->head;
        for(int i = 0; i < pos; i++){
            node = node->next;
        }
    }
    /* find from tail by position */
    else if(pos <= -1)
    {
        node = list->tail;
        for(int i = -1; i > pos; i--){
            node = node->prev;
        }
    }

    /* free old node data */
    if(bd_xjson_free(&node->value))
    {
        THROW_WARNING("data of node free failed");
        return -1;
    }
    /* update type and value */
    if(bd_xjson_copy(&node->value, val))
    {
        THROW_WARNING("copy VAL to data field of node failed");
        return -1;
    }
    return 0;
}

int list_set(bd_xjson_list* list, int pos, const bd_xjson* val)
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
    if(pos > list->size || pos < -list->size - 1)
    {
        THROW_WARNING("try to set in illegal POS");
        return -1;
    }

    if(pos == list->size || pos == -list->size - 1)
    {
        if(list_insert(list, pos, val))
        {
            THROW_WARNING("LIST set using insert method error");
            return -1;
        }
    }
    else
    {
        if(list_update(list, pos, val))
        {
            THROW_WARNING("LIST set using update method error");
            return -1;
        }
    }

    return 0;
}

static void node_swap(bd_xjson_node* node1, bd_xjson_node* node2)
{
    if(node1 == node2)
    {
        return ;
    }
    void* temp = node1->value.data;
    node1->value.data = node2->value.data;
    node2->value.data = temp;
    return ;
}

static void list_qsort_recur(
    int (*compare_fn)(const void*, const void*),
    bd_xjson_node* head,
    bd_xjson_node* pivot,
    bd_xjson_node* tail)
{
    if(head == tail || tail->next == head)
    {
        return ;
    }
    bd_xjson_node* ln = head;
    bd_xjson_node* rn = tail;
    while(ln != rn)
    {
        while(ln != rn && compare_fn(pivot->value.data, rn->value.data) <= 0)
        {
            rn = rn->prev;
        }
        while(ln != rn && compare_fn(ln->value.data, pivot->value.data) <= 0)
        {
            ln = ln->next;
        }
        node_swap(ln, rn);
    }
    node_swap(rn, pivot);
    list_qsort_recur(compare_fn, head, head, rn->prev);
    list_qsort_recur(compare_fn, rn->next, rn->next, tail);
}

void list_qsort(bd_xjson_list* list, int (*compare_fn)(const void*, const void*))
{
    list_qsort_recur(compare_fn, list->head, list->head, list->tail);
}

bd_xjson_list_iter list_begin(const bd_xjson_list* list)
{
    bd_xjson_list_iter iter =
    {
        .index = list->head,
        .data = *(iter.index)
    };
    return iter;
}
bd_xjson_list_iter list_end(const bd_xjson_list* list)
{
    bd_xjson_list_iter iter =
    {
        .index = list->nil
    };
    return iter;
}
bd_xjson_list_iter list_iterate(bd_xjson_list_iter iter)
{
    iter.index = iter.data.next;
    iter.data = *(iter.index);
    return iter;
}
bd_xjson_list_iter list_rbegin(const bd_xjson_list* list)
{
    bd_xjson_list_iter iter =
    {
        .index = list->tail,
        .data = *(iter.index)
    };
    return iter;
}
bd_xjson_list_iter list_rend(const bd_xjson_list* list)
{
    bd_xjson_list_iter iter =
    {
        .index = list->nil
    };
    return iter;
}
bd_xjson_list_iter list_riterate(bd_xjson_list_iter iter)
{
    iter.index = iter.data.prev;
    iter.data = *(iter.index);
    return iter;
}
int list_iter_get(bd_xjson_list_iter iter, bd_xjson* val)
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
    if(bd_xjson_copy(val, &iter.data.value))
    {
        THROW_WARNING("copy failed");
        return -1;
    }
    return 0;
}