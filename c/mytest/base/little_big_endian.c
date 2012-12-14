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
#include <endian.h>

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

void test_little_big_endian2()
{
    static const int v = 1;
    char *endian = *(char*)&v == 1 ? "this is little endian" : "this is big endian";

    printf("%s\n", endian);
}

void test_little_big_endian3()
{
#if BYTE_ORDER == LITTLE_ENDIAN
    printf("this is little endian\n");
#endif

#if BYTE_ORDER == BIG_ENDIAN
    printf("this is big endian\n");
#endif

#if BYTE_ORDER == PDP_ENDIAN
    printf("this is pdp endian\n");
#endif
}
