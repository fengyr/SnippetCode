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

#include "tcp_server.h"
#include "connection.h"
#include "handler_process.h"

/* static Socket s_server_sock; */
/* static TcpServer s_tcp_server; */

void tcp_server_register_event_handler(struct tcp_server_t *server, EventHandler *handler)
{
    assert(server != NULL);

    registerHandler(server->sock, handler);
}

static int init(struct tcp_server_t *server, const char *server_name, const char *local_ip, int local_port)
{
    int rtn = tcp_server_init(server->sock, local_ip, local_port, server_name);

    /* 注册界面控制类型的处理方法 */
    /* s_ui_control_handler.handler_type = HANDLER_TYPE_UI_CONTROL; */
    /* s_ui_control_handler.onRecvAndReplay = ui_control_handler; */
    /* registerHandler(server->sock, &s_ui_control_handler); */

    /* 注册结果图片类型的处理方法 */
    /* s_img_data_handler.handler_type = HANDLER_TYPE_IMG_DATA; */
    /* s_img_data_handler.onRecvAndReplay = ui_img_data_handler; */
    /* registerHandler(server->sock, &s_img_data_handler); */

    /* 注册PING的处理方法 */
    /* s_ping_handler.handler_type = HANDLER_TYPE_PING; */
    /* s_ping_handler.onRecvAndReplay = ping_handler; */
    /* registerHandler(server->sock, &s_ping_handler); */

    /* 注册MODBUS类型的处理方法 */
    /* s_modbus_handler.handler_type = HANDLER_TYPE_MODBUS; */
    /* s_modbus_handler.onRecvAndReplay = modbus_protocol_handler; */
    /* registerHandler(server->sock, &s_modbus_handler); */

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

TcpServer* create_tcp_server_instance()
{
    TcpServer *s_tcp_server = (TcpServer*) malloc(sizeof(TcpServer));
    Socket *s_server_sock = (Socket*) malloc(sizeof(Socket));

    assert(s_tcp_server != NULL);
    assert(s_server_sock != NULL);

    s_tcp_server->sock = s_server_sock;
    s_tcp_server->init = init; 
    s_tcp_server->run = run; 
    s_tcp_server->quit = quit;        
    s_tcp_server->register_event_handler = tcp_server_register_event_handler;

    return s_tcp_server;
}
