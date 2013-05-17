/*
 * =====================================================================================
 *
 *       Filename:  default_handler_proc.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 08, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "connection.h"
#include "handler_process.h"
#include "ui_interface.h"
#include "debug.h"

static int indexOfFd(int fd, Remote *remote)
{
    int i;
    for (i = 0; i < MAX_REMOTE_NUM; i++) {
        if (fd == remote[i].remote_fd) {
            return i;
        }
    }

    return -1;
}

ssize_t replay(int fd, const char *msg)
{
    CmdUi cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.cmd_id = REPLAY_ID;
    strcpy(cmd.cmd_msg, msg);

    ssize_t size = send(fd, &cmd, sizeof(cmd), 0);

    return size;
}

int default_handler(int fd, char *msg, Socket *sock)
{
    Remote *remote = sock->remote;
    int id = indexOfFd(fd, remote); 

    DEBUG("default_handler: fd = %d, msg = %s\n", fd, msg);
    if (id != -1) {
        if (remote[id].remote_name != NULL) {
            memset(remote[id].remote_name, 0, MAX_NAME_LEN);
            strcpy(remote[id].remote_name, msg);

            /* 当前支持三种类型，0表示控制类型，1表示结果图片类型
             * 2表示参考图片类型。
             * 根据ID为0的命令字符串标识。参考handler_process.h */
            if (strcmp(remote[id].remote_name, HANDLER_TYPE_CONTROL) == 0) {
                remote[id].remote_type = 0;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_IMG_DATA) == 0) {
                remote[id].remote_type = 1;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_REF_DATA) == 0) {
                remote[id].remote_type = 2;
            } else {
                remote[id].remote_type = -1;
                replay(fd, REPLAY_FAILED);
                return -1;
            }
            DEBUG("remote[id].remote_name = %s, type = %d\n", remote[id].remote_name, remote[id].remote_type);
        }
    }

    DEBUG("remote name = %s\n", remote[id].remote_name);

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

int call_handler(HandlerProc *handler, int id, int fd, char *msg, Socket *sock)
{
    int rtn = -1;
    int size = SIZE_OF_HANDLER(handler);
    if (id >= size) {
        fprintf(stderr, "call_handler: the handler id is not avaliable, id = %d\n", id);
        return -1;
    } else {
        rtn = handler[id].proc(fd, msg, sock);
    }

    return rtn;
}
