// bien verifier que /dev/jmf N'EXISTE PAS pour ne pas entrer en conflit avec misc dev

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/fs.h>           // define fops
#include <linux/uaccess.h>        // get_user, put_user
#include <linux/miscdevice.h>   // allocation dynamique de /dev

static int dev_open(struct inode *inod,struct file *fil);
static ssize_t dev_read(struct file *fil,char *buff,size_t len,loff_t *off);
static ssize_t dev_write(struct file *fil,const char *buff,size_t len,loff_t *off); 
// static int dev_ioctl(struct inode *inod,struct file *fil, unsigned int cmd, unsigned long arg); // pre-2.6.35
static long dev_ioctl(struct file *f, unsigned int cmd, unsigned long arg); // actuel
static int dev_rls(struct inode *inod,struct file *fil);

struct miscdevice jmfdev;

int hello_start(void);
void hello_end(void);

static struct file_operations fops=
{.read=dev_read,
 .open=dev_open,
 .unlocked_ioctl=dev_ioctl,
 .write=dev_write,
 .release=dev_rls,};

int hello_start()  // init_module(void) 
{ int t=register_chrdev(91,"jmf",&fops);  // major = 90
  if (t<0) printk(KERN_ALERT "registration failed\n");
     else printk(KERN_ALERT "registration success\n");
 //jmfdev.name = "jmf";  // /dev/jmf  de major pris dans la classe misc
 //jmfdev.minor = MISC_DYNAMIC_MINOR;
 //jmfdev.fops = &fops;
 //jmfdev.mode = 0666; // permissions pour user
 //misc_register(&jmfdev);  // creation dynamique de l'entree /dev/jmf

 printk(KERN_INFO "Hello ioctl\n");
 return 0;
}

void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
 unregister_chrdev(91,"jmf");
 // misc_deregister(&jmfdev);
}

static int dev_rls(struct inode *inod,struct file *fil)
{printk(KERN_ALERT "bye\n");
 return 0;
}

static int dev_open(struct inode *inod,struct file *fil)
{printk(KERN_ALERT "ioctl open\n");
 return 0;
}

static ssize_t dev_read(struct file *fil,char *buff,size_t len,loff_t *off)
{char buf[15]="Hello read\n\0";
 int dummy,readPos=strlen(buf);
 printk(KERN_ALERT "read %d %d\n",imajor(fil->f_inode),iminor(fil->f_inode)); // file_inode(fil)
 // readPos=0;while (len && (buf[readPos]!=0)) {put_user(buf[readPos],buff++);readPos++;len--;}
 dummy=copy_to_user(buff,buf,readPos);
 return readPos;
}

static ssize_t dev_write(struct file *fil,const char *buff,size_t len,loff_t *off)
{int dummy,mylen;
 char buf[15]="Hello\0";
 printk(KERN_ALERT "write %d",(int)len);
 if (len>14) mylen=14; else mylen=len;
 dummy=copy_from_user(buf,buff,mylen); // for (l=0;l<mylen;l++) get_user(buf[l],buff+l);
 buf[mylen]=0;
 printk(KERN_ALERT "%s",buf);
 return mylen;
}

// static int dev_ioctl(struct inode *inod,struct file *fil, unsigned int cmd, unsigned long arg) // pre-2.6.35
static long dev_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{static char var[10];
 int dummy;
 printk(KERN_ALERT "ioctl CMD%d",cmd);
 switch ( cmd ) 
 {case 0: dummy=copy_from_user(var,(char*)arg,5);
          printk(KERN_ALERT "ioctl0: %s\n",(char*)var); // NE PAS acceder a arg qui est en userspace
          break;
  case 1: printk(KERN_ALERT "ioctl1: %s\n",(char*)var); // ... idem
          dummy=copy_to_user((char*)arg,var,5);
          break;
  default:printk(KERN_ALERT "unknown ioctl");break;
 }
 return 0;
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");
