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
#include <assert.h>

#include "connection.h"
#include "handler_process.h"
#include "debug.h"

static int server_quit = 0;

static void dumpInfo(Socket *sock)
{
    DEBUG("sock->local_fd = %d\n", sock->local_fd);

    Remote *remote = sock->remote;
    int i;

    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        DEBUG("remote_fd[%d] = %d, remote_name[%d] = %s\n", i, remote[i].remote_fd, i, remote[i].remote_name);
    }
}

static EventHandler* findHandlerByName(Socket *sock, char *name)
{
    int i;
    EventHandler **pHandler = sock->pHandlers;

    DEBUG("findHandlerByName: enter, name = %s\n", name);
    for (i = 0; i < MAX_HANDLER_NUM; i++) {
        DEBUG("findHandlerByName: loop, name = %s\n", name);
        if ( ((pHandler[i] != NULL) && (strcmp(pHandler[i]->handler_type, name) == 0)) 
                || ((pHandler[i] != NULL) && (strcmp(HANDLER_TYPE_DEFAULT, name) == 0))) {
            DEBUG("findHandlerByName: pHandler[i]->handler_type = %s, name = %s\n", pHandler[i]->handler_type, name);
            return pHandler[i];
        }
    }

    return NULL;
}

static void init_socket(Socket *sock)
{
    int i;
    Remote *remote = sock->remote;

    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        remote[i].remote_fd = -1;
        remote[i].remote_type = -1;
        remote[i].remote_name = (char*) malloc(sizeof(char)*MAX_NAME_LEN);
        memset(remote[i].remote_name, 0, MAX_NAME_LEN);
        strcpy(remote[i].remote_name, HANDLER_TYPE_DEFAULT);
    }

    pthread_mutex_init(&(sock->s_mutex), NULL);
}

static void restore_remote(Socket *sock, int id) 
{
    Remote *remote = sock->remote;

    DEBUG("restore_remote: enter\n");
    pthread_mutex_lock(&sock->s_mutex);

    close(remote[id].remote_fd);
    remote[id].remote_fd = -1;
    remote[id].remote_type = -1;
    memset(remote[id].remote_name, 0, MAX_NAME_LEN);
    strcpy(remote[id].remote_name, HANDLER_TYPE_DEFAULT);

    pthread_mutex_unlock(&sock->s_mutex);
    DEBUG("restore_remote: leave\n");
}

static void free_socket(Socket *sock)
{
    server_quit = 1;

    sleep(1);

    int i, *fd;
    Remote *remote = sock->remote;

    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        fd = &remote[i].remote_fd;
        if (*fd != -1) {
            close(*fd);
            *fd = -1;
        }

        remote[i].remote_type = -1;

        if (remote[i].remote_name != NULL) {
            free(remote[i].remote_name);
            remote[i].remote_name = NULL;
        }
    }

    pthread_mutex_destroy(&sock->s_mutex);

    fprintf(stderr, "server exit...\n");
}

static void add_client(Socket *sock, int client_fd)
{
    int i;
    Remote *remote = sock->remote;

    DEBUG("add_client: enter\n");
    pthread_mutex_lock(&sock->s_mutex);
    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        if (remote[i].remote_fd == -1) {
            remote[i].remote_fd = client_fd;
            break;
        }
    }
    dumpInfo(sock);
    pthread_mutex_unlock(&sock->s_mutex);
    DEBUG("add_client: leave\n");
}

static void* server_thread(void *param)
{
    pthread_detach(pthread_self());

    Socket *sock = (Socket*) param;
    Remote *remote = sock->remote;

    int rc, i;
    int max_fd;
    fd_set read_fds;

    while (!server_quit) {
        /* begin set fd_set */
        FD_ZERO(&read_fds);
        max_fd = -1;

        FD_SET(sock->local_fd, &read_fds);
        max_fd = sock->local_fd;

        pthread_mutex_lock(&sock->s_mutex);
        for (i = 0; i < MAX_REMOTE_NUM; i++) {
            if (remote[i].remote_fd != -1) {
                FD_SET(remote[i].remote_fd, &read_fds);
                if (remote[i].remote_fd > max_fd) {
                    max_fd = remote[i].remote_fd;
                }
            }
        }
        pthread_mutex_unlock(&sock->s_mutex);
        /* end set fd_set */

        DEBUG("server_thread: enter select, max_fd = %d\n", max_fd);
        dumpInfo(sock);

        rc = select(max_fd + 1, &read_fds, 0, 0, 0);    
        if (rc < 0) {
            perror("server_thread: select error");
            sleep(1);
            continue;
        } else if (rc == 0) {
            DEBUG("server_thread: select continue\n");
            continue;
        }

        if (FD_ISSET(sock->local_fd, &read_fds)) {
            DEBUG("server_thread: accept loop...\n");
            struct sockaddr_in addr;
            socklen_t alen;
            int client;

            do {
                alen = sizeof(addr);
                client = accept(sock->local_fd, (struct sockaddr*)&addr, &alen);
            } while ((client < 0) && (errno == EINTR));

            if (client < 0) {
                perror("server_thread: accept error");
                continue;
            }

            add_client(sock, client);
            continue;
        }

        for (i = 0; i < MAX_REMOTE_NUM; i++) {
            if (i != sock->local_fd && FD_ISSET(remote[i].remote_fd, &read_fds)) {
                DEBUG("server_thread: enter event handler\n");
                // read from client and replay
                EventHandler *handler = findHandlerByName(sock, remote[i].remote_name);
                if (handler != NULL) {
                    EventHandlerCall call = handler->onRecvAndReplay;
                    rc = (*call)(remote[i].remote_fd, sock);

                    if (rc <= 0) {
                        /* close the socket, because remote has shutdown. */
                        restore_remote(sock, i);
                    } else {
                        DEBUG("server_thread: handler...\n");
                    }    
                } else {
                    fprintf(stderr, "server_thread: not found EventHandler\n");
                    restore_remote(sock, i);
                }
            }
        }
    }

    pthread_exit(NULL);
}

int tcp_server_init(Socket *sock, const char *local_ip, int local_port)
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
    localaddr.sin_port = htons(local_port);
    /* localaddr.sin_addr.s_addr = inet_addr(local_ip); */
    if ((NULL == local_ip) || (strcmp(local_ip, "") == 0)) {
        localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        inet_aton(local_ip, &(localaddr.sin_addr));
    }
    memset(&(localaddr.sin_zero), 0, sizeof(localaddr.sin_zero));

    rtn = bind(fd, (struct sockaddr*)&localaddr, sizeof(localaddr));
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

    // now set NULL to init handler.
    sock->pHandlers = (EventHandler**) malloc(MAX_HANDLER_NUM * sizeof(EventHandler*));

    return rtn;

ERROR:
    free_socket(sock);

    return -1;
}

void tcp_server_run(Socket *sock, int thread_mode)
{
    assert(sock != NULL);

    int res = -1;

    if (thread_mode) {
        res = pthread_create(&sock->pthread, NULL, server_thread, (void*)sock);
        if (res < 0) {
            perror("run_tcp_server: pthread_create error");
            exit(1);
        }
    } else {
        sock->pthread = -1;
        server_thread((void*)sock);
    }

    printf("server running...\n");
}

void tcp_server_quit(Socket *sock)
{
    assert(sock != NULL);

    free_socket(sock);
}

void registerHandler(Socket *sock, EventHandler *handler)
{
    int i;

    if ((sock != NULL) && !server_quit) {
        for (i = 0; i < MAX_HANDLER_NUM; i++) {
            EventHandler **mHandler = &sock->pHandlers[i];
            if (*mHandler != NULL) {
                if (strcmp((*mHandler)->handler_type, handler->handler_type) == 0) {
                    (*mHandler) = handler;
                    DEBUG("registerHandler: replace handler name = %s, i = %d\n", (*mHandler)->handler_type, i);
                    return;
                }
            } else {
                (*mHandler) = handler;
                DEBUG("registerHandler: insert handler name = %s, i = %d\n", (*mHandler)->handler_type, i);
                return;
            }
        }

        fprintf(stderr, "registerHandler error: no enough space\n");
    }
}
