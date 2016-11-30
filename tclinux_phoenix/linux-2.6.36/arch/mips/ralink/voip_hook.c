#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>

unsigned int* (*recv_rtp_src_port_get_hook)(void) = NULL;
EXPORT_SYMBOL(recv_rtp_src_port_get_hook);
