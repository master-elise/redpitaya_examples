#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/ioport.h>       // request_mem
#include <linux/io.h>           // ioremap and writel
#include <linux/version.h>           

struct timer_list exp_timer;

#define IO_BASE1 0xf8000000
#define IO_BASE2 0xe000a000

static void __iomem *jmf_gpio;   //int jmf_gpio;
volatile int jmf_stat=0;

#define mio 0

int hello_start(void); 
void hello_end(void);

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0) || LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0)
static void do_something(struct timer_list *data)
#else
static void do_something(unsigned long data)
#endif
{printk(KERN_INFO "plop: %lu",jiffies);
 jmf_stat=(1<<mio)-jmf_stat;
 writel(jmf_stat,jmf_gpio+0x40); 
 mod_timer(&exp_timer, jiffies + HZ);
}

int hello_start()  // init_module(void) 
{int delay = 1; 
 unsigned int stat;
 printk(KERN_INFO "Hello\n");
 
 if (request_mem_region(IO_BASE1,0x12c,"GPIO cfg")==NULL) 
     printk(KERN_ALERT "mem request failed");
 jmf_gpio = (void __iomem*)ioremap(IO_BASE1, 0x4); 
 stat=readl(jmf_gpio+0x12c);
 printk("stat=%x\n",stat); 
 stat|=(1<<22);
 writel(stat,jmf_gpio+0x12c);    // clock
 release_mem_region(IO_BASE1, 0x12c);

 if (request_mem_region(IO_BASE2,0x2E4,"GPIO test")==NULL) 
     printk(KERN_ALERT "mem request failed");
 jmf_gpio = (void __iomem*)ioremap(IO_BASE2, 0x2E4); 
 writel(1<<mio,jmf_gpio+0x204);   
 writel(1<<mio,jmf_gpio+0x208);   
 writel(1<<mio,jmf_gpio+0x40);   
        
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0) || LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0)
 timer_setup(&exp_timer,do_something,0);
#else
 init_timer_on_stack(&exp_timer);
 exp_timer.data = 0;
 exp_timer.function = do_something;
#endif
 exp_timer.expires = jiffies + delay * HZ; // HZ specifies number of clock ticks generated per second
 add_timer(&exp_timer);

 return 0;
}

void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
 release_mem_region(IO_BASE2, 0x2e4);
 del_timer(&exp_timer);  
}

module_init(hello_start);
module_exit(hello_end);
MODULE_LICENSE("GPL");
