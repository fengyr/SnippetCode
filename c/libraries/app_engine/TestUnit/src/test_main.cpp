/*
 * =====================================================================================
 *
 *       Filename:  test1.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  一月 28, 2015
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
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

static App *s_app_ins;

TEST(AppTest, GetAppIns) 
{
    App *app = appe_get_app_instance();
    EXPECT_TRUE(app != NULL);
}

void SetUp()
{
    s_app_ins->init(s_app_ins);
    s_app_ins->run(s_app_ins);
    s_app_ins->quit(s_app_ins);
}

void TearDown()
{
}

int main(int argc, char **argv) 
{
    InitGoogleTest(&argc, argv);

    s_app_ins = appe_create_app_instance(argc, (const char**)argv);

    return RUN_ALL_TESTS();
}
