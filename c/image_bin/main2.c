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

extern char _binary_a_jpg_size[];
extern char _binary_a_jpg_start[];
extern char _binary_a_jpg_end[];

int b_var_test = 100;

void hello()
{

    static int c_var_test = 200;

    /*printf("hello c = %d\n", c_var_test);*/

    return;
}

int main(int argc, const char *argv[])
{
    int a_var_test = 1000;

    /*while (1) {*/
        /*printf("address of a is %p, hello is %p\n", &a, hello);*/

        /*sleep(1);*/
    /*}*/

    /*char *b = malloc(1024000);*/
    /*brk((void*) 0x8100000);*/

    /*char cmd[64];*/
    /*sprintf(cmd, "cat /proc/self/maps");*/
    /*system(cmd);*/

    if (a_var_test == 1) {
        /*printf("run internal\n");*/
        goto out;
    } else {
        /*printf("run external\n");*/
    }

out:
    /*printf("goto out image size = %d a = %d \n", _binary_a_jpg_end - _binary_a_jpg_start, a_var_test);*/

out2:
    /*printf("goto out 2\n");*/

    return 0;
}

