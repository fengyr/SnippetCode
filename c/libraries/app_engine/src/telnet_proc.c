/*
 * =====================================================================================
 *
 *       Filename:  telnet_proc.c
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
#include "telnet_proc.h"
#include "connection.h"
#include "handler_process.h"
#include "app.h"
#include "options.h"

static App *s_app_ins = get_app_instance(); 

//////////////////////////////////////////////////////
//              telnet处理方法                      //
//////////////////////////////////////////////////////
static int cmd_hello(int fd, char *msg, Socket *sock)
{
    /* do something */
    replay(fd, "hello world!!\n");

    return 0;
}

static TelnetProc s_telnet_proc[] = {
    {"hello", cmd_hello},
    {"", NULL},
};

static int call_telnet_handler(TelnetProc *handler, int id, int fd, char *msg, Socket *sock)
{
    int rtn = -1;
    char *tail = msg + strlen(msg) - 1;
    while ((*tail == '\r') || (*tail == '\n')) {
        *tail = '\0';
        tail--;
    }

    TelnetProc *h = handler;
    for (; (h != NULL) && (h->telnet_proc_handler != NULL); h++) {
        if (!strcmp(msg, h->telnet_cmd)) {
            rtn = h->telnet_proc_handler(fd, msg, sock);
            return rtn;
        }
    }
    
    fprintf(stderr, "telnet: cmd='%s', not defined\n", msg);

    return -1;
}

static int telnet_proc_stub(int fd, Socket *sock, HandlerProc *handler)
{    
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    int recv_size = recv(fd, buffer, 256, 0);

    call_telnet_handler(s_telnet_proc, 0, fd, buffer, sock);

    return recv_size;
}

int telnet_handler(int fd, Socket *sock)
{
    return telnet_proc_stub(fd, sock, NULL);
}
