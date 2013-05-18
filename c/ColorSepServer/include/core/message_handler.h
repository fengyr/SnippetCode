/*
 * =====================================================================================
 *
 *       Filename:  message_handler.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 18, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _message_handler_H_
#define _message_handler_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "message_queue.h"
#include "message.h"

typedef int (*HandlerMessage)(struct message_handler_t *handler, struct message_t *msg);
typedef int (*SendMessage)(struct message_handler_t *handler, struct message_t *msg);

struct message_handler_t {
    struct message_queue_t *queue;
    HandlerMessage handler_message;
    SendMessage send_message;
};
typedef struct message_handler_t MessageHandler, *PMessageHandler;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
void message_handler_init(MessageHandler *handler, struct message_queue_t *queue);
void message_handler_destroy(MessageHandler *handler);
int default_send_message(MessageHandler *handler, struct message_t *msg);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _message_handler_H_ */
