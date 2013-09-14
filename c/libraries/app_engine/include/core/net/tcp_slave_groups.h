/*
 * =====================================================================================
 *
 *       Filename:  tcp_slave_groups.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  八月 24, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _tcp_slave_groups_H_
#define _tcp_slave_groups_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hashmap.h"
#include "tcp_slave.h"

#define MAX_SLAVES  16

struct tcp_slave_groups_t {
    HashMap hashmap_slave_groups;
    char *slave_names[MAX_SLAVES];
    int slave_count;

    int (*register_slave)(struct tcp_slave_groups_t *groups, 
                          const char *slave_name, 
                          const char *server_ip, 
                          int server_port);
    TcpSlave* (*get_slave)(struct tcp_slave_groups_t *groups, 
                           const char *slave_name);
    int (*init)(struct tcp_slave_groups_t *groups); 
    int (*destroy)(struct tcp_slave_groups_t *groups);
};
typedef struct tcp_slave_groups_t TcpSlaveGroups, *PTcpSlaveGroups;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
TcpSlaveGroups* create_tcp_slave_groups_instance();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _tcp_slave_groups_H_ */
