/*
 * =====================================================================================
 *
 *       Filename:  ui_interface.h
 *
 *    Description:  网络通讯接口
 *
 *        Version:  
 *        Created:  五月 06, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _cmd_ui_handler_H_
#define _cmd_ui_handler_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "appe_socket_in.h"

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
/* 以下方法实现了EventHandlerCall接口，用来处理不同的网络协议 */
int ui_control_handler(int fd, AppeSocket *sock);
int ui_img_data_handler(int fd, AppeSocket *sock);
int mobile_handler(int fd, AppeSocket *sock);
int modbus_protocol_handler(int fd, AppeSocket *sock);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _cmd_ui_handler_H_ */
