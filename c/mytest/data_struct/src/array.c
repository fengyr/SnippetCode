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
    obj->name = malloc(sizeof(char) * len);
    strcpy(obj->name, obj_name);
    obj->id = obj_id;

    return obj;
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
        objects = realloc(objects, alloc * sizeof(*objects));
        array->alloc = alloc;
        array->objects = objects;
    }

    /* printf("sizeof(objects)=%d\n", sizeof(*objects)); */

    objects[nr].item = obj;

    int len = strlen(array_name) + 1;
    objects[nr].array_name = malloc(sizeof(char) * len);
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
