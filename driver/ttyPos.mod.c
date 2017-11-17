#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x99ed5c78, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x1c090f77, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x1ac69ad9, __VMLINUX_SYMBOL_STR(put_tty_driver) },
	{ 0xb729c506, __VMLINUX_SYMBOL_STR(tty_unregister_driver) },
	{ 0x37a6c9b0, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0x7852418a, __VMLINUX_SYMBOL_STR(tty_port_destroy) },
	{ 0x72bbc34b, __VMLINUX_SYMBOL_STR(tty_register_driver) },
	{ 0x77c5513b, __VMLINUX_SYMBOL_STR(tty_port_init) },
	{ 0x1801c2cd, __VMLINUX_SYMBOL_STR(tty_set_operations) },
	{ 0x67b27ec1, __VMLINUX_SYMBOL_STR(tty_std_termios) },
	{ 0x3759a627, __VMLINUX_SYMBOL_STR(__tty_alloc_driver) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0x1516ca02, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xb8672fb5, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x6b42edd7, __VMLINUX_SYMBOL_STR(tty_port_tty_set) },
	{ 0xe979d3ef, __VMLINUX_SYMBOL_STR(tty_unregister_device) },
	{ 0x64ab0e98, __VMLINUX_SYMBOL_STR(wait_for_completion) },
	{ 0xcfc940ac, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x45e0878b, __VMLINUX_SYMBOL_STR(tty_port_register_device) },
	{ 0x4201d0dc, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0x264a2ce8, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x9e88526, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x3f79aa78, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x33d53fbc, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0xa8c2e12e, __VMLINUX_SYMBOL_STR(tty_flip_buffer_push) },
	{ 0x7e5dc865, __VMLINUX_SYMBOL_STR(tty_insert_flip_string_fixed_flag) },
	{ 0x952664c5, __VMLINUX_SYMBOL_STR(do_exit) },
	{ 0xb2d5a552, __VMLINUX_SYMBOL_STR(complete) },
	{ 0xf3c00bab, __VMLINUX_SYMBOL_STR(tty_buffer_request_room) },
	{ 0xae911416, __VMLINUX_SYMBOL_STR(usb_reset_device) },
	{ 0xa4f3a815, __VMLINUX_SYMBOL_STR(usb_clear_halt) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xd62c833f, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x5da4b97d, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xf08242c2, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x2207a57f, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x499cc7e7, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0xa6bbd805, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xfd7f87b2, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x786108ad, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v1234p0101d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "5AB993BD6815A26E338AACA");
