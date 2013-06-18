/*
 * =====================================================================================
 *
 *       Filename:  list.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 18, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _list_H_
#define _list_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "message.h"

typedef struct message_t Element;

typedef struct list_elmt {
    Element *data;
    struct list_elmt *next;
} ListElmt, *PListElmt;

typedef struct list_t {
    int size;
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    ListElmt *head;
    ListElmt *tail;
} List, *PList;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
void list_init(List *list, void (*destroy)(void *data));
void list_destroy(List *list);
int list_insert_next(List *list, ListElmt *element, const void *data);
int list_remove_next(List *list, ListElmt *element, void **data);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _list_H_ */
