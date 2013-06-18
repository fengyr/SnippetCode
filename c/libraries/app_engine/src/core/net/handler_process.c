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
    // 消息字符串结尾需要添加\r\n
    sprintf(cmd.cmd_msg, "%s\r\n", msg);

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

            /* 当前支持类型如下：
             * 0表示界面控制类型，
             * 1表示结果图片类型，
             * 2表示参考图片类型，
             * 3表示相机控制类型，
             * 根据ID为0的命令字符串标识。参考handler_process.h */
            if (strcmp(remote[id].remote_name, HANDLER_TYPE_UI_CONTROL) == 0) {
                remote[id].remote_type = ENUM_UI_CONTROL;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_IMG_DATA) == 0) {
                remote[id].remote_type = ENUM_IMG_DATA;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_REF_DATA) == 0) {
                remote[id].remote_type = ENUM_REF_DATA;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_CAMERA_CONTROL) == 0) {
                remote[id].remote_type = ENUM_CAMERA_CONTROL;
            } else {
                remote[id].remote_type = ENUM_NODEFINED;
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

int default_replay_handler(int fd, char *msg, Socket *sock)
{
    DEBUG("default_replay_handler: fd = %d, msg = %s\n", fd, msg);
    return 0;
}

int call_handler(HandlerProc *handler, int id, int fd, char *msg, Socket *sock)
{
    int rtn = -1;
    HandlerProc *h = handler;

    for (; (h != NULL) && (h->cmd_id != TAIL_ID); h++) {
        if (h->cmd_id == id) {
            rtn = handler[id].proc(fd, msg, sock);
            return rtn;
        }
    }

    fprintf(stderr, "call_handler: id = %d, not defined\n", id);
    return -1;
}

int handler_proc_stub(int fd, Socket *sock, HandlerProc *handler)
{    
    int find = 0;
    ssize_t recv_size = 0;
    ssize_t start = 0;
    ssize_t total_size = sizeof(CmdUi);
    int i;
    static char buffer[sizeof(CmdUi)];
    CmdUi cmd;
    memset(&cmd, 0, sizeof(CmdUi));

    while (!find) {
        recv_size = recv(fd, &buffer[start], total_size - start, 0);

        /* for (i = 0; i < recv_size; i++) {
         *     printf("buffer[i]=%c\n", buffer[i]);
         * } */

        if (recv_size < 0) {
            fprintf(stderr, "handler_proc_stub: error, id=%d\n", handler->cmd_id);
            break;
        } else if (recv_size == 0) {
            printf("handler_proc_stub: client quit!!!\n");
            return 0;
        }

        recv_size += start;
        start = 0;

        for (i = 0; i < recv_size; i++) {
            if ((buffer[i] == '\r') 
                && (i + 1 < recv_size)
                && (buffer[i + 1] == '\n')) {
                memcpy(&cmd, &buffer[start], i-start);

                start = i + 1;
                find = 1;
            }
        }

        if (start != recv_size) {
            int remaining = total_size - start;
            memmove(buffer, &buffer[start], remaining);
            start = remaining;
        } else {
            start = 0;
        }
    }

    call_handler(handler, cmd.cmd_id, fd, cmd.cmd_msg, sock);

    return recv_size;
}
