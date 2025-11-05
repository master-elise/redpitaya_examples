#include <linux/version.h>  // VERSION_CODE
#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device *pdev;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void gpio_simple_remove(struct platform_device *);
#else
static int gpio_simple_remove(struct platform_device *);
#endif
static int gpio_simple_probe(struct platform_device *);

static struct platform_driver gpio_simple_driver = {
        .probe          = gpio_simple_probe,
        .remove         = gpio_simple_remove,
        .driver = {.name   = "jmf-simple-demo",},
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
void gpio_simple_remove(struct platform_device *pdev)
#else
int gpio_simple_remove(struct platform_device *pdev)
#endif
{printk(KERN_ALERT "Au revoir\n");
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
 return(0);
#endif
}

static int gpio_simple_probe(struct platform_device *pdev)
{printk(KERN_ALERT "Bonjour\n");return 0;}

static int __init gpio_simple_init(void)
{printk(KERN_ALERT "Hello\n");
 platform_driver_register(&gpio_simple_driver);
 pdev = platform_device_register_simple("jmf-simple-demo", 0, NULL, 0);
 return 0;
}

static void __exit gpio_simple_exit(void)
{printk(KERN_ALERT "Bye\n");
 platform_device_unregister(pdev);
 platform_driver_unregister(&gpio_simple_driver);
}
module_init(gpio_simple_init)
module_exit(gpio_simple_exit)

MODULE_DESCRIPTION("GPIO simple driver");
MODULE_LICENSE("GPL");
