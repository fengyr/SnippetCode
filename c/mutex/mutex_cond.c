#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <error.h>

int g_num;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

void* thread_add(void *param)
{
	sleep(1);
	for (;;) {
        /*加锁互斥量*/
        pthread_mutex_lock(&g_mutex);
        printf("thread 1 lock,");

        /*在唤醒另一个线程的条件变量之前，对变量进行操作*/
		g_num++;
		printf("add num: %d,", g_num);

        pthread_cond_signal(&g_cond);
        /*解锁互斥量*/
        pthread_mutex_unlock(&g_mutex);
        printf("thread 1 unlock\n");
        sleep(1);
	}
}

void* thread_sub(void *param)
{
	for (;;) {
        /*加锁互斥量*/
        pthread_mutex_lock(&g_mutex);
        printf("thread 2 lock,");
        /*等待条件变量被唤醒*/
        pthread_cond_wait(&g_cond, &g_mutex);
        printf("cond waited,");

		g_num--;
		printf("sub num: %d,", g_num);

        /*解锁互斥量*/
        pthread_mutex_unlock(&g_mutex);
        printf("thread 2 unlock\n\n");
        sleep(1);
	}
}

int main(int argc, char **argv)
{
	pthread_t thread1, thread2;
	g_num = 0;

	if (pthread_create(&thread1, NULL, thread_add, (void*)0) < 0) {
		perror("thread 1 error\n");
		exit(1);
	}
	
	if (pthread_create(&thread2, NULL, thread_sub, (void*)0) < 0) {
		perror("thread 2 error\n");
		exit(1);
	}
	
	for (;;) {
		sleep(10);
	}
	return 0;
}
