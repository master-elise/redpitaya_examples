#include <linux/version.h>  // VERSION_CODE
#include <linux/module.h>
#include <linux/platform_device.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void simple_remove(struct platform_device *);
#else
static int simple_remove(struct platform_device *);
#endif

static int simple_probe(struct platform_device *);

static struct platform_driver simple_driver = {
        .probe          = simple_probe,
        .remove         = simple_remove,
        .driver = {.name   = "jmf_simple_demo",},
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void simple_remove(struct platform_device *pdev)
#else
static int simple_remove(struct platform_device *pdev)
#endif
{printk(KERN_ALERT "Driver au revoir\n");
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
 return(0);
#endif
}

static int simple_probe(struct platform_device *pdev)
{printk(KERN_ALERT "Driver bonjour\n");return 0;}

static int __init simple_init(void)
{platform_driver_register(&simple_driver);
 return 0;
}

static void __exit simple_exit(void)
{platform_driver_unregister(&simple_driver);
}

module_init(simple_init)
module_exit(simple_exit)

MODULE_DESCRIPTION("simple driver");
MODULE_LICENSE("GPL");
