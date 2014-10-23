/*
 * =====================================================================================
 *
 *       Filename:  tcp_slave.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  八月 11, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _tcp_slave_H_
#define _tcp_slave_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "hashmap.h"

struct tcp_slave_t;
typedef struct tcp_slave_t TcpSlave, *PTcpSlave;
typedef int (*RecvHandlerCall)(TcpSlave *slave, void *data, int len);

/**
 * @Synopsis 客户端接收数据回调
 */
typedef struct recv_handler_t {
    int req_size;       // 请求的数据大小，小于等于0表示动态模式。
    int recv_timeout;   // 接收数据的超时时间。
    RecvHandlerCall onRecvAndReplay;
} RecvHandler, *PRecvHandler;

/**
 * @Synopsis 客户端连接状态
 */
enum tcp_status_t {
    ENUM_TCP_CONNECTING = 0,
    ENUM_TCP_CONNECTED = 1,
    ENUM_TCP_DISCONNECTED = 2,
    ENUM_TCP_CLOSED = 3,
    ENUM_TCP_ERROR = 4,
};

struct tcp_slave_t {
    int slave_fd;
    int server_port;
    int auto_connect;
    char server_ip[64];
    char slave_name[256];
    struct sockaddr_in serveraddr;
    enum tcp_status_t status;
    RecvHandler *pHandlers;

    pthread_t pthread;
    pthread_mutex_t l_mutex;

    int (*register_recv_handler)(struct tcp_slave_t *slave, 
                                  RecvHandler *handler);
    int (*connect)(struct tcp_slave_t *slave, int auto_connect);
    int (*send)(struct tcp_slave_t *slave, void *data, int size);
    int (*recv)(struct tcp_slave_t *slave, void *data, int size);
    int (*disconnect)(struct tcp_slave_t *slave);
};

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
#define SLAVE_AUTO_RECONNECT        1
#define SLAVE_DISABLE_RECONNECT     0

#define SLAVE_STATUS_RECONNECTED    1
#define SLAVE_STATUS_CONNECT_FIRST  0

int slave_tcp_init(TcpSlave *slave, 
                   const char *name, 
                   const char *server_ip, 
                   int port, 
                   int reconnect,
                   int auto_connect);
int slave_register_handler(TcpSlave *slave, RecvHandler *handler);
int slave_tcp_connect(TcpSlave *slave, int auto_connect);

/**
 * @Synopsis    断开客户端连接，重连需要间隔1秒。
 *
 * @Param slave TcpSlave对象指针。
 *
 * @Returns     成功返回0，失败返回-1。
 */
int slave_tcp_disconnect(TcpSlave *slave);

int slave_tcp_send(TcpSlave *slave, void *data, int size);
int slave_tcp_recv(TcpSlave *slave, void *data, int size);
int slave_tcp_close(TcpSlave *slave, int reconnect);

//////////////////////////////////////////////////////
//                  Slave Groups                    //
//////////////////////////////////////////////////////

#define MAX_SLAVES  256

struct tcp_slave_groups_t {
    HashMap hashmap_slave_groups;
    char *slave_names[MAX_SLAVES];
    int slave_count;

    int (*register_slave)(struct tcp_slave_groups_t *groups, 
                          const char *slave_name, 
                          const char *server_ip, 
                          int server_port);
    TcpSlave* (*get_slave)(struct tcp_slave_groups_t *groups, 
                           const char *slave_name);
    int (*init)(struct tcp_slave_groups_t *groups); 
    int (*destroy)(struct tcp_slave_groups_t *groups);
};
typedef struct tcp_slave_groups_t TcpSlaveGroups, *PTcpSlaveGroups;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
TcpSlaveGroups* create_tcp_slave_groups_instance();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _tcp_slave_H_ */
