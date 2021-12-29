#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "lib/bd_xjson.h"
#include "lib/error.h"

#define THROW_EXCEPTION(msg) \
    err_sys("[----%s----]: "msg, __func__)
#define THROW_WARNING(msg) \
    err_msg("[----%s----]: "msg, __func__)

#define THROW_XMALLOC_EXCEPTION \
    THROW_EXCEPTION("allocation failed")
#define THROW_XREALLOC_EXCEPTION \
    THROW_EXCEPTION("reallocation failed")
#define THROW_XFREE_EXCEPTION \
    THROW_EXCEPTION("free uninitialized memory or duplicated free already done")

#define TERMINATED(fun, msg)  \
do                            \
{                             \
    if(fun)                   \
    {                         \
        THROW_EXCEPTION(msg); \
    }                         \
} while (0)
#define CATCH(fun, msg)     \
do                          \
{                           \
    if(fun)                 \
    {                       \
        THROW_WARNING(msg); \
        return -1;          \
    }                       \
} while (0)

void* xmalloc(int s)
{
    void* p;
    p = malloc(s);
    if(!p && s != 0)
    {
        THROW_XMALLOC_EXCEPTION;
    }
    memset(p, 0, s);
    return (p);
}

void* xrealloc(void* p, int s)
{
    p = realloc(p, s);
    if(!p && s != 0)
    {
        THROW_XREALLOC_EXCEPTION;
    }
    return (p);
}

void xfree(void* p)
{
    if(!p)
    {
        THROW_XFREE_EXCEPTION;
    }
    free(p);
    return ;
}

// static const unsigned type_map[] =
// {
//     [BD_XJSON_OBJECT] = sizeof(bd_xjson_object),
//     [BD_XJSON_STRING] = sizeof(bd_xjson_string),
//     [BD_XJSON_NUMBER] = sizeof(bd_xjson_number),
//     [BD_XJSON_ARRAY] = sizeof(bd_xjson_array),
//     [BD_XJSON_TRUE] = sizeof(bd_xjson_true),
//     [BD_XJSON_FALSE] = sizeof(bd_xjson_false),
//     [BD_XJSON_NULL] = sizeof(bd_xjson_null),
// };

int node_init(bd_xjson_node** node)
{
    if(*node)
    {
        THROW_WARNING("duplicated initialization already done");
        return -1;
    }
    bd_xjson_node* n;
    n = xmalloc(sizeof *n);
    n->value = n->next = n->prev = NULL;
    n->type = -1;

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
    /* only copy type and value */
    dest->type = src->type;
    switch(src->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            dest->value = xmalloc(strlen(src->value) + 1);
            strcat(dest->value, src->value);
            break;
        case BD_XJSON_NUMBER:
            dest->value = xmalloc(sizeof (int));
            *(int*)(dest->value) = *(int*)(src->value);
            break;
        case BD_XJSON_ARRAY:
            if(list_init((bd_xjson_list**)&dest->value))
            {
                THROW_WARNING("list initializaition failed");
                return -1;
            }
            if(list_copy(dest->value, src->value))
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
    switch(node->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
        case BD_XJSON_NUMBER:
            xfree(node->value);
            break;
        case BD_XJSON_ARRAY:
            if(list_clear((bd_xjson_list**)&(node->value)))
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
    l = xmalloc(sizeof *l);
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
 * val: deep copy
 * i = -1/size, insert in tail
 * i =  0, insert in head
 */
int list_insert(bd_xjson_list* list, bd_xjson_type type, int pos, void* val)
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
    node->type = type;
    switch(type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            node->value = xmalloc(strlen(((bd_xjson_string*)val)->data) + 1);
            strcat(node->value, ((bd_xjson_string*)val)->data);
            break;
        case BD_XJSON_NUMBER:
            node->value = xmalloc(sizeof (int));
            *(int*)(node->value) = *(((bd_xjson_number*)val)->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_init((bd_xjson_list**)&(node->value)))
            {
                THROW_WARNING("list initializaition failed");
                return -1;
            }
            if(list_copy(node->value, ((bd_xjson_array*)val)->data))
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
int list_find(bd_xjson_list* list, bd_xjson_type type, int pos, void* val)
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
    if(node->type != type)
    {
        THROW_WARNING("unmatched type");
        return -1;
    }
    switch(type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            ;char** str_data = &((bd_xjson_string*)val)->data;
            if(*str_data)
            {
                xfree(*str_data);
            }
            *str_data = xmalloc(strlen(node->value) + 1);
            strcat(*str_data, node->value);
            break;
        case BD_XJSON_NUMBER:
            ;int** num_data = &((bd_xjson_number*)val)->data;
            if(num_data)
            {
                xfree(*num_data);
            }
            *num_data = xmalloc(sizeof(int));
            **num_data = *(int*)node->value;
            break;
        case BD_XJSON_ARRAY:
            if( ((bd_xjson_array*)val)->data->size ||
                ((bd_xjson_array*)val)->data->head ||
                ((bd_xjson_array*)val)->data->tail
            )
            {
                if(list_clear(&(((bd_xjson_array*)val)->data)))
                {
                    THROW_WARNING("list clear failed");
                    return -1;
                }
                if(list_init(&(((bd_xjson_array*)val)->data)))
                {
                    THROW_WARNING("list initializaition failed");
                    return -1;
                }
            }
            if(
                list_copy(
                ((bd_xjson_array*)val)->data,
                (bd_xjson_list*)(node->value))
            )
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

int list_update(bd_xjson_list* list, bd_xjson_type type, int pos, void* val)
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
    switch(node->type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
        case BD_XJSON_NUMBER:
            xfree(node->value);
            break;
        case BD_XJSON_ARRAY:
            if(list_clear((bd_xjson_list**)&(node->value)))
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
    node->type = type;
    switch(type)
    {
        case BD_XJSON_OBJECT:
            break;
        case BD_XJSON_STRING:
            node->value = xmalloc(strlen(((bd_xjson_string*)val)->data) + 1);
            strcat(node->value, ((bd_xjson_string*)val)->data);
            break;
        case BD_XJSON_NUMBER:
            node->value = xmalloc(sizeof (int));
            *(int*)(node->value) = *(((bd_xjson_number*)val)->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_init((bd_xjson_list**)&(node->value)))
            {
                THROW_WARNING("list initialization failed");
                return -1;
            }
            if(
                list_copy(
                (bd_xjson_list*)(node->value),
                ((bd_xjson_array*)val)->data)
            )
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

void obj_default_cstr(bd_xjson_string* this)
{

}
void obj_copy_cstr(bd_xjson_string* this, bd_xjson_string* str)
{

}
void obj_default_dstr(bd_xjson_string* this)
{

}

/* constructor */
void arr_default_cstr(bd_xjson_array* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized array class try to construct");
    }
    if(list_init(&(this->data)))
    {
        THROW_EXCEPTION("constructor error");
    }

}

void arr_copy_cstr(bd_xjson_array* this, bd_xjson_array* arr)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized array class try to construct");
    }
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array class try to be copied body");
    }
    /* init */
    if(list_init(&(this->data)))
    {
        THROW_EXCEPTION("constructor initialization error");
    }
    /* copy */
    if(list_copy(this->data, arr->data))
    {
        THROW_EXCEPTION("copy constructor error");
    }
}

/* destructor */
void arr_default_dstr(bd_xjson_array* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized array class try to desctruct");
    }
    if(NULL == this->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to clear");
    }
    if(list_clear(&(this->data)))
    {
        THROW_EXCEPTION("destructor error");
    }
}

void arr_add(bd_xjson_array* arr, int pos, void* val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array try to add");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    if(list_insert(arr->data, *(bd_xjson_type*)val, pos, val))
    {
        THROW_EXCEPTION("add error");
    }
}

void arr_delete(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array class try to add");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    if(list_remove(arr->data, pos))
    {
        THROW_EXCEPTION("delete error");
    }
}
void arr_search(bd_xjson_array* arr, int pos, void* val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array class try to search");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to find");
    }
    if(list_find(arr->data, *(bd_xjson_type*)val, pos, val))
    {
        THROW_EXCEPTION("search error");
    }
}
void arr_update(bd_xjson_array* arr, int pos, void* val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array class try to update");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to update");
    }
    if(list_update(arr->data, *(bd_xjson_type*)val, pos, val))
    {
        THROW_EXCEPTION("update error");
    }
}

void str_default_cstr(bd_xjson_string* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to construct");
    }
    this->data = xmalloc(1);
}
void str_copy_cstr(bd_xjson_string* this, bd_xjson_string* str)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to construct");
    }
    if(NULL == str)
    {
        THROW_EXCEPTION("uninitialized string class try to be copied body");
    }
    if(NULL == str->data)
    {
        THROW_EXCEPTION("uninitialized data of string class try to be copied body");
    }
    unsigned size = strlen(str->data) + 1;
    this->data = xmalloc(size);
    strcat(this->data, str->data);
}
void str_assign_cstr(bd_xjson_string* this, const char* chars)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to construct");
    }
    if(NULL == chars)
    {
        THROW_EXCEPTION("uninitialized characters try to be assigned body");
    }
    unsigned size = strlen(chars) + 1;
    this->data = xmalloc(size);
    strcat(this->data, chars);
}
void str_default_dstr(bd_xjson_string* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to desctruct");
    }
    if(NULL == this->data)
    {
        THROW_EXCEPTION("uninitialized data of string class try to desctruct");
    }
    xfree(this->data);
}

void num_default_cstr(bd_xjson_number* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to construct");
    }
    this->data = xmalloc(sizeof (int));
    *(this->data) = 0;
}
void num_copy_cstr(bd_xjson_number* this, bd_xjson_number* str)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to construct");
    }
    if(NULL == str)
    {
        THROW_EXCEPTION("uninitialized number class try to be copied body");
    }
    this->data = xmalloc(sizeof (int));
    *(this->data) = *(str->data);
}
void num_assign_cstr(bd_xjson_number* this, int val)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to construct");
    }
    this->data = xmalloc(sizeof (int));
    *(this->data) = val;
}
void num_default_dstr(bd_xjson_number* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to desctruct");
    }
    if(NULL == this->data)
    {
        THROW_EXCEPTION("uninitialized data of number class try to desctruct");
    }
    xfree(this->data);
}
