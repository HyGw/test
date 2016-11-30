#!/bin/sh

IP_CONFFILE=/var/run/ipoe_emu.conf
if [ -f $IP_CONFFILE ] ; then
	. $IP_CONFFILE
else
	echo "not found ipoe_emu.conf"
	exit 0
fi

if [ "$VLANMode" = "TAG" ] ; then
	VTAG_MODE=3
elif [ "$VLANMode" = "UNTAG" ] ; then
	VTAG_MODE=1
else #transparent
	VTAG_MODE=2
fi

if [ "$VLANID" != "" ] ;then		
	VTAG_VID=$VLANID
else
	VTAG_VID=0
fi

if [ "$DOT1P" != "" ] ;then		
	VTAG_PBIT=$DOT1P
else
	VTAG_PBIT=0
fi

/usr/bin/smuxctl add ipoe $NAS_IF $NASName $VTAG_MODE $VTAG_VID $VTAG_PBIT 0
if [ "$UserMac" != "" ]; then
	if [ "$UserMac" != "00:00:00:00:00:00" ]; then
 		/sbin/ifconfig $NASName hw ether $UserMac
 	fi
fi
/sbin/ifconfig $NASName up

/sbin/udhcpc -i $NASName -s /usr/script/udhcpc_ipoe_sim.sh -p /var/run/ipoe_emu.pid -n -e &

