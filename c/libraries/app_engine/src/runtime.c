/*
 * =====================================================================================
 *
 *       Filename:  runtime.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 18, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "mysql.h"
#include "array.h"
#include "hashmap.h"
#include "list.h"
#include "message.h"

#include "runtime.h"
#include "debug.h"
#include "ui_interface.h"
#include "handler_process.h"
#include "telnet_proc.h"
#include "telnet_server.h"
#include "tcp_server.h"
#include "dev_serial.h"

#include "db_column.h"
#include "db_mysql_wrap.h"

#ifdef USE_MYSQL
static MysqlClient *s_client;
#endif

static Options s_options;

static void test_module_serial()
{
    HwModule *module = NULL;
    HwDevice *dev = NULL;
    SerialDevice *device = NULL;

    if (hw_get_module(MODULE_SERIAL_ID, "./libs/libserial.so", &module) < 0) {
        printf(">>> hw_get_module error: %s\n", MODULE_SERIAL_ID);
    }

    module->methods->open(module, MODULE_SERIAL_ID, &dev);

    if (dev) {
        device = (SerialDevice*)dev;
        if (device->open("/dev/ttyS0") < 0) {
            printf(">>> device->open error\n");
        } else {
            device->init(device->fd, 9600, 8, 1, 'o');
        }

        device->common.close(&device->common);
    }
}

static void set_tables()
{
#ifdef USE_MYSQL
    if (!s_client) {
        DEBUG("set_tables: s_client=NULL\n");
        return;
    }

    ContentTableGroups *groups = s_client->db_table_groups;

    // 批次表
    ContentTable *table = groups->create_table("batch_table");
    
    int id = 0;
    ContentColumn *col1 = table->create_column("_id", ENUM_CONTENT_INT, "null", "primary key", "auto_increment", (void*)&id);
    ContentColumn *col2 = table->create_column("start_time", ENUM_CONTENT_DATETIME, "", "", "", (void*)"");
    ContentColumn *col3 = table->create_column("end_time", ENUM_CONTENT_DATETIME, "", "", "", (void*)"");
    ContentColumn *col4 = table->create_column("persist_time", ENUM_CONTENT_TIME, "", "", "", (void*)"");
    ContentColumn *col5 = table->create_column("exit_status", ENUM_CONTENT_CHAR, "", "", "", (void*)"");

    table->add_column(table, col1);
    table->add_column(table, col2);
    table->add_column(table, col3);
    table->add_column(table, col4);
    table->add_column(table, col5);

    // 图片信息表
    ContentTable *table2 = groups->create_table("pic_table");

    ContentColumn *_col1 = table2->create_column("_id", ENUM_CONTENT_INT, "null", "primary key", "auto_increment", NULL);
    ContentColumn *_col2 = table2->create_column("batch_id", ENUM_CONTENT_INT, "", "", "", NULL);
    ContentColumn *_col3 = table2->create_column("is_sample", ENUM_CONTENT_INT, "", "", "", NULL);
    ContentColumn *_col4 = table2->create_column("width", ENUM_CONTENT_INT, "", "", "", NULL);
    ContentColumn *_col5 = table2->create_column("height", ENUM_CONTENT_INT, "", "", "", NULL);
    ContentColumn *_col6 = table2->create_column("serial_number", ENUM_CONTENT_CHAR, "", "", "", NULL);
    ContentColumn *_col7 = table2->create_column("class", ENUM_CONTENT_INT, "", "", "", NULL);
    ContentColumn *_col8 = table2->create_column("grab_time", ENUM_CONTENT_DATETIME, "", "", "", NULL);
    ContentColumn *_col9 = table2->create_column("process_time", ENUM_CONTENT_INT, "", "", "", NULL);
    ContentColumn *_col10 = table2->create_column("process_status", ENUM_CONTENT_INT, "", "", "", NULL);

    table2->add_column(table2, _col1);
    table2->add_column(table2, _col2);
    table2->add_column(table2, _col3);
    table2->add_column(table2, _col4);
    table2->add_column(table2, _col5);
    table2->add_column(table2, _col6);
    table2->add_column(table2, _col7);
    table2->add_column(table2, _col8);
    table2->add_column(table2, _col9);
    table2->add_column(table2, _col10);

    groups->register_table(groups, table->table_name, table);
    groups->register_table(groups, table2->table_name, table2);

#endif
}

static void test_mysql()
{
#ifdef USE_MYSQL

    MysqlUser user;
    strcpy(user.db_host, "127.0.0.1");
    strcpy(user.db_user, "root");
    strcpy(user.db_passwd, "123456");
    strcpy(user.db_name, "fx01_database");
    user.db_port = 0;
    strcpy(user.db_socket, "");
    user.db_flags = 0;
    
    s_client = db_mysql_init(&user, 1);

    set_tables();

    if (s_client) {
        ContentTableGroups *groups = s_client->db_table_groups;
        ContentTable *table = groups->get_table(groups, "batch_table");
        s_client->add_table(s_client, table, 1);
    }

#endif
}

static void s_mysql_query_handler(ContentTable *table) 
{
    if (NULL == table) {
        printf("test_get_tables: get table NULL\n");
        return;
    }

    if (!strcmp(table->table_name, "batch_table")) {
        ContentColumn *col = table->get_column_by_id(table, 0);
        printf("%s=%d,", col->cont_name, *((int*)col->cont_val));
        printf(" ----- ");

        col = table->get_column_by_id(table, 1);
        printf("%s=%s,", col->cont_name, (char*)col->cont_val);
        printf(" ----- ");

        col = table->get_column_by_id(table, 2);
        printf("%s=%s,", col->cont_name, (char*)col->cont_val);
        printf(" ----- ");

        col = table->get_column_by_id(table, 3);
        printf("%s=%s,", col->cont_name, (char*)col->cont_val);
        printf(" ----- ");

        col = table->get_column_by_id(table, 4);
        printf("%s=%s,", col->cont_name, (char*)col->cont_val);
        printf("\n");
    } else {
        ContentColumn *col;
        while ((col = table->get_next_column(table))) {
            if (col->cont_val) {
                printf("%s=%d,", col->cont_name, *((int*)col->cont_val));
                printf(" ----- ");
            }
        }
        printf("\n");
    }

/*     if (table != NULL) {
 *         ContentColumn *col = (ContentColumn*) table->columns.objects[0].item;
 *         printf("col_name=%s, col_null=%s, col_key=%s, col_extra=%s, col_val=%d\n", 
 *                 col->cont_name, col->cont_null, col->cont_key, col->cont_extra, *((int*)col->cont_val));
 *     } else {
 *         printf("test_get_tables: get table NULL\n");
 *     } */
}

static void s_mysql_exec_handler(void *result)
{
    if (!result) {
        printf("------------------------------ result=NULL\n");
    } else {
        int i;
        MYSQL_ROW row;
        MYSQL_FIELD *fields;
        MYSQL_RES *res = (MYSQL_RES*)result;
        int num_rows = mysql_num_rows(res);
        int num_fields = mysql_num_fields(res);

        fields = mysql_fetch_fields(res);
        while ((row = mysql_fetch_row(res))) {
            printf("===============");
            for(i = 0; i < num_fields; i++) {
                printf(" %s=%s, ", fields[i].name, row[i]);
            }
            printf("===============\n");
        }
    }
}

static void test_get_tables()
{
#ifdef USE_MYSQL
    if (!s_client) {
        DEBUG("test_get_tables: s_client=NULL\n");
        return;
    }
    ContentTableGroups *groups = s_client->db_table_groups;

    // 查询批次表
    ContentTable *table = groups->get_table(groups, "batch_table");
    s_client->query_table_cond(s_client, table, "exit_status", "_id < 10", s_mysql_query_handler); 

    // 查询图片信息表
    ContentTable *table2 = groups->get_table(groups, "pic_table");
    s_client->query_table_cond(s_client, table2, "_id,class", "_id < 10", s_mysql_query_handler); 

    // 执行sql语句
    s_client->exec_sql(s_client, "drop table batch_table;", s_mysql_exec_handler);
    s_client->add_table(s_client, table, 1);
    s_client->exec_sql(s_client, "insert into batch_table (exit_status) values('success');", s_mysql_exec_handler);
    s_client->exec_sql(s_client, "insert into batch_table (exit_status) values('normal');", s_mysql_exec_handler);
    s_client->exec_sql(s_client, "delete from batch_table where exit_status='success';", s_mysql_exec_handler);
    s_client->exec_sql(s_client, "select * from batch_table where _id < 10;", s_mysql_exec_handler);

    db_mysql_free(s_client);
#endif
}

static void test_server_groups(App *app)
{
    Options *options = app->options;
    TcpServerGroups *groups = app->tcp_server_groups;
    groups->register_server(groups, ENUM_SERVER_TCP, "tcp_default", options->cmd.server_ip_addr, options->cmd.server_port);
    groups->register_server(groups, ENUM_SERVER_TELNET, "telnet", options->cmd.server_ip_addr, 11018);
}

static void test_register_telnet_proc(App *app)
{
    TcpServerGroups *groups = app->tcp_server_groups;
    TelnetServer *server = (TelnetServer*) groups->get_server(groups, "telnet");
    static EventHandler s_telnet_handler;

    s_telnet_handler.handler_type = HANDLER_TYPE_TELNET;
    s_telnet_handler.onRecvAndReplay = telnet_handler;
    server->register_event_handler(server, &s_telnet_handler);
}

static void test_register_tcp_handler(App *app)
{
    TcpServerGroups *groups = app->tcp_server_groups;
    TcpServer *server = (TcpServer*) groups->get_server(groups, "tcp_default");
    static EventHandler s_ui_control_handler;
    static EventHandler s_img_data_handler;
    static EventHandler s_mobile_handler;
    static EventHandler s_modbus_handler;

    /* 注册界面控制类型的处理方法 */
    s_ui_control_handler.handler_type = HANDLER_TYPE_UI_CONTROL;
    s_ui_control_handler.onRecvAndReplay = ui_control_handler;
    server->register_event_handler(server, &s_ui_control_handler);

    /* 注册结果图片类型的处理方法 */
    s_img_data_handler.handler_type = HANDLER_TYPE_IMG_DATA;
    s_img_data_handler.onRecvAndReplay = ui_img_data_handler;
    server->register_event_handler(server, &s_img_data_handler);

    /* 注册PING的处理方法 */
    s_mobile_handler.handler_type = HANDLER_TYPE_MOBILE_DATA;
    s_mobile_handler.onRecvAndReplay = mobile_handler;
    server->register_event_handler(server, &s_mobile_handler);

    /* 注册MODBUS类型的处理方法 */
    s_modbus_handler.handler_type = HANDLER_TYPE_MODBUS;
    s_modbus_handler.onRecvAndReplay = modbus_protocol_handler;
    server->register_event_handler(server, &s_modbus_handler);
}

static void dump_object_array(ArrayObj *array)
{
    ArrayEntry *objects = array->objects; 
    Message *msg;
    unsigned int i = 0;

    printf("nr=%d, alloc=%d\n", array->nr, array->alloc);
    for (i = 0; i < array->nr; i++) {
        msg = (Message*) objects[i].item;
        printf("dump_object_array: id=%d, arg1=%d, arg2=%d, msg=%s\n", msg->id, msg->arg1, msg->arg2, (char*)msg->obj);
    }
}

static void dump_list(List *list)
{
    ListElmt *element = list->head;

    if (list_size(list) == 0) {
        printf("list dump == NULL\n");
    } else {
        while (element != NULL) {
            Message *msg = (Message*)element->data;
            printf("dump_list: id=%d, arg1=%d, arg2=%d, msg=%s\n", msg->id, msg->arg1, msg->arg2, (char*)msg->obj);

            element = element->next;
        }
    }
}

static void test_object_array(int size)
{
    ArrayObj obj_array;

    array_obj_init(&obj_array, free_message);

    /* 添加数据前，打印数组信息 */
    dump_object_array(&obj_array); 

    int i = 0;
    for (i = 0; i < size; i++) {
        Message *obj = (Message*)create_empty_message(i);

        array_obj_add((void*)obj, &obj_array);
    }
    /* 添加数据后，打印数组信息 */
    dump_object_array(&obj_array);    

    /* 释放数组内存,打印数组信息 */
    array_obj_destroy(&obj_array);
    dump_object_array(&obj_array);
}

static void test_list(int size)
{
    List list;
    list_init(&list, free_message);

    int i = 0;
    for (i = 0; i < size; i++) {
        Message *obj = (Message*)create_empty_message(i);
        list_insert_next(&list, NULL, obj);
    }

    dump_list(&list);

    list_destroy(&list);
    dump_list(&list);
}

static void test_hashmap()
{
#define KEY_MAX_LENGTH (10240)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (100000)

    typedef struct data_struct_s
    {
        char key_string[KEY_MAX_LENGTH];
        int number;
    } data_struct_t;

    int index;
    int error;
    HashMap mymap;
    char key_string[KEY_MAX_LENGTH];
    data_struct_t* value;

    mymap = hashmap_new();

    // 初始化一组hashmap值
    for (index=0; index<KEY_COUNT; index+=1)
    {
        /* Store the key string along side the numerical value so we can free it later */
        value = (data_struct_t*)malloc(sizeof(data_struct_t));
        snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
        value->number = index;

        error = hashmap_put(mymap, value->key_string, value);
        assert(error==MAP_OK);
        printf("put: key=%s, val_string=%s, val_num=%d\n", value->key_string, value->key_string, value->number);
        /* usleep(300); */
    }

    printf("====================================\n");

    // 检查初始化值是否正确
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));

        /* Make sure the value was both found and the correct number */
        assert(error==MAP_OK);
        assert(value->number==index);
        printf("get: key=%s, val_string=%s, val_num=%d\n", key_string, value->key_string, value->number);
        /* usleep(300); */
    }

    /* Make sure that a value that wasn't in the map can't be found */
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, KEY_COUNT);

    error = hashmap_get(mymap, key_string, (void**)(&value));

    /* Make sure the value was not found */
    assert(error==MAP_MISSING);

    /* Free all of the values we allocated and remove them from the map */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        assert(error==MAP_OK);

        error = hashmap_remove(mymap, key_string);
        assert(error==MAP_OK);

        free(value);        
    }

    /* Now, destroy the map */
    hashmap_free(mymap);
}

static int handler_message(struct message_handler_t *handler, struct message_t *msg)
{
    switch (msg->id) {
        case MSG_ON_START: 
            {
                on_msg_start(handler, msg);

                Message *new_msg = (Message*)create_empty_message(MSG_SEP_1); 
                trans_message(new_msg);
                break;
            }
        case MSG_SEP_1: 
            {
                on_msg_step_1(handler, msg);

                Message *new_msg = (Message*)create_message(MSG_SEP_2, 10, 100, (void*)"hello world"); 
                trans_message(new_msg);
                break;
            }
        case MSG_SEP_2: 
            {
                on_msg_step_2(handler, msg);

                /* Message *new_msg = (Message*)create_empty_message(MSG_ON_EXIT);  */
                Message *new_msg = (Message*)create_empty_message(MSG_ON_START); 
                trans_message(new_msg);
                break;
            }

        default:
            break;
    }

    return 0;
}

void on_msg_start(MessageHandler *handler, Message *old_msg)
{
    /* DEBUG("on_msg_start\n"); */
}

void on_msg_step_1(MessageHandler *handler, Message *old_msg)
{
    /* DEBUG("on_msg_step_1 id=%d, arg1=%d, arg2=%d, obj=%p\n",  */
    /* old_msg->id, old_msg->arg1, old_msg->arg2, old_msg->obj); */
}

void on_msg_step_2(MessageHandler *handler, Message *old_msg)
{
    /* DEBUG("on_msg_step_2 id=%d, arg1=%d, arg2=%d, obj=%s\n",  */
    /* old_msg->id, old_msg->arg1, old_msg->arg2, (char*)(old_msg->obj)); */
}

/**
 * @Synopsis 当App对象创建时，该方法被调用，用于初始化
 *
 * @Param app
 *
 * @Returns 
 */
int on_app_create(struct app_runtime_t *app)
{
    printf("===============\n");
    printf("onCreate called\n");
    app->parse_options(app, &s_options);
    app->register_message_handler(handler_message, RUNTIME_LOOP_THREAD);

    return 0;
}

/**
 * @Synopsis 当App对象销毁时，该方法被调用，用于释放内存
 *
 * @Param app
 *
 * @Returns 
 */
int on_app_destroy(struct app_runtime_t *app)
{
    printf("onDestory called\n");
    printf("===============\n");

    return 0;
}

/**
 * @Synopsis 当App运行时，该方法被循环调用。
 *
 * @Param app
 *
 * @Returns 返回0，则继续循环；否则退出循环。 
 */
int on_app_process(struct app_runtime_t *app)
{
    Logger *logger = app->logger;

    /* test_server_groups(app);
     * test_register_tcp_handler(app);
     * test_register_telnet_proc(app);  */

    /*     test_object_array(10);
     *     DEBUG("on_app_process loginfo\n");
     *     logger->log_i(logger, "-------------------------");
     * 
     *     test_list(10);
     *     logger->log_d(logger, "-------------------------");
     * 
     *     test_hashmap();
     * 
     *     logger->log_e(logger, "-------------------------"); */

    /* test_mysql();
     * test_get_tables(); */

    test_module_serial();

    return -1;
}
