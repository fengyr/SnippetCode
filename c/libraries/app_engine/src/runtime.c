/*
 * =====================================================================================
 *
 *       Filename:  runtime.c
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
#include <pthread.h>
#include <unistd.h>

#include "runtime.h"
#include "debug.h"

static int handler_message(struct message_handler_t *handler, struct message_t *msg)
{
    switch (msg->id) {
        case MSG_ON_START: {
            on_msg_start(handler, msg);

            Message *new_msg = (Message*)create_empty_message(MSG_SEP_1); 
            trans_message(new_msg);
            break;
        }
        case MSG_SEP_1: {
            on_msg_step_1(handler, msg);

            Message *new_msg = (Message*)create_message(MSG_SEP_2, 10, 100, (void*)"hello world"); 
            trans_message(new_msg);
            break;
        }
        case MSG_SEP_2: {
            on_msg_step_2(handler, msg);

            /* Message *new_msg = (Message*)create_empty_message(MSG_ON_EXIT);  */
            Message *new_msg = (Message*)create_empty_message(MSG_ON_START); 
            trans_message(new_msg);
            break;
        }

        default:
            break;
    }

    return 0;
}

void on_msg_start(MessageHandler *handler, Message *old_msg)
{
    DEBUG("on_msg_start\n");
}

void on_msg_step_1(MessageHandler *handler, Message *old_msg)
{
    DEBUG("on_msg_step_1 id=%d, arg1=%d, arg2=%d, obj=%p\n", 
           old_msg->id, old_msg->arg1, old_msg->arg2, old_msg->obj);
}

void on_msg_step_2(MessageHandler *handler, Message *old_msg)
{
    DEBUG("on_msg_step_2 id=%d, arg1=%d, arg2=%d, obj=%s\n", 
           old_msg->id, old_msg->arg1, old_msg->arg2, (char*)(old_msg->obj));
}

/**
 * @Synopsis 当App对象创建时，该方法被调用，用于初始化
 *
 * @Param app
 *
 * @Returns 
 */
int on_app_create(struct app_runtime_t *app)
{
    printf("===============\n");
    printf("onCreate called\n");
    app->register_handler(handler_message, 1);

    return 0;
}

/**
 * @Synopsis 当App对象销毁时，该方法被调用，用于释放内存
 *
 * @Param app
 *
 * @Returns 
 */
int on_app_destroy(struct app_runtime_t *app)
{
    printf("onDestory called\n");
    printf("===============\n");

    return 0;
}

/**
 * @Synopsis 当App运行时，该方法被循环调用。
 *
 * @Param app
 *
 * @Returns 返回0，则继续循环；否则退出循环。 
 */
int on_app_process(struct app_runtime_t *app)
{
    /* printf(">>>>>>>>>>>>>>>\n"); */
    return 0;
}
