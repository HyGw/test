KERNELDIR=$(KERNEL_DIR)

EXTRA_CFLAGS += -O0 -save-temps
EXTRA_CFLAGS += -D CONFIG_TX_WB_DONE \
                -D CONFIG_RX_2B_OFFSET \
#              	-D CONFIG_SUPPORT_SELF_TEST \
#                -D CONFIG_TX_POLLING_BY_MAC \

EXTRA_CFLAGS += -DCONFIG_NEW_QDMA_CSR_OFFSET
EXTRA_CFLAGS += -DQDMA_LAN

BUFMGR_SRC += qdma_bmgr.c qdma_api.c qdma_dev.c qdma_dvt.c
BUFMGR_OBJ := $(BUFMGR_SRC:%.c=%.o)

qdma_lan-objs 	:= $(BUFMGR_OBJ)
obj-m		+= qdma_lan.o


all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded qdma_lan.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean


