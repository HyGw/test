KERNELDIR=$(KERNEL_DIR)
obj-m += personality_parm.o

personality_parm-objs := personality_parm_op.o personality_parm_proc.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded *.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean