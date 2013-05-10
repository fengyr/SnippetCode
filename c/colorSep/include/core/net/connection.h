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

//////////////////////////////////////////////////////
//              网络消息处理                        //
//////////////////////////////////////////////////////
typedef int (*EventHandlerCall)(int remote_fd, Socket *sock);

/**
 * @Synopsis 客户端网络消息处理
 *
 * handler_type: 
 *      handler_type是通信协议中定义的一组交互通信时使用的Key，
 *      该值由本地注册使用。
 *      remote_name是由客户端传递过来的Key(参考Remote结构说明)，
 *      这两个Key值一致，服务器才会接收对方的信息。
 * onRecvAndReplay:
 *      与handler_type绑定的事件处理。该事件主要是来自客户端的
 *      网络消息，可参考<ui_interface.c>。
 */
struct event_handler_t {
    const char *handler_type;
    EventHandlerCall onRecvAndReplay;
};

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
int init_tcp_server(Socket *sock, const char *local_ip, int local_port);
void run_tcp_server(Socket *sock, int thread_mode);
void quit_tcp_server(Socket *sock);
void registerHandler(Socket *sock, EventHandler *handler);

#endif /* end of include guard: _connect_H_ */
