/*
 * =====================================================================================
 *
 *       Filename:  object.h
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
#ifndef _object_H_
#define _object_H_

#ifdef __cplusplus
extern "C" {
#endif

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

void* create_object(const char* obj_name, int obj_id);
void free_object(void *obj);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _object_H_ */
