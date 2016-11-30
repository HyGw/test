KERNELDIR=$(KERNEL_DIR)

EXTRA_CFLAGS += -O0 -save-temps

FHVLAN_SRC += fh_vlan_m.c tc_vlanfilter.c tc_vlan_trans.c tc_l2lu.c
FHVLAN_OBJ := $(FHVLAN_SRC:%.c=%.o)

fh_vlan-objs 	:= $(FHVLAN_OBJ)
obj-m		+= fh_vlan.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded fh_vlan.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean


