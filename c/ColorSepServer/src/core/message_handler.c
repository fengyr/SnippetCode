/*
 * =====================================================================================
 *
 *       Filename:  message_handler.c
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

#include "message_handler.h"

void message_handler_init(MessageHandler *handler, struct message_queue_t *queue)
{
    handler->queue = queue;
}

void message_handler_destroy(MessageHandler *handler)
{
    handler->queue = NULL;
    fprintf(stderr, "message_handler_destroy\n");
}

int default_send_message(MessageHandler *handler, struct message_t *msg)
{
    if ((handler == NULL) || (msg == NULL)) {
        fprintf(stderr, "default_send_message error\n");
        return -1;
    }
    message_queue_push(handler->queue, msg);

    return 0;
}
