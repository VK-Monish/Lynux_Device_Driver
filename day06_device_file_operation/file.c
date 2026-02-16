#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/err.h>

// intilizing the struct
dev_t dev = 0;
static struct class *dev_class;
static struct cdev moni_cdev;

//function protoype 
static int  __init moni_cdev_int(void);
static void __exit moni_cdev_ext(void);
static int moni_open(struct inode *inode,struct file *file);
static int moni_release(struct inode *inode,struct file *file);
static ssize_t moni_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t moni_write(struct file *filp, const char __user*buf, size_t len, loff_t *off);

//file opereation 
static struct file_operations fops = 
{
	.owner   = THIS_MODULE,
	.read    = moni_read,
	.write   = moni_write,
	.open  	 = moni_open,
	.release = moni_release,
};

// This function is called after we open the device file
static int moni_open(struct inode *inode, struct file *file)
{
	pr_info("The opne driver function is called\n");
	return 0;
}

//This function is called after the we release the device file
static int moni_release(struct inode *inode, struct file *file)
{
	pr_info("The release function is called\n");
	return 0;
}

// This funcition is called after we read the device file 
static ssize_t moni_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	pr_info("The read function is called\n");
	return 0; 
}

//This funcition is called after we write in the device file
static ssize_t moni_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("The write function is called\n");
	return len;
}


static int __init moni_cdev_int(void)
{
	if(alloc_chrdev_region(&dev,0,1,"monish_device") < 0)
	{
		pr_err("cannot create the major and minor number\n");
		return -1;
	}
	cdev_init(&moni_cdev,&fops);

	if(cdev_add(&moni_cdev,dev,1))
	{
		pr_err("cannot add the cdev");
		goto r_class;
	}

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
static void __exit moni_cdev_ext(void)
{
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
	cdev_del(&moni_cdev);
	unregister_chrdev_region(dev,1);
	pr_info("The driver is removed");
}


module_init(moni_cdev_int);
module_exit(moni_cdev_ext);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MONISH KUMAR");
MODULE_DESCRIPTION("THE FILE OPERATION");
