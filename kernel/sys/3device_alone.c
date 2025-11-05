#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device *pdev1,*pdev2;

static int __init simple_init(void)
{printk(KERN_ALERT "Device init");
 pdev1 = platform_device_register_simple("jmf_simple_demo", 0, NULL, 0);
 pdev2 = platform_device_register_simple("jmf_simple_demo", 1, NULL, 0);
 return 0;
}

static void __exit simple_exit(void)
{printk(KERN_ALERT "Device exit");
 platform_device_unregister(pdev1);
 platform_device_unregister(pdev2);
}

module_init(simple_init)
module_exit(simple_exit)

MODULE_DESCRIPTION("GPIO simple driver");
MODULE_LICENSE("GPL");
