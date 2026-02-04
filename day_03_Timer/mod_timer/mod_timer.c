#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list my_timer;

static void timer_callback(struct timer_list *t)
{
	pr_info("The timer has been started\n");
}

static int __init set_timer_init(void)
{
	pr_info("Module has been loaded: Successfully\n");

	timer_setup(&my_timer, timer_callback, 0);

	mod_timer(&my_timer, jiffies + 10 * HZ);
	pr_info("Module has been Started\n");

	return 0;
}

static void __exit set_timer_exit(void)
{
	pr_info("The kerenl has been successfully removed\n");
}

module_init(set_timer_init);
module_exit(set_timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish Kuamr");
MODULE_DESCRIPTION(" The setting up the mod_timer");

