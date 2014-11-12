/*
 * =====================================================================================
 *
 *       Filename:  db_sqlite_wrap.hpp
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十一月 09, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _db_sqlite_wrap_H_
#define _db_sqlite_wrap_H_

#include "SQLiteCpp.h"

#define MY_SQLITE_CREATE_TABLE          "create table %s %s;"
#define MY_SQLITE_CREATE_DATABASE       "create database if not exists %s;"
#define MY_SQLITE_QUERY_FORMAT          "select %s from %s;"
#define MY_SQLITE_QUERY_COND_FORMAT     "select %s from %s where %s;"

#define SQL_OPEN_RO     SQLITE_OPEN_READONLY
#define SQL_OPEN_RW     SQLITE_OPEN_READWRITE
#define SQL_OPEN_CR     SQLITE_OPEN_CREATE

typedef SQLite::Column      SqlResult;
typedef SQLite::Statement   SqlQuery;
typedef SQLite::Database    SqlDatabase;
typedef SQLite::Transaction SqlTransaction;

typedef void (*QUERY_BIND)(SqlQuery &query, void *bind_data);
typedef void (*QUERY_GET)(SqlQuery &query, void *get_data);

struct db_sqlite_client_t {
    SqlDatabase     *sqlite_db;
    SqlTransaction  *sqlite_transaction;

    int (*open_db)(struct db_sqlite_client_t *client, 
                    const char *db_file_name, 
                    const int db_open_flags);

    int (*close_db)(struct db_sqlite_client_t *client);

    int (*table_exists)(struct db_sqlite_client_t *client,
                        const char *table_name);

    SqlResult (*exec_get)(struct db_sqlite_client_t *client, 
                        const char *statement);

    int (*query_cond)(struct db_sqlite_client_t *client, 
                        const char *statement,
                        QUERY_BIND query_bind_func,
                        void *bind_data,
                        QUERY_GET query_get_func,
                        void *get_data);

    int (*exec_sql)(struct db_sqlite_client_t *client, 
                    const char *statement);

    int (*transaction_begin)(struct db_sqlite_client_t *client);
    int (*transaction_end)(struct db_sqlite_client_t *client);

    void (*destory)(struct db_sqlite_client_t *client);
};
typedef struct db_sqlite_client_t SqliteClient, *PSqliteClient;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////

SqliteClient* create_sqlite_client_instance();


#endif /* end of include guard: _db_sqlite_wrap_H_ */
