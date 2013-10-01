/*
 * =====================================================================================
 *
 *       Filename:  handler_process.h
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
#ifndef _handler_process_H_
#define _handler_process_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include "connection.h"

#define HANDLER_TYPE_DEFAULT            "type_default"
#define HANDLER_TYPE_UI_CONTROL         "type_ui_control"
#define HANDLER_TYPE_IMG_DATA           "type_img_data"
#define HANDLER_TYPE_MOBILE_DATA        "type_mobile_data"
#define HANDLER_TYPE_MODBUS             "type_modbus"

#define HANDLER_TYPE_TELNET             "type_telnet"

#define TAIL_ID             -1
#define REGISTER_ID         0
#define REPLAY_ID           1
#define REPLAY_SUCCESS      "success"
#define REPLAY_FAILED       "failed"

#define MSG_MAX 1024        // 等于TELNET_BUF_SIZE

struct cmd_ui_t {
    int cmd_id;
    char cmd_msg[MSG_MAX];
};
typedef struct cmd_ui_t CmdUi, *PCmdUi;

/**
 * @Synopsis 与命令ID对应的处理过程
 *
 * cmd_id   命令id
 * proc     处理过程的方法
 */
struct handler_proc_t {
    int cmd_id;
    int (*proc)(int fd, char *msg, Socket *sock);
};
typedef struct handler_proc_t HandlerProc, *PHandlerProc;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
ssize_t replay(int fd, const char *msg);
int default_handler(int fd, char *msg, Socket *sock);
int default_replay_handler(int fd, char *msg, Socket *sock);
int call_handler(HandlerProc *handler, int id, int fd, char *msg, Socket *sock); 
int handler_proc_stub(int fd, Socket *sock, HandlerProc *handler);

// 对保留ID=0的命令的处理
#define DEFAULT_HANDLER {REGISTER_ID, default_handler}
// 对保留ID=1的响应的处理
#define DEFAULT_REPLAY {REPLAY_ID, default_replay_handler}
// 处理命令的结尾标识, ID=-1
// 用户自定义的命令处理ID必须从2开始
#define TAIL {TAIL_ID, NULL}
// 默认接收网络协议命令的方法
#define HANDLER_PROC_STUB(fd, sock, handler)  handler_proc_stub((fd), (sock), (handler))

// 命令数组默认定义的的开始部分
#define BEGIN_HANDLER   \
    DEFAULT_HANDLER,    \
    DEFAULT_REPLAY      \
    
// 命令数组默认定义的的结尾部分
#define END_HANDLER     \
    TAIL

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _handler_process_H_ */
