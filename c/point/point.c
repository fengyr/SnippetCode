/*
 * =====================================================================================
 *
 *       Filename:  point.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  七月 12, 2012
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
#include <string.h>

/*#pragma pack(1)*/

struct myclass {
    double a;
    char b;
    char c;
    int d;
};

int main(int argc, const char *argv[])
{
    printf("\n-------------------TEST VALUE-------------------\n");
    /* 比较char、int、long、double变量的大小 */

    char char_v = 'a';
    int  int_v = 10;
    long long_v = 10000L;
    double double_v = 1.9999f;

    printf("char size: %d addr: %p value: %c\n", sizeof(char_v), &char_v, char_v);
    printf("int size: %d addr: %p value: %d\n", sizeof(int_v), &int_v, int_v);
    printf("long size: %d addr: %p value: %d\n", sizeof(long_v), &long_v, long_v);
    printf("double size: %d addr: %p value: %f\n", sizeof(double_v), &double_v, double_v);

    printf("\n-------------------TEST POINT-------------------\n");
    /* 比较char*、int*、long*、double*变量的大小 */

    char    *char_p;
    int     *int_p;
    long    *long_p;
    double  *double_p;

    char_p = malloc(sizeof(char));
    *char_p = 'a';
    int_p = malloc(sizeof(int));
    *int_p = 10;
    long_p = malloc(sizeof(long));
    *long_p = 10000L;
    double_p = malloc(sizeof(double));
    *double_p = 1.9999f;

    printf("char* size: %d addr: %p value: %c\n", sizeof(char_p), char_p, *char_p);
    printf("int* size: %d addr: %p value: %d\n", sizeof(int_p), int_p, *int_p);
    printf("long* size: %d addr: %p value: %d\n", sizeof(long_p), long_p, *long_p);
    printf("double* size: %d addr: %p value: %f\n", sizeof(double_p), double_p, *double_p);

    printf("\n-------------------TEST INT ARRAY-------------------\n");
    /* 指针的偏移量大小取决于该指针的类型，本例中的数组为int型，但是
     * val_char_p指针类型为char*，因此val_char_p++后的偏移量为1，即一个
     * char大小 */

    int val[] = {99, 100, 102};
    char *val_char_p;
    int i;

    val_char_p = (char*)val;
    printf("val={99, 100, 102}, val_char_p is char*\n");
    for (i = 0; i < 12; i++) {
        printf("val=%d, val addr=%p, val_char_p=%c, val_char_p addr=%d, *val_char_p=%d\n", *val, val, *val_char_p, val_char_p, *val_char_p);
        val_char_p++;
    }

    printf("\n-------------------TEST STRING-------------------\n");
    /* 该测试说明字符串输出时是以'\0'为结尾，因此在使用各种字符处理函数时
     * 需要注意这个特性。 */

    char *str2 = "hello world";
    char str3[] = {'a', 'b', '\0', 'c'};
    printf("str2 = %s, str3 = {'a', 'b', '\\0', 'c'}\n", str2);
    printf("str2 addr: %p, str3 addr: %p\n", str2, str3);
    printf("str2 size: %d, str3 size: %d\n", sizeof(str2), sizeof(str3));
    printf("str2 len: %d, str3 len: %d\n", strlen(str2), strlen(str3));
    str3[0] = 'z';
    printf("*(str2+4): %c, str3[0]: %c\n", *(str2+4), str3[0]);

    printf("\n-------------------TEST STRUCT-------------------\n");
    /* 该测试强制转换一个结构体的变量的地址到一个int型，然后再将其强制
     * 转换成该结构体的指针，当引用这个指针时，可以还原这个结构体的原始
     * 状态。 */

    struct myclass class = {1.0, 2, 3, 4};
    struct myclass *class_p;
    int     context = (int)&class;
    class_p = (struct myclass*)context;

    printf("myclass size: %d, addr: %p, context=%d, class_p=%p\n", sizeof(class), &class, context, class_p);
    printf("class_p dump a=%f, b=%d, c=%d, d=%d\n", class_p->a, class_p->b, class_p->c, class_p->d);

    return 0;
}
