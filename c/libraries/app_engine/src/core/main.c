/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 02, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

#include "app.h"
#include "runtime.h"

static App *app;
extern int g_quit_app;

static void except_quit(int signo)
{
    if (app == NULL) {
        return;
    }

    Logger *logger = app->logger;
    switch (signo) {
        case SIGINT:
            logger->log_e(logger, "Runtime: cause SIGINT.");
            break;
        case SIGKILL:
            logger->log_e(logger, "Runtime: cause SIGKILL.");
            break;
        case SIGTERM:
            logger->log_e(logger, "Runtime: cause SIGTERM.");
            break;
        case SIGABRT:
            logger->log_e(logger, "Runtime: cause SIGABRT.");
            break;
        case SIGSEGV:
            logger->log_e(logger, "Runtime: cause SIGSEGV.");
            break;
        default:
            break;
    }

    signal(signo, SIG_DFL);

    g_quit_app = 1;

    Message *new_msg = (Message*)create_empty_message(MSG_ON_EXIT); 
    trans_message(new_msg);
}

static void signal_handle()
{
    // ignore PIPE, or the process will be die.
    signal(SIGPIPE, SIG_IGN);

    signal(SIGINT, except_quit);
    signal(SIGTERM, except_quit);
    signal(SIGKILL, except_quit);
    signal(SIGABRT, except_quit);
    signal(SIGSEGV, except_quit);

    system("echo './core-%e-%p-%t' > /proc/sys/kernel/core_pattern");
}

int main(int argc, const char *argv[])
{
    signal_handle();

    app = create_app_instance(argc, argv);
    app->onCreate = on_app_create;
    app->onProcess = on_app_process;
    app->onDestory = on_app_destroy;

    app->init(app);
    app->run(app);
    app->quit(app);

    return 0;
}
