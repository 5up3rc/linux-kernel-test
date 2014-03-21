#include <linux/init.h>
#include <linux/module.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/fdtable.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
static pid_t pid = -1;
static int fd = -1;

module_param(pid, int, 0644);
module_param(fd, int , 0644);

struct task_struct *get_proc(pid_t pid)
{
	struct pid *pid_struct = NULL;
	struct task_struct *mytask = NULL;

	pid_struct = find_get_pid(pid);
	if (!pid_struct)
		return NULL;

	mytask = pid_task(pid_struct, PIDTYPE_PID);
	
	return mytask;
}

int get_path(struct task_struct *mytask, int fd)
{
        struct file *myfile = NULL;
        struct files_struct *files = NULL;
	char path[100] = {'\0'};
        char *ppath = path;

        files = mytask->files;
        if (!files) {
		printk("files is null..\n");
		return -1;
	}
	myfile = files->fdt->fd[fd];
        if (!myfile) {
		printk("myfile is null..\n");
		return -1;
	}
	ppath = d_path(&(myfile->f_path), ppath, 100);
        
	printk("path:%s\n", ppath);
	return 0;
}

int get_full_path(pid_t pid, int fd)
{
        struct task_struct *mytask = NULL;
	int ret = -1;

	mytask = get_proc(pid);
	if (!mytask) {
		printk("get_proc error..\n");
		return -1;
	}
        ret = get_path(mytask, fd);        
	if (ret != 0) {
		printk("get_path error..\n");
		return -1;
	}
	return 0;
}

static int __init getpath_init(void)
{
	int ret = -1;
	printk("getpath module is starting..!\n");
	ret = get_full_path(pid, fd);
	return 0;
}

static void __exit getpath_exit(void)
{
	printk("getpath module is finishing....\n");
}

module_init(getpath_init);
module_exit(getpath_exit);
