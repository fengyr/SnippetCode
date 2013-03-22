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
#include <linux/proc_fs.h>

MODULE_LICENSE("Dual BSD/GPL");

struct test_proc_dev {
    struct proc_dir_entry *entry;
    char proc_name[64];
};

struct test_proc_dev g_proc_dev; 

int proc_read(char *page, char **start, off_t off,
			  int count, int *eof, void *data) 
{
    char *msg = "this is a proc test";
    sprintf(page, "--%s--", msg); 

    *eof = 1;
    return strlen(msg)+4;
}


void init_proc_dev(struct test_proc_dev *proc) 
{
    printk(KERN_DEBUG "init_proc_dev");
    sprintf(proc->proc_name, "%s", "test_proc_file");
    create_proc_read_entry(proc->proc_name, 0, NULL, proc_read, NULL);
}

void remove_proc_dev(struct test_proc_dev *proc)
{
    printk(KERN_DEBUG "remove_proc_dev");
    remove_proc_entry(proc->proc_name, NULL);
}

static int test_proc_init(void)
{
    init_proc_dev(&g_proc_dev);
    return 0;
}

static void test_proc_exit(void)
{
    remove_proc_dev(&g_proc_dev);
}

module_init(test_proc_init);
module_exit(test_proc_exit);

