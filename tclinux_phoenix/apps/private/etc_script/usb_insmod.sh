#!/bin/sh
PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi
if [ -d "/proc/bus/usb" ] ;then
	echo "USB already insert"
else
if [ "$TCSUPPORT_USBHOST" != "" ] && [ -f "/proc/tc3162/sys_has_usbhost" ] ;then
echo "TCSUPPORT_USBHOST"
#usb host
insmod /lib/modules/usbhost/scsi_mod.ko
insmod /lib/modules/usbhost/scsi_wait_scan.ko
insmod /lib/modules/usbhost/sd_mod.ko
insmod /lib/modules/usbhost/nls_base.ko
if [ "$TCSUPPORT_SAMBA" != "" ] || [ "$TCSUPPORT_SAMBA_IPv6" != "" ] ;then
insmod /lib/modules/usbhost/nls_utf8.ko
insmod /lib/modules/usbhost/nls_cp936.ko
else
insmod /lib/modules/usbhost/nls_ascii.ko
insmod /lib/modules/usbhost/nls_cp437.ko
fi
insmod /lib/modules/usbhost/fat.ko
insmod /lib/modules/usbhost/vfat.ko
insmod /lib/modules/usbhost/usbcore.ko
if [ "$TCSUPPORT_MUH" != "" ] ;then
if [ "$TCSUPPORT_MUH_TEST" != "" ] ;then
insmod /lib/modules/usbhost/xhci-test.ko
else
insmod /lib/modules/usbhost/xhci-hcd.ko
fi
else
insmod /lib/modules/usbhost/ehci-hcd.ko
insmod /lib/modules/usbhost/ohci-hcd.ko
fi
insmod /lib/modules/usbhost/usb-storage.ko
if [ "$TCSUPPORT_USB_NTFS" != "" ] ;then
insmod lib/modules/usbhost/fuse.ko
fi
if [ "$TCSUPPORT_USB_3G_DONGLE" != "" ] ;then
insmod lib/modules/usbhost/usbserial.ko
if [ "$TCSUPPORT_2_6_36_KERNEL" != "" ] ;then
insmod lib/modules/usbhost/usb_wwan.ko
fi
insmod lib/modules/usbhost/option.ko
insmod $KERNEL_DIR/kernel/lib/crc-ccitt.ko
insmod lib/modules/ppp_async.ko
fi
if [ "$TCSUPPORT_USB_CAMERA" != "" ] ;then
insmod lib/modules/usbhost/v4l1-compat.ko
insmod lib/modules/usbhost/videodev.ko
insmod lib/modules/usbhost/usbvideo.ko
insmod lib/modules/usbhost/uvcvideo.ko
fi
if [ "$TCSUPPORT_USB_PRINTER_SERVER" != "" ] ;then
insmod lib/modules/usbhost/usblp.ko
fi	
fi
if [ "$TCSUPPORT_USB_AUTOMOUNT" != "" ] ;then
/userfs/bin/usb_auto_mount &
fi
if [ "$TCSUPPORT_USBDEVICE20" != "" ] && [ -f "/proc/tc3162/sys_has_usb_device" ];then
echo "TCSUPPORT_USBDEVICE20"
#usb device
insmod /lib/modules/tc3162_udc20.ko
insmod /lib/modules/g_ether.ko

echo "TCSUPPORT_USBDEVICE20: ifconfig usb0 0.0.0.0 up"
ifconfig usb0 0.0.0.0 up

echo "TCSUPPORT_USBDEVICE20: brctl addif br0 usb0"
brctl addif br0 usb0
fi
fi
