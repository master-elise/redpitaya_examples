#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/gpio.h>
/*
#include <linux/pinctrl/pinctrl.h> // test PWM


const struct pinctrl_pin_desc foo_pins[] = {
      PINCTRL_PIN(0, "B2"),
};  // test PWM

static struct pinctrl_desc foo_desc = {
	.name = "foo",
	.pins = foo_pins,
	.npins = ARRAY_SIZE(foo_pins),
	.owner = THIS_MODULE,
};
*/

#define mio 0

int hello_start(void);
void hello_end(void);
int jmf_gpio=906+mio;

int hello_start()  // init_module(void) 
{int err;
 // struct pinctrl_dev *pctl;
 // pctl = pinctrl_register(&foo_desc, <PARENT>, NULL);
 // if (!pctl)
 //    pr_err("could not register foo pin driver\n");
 // return pinctrl_register_and_init(&foo_desc, <PARENT>, NULL, &pctl); NOUVEAU KERNEL.ORG DOC -> A CHANGER POUR NEW KERNEL
 printk(KERN_INFO "Hello\n");
 err=gpio_is_valid(jmf_gpio);
 err=gpio_request_one(jmf_gpio,GPIOF_OUT_INIT_HIGH,"jmf-gpio");
 gpio_set_value(jmf_gpio,1);
 return 0;
}

void hello_end() // cleanup_module(void)
{printk(KERN_INFO "Goodbye\n");
 gpio_set_value(jmf_gpio,0);
 gpio_free(jmf_gpio);
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");  // necessaire pour acceder a gpiolib
