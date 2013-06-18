/*
 * =====================================================================================
 *
 *       Filename:  runtime.h
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
#ifndef _runtime_H_
#define _runtime_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app.h"

//////////////////////////////////////////////////////
//          系统消息处理方法                        //
// 说明：                                           //
// 1. 主应用被创建时，会调用on_app_create方法。     //
//    通常在该方法中做一些初始化处理。              //
// 2. 主应用在运行时，会循环调用on_app_process方法。//
//    通常该方法是主线程的工作流程。如果返回值为0,则//
//    该方法被循环调用;否则，只被调用一次。         //
// 3. 主应用退出时，会调用on_app_destroy方法。      //
//    通常在该方法中做一些内存释放的处理。          //
//////////////////////////////////////////////////////
int on_app_create(struct app_runtime_t *app);
int on_app_destroy(struct app_runtime_t *app);
int on_app_process(struct app_runtime_t *app);

//////////////////////////////////////////////////////
//          自定义消息类型和处理函数方法            //
// 说明：                                           //
// 1. 消息ID为0被系统占用，表示主程序开始运行，     //
//    定义为MSG_ON_START                            //
// 2. 消息ID为9999被系统占用，表示退出程序，        //
//    定义为MSG_ON_EXIT                             //
// 3. 如何自定义消息：                              //
//  3.1 定义一个消息值：例如MSG_SEP_1               //
//  3.2 定义相应的消息处理方法：例如on_msg_step_1   //
//  3.3 在runtime.c中实现on_msg_step_1方法          //
//  3.4 在app.c的handler_message方法中添加消息映射  //
//////////////////////////////////////////////////////
#define MSG_SEP_1       1
#define MSG_SEP_2       2

void on_msg_start(MessageHandler *handler, Message *old_msg);
void on_msg_step_1(MessageHandler *handler, Message *old_msg);
void on_msg_step_2(MessageHandler *handler, Message *old_msg);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _runtime_H_ */
