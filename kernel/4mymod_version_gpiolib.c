#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/io.h>           /* Needed for writel     */
#include <linux/gpio.h>
#include <linux/version.h>

struct timer_list exp_timer;

// int jmf_gpio=906+0; // 7 en conflit avec heartbeat => virer leds_gpio et led_class (sinon +0)
int jmf_gpio=906+10; // ou ('port'-'A')*32+pin dans devicetree
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
 writel(jmf_stat,(void*)(jmf_gpio+0x40)); 
 mod_timer(&exp_timer, jiffies + HZ);
}

int hello_start(void);
void hello_end(void);

int hello_start()  // init_module(void) 
{int delay = 1,err; 

 printk(KERN_INFO "Hello\n");
 err=gpio_is_valid(jmf_gpio);
 printk(KERN_INFO "err: %d\n",err);
 err=gpio_request_one(jmf_gpio, GPIOF_OUT_INIT_LOW, "jmf_gpio"); // voir dans gpio.h
 printk(KERN_INFO "err: %d\n",err);
        
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
 gpio_free(jmf_gpio);
 del_timer(&exp_timer);  
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");  // NECESSAIRE pour exporter les symboles du noyau linux !
