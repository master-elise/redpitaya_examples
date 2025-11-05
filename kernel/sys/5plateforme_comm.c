#include <linux/version.h>  // VERSION_CODE
#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device *pdev;

static ssize_t gpio_simple_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{printk("%s",buf);return(count);}

static ssize_t gpio_simple_show(struct device *dev, struct device_attribute *attr, char *buf)
{return sprintf(buf, "show\n");}
// cat /sys/bus/platform/devices/gpio-simple.0/value 
// show

// JMF avoid error with user (non root) permissions on last bit by overriding 
//     the VERIFY_OCTAL_PERMISSIONS macro
#define VERIFY_OCTAL_PERMISSIONS(p) (p)

static DEVICE_ATTR(jmfr, 0444, gpio_simple_show, NULL);
static DEVICE_ATTR(valuew, 0222, NULL, gpio_simple_store);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void gpio_simple_remove(struct platform_device *pdev)
#else
static int gpio_simple_remove(struct platform_device *pdev)
#endif
{printk(KERN_ALERT "Au revoir\n");
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
 return(0);
#endif
}

static int gpio_simple_probe(struct platform_device *pdev)
{printk(KERN_ALERT "Bonjour\n");return 0;}

static struct platform_driver gpio_simple_driver = {
        .probe          = gpio_simple_probe,
        .remove         = gpio_simple_remove,
        .driver = {.name   = "gpio-comm",},
};

static int __init gpio_simple_init(void)
{platform_driver_register(&gpio_simple_driver);
 pdev = platform_device_register_simple("gpio-comm", 0, NULL, 0);
 device_create_file(&pdev->dev, &dev_attr_jmfr);
 device_create_file(&pdev->dev, &dev_attr_valuew);

 return 0;
}

static void __exit gpio_simple_exit(void)
{device_remove_file(&pdev->dev, &dev_attr_jmfr);
 device_remove_file(&pdev->dev, &dev_attr_valuew);
 platform_device_unregister(pdev);
 platform_driver_unregister(&gpio_simple_driver);
}

module_init(gpio_simple_init)
module_exit(gpio_simple_exit)

MODULE_DESCRIPTION("GPIO simple driver");
MODULE_LICENSE("GPL");
