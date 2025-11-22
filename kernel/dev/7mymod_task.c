#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/fs.h>           // define fops
#include <linux/uaccess.h>      // get_user, put_user
#include <linux/version.h>      // for linux kernel version

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>

#include <linux/sched.h>
#include <linux/signal.h>

int hello_start(void);
void hello_end(void);

static int dummy,irq_id,gpio,irq;

static int dev_open(struct inode *inod,struct file *fil);
static ssize_t dev_write(struct file *fil,const char *buff,size_t len,loff_t *off); 
static int dev_rls(struct inode *inod,struct file *fil);
static ssize_t dev_read(struct file *fil,char *buff,size_t len,loff_t *off);

static struct work_struct irq_work;

static struct file_operations fops=
{.read=dev_read,
 .open=dev_open,
 .write=dev_write,
 .release=dev_rls,};

int pid = 0;

static irqreturn_t irq_handler(int irq, void *dev_id)
{
 schedule_work(&irq_work);  // on le fera quand on aura le temps
 dummy++;
 printk(KERN_INFO "plip %d",dummy);
 return IRQ_HANDLED; 
}

void do_something(struct work_struct *data)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0)
 struct kernel_siginfo sinfo;
#else
 struct siginfo sinfo;
#endif
 struct task_struct *task;

 memset(&sinfo, 0, sizeof(struct siginfo));  // on cherche PID au cas ou` le process aurait disparu
 sinfo.si_signo = SIGUSR1;                   // depuis son enregistrement
 sinfo.si_code = SI_USER;
 task = pid_task(find_vpid(pid), PIDTYPE_PID);
 if (task == NULL) {
     pr_info("Cannot find PID from user program\r\n");
    }
 // else do_send_sig_info(SIGUSR1, &sinfo, task, true); // send_sig_info(SIGUSR1, &sinfo, task);
 else send_sig_info(SIGUSR1, &sinfo, task);


 printk(KERN_INFO "plop");
}

int hello_start()  // init_module(void) 
{int t=register_chrdev(91,"jmf",&fops);  // major = 90
 int err;

 if (t<0) printk(KERN_ALERT "registration failed\n");
    else printk(KERN_ALERT "registration success\n");
 printk(KERN_INFO "Hello\n");
        
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,6,0)
 gpio =512+10;
#else
 gpio =906+10;
#endif
 err=gpio_is_valid(gpio);
 err=gpio_request_one(gpio, GPIOF_IN, "jmf_irq");
 if ((err!=-22)  && (err!=-517))
    {printk(KERN_ALERT "gpio_request %d=%d\n",gpio,err);
     INIT_WORK(&irq_work, do_something);
     irq = gpio_to_irq(gpio);
     printk(KERN_ALERT "gpio_to_irq=%d\n",irq);
     irq_set_irq_type(irq, IRQ_TYPE_EDGE_BOTH);
     err = request_irq(irq, irq_handler, IRQF_SHARED, "GPIO jmf", &irq_id);
     printk(KERN_ALERT "finished IRQ: error=%d\n",err);
     dummy=0;
    }
 else gpio=0;
 if (err==-517) printk(KERN_ALERT "insert gpiolib library");
 return t;
}

void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
 if (gpio!=0) // failed
   {free_irq(irq,&irq_id);
    gpio_free(gpio);  // libere la GPIO pour la prochaine fois
   }
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
