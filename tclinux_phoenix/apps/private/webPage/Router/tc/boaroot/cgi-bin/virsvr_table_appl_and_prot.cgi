#!/bin/sh
echo -e "Content-Type: text/html\n\n<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\">\n</head><script language=\"JavaScript\">"
echo -e "function doDelete(i){\n\tvar f=top.main.document.VIRTUALSVR_form\n\tf.editnum.value=i;\n\tf.delFlag.value=1;\n\tf.submit();\n}"
echo -e "</script><body><FORM METHOD=\"POST\" name=\"VirtualCgiForm\">\n<table id=\"CgiInnerTable\" align=center width=\"440\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">\n<tr height=\"30\"><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Rule</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Application</strong>\n</td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Protocol</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Start Port</strong></td>\n<td width=\"70\" align=\"center\" class=\"tabdata\"><strong>End port</strong></td><td width=\"120\" align=\"center\" class=\"tabdata\"><strong>Local IP Address</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Start Port Local</strong></td>\n<td width=\"70\" align=\"center\" class=\"tabdata\"><strong>End Port Local</strong></td>\n<td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Edit</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Drop</strong></td></tr>"

tcapi_path=/userfs/bin/
cur_pvc=`"$tcapi_path"tcapi get WebCurSet_Entry nat_pvc`
java_str=
for j in 0 1 2 3 4 5 6 7 8 9
do 
	node_name="VirServer_PVC"$cur_pvc"_Entry"$j""
	STARTPORT=`"$tcapi_path"tcapi get $node_name STARTPORT`
	case $STARTPORT in
		"no node information" | "no attribute information" | "connection error") STARTPORT="N/A"
        		ENDPORT="N/A"
         		LOCALIP="N/A" 
         		LOCAL_SPORT="N/A"
         		LOCAL_EPORT="N/A"
				APPLICATION="N/A"
				PROTOCOL="N/A";;
		*)ENDPORT=`"$tcapi_path"tcapi get $node_name ENDPORT`
        	  LOCALIP=`"$tcapi_path"tcapi get $node_name LOCALIP`	
	  	  LOCAL_SPORT=`"$tcapi_path"tcapi get $node_name LOCAL_SPORT`
	          LOCAL_EPORT=`"$tcapi_path"tcapi get $node_name LOCAL_EPORT`
			  APPLICATION=`"$tcapi_path"tcapi get $node_name APPLICATION`
			  PROTOCOL=`"$tcapi_path"tcapi get $node_name PROTOCOL`
		  case $ENDPORT in
			"no node information" | "no attribute information" | "connection error") ENDPORT="N/A";;
		  esac
		  case $LOCALIP in
			"no node information" | "no attribute information" | "connection error") LOCALIP="N/A";;
		  esac
		  case $LOCAL_SPORT in
		  	"no node information" | "no attribute information" | "connection error") LOCAL_SPORT="N/A";;
		  esac
		  case $LOCAL_EPORT in
			"no node information" | "no attribute information" | "connection error") LOCAL_EPORT="N/A";;
		  esac
		  case $APPLICATION in
			"no node information" | "no attribute information" | "connection error") APPLICATION="N/A";;
		  esac
		  case $PROTOCOL in
			"no node information" | "no attribute information" | "connection error") PROTOCOL="N/A";;
		  esac;;
	esac  
	javastr="<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$APPLICATION</td><td align=\"center\" class=\"tabdata\">$PROTOCOL</td><td align=\"center\" class=\"tabdata\">$STARTPORT</td><td align=\"center\" class=\"tabdata\">$ENDPORT</td><td align=\"center\" class=\"tabdata\">$LOCALIP</td><td align=\"center\" class=\"tabdata\">$LOCAL_SPORT</td><td align=\"center\" class=\"tabdata\">$LOCAL_EPORT</td><td align=\"center\" class=\"tabdata\"><IMG src=\"/pen.gif\" onmouseover=\"this.style.cursor='hand'\" onClick='javascript:"
	case $APPLICATION in
		"N/A") javastr=""$javastr"top.main.document.VIRTUALSVR_form.SelectApplication.value=\"N/A\";top.main.document.VIRTUALSVR_form.Application.value=\"--\";" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.SelectApplication.value=\"$APPLICATION\";top.main.document.VIRTUALSVR_form.Application.value=\"$APPLICATION\";" ;;
	esac
	case $PROTOCOL in
		"N/A") javastr=""$javastr"top.main.document.VIRTUALSVR_form.SelectProtocol.value=\"N/A\";" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.SelectProtocol.value=\"$PROTOCOL\";" ;;
	esac
	case $STARTPORT in
		"N/A") javastr=""$javastr"top.main.document.VIRTUALSVR_form.start_port.value=\"0\";" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.start_port.value=\"$STARTPORT\";" ;;
	esac
	case $ENDPORT in
		"N/A") javastr=""$javastr"top.main.document.VIRTUALSVR_form.end_port.value=\"0\";" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.end_port.value=\"$ENDPORT\";" ;;
	esac
	case $LOCALIP in
		"N/A") javastr=""$javastr"top.main.document.VIRTUALSVR_form.Addr.value=\"0.0.0.0\";" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.Addr.value=\"$LOCALIP\";" ;;
	esac
	case $LOCAL_SPORT in
		"N/A") javastr=""$javastr"top.main.document.VIRTUALSVR_form.local_sport.value=\"0\";" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.local_sport.value=\"$LOCAL_SPORT\";" ;;
	esac
	case $LOCAL_EPORT in
		"N/A") javastr=""$javastr"top.main.document.VIRTUALSVR_form.local_eport.value=\"0\";" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.local_eport.value=\"$LOCAL_EPORT\";" ;;
	esac
	case $STARTPORT in
		"N/A")javastr=""$javastr"top.main.document.VIRTUALSVR_form.editnum.value=\"$j\";'></td></tr>" ;;
		*) javastr=""$javastr"top.main.document.VIRTUALSVR_form.editnum.value=\"$j\";'></td><td align=\"center\" class=\"tabdata\"><IMG onClick=\"doDelete($j);\"; src=\"/cross.gif\"></td></tr>" ;;
	esac
	echo $javastr
done
echo "</table></form></body></html>"
