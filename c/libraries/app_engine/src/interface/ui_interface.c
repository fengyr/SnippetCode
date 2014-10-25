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

#include "app.h"
#include "ui_interface.h"

static EventHandler s_ui_control_handler;
static EventHandler s_img_data_handler;
static EventHandler s_ping_handler;
static EventHandler s_modbus_handler;

//////////////////////////////////////////////////////
//              界面控制的接口                      //
//////////////////////////////////////////////////////
/* 设置训练参数 */
static int cmd_train(int fd, char *msg, Socket *sock)
{
    /* do something */
    App *s_app_ins = get_app_instance(); 
    Options *options = s_app_ins->options;
    strcpy(options->cmd.config_file_path, "no file");

    replay(fd, REPLAY_SUCCESS);

    return 0;
}

static HandlerProc control_proc[] = {
    BEGIN_HANDLER,
    {2, cmd_train},
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

//////////////////////////////////////////////////////
//              发送PING的接口                      //
//////////////////////////////////////////////////////
static int send_ping_data(int fd, char *msg, Socket *sock)
{
    printf("send_ping_data: client connected\n");

    return 0;
}

static HandlerProc mobile_data_proc[] = {
    BEGIN_HANDLER,
    {2, send_ping_data},
    END_HANDLER,
}; 

int mobile_handler(int fd, Socket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, mobile_data_proc);
}

//////////////////////////////////////////////////////
//              MODBUS的接口                      //
//////////////////////////////////////////////////////
static int test_modbus(int fd, char *msg, Socket *sock)
{
    printf("test_modbus: client connected\n");

    return 0;
}

static HandlerProc modbus_proc[] = {
    BEGIN_HANDLER,
    {2, test_modbus},
    END_HANDLER,
};

int modbus_protocol_handler(int fd, Socket *sock)
{    
    return HANDLER_PROC_STUB(fd, sock, modbus_proc);
}
