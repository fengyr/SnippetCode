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

void create_myclass(struct myclass **myclass_p)
{
    /* 动态分配一个myclass结构对象 */
    struct myclass *temp = (struct myclass*) malloc(sizeof(struct myclass));
    temp->a = 2.0;
    temp->b = 'a';
    temp->c = 'b';
    temp->d = 200;

    /* 指针的指针解除一级引用，即为指针的内容 */
    *myclass_p = temp;
}

int main(int argc, const char *argv[])
{
    printf("\n-------------------TEST VALUE-------------------\n");
    /* 比较char、int、long、double变量的大小 */

    char char_v = 'a';
    int  int_v = 10;
    long long_v = 10000L;
    double double_v = 1.9999f;
    struct myclass class_v;

    printf("char size: %d addr: %p value: %c\n", sizeof(char_v), &char_v, char_v);
    printf("int size: %d addr: %p value: %d\n", sizeof(int_v), &int_v, int_v);
    printf("long size: %d addr: %p value: %ld\n", sizeof(long_v), &long_v, long_v);
    printf("double size: %d addr: %p value: %f\n", sizeof(double_v), &double_v, double_v);
    printf("struct myclass size: %d addr: %p\n", sizeof(class_v), &class_v);

    printf("\n-------------------TEST POINT-------------------\n");
    /* 比较char*、int*、long*、double*变量的大小 */

    char    *char_p;
    int     *int_p;
    long    *long_p;
    double  *double_p;
    struct myclass *class_point = (struct myclass*)malloc(sizeof(struct myclass));

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
    printf("long* size: %d addr: %p value: %ld\n", sizeof(long_p), long_p, *long_p);
    printf("double* size: %d addr: %p value: %f\n", sizeof(double_p), double_p, *double_p);
    printf("struct myclass* size: %d addr: %p\n", sizeof(class_point), class_point);
/* 
 *     if (class_point != NULL) {
 *         free(class_point);
 *     }
 *     if (char_p != NULL) {
 *         free(char_p);
 *     }
 *     if (int_p != NULL) {
 *         free(int_p);
 *     }
 *     if (long_p != NULL) {
 *         free(long_p);
 *     }
 *     if (double_p != NULL) {
 *         free(double_p);
 *     } */

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
        printf("val=%d, val addr=%p, val_char_p=%c, val_char_p addr=%p, *val_char_p=%d\n", *val, val, *val_char_p, val_char_p, *val_char_p);
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
    str2 = str3;
    printf("str3 = %s\n", str3);

    printf("\n-------------------TEST STRUCT-------------------\n");
    /* 该测试强制转换一个结构体的变量的地址到一个long型，然后再将其强制
     * 转换成该结构体的指针，当引用这个指针时，可以还原这个结构体的原始
     * 状态。 */

    struct myclass class = {1.0, 2, 3, 4};
    struct myclass *class_p;
    long     context = (long)&class;
    class_p = (struct myclass*)context;

    printf("myclass size: %d, addr: %p, context=%ld, class_p=%p\n", sizeof(class), &class, context, class_p);
    printf("class_p dump a=%f, b=%d, c=%d, d=%d\n", class_p->a, class_p->b, class_p->c, class_p->d);

    printf("\n-------------------TEST CONST-------------------\n");
    /* const修饰的变量分两种：
     *      一种是修饰指针指向的变量，即(const int*) 或(int const*)。
     *  这种形式表示指针指向的变量不可被改变。
     *     另一种是修饰指针本身，即(int* const)。
     *  这种形式表示指针本身不可被改变,就是说该指针不能指向其他内存地址。 */
    int var1 = 10;
    int var2 = 101;
    const int *ro_i1 = &var1;   // 10
    int const *ro_i2 = &var1;   // 10
    int * const ro_i3 = &var2;  // 101,不能同时将一个变量的地址赋值给const修饰的两种情况

    /* *ro_i1 = var2; */    // 指针引用的变量不可被改变
    /* *ro_i2 = var2; */    // 指针引用的变量不可被改变
    *ro_i3 = var1;   // 10

    printf("T1: ro_i1 = %d, ro_i2 = %d, ro_i3 = %d\n", *ro_i1, *ro_i2, *ro_i3);

    ro_i1 = &var2;  // 100
    ro_i2 = &var2;  // 100
    /* ro_i3 = &var2; */    // 指针本身不可被改变

    printf("T2: ro_i1 = %d, ro_i2 = %d, ro_i3 = %d\n", *ro_i1, *ro_i2, *ro_i3);

    printf("\n-------------------TEST POINT POINT-------------------\n");
    /* 声明一个结构体指针，要求在函数中对其初始化。需要给函数传入指针的地址，
     * 这样在函数里解除一级引用，就可以改变指针的值。 */
    struct myclass *class_pp;
    create_myclass(&class_pp);
    printf("class_pp ptr = %p, a = %f, b = %c, c = %c, d = %d\n", class_pp, class_pp->a, class_pp->b, class_pp->c, class_pp->d);
    if (class_pp != NULL) {
        free(class_pp);
    }

    return 0;
}
