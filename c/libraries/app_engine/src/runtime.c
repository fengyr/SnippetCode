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

#include "runtime.h"
#include "options.h"

#include "dev_serial.h"
#include "telnet_proc.h"
#include "ui_interface.h"

#ifdef USE_MYSQL
static MysqlClient *s_client;
#endif

static Options s_options;

#ifdef USE_SQLITE
static void query_bind_blob2(SqlQuery &query, void *bind_data)
{
}

static void query_get_blob2(SqlQuery &query, void *get_data)
{
    const void* blob = NULL;
    size_t size;
    FILE *fp = (FILE*) get_data;

    SqlResult colBlob = query.getColumn(1);
    blob = colBlob.getBlob ();
    size = colBlob.getBytes ();
    size_t sizew = fwrite(blob, 1, size, fp);
    fclose (fp);
}

static void query_bind_blob1(SqlQuery &query, void *bind_data)
{
    FILE *fp = (FILE*) bind_data;
    char  buffer[16*1024];
    void* blob = &buffer;
    int size = static_cast<int>(fread(blob, 1, 16*1024, fp));
    buffer[size] = '\0';
    fclose (fp);

    query.bind(1, blob, size);
}

static void query_get_blob1(SqlQuery &query, void *get_data)
{
    int nb = query.exec ();
}

static void query_bind(SqlQuery &query, void *bind_data)
{
    query.bind(1, 1);
}

static void query_get(SqlQuery &query, void *get_data)
{
    int id = query.getColumn(0);
    std::string value = query.getColumn(1);
    int bytes = query.getColumn(1).getBytes();

    printf("QUERY_GET: id=%d, value=%s, bytes=%d\n", id, value.c_str(), bytes);
}
#endif

static void test_sqlite(App *app)
{
#ifdef USE_SQLITE
    SqliteClient *sqlite = app->sqlite_client;

    // 第一次打开
    // 只读模式，简单获取单个数据
    printf("-------- open 1\n");
    sqlite->open_db(sqlite, "./example.db3", SQL_OPEN_RO);
    int exists = sqlite->table_exists(sqlite, "test");
    printf("Table test exists: %d\n", exists);

    try {
        const char *value = sqlite->exec_get(sqlite, "SELECT value FROM test WHERE id=2");
        if (value != NULL) {
            printf("exec_get: %s\n", value);
        }
    }
    catch (std::exception &e)
    {
        printf("Table test exception: %s\n",e.what());
    }
    

    sqlite->query_cond(sqlite, "SELECT id as test_id, value as test_val FROM test WHERE id > ?", query_bind, NULL, query_get, NULL);
    sqlite->close_db(sqlite);

    // 第二次打开
    // 读写模式，创建表，条件查询多个数据
    printf("-------- open 2\n");
    sqlite->open_db(sqlite, "./example.db3", SQL_OPEN_RW | SQL_OPEN_CR);

    sqlite->exec_sql(sqlite, "DROP TABLE IF EXISTS test");
    sqlite->exec_sql(sqlite, "CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");
    // first row
    int nb = sqlite->exec_sql(sqlite, "INSERT INTO test VALUES (NULL, \"test\")");
    printf("exec1 nb: %d\n", nb);
    // second row
    nb = sqlite->exec_sql(sqlite, "INSERT INTO test VALUES (NULL, \"second\")");
    printf("exec2 nb: %d\n", nb);
    // update the second row
    nb = sqlite->exec_sql(sqlite, "UPDATE test SET value=\"second-updated\" WHERE id='2'");
    printf("exec3 nb: %d\n", nb);

    sqlite->query_cond(sqlite, "SELECT * FROM test", NULL, NULL, query_get, NULL);

    // 读写模式，创建表，事务处理
    printf("-------- open 3\n");
    sqlite->transaction_begin(sqlite);
    nb = sqlite->exec_sql(sqlite, "INSERT INTO test VALUES (NULL, \"third\")");
    printf("exec4 nb: %d\n", nb);
    nb = sqlite->exec_sql(sqlite, "INSERT INTO test VALUES (NULL, \"forth\")");
    printf("exec5 nb: %d\n", nb);
    sqlite->transaction_end(sqlite);

    sqlite->query_cond(sqlite, "SELECT * FROM test", NULL, NULL, query_get, NULL);
    sqlite->close_db(sqlite);

    // 第四次打开
    // 读写模式，写入二进制数据
    printf("-------- open 4\n");
    sqlite->open_db(sqlite, "./example_blob.db3", SQL_OPEN_RW | SQL_OPEN_CR);

    sqlite->exec_sql(sqlite, "DROP TABLE IF EXISTS test");
    sqlite->exec_sql(sqlite, "CREATE TABLE test (id INTEGER PRIMARY KEY, value BLOB)");
    
    FILE* fp = fopen("./logo.png", "rb");
    if (NULL != fp) {
        // Insert query
        sqlite->query_cond(sqlite, "INSERT INTO test VALUES (NULL, ?)", query_bind_blob1, (void*)fp, query_get_blob1, NULL);
    }

    fp = fopen("out.png", "wb");
    if (NULL != fp) {
        sqlite->query_cond(sqlite, "SELECT * FROM test", query_bind_blob2, NULL, query_get_blob2, (void*)fp);
    }
    sqlite->close_db(sqlite);
#endif
}

static void task_run(TaskParam param)
{
    printf("TaskRun add\n");
}

static void test_task_manager(App *app)
{
    TaskManager *task_manager = app->task_manager;

    int i;
    for (i = 0; i < 256; i++) {
        task_manager->add_task(task_manager, task_run, NULL);
    }
}

static void test_modbus_master()
{
#ifdef USE_MODBUS
    int rc;
    ModbusConfig config;
    config.debug_mode = 0;
    config.recovery_mode = MODBUS_ERROR_RECOVERY_NONE;
    config.res_timeout_sec = 1;
    config.res_timeout_usec = 0;
    config.byte_timeout_sec = 1;
    config.byte_timeout_usec = 0;
    ModbusMaster *modbus_m = create_modbus_master_tcp("127.0.0.1", 1502, NULL);
    if (!modbus_m) {
        printf("=================create_modbus_master: %p\n", modbus_m);
        return;
    }

    // 写单个线圈
    unsigned char bits[128];
    unsigned short shorts[128];

    printf("==========读写单个线圈==========\n");
    memset(bits, 0, sizeof(bits));
    rc = modbus_m->write_bit(modbus_m, 0x130, 1);
    DEBUG("write_bits: rc=%d\n", rc);
    rc = modbus_m->read_bits(modbus_m, 0x130, 1, bits);
    DEBUG("read_bits: rc=%d, bits[0]=%d\n\n", rc, bits[0]);

    // 写多个线圈
    printf("==========读写多个线圈==========\n");
    const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };
    memset(bits, 0, sizeof(bits));
    rc = modbus_m->write_bits(modbus_m, 0x130, 0x25, UT_BITS_TAB);
    DEBUG("write_bits: rc=%d\n", rc);
    rc = modbus_m->read_bits(modbus_m, 0x130, 0x25, bits);
    DEBUG("read_bits: rc=%d, bits[0]=%x, bits[1]=%x\n\n", rc, bits[0], bits[1]);

    // 读离散量输入
    printf("==========读写离散量输入==========\n");
    const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };
    memset(bits, 0, sizeof(bits));
    rc = modbus_m->read_input_bits(modbus_m, 0x1C4, 0x16, bits);
    DEBUG("read_input_bits: rc=%d, bits[0]=%x, bits[1]=%x\n\n", rc, bits[0], bits[1]);

    // 写单个寄存器
    printf("==========读写单个寄存器==========\n");
    memset(shorts, 0, sizeof(shorts));
    rc = modbus_m->write_register(modbus_m, 0x16B, 0x1234);
    DEBUG("write_register: rc=%d\n", rc);
    rc = modbus_m->read_registers(modbus_m, 0x16B, 1, shorts);
    DEBUG("read_registers: rc=%d, shorts[0]=%x\n\n", rc, shorts[0]);

    printf("==========读写多个寄存器==========\n");
    memset(shorts, 0, sizeof(shorts));
    const uint16_t UT_REGISTERS_TAB[] = { 0x022B, 0x0001, 0x0064 };
    rc = modbus_m->write_registers(modbus_m, 0x16B, 0x3, UT_REGISTERS_TAB);
    DEBUG("write_registers: rc=%d\n", rc);
    rc = modbus_m->read_registers(modbus_m, 0x16B, 0x3, shorts);
    DEBUG("read_registers: rc=%d, shorts[0]=%x, shorts[1]=%x, shorts[2]=%x\n\n", 
          rc, shorts[0], shorts[1], shorts[2]);

    printf("==========同时读写多个寄存器==========\n");
    memset(shorts, 0, sizeof(shorts));
    rc = modbus_m->write_and_read_registers(modbus_m,
                                            0x16B,
                                            0x3,
                                            UT_REGISTERS_TAB,
                                            0x16B,
                                            0x3,
                                            shorts);
    DEBUG("write_and_read_registers: rc=%d, shorts[0]=%x, shorts[1]=%x, shorts[2]=%x\n\n", 
          rc, shorts[0], shorts[1], shorts[2]);

    printf("==========读输入寄存器==========\n");
    memset(shorts, 0, sizeof(shorts));
    rc = modbus_m->read_input_registers(modbus_m, 0x108, 0x01, shorts);
    DEBUG("read_registers: rc=%d, shorts[0]=%x\n\n", rc, shorts[0]);

    printf("==========读取浮点型数据==========\n");
    const float UT_REAL = 916.540649;
    float real;

    memset(shorts, 0, sizeof(shorts));
    modbus_m->set_float(UT_REAL, shorts);
    real = modbus_get_float(shorts);
    DEBUG("modbus_get_float: UT_REAL=%f, real=%f\n", UT_REAL, real);

    memset(shorts, 0, sizeof(shorts));
    modbus_set_float_dcba(UT_REAL, shorts);
    real = modbus_get_float_dcba(shorts);
    DEBUG("modbus_get_float_dcba: UT_REAL=%f, real=%f\n\n", UT_REAL, real);

    free_modbus_master(modbus_m);
#endif
}

static void test_module_serial()
{
#ifdef USE_DEVICE
    HwModule *module = NULL;
    HwDevice *dev = NULL;
    SerialDevice *device = NULL;

    if (hw_get_module(MODULE_SERIAL_ID, "./libs/libserial.so", &module) < 0) {
        printf(">>> hw_get_module error: %s\n", MODULE_SERIAL_ID);
    } else {
        dump_module_info(module);
    }

    DEBUG("----------__init begin, %p\n", &module->methods->__init);
    module->methods->__init(module, MODULE_SERIAL_ID, &dev);
    DEBUG("----------__init end\n");

    if (dev) {
        dump_device_info(dev);
        device = container_of(dev, SerialDevice, common);
        if (device->open("/dev/ttyS0") < 0) {
            printf(">>> device->open error\n");
        } else {
            device->init(device->fd, 9600, 8, 1, 'o');
        }

        DEBUG("----------__exit begin, %p\n", &device->common.__exit);
        device->common.__exit(&device->common);
        DEBUG("----------__exit end\n");
    }
#endif
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

#ifdef USE_MYSQL
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
#endif

static void s_mysql_exec_handler(void *result)
{
#ifdef USE_MYSQL
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
#endif
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

static int slave_recv_call(TcpSlave *slave, void *data, int len)
{

    if (len <=0) {
        printf("==========slave_recv_call: server quit==========\n");
        return -1;
    }

    char buf[20480];
    strncpy(buf, (const char*) data, len);
    printf("slave_recv_call: %s, %d\n", buf, len);
    return 0;
}

static int slave_recv_call2(TcpSlave *slave, void *data, int len)
{
    if (len <=0) {
        printf("==========slave_recv_call2: server quit==========\n");
        return -1;
    }

    printf("slave_recv_call2: %d\n", len);
    return 0;
}

static void test_slave_groups(App *app)
{
    int i = 0;
    int slave_id = 0;
    char slave_name[64];

    TcpSlaveGroups *groups = app->tcp_slave_groups;

    // 宏定义最大客户端数量为256
    for (i = 0; i < 256; i++) {
        memset(slave_name, 0, sizeof(slave_name));
        sprintf(slave_name, "slave-%d", i);
        groups->register_slave(groups, slave_name, "127.0.0.1", 9001);
        TcpSlave *slave = groups->get_slave(groups, slave_name);
    }

    TcpSlave *slave = groups->get_slave(groups, slave_name);

    static RecvHandler s_recv_handler;
    s_recv_handler.req_size = 16;
    s_recv_handler.recv_timeout = 0;
    s_recv_handler.onRecvAndReplay = slave_recv_call;
    slave->register_recv_handler(slave, &s_recv_handler);
    slave->connect(slave, SLAVE_AUTO_RECONNECT);
    /* slave->connect(slave, SLAVE_DISABLE_RECONNECT); */
    slave->send(slave, (void*)"hello", 6);

    int sw = 0;
    int count = 256;
    while (count-- > 0) {
        slave->send(slave, (void*)"hello", 6);
        sleep(0.1);

        // 断开重连
        // sw等于0或1时使用回调
        // sw等于2时使用recv方法
        slave->disconnect(slave);
        if (sw == 0) {
            s_recv_handler.onRecvAndReplay = slave_recv_call;
            slave->register_recv_handler(slave, &s_recv_handler);
            slave->connect(slave, SLAVE_AUTO_RECONNECT);
            sw = 1;
        } else if (sw == 1) {
            s_recv_handler.onRecvAndReplay = slave_recv_call2;
            slave->register_recv_handler(slave, &s_recv_handler);
            slave->connect(slave, SLAVE_AUTO_RECONNECT);
            sw = 2;
        } else {
            s_recv_handler.onRecvAndReplay = NULL;
            slave->register_recv_handler(slave, &s_recv_handler);
            slave->connect(slave, SLAVE_AUTO_RECONNECT);
            sw = 0;

            char buf[1024];
            slave->send(slave, (void*)"hello", 6);
            slave->recv(slave, (void*)buf, 1024);
            printf("------------- %s\n", buf);
        }
    }
}

static void test_server_groups(App *app)
{
    int i = 0;

    char server_name[64];
    Options *options = app->options;
    TcpServerGroups *groups = app->tcp_server_groups;

    // 宏定义最大服务端数量为128
    for (i = 0; i < 128; i++) {
        memset(server_name, 0, sizeof(server_name));
        sprintf(server_name, "tcp-%d", i);
        groups->register_server(groups, ENUM_SERVER_TCP_ASCII, server_name, options->cmd.server_ip_addr, options->cmd.server_port + i);
        /* TelnetServer *server = (TelnetServer*) groups->get_server(groups, server_name); */
        /* printf("tcp name:%s, %p\n", server_name, server); */

        memset(server_name, 0, sizeof(server_name));
        sprintf(server_name, "telnet-%d", i);
        groups->register_server(groups, ENUM_SERVER_TELNET, server_name, options->cmd.server_ip_addr, 31018 + i);
        /* TelnetServer *server2 = (TelnetServer*) groups->get_server(groups, server_name); */
        /* printf("telnet name:%s, %p\n", server_name, server2); */
    }
}

static void test_register_telnet_proc(App *app)
{
    TcpServerGroups *groups = app->tcp_server_groups;
    TelnetServer *server = (TelnetServer*) groups->get_server(groups, "telnet-0");
    static EventHandler s_telnet_handler;

    s_telnet_handler.handler_type = HANDLER_TYPE_TELNET;
    s_telnet_handler.onRecvAndReplay = telnet_handler;
    server->register_event_handler(server, &s_telnet_handler);
}

static void test_register_tcp_handler(App *app)
{
    TcpServerGroups *groups = app->tcp_server_groups;
    TcpServer *server = (TcpServer*) groups->get_server(groups, "tcp-0");
    static EventHandler s_ui_control_handler;
    static EventHandler s_img_data_handler;
    static EventHandler s_mobile_handler;
    static EventHandler s_modbus_handler;

    /* 注册界面控制类型的处理方法 */
    s_ui_control_handler.handler_type = HANDLER_TYPE_1;
    s_ui_control_handler.onRecvAndReplay = ui_control_handler;
    server->register_event_handler(server, &s_ui_control_handler);

    /* 注册结果图片类型的处理方法 */
    s_img_data_handler.handler_type = HANDLER_TYPE_2;
    s_img_data_handler.onRecvAndReplay = ui_img_data_handler;
    server->register_event_handler(server, &s_img_data_handler);

    /* 注册PING的处理方法 */
    s_mobile_handler.handler_type = HANDLER_TYPE_3;
    s_mobile_handler.onRecvAndReplay = mobile_handler;
    server->register_event_handler(server, &s_mobile_handler);

    /* 注册MODBUS类型的处理方法 */
    s_modbus_handler.handler_type = HANDLER_TYPE_4;
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
#define KEY_MAX_LENGTH (1024)
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
    app->save_options(app, &s_options, s_options.cmd.config_file_path);

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

    /* // STRUCT
     * test_object_array(10);
     * test_list(10);
     * test_hashmap(); */

    // MYSQL
    /* test_mysql();
     * test_get_tables();   */

    // SQLITE
    test_sqlite(app);

    // MODBUS
    /* test_modbus_master(); */

    /* test_slave_groups(app); */

/*     test_module_serial();
 * 
 *     test_task_manager(app);
 * 
 *     test_server_groups(app);
 *     test_register_telnet_proc(app);  
 *     test_register_tcp_handler(app);    */

    return 1;
}
