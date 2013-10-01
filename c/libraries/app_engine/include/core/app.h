/*
 * =====================================================================================
 *
 *       Filename:  MainApp.h
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
#ifndef _MainApp_H_
#define _MainApp_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

#include "message.h"
#include "message_queue.h"
#include "tcp_slave_groups.h"
#include "tcp_server_groups.h"
#include "looper.h"
#include "options.h"
#include "zlogwrap.h"

struct app_runtime_t {
    Options *options;

    MessageQueue *msg_queue;
    Looper *looper;

    TcpServerGroups *tcp_server_groups;
    TcpSlaveGroups *tcp_slave_groups;
    Logger *logger;

    // init && destory
    void (*init)(struct app_runtime_t *app);
    void (*run)(struct app_runtime_t *app);
    void (*quit)(struct app_runtime_t *app);

    void (*parse_options)(struct app_runtime_t *app, Options *options);
    void (*register_message_handler)(HandlerMessage handler, int thread_mode);

    // app engine version
    const char* (*version)(const char *min_ver, const char *mac_ver);

    // 用户自定义方法，在App的生命周期中被调用
    int (*onCreate)(struct app_runtime_t *app);
    int (*onDestory)(struct app_runtime_t *app);
    int (*onProcess)(struct app_runtime_t *app);
};
typedef struct app_runtime_t App, *PApp;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
// 创建一个app的实例
App* create_app_instance(int argc, const char *argv[]);
// 获取已创建的app实例
App* get_app_instance();

// 传送消息
int trans_message(Message *msg);
// 获取程序开始运行的时间
struct timeval* get_boot_time();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _MainApp_H_ */