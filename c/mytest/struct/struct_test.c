/*
 * =====================================================================================
 *
 *       Filename:  struct_test.c
 *
 *    Description:  struct在C语言中的使用
 *
 *        Version:  
 *        Created:  七月 11, 2012
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
#include <sys/types.h>

/* 
 * 结构体用作“位”运算，
 * 并可根据结构体的内存布局判断大小端模式。
 *
 * 小端模式：
 * |高位|...|...|低位|
 * |----|---|---|----|
 * | d  | c | b | a  |
 *  */
struct Bits {
	unsigned int a:2;
	unsigned int b:2;
	unsigned int c:2;
	unsigned int d:2;
};

typedef struct Bits BITS;

/* 结构体封装数据和方法 */
struct CLASS {
	char *c;
	char *d;
	int a;
	int b;
	int (*func)(int*);
	double e;
};

union Union {
    char    a;
    int     b;
    long    c;
};

int class_func(int* str)
{
	printf("this is class_func\n");
	int *p = str;
	return *p;
}

int main(int argc, const char *argv[])
{
    printf("------------------TEST struct for class --------------------\n");
	struct CLASS class = {
		.a = 10, 		        \
		.c = "hello", 	        \
		.d = "world", 	        \
		.func = class_func,     \
	};

	int i = 100;
  	printf("Class member is: %d, %d, %s, %s, %f, %d\nCLASS size is: %d, class size is: %d\n", class.a, class.b, class.c, class.d, class.e, class.func(&i), sizeof(struct CLASS), sizeof(class));

    printf("------------------TEST struct for bits--------------------\n");
    /* 二进制：11011001
     * 十进制：217 */
    BITS bits = {
       .a = 01,
       .b = 10,
       .c = 01,
       .d = 11
    };

    printf("BITS is: %d\n", *(int*)&bits);

    printf("------------------TEST union--------------------\n");
    /* 联合数据结构共享同一个内存区域，
     * 并且其中的值取最大的一个。
     *
     * 小端模式：
     * |成员|高位| .. | .. |低位|
     * |----|----|----|----|----|
     * |  a | 00 | 00 | 00 | 01 |
     * |  b | 00 | 00 | 00 | 01 |
     * |  c | 00 | 00 | 00 | 01 |
     *  */
    union Union u;
    u.c = 1;

    printf("u.a = %d, u.b = %d, u.c = %d\n", u.a, u.b, u.c);

	return 0;
}
