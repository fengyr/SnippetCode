/*
 * =====================================================================================
 *
 *       Filename:  cache.c
 *
 *    Description:  比较循环赋值和memcpy的区别
 *
 *        Version:  
 *        Created:  七月 30, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE  4096

#define DUMP_COPY(des, src, size)   \
    int i;                          \
    for (i = 0; i < size; i++) {    \
        des[i] = src[i];            \
    }                               \

#define SMART_COPY(des, src, size)  \
    memcpy(des, src, size);     

void dump_copy(char *des, char *src, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        des[i] = src[i];
    }
}

void smart_copy(char *des, char *src, int size)
{
    memcpy(des, src, size);
}

int main(int argc, const char *argv[])
{
    int mode = 0;

    if (argc < 2) {
        printf("cache 0 | 1\n");
        printf("      0 -> dump copy\n");
        printf("      1 -> smart copy\n");
        exit(1);
    }
    mode = atoi((const char*)argv[1]);

    char source[ARRAY_SIZE], destination[ARRAY_SIZE];
    
    unsigned long i;
    if (0 == mode) {
        for (i = 0; i < 100000000; i++) {
            /* dump_copy(destination, source, ARRAY_SIZE); */
            DUMP_COPY(destination, source, ARRAY_SIZE);
        }
    } else {
        for (i = 0; i < 100000000; i++) {
            /* smart_copy(destination, source, ARRAY_SIZE);  */
            SMART_COPY(destination, source, ARRAY_SIZE);
        }
    }

    return 0;
}
