/*
 * =====================================================================================
 *
 *       Filename:  cleanup.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  三月 06, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>

// __attribute__属性cleanup标识的变量，
// 可为该变量定义一个清除函数。
static inline void fclosep(FILE **fp) { \
    if (*fp) {                          \
        fclose(*fp);                    \
        printf("open file, cleanup!\n");       \
    } else {                            \
        printf("no found file, cleanup!\n");    \
    }                                   \
}
#define _cleanup_fclose_ __attribute__((cleanup(fclosep)))

int main(int argc, const char *argv[])
{
    _cleanup_fclose_ FILE *logfile = fopen("logfile.txt", "r");

    return 0;
}
