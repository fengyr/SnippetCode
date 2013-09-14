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

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
    char server_ip[64];
    char slave_name[256];
    struct sockaddr_in serveraddr;
    enum tcp_status_t status;

    int (*connect)(struct tcp_slave_t *slave);
    int (*send)(struct tcp_slave_t *slave, void *data, int size);
    int (*disconnect)(struct tcp_slave_t *slave);
};
typedef struct tcp_slave_t TcpSlave, *PTcpSlave;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
int slave_tcp_init(TcpSlave *slave, const char *name, const char *server_ip, int port);
int slave_tcp_connect(TcpSlave *slave);
int slave_tcp_disconnect(TcpSlave *slave);
int slave_tcp_send(TcpSlave *slave, void *data, int size);
int slave_tcp_close(TcpSlave *slave);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _tcp_slave_H_ */
