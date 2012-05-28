/**
 *       File       class.c
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.0
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2010年06月02日 12时22分23秒
 *
 * =====================================================================================
 */

#include "class.h"

/**
 * @brief  
 * @param  param desc
 * @return 
 * @see    
 * @note   
 */
static void init(struct _DEMO *this, int id, int age, char *name)
{
	this->Attr.id = id;
	this->Attr.age = age;
	strcpy(this->Attr.name, name);
}

/**
 * @brief  
 * @param  param desc
 * @return 
 * @see    
 * @note   
 */
static void destroy(struct _DEMO *this)
{
	PDEBUG("destroy it\n");
}

/**
 * @brief  
 * @param  param desc
 * @return 
 * @see    
 * @note   
 */
static void display(DEMO * this)
{
	PDEBUG("name: %s, id: %d, age: %d\n", this->Attr.name,
	       this->Attr.id, this->Attr.age);
}

/**
 * @brief  在使用之前，对包进行初始化
 * @param  param desc
 * @return 
 * @see    
 * @note   
 */
void demo_init(DEMO * this, int id, int age, char *name)
{
    /** 初始化包的内部变量 */
    this->number = 0;

     /** 初始化构造和析构函数  */
	this->initialize = init;
	this->destroy = destroy;

    /** 初始化包方法 */
	this->Iface.display = display;

    this->initialize(this, id, age, name);
}

/**
 * @brief  在使用后，销毁包
 * @param  param desc
 * @return 
 * @see    
 * @note   
 */
void demo_free(DEMO * this)
{
    this->destroy(this);
    PDEBUG("free it\n");
}

