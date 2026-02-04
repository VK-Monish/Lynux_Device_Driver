#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

dev_t dev = MKDEV(230,0);

static int __init maj(void)
{
	register_chrdev_region(dev,1,"Hello");
	pr_info(" Major: %d\n Minor: %d\n",MAJOR(dev),MINOR(dev));
	pr_info("The kernel module has been successfully inserted\n");
	return 0;
}

static void __exit minn(void)
{
	unregister_chrdev_region(dev,1);
	pr_info("The kernel module has been successfully removed\n");
}

module_init(maj);
module_exit(minn);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish Kumar");
MODULE_DESCRIPTION("Hello this is Day_04");
