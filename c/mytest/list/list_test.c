/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct list_node {
    char                data[20];
    struct list_node   *next;
}LIST_NODE;

LIST_NODE* Create_node();

void Add_node(LIST_NODE *list, int i, char *data);

void Del_node(LIST_NODE *list, int i);

void TraverseList(LIST_NODE *list);
