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
	var rml = document.getElementsByName('rml');
	if (rml == null)
		return;
	if(rml.length>0)
	{
		for(var n=0;n<(rml.length);n++)
		{
			if(rml[n].value == i){
				if(rml[n].checked)
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
}
function stMacFilter(domain,Name,MACAddress,Enable)
{
this.domain = domain;
this.Name = Name;
this.MACAddress = MACAddress;
this.Enable = Enable;
}
</script>
<body topmargin=\"10\" leftmargin=\"0\"><table width=\"100%\" align=center cellpadding=\"0\" cellspacing=\"0\">"

echo "<tr><td><table width=\"100%\" border=\"1\" align=center cellpadding=\"3\" cellspacing=\"0\" bordercolor=\"#CCCCCC\"
bgcolor=\"#FFFFFF\"><tr><TD class=table_title align=middle width=\"35%\"><STRONG>过滤规则名称</STRONG></TD><td width=\"30%\" align=center
class=\"table_title\"><strong><FONT color=\"#000000\">MAC地址</FONT></strong></td><TD class=table_title align=middle
width=\"20%\"><STRONG><FONT color=#000000>使能</FONT></STRONG></TD><td width=\"15%\" align=center
class=\"table_title\"><strong><FONT color=\"#000000\">删除</FONT></strong></td></TR></tr>"

i=0
iEntry=0
MacName="N/A"
MacAddr="N/A"
Activate="N/A"
Interface="N/A"
cat /tmp/macvalid | while read LINE
do
	k=`expr $i % 5`
	if [ $k = 0 ]
	then
		iEntry=$LINE
	elif [ $k = 1 ]
	then
		MacName=$LINE
	elif [ $k = 2 ]
	then
		MacAddr=$LINE
	elif [ $k = 3 ]
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
	elif [ $k = 4 ]
	then
		Interface=$LINE
		echo "<script language=\"javascript\">
		if(parent.MacFilterRule != null)
		{
			parent.MacFilterRule[parent.MacFilterRuleIndex] = new stMacFilter(\"domain\", \"$MacName\", \"$MacAddr\", \"$Activate\");
			parent.MacFilterRuleIndex++;
		}
		</script>
		<tr align=\"middle\"><td width=\"35%\">$MacName</td><td width=\"30%\">$MacAddr</td><td width=\"20%\">$Activate</td><td
		width=\"15%\"><input type=\"checkbox\" id=\"rml\" name=\"rml\" onclick=\"doDel($iEntry);\" value=\"$iEntry\"></td></tr>"
	fi
	i=`expr $i + 1`
done
rm -f /tmp/macvalid
echo "</table></td></tr></table></body></html>"

