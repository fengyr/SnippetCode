/*
 * =====================================================================================
 *
 *       Filename:  camera_server.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 10, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "camera_server.h"
#include "connection.h"
#include "ui_interface.h"
#include "handler_process.h"

static Socket s_server_sock;
static EventHandler s_ui_control_handler;
static EventHandler s_img_data_handler;
static EventHandler s_ping_handler;
static EventHandler s_modbus_handler;
static CameraServer s_camera_server;

static int init(struct camera_server_t *server, const char *local_ip, int local_port)
{
    int rtn = tcp_server_init(server->sock, local_ip, local_port, "camera");

    /* 注册界面控制类型的处理方法 */
    s_ui_control_handler.handler_type = HANDLER_TYPE_UI_CONTROL;
    s_ui_control_handler.onRecvAndReplay = ui_control_handler;
    registerHandler(server->sock, &s_ui_control_handler);

    /* 注册结果图片类型的处理方法 */
    s_img_data_handler.handler_type = HANDLER_TYPE_IMG_DATA;
    s_img_data_handler.onRecvAndReplay = ui_img_data_handler;
    registerHandler(server->sock, &s_img_data_handler);

    /* 注册PING的处理方法 */
    s_ping_handler.handler_type = HANDLER_TYPE_PING;
    s_ping_handler.onRecvAndReplay = ping_handler;
    registerHandler(server->sock, &s_ping_handler);

    /* 注册MODBUS类型的处理方法 */
    s_modbus_handler.handler_type = HANDLER_TYPE_MODBUS;
    s_modbus_handler.onRecvAndReplay = modbus_protocol_handler;
    registerHandler(server->sock, &s_modbus_handler);

    return rtn;
}

static void run(struct camera_server_t *server, int thread_mode)
{
    tcp_server_run(server->sock, thread_mode);
}

static void quit(struct camera_server_t *server)
{
    tcp_server_quit(server->sock);
}

CameraServer* create_camera_server_instance()
{
    s_camera_server.sock = &s_server_sock;
    s_camera_server.init = init; 
    s_camera_server.run = run; 
    s_camera_server.quit = quit;        

    return &s_camera_server;
}
