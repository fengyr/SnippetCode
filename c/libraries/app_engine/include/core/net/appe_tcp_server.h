/*
 * =====================================================================================
 *
 *       Filename:  tcp_server.h
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
#ifndef _appe_tcp_server_H_
#define _appe_tcp_server_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "appe_socket_in.h"
#include "hashmap.h"

struct tcp_server_t {
    AppeSocket *sock;

    int (*init)(struct tcp_server_t*, const char*, const char*, int);
    void (*run)(struct tcp_server_t*, int);
    void (*quit)(struct tcp_server_t*);
    void (*register_event_handler)(struct tcp_server_t *server, AppeEventHandler *handler);
};
typedef struct tcp_server_t AppeTcpServer, *PAppeTcpServer;

AppeTcpServer* appe_create_tcp_server_instance();

//////////////////////////////////////////////////////
//                  Server Groups                   //
//////////////////////////////////////////////////////
#define MAX_SERVERS  128

typedef void* AppeAnyServer;

enum tcp_server_type_t {
    ENUM_SERVER_NODEFINED = -1,
    ENUM_SERVER_TCP_ASCII = 0,
    ENUM_SERVER_TELNET = 1,
};

struct tcp_server_groups_t {
    HashMap hashmap_server_groups;
    char *server_names[MAX_SERVERS];
    enum tcp_server_type_t server_types[MAX_SERVERS];
    int server_count;

    int (*register_server)(struct tcp_server_groups_t *groups, 
                           enum tcp_server_type_t server_type,
                           const char *server_name, 
                           const char *server_ip, 
                           int server_port);
    AppeAnyServer (*get_server)(struct tcp_server_groups_t *groups, 
                           const char *server_name);
    int (*init)(struct tcp_server_groups_t *groups); 
    int (*destroy)(struct tcp_server_groups_t *groups);
};
typedef struct tcp_server_groups_t AppeTcpServerGroups, *PAppeTcpServerGroups;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
AppeTcpServerGroups* appe_create_tcp_server_groups_instance();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _appe_tcp_server_H_ */
