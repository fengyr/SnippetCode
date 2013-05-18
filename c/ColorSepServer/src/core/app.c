/*
 * =====================================================================================
 *
 *       Filename:  app_runtime.c
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
#include "app.h"

static App s_app;
static Looper s_looper;
static MessageQueue s_msg_queue; 

static void init(struct app_runtime_t *app)
{
    message_queue_init(app->msg_queue);
    looper_init(&s_looper, app->msg_queue);

    if (app->onCreate != NULL) {
        app->onCreate(app);
    }
}

static void run(struct app_runtime_t *app)
{
    looper_run(app->looper);
}

static void quit(struct app_runtime_t *app)
{
    if (app->onDestory != NULL) {
        app->onDestory(app);
    }

    message_queue_destory(app->msg_queue);
    looper_exit(app->looper);
}

App* getApp()
{
    App *app = &s_app;
    app->msg_queue = &s_msg_queue;
    app->looper = &s_looper;
    app->init = init;
    app->run = run;
    app->quit = quit;

    return app;
}


