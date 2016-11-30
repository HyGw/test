#!/bin/sh

echo -e "Content-Type: text/html\n"
/userfs/bin/tcapi set Cwmp_Entry Maintain_Inform 1
/userfs/bin/tcapi commit Cwmp_Entry


echo "<html><head><meta http-equiv=Content-Script-Type
content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html;
charset=gb2312\"></head>
<body onload=\"javascript:window.location='/cgi-bin/mag-syslogmanage.asp'\"><table width=\"580\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">"


echo "</table></body></html>"

