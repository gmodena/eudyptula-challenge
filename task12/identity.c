// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

#define MAX_NAME_LEN 20

struct identity {
	char name[MAX_NAME_LEN];
	int id;
	bool busy;
	struct list_head node;
};

// init list at compile time
LIST_HEAD(identity_list);

int identity_create(char *name, int id)
{
	struct identity *entry = kmalloc(sizeof(struct identity), GFP_KERNEL);

	if (entry == NULL)
		return -ENOMEM;
	strscpy(entry->name, name, MAX_NAME_LEN);
	entry->id = id;
	entry->busy = false;

	list_add(&entry->node, &identity_list);

	return 0;
}

struct identity *identity_find(int id)
{
	struct identity *entry = NULL;

	list_for_each_entry(entry, &identity_list, node)
		if (entry->id == id)
			return entry;

	return NULL;
}

void identity_destroy(int id)
{
	struct identity *entry = identity_find(id);

	if (entry != NULL) {
		list_del(&entry->node);
		kfree(entry);
	}
}


static int __init identity_init(void)
{
	struct identity *temp;
	int ret = 0;

	ret = identity_create("Alice", 1);
	if (ret)
		return ret;

	ret = identity_create("Bob", 2);
	if (ret)
		return ret;

	ret = identity_create("Dave", 3);
	if (ret)
		return ret;

	ret = identity_create("Gena", 10);
	if (ret)
		return ret;

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

static void __exit identity_exit(void)
{
	pr_debug("Unloading... goodbye world\n");
}

module_init(identity_init);
module_exit(identity_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c140882fdd04");
MODULE_DESCRIPTION("Liked list in kernel space");
