#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0xb18ef92f, "module_layout" },
	{ 0xef67aff, "register_netdevice" },
	{ 0x9a1dfd65, "strpbrk" },
	{ 0x2475b8e7, "pci_bus_read_config_byte" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xffb407a9, "mem_map" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x349cba85, "strchr" },
	{ 0x788fe103, "iomem_resource" },
	{ 0xd0d8621b, "strlen" },
	{ 0xc7ec6c27, "strspn" },
	{ 0x517856ac, "dev_set_drvdata" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x3d268109, "pci_get_slot" },
	{ 0x85e1d031, "pci_disable_device" },
	{ 0x47939e0d, "__tasklet_hi_schedule" },
	{ 0xffb05d48, "netif_carrier_on" },
	{ 0x87a45ee9, "_raw_spin_lock_bh" },
	{ 0x14f2d41d, "dev_get_by_name" },
	{ 0xe5ce3f9d, "skb_clone" },
	{ 0x558874ca, "skb_copy" },
	{ 0x4e85bb49, "netif_carrier_off" },
	{ 0x55819f2e, "filp_close" },
	{ 0x27a70c41, "x86_dma_fallback_dev" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x70b94dbd, "pci_release_regions" },
	{ 0x9e1bdc28, "init_timer_key" },
	{ 0x85df9b6c, "strsep" },
	{ 0x999e8297, "vfree" },
	{ 0x2463d8a4, "pci_bus_write_config_word" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x7d11c268, "jiffies" },
	{ 0x379128b5, "skb_trim" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x2b2d0445, "netif_rx" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0xacc1ebd1, "param_ops_charp" },
	{ 0x460ac851, "pci_set_master" },
	{ 0xe1bc7ede, "del_timer_sync" },
	{  0xa1468, "dev_alloc_skb" },
	{ 0xdc78b556, "pci_restore_state" },
	{ 0x11089ac7, "_ctype" },
	{ 0x88941a06, "_raw_spin_unlock_irqrestore" },
	{ 0xe1e73747, "current_task" },
	{ 0x50eedeb8, "printk" },
	{ 0x42224298, "sscanf" },
	{ 0xd9c83c53, "kthread_stop" },
	{ 0x94d32a88, "__tracepoint_module_get" },
	{ 0x5152e605, "memcmp" },
	{ 0x9c6ba4be, "free_netdev" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0xb6ed1e53, "strncpy" },
	{ 0x12424e59, "register_netdev" },
	{ 0x704ceac8, "wireless_send_event" },
	{ 0x6c2e3320, "strncmp" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x3f684662, "pci_bus_write_config_dword" },
	{ 0x6e064ffa, "skb_push" },
	{ 0x43f71393, "__pci_enable_wake" },
	{ 0xb9150762, "dev_close" },
	{ 0x1e6d26a8, "strstr" },
	{ 0x9545af6d, "tasklet_init" },
	{ 0xce095088, "mod_timer" },
	{ 0x71205378, "add_timer" },
	{ 0x76fded60, "kill_pid" },
	{ 0xf96e338f, "dma_release_from_coherent" },
	{ 0x859c6dc7, "request_threaded_irq" },
	{ 0xfe33bc23, "skb_pull" },
	{ 0x5557c173, "dev_kfree_skb_any" },
	{ 0x3bfadb08, "dma_alloc_from_coherent" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0x61651be, "strcat" },
	{ 0x82072614, "tasklet_kill" },
	{ 0xa618c45e, "pci_find_capability" },
	{ 0xb1ff4c44, "module_put" },
	{ 0x295a4199, "skb_copy_expand" },
	{ 0x2efb78dc, "netif_device_attach" },
	{ 0x8ff4079b, "pv_irq_ops" },
	{ 0x37ff1ac0, "netif_device_detach" },
	{ 0xe3cd4dda, "__alloc_skb" },
	{ 0x3af98f9e, "ioremap_nocache" },
	{ 0x7fbe2cac, "pci_bus_read_config_word" },
	{ 0xb0ff1eda, "pci_bus_read_config_dword" },
	{ 0x6223cafb, "_raw_spin_unlock_bh" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x4292364c, "schedule" },
	{ 0xcd483de, "eth_type_trans" },
	{ 0x231b3504, "wake_up_process" },
	{ 0xb6264594, "pskb_expand_head" },
	{ 0x9bce482f, "__release_region" },
	{ 0xa30fefdc, "pci_unregister_driver" },
	{ 0x587c70d8, "_raw_spin_lock_irqsave" },
	{ 0xf09c7f68, "__wake_up" },
	{ 0x842cc9f1, "pci_set_power_state" },
	{ 0xd2965f6f, "kthread_should_stop" },
	{ 0x37a0cba, "kfree" },
	{ 0x2b64ab60, "kthread_create" },
	{ 0xf18f0376, "pci_request_regions" },
	{ 0xe75663a, "prepare_to_wait" },
	{ 0xedc03953, "iounmap" },
	{ 0xa96d5074, "__pci_register_driver" },
	{ 0xb00ccc33, "finish_wait" },
	{ 0xbb80f547, "unregister_netdev" },
	{ 0xb742fd7, "simple_strtol" },
	{ 0x701d0ebd, "snprintf" },
	{ 0xfe925c23, "pci_choose_state" },
	{ 0xdd6705cf, "__netif_schedule" },
	{ 0x8235805b, "memmove" },
	{ 0x8b00c0aa, "skb_put" },
	{ 0x140e5ef6, "pci_enable_device" },
	{ 0x33d169c9, "_copy_from_user" },
	{ 0xf38025a5, "dev_get_drvdata" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0x1af2fba5, "dma_ops" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x4deb6964, "pci_save_state" },
	{ 0xe914e41e, "strcpy" },
	{ 0xd7e41a99, "filp_open" },
	{ 0x3a326f7c, "alloc_etherdev_mqs" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00001814d00003062sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001814d00003562sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001814d00003060sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001814d00003592sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001432d00007711sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001432d00007722sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001814d00003593sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "1CDAB1B47FAB446047D190A");
