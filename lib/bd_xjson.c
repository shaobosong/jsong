#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <assert.h>

#include "lib/bd_xjson_impl.h"
#include "lib/bd_xjson.h"
#include "lib/bd_xjson_list.h"
#include "lib/bd_xjson_htab.h"
#include "lib/bd_xjson_stack.h"
#include "lib/error.h"
#include "lib/alloc.h"
#include "lib/utils.h"

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
void bd_xjson_stringify_string(const char* chars, char** pstr, int* len);
void bd_xjson_stringify_array(const bd_xjson_list* list, char** pstr, int* len);
void bd_xjson_stringify_object(const bd_xjson_htab* htab, char** pstr, int* len);

int bd_xjson_parse_object(const char** const pstr, bd_xjson* json);
int bd_xjson_parse_string(const char** const pstr, bd_xjson* json);
int bd_xjson_parse_number(const char** const pstr, bd_xjson* json);
int bd_xjson_parse_array(const char** const pstr, bd_xjson* json);
int bd_xjson_parse_literal(const char** const pstr, bd_xjson* json);

int bd_xjson_copy(void* dst, const void* src)
{
    bd_xjson *d;
    const bd_xjson *s;
    bd_xjson_htab *sh;
    bd_xjson_htab **pdh;
    bd_xjson_list **pdl;

    d = dst;
    s = src;
    assert(s && d);

    d->type = s->type;
    switch(s->type) {
        case BD_XJSON_OBJECT:
            pdh = (bd_xjson_htab**) &(d->data);
            sh = s->data;
            assert(NULL == *pdh);
            assert(sh->capacity != 0);
            htab_create(pdh, sh->capacity);
            assert(d->data && s->data);
            htab_copy(d->data, s->data);
            break;

        case BD_XJSON_STRING:
            d->data = xzmalloc(strlen(s->data) + 1);
            strcat(d->data, s->data);
            break;

        case BD_XJSON_NUMBER:
            d->data = xzmalloc(sizeof (int));
            *(int*)(d->data) = *(int*)(s->data);
            break;

        case BD_XJSON_ARRAY:
            pdl = (bd_xjson_list**) &d->data;
            assert(NULL == *pdl);
            list_create(pdl);
            assert(d->data && s->data);
            list_copy(d->data, s->data);
            break;

        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            assert(0);
    }
    return 0;
}

int bd_xjson_free_data(bd_xjson* json)
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

int bd_xjson_free(void* json)
{
    if(bd_xjson_free_data(json))
    {
        THROW_WARNING("json data free failed");
        return -1;
    }
    xfree(json);
    return 0;
}

int bd_xjson_reassign(void* dst, const void* src)
{
    bd_xjson *d;
    const bd_xjson *s;
    d = dst;
    s = src;

    if(!d) {
        THROW_WARNING("null");
        return -1;
    }
    if(!s) {
        THROW_WARNING("null");
        return -1;
    }
    if(d->type != s->type) {
        THROW_WARNING("type error");
        return -1;
    }

    if(bd_xjson_free_data(d)) {
        THROW_WARNING("hash table free failed");
        return -1;
    }

    if(bd_xjson_copy(d, s)) {
        THROW_WARNING("hash table copy failed");
        return -1;
    }

    return 0;
}

static int bd_xjson_type_is_valid(bd_xjson_type type)
{
    return type >= 1 && type <= 7;
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

void bd_xjson_stringify_string(const char* chars, char** pstr, int* plen)
{
    *plen = strlen(chars) + 3;
    *pstr = xzmalloc(*plen);
    (*pstr)[0] = '\"';
    strcat(*pstr, chars);
    (*pstr)[*plen - 2] = '\"';
    return ;
}

void bd_xjson_stringify_object(const bd_xjson_htab* htab, char** pstr, int* plen)
{
    bd_xjson_stack(char*) kstk;
    bd_xjson_stack_init(kstk, htab->size);
    bd_xjson_stack(char*) vstk;
    bd_xjson_stack_init(vstk, htab->size);

    *plen = 2;
    bd_xjson_htab_iter iter = htab_begin(htab);
    bd_xjson_htab_iter end = htab_end(htab);
    bd_xjson_htab_foreach(iter, end)
    {
        int vl = 0;
        char* v = NULL;
        switch(iter.value.type)
        {
            case BD_XJSON_OBJECT:
                bd_xjson_stringify_object((bd_xjson_htab*)iter.value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_STRING:
                bd_xjson_stringify_string((char*)iter.value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_NUMBER:
                bd_xjson_stringify_number(*(int*)iter.value.data, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_ARRAY:
                bd_xjson_stringify_array((bd_xjson_list*)iter.value.data, &v, &vl);
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
                MY_ASSERT(bd_xjson_type_is_valid(iter.value.type));
                return ;
        }
        bd_xjson_stack_push(kstk, iter.key);
        bd_xjson_stack_push(vstk, v);
        *plen += (vl + strlen(iter.key) + 3);
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
        strcat(*pstr, "\"");
        strcat(*pstr, bd_xjson_stack_top(kstk));
        m_strcat(*pstr,
            "\":",
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

void bd_xjson_stringify_array(const bd_xjson_list* list, char** pstr, int* plen)
{
    /* create two stacks and their size are equal to size of list */
    bd_xjson_stack(char*) stk;
    bd_xjson_stack_init(stk, list->size);

    *plen = 2;
    bd_xjson_node* node = list->tail;
    bd_xjson_node* enode = list->nil;
    for(; node != enode; node = node->prev)
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
        m_strcat(*pstr, bd_xjson_stack_top(stk), ",", "\0");
        bd_xjson_stack_pop(stk);
    }
    (*pstr)[*plen-2] = ']';
    /* stack clear */
    bd_xjson_stack_clear(stk);

    return ;
}

void bd_xjson_stringify(const void* raw, char** pstr, int* plen)
{
    const bd_xjson* json = raw;
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

static void bypass_white_space(const char** const pstr)
{
    const char* str = *pstr;
    while(*str == '\t' || *str == ' ' || *str == '\n' || *str == '\r')
    {
        str++;
    }
    *pstr = str;
}

int bd_xjson_parse_object(const char** const pstr, bd_xjson* json)
{
    int64_t old_stk_top = g_char_stk.top;
    const char* str = *pstr;
    int64_t save_stack_top;
    bd_xjson sub;
    bd_xjson_htab **pdh;
    bd_xjson_list **pdl;

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
        /* unsupport zero-length key string */
        /* MAYBE_AND_THEN(str, '\"', THROW_WARNING("illegal character"); return -1); */
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
                pdh = (bd_xjson_htab**) &sub.data;
                htab_create(pdh, 1);
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
                pdl = (bd_xjson_list**) &sub.data;
                list_create(pdl);
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

int bd_xjson_parse_array(const char** const pstr, bd_xjson* json)
{
    const char* str = *pstr;
    bd_xjson sub;
    bd_xjson_htab **pdh;
    bd_xjson_list **pdl;

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
                pdh = (bd_xjson_htab**) &sub.data;
                htab_create(pdh, 1);
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
                pdl = (bd_xjson_list**) &sub.data;
                list_create(pdl);
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

int bd_xjson_parse_string(const char** const pstr, bd_xjson* json)
{
    int64_t old_stk_top = g_char_stk.top;
    const char* str = *pstr;

    MY_ASSERT(json->type == BD_XJSON_STRING);
    EXPECT_IF_NOT(str, '\"', THROW_WARNING("illegal character"); return -1);
    /* support zero-length string */
    /* MAYBE_AND_THEN(str, '\"', THROW_WARNING("illegal character"); return -1); */
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
int bd_xjson_parse_number(const char** const pstr, bd_xjson* json)
{
    const char* str = *pstr;
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

int bd_xjson_parse_literal(const char** const pstr, bd_xjson* json)
{
    char* literal = NULL;
    const char* str = *pstr;

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

int bd_xjson_parse_entry(const char* str, bd_xjson* json)
{
    bd_xjson_htab **pdh;
    bd_xjson_list **pdl;
    bypass_white_space(&str);
    switch(*str)
    {
        case '{':
            pdh = (bd_xjson_htab**) &json->data;
            assert(NULL == *pdh);
            htab_create(pdh, 1);
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
            pdl = (bd_xjson_list**) &json->data;
            assert(NULL == *pdl);
            list_create(pdl);
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
            THROW_WARNING("parsing unknown JSON type");
            return -1;
    }
    bypass_white_space(&str);
    EXPECT_IF_NOT(str, '\0', THROW_WARNING("illegal character"); bd_xjson_free_data(json); return -1);
    return 0;
}

int bd_xjson_parse(const char* str, void* raw)
{
    bd_xjson* json = raw;
    bd_xjson old = *json;
    json->data = NULL;

    bd_xjson_stack_init(g_char_stk, 256);
    if(bd_xjson_parse_entry(str, json))
    {
        json->data = old.data;
        bd_xjson_stack_clear(g_char_stk);
        return -1;
    }
    bd_xjson_free_data(&old);
    bd_xjson_stack_clear(g_char_stk);
    return 0;
}