#!/bin/sh

# udhcpc script edited by Tim Riker <Tim@Rikers.org>

[ -z "$1" ] && echo "Error: should be called from udhcpc" && exit 1
PROFILE_CFG=/userfs/profile.cfg
#TYPE=`tcapi get Dproxy_Entry type`
#echo $TYPE
if [ -f $PROFILE_CFG ] ; then
	chmod 777 $PROFILE_CFG
	. $PROFILE_CFG
fi
#if [ "$TCSUPPORT_DNSEACHPVC" != "" ] ;then
	RESOLV_CONF="/etc/resolv_"$interface".conf"
#else
#	RESOLV_CONF="/etc/resolv.conf"
#fi
GATEWAY_FILE="/etc/"$interface"_gateway.conf"

[ -n "$broadcast" ] && BROADCAST="broadcast $broadcast"
[ -n "$subnet" ] && NETMASK="netmask $subnet"


case "$1" in
	deconfig)
		#/sbin/ifconfig $interface down
		/sbin/ifconfig $interface 0.0.0.0
		rm $GATEWAY_FILE
		;;

	renew|bound)
		/sbin/ifconfig $interface $ip $BROADCAST $NETMASK

		if [ -n "$router" ] ; then
			echo "deleting routers"
			while /sbin/route del default gw 0.0.0.0 dev $interface ; do
				:
			done

			echo "adding default routers"
			for i in $router ; do
				echo "route add default gw $i dev $interface"
				/sbin/route add default gw $i dev $interface
				echo "gateway=$i" > $GATEWAY_FILE
			done
		fi

		#if [ $TYPE != "1" ] ; then
			echo -n > $RESOLV_CONF
			[ -n "$domain" ] && echo search $domain >> $RESOLV_CONF
			for i in $dns ; do
				echo adding dns $i
				echo nameserver $i >> $RESOLV_CONF
			done
		#fi
		;;
esac

exit 0
