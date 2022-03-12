#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/bd_xjson_list.h"
#include "lib/utils.h"


static bd_xjson_node* node_create()
{
    return xmallocz(sizeof(bd_xjson_node));
}

static void node_free(bd_xjson_node* n)
{
    assert(n);
    bd_xjson_free_data(&(n->value));
    xfree(n);
}

bd_xjson_list* list_create()
{
    bd_xjson_list *l = xmallocz(sizeof *l);

    /* create a dummy node */
    bd_xjson_node* n = node_create();
    n->next =
    n->prev =
    l->nil =
    l->head =
    l->tail = n;
    l->size = 0;

    return l;
}

bd_xjson_list* list_create_copy(const bd_xjson_list* s)
{
    bd_xjson_list *d;
    const bd_xjson_node *in, *en;
    bd_xjson_node *nn;

    d = list_create();

    in = s->head; /* start in head */
    en = s->nil; /* end */
    for(; in != en; in = in->next)
    {
        nn = node_create();
        bd_xjson_copy(&nn->value, &in->value);
        /* insert */
        nn->next = d->nil;
        nn->prev = d->nil->prev;
        d->nil->prev->next = nn;
        d->nil->prev = nn;
    }

    /* new head and tail node */
    d->head = d->nil->next;
    d->tail = d->nil->prev;
    d->size = s->size;

    return d;
}

void list_free(bd_xjson_list* l)
{
    bd_xjson_node *curr, *end, *next;

    assert(l);
    /* nodes free */
    curr = l->head; /* start in head */
    end = l->nil; /* end */
    next = NULL;
    for(; curr != end; curr = next)
    {
        next = curr->next;
        node_free(curr);
    }
    xfree(l->nil);
    xfree(l);
}

int list_insert_tail(bd_xjson_list* l, const bd_xjson* v)
{
    bd_xjson_node* n = node_create();
    n->value = *v;

    /* insert into tail */
    n->next = l->nil;
    n->prev = l->nil->prev;
    l->nil->prev->next = n;
    l->nil->prev = n;

    /* set list properties */
    l->head = l->nil->next;
    l->tail = l->nil->prev;
    l->size += 1;
    return 0;
}

int list_insert(bd_xjson_list* l, int pos, const bd_xjson* v)
{
    int i;
    bd_xjson_node* n;
    bd_xjson_node* in;

    if(pos > l->size || pos < -l->size-1) {
        THROW_WARNING("try to insert in illegal POS");
        return -1;
    }

    n = node_create();
    bd_xjson_copy(&n->value, v);

    if(pos >= 0) {
        /* head */
        in = l->nil->next;
        /* find node */
        for(i = 0; i < pos; i++)
        {
            in = in->next;
        }
        /* insert */
        n->next = in;
        n->prev = in->prev;
        in->prev->next = n;
        in->prev = n;
    }
    else {
        /* tail */
        in = l->nil->prev;
        /* find node */
        for(i = -1; i > pos; i--)
        {
            in = in->prev;
        }
        /* insert */
        n->next = in->next;
        n->prev = in;
        in->next->prev = n;
        in->next = n;
    }

    l->size += 1;
    l->head = l->nil->next;
    l->tail = l->nil->prev;
    return 0;
}

int list_erase(bd_xjson_list* l, int pos)
{
    int i;
    bd_xjson_node* in;

    if( 0 == l->size) {
        THROW_WARNING("emptry LIST try to erase");
        return -1;
    }
    if(pos >= l->size || pos < -l->size) {
        THROW_WARNING("try to erase in illegal POS");
        return -1;
    }

    /* remove from head by position */
    if(pos >= 0) {
        /* head */
        in = l->nil->next;
        /* find node */
        for(i = 0; i < pos; i++)
        {
            in = in->next;
        }
        /* erase node */
        in->prev->next = in->next;
        in->next->prev = in->prev;
        node_free(in);
    }
    /* remove from tail by position */
    else if(pos <= -1) {
        /* tail */
        in = l->nil->prev;
        /* find node */
        for(i = -1; i > pos; i--)
        {
            in = in->prev;
        }
        /* erase node */
        in->prev->next = in->next;
        in->next->prev = in->prev;
        node_free(in);
    }

    l->size -= 1;
    l->head = l->nil->next;
    l->tail = l->nil->prev;

    return 0;
}

/* val: deep copy */
/* you must initialize 'val->data' in your code */
int list_find(const bd_xjson_list* l, int pos, bd_xjson* val)
{
    int i;
    bd_xjson_node* n;

    if( 0 == l->size) {
        THROW_WARNING("empty l try to find");
        return -1;
    }
    if( pos >= l->size || pos < -l->size) {
        THROW_WARNING("try to find in illegal position");
        return -1;
    }

    /* find from head by position */
    if(pos >= 0) {
        n = l->head;
        for(i = 0; i < pos; i++)
        {
            n = n->next;
        }
    }
    /* find from tail by position */
    else if(pos <= -1) {
        n = l->tail;
        for(i = -1; i > pos; i--)
        {
            n = n->prev;
        }
    }
    if(val->type != n->value.type) {
        THROW_WARNING("type of VAL can't match type of found element");
        return -1;
    }
    /* free exist data */
    if(val->data) {
        bd_xjson_free_data(val);
    }
    bd_xjson_copy(val, &n->value);
    return 0;
}

int list_update(bd_xjson_list* l, int pos, const bd_xjson* v)
{
    int i;
    bd_xjson_node* n;

    if( 0 == l->size) {
        THROW_WARNING("empty l try to update");
        return -1;
    }
    if( pos >= l->size || pos <= -l->size - 1) {
        THROW_WARNING("try to update in illegal position");
        return -1;
    }
    /* find from head by position */
    if(pos >= 0)
    {
        n = l->head;
        for(i = 0; i < pos; i++)
        {
            n = n->next;
        }
    }
    /* find from tail by position */
    else if(pos <= -1)
    {
        n = l->tail;
        for(i = -1; i > pos; i--)
        {
            n = n->prev;
        }
    }

    /* free old node data */
    bd_xjson_free_data(&n->value);
    /* update type and value */
    bd_xjson_copy(&n->value, v);
    return 0;
}

int list_set(bd_xjson_list* l, int pos, const bd_xjson* v)
{

    if(pos > l->size || pos < -l->size - 1) {
        THROW_WARNING("try to set in illegal POS");
        return -1;
    }

    if(pos == l->size || pos == -l->size - 1) {
        if(list_insert(l, pos, v)) {
            THROW_WARNING("LIST set using insert method error");
            return -1;
        }
    }
    else {
        if(list_update(l, pos, v)) {
            THROW_WARNING("LIST set using update method error");
            return -1;
        }
    }

    return 0;
}

static void node_swap(bd_xjson_node* n1, bd_xjson_node* n2)
{
    void* tmp;

    if(n1 == n2) {
        return ;
    }

    tmp = n1->value.data;
    n1->value.data = n2->value.data;
    n2->value.data = tmp;
    return ;
}

static void list_qsort_recur(
    int (*compare_fn)(const void*, const void*),
    bd_xjson_node* head,
    bd_xjson_node* pivot,
    bd_xjson_node* tail)
{
    bd_xjson_node *ln, *rn;

    if(head == tail || tail->next == head) {
        return ;
    }

    ln = head;
    rn = tail;
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

bd_xjson_list_iter list_begin(const bd_xjson_list* l)
{
    bd_xjson_list_iter iter =
    {
        .index = l->head,
        .value = l->head->value
    };
    return iter;
}
bd_xjson_list_iter list_end(const bd_xjson_list* l)
{
    bd_xjson_list_iter iter =
    {
        .index = l->nil
    };
    return iter;
}
bd_xjson_list_iter list_iterate(bd_xjson_list_iter iter)
{
    iter.index = ((bd_xjson_node*)iter.index)->next;
    iter.value = ((bd_xjson_node*)iter.index)->value;
    return iter;
}
bd_xjson_list_iter list_rbegin(const bd_xjson_list* l)
{
    bd_xjson_list_iter iter =
    {
        .index = l->tail,
        .value = l->tail->value
    };
    return iter;
}
bd_xjson_list_iter list_rend(const bd_xjson_list* l)
{
    bd_xjson_list_iter iter =
    {
        .index = l->nil
    };
    return iter;
}
bd_xjson_list_iter list_riterate(bd_xjson_list_iter iter)
{
    iter.index = ((bd_xjson_node*)iter.index)->prev;
    iter.value = ((bd_xjson_node*)iter.index)->value;
    return iter;
}
int list_iter_get(bd_xjson_list_iter iter, bd_xjson* val)
{
    if(val->type != iter.value.type)
    {
        THROW_WARNING("unmatched JSON type to get value in iterator");
        return -1;
    }
    /* free old val data if exist */
    if(val->data) {
        bd_xjson_free_data(val);
    }
    /* copy from iter data */
    bd_xjson_copy(val, &iter.value);
    return 0;
}