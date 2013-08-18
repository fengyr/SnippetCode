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
#include "zlogwrap.h"

static App s_app;
static Looper s_looper;
static MessageQueue s_msg_queue; 
static MessageHandler s_message_handler;
static HandlerMessage s_handler_message;
static int s_loop_run_thread_mode = 1;      // looper是否运行在线程模式
static int s_loop_thread_running = 0;       // 等待looper线程执行
static struct timeval s_start_timeval; 
static Logger s_logger;
static int s_app_exist = 0;                 // App是否已经创建

int g_quit_app = 0;

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

    s_logger.log_i(&s_logger, "Runtime: Background Looper Run.");
    s_loop_thread_running = 1;
    looper_run(app->looper);
    s_logger.log_i(&s_logger, "Runtime: Background Looper Exit.");

    pthread_exit(NULL);
}

static void start_background_loop(App *app)
{
    pthread_t pthread; 
    int res = pthread_create(&pthread, NULL, thread_loop, (void*)app);
    if (res < 0) {
        s_logger.log_e(&s_logger, "Runtime: Background Looper Create Failed.");
        exit(1);
    }
    pthread_detach(pthread);
}

static void start_foreground_loop(App *app)
{
    s_logger.log_i(&s_logger, "Runtime: Foreground Looper Run.");
    looper_run(app->looper);
    s_logger.log_i(&s_logger, "Runtime: Foreground Looper Exit.");
}

static void run(struct app_runtime_t *app)
{
    // record start time
    gettimeofday(&s_start_timeval, NULL);

    // run server after init all module complete.
    CameraServer *camera_server = app->camera_server;
    TelnetServer *telnet_server = app->telnet_server;
    if (camera_server != NULL) {
        camera_server->run(camera_server, 1);
    }
    if (telnet_server != NULL) {
        telnet_server->run(telnet_server, 1);
    }

    trigger_message(app);

    if (s_loop_run_thread_mode) {
        start_background_loop(app);

        // wait for loop start
        while (!s_loop_thread_running) {
            usleep(100000);
        }
        usleep(200000);

        if (NULL == app->onProcess) {
            s_logger.log_e(&s_logger, "Runtime: No Process Running.");
            return;
        }

        s_logger.log_i(&s_logger, "Runtime: Process Start.");
        while (!app->onProcess(app) && !g_quit_app) {
            usleep(100);
        }

        Message *new_msg = (Message*)create_empty_message(MSG_ON_EXIT); 
        trans_message(new_msg);
        usleep(1000);
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

    logger_destroy(&s_logger);
}

const char* get_version()
{
    static char version[128];
    memset(version, 0, sizeof(version));
#ifdef MAC_VERSION
    sprintf(version, "build_%s_%s", APP_VERSION, MAC_VERSION); 
#else
    sprintf(version, "build_%s", APP_VERSION); 
#endif

    return version;
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

    if (s_app_exist) {
        return app;
    }

    app->msg_queue = &s_msg_queue;
    app->looper = &s_looper;
    app->init = init;
    app->register_handler = register_handler_message;
    app->run = run;
    app->quit = quit;

    // init zlog system.
    logger_init(&s_logger, LOG_FILE, LOG_CONFIG_PATH, LOG_FILE_DIR);

    // create camera server instance
    CameraServer *camera_server = create_camera_server_instance();
    camera_server->init(camera_server, options->cmd.server_ip_addr, options->cmd.server_port);

    // create telnet server instance
    TelnetServer *telnet_server = create_telnet_server_instance();
    telnet_server->init(telnet_server, options->cmd.server_ip_addr, 11018);

    // set Options and CameraServer .etc
    app->options = options;
    app->camera_server = camera_server;
    app->telnet_server = telnet_server;
    app->version = get_version;
    app->logger = &s_logger;

    s_app_exist = 1;

    return app;
}

App* get_app_instance()
{
    App *app = &s_app;

    return app;
}

struct timeval* get_boot_time()
{
    return &s_start_timeval;
}
