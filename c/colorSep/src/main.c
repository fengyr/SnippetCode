/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
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
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#include "connection.h"
#include "ui_interface.h"
#include "handler_process.h"

#define UI_PORT 11014

int main(int argc, const char *argv[])
{
    Socket sock_ui; 
    EventHandler event_handler;
    event_handler.handler_name = HANDLER_DEFAULT_NAME;
    event_handler.onRecvAndReplay = cmd_ui_handler;

    init_tcp_server(&sock_ui, "10.10.96.96", UI_PORT);
    registerHandler(&sock_ui, &event_handler);

    run_tcp_server(&sock_ui, 1);

    while (1) {
        sleep(1);
    }

    return 0;
}
