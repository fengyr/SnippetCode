/*
 * =====================================================================================
 *
 *       Filename:  message_queue.h
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
#ifndef _message_queue_H_
#define _message_queue_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include "list.h"

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

#endif /* end of include guard: _message_queue_H_ */
