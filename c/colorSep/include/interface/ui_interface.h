/*
 * =====================================================================================
 *
 *       Filename:  ui_interface.h
 *
 *    Description:  
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

#include "connection.h"

struct cmd_ui_t {
    int cmd_id;
    char cmd_msg[128];
};
typedef struct cmd_ui_t CmdUi, *PCmdUi;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
int ui_control_handler(int fd, Socket *sock);
int ui_data_handler(int fd, Socket *sock);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _cmd_ui_handler_H_ */
