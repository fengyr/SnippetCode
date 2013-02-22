/*
 * =====================================================================================
 *
 *       Filename:  thread_tls.c
 *
 *    Description:  线程保存和读取TLS变量的方法
 *
 *        Version:  
 *        Created:  七月 06, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _THREAD_TLS_H_

#define _THREAD_TLS_H_

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_once_t  gTLSOnce = PTHREAD_ONCE_INIT;
pthread_key_t   gTLSKey = 0;
int             g_loop_quit = 0;
int             g_main_quit = 0;
int             *g_key_data;

/* 清理函数，当线程退出时调用该方法 */
void* cleanup_tls(void* data)
{
    /* if (data != NULL) {
     *     free(data);
     *     printf("cleanup_tls called\n");
     * } */
    int *tmp = (int*)data;
    *tmp = 199;

    return 0;
}

/* 子线程的函数 */
void* thread_loop(void *data)
{
    /* 设置gTLSKey相关的私有数据，通过该线程设置的值，
     * 在其他线程中无法访问 */
    int set_key_rc;
    set_key_rc = pthread_setspecific(gTLSKey, (void*)g_key_data);
    printf("thread loop set_key_rc = %d, key value = %d\n", set_key_rc, *g_key_data);

    /* 等待主线程改变全局变量的值 */
    printf("Wait for main thread chang value\n");
    while (!g_loop_quit) {
        sleep(1);
    }

    /* 获取并打印TLS的值，由于我们传入的是全局变量指针，
     * 因此当主线程修改该值后，线程本地获取的值也会被修改 */
    int *key_val = (int*)pthread_getspecific(gTLSKey);
    if (key_val != NULL) {
        printf("thread loop get key value = %d\n\n", *key_val);
    }

    /* 通知main线程退出循环 */
    g_main_quit = 1;

    pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
    pthread_t pthread_loop;
    int thread_id = -1;
    int key_rc;

    /* 为全局变量分配内存 */
    g_key_data = (int*)malloc(sizeof(int));
    (*g_key_data) = 100;

    /* 创建TLS的key */
    key_rc = pthread_key_create(&gTLSKey, (void*)cleanup_tls);
    /* 创建子线程 */
    thread_id = pthread_create(&pthread_loop, NULL, thread_loop, (void*)0);
    printf("create key_rc = %d, create thread_id = %d\n", key_rc, thread_id);

    /* 等待子线程保存TLS变量 */
    printf("Wait for thread loop save key value\n\n");
    sleep(2);

    /* 子线程保存变量后，修改全局变量 */
    *g_key_data = 99;
    printf("\nmain thread change key value = %d\n\n", *g_key_data);
    /* 通知子线程退出循环，并读取保存的TLS值 */
    g_loop_quit = 1;

    /* 循环等待子线程处理 */
    while (!g_main_quit) {
        sleep(1);
    }

    /* 由于主线程没有保存TLS变量，因此读取key时会返回NULL，
     * 说明只有保存TLS变量的线程，才能够读取自己的变量 */
    int *key_val = (int*)pthread_getspecific(gTLSKey);
    if (key_val != NULL) {
        printf("main get key value = %d\n", *key_val);
    } else {
        printf("main can not get TLS value\n");
    }
 
    /* 删除key */
    pthread_key_delete(gTLSKey);

    return 0;
}

#endif /* end of include guard: _THREAD_TLS_H_ */
