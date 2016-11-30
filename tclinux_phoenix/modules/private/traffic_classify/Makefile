KERNELDIR=$(KERNEL_DIR)

obj-m += traffic_classify.o

traffic_classify-objs := traffic_classify_itf.o parse_traffic.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded *.ko
	
clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean

