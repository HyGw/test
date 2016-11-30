#!/bin/sh

echo "Content-Type: text/html"
echo ""

FILE="/tmp/cwmpfirmwareupgrade"

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head><body><table cellspacing=0 cellPadding=0  border=0><tr>"


echo "<td style='FONT-SIZE: 20px;color:red;padding-top:23px;'>"
if [ -f "$FILE" ];
then
	echo "设备正在进行版本升级、请勿下电"
else
  echo "设备版本已升级完成，可正常使用业务"
fi
echo "</td></tr></table></body></html>"