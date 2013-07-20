/*
 * =====================================================================================
 *
 *       Filename:  hashtable.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  七月 20, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _hashtable_H_
#define _hashtable_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "list.h"

typedef struct hash_table_t {
    int buckets;

    unsigned int (*h)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    int size;
    List *table;
} HashTable, *PHashTable;

int hashtbl_init(HashTable *htbl, int buckets, 
                 unsigned int (*h)(const void *key), 
                 int (*match)(const void *key1, const void *key2),
                 void (*destroy)(void *data));

void hashtbl_destroy(HashTable *htbl);
int hashtbl_insert(HashTable *htbl, const void *data);
int hashtbl_remove(HashTable *htbl, void **data);
int hashtbl_lookup(const HashTable *htbl, void **data);

#define hashtbl_size(htbl) ((htbl)->size)

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _hashtable_H_ */
