/*
 * =====================================================================================
 *
 *       Filename:  main_loop.h
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
#ifndef _main_loop_H_
#define _main_loop_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "appe_message.h"

struct looper_t {
    MessageQueue *msg_queue;
};
typedef struct looper_t AppeLooper, *PAppeLooper;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
#define MSG_ON_START        0
#define MSG_ON_EXIT         9999

void appe_looper_init(AppeLooper *looper, MessageQueue *queue);
void appe_looper_run(AppeLooper *looper);
void appe_looper_exit(AppeLooper *looper);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _main_loop_H_ */
