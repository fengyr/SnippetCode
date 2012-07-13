/*
 * =====================================================================================
 *
 *       Filename:  base_test.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  七月 13, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>

/* 引用外部连接的变量 */
extern int external_var1;
extern external_var2;

#define test_join_func(a, b)            \
    do {                                \
        printf("a=%d, b=%d\n", a, b);   \
        printf("a##b is %d\n", a##b);   \
    } while (0)

#define test_ref_func(a)                \
    do {                                \
        printf("a = %s\n", #a);         \
    } while (0);

int main(int argc, const char *argv[])
{
    printf("------------------TEST EXTERN VAR--------------------\n");
    printf("external_var1 = %d, external_var2 = %d\n", external_var1, external_var2);

    printf("\n------------------TEST ##--------------------\n");
    test_join_func(20, 12);

    printf("\n------------------TEST #--------------------\n");
    test_ref_func("test ref func");
    
    return 0;
}
