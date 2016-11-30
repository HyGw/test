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

if [ $ISP != "2" ] ; then
	exit 0
fi
 
if [ "$CONNECTION" != "Connect_Manually" ] ; then
	exit 0
fi
  #make sure the LCP echo will take no effect about ppp
	PPP_PARAM="unit $i user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 25 usepeerdns lcp-echo-interval 10 lcp-echo-failure 6"

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
	if [ "$MTU" != "0" ] && [ "$MTU" != "" ] ; then
		PPP_PARAM="$PPP_PARAM mtu $MTU mru $MTU"
	fi
	
	PPP_PARAM="$PPP_PARAM persist"

	pppd $PPP_PARAM &
	echo $! > /var/run/pppd$i.pid
	echo "pppd $PPP_PARAM &" > /var/tmp/ppp$i.conf
	
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

/usr/script/ether_mac.sh
if [ $BridgeInterface = "Yes" ] ; then
	/userfs/bin/pppoe-relay -C br0 -S nas$i &
fi
