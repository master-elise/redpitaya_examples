#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/fs.h>           // define fops
#include <linux/uaccess.h>
#include <linux/version.h>          

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched/signal.h>       // send_sig_info
#ifdef __ARMEL__
#include <linux/gpio.h>
//#include <linux/signal.h>           // do_send_sig_info
#else
#endif  

int hello_start(void);
void hello_end(void);

int pid = 0;
#ifdef __ARMEL__
static int dummy,gpio,id;
static int irq;

static irqreturn_t irq_handler(int irq, void *dev_id)
#else
struct timer_list exp_timer;

static void irq_handler(struct timer_list *t)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
 struct siginfo sinfo; // siginfo
#else
 struct kernel_siginfo sinfo; // siginfo
#endif
 struct task_struct *task;
// alternative `a send_sig_info :
// struct pid *mypid;
// mypid= find_vpid(pid);
// if (mypid == NULL) pr_info("Cannot find PID from user program\r\n");
//    else kill_pid(mypid, SIGUSR1, 1);
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
 memset(&sinfo, 0, sizeof(struct siginfo));  // on cherche PID au cas ou` le process aurait disparu
#else
 memset(&sinfo, 0, sizeof(struct kernel_siginfo));  // on cherche PID au cas ou` le process aurait disparu
#endif
 sinfo.si_signo = SIGUSR1;                   // depuis son enregistrement
 sinfo.si_code = SI_USER;
 task = pid_task(find_vpid(pid), PIDTYPE_PID);
 if (task == NULL) pr_info("Cannot find PID from user program\r\n");
    else send_sig_info(SIGUSR1, &sinfo, task);
#ifdef __ARMEL__
 dummy++;
 printk(KERN_INFO "plip %d",dummy);
 return IRQ_HANDLED; 
#else
 printk(KERN_INFO "plip %ld",jiffies);
 mod_timer(t, jiffies + HZ);
#endif
}

static int dev_open(struct inode *inod,struct file *fil);
static ssize_t dev_read(struct file *fil,char *buff,size_t len,loff_t *off);
static ssize_t dev_write(struct file *fil,const char *buff,size_t len,loff_t *off); 
static int dev_rls(struct inode *inod,struct file *fil);

static struct file_operations fops=
{.read=dev_read,
 .open=dev_open,
 .write=dev_write,
 .release=dev_rls,};

int hello_start()  // init_module(void) 
{int t=register_chrdev(91,"jmf",&fops);  // major = 91
#ifdef __ARMEL__
 int err;
#endif

 if (t<0) printk(KERN_ALERT "registration failed\n");
    else printk(KERN_ALERT "registration success\n");
 printk(KERN_INFO "Hello\n");
        
#ifdef __ARMEL__
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,6,0)
 gpio =512+10;
#else
 gpio =906+10;
#endif
 err=gpio_is_valid(gpio);
 err=gpio_request_one(gpio, GPIOF_IN, "jmf_irq");
 if (err!=-22)
    {printk(KERN_ALERT "gpio_request %d=%d\n",gpio,err);
     irq = gpio_to_irq(gpio);
     printk(KERN_ALERT "gpio_to_irq=%d\n",irq);
     irq_set_irq_type(irq, IRQ_TYPE_EDGE_BOTH);
     err = request_irq(irq, irq_handler, IRQF_SHARED, "GPIO jmf", &id);
     printk(KERN_ALERT "finished IRQ: error=%d\n",err);
     dummy=0;
    }
#else
 timer_setup(&exp_timer,irq_handler,0); // was init_timer_on_stack(&exp_timer); -> replaced since 4.14
 exp_timer.expires = jiffies + HZ; // HZ specifies number of clock ticks generated per second
 add_timer(&exp_timer);
#endif
 return t;
}

void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
#ifdef __ARMEL__
 free_irq(irq,&id);
 gpio_free(gpio);  // libere la GPIO pour la prochaine fois
#else
 del_timer(&exp_timer);  
#endif
 unregister_chrdev(91,"jmf");
}

static int dev_rls(struct inode *inod,struct file *fil)
{printk(KERN_ALERT "bye\n");
 return 0;
}

static int dev_open(struct inode *inod,struct file *fil)
{printk(KERN_ALERT "open\n");
 return 0;
}

static ssize_t dev_read(struct file *fil,char *buff,size_t len,loff_t *off)
{char buf[15]="Hello read\n\0";
 int readPos=0;
 printk(KERN_ALERT "read\n");
 while (len && (buf[readPos]!=0)) 
   {put_user(buf[readPos],buff++);
    readPos++;
    len--;
   }
 *off += readPos;
 return readPos;
}

static ssize_t dev_write(struct file *fil,const char *buff,size_t len,loff_t *off)
{int mylen;
 char buf[15];

 printk(KERN_ALERT "write ");
 if (len>14) mylen=14; else mylen=len;
 if (copy_from_user(buf, buff, mylen) == 0)
    sscanf(buf, "%d", &pid);

 printk(KERN_ALERT "PID registered: %d",pid);
 return len;
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");  // NECESSAIRE pour exporter les symboles du noyau linux !
