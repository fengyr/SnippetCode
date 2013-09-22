/*
 * =====================================================================================
 *
 *       Filename:  telnet_server.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 10, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _telnet_server_H_
#define _telnet_server_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "connection.h"

struct telnet_server_t {
    Socket *sock;

    int (*init)(struct telnet_server_t*, const char*, const char*, int);
    void (*run)(struct telnet_server_t*, int);
    void (*quit)(struct telnet_server_t*);
    void (*register_event_handler)(struct telnet_server_t *server, EventHandler *handler);
};
typedef struct telnet_server_t TelnetServer, *PTelnetServer;

#define TELNET_BUF_SIZE     1024    // 等于MSG_MAX

struct telnet_proc_t {
    char telnet_cmd[TELNET_BUF_SIZE];
    int (*telnet_proc_handler)(int fd, char *msg, Socket *sock);
};
typedef struct telnet_proc_t TelnetProc, *PTelnetProc;

TelnetServer* create_telnet_server_instance();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _telnet_server_H_ */
