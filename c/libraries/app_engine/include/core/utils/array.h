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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/* 对象数组的结构体 */
/* +-----------------+            +----------------------+            +------------+
 * | <object_array>  |     +----->| <object_array_entry> |     +----->| <object>   |
 * +-----------------+     |      +----------------------+     |      +------------+
 * |  m: nr (int)    |     |      | m: item (object*)    |-----|      | m: name    |
 * +-----------------+     |      +----------------------+            +------------+
 * |  m: alloc (int) |     |                                          | m: id      |
 * +-----------------+     |                                          +------------+
 * |  m: objects  ---+-----+                             
 * +-----------------+
 *  */
typedef void* ArrayItem;

struct object_array_entry_t {
    ArrayItem item;
};
typedef struct object_array_entry_t ArrayEntry, *PArrayEntry;

struct object_array {
    unsigned int nr;
    unsigned int alloc;
    ArrayEntry *objects; 
    void (*destroy)(void *item);
};
typedef struct object_array ArrayObj, *PArrayObj;

/* 初始化对象数组结构体的宏 */
#define ARRAY_OBJ_INIT {0, 0, NULL, NULL}

void array_obj_init(ArrayObj *array, void (*destroy)(void *item));
void array_obj_add(ArrayItem item, ArrayObj *array);
void array_obj_destroy(ArrayObj *array);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _array_H_ */
