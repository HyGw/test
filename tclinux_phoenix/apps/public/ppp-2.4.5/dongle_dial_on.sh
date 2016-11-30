#! /bin/sh

# Script to initiate a ppp connection.

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi

CONFFILE=/etc/isp11.conf
echo $CONFFILE
if [ -f $CONFFILE ] ;then
	chmod 777 $CONFFILE
	. $CONFFILE
else
	exit 0
fi
if [ "$Active" != "Yes" ] ; then
	exit 0
fi

# avoid to dail up 3g dongle again.
# if there is a file named "pppd11_3gdongle_dialup", exit the shell.
if [ -f /var/run/pppd11_3gdongle_dialup ] ;then
	exit 0
fi
echo 1 > /var/run/pppd11_3gdongle_dialup

#parameter read from conf, which get from cfgmanager
#TELEPHONE="*99#"
#USERNAME=
#PASSWORD=
#LOCAL_IP=0.0.0.0
#REMOTE_IP=0.0.0.0
#NETMASK=255.255.255.0
USBDEV=/dev/$USBDEV #according to 3g card
BOND=921600
#NATENABLE=Enable
#APN="3gnet"
echo "APN=$APN, TELEPHONE=$TELEPHONE, USERNAME=$USERNAME, PASSWORD=$PASSWORD, PIN=$PIN" > /var/tmp/3g_info
export APN TELEPHONE USERNAME PASSWORD PIN

#DIALER_SCRIPT=dlink_isp.sh #Need to change according to isp
DIALER_SCRIPT=ppp_on_dialer.sh
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CD_WIND)
if [ "$TCSUPPORT_CD_WIND" != "" ]; then
	DIALER_SCRIPT=ppp_connect
	fail_count=0
	PPPF_CONFFILE=/var/run/ppp_failcount.conf
	if [ -f $PPPF_CONFFILE ]; then
		chmod 777 $PPPF_CONFFILE
		. $PPPF_CONFFILE
	fi
fi
#endif/*TCSUPPORT_COMPILE*/

if [ "$NATENABLE" = "Enable" ] ;then
	#iptables -t raw -F		#need to do this action in cfgmanager check_nat_enable() later. 
	iptables -t nat -A POSTROUTING -o ppp11 -j MASQUERADE
	#will to do
		#iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
		#iptables -t nat -A POSTROUTING -o ppp11 -j MASQUERADE
		#iptables -t nat -A PREROUTING -j VS_PRE$i
		#iptables -t nat -A PREROUTING -j DMZ_PRE$i
		#iptables -t nat -A PREROUTING -i ppp$i -j PREROUTING_WAN
		#iptables -t filter -A FORWARD -i ppp$i -j FORWARD_WAN
		#iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
fi

#PPP_PARAM need to update according to conf
PPP_PARAM="$USBDEV $BOND unit 11 noipdefault noauth user $USERNAME password $PASSWORD nodetach usepeerdns"
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CD_WIND)
if [ "$TCSUPPORT_CD_WIND" != "" ]; then
	PPP_PARAM="$PPP_PARAM"
	PPP_PARAM="$USBDEV $BOND unit 11 noipdefault noauth user $USERNAME password $PASSWORD nodetach maxfail $fail_count usepeerdns"
fi
#endif/*TCSUPPORT_COMPILE*/

if [ "$DEFAULTROUTE" = "Yes" ] ; then
	PPP_PARAM="$PPP_PARAM defaultroute"
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
else
	PPP_PARAM="$PPP_PARAM mtu 1492 mru 1492"
fi

if [ "$AUTHEN" = "CHAP" ] ; then
	PPP_PARAM="$PPP_PARAM -pap"
elif [ "$AUTHEN" = "PAP" ] ; then
	PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
fi
	
sleep 3
#pppd_3g $PPP_PARAM \
cd && ./usr/bin/pppd $PPP_PARAM \
	connect "/usr/etc/ppp/peers/$DIALER_SCRIPT" &
echo $! > /var/run/pppd11.pid
echo "pppd $PPP_PARAM" > /var/tmp/ppp11.conf
