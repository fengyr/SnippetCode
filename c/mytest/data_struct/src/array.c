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

void add_object_array(struct object *obj, const char *name , struct object_array *array)
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

    printf("sizeof(objects)=%d\n", sizeof(*objects));

    objects[nr].item = obj;
    array->nr = ++nr;
}

void dump_object_array(struct object_array *array)
{
    struct object_array_entry *objects = array->objects; 
    struct object *obj;
    int i = 0;

    printf("nr=%d, alloc=%d\n", array->nr, array->alloc);
    for (i = 0; i < array->nr; i++) {
        obj = objects[i].item;
        printf("object id=%d, name=%s\n", obj->id, obj->name);
    }
}
