#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fcntl.h>

#define MEM_SIZE 100

dev_t dev;
static struct class *dev_class;
static struct cdev led_cdev;

static struct file *uart_filp = NULL;
static loff_t uart_pos = 0;

static char *kernel_buffer;

//function Prototypes 
static int __init led_driver_init(void);
static void __exit led_driver_exit(void);

static int led_open(struct inode *inode, struct file *file);
static int led_release(struct inode *inode, struct file *file);
static ssize_t led_read(struct file *flip,char __user *buf,size_t len,loff_t *off);
static ssize_t led_write(struct file *flip,const char __user *buf,size_t len,loff_t *off);


static struct file_operations fops =
{
    .owner   = THIS_MODULE,
    .read    = led_read,
    .write   = led_write,
    .release = led_release,
    .open    = led_open,
};

//The file is opened
static int led_open(struct inode *inode, struct file *file)
{
    pr_info("The file is opened\n");
    return 0;
}

//The file is released
static int led_release(struct inode *inode, struct file *file)
{
    pr_info("The file is closed\n");
    return 0;
}

//The file is read
static ssize_t led_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    return 0;
}

//The file is write 
static ssize_t led_write(struct file *filp,const char __user *buf, size_t len, loff_t *off)
{
    if (len > MEM_SIZE)
        len = MEM_SIZE;

    if (copy_from_user(kernel_buffer, buf, len))
        return -EFAULT;

    /* Send to USB UART */
    if (uart_filp)
        kernel_write(uart_filp, kernel_buffer, len, &uart_pos);

    pr_info("Sent to STM32: %s\n", kernel_buffer);

    return len;
}

static int __init led_driver_init(void)   // This function is called when we use the command sudo insmod led.ko
{
    if (alloc_chrdev_region(&dev, 0, 1, "led_device") < 0)       // allocating major and minor numabers for the led_device
        return -1;

    cdev_init(&led_cdev, &fops);

    if (cdev_add(&led_cdev, dev, 1) < 0)
        goto r_unreg;

    dev_class = class_create("led_class");
    if (IS_ERR(dev_class))
        goto r_cdev;

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "led_device")))
        goto r_class;

    kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL);  // where the allocating memmory space to the kerenl to send or recive  the data
    if (!kernel_buffer)
        goto r_device;

    /* Open ttyUSB0 */
    uart_filp = filp_open("/dev/ttyUSB0", O_RDWR | O_NOCTTY, 0); // This line represent the pipeline between the led_device and ttyUSB0 
    if (IS_ERR(uart_filp))
    {
        pr_err("Failed to open /dev/ttyUSB0\n");
        goto r_device;
    }

    pr_info("LED Driver Inserted\n");// where this message can see in the sudo dmesg -W, if the driver is inserted correctly
    return 0;

r_device:
    device_destroy(dev_class, dev);
r_class:
    class_destroy(dev_class);
r_cdev:
    cdev_del(&led_cdev);
r_unreg:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit led_driver_exit(void)  // This function called when we use the command rmmod led.ko
{
    if (uart_filp)
        filp_close(uart_filp, NULL);

    kfree(kernel_buffer);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&led_cdev);
    unregister_chrdev_region(dev, 1);

    pr_info("LED Driver Removed\n"); // where this message can see in the dmesg, if the driver is removed correctly
}

module_init(led_driver_init);
module_exit(led_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish Kumar K");
MODULE_DESCRIPTION("Building a piplein between the USB and led_driver");
