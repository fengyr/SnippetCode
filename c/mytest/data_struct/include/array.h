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

extern struct object_array g_object_array; 

/* 自定义单个对象的结构体 */
/* +--------------+
 * |  <object>    |
 * +--------------+
 * |  m: name     |
 * +--------------+
 * |  m: id       |
 * +--------------+ */
struct object {
    char *name;
    int id;
};

/* 对象数组的结构体 */
/* +-----------------+            +----------------------+
 * | <object_array>  |     +----->| <object_array_entry> |
 * +-----------------+     |      +----------------------+
 * |  m: nr          |     |      | m: item              |
 * +-----------------+     |      +----------------------+
 * |  m: alloc       |     |      | m: array_name        |
 * +-----------------+     |      +----------------------+
 * |  m: objects  ---+-----+                             
 * +-----------------+
 *  */
struct object_array {
    unsigned int nr;
    unsigned int alloc;
    struct object_array_entry {
        struct object *item;
        char *array_name;
    } *objects;
};

/* 初始化对象数组结构体的宏 */
#define OBJECT_ARRAY_INIT {0, 0, NULL}

void* create_object(const char *obj_name, int obj_id);
void add_object_array(struct object *obj, const char *array_name, struct object_array *array);
void dump_object_array(struct object_array *array);
void free_object_array(struct object_array *array);

#endif /* end of include guard: _array_H_ */
