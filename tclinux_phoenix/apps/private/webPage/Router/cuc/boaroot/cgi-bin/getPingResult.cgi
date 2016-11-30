#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><body><table cellspacing=0 cellPadding=0  border=0><tr><td>"

echo "<font size='-1'>Ping Result:</font>"
echo "<BR>"

cat /tmp/pingOth | while read LINE
do
	echo "<font size='-1'>$LINE</font>"
	echo "<BR>"
done


echo "</td></tr></table></body></html>"
 
