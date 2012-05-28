/*
 * =====================================================================================
 *
 *       Filename:  tmp.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年07月30日 14时27分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

/*  hello.c */

#include <linux/module.h>       /*  Needed by all modules */
//#include <linux/config.h>       /*  Needed for KERN_ALERT */
#include <linux/init.h>         /*  Needed for the module-macros */

static int __init hello_init(void)    // Module entry function specified by module_init()
{
	printk(KERN_ALERT "Hello,world!\n");

	return 0;
}

static void __exit hello_exit(void)  //Module exit function specified by module_exit()
{
	printk(KERN_ALERT "Goodbye,cruel world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("Dual BSD/GPL");  //should always exist or you’ll get a warning
MODULE_AUTHOR("BENSON"); //optional
MODULE_DESCRIPTION("STUDY_MODULE"); //optional

/*  Makefile */

# Makefile 2.6
obj-m += hello.o
KDIR:=/lib/modules/$(shell uname -r)/build
#PWD=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

obj-m := hello.o表示编译后生成hello.o模块。
$(KDIR) 指定了内核源码的路径，“M=”表示这是个外部模块，M=$(PWD) 指定了该模块文件所在的路径。

注： makefile预定义了$(PWD)变量，此处可以不必重复定义。
执行#make编译成功后
加载模块
#insmod hello.ko
#lsmod 输出内核已加载模块信息，可以查看到刚刚加载成功的hello模块
……
Module                  Size  Used by
hello                   5632  0
可以在日志里查看加载模块时的信息
#vi /var/log/messages
……
Sep 27 13:25:21 localhost kernel: Hello,world!
卸载模块
#rmmod hello.ko
#lsmod 发现hello模块已经被卸载
查看日志信息


