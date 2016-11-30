#!/bin/sh

# udhcpc script edited by Tim Riker <Tim@Rikers.org>
[ -z "$1" ] && echo "Error: should be called from udhcpc" && exit 1

IP_CONFFILE=/var/run/ipoe_emu.conf
if [ -f $IP_CONFFILE ] ; then
	. $IP_CONFFILE
else
	echo "Error: dhcpc not found ipoe_emu.conf"
	exit 1
fi

STATUS_FILE="/var/run/"$interface"/DiagnosticsState"
RESULT_FILE="/var/run/"$interface"/Result"
IP_FILE="/var/run/"$interface"/LocalAddress"
GATEWAY_FILE="/var/run/"$interface"/DefaultGateway"
PING_RESULT="/var/run/"$interface"/PingResult"
PING_PID="/var/run/ipoe_ping.pid"
DIV=1000

[ -n "$broadcast" ] && BROADCAST="broadcast $broadcast"
[ -n "$subnet" ] && NETMASK="netmask $subnet"

case "$1" in
	deconfig)
		/sbin/ifconfig $interface down
		/sbin/ifconfig $interface 0.0.0.0
		;;
	noiface)
		echo "SendDHCPMsgError" > $RESULT_FILE
		echo "Complete" > $STATUS_FILE
		/userfs/bin/tcapi commit IpoeEmulator_Entry
		;;
	rebindfail|renewfail|requestfail|leasefail)
		echo "ServerNotFound" > $RESULT_FILE
		echo "Complete" > $STATUS_FILE
		/userfs/bin/tcapi commit IpoeEmulator_Entry
		;;
	nak)
		echo "ServerDeny" > $RESULT_FILE
		echo "Complete" > $STATUS_FILE
		/userfs/bin/tcapi commit IpoeEmulator_Entry
		;;
	renew)
		/sbin/ifconfig $interface $ip $BROADCAST $NETMASK		
		;;
	bound)
		/sbin/ifconfig $interface $ip $BROADCAST $NETMASK		
		echo $ip > $IP_FILE
		echo $subnet > $NETMASK_FILE

		if [ -n "$router" ] ; then
			for i in $router ; do
				echo $i > $GATEWAY_FILE
				GATEWAY=$i
			done
		fi

		/usr/bin/ip rule add from $ip/32 table 234
		/usr/bin/ip rule add fwmark 0x7e0000/0x7f0000 table 234
		/usr/bin/ip route add $dstnet/$mask dev $interface table 234
		/usr/bin/ip route add default via $GATEWAY dev $interface table 234
		/usr/bin/iptables -t mangle -A OUTPUT -o $interface -j MARK --set-mark 0x7e0000/0x7f0000
		/sbin/route del -net $dstnet netmask $subnet dev $interface
		/usr/bin/ip route flush cache 2>/dev/null
	
		echo "DHCPOK" > $RESULT_FILE
		echo "Complete" > $STATUS_FILE

		if [ -f $PING_PID ]; then	
			kill -9 `cat $PING_PID`
		fi
		if [ -n "$PingDestIPAddress" ] ; then
			/bin/ping -q -l -c $PingNumberOfRepetitions -W `expr $Timeout / $DIV` $PingDestIPAddress &
		else
			echo "0,0,0,0,0" > $PING_RESULT
			/userfs/bin/tcapi commit IpoeEmulator_Entry &
		fi
		;;
esac

exit 0
