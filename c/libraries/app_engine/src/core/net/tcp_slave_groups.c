/*
 * =====================================================================================
 *
 *       Filename:  tcp_slave_groups.c
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
 *  注册一个客户端的流程:
 *  1.通过register_slave创建一个客户端对象.             <tcp_slave.h>,<tcp_slave_groups.c>
 *  2.通过get_slave获取已注册的客户端对象.              <tcp_slave.h>,<tcp_slave_groups.c>
 *  3.调用客户端对象的connect、send、disconnect方法.    <tcp_slave.h>,<tcp_slave.c>
 * =====================================================================================
 */
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tcp_slave.h"
#include "debug.h"
#include "zlogwrap.h"
#include "app.h"

static TcpSlaveGroups s_tcp_slave_groups;

static TcpSlave* __get_slave(struct tcp_slave_groups_t *groups, 
                           const char *slave_name)
{
    int error;
    TcpSlave *slave;
    App *s_app = get_app_instance();
    Logger *logger = s_app->logger;

    error = hashmap_get(groups->hashmap_slave_groups, (char*)slave_name, (void**)(&slave));
    if (error != MAP_OK) {
        logger->log_e(logger, "Net: Get Slave Error.");
    }

    return slave;
}

static int __tcp_slave_groups_register(TcpSlaveGroups *groups, 
                              const char *slave_name, 
                              const char *server_ip, 
                              int server_port)
{
    App *s_app = get_app_instance();
    Logger *logger = s_app->logger;
    
    if (groups->slave_count >= MAX_SLAVES) {
        logger->log_e(logger, "Net: Slave Register, Groups Is Full.");
        return -1; 
    }

    if (!strcmp(slave_name, "")) {
        logger->log_e(logger, "Net: Slave Register, Slave Name Is NULL.");
        return -1;
    }

    // init TcpSlave
    TcpSlave *slave;
    slave = (TcpSlave*) malloc(sizeof(TcpSlave));

    memset(slave->slave_name, 0, sizeof(slave->slave_name));
    strcpy(slave->slave_name, slave_name);
    memset(slave->server_ip, 0, sizeof(slave->server_ip));
    strcpy(slave->server_ip, server_ip);
    slave->server_port = server_port;
    slave->connect = slave_tcp_connect;
    slave->disconnect = slave_tcp_disconnect;
    slave->send = slave_tcp_send;
    slave->recv = slave_tcp_recv;
    slave->register_recv_handler = slave_register_handler;
    slave->status = ENUM_TCP_DISCONNECTED;

    if (groups->slave_names[groups->slave_count] != NULL) {
        strcpy(groups->slave_names[groups->slave_count], slave_name);
    } else {
        /* error = hashmap_remove(groups->hashmap_slave_groups, (char*)slave_name); */
        logger->log_e(logger, "Net: Slave Register, Slave Add Name Failed.");
        return -1;
    }

    int error = hashmap_put(groups->hashmap_slave_groups, groups->slave_names[groups->slave_count], slave);
    if (error != MAP_OK) {
        memset(groups->slave_names[groups->slave_count], 0, 256);
        logger->log_e(logger, "Net: Slave Register, Slave Add Failed.");
        return -1;
    }

    DEBUG("tcp_slave_groups_register: slave_name=%s\n", slave->slave_name);

    groups->slave_count++;

    return 0;
}

static int __tcp_slave_groups_init(TcpSlaveGroups *groups)
{
    groups->hashmap_slave_groups = hashmap_new();

    int i;
    for (i = 0; i < MAX_SLAVES; i++) {
        groups->slave_names[i] = (char*)malloc(256*sizeof(char));
        memset(groups->slave_names[i], 0, 256*sizeof(char));
    }

    groups->slave_count = 0;

    return 0;
}

static int __tcp_slave_groups_destroy(TcpSlaveGroups *groups)
{
    int error;
    TcpSlave *slave = NULL;
    App *s_app = get_app_instance();
    Logger *logger = s_app->logger;

    DEBUG("tcp_slave_groups_destroy: BEGIN\n");
    int i;
    for (i = 0; i < MAX_SLAVES; i++) {
        error = hashmap_get(groups->hashmap_slave_groups, groups->slave_names[i], (void**)(&slave));
        if (slave != NULL) {
            DEBUG("tcp_slave_groups_destroy: slave_name=%s\n", slave->slave_name);
            slave_tcp_close(slave, 0);
            free(slave);
            slave = NULL;

            error = hashmap_remove(groups->hashmap_slave_groups, groups->slave_names[i]);
            if (error != MAP_OK) {
                logger->log_e(logger, "Net: Groups Free, Slave Remove Error.");
            }
        }

        free(groups->slave_names[i]);
        groups->slave_names[i] = NULL;
    }

    groups->slave_count = 0;

    DEBUG("tcp_slave_groups_destroy: OK\n");

    hashmap_free(groups->hashmap_slave_groups);

    return 0;
}

TcpSlaveGroups* create_tcp_slave_groups_instance()
{
    s_tcp_slave_groups.init = __tcp_slave_groups_init;
    s_tcp_slave_groups.destroy= __tcp_slave_groups_destroy;
    s_tcp_slave_groups.get_slave = __get_slave;
    s_tcp_slave_groups.register_slave = __tcp_slave_groups_register;

    return &s_tcp_slave_groups;
}
