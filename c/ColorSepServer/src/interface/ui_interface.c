/*
 * =====================================================================================
 *
 *       Filename:  ui_interface.c
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

//////////////////////////////////////////////////////
//              type_control                        //
//////////////////////////////////////////////////////
/* 设置训练参数 */
static int cmd_train(int fd, char *msg, Socket *sock)
{
    /* do something */

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

/* 设置检测崩边崩角 */
static int cmd_detect_collapse(int fd, char *msg, Socket *sock)
{
    /* do something */

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

/* 设置调试模式 */
static int cmd_debug_mode(int fd, char *msg, Socket *sock)
{
    /* do something */

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

/* 设置色差阀值 */
static int cmd_color_dis(int fd, char *msg, Socket *sock)
{
    /* do something */

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

/* 设置崩边阀值 */
static int cmd_collapse_thresh(int fd, char *msg, Socket *sock)
{
    /* do something */

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

/* 设置矩形参数 */
static int cmd_set_rect(int fd, char *msg, Socket *sock)
{
    /* do something */

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

static HandlerProc control_proc[] = {
    DEFAULT_HANDLER,
    {1, cmd_train},
    {2, cmd_detect_collapse},
    {3, cmd_debug_mode},
    {4, cmd_color_dis},
    {5, cmd_collapse_thresh},
    {6, cmd_set_rect},
}; 

int ui_control_handler(int fd, Socket *sock)
{
    ssize_t recv_size;
    CmdUi cmd;

    memset(&cmd, 0, sizeof(cmd));
    recv_size = recv(fd, &cmd, sizeof(cmd), 0);

    if (recv_size < 0) {
        perror("ui_handler: error");
    } else if (recv_size == 0) {
        printf("ui_handler: client quit!!!\n");
    } else {
        DEBUG("cmd_id = %d, cmd_msg = %s\n", cmd.cmd_id, cmd.cmd_msg);
        call_handler(control_proc, cmd.cmd_id, fd, cmd.cmd_msg, sock);
    }

    return recv_size;
}

//////////////////////////////////////////////////////
//              type_img_data                       //
//////////////////////////////////////////////////////
static int test_img_data(int fd, char *msg, Socket *sock)
{
    const char *replay = "received message: handler data";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static HandlerProc img_data_proc[] = {
    DEFAULT_HANDLER,
    {1, test_img_data},
}; 

int ui_img_data_handler(int fd, Socket *sock)
{
    ssize_t recv_size;
    CmdUi cmd;

    memset(&cmd, 0, sizeof(cmd));
    recv_size = recv(fd, &cmd, sizeof(cmd), 0);

    if (recv_size < 0) {
        perror("img_data_handler: error");
    } else if (recv_size == 0) {
        printf("img_data_handler: client quit!!!\n");
    } else {
        DEBUG("cmd_id = %d, cmd_msg = %s\n", cmd.cmd_id, cmd.cmd_msg);
        call_handler(img_data_proc, cmd.cmd_id, fd, cmd.cmd_msg, sock);
    }

    return recv_size;
}

//////////////////////////////////////////////////////
//              type_ref_data                       //
//////////////////////////////////////////////////////
static int test_ref_data(int fd, char *msg, Socket *sock)
{
    const char *replay = "received message: handler ref data";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static HandlerProc ref_data_proc[] = {
    DEFAULT_HANDLER,
    {1, test_ref_data},
}; 

int ui_ref_data_handler(int fd, Socket *sock)
{
    ssize_t recv_size;
    CmdUi cmd;

    memset(&cmd, 0, sizeof(cmd));
    recv_size = recv(fd, &cmd, sizeof(cmd), 0);

    if (recv_size < 0) {
        perror("ref_data_handler: error");
    } else if (recv_size == 0) {
        printf("ref_data_handler: client quit!!!\n");
    } else {
        DEBUG("cmd_id = %d, cmd_msg = %s\n", cmd.cmd_id, cmd.cmd_msg);
        call_handler(ref_data_proc, cmd.cmd_id, fd, cmd.cmd_msg, sock);
    }

    return recv_size;
}
