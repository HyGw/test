#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"><link rel=\"stylesheet\"
href=\"/JS/stylemain.css\" type=\"text/css\"></head>
</SCRIPT><script language=\"JavaScript\">
function doDel(i)
{ 
	var f = parent.document.DdnsForm;
	var element = document.getElementById(i);

	if(element.checked)
	{
		if(i==0) f.hDdnsEntry0.value=i;
		else if(i==1) f.hDdnsEntry1.value=i;
		else if(i==2) f.hDdnsEntry2.value=i;
		else if(i==3) f.hDdnsEntry3.value=i;
		else if(i==4) f.hDdnsEntry4.value=i;
		else if(i==5) f.hDdnsEntry5.value=i;
		else if(i==6) f.hDdnsEntry6.value=i;
		else if(i==7) f.hDdnsEntry7.value=i;
		else if(i==8) f.hDdnsEntry8.value=i;
		else if(i==9) f.hDdnsEntry9.value=i;
		else if(i==10) f.hDdnsEntry10.value=i;
		else if(i==11) f.hDdnsEntry11.value=i;
		else if(i==12) f.hDdnsEntry12.value=i;
		else if(i==13) f.hDdnsEntry13.value=i;
		else if(i==14) f.hDdnsEntry14.value=i;
		else if(i==15) f.hDdnsEntry15.value=i;
		else if(i==16) f.hDdnsEntry16.value=i;
		else if(i==17) f.hDdnsEntry17.value=i;
		else if(i==18) f.hDdnsEntry18.value=i;
		else if(i==19) f.hDdnsEntry19.value=i;
		else if(i==20) f.hDdnsEntry20.value=i;
		else if(i==21) f.hDdnsEntry21.value=i;
		else if(i==22) f.hDdnsEntry22.value=i;
		else if(i==23) f.hDdnsEntry23.value=i;
		else if(i==24) f.hDdnsEntry24.value=i;
		else if(i==25) f.hDdnsEntry25.value=i;
		else if(i==26) f.hDdnsEntry26.value=i;
		else if(i==27) f.hDdnsEntry27.value=i;
		else if(i==28) f.hDdnsEntry28.value=i;
		else if(i==29) f.hDdnsEntry29.value=i;
		else if(i==30) f.hDdnsEntry30.value=i;
		else if(i==31) f.hDdnsEntry31.value=i;
		else if(i==32) f.hDdnsEntry32.value=i;
		else if(i==33) f.hDdnsEntry33.value=i;
		else if(i==34) f.hDdnsEntry34.value=i;
		else if(i==35) f.hDdnsEntry35.value=i;
		else if(i==36) f.hDdnsEntry36.value=i;
		else if(i==37) f.hDdnsEntry37.value=i;
		else if(i==38) f.hDdnsEntry38.value=i;
		else if(i==39) f.hDdnsEntry39.value=i;
		else if(i==40) f.hDdnsEntry40.value=i;
		else if(i==41) f.hDdnsEntry41.value=i;
		else if(i==42) f.hDdnsEntry42.value=i;
		else if(i==43) f.hDdnsEntry43.value=i;
		else if(i==44) f.hDdnsEntry44.value=i;
		else if(i==45) f.hDdnsEntry45.value=i;
		else if(i==46) f.hDdnsEntry46.value=i;
		else if(i==47) f.hDdnsEntry47.value=i;
		else if(i==48) f.hDdnsEntry48.value=i;
		else if(i==49) f.hDdnsEntry49.value=i;
		else if(i==50) f.hDdnsEntry50.value=i;
		else if(i==51) f.hDdnsEntry51.value=i;
		else if(i==52) f.hDdnsEntry52.value=i;
		else if(i==53) f.hDdnsEntry53.value=i;
		else if(i==54) f.hDdnsEntry54.value=i;
		else if(i==55) f.hDdnsEntry55.value=i;
		else if(i==56) f.hDdnsEntry56.value=i;
		else if(i==57) f.hDdnsEntry57.value=i;
		else if(i==58) f.hDdnsEntry58.value=i;
		else if(i==59) f.hDdnsEntry59.value=i;
		else if(i==60) f.hDdnsEntry60.value=i;
		else if(i==61) f.hDdnsEntry61.value=i;
		else if(i==62) f.hDdnsEntry62.value=i;
		else if(i==63) f.hDdnsEntry63.value=i;
	}
	else
	{
		if(i==0) f.hDdnsEntry0.value=\"-1\";
		else if(i==1) f.hDdnsEntry1.value=\"-1\";
		else if(i==2) f.hDdnsEntry2.value=\"-1\";
		else if(i==3) f.hDdnsEntry3.value=\"-1\";
		else if(i==4) f.hDdnsEntry4.value=\"-1\";
		else if(i==5) f.hDdnsEntry5.value=\"-1\";
		else if(i==6) f.hDdnsEntry6.value=\"-1\";
		else if(i==7) f.hDdnsEntry7.value=\"-1\";
		else if(i==8) f.hDdnsEntry8.value=\"-1\";
		else if(i==9) f.hDdnsEntry9.value=\"-1\";
		else if(i==10) f.hDdnsEntry10.value=\"-1\";
		else if(i==11) f.hDdnsEntry11.value=\"-1\";
		else if(i==12) f.hDdnsEntry12.value=\"-1\";
		else if(i==13) f.hDdnsEntry13.value=\"-1\";
		else if(i==14) f.hDdnsEntry14.value=\"-1\";
		else if(i==15) f.hDdnsEntry15.value=\"-1\";
		else if(i==16) f.hDdnsEntry16.value=\"-1\";
		else if(i==17) f.hDdnsEntry17.value=\"-1\";
		else if(i==18) f.hDdnsEntry18.value=\"-1\";
		else if(i==19) f.hDdnsEntry19.value=\"-1\";
		else if(i==20) f.hDdnsEntry20.value=\"-1\";
		else if(i==21) f.hDdnsEntry21.value=\"-1\";
		else if(i==22) f.hDdnsEntry22.value=\"-1\";
		else if(i==23) f.hDdnsEntry23.value=\"-1\";
		else if(i==24) f.hDdnsEntry24.value=\"-1\";
		else if(i==25) f.hDdnsEntry25.value=\"-1\";
		else if(i==26) f.hDdnsEntry26.value=\"-1\";
		else if(i==27) f.hDdnsEntry27.value=\"-1\";
		else if(i==28) f.hDdnsEntry28.value=\"-1\";
		else if(i==29) f.hDdnsEntry29.value=\"-1\";
		else if(i==30) f.hDdnsEntry30.value=\"-1\";
		else if(i==31) f.hDdnsEntry31.value=\"-1\";
		else if(i==32) f.hDdnsEntry32.value=\"-1\";
		else if(i==33) f.hDdnsEntry33.value=\"-1\";
		else if(i==34) f.hDdnsEntry34.value=\"-1\";
		else if(i==35) f.hDdnsEntry35.value=\"-1\";
		else if(i==36) f.hDdnsEntry36.value=\"-1\";
		else if(i==37) f.hDdnsEntry37.value=\"-1\";
		else if(i==38) f.hDdnsEntry38.value=\"-1\";
		else if(i==39) f.hDdnsEntry39.value=\"-1\";
		else if(i==40) f.hDdnsEntry40.value=\"-1\";
		else if(i==41) f.hDdnsEntry41.value=\"-1\";
		else if(i==42) f.hDdnsEntry42.value=\"-1\";
		else if(i==43) f.hDdnsEntry43.value=\"-1\";
		else if(i==44) f.hDdnsEntry44.value=\"-1\";
		else if(i==45) f.hDdnsEntry45.value=\"-1\";
		else if(i==46) f.hDdnsEntry46.value=\"-1\";
		else if(i==47) f.hDdnsEntry47.value=\"-1\";
		else if(i==48) f.hDdnsEntry48.value=\"-1\";
		else if(i==49) f.hDdnsEntry49.value=\"-1\";
		else if(i==50) f.hDdnsEntry50.value=\"-1\";
		else if(i==51) f.hDdnsEntry51.value=\"-1\";
		else if(i==52) f.hDdnsEntry52.value=\"-1\";
		else if(i==53) f.hDdnsEntry53.value=\"-1\";
		else if(i==54) f.hDdnsEntry54.value=\"-1\";
		else if(i==55) f.hDdnsEntry55.value=\"-1\";
		else if(i==56) f.hDdnsEntry56.value=\"-1\";
		else if(i==57) f.hDdnsEntry57.value=\"-1\";
		else if(i==58) f.hDdnsEntry58.value=\"-1\";
		else if(i==59) f.hDdnsEntry59.value=\"-1\";
		else if(i==60) f.hDdnsEntry60.value=\"-1\";
		else if(i==61) f.hDdnsEntry61.value=\"-1\";
		else if(i==62) f.hDdnsEntry62.value=\"-1\";
		else if(i==63) f.hDdnsEntry63.value=\"-1\";	
	}
}
</script>
	
<body topmargin=\"10\"
leftmargin=\"0\"><table width=\"100%\" align=center cellpadding=\"0\" cellspacing=\"0\">"

echo "<tr><td><table width=\"100%\" border=\"1\" align=center cellpadding=\"3\" cellspacing=\"0\" bordercolor=\"#CCCCCC\"
bgcolor=\"#FFFFFF\"><tr><TD class=table_title align=middle
width=\"15%\"><STRONG>服务商</STRONG></TD><td width=\"20%\" align=center
class=\"table_title\"><strong>接口名</strong></td><TD class=table_title align=middle
width=\"10%\"><STRONG>使能</STRONG></TD><td width=\"10%\" align=center
class=\"table_title\"><strong>协议</strong></td><td width=\"15%\" align=center
class=\"table_title\"><strong>主机/域名</strong></td><td width=\"10%\" align=center
class=\"table_title\"><strong>用户名</strong></td><td width=\"10%\" align=center
class=\"table_title\"><strong>密码</strong></td><td width=\"10%\" align=center
class=\"table_title\"><strong>删除</strong></td></TR></tr>"
node_index1=`/userfs/bin/tcapi get Ddns_Common Entrylist1`
node_index2=`/userfs/bin/tcapi get Ddns_Common Entrylist2`
for j in $node_index1$node_index2
do
	node_name="Ddns_Entry$j"
  	Activate=`/userfs/bin/tcapi get $node_name Active`
  	DDNS_Name=`/userfs/bin/tcapi get $node_name DDNS_Name`
	DDNS_Protocal=`/userfs/bin/tcapi get $node_name DDNS_Protocal`
  	DDNS_Interface=`/userfs/bin/tcapi get $node_name DDNS_Interface`
  	MYHOST=`/userfs/bin/tcapi get $node_name MYHOST`
  	DDNS_Domain=`/userfs/bin/tcapi get $node_name DDNS_Domain`
	USERNAME=`/userfs/bin/tcapi get $node_name USERNAME`

	if [ "$Activate" = "no attribute information" -o "$Activate" = "no node information" -o "$Activate" = "N/A" -o "$Activate" = "" ]
	then
		Activate="N/A"
		continue
	fi
	
  	if [ "$Activate" = "Yes" ] 
  	then
  		Activate="启用"
	elif [ "$Activate" = "No" ] 
	then
		Activate="禁用"
	else  Activate="Wrong setting"
	fi
	
	if [ "$DDNS_Name" = "no attribute information" -o "$DDNS_Name" = "no node information" ]
	then
		DDNS_Name="N/A"
	fi
	
	if [ "$DDNS_Protocal" = "no attribute information" -o "$DDNS_Protocal" = "no node information" ]
	then
		DDNS_Protocal="N/A"
	fi
	
	if [ "$DDNS_Interface" = "no attribute information" -o "$DDNS_Interface" = "no node information" ]
	then
		DDNS_Interface="N/A"
	fi
	
	if [ "$MYHOST" = "no attribute information" -o "$MYHOST" = "no node information" ]
	then
		MYHOST="N/A"
	fi
	
	if [ "$DDNS_Domain" = "no attribute information" -o "$DDNS_Domain" = "no node information" ]
	then
		DDNS_Domain="N/A"
	fi
	
	if [ "$USERNAME" = "no attribute information" -o "$USERNAME" = "no node information" ]
	then
		USERNAME="N/A"
	fi

	if [ "$DDNS_Name" = "China Telecom" ]
	then
		DDNS_Name="中国电信"
	fi
	
	DDNS_Name=`echo "$DDNS_Name" | sed 's/[<]/\&lt;/g; s/[>]/\&gt;/g'`

	echo "<tr align=\"middle\"><td width=\"15%\">$DDNS_Name</td><td
	width=\"20%\">$DDNS_Interface</td><td width=\"10%\">$Activate</td><td
	width=\"10%\">$DDNS_Protocal</td><td
	width=\"15%\">$MYHOST-$DDNS_Domain</td><td
	width=\"10%\">$USERNAME</td><td width=\"10%\">******</td><td width=\"10%\"><input id=\"$j\" type=\"checkbox\" name=\"rml\" onclick=\"doDel($j);\" value=\"$j\"></td></tr>"

done

echo "</table></td></tr></table></body></html>"

