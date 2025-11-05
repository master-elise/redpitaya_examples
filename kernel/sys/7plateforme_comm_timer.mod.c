#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x92997ed8, "_printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x9618ede0, "mutex_unlock" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x828ce6bb, "mutex_lock" },
	{ 0xe914e41e, "strcpy" },
	{ 0x7c0ab1f7, "__platform_driver_register" },
	{ 0x5f754e5a, "memset" },
	{ 0x3e4a4f5c, "platform_device_register_full" },
	{ 0xd8fa6f71, "device_create_file" },
	{ 0xde4bf88b, "__mutex_init" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x24d273d1, "add_timer" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x36c6a892, "device_remove_file" },
	{ 0xe7ecd137, "platform_device_unregister" },
	{ 0x3c264d16, "platform_driver_unregister" },
	{ 0xdc0e4855, "timer_delete" },
	{ 0x726c9cbd, "module_layout" },
};

MODULE_INFO(depends, "");

