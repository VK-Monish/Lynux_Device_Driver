#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static int valueETX;
static int arr_valueETX[4];
static char *nameETX;
static int cb_valueETX;

module_param(valueETX, int, S_IRWXU);
module_param(nameETX, charp, S_IRWXU);
module_param_array(arr_valueETX, int, NULL, S_IRWXU);

MODULE_PARM_DESC(valueETX, "An integer value");
MODULE_PARM_DESC(nameETX, "Name string");
MODULE_PARM_DESC(arr_valueETX, "Integer array");
MODULE_PARM_DESC(cb_valueETX, "Callback integer");

static int notif_param(const char *val, const struct kernel_param *kp)
{
    int res = param_set_int(val, kp);
    if (res)
        return res;

    if (cb_valueETX < 0)
        return -EINVAL;

    pr_info("Callback called, cb_valueETX = %d\n", cb_valueETX);
    return 0;
}

static const struct kernel_param_ops my_param_ops = {
    .set = notif_param,
    .get = param_get_int,
};

module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX, S_IRWXU);

static int __init call_back(void)
{
    int i;

    pr_info("valueETX = %d\n", valueETX);
    pr_info("cb_valueETX = %d\n", cb_valueETX);
    pr_info("nameETX = %s\n", nameETX ? nameETX : "NULL");

    for (i = 0; i < ARRAY_SIZE(arr_valueETX); i++)
        pr_info("arr_valueETX[%d] = %d\n", i, arr_valueETX[i]);

    pr_info("Kernel module inserted successfully\n");
    return 0;
}

static void __exit n_call_back(void)
{
    pr_info("Kernel module removed successfully\n");
}

module_init(call_back);
module_exit(n_call_back);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monish Kumar");
MODULE_DESCRIPTION("Module parameter callback example");

