#!/bin/sh

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    chmod 777 $PROFILE_CFG
    . $PROFILE_CFG
fi

iptables -t mangle -N routep_chain

if [ "$TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND" != "" ] ; then
	echo "TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND_00"
iptables -t mangle -A PREROUTING -j routep_chain
fi

ebtables -N PPPoE_filter
ebtables -N arp_filter
ebtables -N ip_filter
ebtables -N ipv6_filter
ebtables -A FORWARD -p 8863 -j  PPPoE_filter
ebtables -A FORWARD -p 8864 -j  PPPoE_filter
ebtables -A FORWARD -p 0806 -j  arp_filter
ebtables -A FORWARD -p 0800 -j  ip_filter
ebtables -A FORWARD -p 86DD -j  ipv6_filter

ebtables -N PPPoE_chain
ebtables -N arp_chain
ebtables -N ip_chain
ebtables -N ipv6_chain

if [ "$TCSUPPORT_ROUTEPOLICY_PRIOR_PORTBIND" != "" ] ; then
	usr/bin/ebtables -t nat -N PPPoE_nat
	usr/bin/ebtables -t nat -N arp_nat
	ebtables -t nat -N ip_nat
	ebtables -t nat -N ipv6_nat
	
	ebtables -t nat -A PREROUTING -p 8863 -j PPPoE_nat
	ebtables -t nat -A PREROUTING -p 8864 -j PPPoE_nat
	ebtables -t nat -A PREROUTING -p 0806 -j arp_nat
	ebtables -t nat -A PREROUTING -p 0800 -j ip_nat
	ebtables -t nat -A PREROUTING -p 86DD -j ipv6_nat
	
	ebtables -t nat -N PPPoE_nat_chain
	ebtables -t nat -N arp_nat_chain
	ebtables -t nat -N ip_nat_chain
	ebtables -t nat -N ipv6_nat_chain
fi