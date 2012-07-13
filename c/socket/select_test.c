/*
 * =====================================================================================
 *
 *       Filename:  select_test.c
 *
 *    Description:  select I/O多路复用示例
 *
 *        Version:  
 *        Created:  七月 10, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>

static int PORTS[] = {9998, 9999};
static int MAX_FD = 2;
static int TIMEOUT = 5;

int main(int argc, const char *argv[])
{   
    int fd[2], rfd[2];
    struct sockaddr_in sockin[2];
    struct sockaddr_in sockin_remote[2];
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int i;

    /* 初始化socket one */
    for (i = 0; i < MAX_FD; i++) {
        fd[i] = socket(AF_INET, SOCK_STREAM, 0);
        memset(&sockin[i], 0, sizeof(struct sockaddr_in));
        sockin[i].sin_family = AF_INET;
        inet_aton((const char*)"127.0.0.1", &sockin[i].sin_addr);
        sockin[i].sin_port = htons(PORTS[i]);
        bind(fd[i], (struct sockaddr*)&sockin[i], sizeof(struct sockaddr_in));
        listen(fd[i], 10);
        printf("init fd[i]=%d\n", fd[i]);
    }

    fd_set  read_fds;
    int     max_fd;
    int     rc;
    ssize_t ssize;
    struct timeval tv;

    do {
        FD_ZERO(&read_fds);
        max_fd = 0;
        // 初始化可读的文件描述符集合
        for(i = 0; i < MAX_FD; i++){
            if(fd[i] != -1){
                FD_SET(fd[i], &read_fds);
                if(fd[i] > max_fd){
                    max_fd = fd[i];
                }
            }
        }
        // 最后需要将最大的描述符加一 
        max_fd++;

        // 忽略可写的fd，只监听可读的fd
        // 设置timeval超时机制
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;
        rc = select(max_fd, &read_fds, NULL, NULL, &tv);
        printf("select rc=%d\n", rc);
        if(rc == 0) {
            printf("select timeout %d sec\n", TIMEOUT);
            continue;
        }

        for (i = 0; i < MAX_FD; i++) {
            if ((fd[i] != -1) && FD_ISSET(fd[i], &read_fds)) {
                rfd[i] = accept(fd[i], (struct sockaddr*)&sockin_remote[i], &sock_len);
                if ( rfd[i] != -1 ) {
                    ssize = send(rfd[i], "this is a select connect\n", sizeof("this is a select connect\n"), 0);
                    printf("rfd = %d, ssize = %d\n\n", rfd[i], ssize);
                }
            }
        }
    } while (1);

    return 0;
}
