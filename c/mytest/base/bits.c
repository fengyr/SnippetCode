/*
 * =====================================================================================
 *
 *       Filename:  bits.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  四月 11, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void test_bits()
{
    int base = 0x00F0FF0F;
    int mask = 0xF0FF00FF;

    // <与运算>
    // 1.通过掩码可以取对应掩码为1的位。
    // 2.通过掩码可以清零对应掩码为0的位。
    int res = base & mask;
    printf("(%08x & %08x) = %08x\n", base, mask, res);

    // <或运算>
    // 1.需要对某些位置1。
    res = base | mask;
    printf("(%08x | %08x) = %08x\n", base, mask, res);

    // <取反运算>
    // 1.对指定位取反。
    res = ~base;
    printf("(~%08x) = %08x\n", base, res);

    // <异或运算>: 相同位为0，不同为1。
    // 1.通过掩码可以使对应掩码为1的位翻转。
    res = base ^ mask;
    printf("(%08x ^ %08x) = %08x\n", base, mask, res);
    res = 0xFFFFFFFF ^ base;
    printf("0xFFFFFFFF ^ %08x = %08x\n", base, res);

    // <左移运算>
    // 1.向左移动指定位数，高位丢弃，低位补零。
    // 2.相当于乘2。
    base = 0x2;
    res = base << 1;
    printf("(%08x << 1) = %08x\n", base, res);

    // <右移运算>
    // 1.向右移动指定位数，高位正数补零。
    // 2.相当于除2。
    res = base >> 1;
    printf("(%08x >> 1) = %08x\n", base, res);
}
