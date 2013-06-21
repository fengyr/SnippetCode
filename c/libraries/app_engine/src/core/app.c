/*
 * =====================================================================================
 *
 *       Filename:  app.c
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
#include <unistd.h>
#include "app.h"
#include "version.h"

static App s_app;
static Looper s_looper;
static MessageQueue s_msg_queue; 
static MessageHandler s_message_handler;
static HandlerMessage s_handler_message;
static int s_loop_run_thread_mode = 1;

static void init(struct app_runtime_t *app)
{
    message_queue_init(app->msg_queue);
    looper_init(&s_looper, app->msg_queue);

    if (app->onCreate != NULL) {
        app->onCreate(app);
    }
}

static void register_handler_message(HandlerMessage handler, int thread_mode)
{
    s_handler_message = handler;
    s_loop_run_thread_mode = thread_mode;
    printf("register handler success\n");
}

static void* thread_runtime(void *param)
{
    App *app = (App*)param;

    MessageHandler *msg_handler = &s_message_handler;    
    msg_handler->handler_message = s_handler_message;
    msg_handler->queue = app->msg_queue;
    msg_handler->send_message = default_send_message; 

    Message *msg = (Message*)create_empty_message(MSG_ON_START); 
    trans_message(msg);

    pthread_exit(NULL);
}

static void trigger_message(App *app)
{
    pthread_t pthread; 
    int res = pthread_create(&pthread, NULL, thread_runtime, (void*)app);
    if (res < 0) {
        perror("trigger_message: pthread_create error");
        exit(1);
    }
    pthread_detach(pthread);
}

static void* thread_loop(void *param)
{
    App *app = (App*)param;

    looper_run(app->looper);

    pthread_exit(NULL);
}

static void start_background_loop(App *app)
{
    pthread_t pthread; 
    int res = pthread_create(&pthread, NULL, thread_loop, (void*)app);
    if (res < 0) {
        perror("start_loop: pthread_create error");
        exit(1);
    }
    pthread_detach(pthread);
}

static void start_foreground_loop(App *app)
{
    printf("app: running foreground mode, so disable call onProcess !!!\n");
    looper_run(app->looper);
}

static void run(struct app_runtime_t *app)
{
    trigger_message(app);

    if (s_loop_run_thread_mode) {
        start_background_loop(app);

        if (NULL == app->onProcess) {
            fprintf(stderr, "app: no process to run !!!\n");
            return;
        }

        while (!app->onProcess(app)) {
            usleep(100);
        }
    } else {
        start_foreground_loop(app);
    }
}

static void quit(struct app_runtime_t *app)
{
    if (app->onDestory != NULL) {
        app->onDestory(app);
    }

    message_queue_destory(app->msg_queue);
    looper_exit(app->looper);

    if (app->camera_server != NULL) {
        app->camera_server->quit(app->camera_server);
    }

    if (app->telnet_server != NULL) {
        app->telnet_server->quit(app->telnet_server);
    }
}

const char* get_version()
{
    return APP_VERSION;
}

int trans_message(Message *msg)
{
    MessageHandler *msg_handler = &s_message_handler;    
    msg->handler = msg_handler;
    return msg_handler->send_message(msg_handler, msg);
}

App* create_app_instance(Options *options)
{
    App *app = &s_app;
    app->msg_queue = &s_msg_queue;
    app->looper = &s_looper;
    app->init = init;
    app->register_handler = register_handler_message;
    app->run = run;
    app->quit = quit;

    // create camera server instance and run server in background
    CameraServer *camera_server = create_camera_server_instance();
    camera_server->init(camera_server, options->cmd.server_ip_addr, options->cmd.server_port);
    camera_server->run(camera_server, 1);

    // create telnet server instance and run server in background
    TelnetServer *telnet_server = create_telnet_server_instance();
    telnet_server->init(telnet_server, options->cmd.server_ip_addr, 11018);
    telnet_server->run(telnet_server, 1);

    // set Options and CameraServer .etc
    app->options = options;
    app->camera_server = camera_server;
    app->telnet_server = telnet_server;
    app->version = get_version;

    return app;
}

App* get_app_instance()
{
    App *app = &s_app;

    return app;
}
