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
#include "app.h"
#include "options.h"

static App *s_app_ins = get_app_instance(); 

//////////////////////////////////////////////////////
//              界面控制的接口                      //
//////////////////////////////////////////////////////
/* 设置训练参数 */
static int cmd_train(int fd, char *msg, Socket *sock)
{
    /* do something */
    Options *options = s_app_ins->options;
    strcpy(options->cmd.config_file_path, "no file");

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

/* 设置检测崩边崩角 */
static int cmd_detect_collapse(int fd, char *msg, Socket *sock)
{
    /* do something */

    Options *options = s_app_ins->options;

    replay(fd, options->cmd.config_file_path);

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
    BEGIN_HANDLER,
    {2, cmd_train},
    {3, cmd_detect_collapse},
    {4, cmd_debug_mode},
    {5, cmd_color_dis},
    {6, cmd_collapse_thresh},
    {7, cmd_set_rect},
    END_HANDLER,
}; 

int ui_control_handler(int fd, Socket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, control_proc);
}

//////////////////////////////////////////////////////
//              发送分色图像的接口                  //
//////////////////////////////////////////////////////
static int test_img_data(int fd, char *msg, Socket *sock)
{
    const char *replay = "received message: handler data";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static HandlerProc img_data_proc[] = {
    BEGIN_HANDLER,
    {2, test_img_data},
    END_HANDLER,
}; 

int ui_img_data_handler(int fd, Socket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, img_data_proc);
}

//////////////////////////////////////////////////////
//              发送参考图像的接口                  //
//////////////////////////////////////////////////////
static int test_ref_data(int fd, char *msg, Socket *sock)
{
    const char *replay = "received message: handler ref data";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static HandlerProc ref_data_proc[] = {
    BEGIN_HANDLER,
    {2, test_ref_data},
    END_HANDLER,
}; 

int ui_ref_data_handler(int fd, Socket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, ref_data_proc);
}

//////////////////////////////////////////////////////
//              相机控制的接口                      //
//////////////////////////////////////////////////////
static int test_camera_control(int fd, char *msg, Socket *sock)
{
    const char *replay = "received message: handler camera control";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static HandlerProc camera_control_proc[] = {
    BEGIN_HANDLER,
    {2, test_camera_control},
    END_HANDLER,
};

int camera_control_handler(int fd, Socket *sock)
{    
    return HANDLER_PROC_STUB(fd, sock, camera_control_proc);
}
