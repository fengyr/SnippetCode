/*
 * =====================================================================================
 *
 *       Filename:  hashmap.h
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

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_MISSING     -3  /* No such element */
#define MAP_FULL        -2 	/* Hashmap is full */
#define MAP_OMEM        -1 	/* Out of Memory */
#define MAP_OK          0 	/* OK */

typedef void *any_t;
typedef int (*PFany)(any_t, any_t);
typedef any_t HashMap;

extern HashMap hashmap_new();
extern int hashmap_iterate(HashMap in, PFany f, any_t item);
extern int hashmap_put(HashMap in, char* key, any_t value);
extern int hashmap_get(HashMap in, char* key, any_t *arg);
extern int hashmap_remove(HashMap in, char* key);
extern int hashmap_get_one(HashMap in, any_t *arg, int remove);
extern void hashmap_free(HashMap in);
extern int hashmap_length(HashMap in);

#ifdef __cplusplus
}
#endif

#endif
