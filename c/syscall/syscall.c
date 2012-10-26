/*
 * =====================================================================================
 *
 *       Filename:  syscall.c
 *
 *    Description:  测试syscall函数
 *
 *        Version:  
 *        Created:  十月 26, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    pid_t pid;

    pid = syscall(SYS_getpid);

    fprintf(stderr, "pid = %d\n", pid);

    return 0;
}
