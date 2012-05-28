/*
 * =====================================================================================
 *
 *       Filename:  proc_rw.c
 *
 *    Description:  读写char dev示例
 *
 *        Version:  1.0
 *        Created:  2009年07月30日 14时12分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki, 
 *        Company:  
 *
 * =====================================================================================
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/semaphore.h>

char *write_buf;
char *context = "hello world";
int total_size = 0;
dev_t cdev_no;
unsigned dev_minor = 0;
unsigned dev_nr = 1;
const char *dev_name = "demo_cdev";
struct cdev cdev_cdev;

DECLARE_MUTEX(mysem);
struct semaphore sem;

static ssize_t test_read(struct file *file, const char __user * buf,
			 size_t size, loff_t * offset)
{
	static int finished = 0;
	if (finished) {
		finished = 0;
		return 0;
	}

	int len;
	char *kbuf;

	len = strlen(context);
	kbuf = (char *)kmalloc(len, GFP_KERNEL);
	strcpy(kbuf, context);

	if (copy_to_user(buf, (void *)kbuf, len) < 0)
		return -EFAULT;
	else
		printk("copy to user success\n");

	kfree(kbuf);
	finished = 1;

	return len;
}

static ssize_t test_write(struct file *file, char __user * buf, size_t size,
			  loff_t * offset)
{
    if (down_interruptible(&mysem))
        return -ERESTARTSYS;

    int len;
    int max_size = 1024;
    ssize_t retval = -ENOMEM;
    if (total_size > max_size) {
        printk("total size > max size\n");
        goto nomem;
    }

    write_buf = kmalloc(32, GFP_KERNEL);
    if (!write_buf) {
        goto nomem;
    }

    memset(write_buf, 0, sizeof(write_buf));
    len = copy_from_user((void *)write_buf, buf, size);
    total_size += len;
    printk("copy from user data:%s total_size: %d\nsize:%d loffset:%d len:%d\n",
           write_buf, total_size, size, *offset, len);

    if (write_buf != NULL) {
        kfree(write_buf);
        write_buf = NULL;
    }

    up(&mysem);
    return len;

nomem:
    up(&mysem);
    return retval;
}

static int test_open(struct inode *node, struct file *file)
{
    //MOD_INC_USE_COUNT;
    printk("test cdev open\n");

    return 0;
}

static int test_close(struct inode *node, struct file *file)
{
    //MOD_DEC_USE_COUNT;
    printk("test cdev close\n");

    return 0;
}

static struct file_operations cdev_ops = {
    .owner = THIS_MODULE,
    .read = test_read,
    .write = test_write,
    .open = test_open,
    .release = test_close,
};

int make_dev()
{
    int err;
    char buf[32];

    cdev_init(&cdev_cdev, &cdev_ops);
    cdev_cdev.owner = THIS_MODULE;
    cdev_cdev.ops = &cdev_ops;
    cdev_cdev.dev = cdev_no;

    err = cdev_add(&cdev_cdev, cdev_no, 4);
    if (err)
        printk("cdev_add error\n");

    //获取dev的number
    print_dev_t(buf, cdev_no);
    printk("dev no %s", buf);

    return err;
}

static int __init chrdev_init(void)
{
    int res;
    unsigned major;

    res = alloc_chrdev_region(&cdev_no, dev_minor, dev_nr, dev_name);
    if (res < 0) {
        printk("alloc chrdev error\n");
        return res;
    }
    major = MAJOR(cdev_no);
    printk("chrdev major no %d\n", major);

    res = make_dev();

    if (res)
        printk("cdev init err\n");
    else
        printk("insmod cdev ok\n");

    return 0;
}

static void __exit chrdev_exit(void)
{
    cdev_del(&cdev_cdev);
    printk("rmmod cdev ok\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);
MODULE_LICENSE("GPL");
