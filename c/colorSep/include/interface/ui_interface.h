/*
 * =====================================================================================
 *
 *       Filename:  cmd_ui_handler.h
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

struct cmd_ui_t {
    int cmd_id;
    char cmd_msg[128];
};
typedef struct cmd_ui_t CmdUi, *PCmdUi;

int cmd_ui_handler(int fd);

#endif /* end of include guard: _cmd_ui_handler_H_ */
