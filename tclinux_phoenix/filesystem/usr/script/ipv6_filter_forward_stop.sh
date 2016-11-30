PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    chmod 777 $PROFILE_CFG
    . $PROFILE_CFG
fi

if [ "$TCSUPPORT_IPV6_ACL" != "" ] ;then
ip6tables -t filter -D INPUT -j INPUT_ACL
ip6tables -t filter -F INPUT_ACL
ip6tables -t filter -Z INPUT_ACL
ip6tables -t filter -X INPUT_ACL
fi

if [ "$TCSUPPORT_IPV6_FIREWALL" != "" ] ;then
ip6tables -t filter -D FORWARD -j INPUT_SPI
ip6tables -t filter -D INPUT -j INPUT_SPI
ip6tables -t filter -F INPUT_SPI
ip6tables -t filter -Z INPUT_SPI
ip6tables -t filter -X INPUT_SPI
ip6tables -t filter -D INPUT -j INPUT_FW
ip6tables -t filter -F INPUT_FW
ip6tables -t filter -Z INPUT_FW
ip6tables -t filter -X INPUT_FW
fi
