#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "eudyptula"
#define AUTHOR_ID "c140882fdd04"

ssize_t hello_read(struct file *filp,
	char __user *buf,
	size_t count,
	loff_t *f_pos)
{
	return simple_read_from_buffer(buf,
		count,
		f_pos,
		AUTHOR_ID,
		strlen(AUTHOR_ID));
}

ssize_t hello_write(struct file *filp,
	const char __user *buf,
	size_t count,
	loff_t *f_pos)
{
	ssize_t written = 0;
	char my_buf[strlen(AUTHOR_ID)];

	written = simple_write_to_buffer(my_buf, count, f_pos,  buf, count);
	if (written < 0)
		return written;

	if (!strncmp(my_buf, AUTHOR_ID, strlen(AUTHOR_ID))
		&& strlen(AUTHOR_ID) == count-1)
		return count;

	return -EINVAL;
}

static const struct file_operations fops = {
	.write = hello_write,
	.read = hello_read
};

struct miscdevice device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &fops,
};

static int __init hello_init(void)
{
	int ret;

	ret = misc_register(&device);

	if (ret < 0)
		pr_debug("Failed to register misc device\n");
	else
		pr_debug("Hello World!\n");
	return ret;
}

static void __exit hello_exit(void)
{
	misc_deregister(&device);
	pr_debug("Unloading... goodbye world\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c140882fdd04");
MODULE_DESCRIPTION("Hello World module");
