KERNELDIR=$(KERNEL_DIR)
obj-m += mt7510sim.o
#CFLAGS+= -DSIM_TOUT_REG_V2
#CFLAGS+= -DSIM_TOUT_REG_V3

mt7510sim-objs := mt7510_sim.o sim_operation.o

#mt7510sim-objs := mt6306_sim.o 

#mt7510sim-objs += icc_sim_common_mtk.o 


#mt7510sim-objs += icc_switchControl1_mt6306.o 
#mt7510sim-objs += icc_switchControl2_mt6306.o


all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	$(STRIP) --strip-unneeded *.ko

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean
