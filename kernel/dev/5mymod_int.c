#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/version.h>

static int dummy, irq, jmf_gpio, dev_id;

void hello_end(void); // cleanup_module(void)
int hello_start(void); // cleanup_module(void)

static irqreturn_t irq_handler(int irq, void *dev_id)
{
        dummy++;
        printk(KERN_INFO "plip %d",dummy);
	return IRQ_HANDLED; // etait IRQ_NONE
}

int hello_start()  // init_module(void) 
{int err;

 printk(KERN_INFO "Hello\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,6,0)
 jmf_gpio =512+10;
#else
 jmf_gpio =906+10;
#endif
 err=gpio_is_valid(jmf_gpio);
 err=gpio_request_one(jmf_gpio, GPIOF_IN, "jmf_irq");
 if (err!=-22) 
    {printk(KERN_ALERT "gpio_request %d=%d\n",jmf_gpio,err);
     irq = gpio_to_irq(jmf_gpio);
     printk(KERN_ALERT "gpio_to_irq=%d\n",irq);
     irq_set_irq_type(irq, IRQ_TYPE_EDGE_BOTH);
     err = request_irq(irq, irq_handler, IRQF_SHARED, "GPIO jmf", &dev_id);
     printk(KERN_ALERT "finished IRQ: error=%d\n",err);
     dummy=0;
    }
 return 0;
}

void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
 free_irq(irq,&dev_id);
 gpio_free(jmf_gpio);  // libere la GPIO pour la prochaine fois
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");  // NECESSAIRE pour exporter les symboles du noyau linux !
