#include <linux/version.h>  // VERSION_CODE
#include <linux/module.h>
#include <linux/platform_device.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void gpio_simple_remove(struct platform_device *);
#else
static int gpio_simple_remove(struct platform_device *);
#endif
static int gpio_simple_probe(struct platform_device *);

static ssize_t simple_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{printk("%s",buf);return(count);}

static ssize_t simple_show(struct device *dev, struct device_attribute *attr, char *buf)
{return sprintf(buf, "show\n");}
// cat /sys/bus/platform/devices/gpio-simple.0/value 
// show

static DEVICE_ATTR(valuer, 0440, simple_show, NULL);
static DEVICE_ATTR(valuew, 0220, NULL, simple_store);

static struct platform_driver gpio_simple_driver = {
        .probe          = gpio_simple_probe,
        .remove         = gpio_simple_remove,
        .driver = {.name   = "jmf",},
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void gpio_simple_remove(struct platform_device *pdev)
#else
static int gpio_simple_remove(struct platform_device *pdev)
#endif
{printk(KERN_ALERT "jmf bye %d\n",pdev->id);
 device_remove_file(&pdev->dev, &dev_attr_valuer);
 device_remove_file(&pdev->dev, &dev_attr_valuew);
 printk(KERN_ALERT "jmf bye %d\n",pdev->id);
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
 return 0;
#endif
}

static int gpio_simple_probe(struct platform_device *pdev)
{struct resource *r1,*r2;
	  
 device_create_file(&pdev->dev, &dev_attr_valuer);
 device_create_file(&pdev->dev, &dev_attr_valuew);
 printk("resources: %d",pdev->num_resources);
 r1= platform_get_resource(pdev, IORESOURCE_IO, 0);
 printk(KERN_ALERT "start1 %d %x\n",pdev->id,(int)r1->start);
 if (pdev->num_resources>1)
   {r2= platform_get_resource(pdev, IORESOURCE_IO, 1);
    printk(KERN_ALERT "start2 %d %x\n",pdev->id,(int)r2->start);
   } // $LINUX/Documentation/driver-model/platform.txt
//  * platform_device.id ... the device instance number, or else "-1"
//    to indicate there's only one.
 return 0;
}

module_platform_driver(gpio_simple_driver);
MODULE_LICENSE("GPL");
