// mknod /dev/jmf c 90 0

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/fs.h>           // define fops
#include <asm/uaccess.h>        // get_user, put_user

static int dev_open(struct inode *inod,struct file *fil);
static ssize_t dev_read(struct file *fil,char *buff,size_t len,loff_t *off);
static ssize_t dev_write(struct file *fil,const char *buff,size_t len,loff_t *off);
static int dev_rls(struct inode *inod,struct file *fil);

char buf[15]="Hello read\n\0";

static struct file_operations fops=
{.read=dev_read,
 .open=dev_open,
 .write=dev_write,
 .release=dev_rls,};

int hello_start(void);
void hello_end(void);

int hello_start()  // init_module(void) 
{int t=register_chrdev(91,"jmf",&fops);  // major = 91 car 90=mtd
 if (t<0) printk(KERN_ALERT "registration failed\n");
    else printk(KERN_ALERT "registration success\n");
 printk(KERN_INFO "Hello\n");
 return t;
}

// static void __exit hello_end(void) 
void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
 unregister_chrdev(91,"jmf");
}

static int dev_rls(struct inode *inod,struct file *fil)
{printk(KERN_ALERT "bye\n");
 return 0;
}

static int dev_open(struct inode *inod,struct file *fil)
{printk(KERN_ALERT "open %d %d\n",imajor(inod),iminor(inod));
 return 0;
}
/*
 sudo mknod jmf0 c 91 0
 sudo mknod jmf1 c 91 1
 sudo insmod 1mymod.ko 
 dd if=jmf0 count=1
 dd if=jmf1 count=1
 dmesg
[4406727.414960] open 91 0
[4406727.415068] read
[4406727.415081] bye
[4406742.272454] open 91 1
[4406742.272555] read
[4406742.272567] bye
*/

static ssize_t dev_read(struct file *fil,char *buff,size_t len,loff_t *off)
{int readPos=0;
 printk(KERN_ALERT "read\n");
 while (len && (buf[readPos]!=0)) 
   {put_user(buf[readPos],buff++);
    readPos++;
    len--;
   }
 return readPos;
}

static ssize_t dev_write(struct file *fil,const char *buff,size_t len,loff_t *off)
{int l=0,mylen;
 printk(KERN_ALERT "write ");
 if (len>14) mylen=14; else mylen=len;
 for (l=0;l<mylen;l++) get_user(buf[l],buff+l);
 // Essayer avec vvv : kernel panic car acces en userspace
 // for (l=0;l<mylen;l++) buf[l]=buff[l];
 buf[mylen]=0;
 printk(KERN_ALERT "%s \n",buf);
 return len;
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");
