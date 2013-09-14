/*
 * =====================================================================================
 *
 *       Filename:  db_mysql_wrap.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  九月 08, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>

#ifdef USE_MYSQL

#include "db_mysql_wrap.h"
#include "zlogwrap.h"
#include "debug.h"
#include "app.h"

static App *s_app = get_app_instance();

MysqlClient* db_mysql_init(MysqlUser *user, int create_db)
{
    Logger *logger = s_app->logger;
    char err[128], sql[64];

    MysqlClient *client = (MysqlClient*) malloc(sizeof(MysqlClient));

    client->db_conn_ptr = mysql_init(NULL);
    if (!client->db_conn_ptr) {
        memset(err, 0, sizeof(err));
        sprintf(err, "DB: Mysql Init Failed, %s", mysql_error(client->db_conn_ptr));
        logger->log_e(logger, err);
        goto DB_ERROR;
    }

    client->db_conn_ptr = mysql_real_connect(client->db_conn_ptr, 
                                             user->db_host, 
                                             user->db_user, 
                                             user->db_passwd, 
                                             NULL, 
                                             user->db_port, 
                                             user->db_socket, 
                                             user->db_flags);
    if (client->db_conn_ptr) {
        logger->log_e(logger, "DB: Mysql Connect Success.");
    } else {
        memset(err, 0, sizeof(err));
        sprintf(err, "DB: Mysql Connect Failed, %s", mysql_error(client->db_conn_ptr));
        logger->log_e(logger, err);
        goto DB_ERROR;
    }

    if (create_db) {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "%s if not exists %s;", SQL_CREATE_DATABASE, user->db_name);
        if( mysql_query(client->db_conn_ptr, sql) < 0) {
            memset(err, 0, sizeof(err));
            sprintf(err, "DB: Mysql Create DB '%s' Failed, %s", user->db_name, mysql_error(client->db_conn_ptr));
            logger->log_e(logger, err);
        }
    }

    memset(err, 0, sizeof(err));
    if (mysql_select_db(client->db_conn_ptr, user->db_name) < 0) {
        sprintf(err, "DB: Mysql Select DB '%s' Failed, %s", user->db_name, mysql_error(client->db_conn_ptr));
        logger->log_e(logger, err);
    } else {
        sprintf(err, "DB: Mysql Use DB '%s' Success.", user->db_name);
        logger->log_e(logger, err);
    }

    // 拷贝用户信息
    client->db_user_info = (MysqlUser*) malloc(sizeof(MysqlUser));
    memset(client->db_user_info->db_host, 0, sizeof(client->db_user_info->db_host));
    strcpy(client->db_user_info->db_host, user->db_host);
    memset(client->db_user_info->db_user, 0, sizeof(client->db_user_info->db_user));
    strcpy(client->db_user_info->db_user, user->db_user);
    memset(client->db_user_info->db_passwd, 0, sizeof(client->db_user_info->db_passwd));
    strcpy(client->db_user_info->db_passwd, user->db_passwd);
    memset(client->db_user_info->db_name, 0, sizeof(client->db_user_info->db_name));
    strcpy(client->db_user_info->db_name, user->db_name);
    client->db_user_info->db_port = user->db_port;
    memset(client->db_user_info->db_socket, 0, sizeof(client->db_user_info->db_socket));
    strcpy(client->db_user_info->db_socket, user->db_socket); 
    client->db_user_info->db_flags = user->db_flags;

    // 创建抽象数据库
    client->db_table_groups = db_create_table_groups_instance();
    // 设置方法指针
    client->add_table = db_mysql_add_table;

    return client;

DB_ERROR:

    db_mysql_free(client);

    return NULL;
}

void db_mysql_free(MysqlClient *client)
{
    Logger *logger = s_app->logger;
    char err[128];

    if (!client) {
        return;
    }

    if (client->db_conn_ptr != NULL) {
        mysql_close(client->db_conn_ptr);
        sprintf(err, "DB: Mysql Close DB '%s' Success.", client->db_user_info->db_name);
        logger->log_e(logger, err);
    }

    if (client->db_user_info != NULL) {
        free(client->db_user_info);
        client->db_user_info = NULL;
    }

    if (client->db_table_groups != NULL) {
        db_destroy_table_groups(client->db_table_groups);
    }

    free(client);
    client = NULL;
}

static char* make_table_to_str(ContentTable *table, char *table_desc)
{
    int nr = table->columns.nr;

    int i;
    strcat(table_desc, "(");
    for (i = 0; i < nr; i++) {
        ContentColumn *col = (ContentColumn*) table->columns.objects[i].item;
        // 列名
        strcat(table_desc, col->cont_name);
        strcat(table_desc, " ");
        // 类型
        const char *type_desc = db_get_column_type_str(col->cont_type);
        strcat(table_desc, type_desc);
        // 是否为空
        if (strcmp(col->cont_null, "") != 0) {
            strcat(table_desc, " ");
            strcat(table_desc, col->cont_null);
        }
        // key
        if (strcmp(col->cont_key, "") != 0) {
            strcat(table_desc, " ");
            strcat(table_desc, col->cont_key);
        }
        // extra
        if (strcmp(col->cont_extra, "") != 0) {
            strcat(table_desc, " ");
            strcat(table_desc, col->cont_extra);
        }
        // ,号
        if (i != (nr - 1)) {
            strcat(table_desc, ", ");
        }
    }
    strcat(table_desc, ");");

    return table_desc;
}

int db_mysql_add_table(MysqlClient *client, 
                       const char *table_name, 
                       ContentTable *table, 
                       int create_table)
{
    Logger *logger = s_app->logger;
    char err[128];
    char sql[255];
    MYSQL_RES *res;

    if (NULL == client) {
        logger->log_e(logger, "DB: Mysql Add Table, Client Is NULL.");
        return -1;
    }

    db_register_table(client->db_table_groups, table_name, table);

    if (create_table) {
        char table_desc[256];
        memset(table_desc, 0, sizeof(table_desc));
        make_table_to_str(table, table_desc);

        memset(sql, 0, sizeof(sql));
        sprintf(sql, "%s %s %s", SQL_CREATE_TABLE, table->table_name, table_desc);
        DEBUG("db_mysql_add_table: <%s>\n", sql);

        int rtn = mysql_query(client->db_conn_ptr, sql); 
        memset(err, 0, sizeof(err));
        sprintf(err, "DB: Mysql Create Table '%s' %s, %s", 
                table->table_name, 
                (rtn ? "Failed" : "Success"), 
                mysql_error(client->db_conn_ptr));
        logger->log_e(logger, err);

        res = mysql_use_result(client->db_conn_ptr);
        mysql_free_result(res);
    }

    return 0;
}

#endif
