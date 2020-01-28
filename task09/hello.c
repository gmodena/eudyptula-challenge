#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/jiffies.h>

#define AUTHOR_ID "c140882fdd04"
#define ID_PERMISSION 0664

struct dentry *ret;
static DEFINE_MUTEX(foo_mtx);
char msg[PAGE_SIZE];


static ssize_t jiffies_read(struct kobject *kobj, struct kobj_attribute *attr,
			 char *buf)
{
	return sprintf(buf, "%lu\n", jiffies);
}


ssize_t id_read(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	strncpy(buf, AUTHOR_ID, strlen(AUTHOR_ID));
	return strlen(AUTHOR_ID);
}

ssize_t id_write(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{

	if (strlen(AUTHOR_ID) != count-1) || strncmp(buf, AUTHOR_ID, strlen(AUTHOR_ID) != 0)
		return -EINVAL;

	strncpy(AUTHOR_ID, buf, count);

	return count;
}

static ssize_t foo_read(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	ssize_t ret = strlen(msg);

	mutex_lock(&foo_mtx);
	strncpy(buf, msg, len);
	mutex_unlock(&foo_mtx);

	return ret;
}

static ssize_t foo_write(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{

	if (count > PAGE_SIZE)
		return -EINVAL;
	
	mutex_lock(&foo_mtx);
	strncpy(msg, buf, count);
	mutex_unlock(&foo_mtx);

	return count;
}

static struct kobj_attribute foo_attribute =
__ATTR_RW(foo);

static struct kobj_attribute id_attribute =
__ATTR(id, ID_PERMISSION, id_read, id_write);


static struct kobj_attribute jiffies_attribute = __ATTR_RO(jiffies);
__ATTR(id, JIFFIES_PERMISSION, id_read, id_write);



static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&id_attribute.attr,
	&jiffies_attribute.attr,
	NULL, /* need to NULL terminate the list of attributes */
};

static struct attribute_group hello_group = {
	.attrs = attrs,
};

static struct kobject *hello_kobj;


static int __init hello_init(void)
{
	hello_kobj = kobject_create_and_add("eudyptula", kernel_kobj);
	if (!hello_kobj)
		return -ENOMEM;

	int ret = sysfs_create_group(hello_kobj, &hello_group);
	if (ret) {
		kobject_put(hello_kobj);
	}
	return ret;
}

static void __exit hello_exit(void)
{
	kobject_put(hello_kobj);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
