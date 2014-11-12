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

// C++代码做特别处理
#include "SimpleIni.h"
#include "db_sqlite_wrap.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

#include "hw_module.h"

#include "modbus.h"
#include "modbus_master.h"
#include "modbus_rtu.h"
#include "modbus_tcp.h"

#include "db_column.h"
#include "db_mysql_wrap.h"
#include "db_sqlite_wrap.hpp"

#include "connection.h"
#include "handler_process.h"
#include "tcp_server.h"
#include "tcp_slave.h"
#include "telnet_server.h"

#include "array.h"
#include "commandline.h"
#include "hashmap.h"
#include "ini.h"
#include "list.h"
#include "md5.h"
#include "zlog.h"
#include "zlogwrap.h"
#include "threadpool.h"
#include "taskmanager.h"
#include "SimpleIni.h"

#include "debug.h"
#include "looper.h"
#include "message.h"
#include "version.h"

#include "options.h"

struct app_runtime_t {
    // 命令行参数
    Options *options;

    // 内部接口
    MessageQueue *msg_queue;
    Looper *looper;

    // 外部接口
#ifdef USE_SQLITE
    SqliteClient *sqlite_client;
#endif
    TcpServerGroups *tcp_server_groups;
    TcpSlaveGroups *tcp_slave_groups;
    TaskManager *task_manager;
    Logger *logger;

    // init && destory
    void (*init)(struct app_runtime_t *app);
    void (*run)(struct app_runtime_t *app);
    void (*quit)(struct app_runtime_t *app);

    void (*parse_options)(struct app_runtime_t *app, Options *options);
    void (*save_options)(struct app_runtime_t *app, Options *options);

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
#define RUNTIME_LOOP_FORGROUND  0
#define RUNTIME_LOOP_THREAD     1

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
