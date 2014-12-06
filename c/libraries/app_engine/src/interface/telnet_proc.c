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

#include "appe_app.h"
#include "telnet_proc.h"

//////////////////////////////////////////////////////
//              telnet处理方法                      //
//////////////////////////////////////////////////////
static int cmd_hello(int fd, char *msg, AppeSocket *sock)
{
    /* do something */
    appe_replay(fd, "hello world!!\n");

    return 0;
}

static AppeTelnetProc s_telnet_proc[] = {
    {"hello", cmd_hello},
    {"", NULL},
};

static int call_telnet_handler(AppeTelnetProc *handler, int id, int fd, char *msg, AppeSocket *sock)
{
    int rtn = -1;
    char *tail = msg + strlen(msg) - 1;
    while ((*tail == '\r') || (*tail == '\n')) {
        *tail = '\0';
        tail--;
    }

    AppeTelnetProc *h = handler;
    for (; (h != NULL) && (h->telnet_proc_handler != NULL); h++) {
        if (!strcmp(msg, h->telnet_cmd)) {
            rtn = h->telnet_proc_handler(fd, msg, sock);
            return rtn;
        }
    }
    
    fprintf(stderr, "telnet: cmd='%s', not defined\n", msg);

    return -1;
}

static int telnet_proc_stub(int fd, AppeSocket *sock, AppeHandlerProc *handler)
{    
    char buffer[TELNET_BUF_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int recv_size = recv(fd, buffer, TELNET_BUF_SIZE, 0);

    call_telnet_handler(s_telnet_proc, 0, fd, buffer, sock);

    return recv_size;
}

int telnet_handler(int fd, AppeSocket *sock)
{
    return telnet_proc_stub(fd, sock, NULL);
}
