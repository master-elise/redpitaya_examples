#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/module.h>

void plat_gr_release(struct device *dev);

void plat_gr_release(struct device *dev) 
{printk(KERN_INFO "board released\r\n");}

static struct platform_device plat_gr_device = {
 .name = "composant1", // MEME NOM QUE DANS platform_driver ad5624r_driver = {
 .id   = 0,
 .dev  =  {.release = plat_gr_release,},
};

static int gr_board_init(void) {
 int status;
 status = platform_device_register(&plat_gr_device);
 if (status < 0) return status;
 else printk(KERN_INFO "board mounted");
 return status;
}

static void gr_board_exit(void) {
 platform_device_unregister(&plat_gr_device);
 printk(KERN_INFO "board unmounted");
}

module_init(gr_board_init);
module_exit(gr_board_exit);

MODULE_AUTHOR("gwenhael Goavec-Merou");
MODULE_DESCRIPTION("demo BOARD DRIVER");
MODULE_ALIAS("demo");
MODULE_LICENSE("GPL");
