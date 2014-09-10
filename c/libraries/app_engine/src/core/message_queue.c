/*
 * =====================================================================================
 *
 *       Filename:  message_queue.c
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
#include <sys/time.h>
#include <time.h>

#include "message.h"
#include "debug.h"

static List s_message_list;

static int assertQueue(MessageQueue *queue)
{
    if (queue == NULL) {
        fprintf(stderr, "message_queue_init error\n");
        return 0;
    }

    return 1;
}

static void barrier_enter(MessageQueue *queue)
{
    /* DEBUG("barrier_enter-> "); */
    pthread_mutex_lock(&(queue->mutex));
}

static void barrier_signal(MessageQueue *queue)
{
    /* DEBUG("barrier_signal-> "); */
    pthread_cond_signal(&(queue->cond));
    pthread_mutex_unlock(&(queue->mutex));
}

static void make_timeout(struct timespec *ts)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = tv.tv_usec * 1000 + COND_TIMEOUT;
}

static void barrier_wait(MessageQueue *queue)
{
    /* DEBUG("barrier_wait-> "); */
    pthread_mutex_lock(&(queue->mutex));
    struct timespec ts;
    make_timeout(&ts);
    pthread_cond_timedwait(&(queue->cond), &(queue->mutex), &ts);
}

static void barrier_leave(MessageQueue *queue)
{
    /* DEBUG("barrier_leave-> "); */
    pthread_mutex_unlock(&(queue->mutex));
}

void message_queue_init(MessageQueue *queue)
{
    if (!assertQueue(queue)) {
        return;
    }

    queue->list = &s_message_list;
    list_init(queue->list, free_message);

    pthread_mutex_init(&(queue->mutex), NULL);
    pthread_cond_init(&(queue->cond), NULL);
}

void message_queue_destory(MessageQueue *queue)
{
    if (!assertQueue(queue)) {
        return;
    }

    DEBUG("message_queue_destory: enter\n");
    barrier_enter(queue);
    list_destroy(queue->list);
    barrier_signal(queue);

    // destroy cond and mutex
    pthread_cond_destroy(&(queue->cond));
    pthread_mutex_destroy(&(queue->mutex));
    DEBUG("message_queue_destory: leave\n");
}

Message* message_queue_pop(MessageQueue *queue)
{
    if (!assertQueue(queue)) {
        return NULL;
    }

    barrier_wait(queue);
    Message *msg = NULL;
    list_remove_next(queue->list, NULL, (void**)&msg);
    barrier_leave(queue);

    return msg;
}

int message_queue_size(MessageQueue *queue)
{
    if (!assertQueue(queue)) {
        return -1;
    }

    barrier_enter(queue);
    int size = list_size(queue->list);
    barrier_leave(queue);

    return size;  
}

int message_queue_push(MessageQueue *queue, Message *msg)
{
    if (!assertQueue(queue)) {
        return -1;
    }

    barrier_enter(queue);
    list_insert_next(queue->list, list_tail(queue->list), msg);
    int size = list_size(queue->list);
    barrier_signal(queue);

    return size; 
}

// debug
void message_queue_dump(MessageQueue *queue)
{
    if (message_queue_size(queue) <= 0) {
        DEBUG("message_queue_dump: size=0\n");
        return;
    }

    Message *msg;
    while ((msg = message_queue_pop(queue)) != NULL) {
        DEBUG("message_queue_dump id=%d, arg1=%d, arg2=%d, obj=%s\n", 
                msg->id, msg->arg1, msg->arg2, (char*)msg->obj);
        free_message(msg);
    }
}
