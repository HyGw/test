#!/bin/sh
#To delete dslite tunnel
#Usage:./dslite_stop.sh wan_if_name

if [ $# != 1 ] ; then
	echo "usage: $0 wan_if_name"
	exit 0
fi
WAN_IF=$1
DSLITE_NAME=ds.$WAN_IF

/usr/bin/iptables -D FORWARD -o $DSLITE_NAME -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu

/usr/bin/ip -6 tunnel del $DSLITE_NAME
rm -f /var/run/$WAN_IF/dslite_on