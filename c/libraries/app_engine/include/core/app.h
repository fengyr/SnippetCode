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

#include "message.h"
#include "message_queue.h"
#include "camera_server.h"
#include "telnet_server.h"
#include "looper.h"
#include "options.h"

struct app_runtime_t {
    Options *options;

    MessageQueue *msg_queue;
    Looper *looper;

    CameraServer *camera_server;
    TelnetServer *telnet_server;

    // init && destory
    void (*init)(struct app_runtime_t *app);
    void (*register_handler)(HandlerMessage handler, int thread_mode);
    void (*run)(struct app_runtime_t *app);
    void (*quit)(struct app_runtime_t *app);

    // app engine version
    const char* (*version)();

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
App* create_app_instance(Options *options);
// 获取已创建的app实例
App* get_app_instance();

// 传送消息
int trans_message(Message *msg);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _MainApp_H_ */
