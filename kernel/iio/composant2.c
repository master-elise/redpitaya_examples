#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>

#include <linux/init.h>
#include <linux/platform_device.h>

#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/regulator/consumer.h>
#include <linux/module.h>
#include <linux/version.h>

#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/mod_devicetable.h>

#include "composant2.h"

static int composant_read_raw(struct iio_dev *indio_dev,
  struct iio_chan_spec const *chan, int *val, int *val2, long m)  // m = mask
{struct composant_state *st = iio_priv(indio_dev);
 switch (m) {
 case IIO_CHAN_INFO_SCALE:  // si on avait active' BIT(IIO_CHAN_INFO_SCALE)
   *val = st->vref_mv;
   *val2 = chan->scan_type.realbits;
 return IIO_VAL_INT; // ou IIO_VAL_FRACTIONAL_LOG2;
 case IIO_CHAN_INFO_RAW:
   *val=0; 
   return IIO_VAL_INT;
 }
 return -EINVAL;
}

static int composant_write_raw(struct iio_dev *indio_dev,
  struct iio_chan_spec const *chan, int val, int val2, long mask)
{
	switch (mask) {
	case IIO_CHAN_INFO_RAW: break;
	}
	return 0;
}

static const struct iio_info composant_info = {
	.write_raw = composant_write_raw,
	.read_raw = composant_read_raw,
//	.driver_module = THIS_MODULE,
};

#define COMPOSANT_CHANNEL(_chan, _bits) { \
	.type = IIO_VOLTAGE, \
	.indexed = 1, \
	.output = 1, \
	.channel = (_chan), \
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW), \
	.address = (_chan), \
	.scan_type = { \
		.sign = 'u', \
		.realbits = (_bits), \
		.storagebits = 16, \
		.shift = 16 - (_bits), \
	}, \
}

#define DECLARE_COMPOSANT_CHANNELS(_name, _bits) \
	const struct iio_chan_spec _name##_channels[] = { \
		COMPOSANT_CHANNEL(0, _bits), \
		COMPOSANT_CHANNEL(1, _bits), \
}

static DECLARE_COMPOSANT_CHANNELS(composant, 12);
static DECLARE_COMPOSANT_CHANNELS(ad5644r, 14);

static const struct composant_chip_info composant_chip_info_tbl[] = {
 [ID_COMPOSANT12] = {.channels = composant_channels,.int_vref_mv = 1250,},
 [ID_COMPOSANT14] = {.channels = ad5644r_channels,.int_vref_mv = 1250,},
};

static int composant_probe(struct platform_device *pdev)
{struct composant_state *st;
 struct iio_dev *indio_dev;
 int ret; 
 char *filename;

 printk(KERN_ALERT ". Entering probe\n");
 indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(*st));
 if (!indio_dev) return -ENOMEM;
 st = iio_priv(indio_dev);
 platform_set_drvdata(pdev, indio_dev);
 st->chip_info =
   &composant_chip_info_tbl[platform_get_device_id(pdev)->driver_data];

 indio_dev->dev.parent = &pdev->dev;
 filename=(char*)kzalloc(15, GFP_KERNEL);
 sprintf(filename,"pilote de JMF");
 indio_dev->name=filename;

 indio_dev->info = &composant_info;
 indio_dev->modes = INDIO_DIRECT_MODE;
 indio_dev->channels = st->chip_info->channels;
 indio_dev->num_channels = COMPOSANT_CHANNELS;

 printk(KERN_ALERT ". Registering\n");
 ret = iio_device_register(indio_dev);
 printk(KERN_ALERT ". Registered\n");
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

static const struct platform_device_id composant_id[] = {
	{"composant1", ID_COMPOSANT12},
	{"composant2", ID_COMPOSANT14}, {}
};
MODULE_DEVICE_TABLE(platform, composant_id);

static struct platform_driver composant_driver = {
        .driver = {.name = "composant",.owner = THIS_MODULE,},
        .probe = composant_probe,
        .remove = composant_remove,
        .id_table = composant_id,
};
module_platform_driver(composant_driver);

MODULE_AUTHOR("JMF");
MODULE_DESCRIPTION("Dummy demonstration");
MODULE_LICENSE("GPL v2");
