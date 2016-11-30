#! /bin/sh

#terminated the device
#usage: ./ppp_dial_off.sh
#		or ./ppp_dial_off.sh device
#auther gcyin

if [ "$1" = "" ] ;then
DEVICE=ppp11	
else
	echo "usage: $0"
	exit 0
fi
iptables -t nat -D POSTROUTING -o ppp11 -j MASQUERADE
#need to deal with DMZ VSERVER later

#if ppp$1 pid file is present, then the program is running. Stop it.
if [ -f /var/run/ppp11.pid ] ;then
	kill -15 `cat /var/run/ppp11.pid`
	sleep 1
	if [ -f /var/run/ppp11.pid ] ;then
		kill -9 `cat /var/run/ppp11.pid`
	fi
else
	if [ -f /var/run/pppd11.pid ] ;then
		kill -9 `cat /var/run/pppd11.pid`
	fi
fi
#success.
rm -rf /var/run/pppd11.pid
rm -rf /var/run/pppd11_3gdongle_dialup
rm -rf /etc/ppp/resolv_ppp11.conf

if [ -f /var/tmp/ppp11.conf ] ;then
	rm -rf /var/run/ppp11.conf
fi
exit 0
