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

static App          *app;
static TaskManager  *task_manager;

static void task_run(TaskParam param)
{
    if (!param) {
        return;
    }

    int *id = (int*) param;
    printf("|TaskRun add, %d|\n", *id);
    if (id != NULL) {
        free(id);
        id = NULL;
    }
}

class TaskManagerTest : public testing::Test {
protected:
    static void SetUpTestCase() {
        app = appe_get_app_instance();
        EXPECT_TRUE(app != NULL);

        task_manager = app->task_manager;
        EXPECT_TRUE(task_manager != NULL);
    }

    static void TearDownTestCase() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(TaskManagerTest, AddTask)
{
    int res = -1;
    int *id = (int*) malloc(sizeof(int));
    *id = 100;

    res = task_manager->add_task(task_manager, task_run, (TaskParam)id);
    EXPECT_EQ(res, 0);
    res = task_manager->add_task(task_manager, task_run, NULL);
    EXPECT_EQ(res, 0);
    res = task_manager->add_task(task_manager, NULL, NULL);
    EXPECT_EQ(res, -1);
    res = task_manager->add_task(NULL, NULL, NULL);
    EXPECT_EQ(res, -1);
}
