#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"><link rel=\"stylesheet\"
href=\"/JS/stylemain.css\" type=\"text/css\"></head>
</SCRIPT><script language=\"JavaScript\">
		function delCookie(name)
		{  
			var exp = new Date();  
			exp.setTime(exp.getTime() - 10000);  
			document.cookie = name + \"=del;expires=\" + exp.toGMTString();   
			document.cookie = name + \"=del;expires=\" + exp.toGMTString() + \";path=/;\";  
		}
		delCookie(\"UID\");
		delCookie(\"PSW\");
		delCookie(\"LoginTimes\");
		top.window.location.href=\"/cgi-bin/index.asp\";
</script>
<body onload=\"\"><table width=\"580\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">"

node_name="WebCurSet_Entry"
attr_name="CurrentAccess"
curUser=`/userfs/bin/tcapi get $node_name $attr_name`
node_name="Account_Entry$curUser"
attr_name="Logged"
ret=`/userfs/bin/tcapi set $node_name $attr_name 0`
ret=`/userfs/bin/tcapi set $node_name Logoff 1`

echo "</table></body></html>"

