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
#include "runtime.h"

using ::testing::EmptyTestEventListener;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

static App *s_app_ins;

class AppEnvironment : public testing::Environment
{
private:
    int m_argc;
    char **m_argv;

public:
    AppEnvironment(int argc, char **argv) {
        m_argc = argc;
        m_argv = argv;
    }

    virtual void SetUp() {
        std::cout << "-------------------------------------------------" << std::endl;
        std::cout << "-                 BEGIN TEST                    -" << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;
        s_app_ins = appe_create_app_instance(m_argc, (const char**)m_argv);
        s_app_ins->onCreate = on_app_create;
        s_app_ins->onProcess = on_app_process;
        s_app_ins->onDestory = on_app_destroy;
    }

    virtual void TearDown() {
        std::cout << "-------------------------------------------------" << std::endl;
        std::cout << "-                  END TEST                     -" << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;
    }
};

TEST(AppTest, GetAppIns) 
{
    App *app = appe_get_app_instance();
    EXPECT_TRUE(app != NULL);
}

/* void SetUp()
 * {
 *     s_app_ins->init(s_app_ins);
 *     s_app_ins->run(s_app_ins);
 *     s_app_ins->quit(s_app_ins);
 * } */

int main(int argc, char **argv) 
{
    testing::AddGlobalTestEnvironment(new AppEnvironment(argc, argv));
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
