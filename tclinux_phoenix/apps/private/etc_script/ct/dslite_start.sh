#!/bin/sh
#To build dslite tunnel
#Usage:./dslite_start.sh local_wan_ipv6_address tunnel_ipv6_address wan_if_name

if [ $# != 3 ] ; then
	echo "usage: $0 local_wan_ipv6_address remote_tunnel_ipv6_address wan_if_name"
	exit 0
fi
LOCAL_ADDR=$1
REMOTE_ADDR=$2
WAN_IF=$3
DSLITE_NAME=ds.$WAN_IF

/usr/bin/ip -6 tunnel del $DSLITE_NAME

/usr/bin/ip -6 tunnel add $DSLITE_NAME mode ipip6 local $LOCAL_ADDR remote $REMOTE_ADDR encaplimit none
/usr/bin/ip link set up $DSLITE_NAME

echo 1 > /var/run/$WAN_IF/dslite_on
/usr/bin/iptables -D FORWARD -o $DSLITE_NAME -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu

/usr/bin/iptables -A FORWARD -o $DSLITE_NAME -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
