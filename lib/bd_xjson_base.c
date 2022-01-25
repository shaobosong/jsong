#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include "lib/bd_xjson.h"
#include "lib/bd_xjson_base.h"
#include "lib/bd_xjson_list.h"
#include "lib/bd_xjson_htab.h"
#include "lib/bd_xjson_stack.h"
#include "lib/bd_xjson_iter.h"
#include "lib/error.h"
#include "lib/alloc.h"
#include "lib/util.h"

#define EXPECT_IF_NOT(__ptr, __char, __act) \
do \
{ \
    if(*(__ptr) == __char) \
    { \
        (__ptr)++; \
    } \
    else \
    { \
        __act; \
    } \
} while(0)

#define MAYBE_AND_THEN(__ptr, __char, __act) \
do \
{ \
    if(*(__ptr) == __char) \
    { \
        (__ptr)++; \
        __act; \
    } \
} while(0)

static bd_xjson_stack(char*) g_chars_stk;
static bd_xjson_stack(char) g_char_stk;

void bd_xjson_stringify_number(int v, char** pstr, int* len);
void bd_xjson_stringify_string(char* chars, char** pstr, int* len);
void bd_xjson_stringify_array(bd_xjson_list* list, char** pstr, int* len);
void bd_xjson_stringify_object(bd_xjson_htab* htab, char** pstr, int* len);

int bd_xjson_parse_object(char** pstr, bd_xjson* json);
int bd_xjson_parse_string(char** pstr, bd_xjson* json);
int bd_xjson_parse_number(char** pstr, bd_xjson* json);
int bd_xjson_parse_array(char** pstr, bd_xjson* json);
int bd_xjson_parse_literal(char** pstr, bd_xjson* json);

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
    json->data = NULL;
    return 0;
}

static int bd_xjson_type_is_valid(bd_xjson_type type)
{
    return type != 0 && (type & ~7UL) == 0;
}

void bd_xjson_stringify_number(int v, char** pstr, int* plen)
{
    if(v == 0)
    {
        *plen = 2;
        *pstr = xzmalloc(*plen);
        strcat(*pstr, "0");
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
    *plen = sz + 1;
    *pstr = xzmalloc(*plen);
    for(int i=0; sz>0; --sz, ++i)
    {
        (*pstr)[i] = buf[sz-1];
    }
    return ;
}

void bd_xjson_stringify_string(char* chars, char** pstr, int* plen)
{
    *plen = strlen(chars) + 3;
    *pstr = xzmalloc(*plen);
    mstrcat(*pstr, "\"", chars, "\"", "\0");
    return ;
}

void bd_xjson_stringify_object(bd_xjson_htab* htab, char** pstr, int* plen)
{
    bd_xjson_stack(char*) kstk;
    bd_xjson_stack_init(kstk, htab->size);
    bd_xjson_stack(char*) vstk;
    bd_xjson_stack_init(vstk, htab->size);

    *plen = 2;
    bd_xjson_htab_foreach(htab, iter, end)
    {
        int vl = 0;
        char* v = NULL;
        switch(iter.entry.value.type)
        {
            case BD_XJSON_OBJECT:
                bd_xjson_stringify_object((bd_xjson_htab*)iter.entry.value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_STRING:
                bd_xjson_stringify_string((char*)iter.entry.value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_NUMBER:
                bd_xjson_stringify_number(*(int*)iter.entry.value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_ARRAY:
                bd_xjson_stringify_array((bd_xjson_list*)iter.entry.value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
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
                MY_ASSERT(bd_xjson_type_is_valid(iter.entry.value.type));
                return ;
        }
        bd_xjson_stack_push(kstk, iter.entry.key);
        bd_xjson_stack_push(vstk, v);
        *plen += (vl + strlen(iter.entry.key) + 3);
    }
    if(htab->size == 0)
    {
        *plen += 1;
    }
    /* concatenate every element and json string */
    *pstr = xzmalloc(*plen);
    (*pstr)[0] = '{';
    while(!bd_xjson_stack_empty(kstk))
    {
        mstrcat(*pstr,
            "\"", bd_xjson_stack_top(kstk), "\""
            ":",
            bd_xjson_stack_top(vstk),
            ",",
            "\0");
        bd_xjson_stack_pop(kstk);
        bd_xjson_stack_pop(vstk);
    }
    (*pstr)[*plen-2] = '}';
    /* stack clear */
    bd_xjson_stack_clear(kstk);
    bd_xjson_stack_clear(vstk);
    return ;
}

void bd_xjson_stringify_array(bd_xjson_list* list, char** pstr, int* plen)
{
    /* create two stacks and their size are equal to size of list */
    bd_xjson_stack(char*) stk;
    bd_xjson_stack_init(stk, list->size);

    *plen = 2;
    bd_xjson_list_foreach_in_reverse(list, node)
    {
        int vl = 0;
        char* v = NULL;
        switch(node->value.type)
        {
            case BD_XJSON_OBJECT:
                bd_xjson_stringify_object((bd_xjson_htab*)node->value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_STRING:
                bd_xjson_stringify_string((char*)node->value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_NUMBER:
                bd_xjson_stringify_number(*(int*)node->value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_ARRAY:
                bd_xjson_stringify_array((bd_xjson_list*)node->value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
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
                MY_ASSERT(bd_xjson_type_is_valid(node->value.type));
                return ;
        }
        bd_xjson_stack_push(stk, v);
        *plen += vl;
    }
    if(bd_xjson_stack_empty(stk))
    {
        *plen += 1;
    }
    /* concatenate every element and json string */
    *pstr = xzmalloc(*plen);
    (*pstr)[0] = '[';
    while(!bd_xjson_stack_empty(stk))
    {
        mstrcat(*pstr, bd_xjson_stack_top(stk), ",", "\0");
        bd_xjson_stack_pop(stk);
    }
    (*pstr)[*plen-2] = ']';
    /* stack clear */
    bd_xjson_stack_clear(stk);

    return ;
}

void bd_xjson_stringify(void* raw, char** pstr, int* plen)
{
    bd_xjson* json = (bd_xjson*)raw;
    bd_xjson_stack_init(g_chars_stk, 256);
    switch(json->type)
    {
        case BD_XJSON_OBJECT:
            bd_xjson_stringify_object((bd_xjson_htab*)json->data, pstr, plen);
            break;
        case BD_XJSON_STRING:
            bd_xjson_stringify_string((char*)json->data, pstr, plen);
            break;
        case BD_XJSON_NUMBER:
            bd_xjson_stringify_number(*(int*)json->data, pstr, plen);
            break;
        case BD_XJSON_ARRAY:
            bd_xjson_stringify_array((bd_xjson_list*)json->data, pstr, plen);
            break;
        case BD_XJSON_TRUE:
            *plen = 5;
            *pstr = xzmalloc(*plen);
            strcat(*pstr, "true");
            break;
        case BD_XJSON_FALSE:
            *plen = 6;
            *pstr = xzmalloc(*plen);
            strcat(*pstr, "false");
            break;
        case BD_XJSON_NULL:
            *plen = 5;
            *pstr = xzmalloc(*plen);
            strcat(*pstr, "null");
            break;
        default:
            MY_ASSERT(bd_xjson_type_is_valid(json->type));
            return ;
    }
    /* free stack element */
    while(!bd_xjson_stack_empty(g_chars_stk))
    {
        xfree(bd_xjson_stack_top(g_chars_stk));
        bd_xjson_stack_pop(g_chars_stk);
    }
    /* clear stack */
    bd_xjson_stack_clear(g_chars_stk);
    return ;
}

static void bypass_white_space(char** pstr)
{
    char* str = *pstr;
    while(*str == '\t' || *str == ' ' || *str == '\n' || *str == '\r')
    {
        str++;
    }
    *pstr = str;
}

int bd_xjson_parse_object(char** pstr, bd_xjson* json)
{
    int64_t old_stk_top = g_char_stk.top;
    char* str = *pstr;
    int64_t save_stack_top;
    bd_xjson sub;

    MY_ASSERT(json->type == BD_XJSON_OBJECT);
    EXPECT_IF_NOT(str, '{', THROW_WARNING("illegal character"); return -1);
    bypass_white_space(&str);
    MAYBE_AND_THEN(str, '}', *pstr = str; return 0);
    for( ; ; )
    {
        memset(&sub, 0, sizeof(sub));
        /* parse key */
        save_stack_top = g_char_stk.top;
        EXPECT_IF_NOT(str, '\"', THROW_WARNING("illegal character"); return -1);
        MAYBE_AND_THEN(str, '\"', THROW_WARNING("illegal character"); return -1); /* unsupport zero-length string */
        for(; ; str++)
        {
            if(*str == '\"')
            {
                break;
            }
            else if(*str == '\0')
            {
                THROW_WARNING("illegal NULL-Terminated character");
                return -1;
            }
            bd_xjson_stack_push(g_char_stk, *str);
        }
        EXPECT_IF_NOT(str, '\"', THROW_WARNING("illegal character"); return -1);
        bypass_white_space(&str);
        EXPECT_IF_NOT(str, ':', THROW_WARNING("illegal character"); return -1);
        bypass_white_space(&str);
        /* parse value */
        switch(*str)
        {
            case '{':
                sub.type = BD_XJSON_OBJECT;
                MY_ASSERT(htab_create((bd_xjson_htab**)&(sub.data), 1) == 0);
                if(bd_xjson_parse_object(&str, &sub))
                {
                    THROW_WARNING("JSON OBJECT parsing failed");
                    htab_free((bd_xjson_htab*)sub.data);
                    return -1;
                }
                break;
            case '\"':
                sub.type = BD_XJSON_STRING;
                if(bd_xjson_parse_string(&str, &sub))
                {
                    THROW_WARNING("JSON STRING parsing failed");
                    return -1;
                }
                break;
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                sub.type = BD_XJSON_NUMBER;
                if(bd_xjson_parse_number(&str, &sub))
                {
                    THROW_WARNING("JSON NUMBER parsing failed");
                    return -1;
                }
                break;
            case '[':
                sub.type = BD_XJSON_ARRAY;
                MY_ASSERT(list_create((bd_xjson_list**)&(sub.data)) == 0);
                if(bd_xjson_parse_array(&str, &sub))
                {
                    THROW_WARNING("JSON ARRAY parsing failed");
                    list_free((bd_xjson_list*)sub.data);
                    return -1;
                }
                break;
            case 't':
                sub.type = BD_XJSON_TRUE;
                if(bd_xjson_parse_literal(&str, &sub))
                {
                    THROW_WARNING("JSON TRUE parsing failed");
                    return -1;
                }
                break;
            case 'f':
                sub.type = BD_XJSON_FALSE;
                if(bd_xjson_parse_literal(&str, &sub))
                {
                    THROW_WARNING("JSON FALSE parsing failed");
                    return -1;
                }
                break;
            case 'n':
                sub.type = BD_XJSON_NULL;
                if(bd_xjson_parse_literal(&str, &sub))
                {
                    THROW_WARNING("JSON NULL parsing failed");
                    return -1;
                }
                break;
            default:
                THROW_WARNING("parse JSON unknown type");
                return -1;
        }
        htab_insert_direct((bd_xjson_htab*)json->data, &g_char_stk.data[save_stack_top + 1], &sub);
        bypass_white_space(&str);
        switch(*str)
        {
            case ',':
                break;
            case '}':
                MAYBE_AND_THEN(str, '}',
                    bd_xjson_stack_pop2_old_top(g_char_stk, old_stk_top); *pstr = str; return 0);
            default:
                THROW_WARNING("illeaga character");
                return -1;
        }
        EXPECT_IF_NOT(str, ',', THROW_WARNING("illegal character"); return -1);
        bypass_white_space(&str);
    }
    return -1;
}

int bd_xjson_parse_array(char** pstr, bd_xjson* json)
{
    char* str = *pstr;
    bd_xjson sub;

    MY_ASSERT(json->type == BD_XJSON_ARRAY);
    EXPECT_IF_NOT(str, '[', THROW_WARNING("illegal character"); return -1);
    bypass_white_space(&str);
    MAYBE_AND_THEN(str, ']', *pstr = str; return 0);
    for( ; ; )
    {
        memset(&sub, 0, sizeof(sub));
        /* parse value */
        switch(*str)
        {
            case '{':
                sub.type = BD_XJSON_OBJECT;
                MY_ASSERT(htab_create((bd_xjson_htab**)&(sub.data), 1) == 0);
                if(bd_xjson_parse_object(&str, &sub))
                {
                    htab_free((bd_xjson_htab*)sub.data);
                    THROW_WARNING("JSON OBJECT parsing failed");
                    return -1;
                }
                break;
            case '\"':
                sub.type = BD_XJSON_STRING;
                if(bd_xjson_parse_string(&str, &sub))
                {
                    THROW_WARNING("JSON STRING parsing failed");
                    return -1;
                }
                break;
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                sub.type = BD_XJSON_NUMBER;
                if(bd_xjson_parse_number(&str, &sub))
                {
                    THROW_WARNING("JSON NUMBER parsing failed");
                    return -1;
                }
                break;
            case '[':
                sub.type = BD_XJSON_ARRAY;
                MY_ASSERT(list_create((bd_xjson_list**)&(sub.data)) == 0);
                if(bd_xjson_parse_array(&str, &sub))
                {
                    list_free((bd_xjson_list*)sub.data);
                    THROW_WARNING("JSON ARRAY parsing failed");
                    return -1;
                }
                break;
            case 't':
                sub.type = BD_XJSON_TRUE;
                if(bd_xjson_parse_literal(&str, &sub))
                {
                    THROW_WARNING("JSON TRUE parsing failed");
                    return -1;
                }
                break;
            case 'f':
                sub.type = BD_XJSON_FALSE;
                if(bd_xjson_parse_literal(&str, &sub))
                {
                    THROW_WARNING("JSON FALSE parsing failed");
                    return -1;
                }
                break;
            case 'n':
                sub.type = BD_XJSON_NULL;
                if(bd_xjson_parse_literal(&str, &sub))
                {
                    THROW_WARNING("JSON NULL parsing failed");
                    return -1;
                }
                break;
            default:
                THROW_WARNING("parse JSON unknown type");
                return -1;
        }
        list_insert_direct((bd_xjson_list*)json->data, -1, &sub);
        bypass_white_space(&str);
        switch(*str)
        {
            case ',':
                break;
            case ']':
                MAYBE_AND_THEN(str, ']', *pstr = str; return 0);
            default:
                THROW_WARNING("illegal character");
                return -1;
        }
        EXPECT_IF_NOT(str, ',', THROW_WARNING("illegal character"); return -1);
        bypass_white_space(&str);
    }
    return -1;
}

int bd_xjson_parse_string(char** pstr, bd_xjson* json)
{
    int64_t old_stk_top = g_char_stk.top;
    char* str = *pstr;

    MY_ASSERT(json->type == BD_XJSON_STRING);
    EXPECT_IF_NOT(str, '\"', THROW_WARNING("illegal character"); return -1);
    MAYBE_AND_THEN(str, '\"', THROW_WARNING("illegal character"); return -1); /* unsupport zero-length string */
    for(; ; str++)
    {
        switch(*str)
        {
            case '\"':
                EXPECT_IF_NOT(str, '\"', THROW_WARNING("illegal character"); return -1);
                json->data = xzmalloc(g_char_stk.top - old_stk_top + 1);
                strcat(json->data, &g_char_stk.data[old_stk_top + 1]);
                /* restore old stack top */
                bd_xjson_stack_pop2_old_top(g_char_stk, old_stk_top);
                *pstr = str;
                return 0;
            case '\0':
                THROW_WARNING("illegal NULL-Terminated");
                return -1;
            default:
                bd_xjson_stack_push(g_char_stk, *str);
        }
    }
    return -1;
}

/* only support signed integer */
int bd_xjson_parse_number(char** pstr, bd_xjson* json)
{
    char* str = *pstr;
    int res = 0;
    int sign = *str == '-'? -1 : 1;

    MY_ASSERT(json->type == BD_XJSON_NUMBER);
    str = *str=='-' ? str + 1 : str;
    for(; ; str++)
    {
        switch(*str)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if(res > INT_MAX/10 || (res == INT_MAX/10 && (*str - '0') > 7))
                {
                    THROW_WARNING("integer overflow");
                    return -1;
                }
                if(res < INT_MIN/10 || (res == INT_MIN/10 && (*str - '0') > 8))
                {
                    THROW_WARNING("integer overflow");
                    return -1;
                }
                res = 10 * res + sign * (int)(*str - '0');
                break;
            default:
                json->data = xzmalloc(sizeof(int));
                *(int*)json->data = res;
                *pstr = str;
                return 0;
        }
    }
    return -1;
}

int bd_xjson_parse_literal(char** pstr, bd_xjson* json)
{
    char* literal = NULL;
    char* str = *pstr;

    MY_ASSERT(*str == 't' || *str == 'f' || *str == 'n');
    switch(*str)
    {
        case 't':
        /* to do */
        /* alter assertion to throw warning */
            MY_ASSERT(json->type == BD_XJSON_TRUE);
            literal = "true";
            break;
        case 'f':
            MY_ASSERT(json->type == BD_XJSON_FALSE);
            literal = "false";
            break;
        case 'n':
            MY_ASSERT(json->type == BD_XJSON_NULL);
            literal = "null";
            break;
        /* assertion for first letter checking in first line */
    }
    for(; *literal; literal++, str++)
    {
        if(*literal != *str)
        {
            THROW_WARNING("illegal character");
            return -1;
        }
    }
    *pstr = str;
    return 0;
}

int bd_xjson_parse_entry(char* str, bd_xjson* json)
{
    bypass_white_space(&str);
    switch(*str)
    {
        case '{':
            MY_ASSERT(htab_create((bd_xjson_htab**)&(json->data), 1) == 0);
            if(bd_xjson_parse_object(&str, json))
            {
                THROW_WARNING("JSON OBJECT parsing failed");
                htab_free((bd_xjson_htab*)json->data);
                return -1;
            }
            break;
        case '\"':
            if(bd_xjson_parse_string(&str, json))
            {
                THROW_WARNING("JSON STRING parsing failed");
                return -1;
            }
            break;
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if(bd_xjson_parse_number(&str, json))
            {
                THROW_WARNING("JSON NUMBER parsing failed");
                return -1;
            }
            break;
        case '[':
            MY_ASSERT(list_create((bd_xjson_list**)&(json->data)) == 0);
            if(bd_xjson_parse_array(&str, json))
            {
                THROW_WARNING("JSON ARRAY parsing failed");
                list_free((bd_xjson_list*)json->data);
                return -1;
            }
            break;
        case 't':
        case 'f':
        case 'n':
            if(bd_xjson_parse_literal(&str, json))
            {
                THROW_WARNING("JSON LITERAL parsing failed");
                return -1;
            }
            break;
        default:
            THROW_WARNING("JSON parsing unknown type");
            return -1;
    }
    bypass_white_space(&str);
    EXPECT_IF_NOT(str, '\0', THROW_WARNING("illegal character"); FREE_JSON_DATA(json); return -1);
    return 0;
}

int bd_xjson_parse(char* str, void* raw)
{
    bd_xjson* json = (bd_xjson*)raw;
    bd_xjson old = *json;
    json->data = NULL;

    bd_xjson_stack_init(g_char_stk, 256);
    if(bd_xjson_parse_entry(str, json))
    {
        json->data = old.data;
        bd_xjson_stack_clear(g_char_stk);
        return -1;
    }
    FREE_JSON_DATA(&old);
    bd_xjson_stack_clear(g_char_stk);
    return 0;
}