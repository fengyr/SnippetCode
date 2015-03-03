/*
 * =====================================================================================
 *
 *       Filename:  test_object_array.cpp
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  二月 19, 2015
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "limits.h"
#include "gtest/gtest.h"
#include "core/appe_app.h"

using ::testing::EmptyTestEventListener;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

static App *app;
static AppeTcpSlaveGroups *groups;
static int s_slave_count = 1;

static int slave_recv_call(AppeTcpSlave *slave, void *data, int len)
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

class SlaveGroupsTest : public testing::Test {
protected:
    static void SetUpTestCase() {
        app = appe_get_app_instance();
        EXPECT_TRUE(app != NULL);

        groups = app->tcp_slave_groups;
        EXPECT_TRUE(groups != NULL);
    }

    static void TearDownTestCase() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(SlaveGroupsTest, RegisterSlave)
{
    char slave_name[64]; 
    int i, res = -1;

    char *ip = "127.0.0.1";
    int  port = 9001;

    res = groups->register_slave(groups, NULL, ip, port);
    EXPECT_EQ(res, -1);
    res = groups->register_slave(groups, "slave", NULL, port);
    EXPECT_EQ(res, -1);
    res = groups->register_slave(groups, NULL, NULL, port);
    EXPECT_EQ(res, -1);
    res = groups->register_slave(NULL, NULL, NULL, port);
    EXPECT_EQ(res, -1);

    for (i = 0; i < s_slave_count; i++) {
        memset(slave_name, 0, sizeof(slave_name));
        sprintf(slave_name, "slave-%d", i);
        res = groups->register_slave(groups, slave_name, ip, port);
        EXPECT_EQ(res, 0) << "IP=" << ip << " PORT=" << port;
        AppeTcpSlave *slave = groups->get_slave(groups, slave_name);
        EXPECT_TRUE(slave != NULL);
    }

    EXPECT_EQ(groups->slave_count, s_slave_count);
    
    AppeTcpSlave *slave = groups->get_slave(groups, "slave-no-name");
    EXPECT_TRUE(slave == NULL);
    slave = groups->get_slave(groups, slave_name);
    EXPECT_TRUE(slave != NULL);
}

TEST_F(SlaveGroupsTest, ConnectNoServer)
{
    int res = -1;
    char *ip = "127.0.0.1";
    int  port = 9002;
    
    res = groups->register_slave(groups, "slave", ip, port);
    EXPECT_EQ(res, 0) << "IP=" << ip << " PORT=" << port;

    s_slave_count++;

    AppeTcpSlave *slave = groups->get_slave(groups, "slave");
    EXPECT_TRUE(slave != NULL);

    static AppeRecvHandler s_recv_handler;
    s_recv_handler.req_size = 16;
    s_recv_handler.recv_timeout = 0;
    s_recv_handler.onRecvAndReplay = slave_recv_call;


    res = slave->register_recv_handler(NULL, &s_recv_handler);
    EXPECT_EQ(res, -1);
    res = slave->register_recv_handler(NULL, NULL);
    EXPECT_EQ(res, -1);
    res = slave->register_recv_handler(slave, &s_recv_handler);
    EXPECT_EQ(res, 0);
    res = slave->connect(slave, SLAVE_AUTO_RECONNECT);
    EXPECT_EQ(res, -1);


    /* res = slave->send(slave, (void*)"hello", 6);
     * EXPECT_EQ(res, -1);
     * char buf[1024];
     * res = slave->recv(slave, (void*)buf, 1024);
     * EXPECT_EQ(res, -1); */
}

TEST_F(SlaveGroupsTest, Destroy)
{
    EXPECT_TRUE(groups->slave_count == s_slave_count);

    int res = -1;
    res = groups->destroy(NULL);
    EXPECT_EQ(res, -1);
    res = groups->destroy(groups);
    EXPECT_EQ(res, 0);

    EXPECT_EQ(groups->slave_count, 0);
}
