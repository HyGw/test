KERNELDIR=$(KERNEL_DIR)
EXTRA_CFLAGS += -DWAN2LAN

INCDIR		:= inc
SRCDIR		:= src
HEADER_DIR              := inc

ifneq ($(TCSUPPORT_CPU_EN7521),)
	EXTRA_CFLAGS += -I$(MODULES_QDMA_7512_DIR)
else
	EXTRA_CFLAGS += -I$(MODULES_QDMA_DIR)
endif

EXTRA_CFLAGS += -I$(MODULES_PHY_DIR)/$(HEADER_DIR) -O0 -save-temps
EXTRA_CFLAGS += -D CONFIG_SEND_ACK_PLOAMU=3 \
				-D CONFIG_USE_FOR_TEST \
				-D CONFIG_DEBUG 

ifeq ($(CONFIG_USE_MT7520_ASIC),)
#EXTRA_CFLAGS += -DCONFIG_USE_A60901
EXTRA_CFLAGS += -DCONFIG_USE_A60928
endif
				
EXTRA_CFLAGS += -DEPON_MAC_HW_TEST
EXTRA_CFLAGS += -DTCSUPPORT_EPON_RGST_SILENT

ifneq ($(TCSUPPORT_EPON_FPGA_WORKAROUND),)
EXTRA_CFLAGS += -DTCSUPPORT_EPON_FPGA_WORKAROUND
endif


PHY_SRC	+= i2c.c mt7570.c phy.c phy_init.c phy_rx.c phy_tx.c
PHY_OBJ	:= $(PHY_SRC:%.c=$(SRCDIR)/%.o)

TOTAL_OBJ	:= $(SRCDIR)/phy.o  $(PHY_OBJ) 

phy-objs 	:= $(TOTAL_OBJ)
obj-m		:= phy.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded phy.ko
	
clean:
	cd $(FILESYSTEM_DIR)/lib/modules/ && rm -f phy.ko
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean


