/*
 * =====================================================================================
 *
 *       Filename:  cmd_ui_handler.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 06, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "ui_interface.h"
#include "connection.h"
#include "handler_process.h"

static int say_hello(int fd, char *msg, Socket *sock)
{
    const char *replay = "received message: say hello";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static HandlerProc handler_proc[] = {
    DEFAULT_HANDLER,
    {1, say_hello},
}; 

int cmd_ui_handler(int fd, Socket *sock)
{
    ssize_t recv_size;
    CmdUi cmd;

    memset(&cmd, 0, sizeof(cmd));
    recv_size = recv(fd, &cmd, sizeof(cmd), 0);

    if (recv_size < 0) {
        perror("cmd_ui_handler: error");
    } else if (recv_size == 0) {
        printf("cmd_ui_handler: client quit!!!\n");
    } else {
        DEBUG("cmd_id = %d, cmd_msg = %s\n", cmd.cmd_id, cmd.cmd_msg);
        call_handler(handler_proc, cmd.cmd_id, fd, cmd.cmd_msg, sock);
    }

    return recv_size;
}
