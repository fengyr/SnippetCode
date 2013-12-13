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

#define SQL_CREATE_TABLE        "create table %s %s;"
#define SQL_CREATE_DATABASE     "create database if not exists %s;"
#define SQL_QUERY_FORMAT        "select %s from %s;"
#define SQL_QUERY_COND_FORMAT   "select %s from %s where %s;"

struct db_mysql_user_t {
    char db_host[24];           // mysql的ip地址
    char db_user[24];           // mysql的用户名
    char db_passwd[64];         // mysql的密码
    char db_name[64];           // 数据库名称
    unsigned int db_port;       // 数据库端口
    char db_socket[64];         // socket名称
    unsigned long db_flags;     // 标识
};
typedef struct db_mysql_user_t MysqlUser, *PMysqlUser;

struct db_mysql_client_t {
    MYSQL *db_conn_ptr;    
    MysqlUser *db_user_info;
    ContentTableGroups *db_table_groups;

    int (*add_table)(struct db_mysql_client_t *client, 
                     ContentTable *table, 
                     int create_table);

    int (*query_table_cond)(struct db_mysql_client_t *client, 
                            ContentTable *table, 
                            const char *args, 
                            const char *cond,
                            MysqlQueryHandler handler);

    int (*query_table)(struct db_mysql_client_t *client, 
                       ContentTable *table, 
                       const char *args, 
                       MysqlQueryHandler handler);

    int (*exec_sql)(struct db_mysql_client_t *client, 
                    const char *sql,
                    MysqlExecHandler handler);
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

/* 
 * ====================================================
 *                     执行数据库操作
 * ====================================================
 *  */
/**
 * @Synopsis 加入一个表到真正的数据库中
 *
 * @Param client        客户端对象指针
 * @Param table_name    表名
 * @Param table         抽象表指针
 * @Param create_table  是否创建该表
 *
 * @Returns             成功返回0，失败返回参考mysqlclient。
 */
int db_mysql_add_table(MysqlClient *client, 
                       ContentTable *table, 
                       int create_table);

/**
 * @Synopsis 查询数据库操作
 *
 * @Param client        客户端对象指针
 * @Param table         抽象表指针
 * @Param args          查询参数
 * @Param cond          查询条件
 *
 * @Returns             成功返回0，失败返回参考mysqlclient。
 */
int db_mysql_query_table_cond(MysqlClient *client, 
                              ContentTable *table, 
                              const char *args, 
                              const char *cond,
                              MysqlQueryHandler handler);

/**
 * @Synopsis 根据给定条件查询数据库
 *
 * @Param client        客户端对象指针
 * @Param table         抽象表指针
 * @Param args          查询参数
 *
 * @Returns             成功返回0，失败返回参考mysqlclient
 */
int db_mysql_query_table(MysqlClient *client, 
                         ContentTable *table, 
                         const char *args, 
                         MysqlQueryHandler handler);

/**
 * @Synopsis 执行自定义的sql语句
 *
 * @Param client        客户端对象指针
 * @Param table         抽象表指针
 * @Param statement     sql语句 
 * @Param handler       执行回调处理
 *
 * @Returns             成功返回0，失败返回参考mysqlclient
 */
int db_mysql_exec_sql(MysqlClient *client, 
                      const char *sql, 
                      MysqlExecHandler handler);

#ifdef __cplusplus
}
#endif

#endif

#endif /* end of include guard: _mysql_wrap_H_ */
