/*
 * =====================================================================================
 *
 *       Filename:  array.h
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

#ifndef _array_H_
#define _array_H_

#include <stdlib.h>


/* 自定义单个对象的结构体 */
struct object {
    char *name;
    int id;
};

/* 对象数组的结构体 */
struct object_array {
    unsigned int nr;
    unsigned int alloc;
    struct object_array_entry {
        struct object *item
    } *objects;
};

#define OBJECT_ARRAY_INIT {0, 0, NULL}


void add_object_array(struct object *obj, const char *name, struct object_array *array);
void dump_object_array(struct object_array *array);

#endif /* end of include guard: _array_H_ */
