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

#include "array.h"
#include "hashmap.h"
#include "list.h"
#include "message.h"

#include "runtime.h"
#include "debug.h"

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

static void test_hashmap(int size)
{
#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (10)

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
        case MSG_ON_START: {
            on_msg_start(handler, msg);

            Message *new_msg = (Message*)create_empty_message(MSG_SEP_1); 
            trans_message(new_msg);
            break;
        }
        case MSG_SEP_1: {
            on_msg_step_1(handler, msg);

            Message *new_msg = (Message*)create_message(MSG_SEP_2, 10, 100, (void*)"hello world"); 
            trans_message(new_msg);
            break;
        }
        case MSG_SEP_2: {
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
    app->register_handler(handler_message, 1);

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

    test_object_array(10);
    DEBUG("on_app_process loginfo\n");
    logger->log_i(logger, "-------------------------");

    test_list(10);
    logger->log_d(logger, "-------------------------");

    test_hashmap(10);

    logger->log_e(logger, "-------------------------");

    return 1;
}
