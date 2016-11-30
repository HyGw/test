#

# Makefile for USB Host Controller Drivers

#

ifeq ($(CONFIG_USB_DEBUG),y)
EXTRA_CFLAGS		+= -DDEBUG
endif

xhci-test-objs := xhci.o xhci-mem.o xhci-platform.o xhci-ring.o xhci-hub.o xhci-dbg.o mtk-protocol.o mtk-test.o mtk-test-lib.o mtk-usb-hcd.o xhci-mtk-scheduler.o xhci-mtk-power.o mtk-phy.o mtk-phy-ahb.o mtk-phy-a60810.o mtk-phy-gpio.o

obj-$(CONFIG_USB_XHCI_HCD)	+= xhci-test.o
