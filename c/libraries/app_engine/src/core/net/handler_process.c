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
 *  添加新的协议类型:
 *  1.添加HANDLER_TYPE_*作为字符串认证       <handler_process.h>
 *  2.添加enum remote_type_t的标识           <connection.h>
 *  3.根据类型，做默认的认证处理             <handler_process.c>
 *  4.定义每种类型对应的处理方法             <ui_interface.h>
 *  5.注册每种类型对应的处理方法             <runtime.c>
 *  6.实现每种类型对应的处理方法             <ui_interface.c>
 * =====================================================================================
 */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "connection.h"
#include "handler_process.h"
#include "ui_interface.h"
#include "debug.h"
#include "app.h"

static int indexOfFd(int fd, Remote *remote)
{
    int i;
    for (i = 0; i < CONNECT_MAX_REMOTE_NUM; i++) {
        if (fd == remote[i].remote_fd) {
            return i;
        }
    }

    return -1;
}

ssize_t replay(int fd, const char *msg)
{
    ProtoData proto;
    memset_proto(&proto);
    proto.proto_id = HANDLER_REPLAY_ID;
    // 消息字符串结尾需要添加\r\n
    sprintf(proto.proto_data, "%s", msg);

    ssize_t size = send(fd, &proto, sizeof(proto), 0);

    return size;
}

void memset_proto(ProtoData *proto)
{
    memset(proto, 0, sizeof(ProtoData));
    proto->proto_head = ':';
    proto->proto_tail[0] = '\r';
    proto->proto_tail[1] = '\n';
}

int default_handler(int fd, char *msg, Socket *sock)
{
    const App *s_app = get_app_instance();
    Remote *remote = sock->remote;
    int id = indexOfFd(fd, remote); 

    DEBUG("default_handler: fd = %d, msg = %s\n", fd, msg);
    if (id != -1) {
        if (remote[id].remote_name != NULL) {
            memset(remote[id].remote_name, 0, CONNECT_MAX_NAME_LEN);
            strcpy(remote[id].remote_name, msg);

            /* 当前支持4种类型：
             * 0
             * 1
             * 2
             * 3
             * 根据ID为0的命令字符串标识。参考handler_process.h */
            if (strcmp(remote[id].remote_name, HANDLER_TYPE_1) == 0) {
                remote[id].remote_type = ENUM_REMOTE_1;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_2) == 0) {
                remote[id].remote_type = ENUM_REMOTE_2;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_3) == 0) {
                remote[id].remote_type = ENUM_REMOTE_3;
            } else if (strcmp(remote[id].remote_name, HANDLER_TYPE_4) == 0) {
                remote[id].remote_type = ENUM_REMOTE_4;
            } else {
                remote[id].remote_type = ENUM_REMOTE_NODEFINED;
                replay(fd, HANDLER_REPLAY_FAILED);
                return -1;
            }
            DEBUG("remote[id].remote_name = %s, type = %d\n", remote[id].remote_name, remote[id].remote_type);
        }
    }

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "Net: remote connected id='%02d', name='%s'.", id, remote[id].remote_name);
    Logger *logger = s_app->logger;
    logger->log_d(logger, buf);

    replay(fd, HANDLER_REPLAY_SUCCESS);

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
    App *s_app = get_app_instance();
    HandlerProc *h = handler;
    char buf[2048];

    for (; (h != NULL) && (h->proto_id != HANDLER_TAIL_ID); h++) {
        if (h->proto_id == id) {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "Net: id='%02d',   proto='%s'.", id, msg);
            Logger *logger = s_app->logger;
            logger->log_d(logger, buf);

            rtn = handler[id].proc(fd, msg, sock);

            return rtn;
        }
    }

    fprintf(stderr, "call_handler: id = %d, not defined\n", id);
    return -1;
}

int handler_proc_stub(int fd, Socket *sock, HandlerProc *handler)
{    
    App *s_app = get_app_instance();
    int find = 0;
    ssize_t recv_size = 0;
    ssize_t start = 0;
    ssize_t total_size = sizeof(ProtoData);
    int i, head = -1;
    static char buffer[sizeof(ProtoData)];
    ProtoData proto;
    memset_proto(&proto);

    while (!find) {
        recv_size = recv(fd, &buffer[start], total_size - start, 0);

        /* for (i = 0; i < recv_size; i++) {
         *     printf("buffer[i]=%c\n", buffer[i]);
         * } */

        if (recv_size < 0) {
            fprintf(stderr, "handler_proc_stub: error, id=%d, buffer=%s\n", handler->proto_id, buffer);
            break;
        } else if (recv_size == 0) {
            Logger *logger = s_app->logger;
            logger->log_d(logger, "Net: remote quit.");
            return 0;
        }

        recv_size += start;
        start = 0;

        DEBUG("handler_proc_stub: read once, recv_size=%d\n", recv_size);

        for (i = 0; i < recv_size; i++) {
            if (buffer[i] == ':') {
                head = i;
            } else if ((buffer[i] == '\r') 
                && (i + 1 < recv_size)
                && (buffer[i + 1] == '\n')
                && (head != -1)
                && ((i-head) <= HANDLER_DATA_MAX-2)) {
                memcpy(&proto, &buffer[head], i-head);
                DEBUG("handler_proc_stub: find head=%d, end=%d\n", head, i);

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

    call_handler(handler, proto.proto_id, fd, proto.proto_data, sock);

    return recv_size;
}
