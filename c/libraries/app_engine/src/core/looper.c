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

#include "looper.h"
#include "message.h"
#include "debug.h"

static int s_quit = 0;

void looper_init(Looper *looper, MessageQueue *queue)
{
    looper->msg_queue = queue;
}

void looper_run(Looper *looper)
{
    Message *msg;
    MessageHandler *handler;
    MessageQueue *queue = looper->msg_queue;

    while (!s_quit) {
        msg = message_queue_pop(queue);
        if (msg != NULL) {
            if (msg->id == MSG_ON_EXIT) {
                free_message(msg);
                s_quit = 1;
                break;
            }

            handler = msg->handler;
            if (handler != NULL) {
                handler->handler_message(handler, msg);
            } else {
                fprintf(stderr, "not register handler message\n");
            }

            // free message obj
            free_message(msg);
        }

        /* DEBUG("message queue size = %d\n", message_queue_size(queue)); */
    }
}

void looper_exit(Looper *looper)
{
    s_quit = 1;
    fprintf(stderr, "looper_exit\n");
}
