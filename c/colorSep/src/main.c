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

#include "options.h"
#include "tcp_server.h"

int main(int argc, const char *argv[])
{
    Options options;
    getOptions(&options, argc, argv);

    TcpServer *server = getTcpServer();
    server->init(server, options.server_ip_addr, options.server_port);
    server->run(server, 1);
    server->quit(server);

    return 0;
}
