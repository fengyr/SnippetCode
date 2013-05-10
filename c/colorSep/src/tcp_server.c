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

int init(struct tcp_server_t *server, const char *local_ip, int local_port)
{
    int rtn = init_tcp_server(server->sock, local_ip, local_port);

    s_control_handler.handler_type = HANDLER_TYPE_CONTROL;
    s_control_handler.onRecvAndReplay = ui_control_handler;
    registerHandler(server->sock, &s_control_handler);

    s_data_handler.handler_type = HANDLER_TYPE_DATA;
    s_data_handler.onRecvAndReplay = ui_data_handler;
    registerHandler(server->sock, &s_data_handler);

    return rtn;
}

void run(struct tcp_server_t *server, int thread_mode)
{
    run_tcp_server(server->sock, thread_mode);
}

void quit(struct tcp_server_t *server)
{
    quit_tcp_server(server->sock);
}

TcpServer g_tcp_server = {
    .sock = &s_server_sock, 
    .init = init,          
    .run = run,           
    .quit = quit        
};
