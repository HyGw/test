ip6tables -F ipv6_acl_chain
ip6tables -t filter -D INPUT_ACL -p TCP -m multiport --dport ftp,telnet,tftp,http -j ipv6_acl_chain
ip6tables -t filter -D INPUT_ACL -p UDP -m multiport --dport ftp,telnet,tftp,http -j ipv6_acl_chain
ip6tables -t filter -D INPUT_ACL -p ICMPV6 --icmpv6-type echo-request -j ipv6_acl_chain
ip6tables -X ipv6_acl_chain
