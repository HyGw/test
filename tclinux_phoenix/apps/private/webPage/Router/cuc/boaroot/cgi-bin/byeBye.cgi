#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head>"

ret=`/userfs/bin/tcapi set Account_TelnetEntry Active No`
ret=`/userfs/bin/tcapi commit Account_TelnetEntry`

echo "Bye"

echo "</html>"

