/*
 * =====================================================================================
 *
 *       Filename:  aligned.c
 *
 *    Description:  测试字节对齐
 *
 *        Version:  
 *        Created:  十二月 20, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

/**
 * @Synopsis __attribute__属性设置字节对齐
 *          对齐的原则是：1. 结构体内按照一定的规则补齐空白字节；
 *                        2. 整个结构体需要按照规则补齐空白字节。
 */
struct aligned_4 
{
    int     a;      // 4字节
    char    b;      // 1字节
    char    c;      // 对齐：补齐空白的2字节，使整个struct aligned_4字节对齐
} __attribute__((aligned(4)));
/* 4+1+1+(2)=8 */


struct aligned_8
{
    int     a;              // 4字节
    char    b;              // 1字节
    struct  aligned_4 c;    // 对齐：该处的变量要和上面的字节对齐，因此，a+b=5，需要补齐空白的3字节
    char    d;              // 对齐：需要补齐7字节，使整个struct aligned_8对齐 
} __attribute__((aligned(8)));
/* 4+1+(3)+8+1+(7)=24 */

void test_aligned()
{
    struct aligned_4 p4;
    struct aligned_8 p8;

    printf("sizeof(int)=%d, sizeof(short)=%d, sizeof(char)=%d\n",sizeof(int),sizeof(short),sizeof(char));
    printf("sizeof(aligned_4)=%d, sizeof(aligned_8)=%d\n", sizeof(p4),sizeof(p8));
}
