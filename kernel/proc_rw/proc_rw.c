/*
 * =====================================================================================
 *
 *       Filename:  proc_rw.c
 *
 *    Description:  读写/PROC示例
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
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>

#define MY_PROC_NAME "my_test_proc"
char *context = "hello world";

static ssize_t test_write(struct file *file, const char __user *buf, size_t size, loff_t *offset )
{
	static int finished = 0;
	if (finished)
	{
		finished = 0;
		return 0;
	}

	unsigned long len;
	len = strlen(context);
	if (copy_to_user(buf, (void*)context, len))
		return -EFAULT;
	else
		printk("copy to user success\n");

	finished = 1;

	return len;
}

static ssize_t test_read(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	static int finished = 0;
	if (finished) {
		finished = 0;
		return 0;
	}

	unsigned long len;
	char tobuf[1024];
	memset(tobuf, 0, sizeof(tobuf));
	len = copy_from_user((void*)tobuf, buf, size);
	printk("copy from user data: %s\n", tobuf);

	finished = 1;

	return len;
}

static int test_open(struct inode *node, struct file *file)
{
	//MOD_INC_USE_COUNT;
	printk("test file open\n");

	return 0;
}

static int test_close(struct inode *node, struct file *file)
{
	//MOD_DEC_USE_COUNT;
	printk("test file close\n");

	return 0;
}

static struct file_operations MY_FILE_OPERS = {
	.owner = THIS_MODULE, 	\
	.read = test_write, 	\
	.write = test_read, 	\
	.open = test_open, 		\
	.release = test_close, 	\
};

static int __init test_init(void)
{
	struct proc_dir_entry *myety;
	myety = create_proc_entry(MY_PROC_NAME, S_IFREG|S_IRUGO|S_IWUGO, NULL);

	if ( myety == NULL )
		printk("create my proc file error\n");
	else {
		printk("create my proc file ok\n");
		printk("low_no:%d name_len:%d name:%s link:%d\n", myety->low_ino, myety->namelen, myety->name, myety->nlink);
		myety->proc_fops = &MY_FILE_OPERS;
	}

	return 0;
}

static void __exit test_exit(void)
{
	remove_proc_entry(MY_PROC_NAME, NULL);
	printk("rmmod test\n");
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");


