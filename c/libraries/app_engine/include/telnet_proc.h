/*
 * =====================================================================================
 *
 *       Filename:  telnet_proc.h
 *
 *    Description:  telnet接口
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
#ifndef _telnet_proc_H_
#define _telnet_proc_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "connection.h"

struct telnet_proc_t {
    char telnet_cmd[256];
    int (*telnet_proc_handler)(int fd, char *msg, Socket *sock);
} /* optional variable list */;
typedef struct telnet_proc_t TelnetProc, *PTelnetProc;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
int telnet_handler(int fd, Socket *sock);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _telnet_proc_H_ */
