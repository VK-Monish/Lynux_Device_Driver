#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/fs.h>

dev_t dev = 0;

static int __init maj(void)
{
	if(alloc_chrdev_region(&dev,0,1,"Hello iam in the proc/dev file") < 0)
	{
		pr_err("The major number can't be allocated\n");
	}
	pr_info("The kernel has been successfully inserted\n");
	return 0;
}

static void __exit minn(void)
{
	unregister_chrdev_region(dev,1);

	pr_info("The kernel has been removed \n");
}

module_init(maj);
module_exit(minn);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish Kumar");
