#
# Makefile for USB Host Controller Drivers
#

ifeq ($(CONFIG_USB_DEBUG),y)
	EXTRA_CFLAGS		+= -DDEBUG
endif

xhci-hcd-objs := xhci.o xhci-mem.o xhci-plat.o xhci-ring.o xhci-hub.o xhci-dbg.o mtk-phy.o xhci-mtk-scheduler.o xhci-mtk-power.o xhci-mtk.o mtk-phy-ahb.o mtk-phy-a60810.o mtk-phy-gpio.o


obj-$(CONFIG_USB_XHCI_HCD)	+= xhci-hcd.o
