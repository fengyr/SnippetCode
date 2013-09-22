/*
 * =====================================================================================
 *
 *       Filename:  content.h
 *
 *    Description:  该模块定义了一组抽象的数据库模型
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
#ifndef _content_H_
#define _content_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hashmap.h"
#include "array.h"

// 枚举类型的编号与s_column_type_str对应
enum content_type_t {
    ENUM_CONTENT_START = -1,
    ENUM_CONTENT_INT = 0,
    ENUM_CONTENT_BITINT = 1,
    ENUM_CONTENT_FLOAT = 2,
    ENUM_CONTENT_DOUBLE = 3,
    ENUM_CONTENT_DATE = 4,
    ENUM_CONTENT_TIME = 5,
    ENUM_CONTENT_DATETIME = 6,
    ENUM_CONTENT_CHAR = 7,
    ENUM_CONTENT_TEXT = 8,
    ENUM_CONTENT_LONGTEXT = 9,
    ENUM_CONTENT_END = 10,
};

typedef void* ContentVal;

#define DB_MAX_NAME_LEN    64
struct content_t {
    char cont_name[DB_MAX_NAME_LEN];    // 列名称
    enum content_type_t cont_type;      // 列数据类型
    char cont_null[16];                 // 是否可为空
    char cont_key[16];                  // 主键
    char cont_extra[16];                // 其他信息
    ContentVal cont_val;                // 默认值
};
typedef struct content_t ContentColumn, *PContentColumn;

struct table_t {
    char table_name[DB_MAX_NAME_LEN];
    ArrayObj columns;

    ContentColumn* (*create_column)(const char *name, 
                                    enum content_type_t type, 
                                    const char *is_null, 
                                    const char *key, 
                                    const char *extra, 
                                    void *val);
    void (*add_column)(struct table_t *table, struct content_t *column);
};
typedef struct table_t ContentTable, *PContentTable;

#define DB_MAX_TABLES  32
struct table_groups_t {
    HashMap hashmap_table_groups;
    char *table_names[DB_MAX_NAME_LEN];
    int table_count;

    ContentTable* (*create_table)(const char *name);
    int (*register_table)(struct table_groups_t *groups, const char *table_name, struct table_t *table);
    ContentTable* (*get_table)(struct table_groups_t *groups, const char *table_name);
};
typedef struct table_groups_t ContentTableGroups, *PContentTableGroups;

//////////////////////////////////////////////////////
//                  ContentColumn                   //
//////////////////////////////////////////////////////
/**
 * @Synopsis 创建一个抽象列
 *
 * @Param name  抽象列的名称
 * @Param type  抽象列的数据类型
 * @Param val   抽象列的值
 *
 * @Returns 
 */
ContentColumn* db_create_column(const char *name, 
                                enum content_type_t type, 
                                const char *is_null, 
                                const char *key, 
                                const char *extra, 
                                void *val);

/**
 * @Synopsis    销毁一个抽象列 
 *
 * @Param column    抽象列的指针
 */
void db_free_column(ContentColumn *column);

/**
 * @Synopsis 根据枚举类型的抽象类型，返回字符串描述
 *
 * @Param type
 *
 * @Returns 
 */
const char* db_get_column_type_str(enum content_type_t type);

//////////////////////////////////////////////////////
//                  ContentTable                    // 
//////////////////////////////////////////////////////
/**
 * @Synopsis 创建一个抽象表Table
 *
 * @Param name 表名称
 *
 * @Returns 
 */
ContentTable* db_create_table(const char *name);

/**
 * @Synopsis 向一个抽象表中加入一个抽象列
 *
 * @Param table     抽象表指针
 * @Param column    抽象列指针
 */
void db_table_add_column(ContentTable *table, ContentColumn *column);

/**
 * @Synopsis    销毁一个抽象表
 *
 * @Param table 抽象表的指针
 */
void db_destroy_table(ContentTable *table);

//////////////////////////////////////////////////////
//              ContentTableGroups                  //
//////////////////////////////////////////////////////
/**
 * @Synopsis 创建一组Table，对应一个抽象数据库DB
 *
 * @Returns 
 */
ContentTableGroups* db_create_table_groups_instance();

/**
 * @Synopsis 注册一个Table到抽象数据库，注册的该Table是
 *           为了能够根据名称，获取该Table的对象，
 *           实际并未加入到真正的数据库中
 *
 * @Returns 
 */
int db_register_table(ContentTableGroups *groups, 
                      const char *table_name, 
                      ContentTable *table);
/**
 * @Synopsis 从抽象数据库中获取对应名称的Table对象
 *
 * @Param groups        抽象数据库指针
 * @Param table_name    表名称
 *
 * @Returns 
 */
ContentTable* db_get_table(ContentTableGroups *groups, const char *table_name);

/**
 * @Synopsis 销毁一个抽象数据库
 *
 * @Param groups        抽象数据库指针
 */
void db_destroy_table_groups(ContentTableGroups *groups);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _content_H_ */
