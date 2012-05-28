/**
 *       File       class.h
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.0
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2010年06月02日 12时22分43秒
 *
 * =====================================================================================
 */
#ifndef CLASS_TNTOHY6A
#define CLASS_TNTOHY6A

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

typedef struct _DEMO DEMO;

/**
 * @brief 定义包的元素
 * 
 */
typedef struct _DEMO_ELEMENT {
	int id;
	int age;
	char name[256];
} DEMO_ELEMENT;

/**
 * @brief 定义包的方法
 *
 * 该方法应该只对包的元素做操作
 */
typedef struct _DEMO_INTERFACE {
	void (*display) (DEMO * this);
} DEMO_INTERFACE;

/**
 * @brief 定义一个包
 * 
 */
struct _DEMO {
    /** 包内部变量 */
    /*...*/
    int number;

    /** 包的元素和方法 */
	DEMO_ELEMENT Attr;
	DEMO_INTERFACE Iface;

    /** 初始化包的元素 */
	void (*initialize) (struct _DEMO * this, int id, int age, char *name);
    /** 销毁包的元素及内容 */
	void (*destroy) (struct _DEMO * this);
};

void demo_init(DEMO * this, int id, int age, char *name);

void demo_free(DEMO * this);
    
#endif /* end of include guard: CLASS_TNTOHY6A */
