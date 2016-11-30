#!/bin/sh
# boa applicaion use this file
PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi

WLAN_MODULE_RT5390=/lib/modules/rt5390ap.ko
WLAN_MODULE_RT3390=/lib/modules/rt3390ap.ko
WLAN_MODULE_RT3090=/lib/modules/rt3090ap.ko
WLAN_NODULE_RT2561=/lib/modules/rt61ap.ko

VOIP_MODULE_PCMDUMP=/lib/modules/pcmDump.ko
VOIP_MODULE_OVDSP=/lib/modules/ovdsp.ko
VOIP_MODULE_FOIP=/lib/modules/foip.ko
VOIP_MODULE_ACODEC=/lib/modules/acodec_x.ko
VOIP_MODULE_ORTP=/lib/modules/ortp.ko
VOIP_MODULE_KSOCKET=/lib/modules/ksocket.ko
VOIP_MODULE_FXS=/lib/modules/fxs3.ko
VOIP_MODULE_SLIC=/lib/modules/slic3.ko
VOIP_MODULE_LEC=/lib/modules/lec.ko
VOIP_MODULE_PCM=/lib/modules/pcm.ko
VOIP_MODULE_SYS=/lib/modules/sys_mod.ko

/sbin/ifconfig ra0 down 2>/dev/null
/sbin/ifconfig ra1 down 2>/dev/null
/sbin/ifconfig ra2 down 2>/dev/null
/sbin/ifconfig ra3 down 2>/dev/null

if [ -f $WLAN_MODULE_RT5390 ] ; then

	#echo "rmmod rt5390ap"
	/sbin/rmmod rt5390ap 2>/dev/null
fi

if [ -f $WLAN_MODULE_RT3390 ] ; then
	#echo "rmmod rt3390ap"
	/sbin/rmmod rt3390ap 2>/dev/null
fi

if [ -f $WLAN_MODULE_RT3090 ] ; then
	#echo "rmmod rt3090ap"
	/sbin/rmmod rt3090ap 2>/dev/null
fi

if [ -f $WLAN_MODULE_RT2561 ] ; then
	#echo "rmmod rt2561ap"
	/sbin/rmmod rt61ap 2>/dev/null
fi

#remove voip modules
/usr/bin/killall -9 sipclient 2>/dev/null
if [ -f $VOIP_MODULE_PCMDUMP ] ; then
	/sbin/rmmod pcmDump
fi

if [ -f $VOIP_MODULE_OVDSP ] ; then
	/sbin/rmmod ovdsp
fi

if [ -f $VOIP_MODULE_FOIP ] ; then
	/sbin/rmmod foip
fi

if [ -f $VOIP_MODULE_ACODEC ] ; then
	/sbin/rmmod acodec_x
fi

if [ -f $VOIP_MODULE_ORTP ] ; then
	/sbin/rmmod ortp
fi

if [ -f $VOIP_MODULE_KSOCKET ] ; then
	/sbin/rmmod ksocket
fi

if [ -f $VOIP_MODULE_FXS ] ; then
	/sbin/rmmod fxs3
fi

if [ -f $VOIP_MODULE_SLIC ] ; then
	/sbin/rmmod slic3
fi

if [ -f $VOIP_MODULE_LEC ] ; then
	/sbin/rmmod lec
fi

if [ -f $VOIP_MODULE_PCM ] ; then
	/sbin/rmmod pcm
fi
#end of remove voip module

/usr/bin/ebtables -F
/sbin/rmmod ebt_ip 2>/dev/null
/sbin/rmmod ebtable_filter 2>/dev/null
/sbin/rmmod ebtables 2>/dev/null
/sbin/rmmod sch_htb 2>/dev/null

#rmmod usb modules
/usr/bin/killall -9 usb_auto_mount 2>/dev/null
		if [ "$TCSUPPORT_USB_CAMERA" != "" ] ;then
			/sbin/rmmod lib/modules/usbhost/usbvideo.ko
			/sbin/rmmod lib/modules/usbhost/uvcvideo.ko
			/sbin/rmmod lib/modules/usbhost/videodev.ko
			/sbin/rmmod lib/modules/usbhost/v4l1-compat.ko
		fi
		if [ "$TCSUPPORT_USB_NTFS" != "" ] ;then
			/sbin/rmmod fuse.ko 2>/dev/null
		fi
		if [ "$TCSUPPORT_USB_PRINTER_SERVER" != "" ] ;then
			/sbin/rmmod usblp.ko 2>/dev/null
		fi
		/sbin/rmmod usb_storage.ko 2>/dev/null
		/sbin/rmmod option.ko 2>/dev/null
		/sbin/rmmod usb_wwan.ko 2>/dev/null
		if [ "$TCSUPPORT_USB_3G_DONGLE" != "" ] ;then
			/sbin/rmmod usbserial.ko 2>/dev/null
		fi
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
/usr/bin/killall -9 dnsmasq 2>/dev/null
/usr/bin/killall -9 udhcpd 2>/dev/null
/usr/bin/killall -9 radvd 2>/dev/null
/usr/bin/killall -9 arp 2>/dev/null
/usr/bin/killall -9 wscd 2>/dev/null
/usr/bin/killall -9 tr69 2>/dev/null
/usr/bin/killall -9 dropbear 2>/dev/null
/usr/bin/killall -9 utelnetd 2>/dev/null
/usr/bin/killall -9 klogd 2>/dev/null
/usr/bin/killall -9 syslogd 2>/dev/null
/usr/bin/killall -9 snmpd 2>/dev/null
/usr/bin/killall -9 upnpd 2>/dev/null
/usr/bin/killall -9 udhcpc 2>/dev/null
/usr/bin/killall -9 igmpproxy 2>/dev/null
/usr/bin/killall -9 rt2860apd 2>/dev/null
/usr/bin/killall -9 skbmgr 2>/dev/null
/usr/bin/killall -9 ra_menu 2>/dev/null
/usr/bin/killall -9 pppoe-relay 2>/dev/null
/usr/bin/killall -9 siproxd 2>/dev/null
/usr/bin/killall -9 rtdot1xd 2>/dev/null
/usr/bin/killall -9 wpa_supplicant 2>/dev/null
#/usr/bin/killall -9 pppd 2>/dev/null
#/usr/bin/killall -9 br2684ctl 2>/dev/null
/usr/bin/killall -9 dhcp6s 2>/dev/null
/usr/bin/killall -9 sipclient 2>/dev/null
/usr/bin/killall -9 zebra 2>/dev/null
/usr/bin/killall -9 ripd 2>/dev/null
/usr/bin/killall -9 ecmh 2>/dev/null
case $1 in
	"1") 
		/usr/bin/killall -9 tftpd 2>/dev/null
		/usr/bin/killall -9 bftpd 2>/dev/null
		/usr/bin/killall -9 inetd 2>/dev/null ;;
	"2")        
		/usr/bin/killall -9 boa 2>/dev/null
		/usr/bin/killall -9 bftpd 2>/dev/null
		/usr/bin/killall -9 inetd 2>/dev/null ;;
	"3")        
		/usr/bin/killall -9 boa 2>/dev/null
		/usr/bin/killall -9 tftpd 2>/dev/null ;;
esac


echo 0 > /proc/net/skbmgr_hot_list_len
echo 3 > /proc/sys/vm/drop_caches
