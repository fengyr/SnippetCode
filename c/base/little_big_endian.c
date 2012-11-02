/*
 * =====================================================================================
 *
 *       Filename:  little_big_endian.c
 *
 *    Description:  测试大小端
 *
 *        Version:  
 *        Created:  十月 31, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

struct little_big_endian_t {
    unsigned int a:2;
    unsigned int b:2;
    unsigned int c:3;
    unsigned int d:1;
};

/**
 * @Synopsis 11110001 => 241, 小端模式会按照a,b,c,d的顺序从低到高存储数据。
 */
void test_little_big_endian()
{
    struct little_big_endian_t lb = {
        .a = 01,
        .b = 00,
        .c = 111,
        .d = 1
    };

    printf("lb = %d\n", *(int*)&lb); 
}
