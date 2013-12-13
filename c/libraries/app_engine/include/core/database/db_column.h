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

/* 抽象表和抽象列的结构体描述 */
/* +----------------------+            +----------------------+
 * |    <ContentTable>    |     +----->|    <ContentColumn>   |
 * +----------------------+     |      +----------------------+
 * |  m: table_name (str) |     |      | m: cont_name (str)   |
 * +----------------------+     |      +----------------------+
 * |  m: columns (Array)--|-----+      | m: cont_type (enum)  |
 * +----------------------+            +----------------------+
 * |  m: cols_num (int)   |            | m: cont_null (str)   |            
 * +----------------------+            +----------------------+
 * |  m: cols_cursor (int)|            | m: cont_key (str)    |
 * +----------------------+            +----------------------+
 *       抽象表结构                    | m: cont_extra (str)  |
 *                                     +----------------------+
 *                                     | m: ContentVal (void*)|
 *                                     +----------------------+
 *                                           抽象列结构
 ***/

#define MAX_LEN_DATE        64              // 日期类型数据的最大字节数
#define MAX_LEN_CHAR        255             // 字符串类型数据的最大字节数
#define MAX_LEN_LONGTEXT    1024 * 1024     // 长文本类型数据的最大字节数

// 枚举类型的编号与s_column_type_str对应
enum content_type_t {
    ENUM_CONTENT_START = -1,
    ENUM_CONTENT_INT = 0,
    ENUM_CONTENT_BIGINT = 1,
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
    char table_name[DB_MAX_NAME_LEN];   // 抽象表名称
    ArrayObj columns;                   // 抽象列对象数组
    int cols_num;                       // 抽象列对象数量
    int cols_cursor;                    // 当前列游标指针

    // 创建一个抽象列
    struct content_t* (*create_column)(const char *name, 
                                    enum content_type_t type, 
                                    const char *is_null, 
                                    const char *key, 
                                    const char *extra, 
                                    void *val);

    // 映射一个抽象列到真实数据库中
    void (*add_column)(struct table_t *table, struct content_t *column);

    // 根据列名获取抽象列
    struct content_t* (*get_column)(struct table_t *table, 
                                    const char *col_name);

    // 根据序号获取抽象列
    struct content_t* (*get_column_by_id)(struct table_t *table, int index);

    // 依次遍历所有列
    struct content_t* (*get_next_column)(struct table_t *table);

    // 获取游标位置
    int (*get_cursor_pos)(struct table_t *table);

    // 重置游标到初始位置
    void (*reset_cursor_pos)(struct table_t *table);
};
typedef struct table_t ContentTable, *PContentTable;

#define DB_MAX_TABLES   32
struct table_groups_t {
    HashMap hashmap_table_groups;
    char *table_names[DB_MAX_NAME_LEN];
    int table_count;

    ContentTable* (*create_table)(const char *name);
    int (*register_table)(struct table_groups_t *groups, 
                          const char *table_name, 
                          struct table_t *table);
    ContentTable* (*get_table)(struct table_groups_t *groups, 
                               const char *table_name);
};
typedef struct table_groups_t ContentTableGroups, *PContentTableGroups;

// 数据库查询回调
typedef void (*MysqlQueryHandler)(ContentTable *table);

// 数据库执行回调
typedef void (*MysqlExecHandler)(void *res);

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

/**
 * @Synopsis 设置一个抽象列的值
 *
 * @Param column    抽象列指针
 * @Param val       该列的值
 * @Param from_db   数据来源是否从数据库读取.
 *                  数据库读取为字符串类型，需要转换成实际类型
 *                  1->数据来源为数据库，0->数据来源为非数据库
 * 
 */
void db_set_column_val(ContentColumn *column, void *val, int from_db);

/**
 * @Synopsis 通过列名设置一个抽象列的值
 *
 * @Param table     抽象表指针
 * @Param name      列名
 * @Param val       该列的值
 */
void db_set_column_val_by_name(struct table_t *table, 
                               char *col_name, 
                               void *val);

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
 * @Synopsis 根据列名获取一个抽象列对象
 *
 * @Param table     抽象表指针
 * @Param col_name  抽象列名称
 *
 * @Returns         抽象列指针
 */
ContentColumn* db_table_get_column(ContentTable *table, 
                                   const char *col_name);

/**
 * @Synopsis 根据序号获取一个抽象列对象
 *
 * @Param table     抽象表指针
 * @Param index     抽象列序号
 *
 * @Returns         抽象列指针
 */
ContentColumn* db_table_get_column_by_index(ContentTable *table, 
                                            int index);

/**
 * @Synopsis 依次获取一个抽象列对象
 *
 * @Param table     抽象表指针
 *
 * @Returns         抽象列指针
 */
ContentColumn* db_table_get_next_column(ContentTable *table);

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
ContentTable* db_get_table(ContentTableGroups *groups, 
                           const char *table_name);

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
