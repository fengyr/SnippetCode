/*
 * =====================================================================================
 *
 *       Filename:  connect.h
 *
 *    Description: 网络链接管理模块 
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
#ifndef _connect_H_
#define _connect_H_

#include <pthread.h>

#define MAX_REMOTE_NUM   8
#define MAX_NAME_LEN    32
#define DEFAULT_CLIENT_NAME "default"

struct remote_t {
    int remote_fd[MAX_REMOTE_NUM];
    char *remote_name[MAX_REMOTE_NUM];
};
typedef struct remote_t Remote, *PRemote;

typedef int (*EventHandlerCall)(int remote_fd);

struct event_handler_t {
    char *handler_name;

    // Receiving client data , if successful returns 0, -1 on failure.
    // This method is implemented by some communication module. 
    // For example, cmd_ui_handler.
    EventHandlerCall onRecvAndReplay;
};
typedef struct event_handler_t EventHandler, *PEventHandler;

struct socket_t {
    int local_fd;
    Remote remote;
    EventHandler **pHandlers;

    pthread_t pthread;
    pthread_mutex_t s_mutex;
};
typedef struct socket_t Socket, *PSocket;

int init_tcp_server(Socket *sock, char *local_ip, int local_port);
void run_tcp_server(Socket *sock, int thread_mode);
void exit_tcp_server(Socket *sock);
void registerHandler(Socket *sock, EventHandler *handler);

#endif /* end of include guard: _connect_H_ */
