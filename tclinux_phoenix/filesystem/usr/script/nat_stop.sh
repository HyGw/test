#!/bin/sh

if [ $# != 1 ] ; then
	echo "usage: $0 [interface]"
	exit 0
fi

#i=$1
WAN_IF=$1
DMZ_CHAIN=DMZ_$WAN_IF
VS_CHAIN=VS_$WAN_IF

#iptables -t nat -D POSTROUTING -o nas$i -j MASQUERADE
#iptables -t nat -D POSTROUTING -o ppp$i -j MASQUERADE
#iptables -t nat -D POSTROUTING -j ADDRMAP_POS$i
#iptables -t nat -D PREROUTING -j VS_PRE$i
#iptables -t nat -D PREROUTING -j DMZ_PRE$i
#iptables -t nat -D PREROUTING -j ADDRMAP_PRE$i
# flush nat rule
iptables -t nat -F $VS_CHAIN
iptables -t nat -F $DMZ_CHAIN
#iptables -t nat -F ADDRMAP_PRE$i
#iptables -t nat -F ADDRMAP_POS$i

