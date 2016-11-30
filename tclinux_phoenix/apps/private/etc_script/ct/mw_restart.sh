#!/bin/sh

if [ $# != 1 ] ; then
	echo "usage: $0 [0,2]"
	exit 0
fi

killall -9 tr69
killall -9 ctadmin
killall -9 ctadmin0
killall -9 dispatch_mtek
killall -9 rcm_mtek
killall -9 tr_mtek
killall -9 ftpclient

/userfs/bin/ctadmin s=$1
/userfs/bin/tr69 &

