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
#include "appe_db_sqlite_wrap.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

/* <dev> */
#include "hw_module.h"

/* <modbus> */
#include "modbus.h"
#include "appe_modbus_master.h"
#include "modbus_rtu.h"
#include "modbus_tcp.h"

/* <db> */
#include "db_column.h"
#include "db_mysql_wrap.h"

/* <net> */
#include "appe_socket_in.h"
#include "appe_socket_handler.h"
#include "appe_tcp_server.h"
#include "appe_tcp_slave.h"
#include "appe_telnet_server.h"

/* <utils> */
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
#include "gtksdl.h"

/* <core> */
#include "appe_debug.h"
#include "appe_looper.h"
#include "appe_message.h"
#include "appe_version.h"

/* <interface> */
#include "appe_options_in.h"

struct app_runtime_t {
    ////////////////////////////////
    ///         内部接口         ///
    ////////////////////////////////
    // 消息机制
    MessageQueue *msg_queue;
    AppeLooper *looper;
    void (*register_message_handler)(HandlerMessage handler, int thread_mode);

    // 初始化和销毁
    void (*init)(struct app_runtime_t *app);
    void (*run)(struct app_runtime_t *app);
    void (*quit)(struct app_runtime_t *app);

    // 命令行解析
    void (*parse_options)(struct app_runtime_t *app, Options *options);
    void (*save_options)(struct app_runtime_t *app, Options *options, char *config_file_path);

    ////////////////////////////////
    ///         外部接口         ///
    ////////////////////////////////
    // 命令行参数
    Options *options;

    // 网络接口
    AppeTcpServerGroups *tcp_server_groups;
    AppeTcpSlaveGroups *tcp_slave_groups;
    
    // 线程管理
    TaskManager *task_manager;

    // 日志记录
    Logger *logger;

    // 版本信息
    const char* (*version)(const char *min_ver, const char *mac_ver);

    ////////////////////////////////
    ///         框架接口         ///
    ////////////////////////////////
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
App* appe_create_app_instance(int argc, const char *argv[]);
// 获取已创建的app实例
App* appe_get_app_instance();

// 传送消息
int appe_trans_message(Message *msg);
// 获取程序开始运行的时间
struct timeval* get_boot_time();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _MainApp_H_ */
