#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device *pdev1,*pdev2;

static void gpio_simple_remove(struct platform_device *pdev)
{printk(KERN_ALERT "Au revoir %d\n",pdev->id);}

static int gpio_simple_probe(struct platform_device *pdev)
{printk(KERN_ALERT "Bonjour %d\n",pdev->id);return 0;}

static struct platform_driver gpio_simple_driver = {
        .probe          = gpio_simple_probe,
        .remove         = gpio_simple_remove,
        .driver = {.name   = "jmf-demo",},
};

static int __init gpio_simple_init(void)
{platform_driver_register(&gpio_simple_driver);
 pdev1 = platform_device_register_simple("jmf-demo", 0, NULL, 0);
 pdev2 = platform_device_register_simple("jmf-demo", 1, NULL, 0);
 return 0;
}

static void __exit gpio_simple_exit(void)
{platform_device_unregister(pdev1);
 platform_device_unregister(pdev2);
 platform_driver_unregister(&gpio_simple_driver);
}
module_init(gpio_simple_init)
module_exit(gpio_simple_exit)

MODULE_DESCRIPTION("GPIO simple driver");
MODULE_LICENSE("GPL");
