#!/bin/sh

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_VLAN_TAG) || defined(TCSUPPORT_CT_IPV4_RADIO)
PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi
#endif/*TCSUPPORT_COMPILE*/

if [ $# -gt 5 ] ; then
	echo "usage: $0 [interface name] [pvc index] [pvc flag] [pppmaual flag] [pppoe flag]
	pvc flag:if flag=1,br2684ctl will be forked"
	exit 0
fi

WAN_IF=$1
PVC=$2
NAS_IF=nas$PVC
PVC_FLAG=$3
PPPMAUAL_FLAG=$4
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PPPOEPROXY)
PPPOE_FLAG=$5
#endif/*TCSUPPORT_COMPILE*/
VS_CHAIN=VS_$WAN_IF
DMZ_CHAIN=DMZ_$WAN_IF

PVC_CONFFILE=/var/run/$WAN_IF/pvc.conf
if [ -f $PVC_CONFFILE ] ; then
	chmod 777 $PVC_CONFFILE
	. $PVC_CONFFILE
else
	exit 0
fi

IF_CONFFILE=/var/run/$WAN_IF/interface.conf
if [ -f $IF_CONFFILE ] ; then
	chmod 777 $IF_CONFFILE
	. $IF_CONFFILE
else
	exit 0
fi


MAC_CONFFILE=/etc/mac.conf
if [ -f $MAC_CONFFILE ]; then
	chmod 777 $MAC_CONFFILE
	. $MAC_CONFFILE
fi
		
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
	SYS_CONFFILE=/var/run/system.conf
	if [ -f $SYS_CONFFILE ] ; then
		chmod 777 $SYS_CONFFILE
		. $SYS_CONFFILE
	else
		exit 0
	fi
fi
#endif/*TCSUPPORT_COMPILE*/
		
if [ "$Active" != "Yes" ] ; then
	exit 0
fi


#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_WAN_PTM)
if [ "$PTMEnable" = "Yes" ];  then
	NAS_IF=ptm$Barrier
fi
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON)
if [ $GPONEnable = "Yes" ] || [ "$EPONEnable" = "Yes" ] ; then
	NAS_IF=pon
fi
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_DSLITE)
if [ "$DsliteEnable" = "Yes" ] && [ "$ISP" != "3" ]; then
	IPVERSION="IPv6"
fi
#endif/*TCSUPPORT_COMPILE*/

VIRTUAL_NAS_IF=$NASName

#PPPoA not need to fork br2684ctl
if [ "$ENCAP" = "PPPoA LLC" ] || [ "$ENCAP" = "PPPoA VC-Mux" ] ; then
	PVC_FLAG=0
fi	
#pvc flag is 1
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON)
if [ "$TCSUPPORT_CT_PON" = "" ]; then
#endif/*TCSUPPORT_COMPILE*/
if [ "$PVC_FLAG" = "1" ] ; then		
	sleep 1
	if [ "$WAN_MAC" != "" ]; then
	 	/sbin/ifconfig $NAS_IF hw ether $WAN_MAC
	fi
	/sbin/ifconfig $NAS_IF up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
	if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
		if [ "$IPProtocolVersion" != "1" ]; then
			if [ "$IPVERSION" = "IPv4" ] ; then
						echo 1 > /proc/sys/net/ipv6/conf/$NAS_IF/disable_ipv6
			fi
		fi
	fi
#endif/*TCSUPPORT_COMPILE*/		
fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON)
fi
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PPPOEPROXY)
if [ "$PPPOE_FLAG" = "1" ] ; then	
	if [ "$ProxyEnable" != "1" ] ; then
		exit 0
	fi
else
	if [ "$ProxyEnable" = "1" ] ; then
		exit 0
	fi
fi
#endif/*TCSUPPORT_COMPILE*/
	
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
if [ "$TCSUPPORT_CT_VLAN_TAG" != "" ] ;then		
		if [ "$VLANMode" = "TAG" ] ; then
			if [ "$v8021PEnable" = "No" ] ; then
				VTAG_MODE=4
			else
				VTAG_MODE=3
			fi
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
		
		if [ "$MulticastVID" != "" ] ;then		
			VTAG_MultVID=$MulticastVID
		else
			VTAG_MultVID=0
		fi

		if [ "$TCSUPPORT_CT_PON" = "" ] ;then
			if [ "$PTMEnable" != "Yes" ];  then
	#			VTAG_MODE=2
	#			VTAG_VID=0
	#			VTAG_MultVID=0
				if [ "$dot1q" = "Yes" ] ;then		
					VTAG_MODE=3
					VTAG_VID=$VLANID
					if [ "$dot1p" = "Yes" ] ;then		
						VTAG_PBIT=$dot1pData
					else
						VTAG_PBIT=0
					fi
				else
					VTAG_MODE=2
					VTAG_VID=0
					VTAG_PBIT=0
				fi
			fi
		fi	
fi
#endif/*TCSUPPORT_COMPILE*/
	
#Dynamic Mode
if [ $ISP = "0" ] ; then
	#/usr/bin/smuxctl add ipoe $NAS_IF $WAN_IF
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	if [ "$TCSUPPORT_CT_VLAN_TAG" != "" ] ;then
		/usr/bin/smuxctl add ipoe $NAS_IF $NASName $VTAG_MODE $VTAG_VID $VTAG_PBIT $VTAG_MultVID
	else
#endif/*TCSUPPORT_COMPILE*/
	/usr/bin/smuxctl add ipoe $NAS_IF $NASName
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_C9)
	if [ "$TCSUPPORT_CT_PON_C9" != "" ] ;then
		if [ "$WanHWAddr" != "" ]; then
			if [ "$WanHWAddr" != "00:00:00:00:00:00" ]; then
		 		/sbin/ifconfig $NASName hw ether $WanHWAddr
		 	fi
		fi
	fi
#endif/*TCSUPPORT_COMPILE*/
	if [ "$IPVERSION" != "IPv4" ] && [ "$DHCPv6" != "Yes" ]; then
		echo 1 > /proc/sys/net/ipv6/conf/$NASName/autoconf
	fi
	/sbin/ifconfig $NASName up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_C7)
	if [ "$TCSUPPORT_C7" != "" ] ;then
        echo 1 > /proc/sys/net/ipv4/conf/$WAN_IF/arp_ignore
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
	if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
		if [ "$IPProtocolVersion" != "1" ] ; then
			if [ "$IPVERSION" = "IPv4" ] ; then
						echo 1 > /proc/sys/net/ipv6/conf/$NASName/disable_ipv6
			fi
		fi
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
if [ "$TCSUPPORT_CT_VLAN_TAG" = "" ] ;then		
#endif/*TCSUPPORT_COMPILE*/
	if [ $dot1q = "Yes" ] ; then
		/userfs/bin/vconfig add $NASName $VLANID
		/sbin/ifconfig $NASName.$VLANID up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
		if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
			if [ "$IPProtocolVersion" != "1" ] ; then
				if [ "$IPVERSION" = "IPv4" ] ; then
							echo 1 > /proc/sys/net/ipv6/conf/$NASName.$VLANID/disable_ipv6
				fi
			fi
		fi
#endif/*TCSUPPORT_COMPILE*/		
		#check if open 802.1p
		if [ $dot1p = "Yes" ] ; then
			/userfs/bin/vconfig set_egress_map $NASName.$VLANID 0 $dot1pData
			/userfs/bin/vconfig set_ingress_map $NASName.$VLANID 0 $dot1pData
		fi
	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
fi
#endif/*TCSUPPORT_COMPILE*/	
	
	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig $WAN_IF mtu 1500
		iptables -A FORWARD -o $WAN_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig $WAN_IF mtu $MTU
		iptables -A FORWARD -o $WAN_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)	
	if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
		if [ "$IPProtocolVersion" = "3" ] ; then
			if [ "$IPVERSION" != "IPv6" ] ; then
				/sbin/udhcpc -i $WAN_IF -s /usr/script/udhcpc_nodef.sh -p /var/run/$WAN_IF/udhcpc.pid &
				if [ "$NATENABLE" = "Enable" ] ; then
					iptables -t nat -A POSTROUTING -o $WAN_IF -j MASQUERADE
					iptables -t nat -N $VS_CHAIN
					iptables -t nat -N $DMZ_CHAIN
					iptables -t nat -A PREROUTING -j $VS_CHAIN
					iptables -t nat -A PREROUTING -j $DMZ_CHAIN
				fi
			fi
			if [ "$IPVERSION" != "IPv4" ] ; then
				/userfs/bin/dhcp6c -c /var/run/$WAN_IF/dhcp6c.conf -p /var/run/$WAN_IF/dhcp6c.pid -x /var/run/$WAN_IF/pd6 -X /var/run/$WAN_IF/orgpd6 -u /var/run/$WAN_IF/pd6_ptime -v /var/run/$WAN_IF/pd6_vtime $WAN_IF &
				if [ "$GW6_Manual" = "Yes" ]; then
					echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
				fi
			fi
		elif [ "$IPProtocolVersion" = "2" ] ; then
			if [ "$IPVERSION" != "IPv4" ] ; then
				/userfs/bin/dhcp6c -c /var/run/$WAN_IF/dhcp6c.conf -p /var/run/$WAN_IF/dhcp6c.pid -x /var/run/$WAN_IF/pd6 -X /var/run/$WAN_IF/orgpd6 -u /var/run/$WAN_IF/pd6_ptime -v /var/run/$WAN_IF/pd6_vtime $WAN_IF &
				if [ "$GW6_Manual" = "Yes" ]; then
					echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
				fi
			fi		
		elif [ "$IPProtocolVersion" = "1" ] ; then
			if [ "$IPVERSION" != "IPv6" ] ; then
				/sbin/udhcpc -i $WAN_IF -s /usr/script/udhcpc_nodef.sh -p /var/run/$WAN_IF/udhcpc.pid &
				if [ "$NATENABLE" = "Enable" ] ; then
					iptables -t nat -A POSTROUTING -o $WAN_IF -j MASQUERADE
					iptables -t nat -N $VS_CHAIN
					iptables -t nat -N $DMZ_CHAIN
					iptables -t nat -A PREROUTING -j $VS_CHAIN
					iptables -t nat -A PREROUTING -j $DMZ_CHAIN
				fi
			fi			
		fi
	else
#endif/*TCSUPPORT_COMPILE*/
	if [ "$IPVERSION" != "IPv6" ] ; then
#	if [ "$DEFAULTROUTE" = "Yes" ] ; then
#		/sbin/udhcpc -i $WAN_IF -s /usr/script/udhcpc.sh -p /var/run/$WAN_IF/udhcpc.pid &
#	else
		/sbin/udhcpc -i $WAN_IF -s /usr/script/udhcpc_nodef.sh -p /var/run/$WAN_IF/udhcpc.pid &
#	fi
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
		iptables -t nat -A POSTROUTING -o $WAN_IF -j MASQUERADE
		iptables -t nat -N $VS_CHAIN
		iptables -t nat -N $DMZ_CHAIN
		iptables -t nat -A PREROUTING -j $VS_CHAIN
		iptables -t nat -A PREROUTING -j $DMZ_CHAIN
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	fi
	fi
	if [ "$IPVERSION" != "IPv4" ] ; then
		/userfs/bin/dhcp6c -c /var/run/$WAN_IF/dhcp6c.conf -p /var/run/$WAN_IF/dhcp6c.pid -x /var/run/$WAN_IF/pd6 -X /var/run/$WAN_IF/orgpd6 -u /var/run/$WAN_IF/pd6_ptime -v /var/run/$WAN_IF/pd6_vtime $WAN_IF &
		if [ "$GW6_Manual" = "Yes" ]; then
			echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
		fi
	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
	fi
#endif/*TCSUPPORT_COMPILE*/

#Static Mode
elif [ $ISP = "1" ] ; then
	#/usr/bin/smuxctl add ipoe $NAS_IF $WAN_IF
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	if [ "$TCSUPPORT_CT_VLAN_TAG" != "" ] ;then
		/usr/bin/smuxctl add ipoe $NAS_IF $NASName $VTAG_MODE $VTAG_VID $VTAG_PBIT $VTAG_MultVID
	else
#endif/*TCSUPPORT_COMPILE*/
	/usr/bin/smuxctl add ipoe $NAS_IF $NASName
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_C9)
	if [ "$TCSUPPORT_CT_PON_C9" != "" ] ;then
		if [ "$WanHWAddr" != "" ]; then
			if [ "$WanHWAddr" != "00:00:00:00:00:00" ]; then
		 		/sbin/ifconfig $NASName hw ether $WanHWAddr
		 	fi
		fi
	fi
#endif/*TCSUPPORT_COMPILE*/
	
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
		iptables -t nat -A POSTROUTING -o $WAN_IF -j MASQUERADE
		iptables -t nat -N $VS_CHAIN
		iptables -t nat -N $DMZ_CHAIN
		iptables -t nat -A PREROUTING -j $VS_CHAIN
		iptables -t nat -A PREROUTING -j $DMZ_CHAIN
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	fi
	/sbin/ifconfig $NASName up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_C7)
	if [ "$TCSUPPORT_C7" != "" ] ;then
        echo 1 > /proc/sys/net/ipv4/conf/$WAN_IF/arp_ignore
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
	if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
		if [ "$IPProtocolVersion" != "1" ] ; then
			if [ "$IPVERSION" = "IPv4" ] ; then
						echo 1 > /proc/sys/net/ipv6/conf/$NASName/disable_ipv6
			fi
		fi
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	if [ "$TCSUPPORT_CT_VLAN_TAG" = "" ] ;then
#endif/*TCSUPPORT_COMPILE*/
	if [ $dot1q = "Yes" ] ; then
		/userfs/bin/vconfig add $NASName $VLANID
		/sbin/ifconfig $NASName.$VLANID up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
		if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
			if [ "$IPProtocolVersion" != "1" ] ; then
				if [ "$IPVERSION" = "IPv4" ] ; then
							echo 1 > /proc/sys/net/ipv6/conf/$NASName.$VLANID/disable_ipv6
				fi
			fi
		fi
#endif/*TCSUPPORT_COMPILE*/		
		#check if open 802.1p
		if [ $dot1p = "Yes" ] ; then
			/userfs/bin/vconfig set_egress_map $NASName.$VLANID 0 $dot1pData
			/userfs/bin/vconfig set_ingress_map $NASName.$VLANID 0 $dot1pData
		fi
	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
fi
#endif/*TCSUPPORT_COMPILE*/	

	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig $WAN_IF mtu 1500
		iptables -A FORWARD -o $WAN_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig $WAN_IF mtu $MTU
		iptables -A FORWARD -o $WAN_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	if [ "$IPVERSION" != "IPv6" ] ; then
	/sbin/ifconfig $WAN_IF $IPADDR netmask $NETMASK
#	if [ "$DEFAULTROUTE" = "Yes" ] ; then
#		route add default gw $GATEWAY dev $WAN_IF
#	fi
	

	#Notify wan_monitor
	echo "$IPADDR" > /var/run/$WAN_IF/ip
	echo "$NETMASK" >> /var/run/$WAN_IF/netmask
	echo "$GATEWAY" > /var/run/$WAN_IF/gateway
	echo "$DNS" > /var/run/$WAN_IF/dns
	echo "$SecDNS" >> /var/run/$WAN_IF/dns	
	echo "up" > /var/run/$WAN_IF/status
	/userfs/bin/tcapi commit WanInfo_Message_$WAN_IF/4 &
	fi
	
	if [ "$IPVERSION" != "IPv4" ] ; then	
		/sbin/ifconfig $WAN_IF $IPADDR6/$PREFIX6		
#		if [ "$DEFAULTROUTE" = "Yes" ] ; then
#			route -A inet6 add default gw $DEFGATEWAY6 dev $WAN_IF
#		fi
		
		#Notify wan_monitor
		echo "$IPADDR6" > /var/run/$WAN_IF/ip6
		echo "$PREFIX6" > /var/run/$WAN_IF/prefix6
		if [ "$GW6_Manual" = "Yes" ]; then
			echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6
		fi
		echo "$DNS6" > /var/run/$WAN_IF/dns6
		echo "$SecDNS6" >> /var/run/$WAN_IF/dns6		
		echo "up" > /var/run/$WAN_IF/status6
		/userfs/bin/tcapi commit WanInfo_Message_$WAN_IF/6 &
	fi

#PPP mode
elif [ $ISP = "2" ] ; then
	if [ "$PPPMAUAL_FLAG" != "1" ] ; then
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
		if [ "$TCSUPPORT_CT_VLAN_TAG" != "" ] ;then
			if [ "$BridgeInterface" = "Yes" ] ; then
		 		/usr/bin/smuxctl add pppoe_bi $NAS_IF $NASName $VTAG_MODE $VTAG_VID $VTAG_PBIT $VTAG_MultVID
			else
		 		/usr/bin/smuxctl add pppoe $NAS_IF $NASName $VTAG_MODE $VTAG_VID $VTAG_PBIT $VTAG_MultVID
			fi
		else
#endif/*TCSUPPORT_COMPILE*/
			if [ "$BridgeInterface" = "Yes" ] ; then
		 		/usr/bin/smuxctl add pppoe_bi $NAS_IF $NASName
			else
		 		/usr/bin/smuxctl add pppoe $NAS_IF $NASName
		 	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_C9)
	if [ "$TCSUPPORT_CT_PON_C9" != "" ] ;then
		if [ "$WanHWAddr" != "" ]; then
			if [ "$WanHWAddr" != "00:00:00:00:00:00" ]; then
		 		/sbin/ifconfig $NASName hw ether $WanHWAddr
		 	fi
		fi
	fi
#endif/*TCSUPPORT_COMPILE*/
			/sbin/ifconfig $NASName up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
			if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
				if [ "$IPProtocolVersion" != "1" ] ; then
					if [ "$IPVERSION" = "IPv4" ] ; then
								echo 1 > /proc/sys/net/ipv6/conf/$NASName/disable_ipv6
					fi
				fi
			fi
#endif/*TCSUPPORT_COMPILE*/			
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	if [ "$TCSUPPORT_CT_VLAN_TAG" = "" ] ;then
#endif/*TCSUPPORT_COMPILE*/
			if [ $dot1q = "Yes" ] ; then
				/userfs/bin/vconfig add $NASName $VLANID
				/sbin/ifconfig $NASName.$VLANID up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
			if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
				if [ "$IPProtocolVersion" != "1" ] ; then
					if [ "$IPVERSION" = "IPv4" ] ; then
								echo 1 > /proc/sys/net/ipv6/conf/$NASName.$VLANID/disable_ipv6
					fi
				fi
			fi
#endif/*TCSUPPORT_COMPILE*/				
				#check if open 802.1p
				if [ $dot1p = "Yes" ] ; then
					/userfs/bin/vconfig set_egress_map $NASName.$VLANID 0 $dot1pData
					/userfs/bin/vconfig set_ingress_map $NASName.$VLANID 0 $dot1pData
				fi
				VIRTUAL_NAS_IF=$NASName.$VLANID
			fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	fi
#endif/*TCSUPPORT_COMPILE*/			
	fi
	
#	PPP_UNIT=$((8*$PVC + $4))
	PPP_IF=ppp$PPPUNIT
	
	if [ "$PPPMAUAL_FLAG" = "1" ] ; then
		if [ -f /var/run/$WAN_IF/$PPP_IF.pid ] ; then
	        kill -15 `cat /var/run/$WAN_IF/$PPP_IF.pid`
	        sleep 1
		if [ -f /var/run/$WAN_IF/$PPP_IF.pid ] ; then
	        	kill -9 `cat /var/run/$WAN_IF/$PPP_IF.pid`
		fi
		fi
	fi
	
	if [ "$CONNECTION" != "Connect_Manually" ] || [ "$PPPManualStatus" = "connect" ] ; then
  #make sure the LCP echo will take no effect about ppp
#	PPP_PARAM="unit $PPPUNIT user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 1 lcp-echo-failure 60"
	PPP_PARAM="nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 10 lcp-echo-failure 6"

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON)
	if [ "$TCSUPPORT_CT_PON" != "" ]; then
		PPP_PARAM="$PPP_PARAM plugin pppoe $NASName"

		if [ "$SRVNAME" != "" ] ; then
			PPP_PARAM="$PPP_PARAM pppoe_srv_name $SRVNAME"
		fi
			
		if [ "$AUTHEN" = "CHAP" ] ; then
			 PPP_PARAM="$PPP_PARAM -pap"
	    elif [ "$AUTHEN" = "PAP" ] ; then
			 PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
		fi
	else
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_WAN_PTM)
	if [ "$PTMEnable" = "Yes" ] ; then
		PPP_PARAM="$PPP_PARAM plugin pppoe $VIRTUAL_NAS_IF" 
		if [ "$SRVNAME" != "" ] ; then
			PPP_PARAM="$PPP_PARAM pppoe_srv_name $SRVNAME"
		fi
		
		ENCAP=""
	fi
#endif/*TCSUPPORT_COMPILE*/

    if [ "$ENCAP" = "PPPoE LLC" ] || [ "$ENCAP" = "1483 Bridged IP LLC" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoe $VIRTUAL_NAS_IF" 
    elif [ "$ENCAP" = "PPPoE VC-Mux" ] || [ "$ENCAP" = "1483 Bridged IP VC-Mux" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoe $VIRTUAL_NAS_IF"
    elif [ "$ENCAP" = "PPPoA LLC" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoa llc-encaps $VPI.$VCI"
    elif [ "$ENCAP" = "PPPoA VC-Mux" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoa vc-encaps $VPI.$VCI"
    fi

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_WAN_PTM)
	if [ "$PTMEnable" = "Yes" ] ; then
		if [ "$AUTHEN" = "CHAP" ] ; then
				PPP_PARAM="$PPP_PARAM -pap"
		elif [ "$AUTHEN" = "PAP" ] ; then
				PPP_PARAM="$PPP_PARAM -chap"
		fi
		ENCAP=""
		AUTHEN=""
	else
#endif/*TCSUPPORT_COMPILE*/

    # PPPoE
    if [ "$ENCAP" = "PPPoE LLC" ] || [ "$ENCAP" = "PPPoE VC-Mux" ] || [ "$ENCAP" = "1483 Bridged IP LLC" ] || [ "$ENCAP" = "1483 Bridged IP VC-Mux" ] ; then
	#add by xyzhu_nj_20091221 for support service name & pap/chap
	if [ "$SRVNAME" != "" ] ; then
		PPP_PARAM="$PPP_PARAM pppoe_srv_name $SRVNAME"
	fi
	if [ "$AUTHEN" = "CHAP" ] ; then
		PPP_PARAM="$PPP_PARAM -pap"
	elif [ "$AUTHEN" = "PAP" ] ; then
		PPP_PARAM="$PPP_PARAM -chap"
	fi

	
    # PPPoA
    else
	PCR_V=""
	if [ $QOS = "ubr" ] ; then
		PCR_V="qos UBR qos_pcr $PCR"
	elif [ $QOS = "cbr" ] || [ $QOS = "rt-vbr" ] || [ $QOS = "nrt-vbr" ] ; then
		if [ $QOS = "cbr" ] ; then 
			PCR_V="qos CBR"
		elif [ $QOS = "rt-vbr" ] ; then
			PCR_V="qos VBR"
		elif [ $QOS = "nrt-vbr" ] ; then
			PCR_V="qos NRTVBR"
		fi
		PCR_V="$PCR_V qos_pcr $PCR qos_scr $SCR qos_mbs $MBS"
	fi
	PPP_PARAM="$PPP_PARAM $PCR_V"
    fi

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_WAN_PTM)
	fi
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON) 
 fi   
#endif/*TCSUPPORT_COMPILE*/		    

#	if [ "$DEFAULTROUTE" = "Yes" ] ; then
#		PPP_PARAM="$PPP_PARAM defaultroute"
#	fi
	if [ "$PPPGETIP" = "Dynamic" ] ; then
		PPP_PARAM="$PPP_PARAM noipdefault"
	else
		PPP_PARAM="$PPP_PARAM $IPADDR:$GATEWAY"
	fi
	if [ "$NETMASK" != "" ] ; then
		PPP_PARAM="$PPP_PARAM netmask $NETMASK"
	fi
WAN_COMMON_FILE=/var/run/wan_common.conf
	if [ -f $WAN_COMMON_FILE ] ; then
		. $WAN_COMMON_FILE
		if [ "$ReConnect" = "1" ] ; then
			if [ "$CONNECTION" = "Connect_on_Demand" ] ; then
				IDLETIME=$CLOSEIFIDLE
				PPP_PARAM="$PPP_PARAM demand idle $IDLETIME"
			else	
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PPPOEPROXY)
				if [ "$PPPOE_FLAG" != "1" ] ; then
#endif/*TCSUPPORT_COMPILE*/
					PPP_PARAM="$PPP_PARAM persist"
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PPPOEPROXY)
				fi
#endif/*TCSUPPORT_COMPILE*/	
			fi
		fi
	else	
	if [ "$CONNECTION" = "Connect_on_Demand" ] ; then
			IDLETIME=$CLOSEIFIDLE
		PPP_PARAM="$PPP_PARAM demand idle $IDLETIME"
	else
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PPPOEPROXY)
		if [ "$PPPOE_FLAG" != "1" ] ; then
#endif/*TCSUPPORT_COMPILE*/
			PPP_PARAM="$PPP_PARAM persist"
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PPPOEPROXY)
		fi
#endif/*TCSUPPORT_COMPILE*/
	fi
	fi
	
	if [ "$MTU" != "0" ] && [ "$MTU" != "" ] ; then
		PPP_PARAM="$PPP_PARAM mtu $MTU mru $MTU"
	fi

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
	if [ "$IPProtocolVersion" = "3" ] ; then
		if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
			PPP_PARAM="$PPP_PARAM ipv6 , "
			if [ "$GW6_Manual" = "Yes" ]; then
				echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
			fi
		elif [ "$IPVERSION" = "IPv6" ] ; then
			PPP_PARAM="$PPP_PARAM ipv6 , noip"
			if [ "$GW6_Manual" = "Yes" ]; then
				echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
			fi
		fi
	elif [ "$IPProtocolVersion" = "2" ] ; then
		if [ "$IPVERSION" = "IPv4/IPv6" ] || [ "$IPVERSION" = "IPv6" ] ; then
			PPP_PARAM="$PPP_PARAM ipv6 , noip"
			if [ "$GW6_Manual" = "Yes" ]; then
				echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
			fi
		elif [ "$IPVERSION" = "IPv4" ] ; then
			PPP_PARAM="$PPP_PARAM noip"		
		fi
	elif [ "$IPProtocolVersion" = "1" ] ; then
		if [ "$IPVERSION" = "IPv6" ] ; then
			PPP_PARAM="$PPP_PARAM noip"
			if [ "$GW6_Manual" = "Yes" ]; then
				echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
			fi
		fi
	fi
else
#endif/*TCSUPPORT_COMPILE*/
	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
		PPP_PARAM="$PPP_PARAM ipv6 , "
		if [ "$GW6_Manual" = "Yes" ]; then
			echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
		fi
	elif [ "$IPVERSION" = "IPv6" ] ; then
		PPP_PARAM="$PPP_PARAM ipv6 , noip"
		if [ "$GW6_Manual" = "Yes" ]; then
			echo "$GATEWAY6" > /var/run/$WAN_IF/gateway6 
		fi
	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
fi
#endif/*TCSUPPORT_COMPILE*/

#	pppd $PPP_PARAM &
#	echo $! > /var/run/$WAN_IF/$PPP_IF.pid
#	echo "pppd $PPP_PARAM &" > /var/tmp/$PPP_IF.conf

	if [ "$USERNAME" = "" ]; then
		if [ "$PASSWORD" = "" ]; then
			pppd unit $PPPUNIT user "" password "" $PPP_PARAM &	
			echo $! > /var/run/$WAN_IF/$PPP_IF.pid
			echo "pppd unit $PPPUNIT user \"\" password \"\" $PPP_PARAM &" > /var/tmp/$PPP_IF.conf
		else
			pppd unit $PPPUNIT user "" password $PASSWORD $PPP_PARAM &
			echo $! > /var/run/$WAN_IF/$PPP_IF.pid
			echo "pppd unit $PPPUNIT user \"\" password $PASSWORD $PPP_PARAM &" > /var/tmp/$PPP_IF.conf
		fi
	else
		if [ "$PASSWORD" = "" ]; then
			pppd unit $PPPUNIT user $USERNAME password "" $PPP_PARAM &	
			echo $! > /var/run/$WAN_IF/$PPP_IF.pid
			echo "pppd unit $PPPUNIT user $USERNAME password \"\" $PPP_PARAM &" > /var/tmp/$PPP_IF.conf
		else
			pppd unit $PPPUNIT user $USERNAME password $PASSWORD $PPP_PARAM &
			echo $! > /var/run/$WAN_IF/$PPP_IF.pid
			echo "pppd unit $PPPUNIT user $USERNAME password $PASSWORD $PPP_PARAM &" > /var/tmp/$PPP_IF.conf
		fi
	fi

	if [ "$LASTmss" != "0" ] ; then
		iptables -D FORWARD -o $PPP_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
	fi
	
	if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
		iptables -A FORWARD -o $PPP_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		iptables -A FORWARD -o $PPP_IF -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
	fi
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
		iptables -t nat -A POSTROUTING -o $PPP_IF -j MASQUERADE
		iptables -t nat -N $VS_CHAIN
		iptables -t nat -N $DMZ_CHAIN
		iptables -t nat -A PREROUTING -j $VS_CHAIN
		iptables -t nat -A PREROUTING -j $DMZ_CHAIN
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	fi
	
fi

#Bridge Mode
elif [ $ISP = "3" ] ; then
	#/usr/bin/smuxctl add bridge $NAS_IF $WAN_IF
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	if [ "$TCSUPPORT_CT_VLAN_TAG" != "" ] ;then
		/usr/bin/smuxctl add bridge $NAS_IF $NASName $VTAG_MODE $VTAG_VID $VTAG_PBIT $VTAG_MultVID
		if [ "$TCSUPPORT_CT_CUC" = "" ] ;then
			if [ "$IPVERSION" = "IPv6" ] ; then
			/usr/bin/smuxctl ipversion $NASName 2
			fi
			if [ "$IPVERSION" = "IPv4" ] ; then
			/usr/bin/smuxctl ipversion $NASName 1
			fi
		fi
	else
#endif/*TCSUPPORT_COMPILE*/
	/usr/bin/smuxctl add bridge $NAS_IF $NASName
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	fi
#endif/*TCSUPPORT_COMPILE*/
	/sbin/ifconfig $NASName up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
		if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
			if [ "$IPProtocolVersion" != "1" ] ; then
				if [ "$IPVERSION" = "IPv4" ] ; then
							echo 1 > /proc/sys/net/ipv6/conf/$NASName/disable_ipv6
				fi
			fi
		fi
#endif/*TCSUPPORT_COMPILE*/	
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	if [ "$TCSUPPORT_CT_VLAN_TAG" = "" ] ;then
#endif/*TCSUPPORT_COMPILE*/
	if [ $dot1q = "Yes" ] ; then
		/userfs/bin/vconfig add $NASName $VLANID
		/sbin/ifconfig $NASName.$VLANID up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
		if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
			if [ "$IPProtocolVersion" != "1" ] ; then
				if [ "$IPVERSION" = "IPv4" ] ; then
							echo 1 > /proc/sys/net/ipv6/conf/$NASName.$VLANID/disable_ipv6
				fi
			fi
		fi
#endif/*TCSUPPORT_COMPILE*/		
		#check if open 802.1p
		if [ $dot1p = "Yes" ] ; then
			/userfs/bin/vconfig set_egress_map $NASName.$VLANID 0 $dot1pData
			/userfs/bin/vconfig set_ingress_map $NASName.$VLANID 0 $dot1pData
		fi
	fi
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_VLAN_TAG)
	fi
#endif/*TCSUPPORT_COMPILE*/

	brctl addif br0 $WAN_IF
	/sbin/ifconfig $WAN_IF up
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPV4_RADIO)
	if [ "$TCSUPPORT_CT_IPV4_RADIO" != "" ]; then
		if [ "$IPProtocolVersion" != "1" ] ; then
			if [ "$IPVERSION" = "IPv4" ] ; then
						echo 1 > /proc/sys/net/ipv6/conf/$WAN_IF/disable_ipv6
			fi
		fi
	fi
#endif/*TCSUPPORT_COMPILE*/		
	echo "up" > /var/run/$WAN_IF/status
	ebtables -t filter -A INPUT -i $WAN_IF -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP
	ebtables -t filter -A INPUT -i $WAN_IF -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP
	/userfs/bin/tcapi commit WanInfo_Message_$WAN_IF/B &
fi

#Set Lan mac address
/usr/script/ether_mac.sh &

if [ $ISP = "2" ] ; then
	if [ "$CONNECTION" != "Connect_Manually" ] ; then
		if [ "$BridgeInterface" = "Yes" ] ; then
			/userfs/bin/pppoe-relay -C br0 -S $NASName &
			renice -10 `pidof pppoe-relay`
		fi
	fi
fi
/usr/bin/qoscmd dev add $NAS_IF &
#FW_CONF=/etc/firewall.conf
#if [ -f $FW_CONF ]; then
#	chmod +x $FW_CONF
#	. $FW_CONF
#	if [ "$spi_status" = "1" ]; then
#        echo spi status
#		/usr/bin/iptables -A SPI_FW -i $WAN_IF -m state --state NEW,INVALID -j DROP
#        echo spi status2
#		/usr/bin/iptables -A SPI_FW -i $WAN_IF -p icmp --icmp-type echo-request -j DROP			
#	fi
#fi

#/usr/script/ipfilter_start.sh #wtw add
/userfs/bin/hw_nat -!