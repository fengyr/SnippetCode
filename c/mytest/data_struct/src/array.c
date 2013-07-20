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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array.h"

void array_obj_init(ArrayObj *array, void (*destroy)(void *item))
{
    array->nr = 0;
    array->alloc = 0;
    array->objects = NULL;
    array->destroy = destroy;
}

/**
 * @Synopsis 添加一个item对象到array对象数组中，并指定标识该item对象的名称。
 *           如果数组中的数量超过数组大小，会自动扩展数组的大小。
 *
 * @Param item object对象.
 * @Param array object对象数组.
 */
void array_obj_add(ArrayItem item, ArrayObj *array)
{
    unsigned int nr = array->nr;
    unsigned int alloc = array->alloc;
    ArrayEntry *objects = array->objects;

    if (nr >= alloc) {
        alloc = (alloc + 32) * 2;
        objects = (ArrayEntry*) realloc(objects, alloc * sizeof(*objects));
        array->alloc = alloc;
        array->objects = objects;
    }

    /* printf("sizeof(objects)=%d\n", sizeof(*objects)); */

    objects[nr].item = item;

    /* 添加object计数 */
    array->nr = ++nr;
}

void array_obj_destroy(ArrayObj *array)
{
    unsigned int *nr = &array->nr;
    ArrayEntry *objects = array->objects;

    do {
        (*nr)--;
        if (objects[*nr].item != NULL) {
            /* printf("free array_name, objects[*nr].item = %p\n", objects[*nr].item); */
            if (array->destroy != NULL) {
                array->destroy(objects[*nr].item);
            }
        }
    } while (*nr > 0);

    /* printf("after free_object_array, nr = %d\n", *nr); */

    array->nr = *nr;
    array->alloc = *nr;
    free(objects);
    objects = NULL;
    /* printf("leave free_object_array, nr = %d\n", *nr); */
}
