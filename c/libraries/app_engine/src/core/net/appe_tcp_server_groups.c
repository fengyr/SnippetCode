/*
 * =====================================================================================
 *
 *       Filename:  tcp_server_groups.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  九月 13, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 *  注册新服务的流程：
 *  1.创建新的服务抽象类型，参考tcp_server和telnet_server。     <tcp_server.h>,<telnet_server.h>
 *  2.实现新的服务抽象类型.                                     <tcp_server.c>, <telnet_server.c>
 *  3.为每种抽象服务类型定义标识enum tcp_server_type_t.         <tcp_server.h>
 *  4.通过register_server方法注册一个服务.                      <runtime.c>
 *  5.通过get_server方法获取一个服务.                           <runtime.c>
 * =====================================================================================
 */
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "appe_tcp_server.h"
#include "appe_telnet_server.h"
#include "appe_debug.h"
#include "zlogwrap.h"
#include "appe_app.h"

static AppeTcpServerGroups s_tcp_server_groups;

static AppeAnyServer __get_server(struct tcp_server_groups_t *groups, 
                             const char *server_name)
{
    int error;
    App *s_app = appe_get_app_instance();
    Logger *logger = s_app->logger;
    enum tcp_server_type_t server_type = ENUM_SERVER_NODEFINED;

    int i;
    for (i = 0; i < groups->server_count; i++) {
        if (strcmp(groups->server_names[i], server_name) == 0) {
            server_type = groups->server_types[i];    
            break;
        }
    }

    if (server_type == ENUM_SERVER_TCP_ASCII) {
        AppeTcpServer *tcp_server;
        error = hashmap_get(groups->hashmap_server_groups, (char*)server_name, (void**)(&tcp_server));
        return (AppeAnyServer*) tcp_server;
    } else if (server_type == ENUM_SERVER_TELNET) {
        AppeTelnetServer *telnet_server;
        error = hashmap_get(groups->hashmap_server_groups, (char*)server_name, (void**)(&telnet_server));
        return (AppeAnyServer*) telnet_server;
    } else {
        logger->log_e(logger, "Net: Get Server NO Defined.");
        return NULL;
    }

    if (error != MAP_OK) {
        logger->log_e(logger, "Net: Get Server Error.");
    }

    return NULL;
}

static int __tcp_server_groups_register(AppeTcpServerGroups *groups, 
                                      enum tcp_server_type_t server_type,
                                      const char *server_name, 
                                      const char *server_ip, 
                                      int server_port)
{
    App *s_app = appe_get_app_instance();
    Logger *logger = s_app->logger;

    if (groups->server_count >= MAX_SERVERS) {
        logger->log_e(logger, "Net: Server Register, Groups Is Full.");
        return -1; 
    }

    if (!strcmp(server_name, "")) {
        logger->log_e(logger, "Net: Server Register, Server Name Is NULL.");
        return -1;
    }

    int error;
    if (server_type == ENUM_SERVER_TCP_ASCII) {
        // create tcp server instance
        AppeTcpServer *tcp_server = appe_create_tcp_server_instance();
        tcp_server->init(tcp_server, server_name, server_ip, server_port);
        tcp_server->run(tcp_server, 1);

        if (groups->server_names[groups->server_count] != NULL) {
            strcpy(groups->server_names[groups->server_count], server_name);
        } else {
            /* error = hashmap_remove(groups->hashmap_server_groups, (char*)server_name); */
            logger->log_e(logger, "Net: Server Register, Server Add Name Failed.");
            return -1;
        }

        error = hashmap_put(groups->hashmap_server_groups, groups->server_names[groups->server_count], tcp_server);
        DEBUG("tcp_server_groups_register: server_name=%s\n", tcp_server->sock->local_name);
    } else if (server_type == ENUM_SERVER_TELNET) {
        // create telnet server instance
        AppeTelnetServer *telnet_server = appe_create_telnet_server_instance();
        telnet_server->init(telnet_server, server_name, server_ip, server_port);
        telnet_server->run(telnet_server, 1);

        if (groups->server_names[groups->server_count] != NULL) {
            strcpy(groups->server_names[groups->server_count], server_name);
        } else {
            /* error = hashmap_remove(groups->hashmap_server_groups, (char*)server_name); */
            logger->log_e(logger, "Net: Server Register, Server Add Name Failed.");
            return -1;
        }

        error = hashmap_put(groups->hashmap_server_groups, groups->server_names[groups->server_count], telnet_server);
        DEBUG("tcp_server_groups_register: server_name=%s\n", telnet_server->sock->local_name);
    } else {
        logger->log_e(logger, "Net: Server Register, Server Type No Defined.");
        return -1;
    }

    if (error != MAP_OK) {
        memset(groups->server_names[groups->server_count], 0, 256);
        logger->log_e(logger, "Net: Server Register, Server Add Failed.");
        return -1;
    }

    groups->server_types[groups->server_count] = server_type;
    groups->server_count++;

    return 0;
}

static int __tcp_server_groups_init(AppeTcpServerGroups *groups)
{
    groups->hashmap_server_groups = hashmap_new();

    int i;
    for (i = 0; i < MAX_SERVERS; i++) {
        groups->server_names[i] = (char*)malloc(256);
        memset(groups->server_names[i], 0, 256);
        groups->server_types[i] = ENUM_SERVER_NODEFINED;
    }

    groups->server_count = 0;

    return 0;
}

static int __tcp_server_groups_destroy(AppeTcpServerGroups *groups)
{
    int error;
    App *s_app = appe_get_app_instance();
    Logger *logger = s_app->logger;

    DEBUG("tcp_server_groups_destroy: BEGIN\n");
    int i;
    for (i = 0; i < MAX_SERVERS; i++) {
        if (groups->server_types[i] == ENUM_SERVER_TCP_ASCII) {
            AppeTcpServer *tcp_server;
            error = hashmap_get(groups->hashmap_server_groups, groups->server_names[i], (void**)(&tcp_server));
            if (tcp_server != NULL) {
                DEBUG("tcp_server_groups_destroy: server_name=%s\n", tcp_server->sock->local_name);
                tcp_server->quit(tcp_server);

                free(tcp_server->sock);
                tcp_server->sock = NULL;
                free(tcp_server);
                tcp_server = NULL;

                error = hashmap_remove(groups->hashmap_server_groups, groups->server_names[i]);
                if (error != MAP_OK) {
                    logger->log_e(logger, "Net: Groups Free, Server Remove Error.");
                }
            }    
        } else if (groups->server_types[i] == ENUM_SERVER_TELNET) {
            AppeTelnetServer *telnet_server;
            error = hashmap_get(groups->hashmap_server_groups, groups->server_names[i], (void**)(&telnet_server));
            if (telnet_server != NULL) {
                DEBUG("tcp_server_groups_destroy: server_name=%s\n", telnet_server->sock->local_name);
                telnet_server->quit(telnet_server);

                free(telnet_server->sock);
                telnet_server->sock = NULL;
                free(telnet_server);
                telnet_server = NULL;

                error = hashmap_remove(groups->hashmap_server_groups, groups->server_names[i]);
                if (error != MAP_OK) {
                    logger->log_e(logger, "Net: Groups Free, Server Remove Error.");
                }
            }
        } else {
        }

        free(groups->server_names[i]);
        groups->server_names[i] = NULL;
        groups->server_types[i] = ENUM_SERVER_NODEFINED;
    }

    groups->server_count = 0;

    DEBUG("tcp_server_groups_destroy: OK\n");

    hashmap_free(groups->hashmap_server_groups);

    return 0;
}

AppeTcpServerGroups* appe_create_tcp_server_groups_instance()
{
    s_tcp_server_groups.init = __tcp_server_groups_init;
    s_tcp_server_groups.destroy= __tcp_server_groups_destroy;
    s_tcp_server_groups.get_server = __get_server;
    s_tcp_server_groups.register_server = __tcp_server_groups_register;

    return &s_tcp_server_groups;
}
