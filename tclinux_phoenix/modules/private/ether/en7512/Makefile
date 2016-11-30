KERNELDIR := $(KERNEL_DIR)

EXTRA_CFLAGS +=-DTCPHY_SUPPORT -DRT63365_SUPPORT -DMII_INTERFACE -DWAN2LAN

EXTRA_CFLAGS += -DLINUX_OS 

ifneq ($(TCSUPPORT_AUTOBENCH),)
EXTRA_CFLAGS += -DTCPHY_DEBUG
endif
#EXTRA_CFLAGS +=-DLOOPBACK_SUPPORT
#EXTRA_CFLAGS+=-DLOOPBACK_SUPPORT_JUMBO

eth-objs := eth_lan.o eth_proc.o eth_api.o eth_led.o

ifneq ($(TCSUPPORT_AUTOBENCH),)
eth-objs += ../../auto_bench/femac_autobench.o
endif

eth-objs +=./tcphy/tcswitch.o
eth-objs +=./tcphy/tcconsole.o
eth-objs +=./tcphy/phy_api.o
eth-objs +=./tcphy/switch_proc.o
#eth-objs += fe_verify.o

ifneq ($(strip $(TCSUPPORT_MT7530_SWITCH_API)),)
eth-objs += ./tcphy/mtkswitch_api_krl.o
endif
ifneq ($(TCSUPPORT_HEC_6906),)
eth-objs += access_module_cl.o
endif
ifneq ($(TCSUPPORT_WAN_ETHER),)
eth-objs += eth_wan.o
endif
ifneq ($(TCSUPPORT_SEPERATED_ETH_ITF),)
eth-objs += eth_virtual_if.o
endif
ifneq ($(TCSUPPORT_CT_JOYME_BANDWIDTH),)
eth-objs += devBandWidth.o
endif


obj-m = eth.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded *.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean


#eth-objs +=./tcphy/tcetherphy.o
