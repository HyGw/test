#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head>"

`/userfs/bin/tcapi set System_Entry wd_enable 0`
`/userfs/bin/tcapi set System_Entry wd_Stat 1`
`iptables -t nat -F PRE_PHONEAPP`
`iptables -t nat -Z PRE_PHONEAPP`
ret=`iptables -t nat -A PRE_PHONEAPP -j RETURN`

echo "$ret<br>"

echo "</html>"

