#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"><link rel=\"stylesheet\"
href=\"/JS/stylemain.css\" type=\"text/css\"></head>
<script language=\"JavaScript\">
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
	var f = parent.document.UrlFilterForm;
	var tempstr;
	if(this.rml.length>0)
	{
		for(var n=0;n<(this.rml.length);n++)
		{
			if(this.rml[n].value == i){
				if(this.rml[n].checked)
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
		if(this.rml.checked) f.delnum.value = i + \",\";
		else f.delnum.value = \"\";
	}
}</script>
<body topmargin=\"10\" leftmargin=\"0\"><table width=\"320\" align=center cellpadding=\"0\" cellspacing=\"0\">"

echo "<tr><td><table width=\"320\" border=\"1\" align=center cellpadding=\"3\" cellspacing=\"0\" bordercolor=\"#CCCCCC\"
bgcolor=\"#FFFFFF\"><tr><TD class=table_title align=middle width=60><STRONG>序 号</STRONG></TD><td width=180 align=center
class=\"table_title\"><strong><FONT color=\"#000000\">URL</FONT></strong></td><TD class=table_title align=middle width=60><STRONG><FONT color=#000000>删 除</FONT></STRONG></TD></TR></tr>"

i=0
j=0
n=1
cat /tmp/urladdr | while read LINE
do
	k=`expr $i % 2`
	if [ $k = 0 ]
	then
		j=$LINE
	elif [ $k = 1 ]
	then
		echo "<tr><td align=middle width=60>$n</td><td width=180>$LINE</td><td align=middle width=60><input type=\"checkbox\"
		name=\"rml\" id=\"rml\" onclick=\"doDel($j);\" value=\"$j\"></td></tr>"
		n=`expr $n + 1`
	fi
	i=`expr $i + 1`
done
rm -f /tmp/urladdr
echo "</table></td></tr></table></body></html>"

