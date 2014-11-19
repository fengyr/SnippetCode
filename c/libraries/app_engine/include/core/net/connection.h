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

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#define CONNECT_MAX_REMOTE_NUM  16
#define CONNECT_MAX_HANDLER_NUM 16
#define CONNECT_MAX_NAME_LEN    32

struct event_handler_t;
typedef struct event_handler_t EventHandler, *PEventHandler;

//////////////////////////////////////////////////////
//              远程客户端                          //
//////////////////////////////////////////////////////
enum remote_type_t {
    ENUM_REMOTE_NODEFINED = -1,         // NOT DEFINED
    ENUM_REMOTE_1 = 0,                  // see HANDLER_TYPE_1
    ENUM_REMOTE_2 = 1,                  // see HANDLER_TYPE_2
    ENUM_REMOTE_3 = 2,                  // see HANDLER_TYPE_3
    ENUM_REMOTE_4 = 3,                  // see HANDLER_TYPE_4
};

struct remote_t {
    int remote_fd;
    int remote_port;
    char remote_ip[32];

    enum remote_type_t remote_type;    
    char *remote_name;
};
typedef struct remote_t Remote, *PRemote;

//////////////////////////////////////////////////////
//              本地服务器                          //
//////////////////////////////////////////////////////
struct socket_t {
    int local_fd;
    char local_name[256];
    Remote remote[CONNECT_MAX_REMOTE_NUM];
    EventHandler **pHandlers;

    pthread_t pthread;
    pthread_mutex_t s_mutex;
};
typedef struct socket_t Socket, *PSocket;

//////////////////////////////////////////////////////
//              网络协议处理                        //
//////////////////////////////////////////////////////
/**
 * @Synopsis 针对每组网络协议，声明相对应的处理函数。
 *
 * @Param remote_fd     远端客户端连接fd
 * @Param sock          Socket对象
 *
 * @Returns 
 */
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
 *      网络消息，可参考<ui_interface.c>的实现。
 */
struct event_handler_t {
    const char *handler_type;
    EventHandlerCall onRecvAndReplay;
};

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
int tcp_server_init(Socket *sock, const char *local_ip, int local_port, const char *name);
void tcp_server_run(Socket *sock, int thread_mode);
void tcp_server_quit(Socket *sock);
void registerHandler(Socket *sock, EventHandler *handler);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _connect_H_ */
