/*
 * =====================================================================================
 *
 *       Filename:  db_sqlite_wrap.cpp
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
#include <stdlib.h>

#include "db_sqlite_wrap.hpp"
#include "debug.h"

static SqliteClient *s_sqlite_client = NULL; 

static int __open_db(struct db_sqlite_client_t *client, 
                        const char *db_file_name, 
                        const int db_open_flags)
{
    try 
    {
        client->sqlite_db = new SqlDatabase(db_file_name, db_open_flags);
    }
    catch (std::exception &e) 
    {
        return -1;
    }

    return 0;
}

static int __close_db(struct db_sqlite_client_t *client)
{
    if (client->sqlite_db) {
        delete client->sqlite_db;
        client->sqlite_db = NULL;
    }

    if (client->sqlite_transaction) {
        delete client->sqlite_transaction;
        client->sqlite_transaction = NULL;
    }

    return 0;
}

static int __table_exists(struct db_sqlite_client_t *client,
                            const char *table_name)
{
    if (!client || !client->sqlite_db) {
        return -2;
    }

    try 
    {
        bool exists = client->sqlite_db->tableExists(table_name);
        if (exists) {
            return 1;
        }
    }
    catch (std::exception &e) 
    {
        return 0;
    }

    return 0;
}

static SqlResult __exec_get(struct db_sqlite_client_t *client, 
                                    const char *statement)
{
    if (!client || !client->sqlite_db) {
    }

    try 
    {
        return client->sqlite_db->execAndGet(statement);
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

static int __query_cond(struct db_sqlite_client_t *client, 
                        const char *statement,
                        QUERY_BIND query_bind_func,
                        void *bind_data,
                        QUERY_GET query_get_func,
                        void *get_data)
{
    if (!client || !client->sqlite_db) {
        return -1;
    }

    try 
    {
        SqlQuery query(*(client->sqlite_db), statement);

        if (query_bind_func) {
            query_bind_func(query, bind_data); 
        }

        if (query_get_func) {
            while (query.executeStep()) {
                query_get_func(query, get_data);
            }
        }

        query.reset();
    }
    catch (std::exception &e) 
    {
        return -1;
    }

    return 0;
}

static int __exec_sql(struct db_sqlite_client_t *client, 
                        const char *statement)
{
    int nb = 0;

    if (!client || !client->sqlite_db) {
        return -1;
    }

    try 
    {
        nb = client->sqlite_db->exec(statement);
    }
    catch (std::exception &e) 
    {
        return -1;
    }

    return nb;
}

static int __transaction_begin(struct db_sqlite_client_t *client)
{
    if (!client || !client->sqlite_db) {
        return -1;
    }

    try 
    {
        client->sqlite_transaction = new SqlTransaction(*(client->sqlite_db));
    }
    catch (std::exception &e) 
    {
        return -1;
    }

    return 0;
}

static int __transaction_end(struct db_sqlite_client_t *client)
{
    if (!client || !client->sqlite_db || !client->sqlite_transaction) {
        return -1;
    }

    try 
    {
        client->sqlite_transaction->commit();
    }
    catch (std::exception &e) 
    {
        return -1;
    }

    delete client->sqlite_transaction;
    client->sqlite_transaction = NULL;

    return 0;
}

static void __destory(struct db_sqlite_client_t *client)
{
    if (client->sqlite_db) {
        delete client->sqlite_db;
        client->sqlite_db = NULL;
    }

    if (client->sqlite_transaction) {
        delete client->sqlite_transaction;
        client->sqlite_transaction = NULL;
    }

    if (client != NULL) {
        free(client);
        client = NULL;
    }
}

SqliteClient* create_sqlite_client_instance()
{
    if (!s_sqlite_client) {
        s_sqlite_client = (SqliteClient*) malloc(sizeof(SqliteClient));    
        s_sqlite_client->sqlite_db = NULL;
        s_sqlite_client->sqlite_transaction = NULL;

        s_sqlite_client->open_db = __open_db;
        s_sqlite_client->close_db = __close_db;
        s_sqlite_client->table_exists = __table_exists;
        s_sqlite_client->exec_get = __exec_get;
        s_sqlite_client->query_cond = __query_cond;
        s_sqlite_client->exec_sql = __exec_sql;
        s_sqlite_client->transaction_begin = __transaction_begin;
        s_sqlite_client->transaction_end = __transaction_end;
        s_sqlite_client->destory = __destory;
    }

    return s_sqlite_client;
}
