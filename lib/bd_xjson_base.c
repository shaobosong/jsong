#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include "lib/bd_xjson_base.h"
#include "lib/bd_xjson_list.h"
#include "lib/bd_xjson_htab.h"
#include "lib/bd_xjson_stack.h"
#include "lib/bd_xjson_iter.h"
#include "lib/error.h"
#include "lib/alloc.h"
int bd_xjson_copy(bd_xjson* dest, bd_xjson* src)
{
    if(NULL == dest)
    {
        THROW_WARNING("uninitialized bd_xjson try to copy");
        return -1;
    }
    if(NULL == src)
    {
        THROW_WARNING("uninitialized bd_xjson try to be the copied");
        return -1;
    }
    dest->type = src->type;
    switch(src->type)
    {
        case BD_XJSON_OBJECT:
            if(htab_create(
                (bd_xjson_htab**)&(dest->data),
                ((bd_xjson_htab*)src->data)->capacity
            ))
            {
                THROW_WARNING("hash table initializaition failed");
                return -1;
            }
            if(htab_copy(dest->data, src->data))
            {
                THROW_WARNING("hash table copy failed");
                return -1;
            }
            break;
        case BD_XJSON_STRING:
            dest->data = xzmalloc(strlen(src->data) + 1);
            strcat(dest->data, src->data);
            break;
        case BD_XJSON_NUMBER:
            dest->data = xzmalloc(sizeof (int));
            *(int*)(dest->data) = *(int*)(src->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_create((bd_xjson_list**)&dest->data))
            {
                THROW_WARNING("list initializaition failed");
                return -1;
            }
            if(list_copy(dest->data, src->data))
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

int bd_xjson_free(bd_xjson* json)
{
    switch(json->type)
    {
        case BD_XJSON_OBJECT:
            if(htab_free(json->data))
            {
                THROW_WARNING("hash table clear failed");
                return -1;
            }
            break;
        case BD_XJSON_STRING:
        case BD_XJSON_NUMBER:
            xfree(json->data);
            break;
        case BD_XJSON_ARRAY:
            if(list_free(json->data))
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
    return 0;
}



static char* mstrcat(char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    const char* a;
    int t = 0;
    while(s[t])
    {
        ++t;
    }
    while(*(a = va_arg(ap, const char*)))
    {
        int i = 0;
        while(a[i])
        {
            s[t] = a[i];
            ++t;
            ++i;
        }
    }
    s[t] = '\0';
    va_end(ap);
    return s;
}

void num_to_str(int v, char** str, int* len)
{
    if(v == 0)
    {
        *len = 2;
        *str = xzmalloc(*len);
        strcat(*str, "0");
        return ;
    }
    int t = v;
    char buf[11];
    bzero(buf, 11);
    int sz = 0;
    while(t)
    {
        buf[sz] = v>0 ? (t%10 + '0') : ('0'- t%10);
        t /= 10;
        ++sz;
    }
    if(v < 0)
    {
        buf[sz] = '-';
        ++sz;
    }
    *len = sz + 1;
    *str = xzmalloc(*len);
    for(int i=0; sz>0; --sz, ++i)
    {
        (*str)[i] = buf[sz-1];
    }
    return ;
}

void chars_to_str(char* chars, char** str, int* len)
{
    *len = strlen(chars) + 3;
    *str = xzmalloc(*len);
    mstrcat(*str, "\"", chars, "\"", "\0");
    return ;
}
void arr_to_str(bd_xjson_list* list, char** str, int* len);
void obj_to_str(bd_xjson_htab* htab, char** str, int* len)
{
    bd_xjson_stack(char*) kstk;
    bd_xjson_stack_init(kstk, htab->size);
    bd_xjson_stack(char*) vstk;
    bd_xjson_stack_init(vstk, htab->size);
    bd_xjson_stack(char*) tofree;
    bd_xjson_stack_init(tofree, htab->size);

    *len = 2;
    bd_xjson_htab_foreach(htab, iter, end)
    {
        int vl = 0;
        char* v = NULL;
        switch(iter.entry.value.type)
        {
            case BD_XJSON_OBJECT:
                obj_to_str((bd_xjson_htab*)iter.entry.value.data, &v, &vl);
                break;
            case BD_XJSON_STRING:
                chars_to_str(iter.entry.value.data, &v, &vl);
                bd_xjson_stack_push(tofree, v);
                break;
            case BD_XJSON_NUMBER:
                num_to_str(*(int*)iter.entry.value.data, &v, &vl);
                bd_xjson_stack_push(tofree, v);
                break;
            case BD_XJSON_ARRAY:
                arr_to_str((bd_xjson_list*)iter.entry.value.data, &v, &vl);
                bd_xjson_stack_push(tofree, v);
                break;
            case BD_XJSON_TRUE:
                v = "true";
                vl = 5;
                break;
            case BD_XJSON_FALSE:
                v = "false";
                vl = 6;
                break;
            case BD_XJSON_NULL:
                v = "null";
                vl = 5;
                break;
            default:
                THROW_WARNING("illegal type");
                while(!bd_xjson_stack_empty(tofree))
                {
                    xfree(bd_xjson_stack_top(tofree));
                    bd_xjson_stack_pop(tofree);
                }
                return ;
        }
        bd_xjson_stack_push(kstk, iter.entry.key);
        bd_xjson_stack_push(vstk, v);
        *len += (vl + strlen(iter.entry.key) + 3);
    }
    if(htab->size == 0)
    {
        *len += 1;
    }
    /* concatenate every element and json string */
    *str = xzmalloc(*len);
    (*str)[0] = '{';
    while(!bd_xjson_stack_empty(kstk))
    {
        mstrcat(*str,
            "\"", bd_xjson_stack_top(kstk), "\""
            ":",
            bd_xjson_stack_top(vstk),
            ",",
            "\0");
        bd_xjson_stack_pop(kstk);
        bd_xjson_stack_pop(vstk);
    }
    (*str)[*len-2] = '}';
    /* stack free element */
    while(!bd_xjson_stack_empty(tofree))
    {
        xfree(bd_xjson_stack_top(tofree));
        bd_xjson_stack_pop(tofree);
    }
    /* stack clear */
    bd_xjson_stack_clear(kstk);
    bd_xjson_stack_clear(vstk);
    bd_xjson_stack_clear(tofree);
    return ;
}

void arr_to_str(bd_xjson_list* list, char** str, int* len)
{
    /* create two stacks and their size are equal to size of list */
    bd_xjson_stack(char*) stk;
    bd_xjson_stack_init(stk, list->size);
    bd_xjson_stack(char*) tofree;
    bd_xjson_stack_init(tofree, list->size);

    *len = 2;
    bd_xjson_list_foreach_in_reverse(list, node)
    {
        int l;
        char* s = NULL;
        switch(node->value.type)
        {
            case BD_XJSON_OBJECT:
                obj_to_str((bd_xjson_htab*)node->value.data, &s, &l);
                break;
            case BD_XJSON_STRING:
                chars_to_str(node->value.data, &s, &l);
                bd_xjson_stack_push(tofree, s);
                break;
            case BD_XJSON_NUMBER:
                num_to_str(*(int*)node->value.data, &s, &l);
                bd_xjson_stack_push(tofree, s);
                break;
            case BD_XJSON_ARRAY:
                arr_to_str((bd_xjson_list*)node->value.data, &s, &l);
                bd_xjson_stack_push(tofree, s);
                break;
            case BD_XJSON_TRUE:
                s = "true";
                l = 5;
                break;
            case BD_XJSON_FALSE:
                s = "false";
                l = 6;
                break;
            case BD_XJSON_NULL:
                s = "null";
                l = 5;
                break;
            default:
                THROW_WARNING("illegal type");
                while(!bd_xjson_stack_empty(tofree))
                {
                    xfree(bd_xjson_stack_top(tofree));
                    bd_xjson_stack_pop(tofree);
                }
                return ;
        }
        bd_xjson_stack_push(stk, s);
        *len += l;
    }
    if(bd_xjson_stack_empty(stk))
    {
        *len += 1;
    }
    /* concatenate every element and json string */
    *str = xzmalloc(*len);
    (*str)[0] = '[';
    while(!bd_xjson_stack_empty(stk))
    {
        mstrcat(*str, bd_xjson_stack_top(stk), ",", "\0");
        bd_xjson_stack_pop(stk);
    }
    (*str)[*len-2] = ']';
    /* stack free element */
    while(!bd_xjson_stack_empty(tofree))
    {
        xfree(bd_xjson_stack_top(tofree));
        bd_xjson_stack_pop(tofree);
    }
    /* stack clear */
    bd_xjson_stack_clear(stk);
    bd_xjson_stack_clear(tofree);
    return ;
}

char* bd_xjson_stringify(void* __bd_xjson)
{
    char* json_str = NULL;
    int len;
    bd_xjson* json = (bd_xjson*)__bd_xjson;
    switch(json->type)
    {
        case BD_XJSON_OBJECT:
            obj_to_str((bd_xjson_htab*)json->data, &json_str, &len);
            break;
        case BD_XJSON_STRING:
            chars_to_str(json->data, &json_str, &len);
            break;
        case BD_XJSON_NUMBER:
            num_to_str(*(int*)json->data, &json_str, &len);
            break;
        case BD_XJSON_ARRAY:
            arr_to_str((bd_xjson_list*)json->data, &json_str, &len);
            break;
        case BD_XJSON_TRUE:
            json_str = xzmalloc(5);
            strcat(json_str, "true");
            break;
        case BD_XJSON_FALSE:
            json_str = xzmalloc(6);
            strcat(json_str, "false");
            break;
        case BD_XJSON_NULL:
            json_str = xzmalloc(5);
            strcat(json_str, "null");
            break;
        default:
            break;
    }
    return json_str;
}