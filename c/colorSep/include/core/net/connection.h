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

#define MAX_REMOTE_NUM  8
#define MAX_HANDLER_NUM 16
#define MAX_NAME_LEN    32

struct event_handler_t;
typedef struct event_handler_t EventHandler, *PEventHandler;

struct remote_t {
    int remote_fd;
    int remote_type;    /* 0 -> control, 1 -> data */
    char *remote_name;
};
typedef struct remote_t Remote, *PRemote;

struct socket_t {
    int local_fd;
    Remote remote[MAX_REMOTE_NUM];
    EventHandler **pHandlers;

    pthread_t pthread;
    pthread_mutex_t s_mutex;
};
typedef struct socket_t Socket, *PSocket;

////////////////////////////////////////////////////////////
typedef int (*EventHandlerCall)(int remote_fd, Socket *sock);

struct event_handler_t {
    /* 该handler_name需要和remote_name保持一致，
     * 才能够接收对方的信息 */
    const char *handler_name;

    // Receiving client data , if successful returns 0, -1 on failure.
    // This method is implemented by some communication module. 
    // For example, cmd_ui_handler.
    EventHandlerCall onRecvAndReplay;
};
////////////////////////////////////////////////////////////

// public interface
int init_tcp_server(Socket *sock, const char *local_ip, int local_port);
void run_tcp_server(Socket *sock, int thread_mode);
void exit_tcp_server(Socket *sock);
void registerHandler(Socket *sock, EventHandler *handler);

#endif /* end of include guard: _connect_H_ */
