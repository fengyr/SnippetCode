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
#include "tcp_server.h"
#include "connection.h"
#include "ui_interface.h"
#include "handler_process.h"

static Socket s_server_sock;
static EventHandler s_control_handler;
static EventHandler s_data_handler;
static TcpServer s_tcp_server;

static int init(struct tcp_server_t *server, const char *local_ip, int local_port)
{
    int rtn = tcp_server_init(server->sock, local_ip, local_port);

    /* 注册控制通道的处理方法 */
    s_control_handler.handler_type = HANDLER_TYPE_CONTROL;
    s_control_handler.onRecvAndReplay = ui_control_handler;
    registerHandler(server->sock, &s_control_handler);

    /* 注册数据通道的处理方法 */
    s_data_handler.handler_type = HANDLER_TYPE_DATA;
    s_data_handler.onRecvAndReplay = ui_data_handler;
    registerHandler(server->sock, &s_data_handler);

    return rtn;
}

static void run(struct tcp_server_t *server, int thread_mode)
{
    tcp_server_run(server->sock, thread_mode);
}

static void quit(struct tcp_server_t *server)
{
    tcp_server_quit(server->sock);
}

TcpServer* getTcpServer()
{
    s_tcp_server.sock = &s_server_sock;
    s_tcp_server.init = init; 
    s_tcp_server.run = run; 
    s_tcp_server.quit = quit;        

    return &s_tcp_server;
}
