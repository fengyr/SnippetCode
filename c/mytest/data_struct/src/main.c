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

void test_object_array(struct object_array *obj_array, int size)
{
    /* 添加数据前，打印数组信息 */
    dump_object_array(obj_array); 

    int i;
    const char obj_name[16];
    char array_name[16];

    for (i = 0; i < size; i++) {
        sprintf(array_name, "array:%d", i);
        sprintf(obj_name, "object:%d", i);
        struct object *obj = create_object(obj_name, i);

        add_object_array(obj, array_name, obj_array);
    }
    /* 添加数据后，打印数组信息 */
    dump_object_array(obj_array);    

    /* 释放数组内存,打印数组信息 */
    free_object_array(obj_array);
    dump_object_array(obj_array);
}

int main(int argc, const char *argv[])
{
    struct object_array *obj_array = &g_object_array;

    /* 测试object数组 */
    test_object_array(obj_array, 100000);
    test_object_array(obj_array, 10000);
    test_object_array(obj_array, 10);
    test_object_array(obj_array, 100);
    test_object_array(obj_array, 1000);
    

    return 0;
}
