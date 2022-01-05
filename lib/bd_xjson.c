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
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized object class try to construct");
    }
    if(htab_create((bd_xjson_htab**)&(this->data), 4))
    {
        THROW_EXCEPTION("constructor error");
    }

}
void obj_copy_cstr(bd_xjson_object* this, bd_xjson_object* obj)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized object class try to construct");
    }
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to be the copied");
    }
    /* create */
    if(htab_create((bd_xjson_htab**)&(this->data),
        ((bd_xjson_htab*)(obj->data))->capacity))
    {
        THROW_EXCEPTION("create constructor error");
    }
    /* copy */
    if(htab_copy(this->data, obj->data))
    {
        THROW_EXCEPTION("copy constructor error");
    }

}
void obj_default_dstr(bd_xjson_object* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized object class try to desctruct");
    }
    if(NULL == this->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to clear");
    }
    if(htab_free((bd_xjson_htab**)&(this->data)))
    {
        THROW_EXCEPTION("destructor error");
    }
}

void obj_add(bd_xjson_object* obj, const char* key, void* val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object try to add");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to insert");
    }
    if(htab_insert(obj->data, key, val)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}
void obj_delete(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to delete");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to erase");
    }
    if(htab_erase(obj->data, key))
    {
        THROW_EXCEPTION("delete error");
    }
}
void obj_search(bd_xjson_object* obj, const char* key, void* val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to search");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to find");
    }
    if(htab_find(obj->data, key, val))
    {
        THROW_EXCEPTION("search error");
    }
}
void obj_update(bd_xjson_object* obj, const char* key, void* val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to update");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to update");
    }
    if(htab_update(obj->data, key, val))
    {
        THROW_EXCEPTION("update error");
    }
}

/* constructor */
void arr_default_cstr(bd_xjson_array* this)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized array class try to construct");
    }
    if(list_create((bd_xjson_list**)&(this->data)))
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
    /* create */
    if(list_create((bd_xjson_list**)&(this->data)))
    {
        THROW_EXCEPTION("create constructor error");
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
    if(list_free((bd_xjson_list**)&(this->data)))
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
    if(list_insert(arr->data, pos, val)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}

void arr_delete(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array class try to delete");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to remove");
    }
    if(list_erase(arr->data, pos))
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
    if(list_find(arr->data, pos, val))
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
    if(list_update(arr->data, pos, val))
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
    this->data = xzmalloc(1);
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
    if(NULL == str->data)
    {
        THROW_EXCEPTION("uninitialized data of string class try to be the copied");
    }
    unsigned s = strlen(str->data) + 1;
    this->data = xzmalloc(s);
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
        THROW_EXCEPTION("uninitialized characters try to be the assigned");
    }
    unsigned s = strlen(chars) + 1;
    this->data = xzmalloc(s);
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
    this->data = xzmalloc(sizeof(int));
    *(int*)(this->data) = 0;
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
    this->data = xzmalloc(sizeof (int));
    *(int*)this->data = *(int*)num->data;
}
void num_assign_cstr(bd_xjson_number* this, int val)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized number class try to construct");
    }
    this->data = xzmalloc(sizeof (int));
    *(int*)this->data = val;
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
