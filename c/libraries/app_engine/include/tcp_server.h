/*
 * =====================================================================================
 *
 *       Filename:  tcp_server.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 10, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _tcp_server_H_
#define _tcp_server_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "connection.h"

struct tcp_server_t {
    Socket *sock;

    int (*init)(struct tcp_server_t*, const char*, const char*, int);
    void (*run)(struct tcp_server_t*, int);
    void (*quit)(struct tcp_server_t*);
    void (*register_event_handler)(struct tcp_server_t *server, EventHandler *handler);
};
typedef struct tcp_server_t TcpServer, *PTcpServer;

TcpServer* create_tcp_server_instance();
void tcp_server_register_event_handler(struct tcp_server_t *server, EventHandler *handler);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _tcp_server_H_ */
