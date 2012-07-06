#include <stdio.h>

/* 
 * 用一段字符串实现汇编语句，并将其转变成函数地址
 * 该函数实现两个数的交换
 * a = a^b
 * b = b^a
 * a = a^b
 *  */
void(*swap)() = \
    (void(*)()) "\x8b\x44\x24\x04\x8b\x5c\x24\x08\x8b\x00\x8b\x1b\x31\xc3\x31\xd8\x31\xc3\x8b\x4c\x24\x04\x89\x01\x8b\x4c\x24\x08\x89\x19\xc3";
 
int main(){ // works on GCC 3+4
        int a = 37, b = 13;
        swap(&a, &b);
 
        printf("a=%d, b=%d\n\n",a,b);

        a ^= b;
        printf("a^b; a = %d\n", a);
        b ^= a;
        printf("b^a; b = %d\n", b);
        a ^= b;
        printf("a^b; a = %d\n", a);
}
