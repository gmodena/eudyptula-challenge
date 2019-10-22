#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c140882fdd04");

static int __init hello_init(void)
{	
	printk(KERN_DEBUG "Hello World!\n");
	return 0;

}

static void __exit hello_exit(void)
{
	printk(KERN_DEBUG "Unloading... goodbye world\n");
}

module_init(hello_init);
module_exit(hello_exit);
