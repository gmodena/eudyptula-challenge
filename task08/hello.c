#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/jiffies.h>

#define AUTHOR_ID "c140882fdd04"
#define ID_PERMISSION 0666
#define JIFFIES_PERMISSION 0444
#define FOO_PERMISSION 0644

struct dentry *ret;
static DEFINE_MUTEX(foo_mtx);
char msg[PAGE_SIZE];

ssize_t id_read(struct file *filp,
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

ssize_t id_write(struct file *filp,
	const char __user *buf,
	size_t count,
	loff_t *f_pos)
{
	ssize_t written = 0;
	char my_buf[12];

	written = simple_write_to_buffer(my_buf, count, f_pos,  buf, count);
	if (written < 0)
		return written;

	if (!strncmp(my_buf, AUTHOR_ID, strlen(AUTHOR_ID))
		&& strlen(AUTHOR_ID) == count-1)
		return count;

	return -EINVAL;
}

static ssize_t foo_read(struct file *file, char __user *buf,
	size_t count, loff_t *ppos)
{
	int ret = -EINVAL;

	mutex_lock(&foo_mtx);

	ret = simple_read_from_buffer(buf, count, ppos, msg,
		strlen(msg));

	mutex_unlock(&foo_mtx);

	return ret;
}

static ssize_t foo_write(struct file *file, char const __user *buf,
	size_t count, loff_t *ppos)
{
	ssize_t len;

	mutex_lock(&foo_mtx);

	len = simple_write_to_buffer(msg, PAGE_SIZE, ppos, buf,
		count);

	mutex_unlock(&foo_mtx);

	return len;
}

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write
};


static const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write
};

static int __init hello_init(void)
{
	ret = debugfs_create_dir("eudyptula", NULL);
	if (!debugfs_create_file("id", ID_PERMISSION, ret, NULL,
		&id_fops))
		return -ENODEV;

	if (!debugfs_create_u32("jiffies", JIFFIES_PERMISSION, ret, (u32 *)&jiffies))
		return -ENODEV;

	if (!debugfs_create_file("foo", FOO_PERMISSION, ret, NULL,
		&foo_fops))
		return -ENODEV;

	return 0;
}

static void __exit hello_exit(void)
{
	debugfs_remove_recursive(ret);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
