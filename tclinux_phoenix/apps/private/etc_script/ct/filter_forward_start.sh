iptables -t filter -N macfilter_chain
iptables -t filter -N ipupfilter_chain
iptables -t filter -N ipdownfilter_chain
iptables -t filter -N app_filter_chain
iptables -t filter -N url_filter_chain
iptables -t filter -A FORWARD -j macfilter_chain
iptables -t filter -A FORWARD -j ipupfilter_chain
iptables -t filter -A FORWARD -j ipdownfilter_chain
iptables -t filter -A FORWARD -p TCP -m multiport --dport http -j url_filter_chain
iptables -t filter -A FORWARD -p TCP -j app_filter_chain
iptables -t filter -A FORWARD -p UDP -j app_filter_chain
#krammer
iptables -t filter -N SPI_FW
iptables -t filter -N ACL
iptables -t filter -N FIREWALL
iptables -t filter -A INPUT -j SPI_FW
iptables -t filter -A INPUT -j ACL
iptables -t filter -A INPUT -j FIREWALL
#xyzhu
iptables -t nat -N PRE_SERVICE
iptables -t nat -A PREROUTING -j PRE_SERVICE

