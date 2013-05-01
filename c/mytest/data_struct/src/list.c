/*
 * =====================================================================================
 *
 *       Filename:  list.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  四月 30, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

struct list_ g_List;

void list_init(List *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

void list_destroy(List *list)
{
    void *data;
    while (list_size(list) > 0) {
        if ((list_remove_next(list, NULL, (void**)&data) == 0)
            && (list->destroy != NULL)) {
            list->destroy(data);
        }
    }

    memset(list, 0, sizeof(List));
}

/**
 * @Synopsis 插入元素到指定的链表元素之后，
 *           如果element为空，则插入到链表头
 *
 * @Param list 待插入元素的链表
 * @Param element 指定的参考元素
 * @Param data 元素包含的数据
 *
 * @Returns 插入成功返回0，失败返回-1
 */
int list_insert_next(List *list, ListElmt *element, const void *data)
{
    ListElmt *new_element;

    new_element = (ListElmt*) malloc(sizeof(ListElmt));
    if (NULL == new_element) {
        return -1;
    }

    new_element->data = data;

    /* 如果element为空，则加入到链表头 */
    if (NULL == element) {
        /* 链表为空时，需要更新tail指针 */
        if (list_size(list) == 0) {
            list->tail = new_element;
        }

        /* 插入到链表头，并更新链表头指针 */
        new_element->next = list->head;
        list->head = new_element;
    } else {
        /* 如果指定的参考元素为链表尾，则直接更新链表尾指针 */
        if (element->next == NULL) {
            list->tail = new_element;
        }

        /* 更新新元素和参考元素的指针 */
        new_element->next = element->next;
        element->next = new_element;
    }

    /* 更新链表的大小 */
    list->size++;

    return 0;
}

/**
 * @Synopsis 删除指定链表元素后的元素，返回被删除元素的对象给data指针
 *
 * @Param list 待删除元素的链表
 * @Param element 删除该元素后面的元素
 * @Param data 返回被删除元素的对象
 *
 * @Returns 成功删除返回0，否则返回-1
 */
int list_remove_next(List *list, ListElmt *element, void **data)
{
    ListElmt *old_element;

    if (list_size(list) == 0) {
        return -1;
    }

    /* 如果element为空，则删除链表头 */
    if (element == NULL) {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1) {
            list->tail = NULL;
        }
    } else {
        if (element->next == NULL) {
            return -1;
        }

        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        /* 更新tail指针 */
        if (element->next == NULL) {
            list->tail = element;
        }
    }

    /* 释放删除的元素，并更新链表大小 */
    free(old_element);
    list->size--;

    return 0;
}

void list_dump(List *list)
{
    ListElmt *element = list->head;

    if (list_size(list) == 0) {
        printf("list dump == NULL\n");
    } else {
        while (element != NULL) {
            Element *data = element->data;
            printf("list dump name=%s, id=%d\n", data->name, data->id);

            element = element->next;
        }
    }
}
