KERNELDIR=$(KERNEL_DIR)

obj-m += ecnthook.o
ecnthook-objs := ecnt_hook_register.o 
#ecnthook-objs += sample/net_core/ecnt_hook_dev.o sample/net_8021q/ecnt_hook_vlan_dev.o sample/net_core/ecnt_hook_dev_net_recv.o

ifneq ($(TC_CFLAGS),)
EXTRA_CFLAGS+=$(TC_CFLAGS)
endif

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded *.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean

