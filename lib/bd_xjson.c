#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <assert.h>

#include "lib/bd_xjson_impl.h"
#include "lib/bd_xjson.h"
#include "lib/bd_xjson_list.h"
#include "lib/bd_xjson_htab.h"
#include "lib/bd_xjson_stack.h"
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

static void bd_xjson_stringify_number(const bd_xjson *json, char** pstr, int* len);
static void bd_xjson_stringify_string(const bd_xjson *json, char** pstr, int* len);
static void bd_xjson_stringify_array(const bd_xjson* json, char** pstr, int* len);
static void bd_xjson_stringify_object(const bd_xjson* json, char** pstr, int* len);

static int bd_xjson_parse_object(const char** const pstr, bd_xjson* json);
static int bd_xjson_parse_string(const char** const pstr, bd_xjson* json);
static int bd_xjson_parse_number(const char** const pstr, bd_xjson* json);
static int bd_xjson_parse_array(const char** const pstr, bd_xjson* json);
static int bd_xjson_parse_literal(const char** const pstr, bd_xjson* json);

void bd_xjson_copy(bd_xjson* dst, const bd_xjson* src)
{
    /* no more type checking */
    dst->type = src->type;
    switch(src->type)
    {
        case BD_XJSON_OBJECT:
            assert(NULL == dst->data);
            dst->data = htab_create_copy(src->data);
            break;
        case BD_XJSON_STRING:
            dst->data = xmallocz(strlen(src->data) + 1);
            strcat(dst->data, src->data);
            break;
        case BD_XJSON_NUMBER:
            dst->data = xmallocz(sizeof (int));
            *(int*)(dst->data) = *(int*)(src->data);
            break;
        case BD_XJSON_ARRAY:
            assert(NULL == dst->data);
            dst->data = list_create_copy(src->data);
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            break;
        default:
            assert(0);
    }
}

void bd_xjson_free_data(bd_xjson* json)
{
    switch(json->type)
    {
        case BD_XJSON_OBJECT:
            assert(json->data);
            htab_free(json->data);
            break;
        case BD_XJSON_STRING:
        case BD_XJSON_NUMBER:
            assert(json->data);
            xfree(json->data);
            break;
        case BD_XJSON_ARRAY:
            assert(json->data);
            list_free(json->data);
            break;
        case BD_XJSON_TRUE:
        case BD_XJSON_FALSE:
        case BD_XJSON_NULL:
            assert(NULL == json->data);
            break;
        default:
            assert(0);
    }
    json->data = NULL;
}

int bd_xjson_free(void* json)
{
    assert(json);
    bd_xjson_free_data(json);
    xfree(json);
    return 0;
}

int bd_xjson_reassign(void* dst, const void* src)
{
    bd_xjson *d;
    const bd_xjson *s;
    d = dst;
    s = src;

    assert(d);
    bd_xjson_free_data(d);

    assert(d && s);
    assert(d->type == s->type);
    bd_xjson_copy(d, s);

    return 0;
}

static void
bd_xjson_stringify_number(const bd_xjson* json, char** pstr, int* plen)
{
    int num;

    num = *(int*)json->data;
    *plen = snprintf(NULL, 0, "%d", num) + 1;
    *pstr = xmallocz(*plen);
    snprintf(*pstr, *plen, "%d", num);
}

static void
bd_xjson_stringify_string(const bd_xjson *json, char **str, int *len)
{
    int i, n, l, u, hu;
    const char *s = json->data;
    char *d;
    char dec_hex[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    /* TODO: support '\0' */
    l = strlen(s);
    n = 0;
    for(i = 0; i < l; i++) {
        switch(s[i]) {
        case '\"':
        case '\\':
        /* case '/': */
        case '\b':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
            n += 2;
            break;
        default:
            /* utf-8: 2~4 bytes */
            if((0xc0 & s[i]) == 0x0c0) {
                switch(s[i] & 0xf0) {
                case 0xc0:
                case 0xd0:
                    i += 1;
                    break;
                case 0xe0:
                    i += 2;
                    break;
                /* surrogate pair */
                case 0xf0:
                    i += 3;
                    n += 6;
                    break;
                default:
                    assert(0);
                }
                n += 6;
            /* utf-8: 1 byte */
            } else {
                n++;
            }
        }
    }

    d = xmallocz(n + 3);
    n = 0;
    d[n++] = '\"';
    for(i = 0; i < l; i++) {
        switch(s[i]) {
        case '\"':
        case '\\':
        /* case '/': */
            d[n++] = '\\';
            d[n++] = s[i];
            break;
        case '\b':
            d[n++] = '\\';
            d[n++] = 'b';
            break;
        case '\f':
            d[n++] = '\\';
            d[n++] = 'f';
            break;
        case '\n':
            d[n++] = '\\';
            d[n++] = 'n';
            break;
        case '\r':
            d[n++] = '\\';
            d[n++] = 'r';
            break;
        case '\t':
            d[n++] = '\\';
            d[n++] = 't';
            break;
        default:
            /* utf-8: 2~4 bytes */
            if((0xc0 & s[i]) == 0x0c0) {
                d[n++] = '\\';
                d[n++] = 'u';
                u = 0;
                switch(s[i] & 0xf0) {
                /* U+0080 ~ U+07FF: 110XXXXX 10XXXXXX */
                case 0xc0:
                case 0xd0:
                    u |= (s[i++] & 0x1f) << 6;
                    u |= s[i] & 0x3f;
                    break;
                /* U+0800 ~ U+FFFF: 1110XXXX 10XXXXXX 10XXXXXX */
                case 0xe0:
                    u |= (s[i++] & 0xf)  << 12;
                    u |= (s[i++] & 0x3f) << 6;
                    u |= s[i] & 0x3f;
                    break;
                /* U+10000 ~ U+10FFFF: 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX */
                case 0xf0:
                    /* surrogate pair: */
                    u |= (s[i++] & 0x3)  << 18;
                    u |= (s[i++] & 0x3f) << 12;
                    u |= (s[i++] & 0x3f) << 6;
                    u |= s[i] & 0x3f;
                    /* high surrogate */
                    hu = extract32(u - 0x10000, 10, 10);
                    hu += 0xd800;
                    d[n++] = dec_hex[(hu >> 12) & 0xf];
                    d[n++] = dec_hex[(hu >> 8) & 0xf];
                    d[n++] = dec_hex[(hu >> 4) & 0xf];
                    d[n++] = dec_hex[(hu >> 0) & 0xf];
                    /* low surrogate */
                    d[n++] = '\\';
                    d[n++] = 'u';
                    u = extract32(u - 0x10000, 0, 10);
                    u += 0xdc00;
                    break;
                default:
                    assert(0);
                }
                d[n++] = dec_hex[(u >> 12) & 0xf];
                d[n++] = dec_hex[(u >> 8) & 0xf];
                d[n++] = dec_hex[(u >> 4) & 0xf];
                d[n++] = dec_hex[(u >> 0) & 0xf];
            /* utf-8: 1 byte */
            } else {
                d[n++] = s[i];
            }
        }
    }
    d[n++] = '\"';
    d[n++] = '\0';

    *len = n;
    *str = d;
    return ;
}

static void
bd_xjson_stringify_object(const bd_xjson* json, char** pstr, int* plen)
{
    int vl;
    char* v;
    const bd_xjson_htab* htab;
    bd_xjson_stack(char*) kstk, vstk;
    bd_xjson_htab_iter iter, end;

    htab = json->data;
    bd_xjson_stack_init(kstk, htab->size);
    bd_xjson_stack_init(vstk, htab->size);

    *plen = 2;

    assert(htab);
    iter = htab_begin(htab);
    end = htab_end(htab);
    bd_xjson_htab_foreach(iter, end)
    {
        vl = 0;
        v = NULL;
        switch(iter.value.type)
        {
            case BD_XJSON_OBJECT:
                bd_xjson_stringify_object(&iter.value, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_STRING:
                bd_xjson_stringify_string(&iter.value, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_NUMBER:
                bd_xjson_stringify_number(&iter.value, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_ARRAY:
                bd_xjson_stringify_array(&iter.value, &v, &vl);
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
                assert(0);
        }
        bd_xjson_stack_push(kstk, iter.key);
        bd_xjson_stack_push(vstk, v);
        *plen += (vl + strlen(iter.key) + 3);
    }
    if(htab->size == 0) {
        *plen += 1;
    }
    /* concatenate every element and json string */
    *pstr = xmallocz(*plen);
    (*pstr)[0] = '{';
    while(!bd_xjson_stack_empty(kstk))
    {
        strcat(*pstr, "\"");
        strcat(*pstr, bd_xjson_stack_top(kstk));
        xmstrcat(*pstr,
            "\":",
            bd_xjson_stack_top(vstk),
            ",",
            NULL);
        bd_xjson_stack_pop(kstk);
        bd_xjson_stack_pop(vstk);
    }
    (*pstr)[*plen-2] = '}';
    /* stack clear */
    bd_xjson_stack_clear(kstk);
    bd_xjson_stack_clear(vstk);
    return ;
}

static void
bd_xjson_stringify_array(const bd_xjson* json, char** pstr, int* plen)
{
    int vl;
    char* v;
    bd_xjson_list *list;
    bd_xjson_node *node, *enode;

    list = json->data;
    /* create two stacks and their size are equal to size of list */
    bd_xjson_stack(char*) stk;
    bd_xjson_stack_init(stk, list->size);

    *plen = 2;

    assert(list);
    node = list->tail;
    enode = list->nil;
    for(; node != enode; node = node->prev)
    {
        vl = 0;
        v = NULL;
        switch(node->value.type)
        {
            case BD_XJSON_OBJECT:
                bd_xjson_stringify_object(&node->value, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_STRING:
                bd_xjson_stringify_string(&node->value, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_NUMBER:
                bd_xjson_stringify_number(&node->value, &v, &vl);
                bd_xjson_stack_push(g_chars_stk, v);
                break;
            case BD_XJSON_ARRAY:
                bd_xjson_stringify_array(&node->value, &v, &vl);
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
                assert(0);
                return ;
        }
        bd_xjson_stack_push(stk, v);
        *plen += vl;
    }
    if(bd_xjson_stack_empty(stk)) {
        *plen += 1;
    }
    /* concatenate every element and json string */
    *pstr = xmallocz(*plen);
    (*pstr)[0] = '[';
    while(!bd_xjson_stack_empty(stk))
    {
        xmstrcat(*pstr, bd_xjson_stack_top(stk), ",", NULL);
        bd_xjson_stack_pop(stk);
    }
    (*pstr)[*plen-2] = ']';
    /* stack clear */
    bd_xjson_stack_clear(stk);

    return ;
}

int bd_xjson_stringify(const void* json, char** pstr, int* plen)
{
    int l;

    if(!plen) {
        plen = &l;
    }
    bd_xjson_stack_init(g_chars_stk, 256);
    switch(((bd_xjson*)json)->type)
    {
        case BD_XJSON_OBJECT:
            bd_xjson_stringify_object(json, pstr, plen);
            break;
        case BD_XJSON_STRING:
            bd_xjson_stringify_string(json, pstr, plen);
            break;
        case BD_XJSON_NUMBER:
            bd_xjson_stringify_number(json, pstr, plen);
            break;
        case BD_XJSON_ARRAY:
            bd_xjson_stringify_array(json, pstr, plen);
            break;
        case BD_XJSON_TRUE:
            *plen = 5;
            *pstr = xmallocz(*plen);
            strcat(*pstr, "true");
            break;
        case BD_XJSON_FALSE:
            *plen = 6;
            *pstr = xmallocz(*plen);
            strcat(*pstr, "false");
            break;
        case BD_XJSON_NULL:
            *plen = 5;
            *pstr = xmallocz(*plen);
            strcat(*pstr, "null");
            break;
        default:
            assert(0);
    }
    /* free stack element */
    while(!bd_xjson_stack_empty(g_chars_stk))
    {
        xfree(bd_xjson_stack_top(g_chars_stk));
        bd_xjson_stack_pop(g_chars_stk);
    }
    /* clear stack */
    bd_xjson_stack_clear(g_chars_stk);
    return l;
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

static int
bd_xjson_parse_object(const char** const pstr, bd_xjson* json)
{
    const char* str = *pstr;
    int64_t old_stk_top, tmp_stk_top;
    bd_xjson sub;

    old_stk_top = g_char_stk.top;

    EXPECT_IF_NOT(str, '{', assert(0));
    bypass_white_space(&str);
    MAYBE_AND_THEN(str, '}', *pstr = str; return 0);
    for( ; ; )
    {
        memset(&sub, 0, sizeof(sub));
        /* parse key */
        tmp_stk_top = g_char_stk.top;
        EXPECT_IF_NOT(str, '\"', goto parse_obj_err);
        /* support zero-length key string */
        // MAYBE_AND_THEN(str, '\"', goto parse_obj_err);
        for(; ; str++)
        {
            if(*str == '\"') {
                break;
            }
            else if(*str == '\0') {
                goto parse_obj_err;
            }
            bd_xjson_stack_push(g_char_stk, *str);
        }
        EXPECT_IF_NOT(str, '\"', goto parse_obj_err);
        bypass_white_space(&str);
        EXPECT_IF_NOT(str, ':', goto parse_obj_err);
        bypass_white_space(&str);
        /* parse value */
        switch(*str)
        {
            case '{':
                sub.type = BD_XJSON_OBJECT;
                assert(NULL == sub.data); /* for test */
                sub.data = htab_create(1);
                if(bd_xjson_parse_object(&str, &sub)) {
                    htab_free(sub.data);
                    goto parse_obj_err;
                }
                break;

            case '\"':
                sub.type = BD_XJSON_STRING;
                if(bd_xjson_parse_string(&str, &sub)) {
                    goto parse_obj_err;
                }
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '-':
                sub.type = BD_XJSON_NUMBER;
                if(bd_xjson_parse_number(&str, &sub)) {
                    goto parse_obj_err;
                }
                break;
            case '[':
                sub.type = BD_XJSON_ARRAY;
                assert(NULL == sub.data); /* for test */
                sub.data = list_create();
                if(bd_xjson_parse_array(&str, &sub)) {
                    list_free(sub.data);
                    goto parse_obj_err;
                }
                break;
            case 't':
                sub.type = BD_XJSON_TRUE;
                if(bd_xjson_parse_literal(&str, &sub)) {
                    goto parse_obj_err;
                }
                break;
            case 'f':
                sub.type = BD_XJSON_FALSE;
                if(bd_xjson_parse_literal(&str, &sub)) {
                    goto parse_obj_err;
                }
                break;
            case 'n':
                sub.type = BD_XJSON_NULL;
                if(bd_xjson_parse_literal(&str, &sub)) {
                    goto parse_obj_err;
                }
                break;
            default:
                goto parse_obj_err;
        }
        htab_insert_ref(json->data, &g_char_stk.data[tmp_stk_top + 1], &sub);
        bypass_white_space(&str);
        switch(*str)
        {
            case ',':
                break;
            case '}':
                MAYBE_AND_THEN(str, '}',
                    bd_xjson_stack_pop2_old_top(g_char_stk, old_stk_top);
                    *pstr = str;
                    return 0;
                );
            default:
                goto parse_obj_err;
        }
        EXPECT_IF_NOT(str, ',', assert(0));
        bypass_white_space(&str);
    }
parse_obj_err:
    *pstr = str;
    return -1;
}

static int
bd_xjson_parse_array(const char** const pstr, bd_xjson* json)
{
    const char* str = *pstr;
    bd_xjson sub;

    EXPECT_IF_NOT(str, '[', assert(0));
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
                assert(NULL == sub.data); /* for test */
                sub.data = htab_create(1);
                if(bd_xjson_parse_object(&str, &sub)) {
                    htab_free(sub.data);
                    goto parse_arr_err;
                }
                break;
            case '\"':
                sub.type = BD_XJSON_STRING;
                if(bd_xjson_parse_string(&str, &sub)) {
                    goto parse_arr_err;
                }
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '-':
                sub.type = BD_XJSON_NUMBER;
                if(bd_xjson_parse_number(&str, &sub)) {
                    goto parse_arr_err;
                }
                break;
            case '[':
                sub.type = BD_XJSON_ARRAY;
                assert(NULL == sub.data); /* for test */
                sub.data = list_create();
                if(bd_xjson_parse_array(&str, &sub)) {
                    list_free(sub.data);
                    goto parse_arr_err;
                }
                break;
            case 't':
                sub.type = BD_XJSON_TRUE;
                if(bd_xjson_parse_literal(&str, &sub)) {
                    goto parse_arr_err;
                }
                break;
            case 'f':
                sub.type = BD_XJSON_FALSE;
                if(bd_xjson_parse_literal(&str, &sub)) {
                    goto parse_arr_err;
                }
                break;
            case 'n':
                sub.type = BD_XJSON_NULL;
                if(bd_xjson_parse_literal(&str, &sub)) {
                    goto parse_arr_err;
                }
                break;
            default:
                goto parse_arr_err;
        }
        list_insert_tail(json->data, &sub);
        bypass_white_space(&str);
        switch(*str)
        {
            case ',':
                break;
            case ']':
                MAYBE_AND_THEN(str, ']', *pstr = str; return 0);
            default:
                goto parse_arr_err;
        }
        EXPECT_IF_NOT(str, ',', assert(0));
        bypass_white_space(&str);
    }
parse_arr_err:
    *pstr = str;
    return -1;
}

static int
bd_xjson_parse_string(const char ** const text, bd_xjson *json)
{
    int n, u, hu;
    char c;
    const char *s = *text;
    int64_t old_stk_top = g_char_stk.top;
    hu = 0;

    EXPECT_IF_NOT(s, '\"', assert(0));
    /* support zero-length string */
    // MAYBE_AND_THEN(str, '\"', goto parse_str_err);
    for(; ; s++)
    {
        switch(*s)
        {
            case '\\':
                s++;
                switch(*s) {
                case '\"':
                case '\\':
                case '/':
                    bd_xjson_stack_push(g_char_stk, *s);
                    break;
                case 'b':
                    bd_xjson_stack_push(g_char_stk, '\b');
                    break;
                case 'f':
                    bd_xjson_stack_push(g_char_stk, '\f');
                    break;
                case 'n':
                    bd_xjson_stack_push(g_char_stk, '\n');
                    break;
                case 'r':
                    bd_xjson_stack_push(g_char_stk, '\r');
                    break;
                case 't':
                    bd_xjson_stack_push(g_char_stk, '\t');
                    break;
                case 'u':
                    n = 0;
                    u = 0;
                    for(; n < 4; n++) {
                        s++;
                        u <<= 4;
                        switch(*s) {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            u |= (*s - '0');
                            break;
                        case 'a': case 'b': case 'c': case 'd': case 'e':
                        case 'f':
                            u |= (*s - 'a' + 0xa);
                            break;
                        case 'A': case 'B': case 'C': case 'D': case 'E':
                        case 'F':
                            u |= (*s - 'A' + 0xa);
                            break;
                        default:
                            goto parse_str_err;
                        }
                    }
                    /* utf-8: 1 byte */
                    if(!hu && u < 0x80) { /* U+0000 ~ U+007F*/
                        /* TODO: support '\u0000' */
                        c = u;
                        if(c == '\0') {
                            goto parse_str_err;
                        }
                        bd_xjson_stack_push(g_char_stk, c);
                    /* utf-8: 2~4 bytes */
                    } else if(!hu && u < 0x800) { /* U+0080 ~ U+07FF*/
                        c = 0xc0 | extract32(u, 6, 5);
                        bd_xjson_stack_push(g_char_stk, c);
                        c = 0x80 | extract32(u, 0, 6);
                        bd_xjson_stack_push(g_char_stk, c);
                    } else if(u < 0x10000) { /* U+0800 ~ U+FFFF */
                        /* case: had high surrogate */
                        if(hu) {
                            /* case: has low surrogate */
                            if(u >= 0xdc00 && u <= 0xdfff) {
                                u = 0x10000 + ((hu - 0xd800) << 10) + (u - 0xdc00);
                                c = 0xf0 | extract32(u, 18, 3);
                                bd_xjson_stack_push(g_char_stk, c);
                                c = 0x80 | extract32(u, 12, 6);
                                bd_xjson_stack_push(g_char_stk, c);
                                c = 0x80 | extract32(u, 6, 6);
                                bd_xjson_stack_push(g_char_stk, c);
                                c = 0x80 | extract32(u, 0, 6);
                                bd_xjson_stack_push(g_char_stk, c);
                                hu = 0;
                            /* default: has no low surrogate */
                            } else {
                                goto parse_str_err;
                            }
                        /* default: had no high surrogate */
                        } else {
                            /* case: has high surrogate */
                            if(u >= 0xd800 && u <= 0xdbff) {
                                hu = u;
                            /* case: has low surrogate */
                            } else if(u >= 0xdc00 && u <= 0xdfff) {
                                goto parse_str_err;
                            /* default: not surrogate pair */
                            } else {
                                c = 0xe0 | extract32(u, 12, 4);
                                bd_xjson_stack_push(g_char_stk, c);
                                c = 0x80 | extract32(u, 6, 6);
                                bd_xjson_stack_push(g_char_stk, c);
                                c = 0x80 | extract32(u, 0, 6);
                                bd_xjson_stack_push(g_char_stk, c);
                            }
                        }
                    } else {
                        goto parse_str_err;
                    }
                    break;
                default:
                    goto parse_str_err;
                }
                break;
            case '\"':
                EXPECT_IF_NOT(s, '\"', assert(0)); /* s++ */
                json->data = xmallocz(g_char_stk.top - old_stk_top + 1);
                strcat(json->data, &g_char_stk.data[old_stk_top + 1]);
                /* restore old stack top */
                bd_xjson_stack_pop2_old_top(g_char_stk, old_stk_top);
                *text = s;
                return 0;
            case '\0':
                goto parse_str_err;
            default:
                bd_xjson_stack_push(g_char_stk, *s);
        }
    }
parse_str_err:
    *text = s;
    return -1;
}

/* only support signed 32-bit integer */
static int
bd_xjson_parse_number(const char** const pstr, bd_xjson* json)
{
    const char* str = *pstr;
    int res = 0;
    int sign = *str == '-'? -1 : 1;

    MAYBE_AND_THEN(str, '-',);
    for(; ; str++)
    {
        switch(*str)
        {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                if((res == INT_MAX/10 && (*str - '0') > 7) ||
                    res > INT_MAX/10 ||
                   (res == INT_MIN/10 && (*str - '0') > 8) ||
                    res < INT_MIN/10) {
                    THROW_WARNING("integer overflow");
                    goto parse_num_err;
                }
                res = 10 * res + sign * (int)(*str - '0');
                break;
            default:
                json->data = xmallocz(sizeof(int));
                *(int*)json->data = res;
                *pstr = str;
                return 0;
        }
    }
parse_num_err:
    *pstr = str;
    return -1;
}

static int
bd_xjson_parse_literal(const char** const pstr, bd_xjson* json)
{
    char* literal;
    const char* str = *pstr;

    assert(*str == 't' || *str == 'f' || *str == 'n');
    switch(*str)
    {
        case 't':
            literal = "true";
            break;
        case 'f':
            literal = "false";
            break;
        case 'n':
            literal = "null";
            break;
    }
    for(; *literal; literal++, str++)
    {
        if(*literal != *str)
        {
            *pstr = str;
            return -1;
        }
    }
    *pstr = str;
    return 0;
}

static const char*
bd_xjson_parse_entry(const char* str, bd_xjson* json)
{
    bypass_white_space(&str);
    switch(*str)
    {
        case '{':
            assert(json->type == BD_XJSON_OBJECT);
            assert(NULL == json->data); /* for test */
            json->data = htab_create(1);
            if(bd_xjson_parse_object(&str, json)) {
                htab_free(json->data);
                return str;
            }
            break;
        case '\"':
            assert(json->type == BD_XJSON_STRING);
            if(bd_xjson_parse_string(&str, json)) {
                return str;
            }
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '-':
            assert(json->type == BD_XJSON_NUMBER);
            if(bd_xjson_parse_number(&str, json)) {
                return str;
            }
            break;
        case '[':
            assert(json->type == BD_XJSON_ARRAY);
            assert(NULL == json->data); /* for test */
            json->data = list_create();
            if(bd_xjson_parse_array(&str, json)) {
                list_free(json->data);
                return str;
            }
            break;
        case 't':
            assert(json->type == BD_XJSON_TRUE);
            if(bd_xjson_parse_literal(&str, json)) {
                return str;
            }
            break;
        case 'f':
            assert(json->type == BD_XJSON_FALSE);
            if(bd_xjson_parse_literal(&str, json)) {
                return str;
            }
            break;
        case 'n':
            assert(json->type == BD_XJSON_NULL);
            if(bd_xjson_parse_literal(&str, json)) {
                return str;
            }
            break;
        default:
            return str;
    }
    bypass_white_space(&str);
    EXPECT_IF_NOT(str, '\0', bd_xjson_free_data(json); return str);
    return NULL;
}

static void parse_fail_print(const char* str, const char* err)
{
    if(*err == '\0') {
        /* explicitly print '\0' */
        log_printf("PARSE FAIL: %.*s\\0<<<< illegal NULL-Terminator\n",
            (int)(err - str + 1), str);
    } else {
        log_printf("PARSE FAIL: %.*s<<<< illegal characters\n",
            (int)(err - str + 1), str);
    }
}

int bd_xjson_parse(const char* str, void* val)
{
    const char* err;
    bd_xjson *json = val;
    bd_xjson old = *json;
    /*
     * User behavior errors are not allowed, e.g. passing a null
     * pointer of an object into a function. Parsing is a sequential
     * process, we just assert(@json) once.
     * We must give high level warnings(quit) to user for misuse
     * of code.
     *
     * Parsing behavior errors are allowed, e.g. a illegal character
     * is encountered.
     * Return incorrect character to user while parsing failed. We
     * provide a config option CONFIG_LOG to print these warnings to
     * stderr or generate a log file in /tmp/bd_xjson.log
     *
     * But there is one exception that will also quit while parsing
     * first not whitespace valid character if you provide a bd_xjson
     * with mismatch type.
     */
    assert(json);
    /*
     * Here we get a empty bd_xjson with legal type, we think it's
     * valid while parsing a json string.
     */
    json->data = NULL;

    bd_xjson_stack_init(g_char_stk, 256);
    if(!!(err = bd_xjson_parse_entry(str, json))) {
        parse_fail_print(str, err);
        /* parse failed and restore it */
        json->data = old.data;
        bd_xjson_stack_clear(g_char_stk);
        return -1;
    }
    bd_xjson_free_data(&old);
    bd_xjson_stack_clear(g_char_stk);
    return 0;
}