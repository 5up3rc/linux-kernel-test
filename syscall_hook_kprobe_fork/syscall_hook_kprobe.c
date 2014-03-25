#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/ptrace.h>

MODULE_LICENSE("GPL");

static struct jprobe jp;

asmlinkage int new_do_fork(unsigned long clone_flags,
		unsigned long stack_start,
		struct pt_regs *regs,
		unsigned long stack_size,
		int __user *parent_tidptr,
		int __user *child_tidptr)
{
	printk("do something before sys_fork..\n");
	jprobe_return();
	return 0;
}

static int __init syscall_hook_init(void)
{
	int ret = -1;
	
	printk("syscall_hook module is starting..!\n");
	jp.entry = (kprobe_opcode_t *)new_do_fork;
	jp.kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("do_fork");
	if (!jp.kp.addr) {
		printk("can't find the address of sys_fork..\n");
		return -1;
	}

	if ((ret = register_jprobe(&jp)) < 0) {
		printk("register_jprobe failed, returned %d\n", ret);
		return -1;
	}
	return 0;
}

static void __exit syscall_hook_exit(void)
{
	printk("syscall_hook module is finishing....\n");
	unregister_jprobe(&jp);
}

module_init(syscall_hook_init);
module_exit(syscall_hook_exit);
