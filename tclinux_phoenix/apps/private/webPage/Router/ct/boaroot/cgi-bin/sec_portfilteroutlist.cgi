#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"><link rel=\"stylesheet\"
href=\"/JS/stylemain.css\" type=\"text/css\"></head><SCRIPT language=javascript src="/JS/util.js"></SCRIPT><script language=\"JavaScript\">
function formatstr(sourcestr, searchstr)
{
	var restr;
	var offset = sourcestr.indexOf(searchstr);
	if(offset == -1)
		return null;
	var lenstr = searchstr.toString();	
	restr = sourcestr.substring(0, offset) + sourcestr.substring((offset+parseInt(lenstr.length)+1), sourcestr.length);
	return restr;
}
function doDel(i)
{
	var f = parent.document.ConfigForm;
	var tempstr;
	if(this.rmlout.length>0)
	{
		for(var n=0;n<(this.rmlout.length);n++)
		{
			if(this.rmlout[n].value == i){
				if(this.rmlout[n].checked)
				{
					tempstr = i + \",\";
					f.delnum.value = f.delnum.value + tempstr;
					
				}
				else
				{
					f.delnum.value = formatstr(f.delnum.value,i);
				}
				break;
			}
		}
	}
	else
	{
		if(this.rmlout.checked) f.delnum.value = i + \",\";
		else f.delnum.value = \"\";
	}
}
</script><body topmargin=\"10\" leftmargin=\"0\"><table width=\"450\" align=center cellpadding=\"0\" cellspacing=\"0\">"

echo "<tr><td><table width=\"450\" border=\"1\" align=center cellpadding=\"3\" cellspacing=\"0\" bordercolor=\"#CCCCCC\"
bgcolor=\"#FFFFFF\"><tr><TD class=table_title align=middle width=\"18%\"><STRONG>过滤名称</STRONG></TD><td width=\"10%\" align=center
class=\"table_title\"><strong>协议</strong></td><TD class=table_title align=middle
width=\"14%\"><STRONG>源地址</STRONG></TD><td width=\"12%\" align=center
class=\"table_title\"><strong>源端口</strong></td><td width=\"12%\" align=center
class=\"table_title\"><strong>目的地址</strong></td><td width=\"12%\" align=center
class=\"table_title\"><strong>目的端口</strong></td><td width=\"12%\" align=center
class=\"table_title\"><strong>使能</strong></td><td width=\"10%\" align=center
class=\"table_title\"><strong>删除</strong></td></TR></tr>"

i=0
Activate="N/A"
Interface="N/A"
IPName="N/A"
Protocol="N/A"
SrcIPAddr="N/A"
SrcIPMask="N/A"
SrcPort="N/A"
DesIPAddr="N/A"
DesIPMask="N/A"
DesPort="N/A"
cat /tmp/ipupvalid | while read LINE
do
	k=`expr $i % 11`
	if [ $k = 0 ]
	then
		iEntry=$LINE
	elif [ $k = 1 ]
	then
		Activate=$LINE
		if [ "$Activate" = "Yes" ] 
  		then
  			Activate="启用"
		elif [ "$Activate" = "No" ] 
		then
			Activate="禁用"
		else  Activate="Wrong setting"
		fi
	elif [ $k = 3 ]
	then
		IPName=$LINE
	elif [ $k = 4 ]
	then
		Protocol=$LINE
	elif [ $k = 5 ]
	then
		SrcIPAddr=$LINE
	elif [ $k = 7 ]
	then
		SrcPort=$LINE
	elif [ $k = 8 ]
	then
		DesIPAddr=$LINE
	elif [ $k = 10 ]
	then
		DesPort=$LINE
		echo "<tr align=\"middle\"><td width=\"18%\">$IPName</td><td width=\"10%\">$Protocol</td><td width=\"14%\">$SrcIPAddr</td><td
		width=\"12%\">$SrcPort</td><td width=\"12%\">$DesIPAddr</td><td width=\"12%\">$DesPort</td><td
		width=\"12%\">$Activate</td><td width=\"10%\"><input type=\"checkbox\" name=\"rmlout\" onclick=\"doDel($iEntry);\" value=\"$iEntry\"></td></tr>"
	fi
	i=`expr $i + 1`

done
rm -f /tmp/ipupvalid
echo "</table></td></tr></table></body></html>"

