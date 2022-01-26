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
    if(htab_create((bd_xjson_htab**)&(this->data), 1))
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
    if(htab_free(this->data))
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

void obj_add_str(bd_xjson_object* obj, const char* key, char* val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object try to add");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_STRING, .data = val};
    if(htab_insert(obj->data, key, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}

void obj_add_num(bd_xjson_object* obj, const char* key, int val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object try to add");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_NUMBER, .data = &val};
    if(htab_insert(obj->data, key, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}

void obj_add_true(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object try to add");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_TRUE, .data = NULL};
    if(htab_insert(obj->data, key, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}
void obj_add_false(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object try to add");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_FALSE, .data = NULL};
    if(htab_insert(obj->data, key, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}
void obj_add_null(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object try to add");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_NULL, .data = NULL};
    if(htab_insert(obj->data, key, &json)) /* cast to parent class */
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
void obj_set(bd_xjson_object* obj, const char* key, void* val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to set");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to set");
    }
    if(htab_set(obj->data, key, val))
    {
        THROW_EXCEPTION("set error");
    }
}

void obj_set_str(bd_xjson_object* obj, const char* key, const char* val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to set");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to set");
    }
    bd_xjson json = {.type = BD_XJSON_STRING, .data = (char*)val};
    if(htab_set(obj->data, key, &json))
    {
        THROW_EXCEPTION("set error");
    }
}

void obj_set_num(bd_xjson_object* obj, const char* key, int val)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to set");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to set");
    }
    bd_xjson json = {.type = BD_XJSON_NUMBER, .data = &val};
    if(htab_set(obj->data, key, &json))
    {
        THROW_EXCEPTION("set error");
    }
}

void obj_set_true(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to set");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to set");
    }
    bd_xjson json = {.type = BD_XJSON_TRUE, .data = NULL};
    if(htab_set(obj->data, key, &json))
    {
        THROW_EXCEPTION("set error");
    }
}

void obj_set_false(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to set");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to set");
    }
    bd_xjson json = {.type = BD_XJSON_FALSE, .data = NULL};
    if(htab_set(obj->data, key, &json))
    {
        THROW_EXCEPTION("set error");
    }
}

void obj_set_null(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to set");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to set");
    }
    bd_xjson json = {.type = BD_XJSON_NULL, .data = NULL};
    if(htab_set(obj->data, key, &json))
    {
        THROW_EXCEPTION("set error");
    }
}

void obj_get(bd_xjson_object* obj, const char* key, void* val)
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

char* obj_get_str(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to get");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to get");
    }
    bd_xjson json = {.type = BD_XJSON_STRING, .data = NULL};
    if(htab_find(obj->data, key, &json))
    {
        THROW_EXCEPTION("get error");
    }
    return json.data;
}

int obj_get_num(bd_xjson_object* obj, const char* key)
{
    if(NULL == obj)
    {
        THROW_EXCEPTION("uninitialized object class try to get");
    }
    if(NULL == obj->data)
    {
        THROW_EXCEPTION("uninitialized data of object class try to get");
    }
    bd_xjson json = {.type = BD_XJSON_NUMBER, .data = NULL};
    int num;
    if(htab_find(obj->data, key, &json))
    {
        THROW_EXCEPTION("get error");
    }
    num = *(int*)json.data;
    xfree(json.data);
    return num;
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
    if(list_free(this->data))
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

void arr_add_str(bd_xjson_array* arr, int pos, char* val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array try to add");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_STRING, .data = val};
    if(list_insert(arr->data, pos, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}

void arr_add_num(bd_xjson_array* arr, int pos, int val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array try to add");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_NUMBER, .data = &val};
    if(list_insert(arr->data, pos, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}
void arr_add_true(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array try to add");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_TRUE, .data = NULL};
    if(list_insert(arr->data, pos, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}
void arr_add_false(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array try to add");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_FALSE, .data = NULL};
    if(list_insert(arr->data, pos, &json)) /* cast to parent class */
    {
        THROW_EXCEPTION("add error");
    }
}
void arr_add_null(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array try to add");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to insert");
    }
    bd_xjson json = {.type = BD_XJSON_NULL, .data = NULL};
    if(list_insert(arr->data, pos, &json)) /* cast to parent class */
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
void arr_set(bd_xjson_array* arr, int pos, void* val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to set");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to set");
    }
    if(list_set(arr->data, pos, val))
    {
        THROW_EXCEPTION("set error");
    }
}
void arr_set_str(bd_xjson_array* arr, int pos, char* val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to set");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to set");
    }
    bd_xjson json = {.type = BD_XJSON_STRING, .data = val};
    if(list_set(arr->data, pos, &json))
    {
        THROW_EXCEPTION("set error");
    }
}
void arr_set_num(bd_xjson_array* arr, int pos, int val)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to set");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to set");
    }
    bd_xjson json = {.type = BD_XJSON_NUMBER, .data = &val};
    if(list_set(arr->data, pos, &json))
    {
        THROW_EXCEPTION("set error");
    }
}
void arr_set_true(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to set");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to set");
    }
    bd_xjson json = {.type = BD_XJSON_TRUE, .data = NULL};
    if(list_set(arr->data, pos, &json))
    {
        THROW_EXCEPTION("set error");
    }
}
void arr_set_false(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to set");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to set");
    }
    bd_xjson json = {.type = BD_XJSON_FALSE, .data = NULL};
    if(list_set(arr->data, pos, &json))
    {
        THROW_EXCEPTION("set error");
    }
}
void arr_set_null(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to set");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to set");
    }
    bd_xjson json = {.type = BD_XJSON_NULL, .data = NULL};
    if(list_set(arr->data, pos, &json))
    {
        THROW_EXCEPTION("set error");
    }
}
void arr_get(bd_xjson_array* arr, int pos, void* val)
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
char* arr_get_str(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to get");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to get");
    }
    bd_xjson json = {.type = BD_XJSON_STRING, .data = NULL};
    if(list_find(arr->data, pos, &json))
    {
        THROW_EXCEPTION("get error");
    }
    return json.data;
}
int arr_get_num(bd_xjson_array* arr, int pos)
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("unitialized array class try to get");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("unitialized data of array calss try to get");
    }
    bd_xjson json = {.type = BD_XJSON_NUMBER, .data = NULL};
    int num;
    if(list_find(arr->data, pos, &json))
    {
        THROW_EXCEPTION("get error");
    }
    num = *(int*)json.data;
    xfree(json.data);
    return num;
}
void arr_qsort(bd_xjson_array* arr, int (*compare_fn)(const void*, const void*))
{
    if(NULL == arr)
    {
        THROW_EXCEPTION("uninitialized array class try to quick sort");
    }
    if(NULL == arr->data)
    {
        THROW_EXCEPTION("uninitialized data of array class try to quick sort");
    }
    list_qsort(arr->data, compare_fn);
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
void str_assign_cstr(bd_xjson_string* this, const char* val)
{
    if(NULL == this)
    {
        THROW_EXCEPTION("uninitialized string class try to construct");
    }
    if(NULL == val)
    {
        THROW_EXCEPTION("uninitialized characters try to be the assigned");
    }
    unsigned s = strlen(val) + 1;
    this->data = xzmalloc(s);
    strcat(this->data, val);
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
void str_set(bd_xjson_string* str, char* val)
{
    if(NULL == str)
    {
        THROW_EXCEPTION("uninitialized string class try to set");
    }
    if(NULL == str->data)
    {
        THROW_EXCEPTION("uninitialized data of string class try to set");
    }
    xfree(str->data);
    str->data = xzmalloc(strlen(val) + 1);
    strcat(str->data, val);
}
char* str_get(bd_xjson_string* str)
{
    if(NULL == str)
    {
        THROW_EXCEPTION("uninitialized string class try to get");
    }
    if(NULL == str->data)
    {
        THROW_EXCEPTION("uninitialized data of string class try to get");
    }
    char* chars = xzmalloc(strlen(str->data) + 1);
    strcat(chars, str->data);
    return chars;
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
void num_set(bd_xjson_number* num, int val)
{
    if(NULL == num)
    {
        THROW_EXCEPTION("uninitialized number class try to set");
    }
    if(NULL == num->data)
    {
        THROW_EXCEPTION("uninitialized data of number class try to set");
    }
    *(int*)num->data = val;
}
int num_get(bd_xjson_number* num)
{
    if(NULL == num)
    {
        THROW_EXCEPTION("uninitialized number class try to get");
    }
    if(NULL == num->data)
    {
        THROW_EXCEPTION("uninitialized data of number class try to get");
    }
    return *(int*)num->data;
}