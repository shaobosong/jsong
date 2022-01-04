#include <string.h>
#include <stddef.h>
#include "lib/bd_xjson_base.h"
#include "lib/bd_xjson_list.h"
#include "lib/bd_xjson_htab.h"
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
            if(htab_free((bd_xjson_htab**)&(json->data)))
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
            if(list_free((bd_xjson_list**)&(json->data)))
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