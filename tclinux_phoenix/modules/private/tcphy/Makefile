KERNELDIR := $(KERNEL_DIR)

EXTRA_CFLAGS += -DTCPHY_DEBUG
EXTRA_CFLAGS += -DLINUX_OS

eth_ephy-objs := tcetherphy_7512.o
eth_ephy-objs += tcetherphy_hook.o

obj-m = eth_ephy.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded *.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean

