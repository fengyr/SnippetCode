/*
 * =====================================================================================
 *
 *       Filename:  array.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  四月 08, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "array.h"
#include <stdlib.h>
#include <stdio.h>

struct object_array g_object_array = OBJECT_ARRAY_INIT;

/**
 * @Synopsis 创建一个object对象，该对象通过malloc动态分配内存
 *
 * @Param obj_name object的名字
 * @Param obj_id   object的id
 *
 * @Returns        object对象的指针
 */
void* create_object(const char* obj_name, int obj_id)
{
    struct object *obj;
    int len = strlen(obj_name) + 1;

    obj = (struct object*) malloc(sizeof(*obj));
    obj->name = (char*) malloc(sizeof(char) * len);
    strcpy(obj->name, obj_name);
    obj->id = obj_id;

    return obj;
}

static void free_object(struct object *obj)
{
    if (!obj) {
        return;
    }

    /* printf("free_object, obj = %p, obj->name = %s, obj->id = %p\n", obj, obj->name, &obj->id); */
    if (obj->name != NULL) {
        free(obj->name);
        obj->name = NULL;
    }

    free(obj);
    obj = NULL;
}

/**
 * @Synopsis 添加一个obj对象到array对象数组中，并指定标识该obj对象的名称。
 *           如果数组中的数量超过数组大小，会自动扩展数组的大小。
 *
 * @Param obj object对象.
 * @Param array_name obj对象在array数组中的名称.
 * @Param array object对象数组.
 */
void add_object_array(struct object *obj, const char *array_name , struct object_array *array)
{
    unsigned int nr = array->nr;
    unsigned int alloc = array->alloc;
    struct object_array_entry *objects = array->objects;

    if (nr >= alloc) {
        alloc = (alloc + 32) * 2;
        objects = (struct object_array_entry*) realloc(objects, alloc * sizeof(*objects));
        array->alloc = alloc;
        array->objects = objects;
    }

    /* printf("sizeof(objects)=%d\n", sizeof(*objects)); */

    objects[nr].item = obj;

    int len = strlen(array_name) + 1;
    objects[nr].array_name = (char*) malloc(sizeof(char) * len);
    strcpy(objects[nr].array_name, array_name);

    /* 添加object计数 */
    array->nr = ++nr;
}

/**
 * @Synopsis 打印array指向的对象数组的信息.
 *
 * @Param array
 */
void dump_object_array(struct object_array *array)
{
    struct object_array_entry *objects = array->objects; 
    struct object *obj;
    int i = 0;

    printf("nr=%d, alloc=%d\n", array->nr, array->alloc);
    for (i = 0; i < array->nr; i++) {
        obj = objects[i].item;
        printf("array_name=%s, object id=%d, name=%s\n", objects[i].array_name, obj->id, obj->name);
    }
}

void free_object_array(struct object_array *array)
{
    int *nr = &array->nr;
    struct object_array_entry *objects = array->objects;
    char *array_name;

    printf("entry free_object_array, nr = %d\n", *nr);
    do {
        (*nr)--;
        if (objects[*nr].array_name != NULL) {
            /* printf("free array_name, objects[*nr].array_name = %p, nr = %d\n", objects[*nr].array_name, *nr); */
            free(objects[*nr].array_name);
        }
        if (objects[*nr].item != NULL) {
            /* printf("free array_name, objects[*nr].item = %p\n", objects[*nr].item); */
            free_object(objects[*nr].item);
        }
    } while (*nr > 0);

    array->nr = *nr;
    array->alloc = *nr;
    objects = NULL;
    /* printf("leave free_object_array, nr = %d\n", *nr); */
}
