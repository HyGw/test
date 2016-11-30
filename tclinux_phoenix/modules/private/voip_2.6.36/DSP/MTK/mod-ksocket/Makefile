##
#	@author mtk01662 
#

##
#	@brief module info
#
NAME=ksocket
KSOCK_DIR :=$(shell pwd)
PREFIX   = ../install
include $(MODULES_MTK_KSOCK_DIR)/$(PREFIX)/config.mk

KERNELDIR ?= $(KERNEL_DIR)

##
#       @brief the module installation path
#
INSTALL_MOD_DIR=kernel/drivers/misc

##
#	@brief object files of this module 
#
$(NAME)-objs=src/ksocket_main.o src/ksocket_api.o

obj-m +=$(NAME).o
##
#	@brief Compile Configuraton
# 
CONFIG_PROC_FS=1

##
#	@brief
# 
#CONFIG=-I$(shell pwd) -include base/conf_base.h -include bsp/conf_bsp.h -include syss/conf_syss.h

ifeq ($(strip $(CONFIG_PROC_FS)),1)
    CONFIG += -D CONFIG_PROC_FS=1
endif

##
#	@brief unit test to generate 
#
UNIT=

##
#       @brief set your own config here
#
CONFIG+=

##
#       @breif include the kernel module Makefile template
#
#include $(PREFIX)/Makefile.kmod.template
EXTRA_CFLAGS +=$(CONFIG) -I$(MODULES_MTK_KSOCK_DIR)/include -I$(MODULES_MTK_DSP_DIR)/install/mips-linux-uclibc/include

export EXTRA_CFLAGS
all:
	$(MAKE) -C $(KERNEL_DIR) M=`pwd` modules
	$(STRIP) --strip-unneeded $(MODULES_MTK_KSOCK_DIR)/$(NAME).ko
	@date
clean:
	rm -rf $(KSOCK_DIR)/src/*.o $(KSOCK_DIR)/*.ko $(KSOCK_DIR)/*.o $(KSOCK_DIR)/*.mod.c $(KSOCK_DIR)/*.cmd
	rm -f Module.symvers
	@date


