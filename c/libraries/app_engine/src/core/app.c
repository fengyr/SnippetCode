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
#include <string.h>

#include "app.h"

static App s_app;
static Looper s_looper;
static MessageQueue s_msg_queue; 
static MessageHandler s_message_handler;
static HandlerMessage s_handler_message;
static int s_loop_thread_mode = RUNTIME_LOOP_THREAD;    // looper是否运行在线程模式
static int s_loop_thread_running = 0;                   // 等待looper线程执行
static struct timeval s_start_timeval; 
static Logger s_logger;
static int s_app_exist = 0;                             // App是否已经创建

static int s_argc;
static const char **s_argv;

int g_quit_app = 0;

static void init(struct app_runtime_t *app)
{
    // 解析命令行参数
    pushOptions(&app->options);
    app->parse_options(app, app->options);

    // 初始化消息队列
    message_queue_init(app->msg_queue);
    looper_init(&s_looper, app->msg_queue);

    if (app->onCreate != NULL) {
        app->onCreate(app);
    }
}

static void register_message_handler(HandlerMessage handler, int thread_mode)
{
    s_handler_message = handler;
    s_loop_thread_mode = thread_mode;
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

    trigger_message(app);

    if (s_loop_thread_mode) {
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

    if (app->task_manager != NULL) {
        app->task_manager->destory(app->task_manager);
    }

    message_queue_destory(app->msg_queue);
    looper_exit(app->looper);

    if (app->tcp_server_groups != NULL) {
        app->tcp_server_groups->destroy(app->tcp_server_groups);        
    }

    if (app->tcp_slave_groups != NULL) {
        app->tcp_slave_groups->destroy(app->tcp_slave_groups);        
    }

#ifdef USE_SQLITE
    if (app->sqlite_client != NULL) {
        app->sqlite_client->destory(app->sqlite_client);
    }
#endif

    logger_destroy(&s_logger);
}

const char* get_version(const char *min_ver, const char *mac_ver)
{
    static char ver[128];
    memset(ver, 0, sizeof(ver));
    sprintf(ver, "build_%s.%s_%s", APP_VERSION, min_ver, mac_ver); 

    return ver;
}

int trans_message(Message *msg)
{
    MessageHandler *msg_handler = &s_message_handler;    
    msg->handler = msg_handler;
    return msg_handler->send_message(msg_handler, msg);
}

void parse_options(struct app_runtime_t *app, Options *options)
{
    getOptions(options, s_argc, s_argv);
    app->options = options;
}

void save_options(struct app_runtime_t *app, Options *options, char *config_file_path)
{
    if (!strcmp(config_file_path, "")) {
        return; 
    }

    int rc = -1;
    int buf_size = 8*1024*1024;
    char *buffer = (char*) malloc(buf_size);
    memset(buffer, 0, buf_size);

    CSimpleIniA ini;
    ini.SetUnicode();

    int is_write = formatOptions(options, buffer, buf_size);
    if (is_write != OPTIONS_WRITE) {
        goto SAVE_QUIT;
    }

    rc = ini.LoadData(buffer, strlen(buffer));
    if (rc < 0) {
        s_logger.log_e(&s_logger, "APP: Load Options Error.");
    }

    rc = ini.SaveFile(config_file_path);
    if (rc < 0) {
        s_logger.log_e(&s_logger, "APP: Save Options Error.");
    }

SAVE_QUIT:
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
}

App* create_app_instance(int argc, const char *argv[])
{
    App *app = &s_app;
    s_argc = argc;
    s_argv = argv;

    if (s_app_exist) {
        return app;
    }

    app->msg_queue = &s_msg_queue;
    app->looper = &s_looper;
    app->init = init;
    app->register_message_handler = register_message_handler;
    app->parse_options = parse_options;
    app->save_options = save_options;
    app->run = run;
    app->quit = quit;

    // init zlog system.
    /* char log_file_dir[1024];
     * memset(log_file_dir, 0, sizeof(log_file_dir));
     * getcwd(log_file_dir, sizeof(log_file_dir)); */
    logger_init(&s_logger, LOG_FILE, LOG_CONFIG_PATH, LOG_FILE_DIR);

    // TaskManager
    TaskManager *task_manager = create_taskmanager_instance();

    // create tcp server groups instance
    TcpServerGroups *tcp_server_groups = create_tcp_server_groups_instance();
    tcp_server_groups->init(tcp_server_groups);

    // create tcp slave groups instance
    TcpSlaveGroups *tcp_slave_groups = create_tcp_slave_groups_instance();
    tcp_slave_groups->init(tcp_slave_groups);

#ifdef USE_SQLITE
    // create sqlite instance
    SqliteClient *sqlite_client = create_sqlite_client_instance();
    app->sqlite_client = sqlite_client;
#endif

    // set Servers .etc
    app->tcp_server_groups = tcp_server_groups;
    app->tcp_slave_groups = tcp_slave_groups;
    app->task_manager = task_manager;
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
