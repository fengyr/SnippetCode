/*
 * =====================================================================================
 *
 *       Filename:  base_test.c
 *
 *    Description:  C语言基本语法的测试示例
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
extern external_var2;       // 该形式的声明会给出warning提示，没有int类型声明

extern int array[10];       // 错误的外部引用：int *array
extern int *point;          // 错误的外部引用：int point[];

/* ## 表示连接符号，将a，b两个符号进行连接，而不进行类型的判断 */
#define test_join_func(a, b)            \
    do {                                \
        printf("a=%d, b=%d\n", a, b);   \
        printf("a##b is %d\n", a##b);   \
    } while (0)

/* # 表示引用符号，var变量被va和参数r组合而成：va##a == var */
#define test_ref_func(a)                \
    do {                                \
        int var = 100;                  \
        int va = 10;                    \
        printf("var = %d\n", va##a);         \
    } while (0);

int main(int argc, const char *argv[])
{
    printf("------------------TEST EXTERN VAR--------------------\n");
    printf("external_var1 = %d, external_var2 = %d\n", external_var1, external_var2);
    printf("array[0] = %d, *point = %d\n", array[9], *point);

    printf("\n------------------TEST ##--------------------\n");
    test_join_func(20, 12);

    printf("\n------------------TEST #--------------------\n");
    int r = 1;
    test_ref_func(r);
    
    printf("\n------------------TEST va_list --------------------\n");
    var_list_test("my name is %s, age is %d, code is %c.\n", "zenki", 30, 'Z'); 

    printf("\n------------------TEST pointer_to_address --------------------\n");
    test_pointer_to_address();

    printf("\n------------------TEST little_big_endian --------------------\n");
    test_little_big_endian();

    return 0;
}
