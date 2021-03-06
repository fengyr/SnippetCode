/*
 * =====================================================================================
 *
 *       Filename:  tcp_server.c
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
#include <assert.h>
#include <stdlib.h>

#include "appe_socket_in.h"
#include "appe_tcp_server.h"
#include "appe_socket_handler.h"

static void __tcp_server_register_event_handler(struct tcp_server_t *server, AppeEventHandler *handler)
{
    assert(server != NULL);
    assert(handler != NULL);

    appe_registerHandler(server->sock, handler);
}

static int __init(struct tcp_server_t *server, const char *server_name, const char *local_ip, int local_port)
{
    int rtn = appe_tcp_server_init(server->sock, local_ip, local_port, server_name);

    return rtn;
}

static void __run(struct tcp_server_t *server, int thread_mode)
{
    appe_tcp_server_run(server->sock, thread_mode);
}

static void __quit(struct tcp_server_t *server)
{
    appe_tcp_server_quit(server->sock);
}

AppeTcpServer* appe_create_tcp_server_instance()
{
    AppeTcpServer *s_tcp_server = (AppeTcpServer*) malloc(sizeof(AppeTcpServer));
    AppeSocket *s_server_sock = (AppeSocket*) malloc(sizeof(AppeSocket));

    assert(s_tcp_server != NULL);
    assert(s_server_sock != NULL);

    s_tcp_server->sock = s_server_sock;
    s_tcp_server->init = __init; 
    s_tcp_server->run = __run; 
    s_tcp_server->quit = __quit;        
    s_tcp_server->register_event_handler = __tcp_server_register_event_handler;

    return s_tcp_server;
}
