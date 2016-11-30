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
fi
/usr/bin/killall klogd
/usr/bin/killall bftpd
/usr/bin/killall utelnetd 
/usr/bin/killall udhcpd
/usr/bin/killall radvd
/usr/bin/killall arp

echo "3" > /proc/sys/vm/drop_caches
