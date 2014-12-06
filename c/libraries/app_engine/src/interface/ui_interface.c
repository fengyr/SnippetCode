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

#include "appe_app.h"
#include "options.h"
#include "ui_interface.h"

static AppeEventHandler s_ui_control_handler;
static AppeEventHandler s_img_data_handler;
static AppeEventHandler s_ping_handler;
static AppeEventHandler s_modbus_handler;

//////////////////////////////////////////////////////
//              界面控制的接口                      //
//////////////////////////////////////////////////////
/* 设置训练参数 */
static int cmd_hello(int fd, char *msg, AppeSocket *sock)
{
    char buf[2048];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "cmd_hello: |%s|, <%c>\n", msg, msg[strlen(msg)-1]);
    printf(buf);

    appe_replay(fd, HANDLER_REPLAY_SUCCESS);

    return 0;
}

static AppeHandlerProc control_proc[] = {
    BEGIN_HANDLER,
    {2, cmd_hello},
    END_HANDLER,
}; 

int ui_control_handler(int fd, AppeSocket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, control_proc);
}

//////////////////////////////////////////////////////
//              发送分色图像的接口                  //
//////////////////////////////////////////////////////
static int test_img_data(int fd, char *msg, AppeSocket *sock)
{
    const char *replay = "received message: handler data";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static AppeHandlerProc img_data_proc[] = {
    BEGIN_HANDLER,
    {2, test_img_data},
    END_HANDLER,
}; 

int ui_img_data_handler(int fd, AppeSocket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, img_data_proc);
}

//////////////////////////////////////////////////////
//              发送参考图像的接口                  //
//////////////////////////////////////////////////////
static int test_ref_data(int fd, char *msg, AppeSocket *sock)
{
    const char *replay = "received message: handler ref data";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static AppeHandlerProc ref_data_proc[] = {
    BEGIN_HANDLER,
    {2, test_ref_data},
    END_HANDLER,
}; 

int ui_ref_data_handler(int fd, AppeSocket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, ref_data_proc);
}

//////////////////////////////////////////////////////
//              相机控制的接口                      //
//////////////////////////////////////////////////////
static int test_camera_control(int fd, char *msg, AppeSocket *sock)
{
    const char *replay = "received message: handler camera control";
    send(fd, replay, strlen(replay)*sizeof(char), 0);

    return 0;
}

static AppeHandlerProc camera_control_proc[] = {
    BEGIN_HANDLER,
    {2, test_camera_control},
    END_HANDLER,
};

int camera_control_handler(int fd, AppeSocket *sock)
{    
    return HANDLER_PROC_STUB(fd, sock, camera_control_proc);
}

//////////////////////////////////////////////////////
//              发送PING的接口                      //
//////////////////////////////////////////////////////
static int send_ping_data(int fd, char *msg, AppeSocket *sock)
{
    printf("send_ping_data: client connected\n");

    return 0;
}

static AppeHandlerProc mobile_data_proc[] = {
    BEGIN_HANDLER,
    {2, send_ping_data},
    END_HANDLER,
}; 

int mobile_handler(int fd, AppeSocket *sock)
{
    return HANDLER_PROC_STUB(fd, sock, mobile_data_proc);
}

//////////////////////////////////////////////////////
//              MODBUS的接口                      //
//////////////////////////////////////////////////////
static int test_modbus(int fd, char *msg, AppeSocket *sock)
{
    printf("test_modbus: client connected\n");

    return 0;
}

static AppeHandlerProc modbus_proc[] = {
    BEGIN_HANDLER,
    {2, test_modbus},
    END_HANDLER,
};

int modbus_protocol_handler(int fd, AppeSocket *sock)
{    
    return HANDLER_PROC_STUB(fd, sock, modbus_proc);
}
