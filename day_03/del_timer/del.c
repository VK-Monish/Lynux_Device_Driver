#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list my_timer;

static void timer_callback(struct timer_list *t)
{
	pr_info("The timer callback has been started\n");
}

static int __init my_timer_set(void)
{
	pr_info("The timer has been loaded\n");

	timer_setup(&my_timer,timer_callback,0);

	mod_timer(&my_timer, jiffies + 3 * HZ);
	return 0;
}

static void __exit my_timer_exit(void)
{
	del_timer_sync(&my_timer);

	pr_info("The Timer has been removed\n");

	pr_info("The module is removed successfully\n");
}

module_init(my_timer_set);
module_exit(my_timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish Kumar");
