PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    chmod 777 $PROFILE_CFG
    . $PROFILE_CFG
fi

if [ "$TCSUPPORT_IPV6_ACL" != "" ] ;then
ip6tables -t filter -N INPUT_ACL
ip6tables -t filter -A INPUT -j INPUT_ACL
fi

if [ "$TCSUPPORT_IPV6_FIREWALL" != "" ] ;then
ip6tables -t filter -N INPUT_SPI
ip6tables -t filter -N INPUT_FW
ip6tables -t filter -I INPUT -j INPUT_SPI
ip6tables -t filter -A INPUT -j INPUT_FW
ip6tables -t filter -I FORWARD -j INPUT_SPI
fi

if [ "$TCSUPPORT_CZ_OTE" != "" ] ;then
ip6tables -t filter -I INPUT -i ppp+ -p udp -m multiport --dport 53 -j DROP
ip6tables -t filter -I INPUT -i nas+ -p udp -m multiport --dport 53 -j DROP
ip6tables -t filter -I INPUT -i ppp+ -p tcp -m multiport --dport 53 -j DROP
ip6tables -t filter -I INPUT -i nas+ -p tcp -m multiport --dport 53 -j DROP
fi