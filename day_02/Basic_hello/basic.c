#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init nm_init(void)
{
	pr_info("Hello linux device driver\n");
	pr_info("This is Monish\n");
	pr_info("The kernel module is successfully\n");
	return 0;
}

static void __exit nm_exit(void)
{
	pr_info("T kernel module is removed\n");
}

module_init(nm_init);
module_exit(nm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish");
MODULE_VERSION("1.1.1");
