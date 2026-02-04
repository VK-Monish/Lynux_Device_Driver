#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>

static struct timer_list my_timer;

static void timer_callback(struct timer_list *t)
{
	pr_info("timer callback is executed\n");
}

static int __init timer_setup_init(void)
{
	pr_info("The timer module is loaded\n");

	timer_setup(&my_timer,timer_callback,0);

	pr_info("The timer is successfully initialized");
	return 0;
}

static void __exit timer_setup_exit(void)
{
	pr_info("The timer is successfully removed");
}

module_init(timer_setup_init);
module_exit(timer_setup_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish Kumar");
MODULE_DESCRIPTION("Runtime-Configurable Kernel Module (Bootlin-style)");
