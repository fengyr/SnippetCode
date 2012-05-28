/*
 * =====================================================================================
 *
 *       Filename:  a.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年01月19日 11时00分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#pragma pack(1)*/

struct type1 {
    int d;
    double a;
    char b;
    char c;
};

void main()
{
    int *p;

    printf("size: %d addr: %p\n", sizeof(p), p);

    p = malloc(sizeof(int)*4);
    *p = 10;

    printf("size: %d addr: %p value: %d\n", sizeof(p), p, *p);
    p++;
    printf("size: %d addr: %p value: %d\n", sizeof(p), p, *p);

    int val[] = {100, 101, 102};
    char *valp;

    valp = (char*)val;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));
    valp++;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));
    valp++;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));
    valp++;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));
    valp++;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));
    valp++;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));
    valp++;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));
    valp++;
    printf("val %d,%p valp %c,%p valp++ %d\n", *val, val, *valp, valp, *(valp));


    int array[] = {1, 2, 3, 4, 5, 6, 7};
    char *str = "hello world";
    printf("size: %d\n", sizeof(array));
    printf("%c, %s\n", *str, str);
    str++;
    printf("%c, %s\n", *str, str);

    char *str2 = "hello world";
    char str3[] = {'a', '\0', 'c'};
    printf("str2: %s, str3: %s\n", str2, str3);
    printf("str2p: %p, str3p: %p\n", str2, str3);
    printf("str2s: %d, str3s: %d\n", sizeof(str2), sizeof(str3));
    printf("str2l: %d, str3l: %d\n", strlen(str2), strlen(str3));
    str3[0] = 'z';
    printf("str2l: %c, str3l: %c\n", *(str2+4), str3[0]);

    struct type1 s;
    double *d;
    float e;
    printf("type1: %d, double: %d, float: %d\n", sizeof(s), sizeof(d), sizeof(e));
}

