/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  
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
#include <poll.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>


int main(int argc, char *argv[])
{
    int fd, rfd;
    struct pollfd pfd;
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

    for (;;) {
        pfd.fd = fd;
        pfd.events = POLLIN | POLLOUT;
        pfd.revents = 0;

        rtn = poll(&pfd, 1, timeout);
        printf("rtn: %d\n", rtn);
        if (rtn > 0) {
            rfd = accept(fd, (struct sockaddr*)&sockin_remote, &sock_len);
            if ( rfd != -1 ) {
                send(rfd, "this is a poll connect", sizeof("this is a poll connect"), 0);
            }
        }
    }

    while (1) {
        sleep(1);
    }

    return 0;
}


