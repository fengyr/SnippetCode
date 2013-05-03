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

struct remote_t {
    int remote_fd[MAX_REMOTE_NUM];
    char *remote_name[MAX_REMOTE_NUM];
};
typedef struct remote_t Remote, *PRemote;

struct socket_t {
    int local_fd;
    Remote remote;

    pthread_mutex_t s_mutex;
};
typedef struct socket_t Socket, *PSocket;

extern Socket g_Socket;

int init_tcp_server(Socket *sock, char *local_ip, int local_port);
void run_tcp_server(Socket *sock);
void exit_tcp_server(Socket *sock);

#endif /* end of include guard: _connect_H_ */
