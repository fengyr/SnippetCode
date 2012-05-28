/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  用户端发送示例
 *
 *        Version:  1.0
 *        Created:  2009年07月30日 13时49分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <linux/module.h>
#include <linux/sched.h>

static void __exit test_exit(void)
{
	printk("hi, i go\n");
}

module_exit(test_exit);
MODULE_LICENSE("GPL");
