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

#include "message.h"

struct looper_t {
    MessageQueue *msg_queue;
};
typedef struct looper_t Looper, *PLooper;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
#define MSG_ON_START        0
#define MSG_ON_EXIT         9999

void looper_init(Looper *looper, MessageQueue *queue);
void looper_run(Looper *looper);
void looper_exit(Looper *looper);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _main_loop_H_ */
