#!/bin/sh

echo "Content-Type: text/html"
echo ""

FILE="/tmp/cwmpfirmwareupgrade"

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head><body><table cellspacing=0 cellPadding=0  border=0><tr>"


echo "<td style='FONT-SIZE: 20px;color:red;padding-top:23px;'>"
if [ -f "$FILE" ];
then
	echo "�豸���ڽ��а汾�����������µ�"
else
  echo "�豸�汾��������ɣ�������ʹ��ҵ��"
fi
echo "</td></tr></table></body></html>"