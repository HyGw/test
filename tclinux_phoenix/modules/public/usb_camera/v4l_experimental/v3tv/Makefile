#
# Voodoo3 3500 TV Video For Linux
#

obj-$(CONFIG_VIDEO_V3TV) += v3tv.o vpx3224.o i2c-voodoo3.o

CPPFLAGS:= -D__KERNEL__ -I$(M)/../../linux/include -I$(M)/../../v4l $(CPPFLAGS) -I$(KDIR)/include

ifeq ($(KERNELRELEASE),)
  ifeq ($(strip $(KREL)),)
    KREL := $(shell uname -r)
  endif
  ifeq ($(strip $(KDIR)),)
    KDIR := /lib/modules/$(KREL)/build
  endif
  INSTALL_MOD_DIR := v3tv

  .PHONY: all default install clean modules
  default: all
  all: modules

  M:=$(shell pwd)

  CPPFLAGS:= -D__KERNEL__ -I$(M)/../../linux/include -I$(M)/../../v4l $(CPPFLAGS) -I$(KDIR)/include

  modules modules_install clean:
	$(MAKE) -C $(M)/../.. links
	$(MAKE) INSTALL_MOD_DIR=$(INSTALL_MOD_DIR) "CPPFLAGS=$(CPPFLAGS)" -C $(KDIR) M=$(M) CONFIG_VIDEO_V3TV=m $@

  install:
	$(MAKE) -C $(M)/../.. links
	$(MAKE) INSTALL_MOD_DIR=$(INSTALL_MOD_DIR) "CPPFLAGS=$(CPPFLAGS)" -C $(KDIR) M=$(M) CONFIG_VIDEO_V3TV=m modules_install

else

  # Backwards compatibility in case kbuild can't find Kbuild on its own.
  include Kbuild

endif
