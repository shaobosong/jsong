#ifndef lib_bd_xjson_h
#define lib_bd_xjson_h

#include <stddef.h>

typedef struct bd_xjson_object bd_xjson_object;
typedef struct bd_xjson_string bd_xjson_string;
typedef struct bd_xjson_number bd_xjson_number;
typedef struct bd_xjson_array bd_xjson_array;
typedef struct bd_xjson_true bd_xjson_true;
typedef struct bd_xjson_false bd_xjson_false;
typedef struct bd_xjson_null bd_xjson_null;

typedef void (*bd_xjson_true_default_cstr)(bd_xjson_true*);

typedef enum bd_xjson_type bd_xjson_type;
typedef struct bd_xjson_object_item bd_xjson_object_item;
typedef struct bd_xjson_hashtable bd_xjson_hashtable;

typedef struct bd_xjson_node bd_xjson_node;
typedef struct bd_xjson_list bd_xjson_list;

void* xmalloc(int size);
void* xrealloc(void* ptr, int size);
void xfree(void* ptr);

enum bd_xjson_type
{
    BD_XJSON_OBJECT,
    BD_XJSON_STRING,
    BD_XJSON_NUMBER,
    BD_XJSON_ARRAY,
    BD_XJSON_TRUE,
    BD_XJSON_FALSE,
    BD_XJSON_NULL
};

#define BD_XJSON(type, p)        \
    type* p = xmalloc(sizeof *p)
#define FREE_BD_XJSON(p)                           \
    switch(p->type)                                \
    {                                              \
        case BD_XJSON_OBJECT:                      \
        case BD_XJSON_STRING:                      \
            str_default_dstr((bd_xjson_string*)p); \
            break;                                 \
        case BD_XJSON_NUMBER:                      \
            num_default_dstr((bd_xjson_number*)p); \
            break;                                 \
        case BD_XJSON_ARRAY:                       \
            arr_default_dstr((bd_xjson_array*)p);  \
            break;                                 \
        default:;                                  \
    }                                              \
    xfree(p);                                      \
    p = NULL

struct bd_xjson_node
{
    void* value; /* bd_xjson or bd_xjson->data */
    bd_xjson_type type;
    bd_xjson_node *next;
    bd_xjson_node *prev;
};
int node_free(bd_xjson_node* node);
struct bd_xjson_list
{
    bd_xjson_node* head, * tail;
    int size;
};
int list_init(bd_xjson_list** list);
int list_copy(bd_xjson_list* list, bd_xjson_list* copy);
int list_insert(bd_xjson_list* list, bd_xjson_type type, int pos, void* val);
int list_remove(bd_xjson_list* list, int pos);
int list_clear(bd_xjson_list** list);
int list_find(bd_xjson_list* list, bd_xjson_type type, int pos, void* val);
int list_update(bd_xjson_list* list, bd_xjson_type type, int pos, void* val);

struct bd_xjson_hashtable
{};

/* private */
/* constructor */
void obj_default_cstr(bd_xjson_string* this);
void obj_copy_cstr(bd_xjson_string* this, bd_xjson_string* str);
/* destructor */
void obj_default_dstr(bd_xjson_string* this);
struct bd_xjson_object
{
    bd_xjson_type type;
    bd_xjson_hashtable* data;
/* public */
    /* member functions */
    /* 1. parameter memory allocated is maintained by user himself */
    /* 2. deep copy from parameter inside member functions, such as 'obj' etc. */
    void (*add)(bd_xjson_object* this, const char* key, void* val);
    void (*delete)(bd_xjson_object* this, const char* key);
    void (*search)(bd_xjson_object* this, const char* key, void* val);
    void (*update)(bd_xjson_object* this, const char* key, void* val);
};

/* private */
/* constructor */
void str_default_cstr(bd_xjson_string* this);
void str_copy_cstr(bd_xjson_string* this, bd_xjson_string* str);
void str_assign_cstr(bd_xjson_string* this, const char* chars);
/* destructor */
void str_default_dstr(bd_xjson_string* this);
struct bd_xjson_string
{
    bd_xjson_type type;
    char* data;
};
#define BD_XJSON_STRING_CLASS(p)        \
    BD_XJSON(bd_xjson_string, p);       \
    p->type = BD_XJSON_STRING;          \
    p->data = NULL
/* public */
#define BD_XJSON_STRING(p)              \
    BD_XJSON_STRING_CLASS(p);           \
    str_default_cstr(p)
#define BD_XJSON_STRING_COPY(p,c)       \
    BD_XJSON_STRING_CLASS(p);           \
    str_copy_cstr(p,c)
#define BD_XJSON_STRING_ASSIGN(p,c)     \
    BD_XJSON_STRING_CLASS(p);           \
    str_assign_cstr(p,c)


/* private */
/* constructor */
void num_default_cstr(bd_xjson_number* this);
void num_copy_cstr(bd_xjson_number* this, bd_xjson_number* num);
void num_assign_cstr(bd_xjson_number* this, int val);
/* destructor */
void num_default_dstr(bd_xjson_number* this);
struct bd_xjson_number
{
/* private */
    bd_xjson_type type;
    int* data;
};
#define BD_XJSON_NUMBER_CLASS(p)        \
    BD_XJSON(bd_xjson_number, p);       \
    p->type = BD_XJSON_NUMBER;          \
    p->data = NULL
/* public */
#define BD_XJSON_NUMBER(p)              \
    BD_XJSON_NUMBER_CLASS(p);           \
    num_default_cstr(p)
#define BD_XJSON_NUMBER_COPY(p,c)       \
    BD_XJSON_NUMBER_CLASS(p);           \
    num_copy_cstr(p,c)
#define BD_XJSON_NUMBER_ASSIGN(p,c)     \
    BD_XJSON_NUMBER_CLASS(p);           \
    num_assign_cstr(p,c)

/* private */
/* constructor */
void arr_default_cstr(bd_xjson_array* this);
void arr_copy_cstr(bd_xjson_array* this, bd_xjson_array* arr);
/* desctrctor */
void arr_default_dstr(bd_xjson_array* this);
/* member functions */
void arr_add(bd_xjson_array* arr, int pos, void* val);
void arr_delete(bd_xjson_array* arr, int pos);
void arr_search(bd_xjson_array* arr, int pos, void* val);
void arr_update(bd_xjson_array* arr, int pos, void* val);
struct bd_xjson_array
{
/* private */
    bd_xjson_type type;
    bd_xjson_list* data; /* a linked list */
/* public */
    /* member functions */
    void (*add)(bd_xjson_array* this, int pos, void* val);
    void (*delete)(bd_xjson_array* this, int pos);
    void (*search)(bd_xjson_array* this, int pos, void* val);
    void (*update)(bd_xjson_array* this, int pos, void* val);
};
/* private */
#define BD_XJSON_ARRAY_CLASS(p)         \
    BD_XJSON(bd_xjson_array, p);        \
    p->type = BD_XJSON_ARRAY;           \
    p->data = NULL;                     \
    p->add = arr_add;                   \
    p->delete = arr_delete;             \
    p->search = arr_search;             \
    p->update = arr_update
/* public */
#define BD_XJSON_ARRAY(p)               \
    BD_XJSON_ARRAY_CLASS(p);            \
    arr_default_cstr(p)
#define BD_XJSON_ARRAY_COPY(p,c)        \
    BD_XJSON_ARRAY_CLASS(p);            \
    arr_copy_cstr(p,c)


/* private */
struct bd_xjson_true
{
    bd_xjson_type type;
    void* data;
};
#define BD_XJSON_TRUE_CLASS(p)      \
    BD_XJSON(bd_xjson_true, p);     \
    P->type = BD_XJSON_TRUE;        \
    p->data = NULL
/* public api */
#define BD_XJSON_TRUE(p)            \
    BD_XJSON_TRUE_CLASS(p)

/* private */
struct bd_xjson_false
{
    bd_xjson_type type;
    void* data;
};
#define BD_XJSON_FALSE_CLASS(p)      \
    BD_XJSON(bd_xjson_false, p);     \
    p->type = BD_XJSON_FALSE;        \
    p->data = NULL
/* public */
#define BD_XJSON_FALSE(p)            \
    BD_XJSON_FALSE_CLASS(p)

/* private */
struct bd_xjson_null
{
    bd_xjson_type type;
    void* data;
};
#define BD_XJSON_NULL_CLASS(p)      \
    BD_XJSON(bd_xjson_null, p);     \
    P->type = BD_XJSON_NULL;        \
    p->data = NULL
/* public */
#define BD_XJSON_NULL(p)            \
    BD_XJSON_NULL_CLASS(p)

#endif
