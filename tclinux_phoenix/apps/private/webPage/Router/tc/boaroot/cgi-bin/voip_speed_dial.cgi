#!/bin/sh
echo -e "Content-Type: text/html\n\n<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\">\n</head><script language=\"JavaScript\">"
echo -e "function doDelete(i){\n\tvar f=top.main.document.VOIP_SPEED_DIAL_form\n\tf.editnum.value=i;\n\tf.delFlag.value=1;\n\tf.submit();\n}"
echo -e "function changeswitch(i){\n\tvar f=top.main.document.VOIP_SPEED_DIAL_form\n\t
	top.main.document.getElementById('EditIndex').innerHTML = top.main.document.VOIP_SPEED_DIAL_form.editnum.value;
	if(i==\"N/A\"){
		f.VoIPDestination.selectedIndex=0;
		f.VoIPUserId.disabled=false;
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
		return;
	}
	for(j=0;j<f.length;j++){
		if(f.VoIPDestination.value==i){
			f.VoIPDestination.selectedIndex=j;
			break;
		}
	}
	if(i==\"Proxy\"){
		f.VoIPUserId.disabled=false;
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
	}else if(i==\"Local Line\"){
		f.VoIPUserId.disabled=true;
		f.VoIPSIPSpeedLine[0].disabled=false;
		f.VoIPSIPSpeedLine[1].disabled=false;		
	}else if(i==\"Direct Call\"){
		f.VoIPUserId.disabled=false;
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
	}

	}"
echo -e "</script><body><FORM METHOD=\"POST\" name=\"VoIPSpeedDialCgiForm\">\n<table id=\"CgiInnerTable\" align=center width=\"440\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">\n<tr height=\"30\"><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Index</strong></td><td width=\"170\" align=\"center\" class=\"tabdata\"><strong>Speed Dial Number</strong></td>\n<td width=\"170\" align=\"center\" class=\"tabdata\"><strong>User Id</strong></td>\n<td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Edit</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Drop</strong></td></tr>"
tcapi_path=/userfs/bin/
java_str=
for j in 0 1 2 3 4 5 6 7 8 9
do
	node_name="VoIPSpeed_Entry"$j""
	Destination=`"$tcapi_path"tcapi get $node_name Destination`
	SpeedDialNumber=`"$tcapi_path"tcapi get $node_name SpeedDialNumber`
	case $SpeedDialNumber in
		"no node information" | "no attribute information" | "connection error") SpeedDialNumber="N/A"
        		UserId="N/A"
			Destination="N/A";;
		*)UserId=`"$tcapi_path"tcapi get $node_name UserId`
		  Destination=`"$tcapi_path"tcapi get $node_name Destination`
		  SIPSpeedLine=`"$tcapi_path"tcapi get $node_name SIPSpeedLine`
		  case $UserId in
			"no node information" | "no attribute information" | "connection error") UserId="N/A";;
		  esac
		  case $Destination in
			"no node information" | "no attribute information" | "connection error") Destination="N/A";;
		  esac 
		  case $SIPSpeedLine in
			"no node information" | "no attribute information" | "connection error") SIPSpeedLine="N/A";;
		  esac;;
	esac
	javastr="<tr><td align=\"center\" class=\"tabdata\">"$j"</td><td align=\"center\" class=\"tabdata\">$SpeedDialNumber</td><td align=\"center\" class=\"tabdata\">"
	case $Destination in
		"Proxy" ) javastr=""$javastr"$UserId</td>" ;;
		"Local Line" ) javastr=""$javastr"line$SIPSpeedLine</td>" ;;
		"Direct Call" | "N/A" ) javastr=""$javastr"$UserId</td>" ;;
	esac
	javastr=""$javastr"</td><td align=\"center\" class=\"tabdata\"><IMG src=\"/pen.gif\" onmouseover=\"this.style.cursor='hand'\" onClick='javascript:"
	case $SpeedDialNumber in
		"N/A") javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.VoIPSpeedDialNumber.value=\"\";" ;;
		*) javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.VoIPSpeedDialNumber.value=\"$SpeedDialNumber\";" ;;
	esac
	case $UserId in
		"N/A") javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.VoIPUserId.value=\"\";" ;;
	*) javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.VoIPUserId.value=\"$UserId\";" ;;
	esac
	case $SIPSpeedLine in
		"1") javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.VoIPSIPSpeedLine[1].checked=true;" ;;
		*) javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.VoIPSIPSpeedLine[0].checked=true;" ;;
	esac
	case $SpeedDialNumber in
		"N/A")javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.editnum.value=\"$j\"; changeswitch(\""$Destination"\");'></td></tr>" ;;
		*) javastr=""$javastr"top.main.document.VOIP_SPEED_DIAL_form.editnum.value=\"$j\"; changeswitch(\""$Destination"\");'></td><td align=\"center\" class=\"tabdata\"><IMG onClick=\"doDelete($j);\"; src=\"/cross.gif\"></td></tr>" ;;
	esac
	echo $javastr
done
echo "</table></form></body></html>"
