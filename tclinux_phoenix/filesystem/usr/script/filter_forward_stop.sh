iptables -t filter -X macfilter_chain
iptables -t filter -X ipupfilter_chain
iptables -t filter -X ipdownfilter_chain
iptables -t filter -X app_filter_chain
iptables -t filter -X url_filter_chain
#krammer
iptables -t filter -X SPI_FW
iptables -t filter -X ACL
iptables -t filter -X FIREWALL
#xyzhu
iptables -t nat -F PRE_SERVICE
iptables -t nat -D PREROUTING -j PRE_SERVICE
iptables -t nat -X PRE_SERVICE
