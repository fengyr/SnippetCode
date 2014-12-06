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
#ifndef _appe_socket_handler_H_
#define _appe_socket_handler_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include "appe_socket_in.h"

#define HANDLER_TYPE_DEFAULT            "type_default"
#define HANDLER_TYPE_1                  "type_handler_1"
#define HANDLER_TYPE_2                  "type_handler_2"
#define HANDLER_TYPE_3                  "type_handler_3"
#define HANDLER_TYPE_4                  "type_handler_4"

#define HANDLER_TYPE_TELNET             "type_telnet"

#define HANDLER_TAIL_ID                 -1
#define HANDLER_REGISTER_ID             0
#define HANDLER_REPLAY_ID               1
#define HANDLER_REPLAY_SUCCESS          "success"
#define HANDLER_REPLAY_FAILED           "failed"

#define HANDLER_DATA_MAX 1024               // 等于TELNET_BUF_SIZE
#define HANDLER_MSG_MAX ((HANDLER_DATA_MAX) - 7)

// 总共命令行大小为1024字节
struct proto_data_t {
    char proto_head;                        // 1 byte
    unsigned int proto_id;                  // 4 bytes
    char proto_data[HANDLER_MSG_MAX];       // 1017 bytes
    char proto_tail[2];                     // 2 bytes
};
typedef struct proto_data_t AppeProtoData, *PAppeProtoData;

/**
 * @Synopsis 与命令ID对应的处理过程
 *
 * proto_id   命令id
 * proc     处理过程的方法
 */
struct handler_proc_t {
    int proto_id;
    int (*proc)(int fd, char *msg, AppeSocket *sock);
};
typedef struct handler_proc_t AppeHandlerProc, *PAppeHandlerProc;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
ssize_t appe_replay(int fd, const char *msg);
int appe_default_handler(int fd, char *msg, AppeSocket *sock);
int appe_default_replay_handler(int fd, char *msg, AppeSocket *sock);
int appe_call_handler(AppeHandlerProc *handler, int id, int fd, char *msg, AppeSocket *sock); 
int appe_handler_proc_stub(int fd, AppeSocket *sock, AppeHandlerProc *handler);
void appe_memset_proto(AppeProtoData *proto);

// 对保留ID=0的命令的处理
#define DEFAULT_HANDLER {HANDLER_REGISTER_ID, appe_default_handler}
// 对保留ID=1的响应的处理
#define DEFAULT_REPLAY {HANDLER_REPLAY_ID, appe_default_replay_handler}
// 处理命令的结尾标识, ID=-1
// 用户自定义的命令处理ID必须从2开始
#define HANDLER_TAIL {HANDLER_TAIL_ID, NULL}
// 默认接收网络协议命令的方法
#define HANDLER_PROC_STUB(fd, sock, handler)  appe_handler_proc_stub((fd), (sock), (handler))

// 命令数组默认定义的的开始部分
#define BEGIN_HANDLER   \
    DEFAULT_HANDLER,    \
    DEFAULT_REPLAY      \
    
// 命令数组默认定义的的结尾部分
#define END_HANDLER     \
    HANDLER_TAIL

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _appe_socket_handler_H_ */
