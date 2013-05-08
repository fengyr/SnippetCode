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

#include "connection.h"

#define HANDLER_DEFAULT_NAME   "type_default"
#define HANDLER_CONTROL_NAME   "type_control"
#define HANDLER_DATA_NAME      "type_data"

struct handler_proc_t {
    int cmd_id;
    int (*proc)(int fd, char *msg, Socket *sock);
};
typedef struct handler_proc_t HandlerProc, *PHandlerProc;

int default_handler(int fd, char *msg, Socket *sock);
int call_handler(HandlerProc *handler, int id, int fd, char *msg, Socket *sock); 

#define DEFAULT_HANDLER {0, default_handler}
#define SIZE_OF_HANDLER(PHANDLER) (sizeof((*PHANDLER))/sizeof(PHandlerProc))

#endif /* end of include guard: _handler_process_H_ */
