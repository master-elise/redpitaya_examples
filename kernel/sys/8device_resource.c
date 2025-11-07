#include <linux/module.h>
#include <linux/platform_device.h>

static struct resource jmf_resources1[] = {
        {.start  = 0x378,
         .end    = 0x379,
         .flags  = IORESOURCE_IO, // PAS IORESOURCE_MEM
         .name   = "io-memory1"
        },
        {.start  = 0x37a,
         .end    = 0x37b,
         .flags  = IORESOURCE_IO,
         .name   = "io-memory2"
        },
    };

static struct resource jmf_resources2[] = {
        {.start  = 0x37c,
         .end    = 0x37d,
         .flags  = IORESOURCE_IO,
         .name   = "io-memory3"
        },
    };
// https://lwn.net/Articles/448499/

static struct platform_device *pdev1,*pdev2;

static int __init gpio_simple_init(void)
{pdev1=platform_device_register_simple("jmf",0,jmf_resources1,ARRAY_SIZE(jmf_resources1));
 pdev2=platform_device_register_simple("jmf",1,jmf_resources2,ARRAY_SIZE(jmf_resources2));
// $LINUX/Documentation/driver-model/platform.txt
 printk(KERN_ALERT "platform init");
 return 0;
}

static void __exit gpio_simple_exit(void)
{platform_device_unregister(pdev1);
 platform_device_unregister(pdev2);
 printk(KERN_ALERT "platform exit");
}
module_init(gpio_simple_init)
module_exit(gpio_simple_exit)
MODULE_LICENSE("GPL");
