/*
 * =====================================================================================
 *
 *       Filename:  kernel.c
 *
 *    Description:  内核端示例
 *
 *        Version:  1.0
 *        Created:  2009年07月30日 14时12分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

static __initdata char *whom = "world";
static __initdata int  numc = 1;

module_param(whom, charp, S_IRUGO);
module_param(numc, int, S_IRUGO);


static int __init test_init(void)
{
	int i;
	for (i=0; i<numc; i++) {
		printk("hello %s, i come\n", whom);
	}

	printk("current name %s pid %d\n", current->comm, current->pid);
	return 0;
}

module_init(test_init);
MODULE_LICENSE("GPL");


