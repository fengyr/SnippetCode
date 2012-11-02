/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  C和嵌入汇编
 *
 *        Version:  
 *        Created:  十一月 01, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>

/* void my_strcpy(const char *src, char *dest)
 * {
 *     int a = 10;
 * 
 *     printf("src = %s, a = %d\n", src, a);
 * } */

int add1(int a, int b)
{
    int d = a + b;
    return d;
}

int add2(int a, int *b)
{
    int d = a + *b;

    *b = 100;

    return d;
}

int main(int argc, const char *argv[])
{
    /* char *a = "forget it and move on!"; */
    /* char b[64]; */

    int bp = 10;
    int cp = 11;
    add1(bp, cp);
    add2(bp, &cp);
    /* my_strcpy(a, b); */
    /* printf("original: %s", a); */
    /* printf("copyed: %s", b); */
    return 0;
} 
