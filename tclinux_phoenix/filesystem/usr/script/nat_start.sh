#!/bin/sh

if [ $# != 1 ] ; then
	echo "usage: $0 [interface]"
	exit 0
fi

#i=$1
WAN_IF=$1

#
# Rebuild NAT rule
#
#NAT Virtual Server
for j in 0 1 2 3 4 5 6 7 8 9
do
	#VS_CONF=/etc/nat_pvc$i/vserver$j
	VS_CONF=/var/run/$WAN_IF/vserver$j
	if [ -f $VS_CONF ] ; then
		/usr/script/vserver.sh add $WAN_IF $j
	fi
done

#NAT DMZ
DMZ_CONF=/var/run/$WAN_IF/dmz
if [ -f $DMZ_CONF ] ; then
	/usr/script/dmz.sh $WAN_IF
fi

#NAT IP address mapping
#for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
#do
#	IPADDRMAPPING_CONF=/etc/nat_pvc$i/ipaddr_mapping$j
#	if [ -f $IPADDRMAPPING_CONF ] ; then
#		/usr/script/ipaddr_mapping.sh add $i $j
#	fi
#done
