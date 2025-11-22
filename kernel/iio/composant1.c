#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>   // kzalloc
#include <linux/module.h>
#include <linux/iio/iio.h>
#include <linux/mutex.h>                            
#include <linux/mod_devicetable.h>                            
#include <linux/version.h>                            

#include "composant1.h"

int valeur=0;
struct mutex mymutex;

static int composant_read_raw(struct iio_dev *indio_dev,
  struct iio_chan_spec const *chan, int *val, int *val2, long m)  // m = mask
{struct composant_state *st = iio_priv(indio_dev); // recupere la description du pilote
 switch (m) {
 case IIO_CHAN_INFO_SCALE:  // si on avait active' BIT(IIO_CHAN_INFO_SCALE)
   *val = st->vref_mv;
   *val2 = chan->scan_type.realbits;
 return IIO_VAL_INT; // ou IIO_VAL_FRACTIONAL_LOG2;
 case IIO_CHAN_INFO_RAW:
   mutex_lock(&mymutex);   // bloque
   *val=valeur; 
   valeur++;
   mutex_unlock(&mymutex); // debloque
   return IIO_VAL_INT;
 }
 return -EINVAL;
}

static int composant_write_raw(struct iio_dev *indio_dev,
  struct iio_chan_spec const *chan, int val, int val2, long mask)
{switch (mask) {
  case IIO_CHAN_INFO_RAW:
     mutex_lock(&mymutex);   // bloque
     valeur=val;
     mutex_unlock(&mymutex); // debloque
     return 0;
  break;
 }
 return 0;
}

static const struct iio_info composant_info = {
	.write_raw = composant_write_raw,
	.read_raw = composant_read_raw,
//	.driver_module = THIS_MODULE,
};

// il faut absolument autant de channels que COMPOSANT_CHANNELS dans composant.h
const struct iio_chan_spec composant_channels[] = {
 {.type = IIO_ACCEL,                             \
  .channel = (100),                              \
  .info_mask_separate = BIT(IIO_CHAN_INFO_RAW),  \
  .address = (0),                                \
  .indexed = 1,                                  \
 },
 {.type = IIO_VOLTAGE,                           \
  .channel = (101),                              \
  .output = 1,                                   \
  .info_mask_separate = BIT(IIO_CHAN_INFO_RAW)|BIT(IIO_CHAN_INFO_SCALE),  \
  .address = (1),                                \
  .indexed = 1,                                  \
 }
};
// si on retire .output=1, on a des entrees comme ci-dessous (default val)
/*
$ ls -l /sys/bus/iio/devices/iio\:device0/
total 0
-r--r--r-- 1 root root 4096 Dec  8 14:57 dev
-rw-r--r-- 1 root root 4096 Dec  8 14:57 in_accel100_raw
-rw-r--r-- 1 root root 4096 Dec  8 14:57 in_voltage101_raw
-rw-r--r-- 1 root root 4096 Dec  8 14:57 in_voltage101_scale
*/
static const struct platform_device_id composant_id[] = {
	{"composant1", ID_COMPOSANT12},
        {}
};
MODULE_DEVICE_TABLE(platform, composant_id);       // pour platform

static const struct composant_chip_info composant_chip_info_tbl[] = {
 [ID_COMPOSANT12] = {.channels = composant_channels,},
};

static int composant_probe(struct platform_device *pdev)
{struct composant_state *st;
 struct iio_dev *indio_dev;
 int ret; 
 char *mon_nom;

 printk(KERN_ALERT ". Entering probe\n");
 indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(*st)); // alloc taille structure IIO + description pilote (st)
 if (!indio_dev) return -ENOMEM;
 st = iio_priv(indio_dev);                                  // recup du pointeur de la description pilote
 platform_set_drvdata(pdev, indio_dev);

 st->chip_info =
   &composant_chip_info_tbl[platform_get_device_id(pdev)->driver_data];

 indio_dev->dev.parent = &pdev->dev;
 mon_nom=(char*)kzalloc(15, GFP_KERNEL);
 sprintf(mon_nom,"pilote de JMF");
 indio_dev->name=mon_nom;

 indio_dev->info = &composant_info;
 indio_dev->modes = INDIO_DIRECT_MODE;
 indio_dev->channels = st->chip_info->channels;
 indio_dev->num_channels = COMPOSANT_CHANNELS;

 ret = iio_device_register(indio_dev);
 printk(KERN_ALERT ". Registered\n");
 mutex_init(&mymutex);
 return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
static void composant_remove(struct platform_device *pdev)
#else
static int composant_remove(struct platform_device *pdev)
#endif
{struct iio_dev *indio_dev = platform_get_drvdata(pdev);
 printk(KERN_ALERT ". Removing\n");
 iio_device_unregister(indio_dev);  // c'est un driver IIO
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
 return 0;
#endif
}

static struct platform_driver composant_driver = {
        .driver = {.name = "composant",.owner = THIS_MODULE,},
        .probe = composant_probe,
        .remove = composant_remove,
        .id_table = composant_id,  // pour platform
};
module_platform_driver(composant_driver);

MODULE_AUTHOR("JMF");
MODULE_DESCRIPTION("Dummy demonstration");
MODULE_LICENSE("GPL v2");
