#!/bin/sh
# boa applicaion use this file
PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi

WLAN_MODULE_RT5392=/lib/modules/rt5392ap.ko
WLAN_MODULE_RT5390=/lib/modules/rt5390ap.ko
WLAN_MODULE_RT3390=/lib/modules/rt3390ap.ko
WLAN_MODULE_RT3090=/lib/modules/rt3090ap.ko
WLAN_MODULE_RT2561=/lib/modules/rt61ap.ko

/sbin/ifconfig ra0 down
/sbin/ifconfig ra1 down
/sbin/ifconfig ra2 down
/sbin/ifconfig ra3 down
if [ -f $WLAN_MODULE_RT5392 ] ; then
        /sbin/rmmod rt5392ap
fi

if [ -f $WLAN_MODULE_RT5390 ] ; then
	/sbin/rmmod rt5390ap
fi

if [ -f $WLAN_MODULE_RT3390 ] ; then
	/sbin/rmmod rt3390ap
fi

if [ -f $WLAN_MODULE_RT3090 ] ; then
	/sbin/rmmod rt3090ap
fi

if [ -f $WLAN_MODULE_RT2561 ] ; then
	/sbin/rmmod rt61ap
fi

if [ $# = 1 ] ; then
if [ $1 != "ROM" ] ; then
	/usr/bin/killall syslogd
fi
else
	/usr/bin/killall syslogd
	#rmmod usb modules
	/usr/bin/killall -9 usb_auto_mount 2>/dev/null
	if [ "$TCSUPPORT_USB_NTFS" != "" ] ;then
		/sbin/rmmod fuse.ko 2>/dev/null
	fi
	/sbin/rmmod usb_storage.ko 2>/dev/null
	/sbin/rmmod option.ko 2>/dev/null
	/sbin/rmmod usb_wwan.ko 2>/dev/null
	if [ "$TCSUPPORT_USBHOST" != "" ] && [ -f "/proc/tc3162/sys_has_usbhost" ] ;then
		/sbin/rmmod sd_mod.ko 2>/dev/null
		/sbin/rmmod scsi_mod.ko 2>/dev/null
		/sbin/rmmod scsi_wait_scan.ko 2>/dev/null
	fi
	/sbin/rmmod nls_ascii.ko 2>/dev/null
	/sbin/rmmod nls_cp437.ko 2>/dev/null
	/sbin/rmmod ehci_hcd.ko 2>/dev/null
	/sbin/rmmod ohci_hcd.ko 2>/dev/null
	/sbin/rmmod ppp_async.ko 2>/dev/null
	/sbin/rmmod usbcore.ko 2>/dev/null
	#rmmod hwnat module
	if [ -f "/proc/tc3162/sys_has_hwnat" ] ;then
		if [ "$TCSUPPORT_HWNAT" != "" ] ;then
			/usr/bin/hwnat active 0
			/sbin/rmmod hwnat.ko 2>/dev/null
		fi
	fi
	#kill apps
	/usr/bin/killall -9 upnp-dm
	#set cwmp download flag
	/userfs/bin/tcapi set WebCurSet_Entry cwmpFWUpgrade Yes
fi
/usr/bin/killall klogd
/usr/bin/killall bftpd
/usr/bin/killall utelnetd 
/usr/bin/killall udhcpd
/usr/bin/killall radvd
/usr/bin/killall arp

echo "3" > /proc/sys/vm/drop_caches
