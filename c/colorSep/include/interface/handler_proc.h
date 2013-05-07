/*
 * =====================================================================================
 *
 *       Filename:  handler_proc.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 07, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _handler_proc_H_
#define _handler_proc_H_

struct handler_proc_t {
    int cmd_id;
    int (*proc)(int fd, char *msg);
};
typedef struct handler_proc_t HandlerProc, *PHandlerProc;

#endif /* end of include guard: _handler_proc_H_ */
