#!/bin/sh

if [ $# != 1 ] ; then
	echo "usage: $0 [PVCn]"
	exit 0
fi

i=$1
CONFFILE=/etc/isp$i.conf
echo $CONFFILE

if [ -f $CONFFILE ] ; then
	chmod 777 $CONFFILE
	. $CONFFILE
else
	exit 0
fi

MAC_CONFFILE=/etc/mac.conf
if [ -f $MAC_CONFFILE ]; then
	chmod 777 $MAC_CONFFILE
	. $MAC_CONFFILE
fi
		
if [ "$Active" != "Yes" ] ; then
	exit 0
fi

if [ $ISP = "0" ] ; then
	if [ $QOS = "ubr" ] ; then
		PCR_V="-p $PCR"
	elif [ $QOS = "cbr" ] || [ $QOS = "rt-vbr" ] || [ $QOS = "nrt-vbr" ] ; then
		PCR_V="-p $PCR -q $SCR -m $MBS"
	fi
	
	if [ "$ENCAP" = "1483 Bridged IP LLC" ] ; then
		ENCAP_T="-e 0"
	elif [ "$ENCAP" = "1483 Bridged IP VC-Mux" ] ; then
		ENCAP_T="-e 1"
	elif [ "$ENCAP" = "1483 Routed IP LLC(IPoA)" ] ; then
		ENCAP_T="-e 2"
	elif [ "$ENCAP" = "1483 Routed IP VC-Mux" ] ; then
		ENCAP_T="-e 3"
	fi
	
	br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
	echo $! > /var/run/nas$i.pid
	sleep 1
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
	fi
	/sbin/ifconfig nas$i 0.0.0.0
	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig nas$i mtu 1500
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig nas$i mtu $MTU
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	if [ "$DEFAULTROUTE" = "Yes" ] ; then
		/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid &
	else
		/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid &
	fi
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
		iptables -t nat -A POSTROUTING -o nas$i -j MASQUERADE
		iptables -t nat -A PREROUTING -j VS_PRE$i
		iptables -t nat -A PREROUTING -j DMZ_PRE$i
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	fi
	WAN_IF=nas$i
	rm -f /etc/dhcp6c_$WAN_IF.conf
	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
		if [ "$EnableDynIPv6" = "1" ] ; then
			if [ "$PPPv6PD" = "Yes" ] ; then
				echo -e "interface $WAN_IF { \\n\\tsend ia-pd 213;\\n\\tsend ia-na 210;\\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\n id-assoc pd 213 {\\n\\tprefix-interface br0\\n\\t{\\n\\t\\tsla-len 0;\\n\\t};\\n};\\nid-assoc na 210 { };\\n ">/etc/dhcp6c_$WAN_IF.conf
			else
				echo -e "interface $WAN_IF { \\n\\tsend ia-na 210;\\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\nid-assoc na 210 { };\\n ">/etc/dhcp6c_$WAN_IF.conf
			fi
			
		else
			if [ "$PPPv6PD" = "Yes" ] ; then
				echo -e "interface $WAN_IF { \\n\\tsend ia-pd 213;\\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\n id-assoc pd 213 {\\n\\tprefix-interface br0\\n\\t{\\n\\t\\tsla-len 0;\\n\\t};\\n};\\n">/etc/dhcp6c_$WAN_IF.conf
			else
				echo -e "interface $WAN_IF { \\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\n">/etc/dhcp6c_$WAN_IF.conf
			fi
			
		fi
	fi
	
elif [ $ISP = "1" ] ; then
	if [ $QOS = "ubr" ] ; then
		PCR_V="-p $PCR"
	elif [ $QOS = "cbr" ] || [ $QOS = "rt-vbr" ] || [ $QOS = "nrt-vbr" ] ; then
		PCR_V="-p $PCR -q $SCR -m $MBS"
	fi
	
	if [ "$ENCAP" = "1483 Bridged IP LLC" ] ; then
		ENCAP_T="-e 0"
	elif [ "$ENCAP" = "1483 Bridged IP VC-Mux" ] ; then
		ENCAP_T="-e 1"
	elif [ "$ENCAP" = "1483 Routed IP LLC(IPoA)" ] ; then
		ENCAP_T="-e 2"
	elif [ "$ENCAP" = "1483 Routed IP VC-Mux" ] ; then
		ENCAP_T="-e 3"
	fi
	
	br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
	echo $! > /var/run/nas$i.pid
	sleep 1
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
	fi
	/sbin/ifconfig nas$i $IPADDR netmask $NETMASK up
	/sbin/ifconfig nas$i $IPADDR6/$PREFIX6
	echo -e "server=$DNSIPv61st@nas$i\\nserver=$DNSIPv62nd@nas$i">>/etc/dnsmasq.conf

	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig nas$i mtu 1500
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig nas$i mtu $MTU
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	if [ "$DEFAULTROUTE" = "Yes" ] ; then
		route add default gw $GATEWAY dev nas$i
		route -A inet6 add default gw $DEFGATEWAY6 dev nas$i 
		echo -e "$IPADDR6/$PREFIX6">/var/tmp/ipaddr6_nas$i.msg
		echo -e "$DEFGATEWAY">/var/tmp/gw6_nas$i.msg
		#/sbin/route -A inet6 add $IPADDR6/$PREFIX6 gw $DEFGATEWAY6 dev $WAN_IF
	# route add default gw 
	fi
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
		iptables -t nat -A POSTROUTING -o nas$i -j MASQUERADE
		iptables -t nat -A PREROUTING -j VS_PRE$i
		iptables -t nat -A PREROUTING -j DMZ_PRE$i
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	fi
	WAN_IF=nas$i
elif [ $ISP = "2" ] ; then
if [ "$CONNECTION" != "Connect_Manually" ] ; then
  #make sure the LCP echo will take no effect about ppp
#	PPP_PARAM="unit $i user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 25 usepeerdns lcp-echo-interval 10 lcp-echo-failure 6"
	PPP_PARAM="nodetach holdoff 4 maxfail 25 usepeerdns lcp-echo-interval 10 lcp-echo-failure 6"

    if [ "$ENCAP" = "PPPoE LLC" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoe nas$i" 
    elif [ "$ENCAP" = "PPPoE VC-Mux" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoe nas$i"
    elif [ "$ENCAP" = "PPPoA LLC" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoa llc-encaps $VPI.$VCI"
    elif [ "$ENCAP" = "PPPoA VC-Mux" ] ; then
	PPP_PARAM="$PPP_PARAM plugin pppoa vc-encaps $VPI.$VCI"
    fi

    # PPPoE
    if [ "$ENCAP" = "PPPoE LLC" ] || [ "$ENCAP" = "PPPoE VC-Mux" ] ; then
	#add by xyzhu_nj_20091221 for support service name & pap/chap
	if [ "$SRVNAME" != "" ] ; then
		PPP_PARAM="$PPP_PARAM pppoe_srv_name $SRVNAME"
	fi
	if [ "$AUTHEN" = "CHAP" ] ; then
		PPP_PARAM="$PPP_PARAM -pap"
	elif [ "$AUTHEN" = "PAP" ] ; then
		PPP_PARAM="$PPP_PARAM -chap"
	fi

	if [ $QOS = "ubr" ] ; then
		PCR_V="-p $PCR"
	elif [ $QOS = "cbr" ] || [ $QOS = "rt-vbr" ] || [ $QOS = "nrt-vbr" ] ; then
		PCR_V="-p $PCR -q $SCR -m $MBS"
	fi
    	if [ "$ENCAP" = "PPPoE LLC" ] ; then
		ENCAP_T="-e 0"
    	elif [ "$ENCAP" = "PPPoE VC-Mux" ] ; then
		ENCAP_T="-e 1"
	fi
	br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
	echo $! > /var/run/nas$i.pid
	sleep 1
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
	fi
	/sbin/ifconfig nas$i 0.0.0.0
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

	if [ "$DEFAULTROUTE" = "Yes" ] ; then
		PPP_PARAM="$PPP_PARAM defaultroute"
	fi
	if [ "$PPPGETIP" = "Dynamic" ] ; then
		PPP_PARAM="$PPP_PARAM noipdefault"
	else
		PPP_PARAM="$PPP_PARAM $IPADDR:$GATEWAY"
	fi
	if [ "$NETMASK" != "" ] ; then
		PPP_PARAM="$PPP_PARAM netmask $NETMASK"
	fi
	if [ "$CONNECTION" = "Connect_on_Demand" ] ; then
		# transform minute to second. shnwind 2008.4.17
		IDLETIME=$(expr $CLOSEIFIDLE \* 60)
		PPP_PARAM="$PPP_PARAM demand idle $IDLETIME"
	else
		PPP_PARAM="$PPP_PARAM persist"
	fi
	
	if [ "$MTU" != "0" ] && [ "$MTU" != "" ] ; then
		PPP_PARAM="$PPP_PARAM mtu $MTU mru $MTU"
	fi

	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
		PPP_PARAM="$PPP_PARAM ipv6 , "
	fi

#	pppd $PPP_PARAM &
#	echo $! > /var/run/pppd$i.pid
#	echo "pppd $PPP_PARAM &" > /var/tmp/ppp$i.conf
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
		iptables -D FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
	fi
	
	if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
		iptables -A FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		iptables -A FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
	fi
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
		iptables -t nat -A POSTROUTING -o ppp$i -j MASQUERADE
		iptables -t nat -A PREROUTING -j VS_PRE$i
		iptables -t nat -A PREROUTING -j DMZ_PRE$i
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	fi
	WAN_IF=ppp$i
# feature added by rclv 20100536 for ipv6 options 
	rm -f /etc/dhcp6c_$WAN_IF.conf
if [ "$IPVERSION" = "IPv4/IPv6" ] ; then	
	if [ "$PPPv6Mode" = "1" ] ; then
		if [ "$PPPv6PD" = "Yes" ] ; then
			echo -e "interface $WAN_IF { \\n\\tsend ia-pd 213;\\n\\tsend ia-na 210;\\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\n id-assoc pd 213 {\\n\\tprefix-interface br0\\n\\t{\\n\\t\\tsla-len 0;\\n\\t};\\n};\\nid-assoc na 210 { };\\n ">/etc/dhcp6c_$WAN_IF.conf 
		else
			echo -e "interface $WAN_IF { \\n\\tsend ia-na 210;\\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\nid-assoc na 210 { };\\n ">/etc/dhcp6c_$WAN_IF.conf
		fi

	else
		if [ "$PPPv6PD" = "Yes" ] ; then
			echo -e "interface $WAN_IF { \\n\\tsend ia-pd 213;\\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\n id-assoc pd 213 {\\n\\tprefix-interface br0\\n\\t{\\n\\t\\tsla-len 0;\\n\\t};\\n};\\n">/etc/dhcp6c_$WAN_IF.conf
		else
			echo -e "interface $WAN_IF { \\n\\trequest domain-name;\\n\\trequest domain-name-servers; \\n\\tscript \"/usr/script/dhcp6c_script\";\\n};\\n">/etc/dhcp6c_$WAN_IF.conf
		fi

	fi
fi
		
fi
#end of code added by rclv

elif [ $ISP = "3" ] ; then
	if [ $QOS = "ubr" ] ; then
		PCR_V="-p $PCR"
	elif [ $QOS = "cbr" ] || [ $QOS = "rt-vbr" ] || [ $QOS = "nrt-vbr" ] ; then
		PCR_V="-p $PCR -q $SCR -m $MBS"
	fi
	
	if [ "$ENCAP" = "1483 Bridged IP LLC" ] ; then
		ENCAP_T="-e 0"
	elif [ "$ENCAP" = "1483 Bridged IP VC-Mux" ] ; then
		ENCAP_T="-e 1"
	fi
	
	br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
	echo $! > /var/run/nas$i.pid
	sleep 1
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
	fi
	brctl addif br0 nas$i
	/sbin/ifconfig nas$i 0.0.0.0
	WAN_IF=nas$i
fi
#jrchen
#	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
#		sleep 1
#		/sbin/ifconfig $WAN_IF inet6 $IPADDR6/$PREFIX6
#		sleep 1
#		route -A inet6 add default gw $DEFGATEWAY6 dev $WAN_IF &
#	fi

/usr/script/ether_mac.sh &

if [ $ISP != "3" ] ; then
	if [ "$CONNECTION" != "Connect_Manually" ] ; then
		if [ $BridgeInterface = "Yes" ] ; then
			/userfs/bin/pppoe-relay -C br0 -S nas$i &
		fi
	fi
fi
/usr/bin/qoscmd dev add nas$i &
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