/*
 * =====================================================================================
 *
 *       Filename:  taskmanager.h
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
#ifndef _taskmanager_H_
#define _taskmanager_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "taskmanager.h"

typedef void* TaskParam;
typedef void (*TaskRun)(TaskParam param);

#define TASK_NUM        256
#define TASK_QUEUE_SIZE 256

typedef struct taskmanager_t {
    threadpool_t *pool;

    int (*add_task)(struct taskmanager_t *task_manager, 
                 TaskRun task, 
                 TaskParam);
    int (*destory)(struct taskmanager_t *task_manager);
} TaskManager, *PTaskManager;

TaskManager* create_taskmanager_instance();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _taskmanager_H_ */
