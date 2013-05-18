/*
 * =====================================================================================
 *
 *       Filename:  message.c
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
#include <stdlib.h>

#include "message.h"
#include "debug.h"

void* create_message(int msg_id, int arg1, int arg2, void *obj)
{
    Message *msg;

    msg = (Message*) malloc(sizeof(*msg));
    msg->id = msg_id;
    msg->arg1 = arg1;
    msg->arg2 = arg2;
    msg->obj = obj;

    return (void*)msg;
}

void* create_empty_message(int msg_id)
{
    Message *msg;

    msg = (Message*) malloc(sizeof(*msg));
    msg->id = msg_id;
    msg->arg1 = -1;
    msg->arg2 = -1;
    msg->obj = NULL;

    return (void*)msg;
}

void free_message(void *message)
{
    if (!message) {
        return;
    }

    Message *msg = (Message*)message;
    DEBUG("free_message, msg_id=%d, arg1=%d, arg2=%d, obj = %p\n", msg->id, msg->arg1, msg->arg2, msg->obj);
    free(msg);
    msg = NULL;
}
