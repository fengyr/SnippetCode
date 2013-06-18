/*
 * =====================================================================================
 *
 *       Filename:  telnet_server.c
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
#include "telnet_server.h"
#include "connection.h"
#include "telnet_proc.h"
#include "handler_process.h"

static Socket s_server_sock;
static EventHandler s_telnet_handler;
static TelnetServer s_telnet_server;

static int init(struct telnet_server_t *server, const char *local_ip, int local_port)
{
    int rtn = tcp_server_init(server->sock, local_ip, local_port, "telnet");

    /* 注册界面控制类型的处理方法 */
    s_telnet_handler.handler_type = HANDLER_TYPE_TELNET;
    s_telnet_handler.onRecvAndReplay = telnet_handler;
    registerHandler(server->sock, &s_telnet_handler);

    return rtn;
}

static void run(struct telnet_server_t *server, int thread_mode)
{
    tcp_server_run(server->sock, thread_mode);
}

static void quit(struct telnet_server_t *server)
{
    tcp_server_quit(server->sock);
}

TelnetServer* create_telnet_server_instance()
{
    s_telnet_server.sock = &s_server_sock;
    s_telnet_server.init = init; 
    s_telnet_server.run = run; 
    s_telnet_server.quit = quit;        

    return &s_telnet_server;
}
