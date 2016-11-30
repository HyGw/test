KERNELDIR := $(KERNEL_DIR)



EXTRA_CFLAGS += -DLINUX_OS 


fe_core-objs := fe.o fe_api.o 


obj-m = fe_core.o

FE_TEST=0
ifeq ($(FE_TEST),1)
fetest-objs := fe_test.o fe_test_api.o
obj-m += fetest.o
endif

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded *.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean

