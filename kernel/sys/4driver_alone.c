#include <linux/module.h>
#include <linux/platform_device.h>

static void simple_remove(struct platform_device *);
static int simple_probe(struct platform_device *);

static struct platform_driver simple_driver = {
        .probe          = simple_probe,
        .remove         = simple_remove,
        .driver = {.name   = "jmf_simple",},
};

static void simple_remove(struct platform_device *pdev)
{printk(KERN_ALERT "Au revoir\n");}

static int simple_probe(struct platform_device *pdev)
{printk(KERN_ALERT "Bonjour\n");return 0;}

module_platform_driver(simple_driver);
MODULE_LICENSE("GPL");
