/*
 * =====================================================================================
 *
 *       Filename:  malloc_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年07月27日 13时57分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int MB = 0;
    int *p;

    while ( (p = malloc(1<<20)) ) {
        MB++;

        if (MB > 1000) {
            *p = 10;
        }
    }

    printf("Allocated %d MB Total\n", MB);

    sleep(1);

    return 0;
}
