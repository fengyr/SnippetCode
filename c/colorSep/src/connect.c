/*
 * =====================================================================================
 *
 *       Filename:  connect.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 02, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "connect.h"

Socket g_Socket;

static int server_quit = 0;

static void init_socket(Socket *sock)
{
    int i;
    Remote *remote = &sock->remote;
    
    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        remote->remote_fd[i] = -1;
        remote->remote_name[i] = (char*) malloc(sizeof(char)*MAX_NAME_LEN);
        remote->remote_name[i][0] = '\0';
    }

    pthread_mutex_init(&(sock->s_mutex), NULL);
}

static void free_socket(Socket *sock)
{
    int i, *fd;
    Remote *remote = &sock->remote;
    
    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        fd = &remote->remote_fd[i];
        if (*fd != -1) {
            close(*fd);
            *fd = -1;
        }

        if (remote->remote_name[i] != NULL) {
            free(remote->remote_name[i]);
            remote->remote_name[i] = NULL;
        }
    }

    pthread_mutex_destroy(&sock->s_mutex);
}

static void add_client(Socket *sock, int client_fd)
{
    int i;
    Remote *remote = &(sock->remote);

    pthread_mutex_lock(&sock->s_mutex);
    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        if (remote->remote_fd[i] == -1) {
            remote->remote_fd[i] = client_fd;
            break;
        }
    }
    pthread_mutex_unlock(&sock->s_mutex);
}

static void server_thread(void *param)
{
    Socket *sock = (Socket*) param;
    Remote *remote = &(sock->remote);

    int rc, i;
    int max_fd;
    fd_set read_fds;

    while (!server_quit) {
        /* begin set fd_set */
        FD_ZERO(&read_fds);
        max_fd = -1;

        FD_SET(sock->local_fd, &read_fds);

        pthread_mutex_lock(&sock->s_mutex);
        for (i = 0; i < MAX_REMOTE_NUM; i++) {
            if (remote->remote_fd[i] != -1) {
                FD_SET(remote->remote_fd[i], &read_fds);
                if (remote->remote_fd[i] > max_fd) {
                    max_fd = remote->remote_fd[i];
                }
            }
        }
        pthread_mutex_unlock(&sock->s_mutex);
        /* end set fd_set */

        rc = select(max_fd + 1, &read_fds, 0, 0, 0);    
        if (rc < 0) {
            perror("server_thread: select error");
            sleep(1);
            continue;
        } else if (rc == 0) {
            continue;
        }

        if (FD_ISSET(sock->local_fd, &read_fds)) {
            struct sockaddr_in addr;
            socklen_t alen;
            int client;

            do {
                alen = sizeof(addr);
                client = accept(sock->local_fd, (struct sockaddr*)&addr, &alen);
                printf("server_thread: accept loop...\n");
            } while ((client < 0) && (errno == EINTR));

            if (client < 0) {
                perror("server_thread: accept error");
                continue;
            }

            add_client(sock, client);
            continue;
        }

        for (i = 0; i < MAX_REMOTE_NUM; i++) {
            if (i != sock->local_fd && FD_ISSET(remote->remote_fd[i], &read_fds)) {
                // TODO: read from client

                if (rc == 0) {
                    /* close the socket */
                    close(remote->remote_fd[i]);
                    remote->remote_fd[i] = -1;
                    FD_CLR(remote->remote_fd[i], &read_fds);
                } else {
                    // TODO: write to client
                }
            }
        }
    }
}

int init_tcp_server(Socket *sock, char *local_ip, int local_port)
{
    int rtn = -1;
    int val = MAX_REMOTE_NUM;
    struct sockaddr_in localaddr;

    init_socket(sock);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        perror("init_tcp_server: init error");
        goto ERROR;
    }

    memset(&localaddr, 0, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    /* localaddr.sin_addr.s_addr = inet_addr(local_ip); */
    if (NULL == local_ip) {
        localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        inet_aton(local_ip, &(localaddr.sin_addr));
    }

    localaddr.sin_port = htons(local_port);
    memset(&(localaddr.sin_zero), 0, sizeof(localaddr.sin_zero));

    rtn = bind(fd, (struct sockaddr*)&localaddr,
            sizeof(localaddr));
    if (rtn == -1){
        perror("init_tcp_server: bind error");
        goto ERROR;
    }

    rtn = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (rtn < 0){
        perror("init_tcp_server: setsockopt error");
        goto ERROR;
    }

    rtn = listen(fd, MAX_REMOTE_NUM);
    if (rtn < 0) {
        perror("init_tcp_server: listen error");
        goto ERROR;
    }

    sock->local_fd = fd;

    return rtn;

ERROR:
    free_socket(sock);

    return -1;
}

void run_tcp_server(Socket *sock)
{

}

void exit_tcp_server(Socket *sock)
{

}
