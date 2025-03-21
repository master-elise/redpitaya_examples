#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/ioport.h>       // request_mem
#include <linux/io.h>           // ioremap

struct timer_list exp_timer;

volatile int jmf_stat=0;

int hello_start(void); 
void hello_end(void);

static void do_something(struct timer_list *t)
{printk(KERN_INFO "plop: %lu",jiffies);
 mod_timer(t, jiffies + HZ);
}

int hello_start()  // init_module(void) 
{int delay = 1; 

 printk(KERN_INFO "Hello\n");
 
 // https://lwn.net/Articles/735887/
 timer_setup(&exp_timer,do_something,0);   // was init_timer_on_stack(&exp_timer); -> replaced since 4.14
 exp_timer.expires = jiffies + delay * HZ; // HZ specifies number of clock ticks generated per second
// exp_timer.data = 0;                     // obsolete
// exp_timer.function = do_something;      // dupplicate with the new timer_setup
 add_timer(&exp_timer);

 return 0;
}

void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
 del_timer(&exp_timer);  
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");
