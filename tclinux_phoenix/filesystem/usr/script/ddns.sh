#!/bin/sh

SERVER_DYNDNS=www.dyndns.org
SERVER_JUSTLINUX=www.justlinux.com
SERVER_DHS=www.dhs.org
SERVER_ODS=www.ods.org
SERVER_GNUDIP=www.gnudip.cheapnet.net
SERVER_TZO=www.tzo.net
SERVER_EASYDNS=www.easydns.com
SERVER_ZONEEDIT=www.zoneedit.com
SERVER_HN=www.hn.org
SERVER_EZIP=www.ez-ip.net
SERVER_NOIP=www.no-ip.com

IPUPDATE_CONF=/etc/ipupdate.conf
DDNS_CONF=/etc/ddns.conf
IPUPDATE_PID=/var/log/ez-ipupdate.pid

	if [ ! -f $DDNS_CONF ] ; then
		echo "error:$DDNS_CONF"
		exit 1
	fi

	rm -f $IPUPDATE_CONF
	chmod 755 $DDNS_CONF	
	. $DDNS_CONF
	if [ "$Active" = "No" ] ; then
		exit 0
	fi
	
	echo "user=$USERNAME:$PASSWORD" >>$IPUPDATE_CONF
	echo "host=$MYHOST" >>$IPUPDATE_CONF
#	echo "cache-file=/tmp/ez-ipupdate.cache" >>$IPUPDATE_CONF
	
	if [	"$WILDCARD" = "Yes" ] ; then
		echo "wildcard" >>$IPUPDATE_CONF
	fi
	
	if [  "$SERVERNAME" = "$SERVER_DYNDNS" ] ;  then
		echo "service-type=dyndns" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_JUSTLINUX" ] ; then
		echo "service-type=justlinux" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_DHS" ] ; then
		echo "service-type=dhs" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [ "$SERVERNAME" = "$SERVER_ODS" ] ; then
	  	echo "service-type=ods" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_GNUDIP" ] ; then
		echo "service-type=gnudip" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_TZO" ] ; then
		echo "service-type=tzo" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_EASYDNS" ] ; then
		echo "service-type=easydns" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_ZONEEDIT" ] ; then
		echo "service-type=zoneedit" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_HN" ] ; then
		echo "service-type=hn" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_EZIP" ] ; then
		echo "service-type=ez-ip" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_NOIP" ] ; then
		echo "service-type=no-ip" >>$IPUPDATE_CONF
	else
		echo "error:$SERVERNAME"
		exit 1
	fi
	
#	echo kill run script
#	if [ -e $IPUPDATE_PID ]; then
#		kill -9 `cat $IPUPDATE_PID`
#	fi
	
#/etc/ddns_run.sh
