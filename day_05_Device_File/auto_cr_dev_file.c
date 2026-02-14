#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>

dev_t dev = 0;
static struct class *dev_class;

static int __init first(void)
{
	if((alloc_chrdev_region(&dev,0,1,"monish_device")) < 0)
	{
		pr_err("Cannot allocated major and mnor number");
		return -1;
	}
	pr_info("Major: %d Minor: %d\n",MAJOR(dev),MINOR(dev));

	//the newer version is 
	dev_class = class_create("moni_class");
	if(IS_ERR(dev_class))
	{
		pr_err("cannot create the class");
		goto r_class;
	}

	if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"monish_device")))
	{
		pr_err("cannot create the device");
		goto r_device;
	}
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1); 
	return -1;
}

static void __exit last(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
	pr_info("kernel has been sucessfully removerd");
}

module_init(first);
module_exit(last);


MODULE_AUTHOR("Monish Kumar");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("The automation of device");
