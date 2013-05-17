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

#define HANDLER_TYPE_DEFAULT    "type_default"
#define HANDLER_TYPE_CONTROL    "type_ui_control"
#define HANDLER_TYPE_IMG_DATA   "type_img_data"
#define HANDLER_TYPE_REF_DATA   "type_ref_data"

#define REPLAY_ID           100
#define REPLAY_SUCCESS      "success"
#define REPLAY_FAILED       "failed"

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
int call_handler(HandlerProc *handler, int id, int fd, char *msg, Socket *sock); 

#define DEFAULT_HANDLER {0, default_handler}
#define SIZE_OF_HANDLER(PHANDLER) (sizeof((*PHANDLER))/sizeof(PHandlerProc))

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _handler_process_H_ */
