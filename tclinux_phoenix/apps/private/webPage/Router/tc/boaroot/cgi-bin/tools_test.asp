<%
tcWebApi_set("WebCurSet_Entry","diag_pvc","Test_PVC")
tcWebApi_set("WebCurSet_Entry","diag_pvc_ext","Test_PVC_Ext")
TCWebApi_set("Diagnostic_PVC","PingOtherIPaddr","IP")
TCWebApi_set("Diagnostic_PVC","PingOtherType","pingtest_type")

if tcWebApi_get("WebCustom_Entry","isTracertSupported","h") = "Yes" then
	TCWebApi_set("Diagnostic_PVC","TracertEn","tracert_enable")
If Request_Form("tracert_enable") = "1" then
	TCWebApi_set("Diagnostic_PVC","TracertIPaddr","tracert_destaddr")
end If
end if

If Request_Form("testFlag") = "1" then
tcWebApi_commit("Diagnostic_PVC")
elseif Request_Form("testFlag") = "2" then
tcWebApi_unset("Diagnostic_PVC")
tcWebApi_set("WebCurSet_Entry","dev_pvc","Test_PVC")
tcWebApi_set("WebCurSet_Entry","dev_pvc_ext","Test_PVC_Ext")
end if
%>


<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" tppabs="http://192.168.1.1/css/style.css" type="text/css">
<script language="JavaScript" src="/val.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript">
	function DoRefresh() {
		document.Test_Form.testFlag.value = 2;
		document.Test_Form.submit();
	}
	
	function doTest(){
	
	  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
			if(!confirm("Are you sure you want to save these configurations?"))
				return;
		<%end if%>	
		
		if(document.Test_Form.pingtest_type[0].checked){
		    value = document.Test_Form.IP.value;
    		if(inValidIPAddr(value)){
        		return false;
        	}
		}    
		document.Test_Form.testFlag.value = 1;
		document.Test_Form.submit();
	}
	function doReload() {
	    if(document.Test_Form.pingtest_type[0].checked){
			document.Test_Form.pingtest_type.value = "Yes";
		}
		else{
			document.Test_Form.pingtest_type.value = "No";
		}
		document.Test_Form.submit();
		return;
    }

<% if tcWebApi_get("WebCustom_Entry","isTracertSupported","h") = "Yes" then %>	    
	function doTracertTest()
	{
	        setDisplay('WaitMsgTracert', 1);        
	   	document.Test_Form.testFlag.value = 1;
	   	document.Test_Form.tracert_enable.value = 1;
		document.Test_Form.submit();
		return ;
	}
	
	function RefeshForTracert()
	{
		var str1="Wait";
		var str2=document.Test_Form.tracert_result.value;
	
		if(str1 == str2){
			setTimeout("RefeshForTracert()",5000);
			document.location.href="/cgi-bin/tools_test.asp";
		}
	}
<%end if%>
</script>
</head>
<% if tcWebApi_get("WebCustom_Entry","isTracertSupported","h") = "Yes" then %>
	<body onLoad="setTimeout('RefeshForTracert()',5000);">
<%else%>
	<body>
<%end if%>
<FORM METHOD="POST" ACTION="/cgi-bin/tools_test.asp" name="Test_Form">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF"><INPUT TYPE="HIDDEN" NAME="testFlag" VALUE="0">
	<tr>
		<td height="5" class="light-orange" colspan="6">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Diagnostic Test</font>
		<td width="10" class="black">&nbsp;</td>
		<td colspan="2"></td>
		<td width="60"> </td>
		<td width="215"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td width="50" >&nbsp;</td>
<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" then%>
		<td width="275"  class="tabdata"><font color="#000000">WAN:</font>
			<SELECT NAME="Test_PVC" SIZE="1" onChange="DoRefresh()">
				<OPTION value="0" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "0" then asp_Write("selected") end if %>> 0
				<OPTION value="1" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "1" then asp_Write("selected") end if %>> 1
				<OPTION value="2" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "2" then asp_Write("selected") end if %>> 2
				<OPTION value="3" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "3" then asp_Write("selected") end if %>> 3
				<OPTION value="4" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "4" then asp_Write("selected") end if %>> 4
				<OPTION value="5" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "5" then asp_Write("selected") end if %>> 5
				<OPTION value="6" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "6" then asp_Write("selected") end if %>> 6
				<OPTION value="7" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "7" then asp_Write("selected") end if %>> 7
<%else%>
		<td width="275"  class="tabdata"><font color="#000000">Virtual Circuit:</font>
			<SELECT NAME="Test_PVC" SIZE="1" onChange="DoRefresh()">
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
				<OPTION value="0" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "0" then asp_Write("selected") end if %>>PVC0
				<OPTION value="1" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "1" then asp_Write("selected") end if %>>PVC1
				<OPTION value="2" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "2" then asp_Write("selected") end if %>>PVC2
				<OPTION value="3" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "3" then asp_Write("selected") end if %>>PVC3
				<OPTION value="4" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "4" then asp_Write("selected") end if %>>PVC4
				<OPTION value="5" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "5" then asp_Write("selected") end if %>>PVC5
				<OPTION value="6" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "6" then asp_Write("selected") end if %>>PVC6
				<OPTION value="7" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "7" then asp_Write("selected") end if %>>PVC7
<%else%>
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
				<OPTION value="0" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "0" then asp_Write("selected") end if %>>PVC0
				<OPTION value="1" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "1" then asp_Write("selected") end if %>>PVC1
				<OPTION value="2" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "2" then asp_Write("selected") end if %>>PVC2
				<OPTION value="3" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "3" then asp_Write("selected") end if %>>PVC3
				<OPTION value="4" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "4" then asp_Write("selected") end if %>>PVC4
				<OPTION value="5" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "5" then asp_Write("selected") end if %>>PVC5
				<OPTION value="6" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "6" then asp_Write("selected") end if %>>PVC6
				<OPTION value="7" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "7" then asp_Write("selected") end if %>>PVC7
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
				<OPTION value="8" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "8" then asp_Write("selected") end if %>>PTM0
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
				<OPTION value="10" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "10" then asp_Write("selected") end if %>>WAN0
	<%end if%>

<%end if%>
<%end if%>
			</SELECT></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<%if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td width="50" >&nbsp;</td>
	<td width="275"  class="tabdata"><font color="#000000">Service:</font>
		<SELECT NAME="Test_PVC_Ext" SIZE="1" onChange="DoRefresh()">
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
	</SELECT></td></tr></tr>
<%end if%>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
<%if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td width="50" >&nbsp;</td>
	<td width="275"  class="tabdata"><font color="#000000">Service:</font>
		<SELECT NAME="Test_PVC_Ext" SIZE="1" onChange="DoRefresh()">
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
	</SELECT></td></tr></tr>
<%end if%>
<%end if%>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td ><hr noshade class="light-orange-line"></td>
		<td ><hr noshade class="light-orange-line"></td>
		<td ><hr noshade class="light-orange-line"></td>
		<td ><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt; <font color="#000000">Testing Ethernet LAN connection</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strlanconn = "<%TCWebApi_get("Diagnostic_PVC","EtherLanConn","s")%>";
		if(strlanconn == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +strlanconn+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strlanconn+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt; <font color="#000000">Testing <%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "1" then%>GPON PLOAM<%elseif tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" then%>EPON MPCP<%else%>xPON MAC<%end if%> Status</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var xponLinkStatus = "<%TCWebApi_get("Diagnostic_PVC","xPONLinkSta","s")%>";
		if(xponLinkStatus == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +xponLinkStatus+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +xponLinkStatus+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Testing <%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "1" then%>GPON OMCI<%elseif tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" then%>EPON OAM<%else%>xPON Link<%end if%> Status</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var xPONAppLinkStatus = "<%TCWebApi_get("Diagnostic_PVC","xPONAppLinkSta","s")%>";
		if(xPONAppLinkStatus == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +xPONAppLinkStatus+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +xPONAppLinkStatus+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
<%else%>
<%if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") <> "10" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt; <font color="#000000">Testing <%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>x<%else%>A<%end if%>DSL Synchronization</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var stradslsyn = "<%TCWebApi_get("Diagnostic_PVC","ADSLSyn","s")%>";
		if(stradslsyn == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +stradslsyn+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +stradslsyn+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
<%if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") <> "8" then%>
<%if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") <> "9" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Testing ATM OAM segment ping</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var stratmoa = "<%TCWebApi_get("Diagnostic_PVC","ATMOAMSeg","s")%>";
		if(stratmoa == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +stratmoa+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +stratmoa+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt; <font color="#000000">Testing ATM OAM end to end ping</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var stratmoaend = "<%TCWebApi_get("Diagnostic_PVC","ATMOAMEnd2End","s")%>";
		if(stratmoaend == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +stratmoaend+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +stratmoaend+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
<%end if%>
<%end if%>
<%end if%>
<%end if%>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Ping DHCP Primary DNS ( </font>
			<%If tcWebApi_get("Dproxy_Entry","type","h") = "1" Then tcWebApi_get("Dproxy_Entry","Primary_DNS","s") else tcWebApi_get("DeviceInfo_PVC","DNSIP","s") end if%><font color="#000000"> )</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strpingdns = "<%TCWebApi_get("Diagnostic_PVC","PingPriDNS","s")%>";
		if(strpingdns == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +strpingdns+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strpingdns+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Ping www.yahoo.com ( use DHCP Pri. or Sec. DNS )</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strpingyahoo = "<%TCWebApi_get("Diagnostic_PVC","PingYahoo","s")%>";
		if(strpingyahoo == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +strpingyahoo+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strpingyahoo+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Ping other IP address :  </font>			
			<INPUT TYPE="RADIO" NAME="pingtest_type" VALUE="Yes" <% if tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "Yes" then asp_Write("checked")end if %> onClick="doReload()"> <font color="#000000">Yes</font>
			<INPUT TYPE="RADIO" NAME="pingtest_type" VALUE="No"  <% if tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "N/A" then asp_Write("checked") end if %> onClick="doReload()"> <font color="#000000">No</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strpingother = "<%TCWebApi_get("Diagnostic_PVC","PingOther","s")%>";
		if(strpingother == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +strpingother+ '</strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strpingother+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>	
	<% If tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "Yes" Then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;<div align=center><font color="#000000">IP :  </font><INPUT TYPE="TEXT" NAME="IP" SIZE="15" MAXLENGTH="15" VALUE="<%tcWebApi_get("Diagnostic_PVC","PingOtherIPaddr","s")%>" ></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>		
	<% End If %>
	<tr>
		<td colspan="2" class="light-orange" >&nbsp;</td>
		<!--<td class="light-orange"></td>-->
		<td colspan="4">&nbsp;</td>
		<!--<td width="60">&nbsp;</td><td width="215">&nbsp;</td>-->
</table>
<% if tcWebApi_get("WebCustom_Entry","isTracertSupported","h") = "Yes" then %>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="light-orange" height="42" >&nbsp;</td>
		<td width="160" >&nbsp;</td>
		<td width="440" ><INPUT TYPE="SUBMIT" NAME="TestBtn" VALUE="START" onClick="doTest();"></td>
	</tr>
</table></div>
<%else%>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td>
		<td width="160" class="orange">&nbsp;</td>
		<td width="440" class="orange"><INPUT TYPE="SUBMIT" NAME="TestBtn" VALUE="START" onClick="doTest();"></td>
	</tr>
</table></div>
<%end if%>

<% if tcWebApi_get("WebCustom_Entry","isTracertSupported","h") = "Yes" then %>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Trace Route Test</font>
		<td width="10" class="black">&nbsp;</td>
		<td width="50" ><hr noshade ></td>
		<td width="275" ><hr noshade></td>
		<td width="60"> <hr noshade> </td>
		<td width="215"> <hr noshade> </td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata"><font color="#000000">Dest Address:</font>
			<INPUT TYPE="TEXT" NAME="tracert_destaddr" SIZE="30" MAXLENGTH="60" VALUE="<%If TCWebApi_get("Diagnostic_PVC","TracertIPaddr","h")  <> "N/A" then TCWebApi_get("Diagnostic_PVC","TracertIPaddr","s") else asp_Write("") end if%>" >
		</td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strTracertResult = "<%TCWebApi_get("Diagnostic_PVC","TracertResult","s")%>";
		if(strTracertResult == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong>' +strTracertResult+ '</strong></div>');
		else	if(strTracertResult != "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strTracertResult+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<DIV id="WaitMsgTracert"><font color="#FF0000" size="-1"> traceroute is testing, please wait </font></DIV>
			<script language="JavaScript" type="text/JavaScript">
<% if tcWebApi_get("Diagnostic_PVC","TracertResult","h") = "Wait" then %>
				setDisplay('WaitMsgTracert', 1);
<%else%>
				setDisplay('WaitMsgTracert', 0);
<%end if%>
			</script>
		</td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>

<% if tcWebApi_get("Diagnostic_PVC","TracertResult","h") = "PASS" then %>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td colSpan="3">
			<script language="JavaScript" type="text/JavaScript">
			document.writeln("<iframe src='/cgi-bin/getTracentResult.cgi' frameborder='0' width='500' ></iframe>" );
			</script>		
		</td>
	</tr>
<%end if%>	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td colSpan="4">&nbsp;</td>
	</tr>
	<INPUT TYPE="HIDDEN" NAME="tracert_enable" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="tracert_result" VALUE="<%TCWebApi_get("Diagnostic_PVC","TracertResult","s")%>"">
</table>

<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td>
		<td width="160" class="orange">&nbsp;</td>
		<td width="440" class="orange"><INPUT TYPE="BUTTON" NAME="TestTracertBtn" VALUE="START" onClick="doTracertTest();"></td>
	</tr>
</table>
<%end if%>

</form>
</body></html>
