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
    const char *obj_name = "zenki";

    /* 添加数据前，打印数组信息 */
    dump_object_array(&g_obj_array); 
    int i;
    char array_name[16];
    for (i = 0; i < 1000; i++) {
        sprintf(array_name, "array:%d", i);
        struct object *obj = create_object(obj_name, i);

        add_object_array(obj, array_name, &g_obj_array);
    }
    /* 添加数据后，打印数组信息 */
    dump_object_array(&g_obj_array);    

    return 0;
}
