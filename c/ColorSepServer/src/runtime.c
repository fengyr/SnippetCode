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

#define MSG_STARTING    0
#define MSG_SEP_1       1
#define MSG_SEP_2       2

static MessageHandler s_message_handler;

static int do_send_message(Message *msg)
{
    MessageHandler *msg_handler = &s_message_handler;    
    msg->handler = msg_handler;
    return msg_handler->send_message(msg_handler, msg);
}

static void on_msg_running()
{
    Message *msg = (Message*)create_empty_message(MSG_SEP_1); 
    do_send_message(msg);
}


static void on_msg_sep_1()
{
    printf("msg sep 1\n");
    Message *msg = (Message*)create_empty_message(MSG_SEP_2); 
    do_send_message(msg);
}

static void on_msg_sep_2()
{
    printf("msg sep 2\n");
    Message *msg = (Message*)create_empty_message(MSG_LOOPER_EXIT); 
    /* Message *msg = (Message*)create_empty_message(MSG_STARTING);  */
    do_send_message(msg);
}

static int handler_message(struct message_handler_t *handler, struct message_t *msg)
{
    switch (msg->id) {
        case MSG_STARTING:
            printf("msg id=%d, arg1=%d, arg2=%d, obj=%p\n", msg->id, msg->arg1, msg->arg2, msg->obj);
            on_msg_running();
            break;
        case MSG_SEP_1:
            on_msg_sep_1();
            break;
        case MSG_SEP_2:
            on_msg_sep_2();
            break;
        default:
            break;
    }

    return 0;
}

static void* thread_runtime(void *param)
{
    App *app = (App*)param;

    MessageHandler *msg_handler = &s_message_handler;    
    msg_handler->handler_message = handler_message;
    msg_handler->queue = app->msg_queue;
    msg_handler->send_message = default_send_message; 

    Message *msg = (Message*)create_empty_message(MSG_STARTING); 
    do_send_message(msg);

    pthread_exit(NULL);
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
    printf("onCreate called\n");

    pthread_t pthread; 
    int res = pthread_create(&pthread, NULL, thread_runtime, (void*)app);
    if (res < 0) {
        perror("on_app_create: pthread_create error");
        exit(1);
    }
    pthread_detach(pthread);

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

    return 0;
}
