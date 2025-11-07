// while true; do cat <  /sys/devices/platform/gpio-comm.0/valuer ;done
#include <linux/version.h>  // VERSION_CODE
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/version.h>

struct timer_list exp_timer;
struct mutex m;
int delay = 3;

static ssize_t gpio_simple_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{printk("%s",buf);return(count);}

static ssize_t gpio_simple_show(struct device *dev, struct device_attribute *attr, char *buf)
{mutex_lock(&m);  // lock, et ne sera delocke' que par timer
 return sprintf(buf, "show\n");
}
// cat /sys/bus/platform/devices/gpio-simple.0/value
// show

// https://stackoverflow.com/questions/40776170/cannot-set-attribute-permissions-to-0666-in-sysfs
#undef VERIFY_OCTAL_PERMISSIONS
#define VERIFY_OCTAL_PERMISSIONS(perms) (perms)

static DEVICE_ATTR(valuer, 0444, gpio_simple_show, NULL);
static DEVICE_ATTR(valuew, 0222, NULL, gpio_simple_store);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void gpio_simple_remove(struct platform_device *pdev)
#else
static int gpio_simple_remove(struct platform_device *pdev)
#endif
{printk(KERN_ALERT "Au revoir\n");
 device_remove_file(&pdev->dev, &dev_attr_valuer);
 device_remove_file(&pdev->dev, &dev_attr_valuew);
 platform_device_unregister(pdev);
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
 return(0);
#endif
}

static int gpio_simple_probe(struct platform_device *pdev)
{device_create_file(&pdev->dev, &dev_attr_valuer);
 device_create_file(&pdev->dev, &dev_attr_valuew);
 printk(KERN_ALERT "Bonjour\n");
 return 0;
}

static struct platform_driver gpio_simple_driver = {
        .probe          = gpio_simple_probe,
        .remove         = gpio_simple_remove,
        .driver = {.name   = "gpio-comm",},
};

#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,14,0)
static void do_something(unsigned long data)
#else
static void do_something(struct timer_list *t)
#endif
{printk(KERN_INFO "plop: %lu",jiffies);
 mutex_unlock(&m);
 mod_timer(&exp_timer, jiffies + delay*HZ);
}

static int __init gpio_simple_init(void)
{platform_driver_register(&gpio_simple_driver);
 platform_device_register_simple("gpio-comm", 0, NULL, 0);
 mutex_init(&m);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,14,0)
 init_timer_on_stack(&exp_timer);  // -> replaced since 4.14
 exp_timer.data = 0;                     // obsolete
 exp_timer.function = do_something;      // dupplicate with the new timer_setup
#else
 timer_setup(&exp_timer,do_something,0);
#endif
 exp_timer.expires = jiffies + delay * HZ; // HZ specifies number of clock ticks generated per second
 add_timer(&exp_timer);
 mutex_lock(&m);  // lock, et ne sera delocke' que par timer
 return 0;
}

static void __exit gpio_simple_exit(void)
{platform_driver_unregister(&gpio_simple_driver);
 del_timer(&exp_timer);
}

module_init(gpio_simple_init)
module_exit(gpio_simple_exit)

MODULE_DESCRIPTION("GPIO simple driver");
MODULE_LICENSE("GPL");
