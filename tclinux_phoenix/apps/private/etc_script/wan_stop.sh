#!/bin/sh

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    chmod 777 $PROFILE_CFG
    . $PROFILE_CFG
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
if [ $# != 2 ] ; then
	echo "usage: $0 [PVCn] [Service_Num]"
	exit 0
fi
else	
if [ $# != 1 ] ; then
	echo "usage: $0 [PVCn]"
	exit 0
fi
fi

i=$1
if [ "$i" = "8" ] || [ "$i" = "9" ] || [ "$i" = "10" ] ; then
	isPTMETHER=1
else
	isPTMETHER=0
fi  
k=$1
if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
	if [ "$isPTMETHER" = "1" ] ; then
		org_i=$i
		serv_num=$2
		i="$i"_"$serv_num"
		k="$org_i""$serv_num"
	fi	
fi

CONFFILE=/etc/isp$i.conf
#echo $CONFFILE
ebtables -t filter -D INPUT -i nas$i -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP 2>/dev/null
ebtables -t filter -D INPUT -i nas$i -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP 2>/dev/null
ebtables -t filter -D OUTPUT -o nas$i -p IPv6 --ip6-proto 58 --ip6-icmpv6type 134 -j DROP 2>/dev/null

iptables -D FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
iptables -D FORWARD -o ppp$k -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu

iptables -t nat -D POSTROUTING -o nas$i -j MASQUERADE
iptables -t nat -D POSTROUTING -o ppp$k -j MASQUERADE
#iptables -t nat -D POSTROUTING -j ADDRMAP_POS$i
if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
iptables -t nat -D ADDRMAP_POS -j ADDRMAP_POS$i
fi
iptables -t nat -D PREROUTING -j VS_PRE$i
iptables -t nat -D PREROUTING -j DMZ_PRE$i
#iptables -t nat -D PREROUTING -j ADDRMAP_PRE$i
if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
iptables -t filter -D ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
fi
if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
iptables -t nat -D PREROUTING -i nas$i -j PREROUTING_WAN
iptables -t nat -D PREROUTING -i ppp$k -j PREROUTING_WAN
iptables -t filter -D FORWARD -i nas$i -j FORWARD_WAN
iptables -t filter -D FORWARD -i ppp$k -j FORWARD_WAN
fi
if [ "$TCSUPPORT_DHCP_NOFORWARD" != "" ] ;then
	iptables -t filter -D FORWARD -o ppp$k -p udp --sport 67 -j DROP
fi
# flush nat rule
iptables -t nat -F VS_PRE$i
iptables -t nat -F DMZ_PRE$i
#iptables -t nat -F ADDRMAP_PRE$i
#iptables -t nat -F ADDRMAP_POS$i
# flush ipfilter rule
iptables -F PKT_FLT

if [ -f /var/run/ppp$k.pid ] ; then
        kill -15 `cat /var/run/ppp$k.pid`
        sleep 2
	if [ -f /var/run/ppp$k.pid ] ; then
        	kill -9 `cat /var/run/ppp$k.pid`
		rm -f /var/run/ppp$k.pid
	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_BHARTI)
else 
if [ -f /var/run/ppp$k.pid ] ; then
        kill -9 `cat /var/run/pppd$k.pid`
fi
#endif/*TCSUPPORT_COMPILE*/
fi

if [ -f /var/run/udhcpc-nas$i.pid ] ; then
	#shnwind modify ->use SIGTERM to send release
	kill -SIGTERM `cat /var/run/udhcpc-nas$i.pid`
	IS_TERM=0
	times=0
	#timeout after 2 sec
	while [ "$IS_TERM" = 0 ] && [ $times -lt 20 ]
	do
		UDHCPC_PIDOF=`/bin/pidof udhcpc`
		IS_TERM=1
		for j in $UDHCPC_PIDOF ; do
			if [ "$j" = "$UDHCPC_PID" ] ; then
				IS_TERM=0
			fi
		done
		if [ -f /var/run/udhcpc_nas$i.pid ] ; then
			times=`expr $times + 1`
			sleep 0.1
		fi
	done

	if [ "$times" = 20 ] ; then
		kill -9 `cat /var/run/udhcpc-nas$i.pid` 
	fi
	rm -f /var/run/udhcpc-nas$i.pid
fi
if [ -f /var/run/dhcp6c_nas$i.pid ] ; then
	#kill -9 `cat /var/run/dhcp6c_nas$i.pid`
	kill -SIGTERM `cat /var/run/dhcp6c_nas$i.pid` 
	IS_TERM=0
	times=0
	#Wait dhcp6c being killed
	#timeout after 2 sec
	while [ "$IS_TERM" = 0 ] && [ $times -lt 20 ]
	do
		DHCP6C_PIDOF=`/bin/pidof dhcp6c`
		IS_TERM=1
		for j in $DHCP6C_PIDOF ; do
			if [ "$j" = "$DHCP6C_PID" ] ; then
				IS_TERM=0
			fi
		done
		times=`expr $times + 1`
		sleep 0.1
	done

	if [ "$times" = 20 ] ; then
		kill -9 `cat /var/run/dhcp6c_nas$i.pid` 
	fi

	rm -f /var/run/dhcp6c_nas$i.pid
fi

if [ -f /var/run/dhcp6c-nas$i.info ] ; then
	rm -f /var/run/dhcp6c-nas$i.info
fi

if [ -f /var/run/dhcp6c_ppp$k.pid ] ; then
	#kill -9 `cat /var/run/dhcp6c_ppp$i.pid`
	kill -SIGTERM `cat /var/run/dhcp6c_ppp$k.pid`
	IS_TERM=0
	times=0
	#Wait dhcp6c being killed
	#timeout after 2 sec
	while [ "$IS_TERM" = 0 ] && [ $times -lt 20 ]
	do
		DHCP6C_PIDOF=`/bin/pidof dhcp6c`
		IS_TERM=1
		for j in $DHCP6C_PIDOF ; do
			if [ "$j" = "$DHCP6C_PID" ] ; then
				IS_TERM=0
			fi
		done
		times=`expr $times + 1`
		sleep 0.1
	done

	if [ "$times" = 20 ] ; then
		kill -9 `cat /var/run/dhcp6c_ppp$k.pid`
	fi

	rm -f /var/run/dhcp6c_ppp$k.pid
fi

if [ -f /var/run/dhcp6c-ppp$k.info ] ; then
	rm -f /var/run/dhcp6c-ppp$k.info
fi
if [ -f /var/run/dslite_on_nas$i ] ; then
	ip -6 tunnel del dslite
	rm -f /var/run/dslite_on_nas$i
fi
if [ -f /var/run/dslite_on_ppp$k ] ; then
	ip -6 tunnel del dslite
	rm -f /var/run/dslite_on_ppp$k
fi

/sbin/ifconfig nas$i down
brctl delif br0 nas$i

if [ -f /var/run/nas$i.pid ] ; then
	kill -9 `cat /var/run/nas$i.pid`
	rm /var/run/nas$i.pid
fi

MAC_CONFFILE=/etc/mac.conf
if [ -f $MAC_CONFFILE ]; then
	chmod 777 $MAC_CONFFILE
	. $MAC_CONFFILE
fi
		
if [ "$LAN_MAC" != "" ]; then
	/sbin/ifconfig br0 hw ether $LAN_MAC
fi

if [ -f /var/run/pppoe-relay_nas$i.pid ] ; then
	kill -9 `cat /var/run/pppoe-relay_nas$i.pid`
	rm /var/run/pppoe-relay_nas$i.pid
fi
if [ -f /var/tmp/ipaddr6_nas$i.msg ] ; then

	if [ -f /var/tmp/gw6_nas$i.msg ] ; then 
		#/sbin/route -A inet6 del default gw `/var/tmp/gw6_nas$i.msg` dev nas$i 
		 ip route -A inet6 del default gw `/var/tmp/gw6_nas$i.msg` dev nas$i
	fi

# ifconfig control nas device, may influence smuxctl unregister nas device
# so use forground mode to run this command
	/sbin/ifconfig nas$i del `cat /var/tmp/ipaddr6_nas$i.msg` 
	
	rm -f /var/tmp/ipaddr6_nas$i.msg
	rm -f /var/tmp/gw6_nas$i.msg

fi
# 	below line must be marked!!!!!!
#	echo -e  " ">/etc/dnsmasq.conf

# qoscmd use tc to control nas device, influence smuxctl unregister nas device
# so use forground mode to run this command
/usr/bin/qoscmd dev del nas$i 

if [ "$TCSUPPORT_WAN_GPON" != "" ] || [ "$TCSUPPORT_WAN_EPON" != "" ]; then
	/usr/bin/smuxctl rem nas$i           
fi

if [ "$TCSUPPORT_RA_HWNAT" != ""]; then
	/userfs/bin/hw_nat -!
fi

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
