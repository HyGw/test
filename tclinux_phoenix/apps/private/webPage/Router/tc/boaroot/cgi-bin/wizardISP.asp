<%
 tcWebApi_set("Wan_PVC","ISP","TypeFlag")
%>
<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" type="text/javascript" src="/jsl.js"></script>
<script language="JavaScript" type="text/javascript" src="/val.js"></script>
<script language="JavaScript" type="text/javascript" src="/pvc.js"></script>
<script language="JavaScript" type="text/javascript" src="/ip_new.js"></script>
<script language="JavaScript" type="text/javascript" src="/Isplist.js"></script>
<style type="text/css">

</style>
<script>
function ExitWizard() {
    if (confirm("Quit setup wizard and discard settings ?")) {
		window.close();
	}
}

function uiSave() {
//	if(!pppDoValidatePage() || !pvcDoValidatePage())
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if(!pvcDoValidatePage())
		return ;
<%end if%>
	
	
	if(document.wzISPform.uiViewEncaps.selectedIndex < 5){
		document.wzISPform.TypeFlag.value=2;
		if(quotationCheck(document.forms[0].uiViewUserNameMark) ) 
			return;
		if(quotationCheck(document.forms[0].uiViewPasswordMark) ) 
			return;
		if((document.wzISPform.pppmtu.value >1492) || (document.wzISPform.pppmtu.value < 100)){
			alert("The range of TCP MTU:100 ~ 1492 or 0 as default value!");
			return;
		}
	}
	else if(document.wzISPform.uiViewEncaps.selectedIndex < 7)
		document.wzISPform.TypeFlag.value=0;
	else if(document.wzISPform.uiViewEncaps.selectedIndex < 9){
		value = document.forms[0].wzStatic_IPaddr.value;
		if (inValidIPAddr(value))
			return;
				
		value = document.forms[0].wzStatic_SubMask.value;
		if (inValidSubnetMask(value))
			return;
			
		value = document.forms[0].wzStatic_GatewayAdd.value;
		if (inValidIPAddr(value))
			return;
		if(!doSubnetCheck()){
			return;
		}
		document.wzISPform.TypeFlag.value=1;
	}
	document.wzISPform.submit();
}

function doSubnetCheck()
{
	var sIP = document.wzISPform.wzStatic_IPaddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits = sIP[0].split(".");
	var mask = document.wzISPform.wzStatic_SubMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");
	var gatewayIP = document.wzISPform.wzStatic_GatewayAdd.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits3 = gatewayIP[0].split(".");
	for(i=0;i<4;i++)
	{
	  if((digits2[i] & digits3[i]) != (digits2[i] & digits[i]))
		{
			alert("Static IP and ISP gateway are not in the same subnet");                                   						
			return false;
		}
	}
	return true;
}

function pvcDoValidatePage() {
	var value;
	var message;

	value = document.wzISPform.wzDHCP_VPI.value;
	if(!isNumeric(value)){
		alert("The VPI is not an Integer! Please set an integer");
		return false;
	}
	else if(Number(value) > 255 || Number(value)<0) {
		alert("VPI must be in the range 0-255");
		return false;
	}
	value = document.wzISPform.wzDHCP_VCI.value;
	if(!isNumeric(value)) {
		alert("The VCI is not an integer! Please set an integer");
		return false;
	}
	else if(Number(value) > 65535 || Number(value)<1) {
		alert("VCI must be in the range 1-65535");
		return false;
	}
	return true;
}

function pppDoValidatePage() {
    var message;
    var value;
    var count=0;

    value = document.wzISPform.uiViewUserNameMark.value;
    message = valDoValidateUsrPwd(value,'1');
    if(message!=null) { return false; erlDoAddError("bad","uiViewUserNameMark","",value,message); count++; }

    value = document.wzISPform.uiViewPasswordMark.value;
    message = valDoValidateUsrPwd(value,'0');
    if(message!=null) { return false; erlDoAddError("bad","uiViewPasswordMark","",value,message);  count++; }
    if(count!=0) message="PPP";
    else message="";
    return true;
}

function quotationCheck(object) {
	var len = object.value.length;
	var c;
	var i;
    for (i = 0; i < len; i++)
    {
	 	 var c = object.value.charAt(i);
      
	 	 if (c == '"' || c == '$' || c == '\\' || c == '`')
		 {
				alert('Can\'t input "quotation marks" or "dollar sign" or "backslash" or "acute accent" character!!');      	  								    	    	   		
		 		return true;
		 }
    }
    
	return false;
}

</script>
</head>

<body topmargin="10" leftmargin="0" onLoad="InitIsp_page()">

<form name="wzISPform" method="post" ACTION="/cgi-bin/wizardcomp.asp">
<INPUT TYPE="HIDDEN" NAME="wan_VCStatus" VALUE="Yes">

<INPUT TYPE="HIDDEN" NAME="TypeFlag" VALUE="2">
<INPUT TYPE="HIDDEN" NAME="ATM_QoS" VALUE="ubr">
<INPUT TYPE="HIDDEN" NAME="ATM_PCR" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="ATM_SCR" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="ATM_MBS" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wan_NAT" VALUE="Enable">
<INPUT TYPE="HIDDEN" NAME="wan_DefaultRoute" VALUE="Yes">
<INPUT TYPE="HIDDEN" NAME="wan_TCPMTU" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wan_RIPVersion" VALUE="RIP1">
<INPUT TYPE="HIDDEN" NAME="wan_RIPDirection" VALUE="None">
<INPUT TYPE="HIDDEN" NAME="wan_IGMP" VALUE="Disabled">
<INPUT TYPE="HIDDEN" NAME="wan_ConnectSelect" VALUE="Connect_Keep_Alive">
<INPUT TYPE="HIDDEN" NAME="wan_IdleTimeT" VALUE="">
<INPUT TYPE="HIDDEN" NAME="wan_TCPMSS" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wan_PPPGetIP" VALUE="Dynamic">
<INPUT TYPE="HIDDEN" NAME="wan_StaticIPaddr" VALUE="">
<INPUT TYPE="HIDDEN" NAME="wan_StaticIPSubMask" VALUE="">
<INPUT TYPE="HIDDEN" NAME="wan_StaticIpGateway" VALUE="">
<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then %>
<INPUT TYPE="HIDDEN" NAME="wan_IPVERSION" VALUE="IPv4/IPv6">	
<% end if %>
<%if tcWebApi_get("WebCustom_Entry","isISPWizard","h") = "Yes" then%>
<INPUT TYPE="HIDDEN" NAME="WAN_MTU_SAVE" VALUE="11">
<%end if%>
<table width="500" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="500" height="5" valign="baseline" class="orange"></td>
  </tr>
</table>

<table border="0" width="500" align="center" cellspacing="0" cellpadding="0">
  <tr>
    <td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
    <td width="300" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
	  <%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then%>xPON Router
	  <%else%>
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%>ADSL Router
	  <%end if%>
	</td>
  </tr>
  <tr>
    <td width="500" colspan="2" class="orange"> </td>
  </tr>
</table>

<table width="500" height="2" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td class="orange"> </td>
  </tr>
</table>

<table width="500" height="35" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
  <tr>
    <td width="500" bgcolor="#FFFFFF" class="headline">&nbsp; Quick Start - 
	Auto setup by ISP list
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
    Choose from ISP List to set your ISP information. Click <B>NEXT</B> 
	to continue.
    
    </td>
  </tr>
  <tr>
    <td height="10"></td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    Country:
    
    </td>
    <td width="310" class="tabdata" align="left">
	<select id=country onchange='getISP()'></select>
    </td>
  </tr>
    <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    ISP:
    
    </td>
    <td width="310" class="tabdata" align="left">
	<select id=isplist onChange='disableOption()'>
	<option value=0>Select</option>
	<option value=BT>BT</option>
	<option value=AOL>AOL</option>
	<option value=Others>Others</option>
	</select>
    </td>
  </tr>
  </table>
  
  <div id="username" style="display:none">
  <table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    Username:
    
    </td>
    <td width="310" class="tabdata" align="left">        
        <input type="text" value="<%If tcWebApi_get("Wan_PVC","USERNAME","h") <> "N/A" then tcWebApi_get("Wan_PVC","USERNAME","s") end if%>" size="15" maxlength="32" class="" name="uiViewUserNameMark" id="uiViewUserName">
        
    </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    Password:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input type="password" value="<%If tcWebApi_get("Wan_PVC","PASSWORD","h") <> "N/A" then tcWebApi_get("Wan_PVC","PASSWORD","s") end if%>" size="15" maxlength="32" class="" name="uiViewPasswordMark" id="uiViewPassword">
        
    </td>
  </tr>
  </table>
  </div>
  
  <table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    VPI:
    
    </td>
    <td width="310" class="tabdata" align="left">        
        <input id="vpi" size="15" maxlength="15" name="wzDHCP_VPI">
    </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    VCI:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input id="vci" size="15" maxlength="15" name="wzDHCP_VCI">
    </td>
  </tr>
  </table>
  
  <div id="staticIP" style="display:none">
  <table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    IP Address: 
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="wzStatic_IPaddr" value="<%If tcWebApi_get("Wan_PVC","IPADDR","h") <> "N/A" then tcWebApi_get("Wan_PVC","IPADDR","s") end if%>" id="uiViewIpAddress">
        
        </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    Subnet mask: 
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="wzStatic_SubMask" value="<%If tcWebApi_get("Wan_PVC","NETMASK","h") <> "N/A" then tcWebApi_get("Wan_PVC","NETMASK","s") end if%>" id="uiViewNetMask">
        
        </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    ISP Gateway:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="wzStatic_GatewayAdd" value="<%If tcWebApi_get("Wan_PVC","GATEWAY","h") <> "N/A" then tcWebApi_get("Wan_PVC","GATEWAY","s") end if%>" id="uiViewGateway">
        
        </td>
  </tr>
  </table>
  </div>
  
  <table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    Connection Type: 
    
    </td>
    <td width="310" class="tabdata" align="left">
		<select id="uiViewEncaps" name="uiViewEncaps" onchange = 'contypeChange()'>
		<option value=default selected>Click to select</option>
		<option value="PPPoE VC-Mux">PPPoE VC-Mux</option>
		<option value="PPPoE LLC">PPPoE LLC</option>
		<option value="PPPoA VC-Mux">PPPoA VC-Mux</option>
		<option value="PPPoA LLC">PPPoA LLC</option>
		<option value="1483 Bridged IP LLC">1483 Bridged IP LLC(Dynamic IP)</option>
		<option value="1483 Bridged IP VC-Mux">1483 Bridged IP VC-Mux(Dynamic IP)</option>
		<option value="1483 Bridged IP LLC">1483 Bridged IP LLC(Static IP)</option>
		<option value="1483 Bridged IP VC-Mux">1483 Bridged IP VC-Mux(Static IP)</option>
		<option value="1483 Routered IP LLC">1483 Bridged IP LLC</option>
		<option value="1483 Routered IP VC-Mux">1483 Bridged IP VC-Mux</option>
        </select>        
    </td>
  </tr>
  </table>
  
 <div id="mtuinfo">
 <table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    MTU: 
    
    </td>
    <td width="310" class="tabdata" align="left">
    	<input type='text' size="15" maxlength="15" name='pppmtu'>        
    </td>
  </tr>
 </table>
 </div>
<%end if%>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td height="25">
        
        <input type="hidden" name="wzPPPExitFlag">
        
    </td>
  </tr>
</table>
  
<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="right">
	<input name="BackBtn" type="button" width="50" value="BACK" class="tabdata" onClick="javascript:window.location='/cgi-bin/wizardConType.asp'">
	<input name="NextBtn" id="Next" type="button" width="50" value="NEXT" class="tabdata" onClick="uiSave()">
	<input name="ExitBtn" type="button" width="50" value="EXIT" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>


</form>
</body>
</html>        
