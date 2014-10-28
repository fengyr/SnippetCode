/*
 * =====================================================================================
 *
 *       Filename:  taskmanager.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十月 27, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "app.h"

static TaskManager *s_task_manager = NULL;

static int _destory(struct taskmanager_t *task_manager)
{
    if (task_manager != NULL) {

        if (task_manager->pool != NULL) {
            threadpool_destroy(task_manager->pool, threadpool_graceful);
        }

        free(task_manager);
        task_manager = NULL;
        return 0;
    }

    DEBUG("TaskManager destory: OK.");

    return -1;
}

static int __add_task(struct taskmanager_t *task_manager, 
                    TaskRun task, 
                    TaskParam param)
{
    App *s_app = get_app_instance();
    Logger *logger = s_app->logger;
    char info[64];

    if ((task_manager != NULL) && (task_manager->pool != NULL)) {
        int error;
        error = threadpool_add(task_manager->pool, task, param, 0);
        if (error < 0) {
            memset(info, 0, sizeof(info));
            sprintf(info, "TaskManager: Add Task Failed, code: %d.", error);
            logger->log_e(logger, info);
            DEBUG("TaskManager add task: Failed.");
        } else {
            DEBUG("TaskManager add task: OK.");
        }
        return 0;
    }

    return -1;
}

TaskManager* create_taskmanager_instance()
{
    if (!s_task_manager) {
        s_task_manager = (TaskManager*) malloc(sizeof(TaskManager));
        memset(s_task_manager, 0, sizeof(TaskManager));

        s_task_manager->pool = threadpool_create(TASK_NUM, 
                                                TASK_QUEUE_SIZE, 0);
        s_task_manager->add_task = __add_task;
        s_task_manager->destory = _destory;
    }

    return s_task_manager;
}
