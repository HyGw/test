#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head><body><table cellspacing=0 cellPadding=0  border=0><tr>"

reqMethod=`/userfs/bin/tcapi get System_Entry reqMethod`

echo "<td style='FONT-SIZE: 25px;color:red;padding-top:23px;padding-left:200px;'>"

if [ "$reqMethod" = "Post" ] 
then
ret=`/userfs/bin/tcapi set System_Entry reqMethod Get`
echo "�û�����д�뿨���ɹ���������Ч!"
else	echo "�����Ϸ��ļ�ͥ���ؿ���������ͥ����!"
fi
	
echo "</td></tr></table></body></html>"