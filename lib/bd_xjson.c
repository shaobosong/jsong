#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "lib/bd_xjson.h"
#include "lib/bd_xjson_list.h"
#include "lib/bd_xjson_htab.h"
#include "lib/alloc.h"
#include "lib/error.h"

void obj_default_cstr(bd_xjson_object* this)
{

}
void obj_copy_cstr(bd_xjson_object* this, bd_xjson_object* obj)
{

}
void obj_default_dstr(bd_xjson_object* this)
{

}

void obj_add(bd_xjson_object* this, const char* key, void* val)
{

}
void obj_delete(bd_xjson_object* this, const char* key)
{

}
void obj_search(bd_xjson_object* this, const char* key, void* val)
{

}
void obj_update(bd_xjson_object* this, const char* key, void* val)
{

}

/* constructor */
void arr_default_cstr(bd_xjson_array* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized array class try to construct");
    }
    if(list_init((bd_xjson_list**)&(this->data.data)))
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
        THROW_EXCEPTION("uninitialized array class try to be the copied");
    }
    /* init */
    if(list_init((bd_xjson_list**)&(this->data.data)))
    {
        THROW_EXCEPTION("constructor initialization error");
    }
    /* copy */
    if(list_copy(this->data.data, arr->data.data))
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
    if(NULL == this->data.data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to clear");
    }
    if(list_clear((bd_xjson_list**)&(this->data.data)))
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
    if(NULL == arr->data.data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    if(list_insert(arr->data.data, pos, val)) /* cast to parent class */
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
    if(NULL == arr->data.data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    if(list_remove(arr->data.data, pos))
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
    if(NULL == arr->data.data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to find");
    }
    if(list_find(arr->data.data, pos, val))
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
    if(NULL == arr->data.data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to update");
    }
    if(list_update(arr->data.data, pos, val))
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
    this->data.data = xzmalloc(1);
}
void str_copy_cstr(bd_xjson_string* this, bd_xjson_string* str)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to construct");
    }
    if(NULL == str)
    {
        THROW_EXCEPTION("uninitialized string class try to be the copied");
    }
    if(NULL == str->data.data)
    {
        THROW_EXCEPTION("uninitialized data of string class try to be the copied");
    }
    unsigned s = strlen(str->data.data) + 1;
    this->data.data = xzmalloc(s);
    strcat(this->data.data, str->data.data);
}
void str_assign_cstr(bd_xjson_string* this, const char* chars)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to construct");
    }
    if(NULL == chars)
    {
        THROW_EXCEPTION("uninitialized characters try to be the assigned");
    }
    unsigned s = strlen(chars) + 1;
    this->data.data = xzmalloc(s);
    strcat(this->data.data, chars);
}
void str_default_dstr(bd_xjson_string* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to desctruct");
    }
    if(NULL == this->data.data)
    {
        THROW_EXCEPTION("uninitialized data of string class try to desctruct");
    }
    xfree(this->data.data);
}

void num_default_cstr(bd_xjson_number* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to construct");
    }
    this->data.data = xzmalloc(sizeof(int));
    *(int*)(this->data.data) = 0;
}
void num_copy_cstr(bd_xjson_number* this, bd_xjson_number* num)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to construct");
    }
    if(NULL == num)
    {
        THROW_EXCEPTION("uninitialized number class try to be the copied");
    }
    this->data.data = xzmalloc(sizeof (int));
    *(int*)this->data.data = *(int*)num->data.data;
}
void num_assign_cstr(bd_xjson_number* this, int val)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to construct");
    }
    this->data.data = xzmalloc(sizeof (int));
    *(int*)this->data.data = val;
}
void num_default_dstr(bd_xjson_number* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to desctruct");
    }
    if(NULL == this->data.data)
    {
        THROW_EXCEPTION("uninitialized data of number class try to desctruct");
    }
    xfree(this->data.data);
}
