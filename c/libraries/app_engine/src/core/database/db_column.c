/*
 * =====================================================================================
 *
 *       Filename:  content.c
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
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "app.h"
#include "db_column.h"
#include "zlogwrap.h"
#include "debug.h"

static App *s_app = get_app_instance();

static char const *s_column_type_str[] = {
    "INTEGER",
    "BIGINT",
    "FLOAT",
    "DOUBLE",
    "DATE",
    "TIME",
    "DATETIME",
    "CHAR(255)",
    "TEXT",
};

ContentColumn* db_create_column(const char *name, 
                                enum content_type_t type, 
                                const char *is_null, 
                                const char *key, 
                                const char *extra, 
                                void *val)
{
    ContentColumn *col;

    col = (ContentColumn*) malloc(sizeof(*col));
    memset(col->cont_name, 0, sizeof(col->cont_name));
    strcpy(col->cont_name, name);
    memset(col->cont_null, 0, sizeof(col->cont_null));
    strcpy(col->cont_null, is_null);
    memset(col->cont_key, 0, sizeof(col->cont_key));
    strcpy(col->cont_key, key);
    memset(col->cont_extra, 0, sizeof(col->cont_extra));
    strcpy(col->cont_extra, extra);
    col->cont_type = type;

    int temp_i, *val_i;
    long long temp_l, *val_l;
    float temp_f, *val_f;
    double temp_d, *val_d;
    char *temp_s, *val_s;
    switch (type) {
        case ENUM_CONTENT_INT:
            temp_i = *((int*)val);
            col->cont_val = (int*) malloc(sizeof(int));
            val_i = (int*)(col->cont_val);
            *val_i = temp_i;
            break;
        case ENUM_CONTENT_BITINT:
            temp_l = *((long long*)val);
            col->cont_val = (long long*) malloc(sizeof(long long));
            val_l = (long long*)(col->cont_val);
            *val_l = temp_l;
            break;
        case ENUM_CONTENT_FLOAT:
            temp_f = *((float*)val);
            col->cont_val = (float*) malloc(sizeof(float));
            val_f = (float*)(col->cont_val);
            *val_f = temp_f;
            break;
        case ENUM_CONTENT_DOUBLE:
            temp_d = *((double*)val);
            col->cont_val = (double*) malloc(sizeof(double));
            val_d = (double*)(col->cont_val);
            *val_d = temp_d;
            break;
        case ENUM_CONTENT_DATE:
        case ENUM_CONTENT_TIME:
        case ENUM_CONTENT_DATETIME:
        case ENUM_CONTENT_TEXT:
        case ENUM_CONTENT_LONGTEXT:
        case ENUM_CONTENT_CHAR:
            temp_s = (char*)val;
            col->cont_val = (char*) malloc(strlen(temp_s)+1);
            val_s = (char*)(col->cont_val);
            strcpy(val_s, temp_s);
            break;
        default:
            col->cont_val = NULL;
            break;
    }

    return col;
}

const char* db_get_column_type_str(enum content_type_t type)
{
    if ((type <= ENUM_CONTENT_START) || 
        (type >= ENUM_CONTENT_END)) {
        return "";
    }

    return s_column_type_str[type];
}

void db_free_column(void *column)
{
    if (!column) {
        return;
    }

    ContentColumn *col = (ContentColumn*) column;
    if (col->cont_val != NULL) {
        free(col->cont_val);
        col->cont_val = NULL;
    }

    free(col);
    col = NULL;
}

ContentTable* db_create_table(const char *name)
{
    ContentTable *table = (ContentTable*) malloc(sizeof(ContentTable));
    assert(table != NULL);

    memset(table->table_name, 0, sizeof(table->table_name));
    strcpy(table->table_name, name);
    array_obj_init(&table->columns, db_free_column);

    // 设置方法指针
    table->create_column = db_create_column;
    table->add_column = db_table_add_column;

    return table;
}

void db_table_add_column(ContentTable *table, ContentColumn *column)
{
    assert(table != NULL);
    assert(column != NULL);
    
    array_obj_add((void*)column, &table->columns);
}

void db_destroy_table(ContentTable *table)
{
    if (NULL == table) {
        return;
    }

    array_obj_destroy(&table->columns);
    free(table);
    table = NULL;
}

void db_destroy_table_groups(ContentTableGroups *groups)
{
    int error;
    ContentTable *table;
    Logger *logger = s_app->logger;

    DEBUG("db_destroy_tables: BEGIN\n");
    int i;
    for (i = 0; i < DB_MAX_TABLES; i++) {
        error = hashmap_get(groups->hashmap_table_groups, groups->table_names[i], (void**)(&table));
        if (table != NULL) {
            DEBUG("db_destroy_tables: table_name=%s\n", table->table_name);
            db_destroy_table(table);

            error = hashmap_remove(groups->hashmap_table_groups, groups->table_names[i]);
            if (error != MAP_OK) {
                logger->log_e(logger, "DB: Groups Free, Table Removed Error.");
            }
        }

        free(groups->table_names[i]);
        groups->table_names[i] = NULL;
    }

    groups->table_count = 0;

    DEBUG("db_destroy_tables: OK\n");

    hashmap_free(groups->hashmap_table_groups);

    free(groups);
    groups = NULL;
}

ContentTableGroups* db_create_table_groups_instance()
{
    ContentTableGroups *groups = (ContentTableGroups*) malloc(sizeof(ContentTableGroups));
    groups->hashmap_table_groups = hashmap_new();
    groups->table_count = 0;

    int i;
    for (i = 0; i < DB_MAX_TABLES; i++) {
        groups->table_names[i] = (char*)malloc(DB_MAX_NAME_LEN);
        memset(groups->table_names[i], 0, DB_MAX_NAME_LEN);
    }

    // 设置方法指针
    groups->create_table = db_create_table;
    groups->register_table = db_register_table;
    groups->get_table = db_get_table;

    return groups;
}

int db_register_table(ContentTableGroups *groups, 
        const char *table_name, 
        ContentTable *table)
{
    assert(groups != NULL);

    Logger *logger = s_app->logger;


    if (groups->table_count >= MAX_SLAVES) {
        logger->log_e(logger, "DB: Register Table, Groups Is Full.");
        return -1; 
    }

    if (!strcmp(table_name, "")) {
        logger->log_e(logger, "DB: Register Table, Table Name Is NULL.");
        return -1;
    }

    if (groups->table_names[groups->table_count] != NULL) {
        strcpy(groups->table_names[groups->table_count], table_name);
    } else {
        logger->log_e(logger, "DB: Register Table, Add Name Failed.");
        return -1;
    }

    int error = hashmap_put(groups->hashmap_table_groups, (char*)table_name, table);
    if (error != MAP_OK) {
        logger->log_e(logger, "DB: Register Table, Add Failed.");
        return -1;
    }

    groups->table_count++;

    return 0;
}

ContentTable* db_get_table(ContentTableGroups *groups, const char *table_name)
{
    int error;
    ContentTable *table;
    Logger *logger = s_app->logger;

    error = hashmap_get(groups->hashmap_table_groups, (char*)table_name, (void**)(&table));
    if (error != MAP_OK) {
        logger->log_e(logger, "DB: Get Table Error.");
    }

    return table;
}
