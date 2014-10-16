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

MysqlClient* db_mysql_init(MysqlUser *user, int create_db)
{
    const App *s_app = get_app_instance();
    Logger *logger = s_app->logger;
    char err[128], sql[64];

    MysqlClient *client = (MysqlClient*) malloc(sizeof(MysqlClient));
    client->db_conn_ptr = NULL;
    client->db_user_info = NULL;
    client->db_table_groups = NULL;

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
        sprintf(sql, SQL_CREATE_DATABASE, user->db_name);
        if(mysql_query(client->db_conn_ptr, sql)) {
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
    // 查询数据库
    client->query_table = db_mysql_query_table;
    // 条件查询数据库
    client->query_table_cond = db_mysql_query_table_cond;
    // 执行sql语句
    client->exec_sql = db_mysql_exec_sql;

    return client;

DB_ERROR:

    db_mysql_free(client);

    return NULL;
}

void db_mysql_free(MysqlClient *client)
{
    const App *s_app = get_app_instance();
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

static char* make_table_desc(ContentTable *table, char *table_desc)
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
                       ContentTable *table, 
                       int create_table)
{
    const App *s_app = get_app_instance();
    Logger *logger = s_app->logger;
    char err[128];
    char sql[255];
    MYSQL_RES *res;

    if (NULL == client) {
        logger->log_e(logger, "DB: Mysql Add Table, Client Is NULL.");
        return -1;
    }

    if (NULL == table) {
        logger->log_e(logger, "DB: Mysql Add Table, Table Is NULL.");
        return -1;
    }

    db_register_table(client->db_table_groups, table->table_name, table);

    int rtn;
    if (create_table) {
        char table_desc[256];
        memset(table_desc, 0, sizeof(table_desc));
        make_table_desc(table, table_desc);

        memset(sql, 0, sizeof(sql));
        sprintf(sql, SQL_CREATE_TABLE, table->table_name, table_desc);
        DEBUG("db_mysql_add_table: <%s>\n", sql);

        rtn = mysql_query(client->db_conn_ptr, sql); 
        memset(err, 0, sizeof(err));
        sprintf(err, "DB: Mysql Create Table '%s' %s, %s", 
                table->table_name, 
                (rtn ? "Failed" : "Success"), 
                mysql_error(client->db_conn_ptr));
        logger->log_e(logger, err);

        res = mysql_use_result(client->db_conn_ptr);
        mysql_free_result(res);
    }

    return rtn;
}

static void process_query_result(MYSQL_RES *res, 
                                 ContentTable *table, 
                                 MysqlQueryHandler handler)
{
    int i;
    int num_rows = mysql_num_rows(res);
    int num_fields = mysql_num_fields(res);

    MYSQL_ROW row;
    MYSQL_FIELD *fields;
    /* unsigned long *lengths; */

    fields = mysql_fetch_fields(res);
    while ((row = mysql_fetch_row(res))) {
        /* lengths = mysql_fetch_lengths(res); */

        if (handler != NULL) {
            ContentColumn *column = NULL;

            for(i = 0; i < num_fields; i++) {
                column = table->get_column(table, fields[i].name);
                if (column != NULL) {
                    db_set_column_val(column, row[i], 1);
                }
            }
            // 重置当前列的游标到初始位置
            table->reset_cursor_pos(table);

            (*handler)(table);
        }
    }

    DEBUG("process_query_result: num_rows=<%d>, num_fields=<%d>\n", num_rows, num_fields);
}

int db_mysql_query_table_cond(MysqlClient *client, 
                              ContentTable *table, 
                              const char *args, 
                              const char *cond,
                              MysqlQueryHandler handler)
{
    const App *s_app = get_app_instance();
    Logger *logger = s_app->logger;
    char err[128];
    char sql[512];
    MYSQL_RES *res;

    if (NULL == client) {
        logger->log_e(logger, "DB: db_mysql_query_table_cond, Client Is NULL.");
        return -1;
    }

    if (NULL == table) {
        logger->log_e(logger, "DB: db_mysql_query_table_cond, Table Is NULL.");
        return -1;
    }

    memset(sql, 0, sizeof(sql));
    if (!strcmp(cond, "")) {
        sprintf(sql, SQL_QUERY_FORMAT, args, table->table_name);
    } else {
        sprintf(sql, SQL_QUERY_COND_FORMAT, args, table->table_name, cond);
    }
    DEBUG("db_mysql_query_table_cond: <%s>\n", sql);

    int rtn;
    rtn = mysql_query(client->db_conn_ptr, sql); 
    memset(err, 0, sizeof(err));
    sprintf(err, "DB: db_mysql_query_table_cond '%s' %s, %s", 
            table->table_name, 
            (rtn ? "Failed" : "Success"), 
            mysql_error(client->db_conn_ptr));
    logger->log_e(logger, err);

    res = mysql_store_result(client->db_conn_ptr);
    if (res) {
        process_query_result(res, table, handler);
    } else {
        memset(err, 0, sizeof(err));
        sprintf(err, "DB: Mysql Store Result Failed '%s', %s", 
            table->table_name, 
            mysql_error(client->db_conn_ptr));
        logger->log_e(logger, err);
        rtn = -1;
    }

    mysql_free_result(res);

    return rtn;
}

int db_mysql_query_table(MysqlClient *client, 
                         ContentTable *table, 
                         const char *args, 
                         MysqlQueryHandler handler)
{
    return db_mysql_query_table_cond(client, table, args, "", handler);
}

int db_mysql_exec_sql(MysqlClient *client,
                      const char *sql,
                      MysqlExecHandler handler)
{
    const App *s_app = get_app_instance();
    Logger *logger = s_app->logger;

    if (NULL == client) {
        logger->log_e(logger, "DB: db_mysql_exec_sql, Client Is NULL.");
        return -1;
    }

    int rtn;
    char err[128];
    MYSQL_RES *res;
    int num_rows = -1;

    rtn = mysql_query(client->db_conn_ptr, sql); 
    memset(err, 0, sizeof(err));
    sprintf(err, "DB: db_mysql_exec_sql <%s> %s, %s", 
            sql, (rtn ? "Failed" : "Success"), 
            mysql_error(client->db_conn_ptr));
    logger->log_e(logger, err);

    res = mysql_store_result(client->db_conn_ptr);

    if (res) {
        (*handler)((void*)res);
    } else {
        if(0 == mysql_field_count(client->db_conn_ptr)) {
            num_rows = mysql_affected_rows(client->db_conn_ptr);
            if (handler != NULL) {
                (*handler)((void*)res);
            }
        } else {
            memset(err, 0, sizeof(err));
            sprintf(err, "DB: Mysql Store Result Failed <%s>, %s", 
                    sql, mysql_error(client->db_conn_ptr));
            logger->log_e(logger, err);
            rtn = -1;
        }
    }

    mysql_free_result(res);

    return rtn;
}

#endif
