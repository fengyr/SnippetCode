/*
 * =====================================================================================
 *
 *       Filename:  runtime.cpp
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十月 10, 2014
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
#include <string.h>
#include <sys/socket.h>
#include <sys/vfs.h>
#include <time.h>
#include <errno.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

#include "runtime.h"
#include "options.h"

static App *s_app = appe_get_app_instance();

static int handler_message(struct message_handler_t *handler, struct message_t *msg)
{
    switch (msg->id) {
        case MSG_ON_START: {
            on_msg_start(handler, msg);
            break;
        }
        case MSG_IDLE: {
            on_msg_idle(handler, msg);
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

void on_msg_idle(MessageHandler *handler, Message *old_msg)
{
    DEBUG("on_msg_idle\n");
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
    // Options Hooks
    Logger *logger = app->logger;
    Options *options = app->options;

    // 消息采用线程模式
    app->register_message_handler(handler_message, RUNTIME_LOOP_THREAD);

    logger->log_i(logger, "==========   App Start   ==========");

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
    Logger *logger = app->logger;

    logger->log_i(logger, "==========   App Destroy     ==========");
    app->save_options(app, app->options, app->options->cmd.config_file_path);

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
#if USE_DEBUG
    struct timeval tpstart, tpend;
    float timeuse = 0;
    gettimeofday(&tpstart, NULL);
#endif

    Options *options = s_app->options;

#if USE_DEBUG
    gettimeofday(&tpend, NULL);
    timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
    timeuse /= 1000000;
    // fprintf(stderr, ">>>>> Loop once time: %.3fs <<<<<\n", timeuse);
#endif

    return 0;
}
