#include <linux/version.h>  // VERSION_CODE
#include <linux/module.h>
#include <linux/platform_device.h>

static ssize_t gpio_simple_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{printk("%s",buf);return(count);}

static ssize_t gpio_simple_show(struct device *dev, struct device_attribute *attr, char *buf)
{return sprintf(buf, "show\n");}
// cat /sys/bus/platform/devices/gpio-simple.0/value
// show

/* warning! need write-all permission so overriding check */
#undef VERIFY_OCTAL_PERMISSIONS
#define VERIFY_OCTAL_PERMISSIONS(perms) (perms)
static DEVICE_ATTR(value, 0666, gpio_simple_show, gpio_simple_store);
//static DEVICE_ATTR(valuew, 0220, NULL, gpio_simple_store);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void gpio_simple_remove(struct platform_device *pdev)
#else
static int gpio_simple_remove(struct platform_device *pdev)
#endif
{printk(KERN_ALERT "Au revoir\n");
 device_remove_file(&pdev->dev, &dev_attr_value);
 platform_device_unregister(pdev);
// device_remove_file(&pdev->dev, &dev_attr_valuew);
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
 return(0);
#endif
}

static int gpio_simple_probe(struct platform_device *pdev)
{device_create_file(&pdev->dev, &dev_attr_value);
// device_create_file(&pdev->dev, &dev_attr_valuew);
 printk(KERN_ALERT "Bonjour\n");return 0;
}

static struct platform_driver gpio_simple_driver = {
        .probe          = gpio_simple_probe,
        .remove         = gpio_simple_remove,
        .driver = {.name   = "gpio-comm-rw",},
};

static int __init gpio_simple_init(void)
{platform_driver_register(&gpio_simple_driver);
 platform_device_register_simple("gpio-comm-rw", 0, NULL, 0);
 return 0;
}

static void __exit gpio_simple_exit(void)
{
 platform_driver_unregister(&gpio_simple_driver);
}

module_init(gpio_simple_init)
module_exit(gpio_simple_exit)

MODULE_DESCRIPTION("GPIO simple driver");
MODULE_LICENSE("GPL");
