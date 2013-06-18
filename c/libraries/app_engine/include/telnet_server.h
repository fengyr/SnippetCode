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

    int (*init)(struct telnet_server_t*, const char*, int);
    void (*run)(struct telnet_server_t*, int);
    void (*quit)(struct telnet_server_t*);
};
typedef struct telnet_server_t TelnetServer, *PTelnetServer;

TelnetServer* create_telnet_server_instance();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _telnet_server_H_ */
