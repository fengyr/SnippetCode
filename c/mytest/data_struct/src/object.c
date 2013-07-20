/*
 * =====================================================================================
 *
 *       Filename:  object.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  四月 30, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "object.h"

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

/**
 * @Synopsis 释放obj对象所分配的资源
 *
 * @Param obj 待释放的obj对象
 */
void free_object(void *object)
{
    struct object *obj = (struct object*)object;

    if (!obj) {
        return;
    }

    /* printf("free_object, obj = %p, obj->name = %s, obj->id = %d\n", obj, obj->name, obj->id); */
    if (obj->name != NULL) {
        free(obj->name);
        obj->name = NULL;
    }

    free(obj);
    obj = NULL;
}
