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

#include "options.h"
#include "app.h"
#include "runtime.h"

static void signal_handle()
{
    // ignore PIPE, or the process will be die.
    signal(SIGPIPE, SIG_IGN);
}

int main(int argc, const char *argv[])
{
    signal_handle();

    Options options;
    getOptions(&options, argc, argv);

    App *app = create_app_instance(&options);
    app->onCreate = on_app_create;
    app->onProcess = on_app_process;
    app->onDestory = on_app_destroy;

    app->init(app);
    app->run(app);
    app->quit(app);

    return 0;
}
