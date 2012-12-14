/**
 *       File       main.c
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.00.00
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2011年12月21日 17时10分05秒
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void hello()
{
    return;
}

int main(int argc, const char *argv[])
{
    int a = 0;
    char *b;

    b = (char*) malloc(100);
    while (1) {
        printf("address of a is %p, hello is %p, b is %p\n", &a, hello, b);

        sleep(1);
    }

    if (a == 1) {
        printf("run internal\n");
        goto out;
    } else {
        printf("run external\n");
    }

out:
    printf("goto out\n");

out2:
    printf("goto out 2\n");

    return 0;
}

