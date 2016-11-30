#!/bin/sh

echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type
content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html;
charset=gb2312\"><LINK href=\"/JS/stylemain.css\" type=\"text/css\"
rel=\"stylesheet\"></head><script language=\"JavaScript\">function
onloadfuns{alert("OK");} </script>"

echo -e "<body onload=\"onloadfuns()\"><form id=\"regConfigForm\" name=\"regConfigForm\"><table width=\"580\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">"

state=`/userfs/bin/tcapi get Register_Entry RegProcessState`
times=`/userfs/bin/tcapi get Register_Entry RegProcessTimes`
percent=`/userfs/bin/tcapi get Register_Entry RegProcessPercent`

echo -e "<tr> <td>"

echo -e "<input type=\"hidden\" name=\"state\" value=\"$state\">"
echo -e "<input type=\"hidden\" name=\"times\" value=\"$times\">"
echo -e "<input type=\"hidden\" name=\"percent\" value=\"$percent\">"

echo -e "</td></tr>"
echo "</table></form></body></html>"

