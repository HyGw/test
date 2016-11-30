#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head>"

node_name="SysInfo_Entry"
attr_name="GateWay"
gateway=`/userfs/bin/tcapi get $node_name $attr_name`
node_name="Account_TelnetEntry"
attr_name="telnet_username"
UID=`/userfs/bin/tcapi get $node_name $attr_name`
attr_name="telnet_passwd"
PSW=`/userfs/bin/tcapi get $node_name $attr_name`

node_name="Account_TelnetEntry"
attr_name="Active"
TelnetActive=`/userfs/bin/tcapi get $node_name $attr_name`

if [ "$TelnetActive" = "No" ] 
then
ret=`/userfs/bin/tcapi set Account_TelnetEntry Active Yes`
ret=`/userfs/bin/tcapi commit Account_TelnetEntry`	
fi

echo "$gateway<br>"
echo "$UID<br>"
echo "$PSW<br>"
echo "$TelnetActive"


echo "</html>"

