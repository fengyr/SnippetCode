/*
 * =====================================================================================
 *
 *       Filename:  epoll.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年02月22日 13时05分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/resource.h>

#define MAX_EVENTS 10

int main(int argc, char *argv[])
{
    int fd, rfd, nfds;
    int epollfd;
    struct epoll_event ev, events[10];
    struct sockaddr_in sockin;
    struct sockaddr_in sockin_remote;
    int timeout = 3000;
    int rtn = -1;
    int sock_len;
    sock_len = sizeof(sockin_remote);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&sockin, 0, sizeof(sockin));
    sockin.sin_family = AF_INET;
    inet_aton("127.0.0.1", &sockin.sin_addr);
    sockin.sin_port = htons(9999);
    bind(fd, (struct sockaddr*)&sockin, sizeof(sockin));
    listen(fd, 10);

    struct rlimit rlim;
    getrlimit(RLIMIT_STACK, &rlim);
    printf("stack cur:%d, max:%d\n", rlim.rlim_cur, rlim.rlim_max);

    epollfd = epoll_create(MAX_EVENTS);
    if (epollfd == -1) {
        perror("epoll create err");
        exit(1);
    }

    /*添加需要监听的文件描述符号*/
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    rtn = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    if (rtn == -1) {
        perror("epoll ctl err");
        exit(1);
    }

    int i;
    for (;;) {
        /*等待epoll事件*/
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll wait err");
            exit(1);
        }

        for (i=0; i<nfds; i++) {
            if (events[i].data.fd == fd) {
                rfd = accept(fd, (struct sockaddr*)&sockin_remote, &sock_len);

                printf("nfds: %d rfd: %d\n", nfds, rfd);

                if ( rfd != -1 ) {
                    send(rfd, "this is a poll connect", sizeof("this is a poll connect"), 0);
                }
                close(rfd);
            }
        }
        
    }

    return 0;
}

