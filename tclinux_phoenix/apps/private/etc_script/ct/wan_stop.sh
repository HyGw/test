#!/bin/sh

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON)
PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi
#endif/*TCSUPPORT_COMPILE*/

if [ $# != 3 ] ; then
	echo "usage: $0 [interface name] [pvc index] [pvc flag]"
	exit 0
fi

WAN_IF=$1
NAS_IF=nas$2
PVC_FLAG=$3
VS_CHAIN=VS_$WAN_IF
DMZ_CHAIN=DMZ_$WAN_IF
#i=$1
#CONFFILE=/etc/isp$i.conf
#echo $CONFFILE
IF_PATH=/var/run/$WAN_IF

IF_CONFFILE=/var/run/$WAN_IF/interface.conf
if [ -f $IF_CONFFILE ] ; then
	chmod 777 $IF_CONFFILE
	. $IF_CONFFILE
else
	exit 0
fi

PVC_CONFFILE=/var/run/$WAN_IF/pvc.conf
if [ -f $PVC_CONFFILE ] ; then
	chmod 777 $PVC_CONFFILE
	. $PVC_CONFFILE
else
	exit 0
fi


#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_WAN_PTM)
if [ "$PTMEnable" = "Yes" ];  then
	NAS_IF=ptm$Barrier
	ENCAP=""
fi
#endif/*TCSUPPORT_COMPILE*/


VIRTUAL_NAS_IF=$NASName
#PPP_IF=ppp$PPPUNIT

ebtables -t filter -D INPUT -i $WAN_IF -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP 2>/dev/null
ebtables -t filter -D INPUT -i $WAN_IF -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP 2>/dev/null

iptables -D FORWARD -o $WAN_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
#iptables -D FORWARD -o $PPP_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu

iptables -t nat -D POSTROUTING -o $WAN_IF -j MASQUERADE
#iptables -t nat -D POSTROUTING -o $PPP_IF -j MASQUERADE
#iptables -t nat -D POSTROUTING -j ADDRMAP_POS$i
iptables -t nat -D PREROUTING -j $VS_CHAIN
iptables -t nat -D PREROUTING -j $DMZ_CHAIN
#iptables -t nat -D PREROUTING -j ADDRMAP_PRE$i
# flush nat rule
iptables -t nat -F $VS_CHAIN
iptables -t nat -F $DMZ_CHAIN
#iptables -t nat -F ADDRMAP_PRE$i
#iptables -t nat -F ADDRMAP_POS$i
# flush ipfilter rule
iptables -F PKT_FLT

if [ -f $IF_PATH/udhcpc.pid ] ; then
	#shnwind modify ->use SIGTERM to send release
	kill -SIGTERM `cat $IF_PATH/udhcpc.pid`
fi

if [ -f $IF_PATH/dhcp6c.pid ] ; then
	kill -SIGTERM `cat $IF_PATH/dhcp6c.pid`
	rm $IF_PATH/dhcp6c.pid
fi

if [ -f /var/run/$WAN_IF/ppp$PPPUNIT.pid ] ; then
        kill -15 `cat /var/run/$WAN_IF/ppp$PPPUNIT.pid`
        sleep 1
	if [ -f /var/run/$WAN_IF/ppp$PPPUNIT.pid ] ; then
        	kill -9 `cat /var/run/$WAN_IF/ppp$PPPUNIT.pid`
	fi
fi

#/sbin/ifconfig nas$i down
#brctl delif br0 nas$i

MAC_CONFFILE=/etc/mac.conf
if [ -f $MAC_CONFFILE ]; then
	chmod 777 $MAC_CONFFILE
	. $MAC_CONFFILE
fi
		
if [ "$LAN_MAC" != "" ]; then
	/sbin/ifconfig br0 hw ether $LAN_MAC
fi

if [ -f /var/run/pppoe-relay_$VIRTUAL_NAS_IF.pid ] ; then
	kill -9 `cat /var/run/pppoe-relay_$VIRTUAL_NAS_IF.pid`
	rm /var/run/pppoe-relay_$VIRTUAL_NAS_IF.pid
fi
	echo -e  " ">/etc/dnsmasq.conf

/usr/bin/qoscmd dev del $NAS_IF &

if [ $dot1q = "Yes" ] ; then
	echo "802.1q remove"
	/sbin/ifconfig $VIRTUAL_NAS_IF.$VLANID down
	/userfs/bin/vconfig rem $VIRTUAL_NAS_IF.$VLANID
fi

#No nas interface for PPPoA mode
if [ "$ENCAP" != "PPPoA LLC" ] && [ "$ENCAP" != "PPPoA VC-Mux" ] ; then
	/sbin/ifconfig $VIRTUAL_NAS_IF down
	/usr/bin/smuxctl rem $VIRTUAL_NAS_IF		
fi

if [ "$TCSUPPORT_RA_HWNAT" != ""]; then
	/userfs/bin/hw_nat -!
fi

#record down interface info, bridge or route mode, ipv4 or ipv6
if [ "$ISP" = "3" ] ; then
	echo "B -1">/var/run/down.conf
else
	if [ "$IPVERSION" = "IPv4" ] ; then
		echo "R IPv4">/var/run/down.conf
	elif [ "$IPVERSION" = "IPv6" ] ; then
		echo "R IPv6">/var/run/down.conf
	else
		echo "R IPv4/IPv6">/var/run/down.conf
	fi
fi

#Delete interface folder
rm -rf /var/run/$WAN_IF

#FW_CONF=/etc/firewall.conf
#if [ -f $FW_CONF ]; then
#	chmod +x $FW_CONF
#	. $FW_CONF
#	if [ "$spi_status" = "1" ]; then
#		/usr/bin/iptables -D SPI_FW -i nas$i -m state --state NEW,INVALID -j DROP
#		/usr/bin/iptables -D SPI_FW -i ppp$i -m state --state NEW,INVALID -j DROP
#		/usr/bin/iptables -D SPI_FW -i nas$i -p icmp --icmp-type echo-request -j DROP
#		/usr/bin/iptables -D SPI_FW -i ppp$i -p icmp --icmp-type echo-request -j DROP
#	fi
#fi									

#/usr/script/ipfilter_stop.sh #wtw add
/userfs/bin/hw_nat -!