/*
 * =====================================================================================
 *
 *       Filename:  message.h
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
#ifndef _message_H_
#define _message_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include "list.h"

//////////////////////////////////////////////////////
//                  Message                         //
//////////////////////////////////////////////////////
struct message_t {
    int     id;
    int     arg1;
    int     arg2;
    void    *obj;

    struct message_handler_t *handler;
};
typedef struct message_t Message, *PMessage;

void* create_message(int msg_id, int arg1, int arg2, void *obj);
void* create_empty_message(int msg_id);
void free_message(void *msg);

//////////////////////////////////////////////////////
//                  Message Handler                 //
//////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////
//                  Message Queue                   //
//////////////////////////////////////////////////////
struct message_queue_t {
    struct list_t *list;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
typedef struct message_queue_t MessageQueue, *PMessageQueue;

// debug
void message_queue_dump(MessageQueue *queue);

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
#define COND_TIMEOUT    100*1000

void message_queue_init(MessageQueue *queue);
void message_queue_destory(MessageQueue *queue);
struct message_t* message_queue_pop(MessageQueue *queue);
int message_queue_push(MessageQueue *queue, struct message_t *msg);
int message_queue_size(MessageQueue *queue);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _message_H_ */
