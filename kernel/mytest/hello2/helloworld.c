/**
 *       File       hellworld.c
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.00.00
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2011年11月23日 08时06分41秒
 *
 * =====================================================================================
 */

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);

static int hello_init(void)
{
    printk(KERN_ALERT "Hello, %s\n", whom);
    return 0;
}

static void hello_exit(void)
{

    printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);



