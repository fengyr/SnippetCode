/*
 * =====================================================================================
 *
 *       Filename:  hashtable.c
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
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

int hashtbl_init(HashTable *htbl, int buckets, 
                 unsigned int (*h)(const void *key), 
                 int (*match)(const void *key1, const void *key2),
                 void (*destroy)(void *data))
{
    int i;

    // 分配hash table的内存空间
    htbl->table = (List*) malloc(buckets * sizeof(List));
    if (NULL == htbl->table) {
        return -1;
    }

    htbl->buckets = buckets;
    for (i = 0; i < htbl->buckets; i++) {
        list_init(&htbl->table[i], destroy); 
    }

    htbl->h = h;
    htbl->match = match;
    htbl->destroy = destroy;

    htbl->size = 0;

    return 0;
}

void hashtbl_destroy(HashTable *htbl)
{
    int i;

    for (i = 0; i < htbl->buckets; i++) {
        list_destroy(&htbl->table[i]);
    }

    free(htbl->table);

    memset(htbl, 0, sizeof(htbl));

    return;
}

int hashtbl_insert(HashTable *htbl, const void *data)
{
    void *temp;
    int bucket, retval;

    temp = (void*)data;
    if (hashtbl_lookup(htbl, &temp) == 0) {
        return 1;
    }

    bucket = htbl->h(data) % htbl->buckets;

    retval = list_insert_next(&htbl->table[bucket], NULL, data);
    if (retval == 0) {
        htbl->size++;
    }

    return retval;
}

int hashtbl_remove(HashTable *htbl, void **data)
{
    ListElmt *element, *prev;
    int bucket;

    bucket = htbl->h(*data) % htbl->buckets;
    prev = NULL;

    for (element = list_head(&htbl->table[bucket]); 
         element != NULL; 
         element = list_next(element)) {
        if (htbl->match(*data, list_data(element))) {
            if (list_remove_next(&htbl->table[bucket], prev, data) == 0) {
                htbl->size--;
                return 0;
            } else {
                return -1;
            }
        }
        prev = element;
    }

    return -1;
}

int hashtbl_lookup(const HashTable *htbl, void **data)
{
    ListElmt *element;
    int bucket;

    bucket = htbl->h(*data) % htbl->buckets;

    for (element = list_head(&htbl->table[bucket]); 
         element != NULL; 
         element = list_next(element)) {
        if (htbl->match(*data, list_data(element))) {
            *data = list_data(element);
            return 0;
        }
    }

    return -1;
}
