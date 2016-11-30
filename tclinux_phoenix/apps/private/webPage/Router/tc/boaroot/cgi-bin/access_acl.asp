<%
/*add a rule*/
if request_Form("ACLActionFlag") = "0" then

	TCWebAPI_set("ACL_Common","Activate","ActivateRDO")
	
	if TCWebAPI_get("ACL_Common", "Activate", "h") = "Yes" then
		TCWebAPI_set("WebCurSet_Entry","acl_id","RuleIndexSEL")
		/*remove original data*/
		TCWebAPI_unset("ACL_Entry")
		
		TCWebAPI_set("ACL_Entry","Activate","RuleActiveRDO")
		TCWebAPI_set("ACL_Entry","ScrIPAddrBegin","ScrIPaddrBeginTXT")
		TCWebAPI_set("ACL_Entry","ScrIPAddrEnd","ScrIPaddrEndTXT")
		TCWebAPI_set("ACL_Entry","Application","ApplicationSEL")
		TCWebAPI_set("ACL_Entry","Interface","InterfaceSEL")
	end if
if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then
	TCWebAPI_set("Ipv6ACL_Common","Activate","IPv6ActivateRDO")
	
	if TCWebAPI_get("Ipv6ACL_Common", "Activate", "h") = "Yes" then
		TCWebAPI_set("WebCurSet_Entry","ipv6_acl_id","IPv6RuleIndexSEL")
		/*remove original data*/
		TCWebAPI_unset("Ipv6ACL_Entry")
		
		TCWebAPI_set("Ipv6ACL_Entry","Activate","Ipv6RuleActiveRDO")
		TCWebAPI_set("Ipv6ACL_Entry","Ipv6Addr","Ipv6AddrTXT")
		TCWebAPI_set("Ipv6ACL_Entry","PrefixLen","Ipv6AddrPrefixTXT")
		TCWebAPI_set("Ipv6ACL_Entry","Application","Ipv6ApplicationSEL")
		TCWebAPI_set("Ipv6ACL_Entry","Interface","Ipv6InterfaceSEL")
	end if
end if	
	/*commit changes*/
	TCWebAPI_commit("ACL_Entry")
if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then
	TCWebAPI_commit("Ipv6ACL_Entry")	
end if	
/*select a rule*/	
elseif Request_Form("ACLActionFlag") = "1" then
	TCWebAPI_set("ACL_Common","Activate","ActivateRDO")
	TCWebAPI_set("WebCurSet_Entry","acl_id","RuleIndexSEL")
if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then
	TCWebAPI_set("Ipv6ACL_Common","Activate","IPv6ActivateRDO")
	TCWebAPI_set("WebCurSet_Entry","ipv6_acl_id","IPv6RuleIndexSEL")
end if	
/*remove a rule*/
elseif Request_Form("ACLActionFlag") = "2" then

	TCWebAPI_set("ACL_Common","Activate","ActivateRDO")

	if TCWebAPI_get("ACL_Common", "Activate", "h") = "Yes" then
		TCWebAPI_unset("ACL_Entry")
	end if
if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then
	TCWebAPI_set("Ipv6ACL_Common","Activate","IPv6ActivateRDO")

	if TCWebAPI_get("Ipv6ACL_Common", "Activate", "h") = "Yes" then
		TCWebAPI_unset("Ipv6ACL_Entry")
	end if
end if	
	TCWebAPI_commit("ACL_Entry")
if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then
	TCWebAPI_commit("Ipv6ACL_Entry")	
end if
end if
%>

<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
</head>

<!--<script language="javascript" src="/tc2func.js"></script>-->
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript">
	
function init(){
	if(document.ACL_form.ActivateRDO[1].checked){
		aclSwitch(0);
	}
	<%if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then%>
	if(document.ACL_form.IPv6ActivateRDO[1].checked){
		ipv6AclSwitch(0);
	}
	<%end if%>
	return;
}

function doAdd() {
	
  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to add these configurations?")){
			return;
		}
	<%end if%>	

	document.forms[0].ACLActionFlag.value = 0;
}

function doIndexChange() {
	document.forms[0].ACLActionFlag.value = 1;
	document.forms[0].submit();
	return;
}

function doDel(){
	
  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to delete it?"))
		{
			return;
		}
	<%end if%>	

	document.forms[0].ACLActionFlag.value = 2;
}              

function aclSwitch(on_off){
	var value;
	form = document.ACL_form;
	
	if(on_off == 0){
		value = true;
	}else{
		value = false;
	}
	form.RuleIndexSEL.disabled = value;
	form.RuleActiveRDO[0].disabled = value;
	form.RuleActiveRDO[1].disabled = value;
	form.ScrIPaddrBeginTXT.disabled = value;
	form.ScrIPaddrEndTXT.disabled = value;
	form.ApplicationSEL.disabled = value;
	form.InterfaceSEL.disabled = value;
	
}

function aclRuleSwitch(on_off){
	var value;
	form = document.ACL_form;

	if(on_off == 0){
		value = true;
	}else{
		value = false;
	}
	form.ScrIPaddrBeginTXT.disabled = value;
	form.ScrIPaddrEndTXT.disabled = value;
	form.ApplicationSEL.disabled = value;
	form.InterfaceSEL.disabled = value;
}

<%if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then%>
function ipv6AclSwitch(on_off){
	var value;
	form = document.ACL_form;
	
	if(on_off == 0){
		value = true;
	}else{
		value = false;
	}
	form.IPv6RuleIndexSEL.disabled = value;
	form.Ipv6RuleActiveRDO[0].disabled = value;
	form.Ipv6RuleActiveRDO[1].disabled = value;
	form.Ipv6AddrTXT.disabled = value;
	form.Ipv6AddrPrefixTXT.disabled = value;
	form.Ipv6ApplicationSEL.disabled = value;
	form.Ipv6InterfaceSEL.disabled = value;
	
}

function ipv6AclRuleSwitch(on_off){
	var value;
	form = document.ACL_form;

	if(on_off == 0){
		value = true;
	}else{
		value = false;
	}
	form.Ipv6AddrTXT.disabled = value;
	form.Ipv6AddrPrefixTXT.disabled = value;
	form.Ipv6ApplicationSEL.disabled = value;
	form.Ipv6InterfaceSEL.disabled = value;
}

function verifyIpv6Addr(){
	form = document.ACL_form;
	var ipv6Addr = form.Ipv6AddrTXT.value;
	var prefixLen = form.Ipv6AddrPrefixTXT.value;
	var Ipv6_ACL_active = form.IPv6ActivateRDO[1];
	var hidden = form.ACLActionFlag.value;
	
	if(hidden == 0 && Ipv6_ACL_active.checked == false)
	{
		if((ipv6Addr == "::")&& (prefixLen == "0")){
			return true;
		}else{
			if((!doValidateIPv6Addr(ipv6Addr)) || (!doValidatePrefixLen(prefixLen))){
				return false;
			}else{
				return true;
			}
		}
	}
	return true;
}

function doValidateIPv6Addr(Address) {
	if(inValidIPv6Addr(Address)){
  	return false;
  }
  return true;
}

function doValidatePrefixLen(value1) {

	
	if(value1.match("[^0-9]") != null)  {

        alert("Prefix Length should be a number!!");
    	                    
        return false;
    }
	var PrefixLen = parseInt(value1);
	if (value1=="") {

        alert("Prefix Length should not be empty!!");

		return false;
	}
	if ( (PrefixLen > 128) || (PrefixLen < 0) ) {

        alert("Prefix Length should be between 0 and 128!!");

		return false;
	}
	return true;
}
<%end if%>

function verifyForm(){
	form = document.ACL_form;
	var addrBegin = form.ScrIPaddrBeginTXT.value;
	var addrEnd = form.ScrIPaddrEndTXT.value;
	var hidden = form.ACLActionFlag.value;
	var ACL_active = form.ActivateRDO[1];
	
<%if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then%>	
	if(!verifyIpv6Addr())
		return false;
<%end if%>		
	if(hidden == 0 && ACL_active.checked == false)
	{
		if((addrBegin == "0.0.0.0")&&(addrEnd=="0.0.0.0")){
			return true;
		}else{
			if((!doValidateIP(addrBegin,0,1,0))||(!doValidateIP(addrEnd,0,1,0))){
				return false;
			}else{
				return doValidateRange(addrBegin, addrEnd);
			}
		}
	}
}

function onClickACLSummary() {
	window.open("/cgi-bin/access_acllist.cgi","ACL_List","toolbar=no,menubar=no,scrollbars=yes,height=300, width=520,location=0,left=100,top=100");
	return false;
}

function doValidateIP(Address, option1, option2, Where) {

	var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var i;

	if(((option1 == 1 || option1 == 4) && Address == "0.0.0.0") || (option1 == 2 && Address == "255.255.255.255"))
		return true;
		
	if(address == null)
	{
		if(option1 == 4 && Address == "")
			return true;
		if(Where == 1){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
			alert("Invalid destination IP address: "+Address);
			<!-- RpEnd -->   
		}else if(Where == 2){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
			alert("Invalid Source IP Address!");
			<!-- RpEnd -->
		}else {
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg0" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg0" RpTextType="ASCII" -->	  	  	         		       
			alert("IP address is empty!");
		<!-- RpEnd -->
		}	    	        				         
		return false; 
	}
	
	digits = address[0].split(".");
	
	for(i=0; i < 4; i++)
	{
		if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (option1 != 4 && Number(digits[0]) > 223))
		{ 
			if(Where == 1){
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
				alert("Invalid destination IP address: "+Address);
				<!-- RpEnd --> 
			}else if(Where == 2){
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
				alert("Invalid Source IP Address!");
				<!-- RpEnd -->
			}else{	  
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg1" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg1" RpTextType="ASCII" -->  	               
				alert("Invalid IP address: " + Address);
				<!-- RpEnd --> 
			}     	
			return false; 
		}
	}

	if((!isValidIpAddr(digits[0],digits[1],digits[2],digits[3])) || (option1 == 3 && Address == "1.0.0.0") || (option2 == 1 && digits[3] == 0))
	{
		if(Where == 1)
		{
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
			alert("Invalid destination IP address: "+Address);
			<!-- RpEnd --> 
		}else if(Where == 2){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
			alert("Invalid Source IP Address!");
			<!-- RpEnd -->
		}else{	    
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg1" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg1" RpTextType="ASCII" -->  	               
			alert("Invalid IP address: " + Address);
			<!-- RpEnd --> 
		}     	
		return false;
	}

	return true;
}

function doValidateRange(startIP, endIP) {
	var staddress;
	var edaddress;
	var cnt;

	staddress=startIP.split(".");
	edaddress=endIP.split(".");
	
	for(cnt=0; cnt < 4; cnt++)
	{
		if((cnt<3)&&( Number(edaddress[cnt])!= Number(staddress[cnt])))
		{
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg88" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg88" RpTextType="ASCII" -->  	                       
		alert("End IP address and Start IP address are not in the same subnet!");
		<!-- RpEnd -->
		return false;
		}
	
		if((cnt==3)&&( Number(edaddress[cnt]) < Number(staddress[cnt])))
		{
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg7" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg7" RpTextType="ASCII" -->  	                       
			alert("End IP address is less than Start IP address");
			<!-- RpEnd -->      	            
			return false;
		}
	}

	return true;
}

function isValidIpAddr(ip1,ip2,ip3,ip4) {

	if(ip1==0 || ip4==255 || ip1==127 || ip4==0)
	return false;

	return true;
}

</script>

<body onLoad="init()" >
<form METHOD="POST" action="/cgi-bin/access_acl.asp" name="ACL_form" onSubmit="return verifyForm();">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td height="5" class="light-orange" colspan="5">&nbsp;</td>
		</tr>
		<tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF">Access Control Setup</font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"></td>
			<td width="10" ></td>
			<td width="440"></td>
		</tr>
		<tr>
			<td height="25" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">ACL</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="ActivateRDO" VALUE="Yes"
				<%if TCWebAPI_get("ACL_Common", "Activate", "h") = "Yes" then asp_write("checked") end if %> onClick="aclSwitch(1);init()"><font color="#000000">Activated</font>
				<INPUT TYPE="RADIO" NAME="ActivateRDO" VALUE="No"
				<%if TCWebAPI_get("ACL_Common", "Activate", "h") = "No" then asp_write("checked") end if %> onClick="aclSwitch(0)"><font color="#000000">Deactivated</font>
			</td>
		</tr>
		<tr>
			<td height="25" class="title-sub"><font color="#000000">Access Control Editing</font></td>
			<td class="light-orange"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td width="150" height="25" class="light-orange">&nbsp;</td>
			<td width="10" class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">ACL Rule Index</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="RuleIndexSEL" SIZE="1" onChange="doIndexChange()">
					<OPTION VALUE="0" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "0" then asp_Write("selected") end if %>>1
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "1" then asp_Write("selected") end if %>>2
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "2" then asp_Write("selected") end if %>>3
					<OPTION VALUE="3" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "3" then asp_Write("selected") end if %>>4
					<OPTION VALUE="4" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "4" then asp_Write("selected") end if %>>5
					<OPTION VALUE="5" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "5" then asp_Write("selected") end if %>>6
					<OPTION VALUE="6" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "6" then asp_Write("selected") end if %>>7
					<OPTION VALUE="7" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "7" then asp_Write("selected") end if %>>8
					<OPTION VALUE="8" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "8" then asp_Write("selected") end if %>>9
					<OPTION VALUE="9" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "9" then asp_Write("selected") end if %>>10
					<OPTION VALUE="10" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "10" then asp_Write("selected") end if %>>11
					<OPTION VALUE="11" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "11" then asp_Write("selected") end if %>>12
					<OPTION VALUE="12" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "12" then asp_Write("selected") end if %>>13
					<OPTION VALUE="13" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "13" then asp_Write("selected") end if %>>14
					<OPTION VALUE="14" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "14" then asp_Write("selected") end if %>>15
					<OPTION VALUE="15" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "15" then asp_Write("selected") end if %>>16
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Active</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="RuleActiveRDO" VALUE="Yes" 
				<%if TCWebAPI_get("ACL_Entry", "Activate", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000">Yes</font>
				<INPUT TYPE="RADIO" NAME="RuleActiveRDO" VALUE="No" 
				<%if TCWebAPI_get("ACL_Entry", "Activate", "h") = "No" then asp_write("checked") end if
				  if TCWebAPI_get("ACL_Entry", "Activate", "h") = "N/A" then asp_write("checked") end if  %>><font color="#000000">No</font>
			</td>
			</td>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Secure IP Address</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="ScrIPaddrBeginTXT" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("ACL_Entry", "ScrIPAddrBegin", "h") <> "N/A" then TCWebAPI_get("ACL_Entry", "ScrIPAddrBegin", "s") else asp_Write("0.0.0.0") end if %>" >~
				<INPUT TYPE="TEXT" NAME="ScrIPaddrEndTXT" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("ACL_Entry", "ScrIPAddrEnd", "h") <> "N/A" then TCWebAPI_get("ACL_Entry", "ScrIPAddrEnd", "s") else asp_Write("0.0.0.0") end if %>" ><font color="#000000">(0.0.0.0 ~ 0.0.0.0 means all IPs)</font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Application</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="ApplicationSEL" SIZE="1">
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "Web" then asp_write("selected") end if %>>Web
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "FTP" then asp_write("selected") end if %>>FTP
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "Telnet" then asp_write("selected") end if %>>Telnet
                                        <% if TCWebAPI_get("ACL_Common", "sshOption", "h") = "support" then %>
                                        <OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "SSH" then asp_write("selected") end if %>>SSH
                                        <% end if %>
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "TFTP" then asp_write("selected") end if %>>TFTP
                                        <OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "SNMP" then asp_write("selected") end if %>>SNMP
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "Ping" then asp_write("selected") end if %>>Ping
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "ALL" then asp_write("selected") end if %>>ALL
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Interface</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="InterfaceSEL" SIZE="1">
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Interface", "h") = "WAN" then asp_write("selected") end if %>>WAN
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Interface", "h") = "LAN" then asp_write("selected") end if %>>LAN
					<OPTION <%if TCWebAPI_get("ACL_Entry", "Interface", "h") = "Both" then asp_write("selected") end if %>>Both
				</SELECT>
			</td>
		</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="150" class="title-sub"><font color="#000000">Access Control Listing</font></td>
			<td width="10" class="light-orange"></td><td width="10" ><hr noshade class="light-orange-line"></td><td width="580"><hr noshade class="light-orange-line"></td><td width="10" ><hr noshade class="light-orange-line"></td></tr><tr>
			<td class="light-orange"></td><td class="light-orange"></td>
			<td ></td>
			<td>
				<!--
				<iframe src="/cgi-bin/access_acllist.cgi" frameborder="0" width="550" height="200"></iframe>
				-->
			<table width="482" border="1" align=center cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
			<tr height="20">
				<td width="60" align=center class="tabdata"><strong><font color="#000000">Index</font></strong></td>
				<td width="60" align=center class="tabdata"><strong><font color="#000000">Active</font></strong></td>
				<td width="180" align=center class="tabdata"><strong><font color="#000000">secure IP</font></strong></td>
				<td width="90" align=center class="tabdata"><strong><font color="#000000">Application</font></strong></td>
				<td width="80" align=center class="tabdata"><strong><font color="#000000">Interface</font></strong></td>
			</tr>
			<% tcWebApi_get("GUITemp_Entry0","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry1","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry2","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry3","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry4","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry5","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry6","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry7","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry8","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry9","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry10","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry11","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry12","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry13","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry14","aclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry15","aclFilterTRLine","s") %>
			</table>

			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td>&nbsp;</td><td>&nbsp;</td>
			<td class="tabdata">&nbsp;</td>
		</tr>
	</table>
<%if tcwebApi_get("WebCustom_Entry","isIpv6ACLSupported","h")="Yes" then%>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

		<tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF">IPv6 Access Control Setup</font></td>
			<td width="10" class="light-orange"></td>
			<td width="150" ><hr noshade class="light-orange-line"></td>
			<td width="10"><hr noshade class="light-orange-line"></td>
			<td width="440" ><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td height="25" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">IPv6 ACL</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="IPv6ActivateRDO" VALUE="Yes"
				<%if TCWebAPI_get("Ipv6ACL_Common", "Activate", "h") = "Yes" then asp_write("checked") end if %> onClick="ipv6AclSwitch(1);init()"><font color="#000000">Activated</font>
				<INPUT TYPE="RADIO" NAME="IPv6ActivateRDO" VALUE="No"
				<%if TCWebAPI_get("Ipv6ACL_Common", "Activate", "h") = "No" then asp_write("checked") end if %> onClick="ipv6AclSwitch(0)"><font color="#000000">Deactivated</font>
			</td>
		</tr>
		<tr>
			<td height="25" class="title-sub"><font color="#000000">IPv6 Access Control Editing</font></td>
			<td class="light-orange"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td width="150" height="25" class="light-orange">&nbsp;</td>
			<td width="10" class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">IPv6 ACL Rule Index</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="IPv6RuleIndexSEL" SIZE="1" onChange="doIndexChange()">
					<OPTION VALUE="0" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "0" then asp_Write("selected") end if %>>1
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "1" then asp_Write("selected") end if %>>2
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "2" then asp_Write("selected") end if %>>3
					<OPTION VALUE="3" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "3" then asp_Write("selected") end if %>>4
					<OPTION VALUE="4" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "4" then asp_Write("selected") end if %>>5
					<OPTION VALUE="5" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "5" then asp_Write("selected") end if %>>6
					<OPTION VALUE="6" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "6" then asp_Write("selected") end if %>>7
					<OPTION VALUE="7" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "7" then asp_Write("selected") end if %>>8
					<OPTION VALUE="8" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "8" then asp_Write("selected") end if %>>9
					<OPTION VALUE="9" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "9" then asp_Write("selected") end if %>>10
					<OPTION VALUE="10" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "10" then asp_Write("selected") end if %>>11
					<OPTION VALUE="11" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "11" then asp_Write("selected") end if %>>12
					<OPTION VALUE="12" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "12" then asp_Write("selected") end if %>>13
					<OPTION VALUE="13" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "13" then asp_Write("selected") end if %>>14
					<OPTION VALUE="14" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "14" then asp_Write("selected") end if %>>15
					<OPTION VALUE="15" <%if TCWebAPI_get("WebCurSet_Entry", "ipv6_acl_id", "h") = "15" then asp_Write("selected") end if %>>16
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Active</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="Ipv6RuleActiveRDO" VALUE="Yes" 
				<%if TCWebAPI_get("Ipv6ACL_Entry", "Activate", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000">Yes</font>
				<INPUT TYPE="RADIO" NAME="Ipv6RuleActiveRDO" VALUE="No" 
				<%if TCWebAPI_get("Ipv6ACL_Entry", "Activate", "h") = "No" then asp_write("checked") end if
				  if TCWebAPI_get("Ipv6ACL_Entry", "Activate", "h") = "N/A" then asp_write("checked") end if  %>><font color="#000000">No</font>
			</td>
			</td>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Secure IPv6 Address</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="Ipv6AddrTXT" SIZE="39" MAXLENGTH="39" VALUE=
				"<%if TCWebAPI_get("Ipv6ACL_Entry", "Ipv6Addr", "h") <> "N/A" then TCWebAPI_get("Ipv6ACL_Entry", "Ipv6Addr", "s") else asp_Write("::") end if %>" >
				<font size=+1>&nbsp;/&nbsp;</font>
				<INPUT TYPE="TEXT" NAME="Ipv6AddrPrefixTXT" SIZE="3" MAXLENGTH="3" VALUE=
				"<%if TCWebAPI_get("Ipv6ACL_Entry", "PrefixLen", "h") <> "N/A" then TCWebAPI_get("Ipv6ACL_Entry", "PrefixLen", "s") else asp_Write("0") end if %>" ><font color="#000000">(::/0 means all IPs)</font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Application</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="Ipv6ApplicationSEL" SIZE="1">
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Application", "h") = "Web" then asp_write("selected") end if %>>Web
<%if tcwebApi_get("WebCustom_Entry","isIPV6FTPSupported","h")="Yes" then%>
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Application", "h") = "FTP" then asp_write("selected") end if %>>FTP
<%end if%>
<%if tcwebApi_get("WebCustom_Entry","isIPV6TELNETSupported","h")="Yes" then%>
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Application", "h") = "Telnet" then asp_write("selected") end if %>>Telnet
<%end if%>
<%if tcwebApi_get("WebCustom_Entry","isIPV6TFTPSupported","h")="Yes" then%>
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Application", "h") = "TFTP" then asp_write("selected") end if %>>TFTP
<%end if%>
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Application", "h") = "Ping" then asp_write("selected") end if %>>Ping
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Application", "h") = "ALL" then asp_write("selected") end if %>>ALL
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Interface</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="Ipv6InterfaceSEL" SIZE="1">
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Interface", "h") = "WAN" then asp_write("selected") end if %>>WAN
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Interface", "h") = "LAN" then asp_write("selected") end if %>>LAN
					<OPTION <%if TCWebAPI_get("Ipv6ACL_Entry", "Interface", "h") = "Both" then asp_write("selected") end if %>>Both
				</SELECT>
			</td>
		</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="150" class="title-sub"><font color="#000000">IPv6 Access Control Listing</font></td>
			<td width="10" class="light-orange"></td><td width="10" ><hr noshade class="light-orange-line"></td><td width="580"><hr noshade class="light-orange-line"></td><td width="10" ><hr noshade class="light-orange-line"></td></tr><tr>
			<td class="light-orange"></td><td class="light-orange"></td>
			<td ></td>
			<td>
				<!--
				<iframe src="/cgi-bin/access_ipv6_acllist.cgi" frameborder="0" width="550" height="200"></iframe>
				-->
			<table width="482" border="1" align=center cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
			<tr height="20">
				<td width="60" align=center class="tabdata"><strong><font color="#000000">Index</font></strong></td>
				<td width="60" align=center class="tabdata"><strong><font color="#000000">Active</font></strong></td>
				<td width="180" align=center class="tabdata"><strong><font color="#000000">Secure IPv6 Address</font></strong></td>
				<td width="90" align=center class="tabdata"><strong><font color="#000000">Application</font></strong></td>
				<td width="80" align=center class="tabdata"><strong><font color="#000000">Interface</font></strong></td>
			</tr>
			<% tcWebApi_get("GUITemp_Entry0","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry1","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry2","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry3","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry4","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry5","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry6","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry7","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry8","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry9","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry10","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry11","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry12","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry13","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry14","ipv6AclFilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry15","ipv6AclFilterTRLine","s") %>
			</table>

			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td>&nbsp;</td><td>&nbsp;</td>
			<td class="tabdata">&nbsp;</td>
		</tr>
	</table>
<%end if%>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="160" class="black" height="42" >&nbsp;</td>
			<td width="160" class="orange">&nbsp;</td>
			<td width="440" class="orange">
				<INPUT TYPE="HIDDEN" NAME="ACLActionFlag" VALUE="0">
				<INPUT TYPE="SUBMIT" NAME="ACLAddBTN" VALUE="SET" onClick="doAdd();">
				<INPUT TYPE="SUBMIT" NAME="ACLDelBTN" VALUE="DELETE" onClick="doDel();">
				<INPUT TYPE="RESET"  NAME="ACLCancelBTN" VALUE="CANCEL"onClick="document.forms[0].ACLActionFlag.value = 3; javascript:window.location='access_acl.asp'">
			</td>
		</tr>
	</table>
</form>
</body>
</html>
