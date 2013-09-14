/*
 * =====================================================================================
 *
 *       Filename:  mysql_wrap.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  九月 06, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _mysql_wrap_H_
#define _mysql_wrap_H_

#ifdef USE_MYSQL

#ifdef __cplusplus
extern "C" {
#endif

#include "db_column.h"

#include "mysql.h"

#define SQL_CREATE_TABLE        "create table"
#define SQL_CREATE_DATABASE     "create database"

struct db_mysql_user_t {
    char db_host[24];
    char db_user[24];
    char db_passwd[64];
    char db_name[64];
    unsigned int db_port;
    char db_socket[64];
    unsigned long db_flags;
};
typedef struct db_mysql_user_t MysqlUser, *PMysqlUser;

struct db_mysql_client_t {
    MYSQL *db_conn_ptr;    
    MysqlUser *db_user_info;
    ContentTableGroups *db_table_groups;

    int (*add_table)(struct db_mysql_client_t *client, 
                     const char *table_name, 
                     ContentTable *table, 
                     int create_table);
};
typedef struct db_mysql_client_t MysqlClient, *PMysqlClient;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
/**
 * @Synopsis 初始化Mysql客户端对象
 *
 * @Param user          Mysql用户信息
 * @Param create_db     是否创建指定的数据库
 *
 * @Returns 
 */
MysqlClient* db_mysql_init(MysqlUser *user, int create_db);

/**
 * @Synopsis 释放Mysql客户端对象
 *
 * @Param client    Mysql客户端对象指针
 */
void db_mysql_free(MysqlClient *client);

/**
 * @Synopsis 加入一个表到真正的数据库中
 *
 * @Param client        客户端对象指针
 * @Param table_name    表名
 * @Param table         抽象表指针
 * @Param create_table  是否创建该表
 *
 * @Returns 
 */
int db_mysql_add_table(MysqlClient *client, const char *table_name, ContentTable *table, int create_table);

#ifdef __cplusplus
}
#endif

#endif

#endif /* end of include guard: _mysql_wrap_H_ */
