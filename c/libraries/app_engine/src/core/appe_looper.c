/*
 * =====================================================================================
 *
 *       Filename:  looper.c
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
#include <stdio.h>

#include "appe_looper.h"
#include "appe_message.h"
#include "appe_debug.h"

static int s_looper_quit = 0;

void appe_looper_init(AppeLooper *looper, MessageQueue *queue)
{
    looper->msg_queue = queue;
}

void appe_looper_run(AppeLooper *looper)
{
    Message *msg;
    MessageHandler *handler;
    MessageQueue *queue = looper->msg_queue;

    while (!s_looper_quit) {
        msg = appe_message_queue_pop(queue);
        if (msg != NULL) {
            if (msg->id == MSG_ON_EXIT) {
                appe_free_message(msg);
                s_looper_quit = 1;
                break;
            }

            handler = msg->handler;
            if ((handler != NULL) && (handler->handler_message != NULL)) {
                handler->handler_message(handler, msg);
            } else {
                fprintf(stderr, "not register handler message\n");
            }

            // free message obj
            appe_free_message(msg);
        }
    }
}

void appe_looper_exit(AppeLooper *looper)
{
    s_looper_quit = 1;
    DEBUG("looper_exit\n");
}
