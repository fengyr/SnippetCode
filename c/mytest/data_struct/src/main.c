/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  四月 11, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "array.h"

struct object_array g_obj_array = OBJECT_ARRAY_INIT;

int main(int argc, const char *argv[])
{
    struct object *obj = malloc(sizeof(struct object));
    obj->id = 1;
    obj->name = "zenki";

    dump_object_array(&g_obj_array); 

    int i;
    for (i = 0; i < 10; i++) {
        add_object_array(obj, "hello", &g_obj_array);
    }
    dump_object_array(&g_obj_array);    

    return 0;
}
