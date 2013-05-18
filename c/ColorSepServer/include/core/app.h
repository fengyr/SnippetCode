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

#include "message_queue.h"
#include "looper.h"

struct app_runtime_t {
    MessageQueue *msg_queue;
    Looper *looper;

    void (*init)(struct app_runtime_t *app);
    void (*run)(struct app_runtime_t *app);
    void (*quit)(struct app_runtime_t *app);

    // 用户自定义方法，在App的生命周期中被调用
    int (*onCreate)(struct app_runtime_t *app);
    int (*onDestory)(struct app_runtime_t *app);
};
typedef struct app_runtime_t App, *PApp;

App* getApp();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _MainApp_H_ */
