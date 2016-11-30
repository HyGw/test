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

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00001814d00003091sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001400d00007650sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001400d00007662sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000014C3d00007662sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000014C3d00007632sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000014C3d00007612sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000014C3d00007602sv*sd*bc*sc*i*");
