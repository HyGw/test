#!/bin/sh

PPP_CONFFILE=/var/run/pppoe_sim.conf
if [ -f $PPP_CONFFILE ] ; then
	. $PPP_CONFFILE
else
	echo "not found pppoe_sim" > /var/tmp/pppoe_sim_err.conf
	exit 0
fi

SYS_CONFFILE=/var/run/system.conf
if [ -f $SYS_CONFFILE ] ; then
	chmod 777 $SYS_CONFFILE
	. $SYS_CONFFILE
else
	echo "not found system.conf" > /var/tmp/pppoe_sim_err.conf
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

/usr/bin/smuxctl add pppoe $NAS_IF $NASName $VTAG_MODE $VTAG_VID $VTAG_PBIT 0
/sbin/ifconfig $NASName up

PPP_PARAM="nodetach holdoff 4 maxfail $RetryTimes usepeerdns lcp-echo-interval 1 lcp-echo-failure 60 plugin pppoe $NASName noipdefault persist mtu 1492 mru 1492"

if [ "$PPPAuthenticationProtocol" = "CHAP" ] ; then
	 PPP_PARAM="$PPP_PARAM -pap"
elif [ "$PPPAuthenticationProtocol" = "PAP" ] ; then
	 PPP_PARAM="$PPP_PARAM -chap"
fi

if [ "$IPProtocolVersion" = "3" ] ; then
	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
		PPP_PARAM="$PPP_PARAM ipv6 , "
	elif [ "$IPVERSION" = "IPv6" ] ; then
		PPP_PARAM="$PPP_PARAM ipv6 , noip"
	fi
elif [ "$IPProtocolVersion" = "2" ] ; then
	if [ "$IPVERSION" = "IPv4/IPv6" ] || [ "$IPVERSION" = "IPv6" ] ; then
		PPP_PARAM="$PPP_PARAM ipv6 , noip"
	elif [ "$IPVERSION" = "IPv4" ] ; then
		PPP_PARAM="$PPP_PARAM noip"		
	fi
elif [ "$IPProtocolVersion" = "1" ] ; then
	if [ "$IPVERSION" = "IPv6" ] ; then
		PPP_PARAM="$PPP_PARAM noip"
	fi
fi

if [ "$Username" = "" ]; then
	if [ "$Password" = "" ]; then
		pppd unit $PPPUnit user "" password "" $PPP_PARAM &	
		echo $! > /var/run/pppoe_sim.pid
		echo "pppd unit $PPPUnit user \"\" password \"\" $PPP_PARAM &" > /var/tmp/pppoe_sim_pppd.conf
	else
		pppd unit $PPPUnit user "" password $Password $PPP_PARAM &
		echo $! > /var/run/pppoe_sim.pid
		echo "pppd unit $PPPUnit user \"\" password $Password $PPP_PARAM &" > /var/tmp/pppoe_sim_pppd.conf
	fi
else
	if [ "$Password" = "" ]; then
		pppd unit $PPPUnit user $Username password "" $PPP_PARAM &	
		echo $! > /var/run/pppoe_sim.pid
		echo "pppd unit $PPPUnit user $Username password \"\" $PPP_PARAM &" > /var/tmp/pppoe_sim_pppd.conf
	else
		pppd unit $PPPUnit user $Username password $Password $PPP_PARAM &
		echo $! > /var/run/pppoe_sim.pid
		echo "pppd unit $PPPUnit user $Username password $Password $PPP_PARAM &" > /var/tmp/pppoe_sim_pppd.conf
	fi
fi
