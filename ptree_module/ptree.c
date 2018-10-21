#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

MODULE_LICENSE("GPL");
#define __NR_hello 325

static int (*oldcall)(void);

static unsigned long **find_sys_call_table(void) {
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;

	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close) {
			printk(KERN_INFO "Found syscall table at address: 0x%02lX",
					(unsigned long) sct);
			return sct;
		}

		offset += sizeof(void *);
	}

	return NULL;
}

static long sys_hello(void){
	printk(KERN_INFO "hello, world!\n");
	return 0;
}

static int addsyscall_init(void) {
	long *syscall = (long *)find_sys_call_table();
	oldcall = (int(*)(void))(syscall[__NR_hello]);	    
	write_cr0 (read_cr0 () & (~ 0x10000));	        
	syscall[__NR_hello] = (unsigned long) sys_hello;
	write_cr0 (read_cr0 () | 0x10000);
	printk(KERN_INFO "module load\n");	    
	return 0;
}

static void addsyscall_exit(void) {
	long *syscall = (long *) find_sys_call_table();        
	write_cr0 (read_cr0 () & (~ 0x10000));	    
	syscall[__NR_hello] = (unsigned long) oldcall;	        
	write_cr0 (read_cr0 () | 0x10000);		    
	printk(KERN_INFO "module exit\n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);
