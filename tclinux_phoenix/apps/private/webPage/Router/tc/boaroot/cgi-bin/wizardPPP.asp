<%
if tcwebApi_get("WebCustom_Entry","isC5VOLLCOMMSupport","h") = "Yes" then
	tcWebApi_set("Account_Entry0","web_passwd","uiViewPassword")
	TCWebApi_set("Account_Entry0","console_passwd","uiViewPassword")
end if
 tcWebApi_set("Wan_PVC","ISP","TypeFlag")
if request_Form("wanMultiService") = "1" then
	tcWebApi_set("WebCurSet_Entry", "wan_pvc_ext", "WAN_PVC_EXT")
	tcWebApi_set("WebCurSet_Entry", "nat_pvc_ext", "WAN_PVC_EXT")
end if	
%>
<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" src="OutVariant.asp"></script>
<script language="JavaScript" type="text/javascript" src="/jsl.js"></script>
<script language="JavaScript" type="text/javascript" src="/val.js"></script>
<script language="JavaScript" type="text/javascript" src="/pvc.js"></script>
<style type="text/css">

</style>
<script>
function ExitWizard() {
    if (confirm("Quit setup wizard and discard settings ?")) {
		window.close();
	}
}

function check_vci()
{
	var value;
	var value1;
	var pvc;
	var form=document.wzPPPform;
	pvc = <%tcwebApi_get("WebCurSet_Entry","wan_pvc","s")%>;
	value =form.wzDHCP_VCI.value;
	value1=form.wzDHCP_VPI.value;

<%if tcWebApi_get("WebCustom_Entry","sharepvc","h") = "Yes" then%>
	return true;
<%end if%>
	if(pvc == 0)
	{
		if((value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 1)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}
	if(pvc == 2)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}
	if(pvc == 3)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 4)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}
	if(pvc == 5)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 6)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 7)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	return false;
}

function uiSave() {
//	if(!pppDoValidatePage() || !pvcDoValidatePage())
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if(!check_vci())
		return;
	   	
	if(!pvcDoValidatePage())
		return ;
<%end if%>
  if(quotationCheck(document.forms[0].uiViewUserNameMark) ) 
		return;
	if(quotationCheck(document.forms[0].uiViewPasswordMark) ) 
		return;
	
		document.wzPPPform.submit();

}

function pvcDoValidatePage() {
	var value;
	var message;

	value = document.wzPPPform.uiViewPvcVpi.value;
	if(!isNumeric(value)){
		alert("The VPI is not an Integer! Please set an integer");
		return false;
	}
	else if(Number(value) > 255 || Number(value)<0) {
		alert("VPI must be in the range 0-255");
		return false;
	}
	value = document.wzPPPform.uiViewPvcVci.value;
	if(!isNumeric(value)) {
		alert("The VCI is not an integer! Please set an integer");
		return false;
	}
	else if(Number(value) > 65535 || Number(value)<32) {
		alert("VCI must be in the range 32-65535");
		return false;
	}
	return true;
}

function pppDoValidatePage() {
    var message;
    var value;
    var count=0;

    value = document.wzPPPform.Al_PPPUsername.value;
    message = valDoValidateUsrPwd(value,'1');
    if(message!=null) { return false; erlDoAddError("bad","Al_PPPUsername","",value,message); count++; }

    value = document.wzPPPform.Al_PPPPassword.value;
    message = valDoValidateUsrPwd(value,'0');
    if(message!=null) { return false; erlDoAddError("bad","Al_PPPPassword","",value,message);  count++; }
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
function doLoad()
{
	if(document.wzPPPform.uiViewPasswordMark != null)
		document.wzPPPform.uiViewPasswordMark.value = pppPwd;
}
</script>
</head>

<body topmargin="10" leftmargin="0" onLoad="doLoad()">

<form name="wzPPPform" method="post" ACTION="/cgi-bin/wizardcomp.asp">
<INPUT TYPE="HIDDEN" NAME="VCI0" VALUE='<%if tcWebApi_get("Wan_PVC0","Active","h") = "Yes" then tcWebApi_get("Wan_PVC0","VCI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VCI1" VALUE='<%if tcWebApi_get("Wan_PVC1","Active","h") = "Yes" then tcWebApi_get("Wan_PVC1","VCI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VCI2" VALUE='<%if tcWebApi_get("Wan_PVC2","Active","h") = "Yes" then tcWebApi_get("Wan_PVC2","VCI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VCI3" VALUE='<%if tcWebApi_get("Wan_PVC3","Active","h") = "Yes" then tcWebApi_get("Wan_PVC3","VCI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VCI4" VALUE='<%if tcWebApi_get("Wan_PVC4","Active","h") = "Yes" then tcWebApi_get("Wan_PVC4","VCI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VCI5" VALUE='<%if tcWebApi_get("Wan_PVC5","Active","h") = "Yes" then tcWebApi_get("Wan_PVC5","VCI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VCI6" VALUE='<%if tcWebApi_get("Wan_PVC6","Active","h") = "Yes" then tcWebApi_get("Wan_PVC6","VCI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VCI7" VALUE='<%if tcWebApi_get("Wan_PVC7","Active","h") = "Yes" then tcWebApi_get("Wan_PVC7","VCI","s") else asp_write("0") end if%>'>

<INPUT TYPE="HIDDEN" NAME="VPI0" VALUE='<%if tcWebApi_get("Wan_PVC0","Active","h") = "Yes" then tcWebApi_get("Wan_PVC0","VPI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VPI1" VALUE='<%if tcWebApi_get("Wan_PVC1","Active","h") = "Yes" then tcWebApi_get("Wan_PVC1","VPI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VPI2" VALUE='<%if tcWebApi_get("Wan_PVC2","Active","h") = "Yes" then tcWebApi_get("Wan_PVC2","VPI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VPI3" VALUE='<%if tcWebApi_get("Wan_PVC3","Active","h") = "Yes" then tcWebApi_get("Wan_PVC3","VPI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VPI4" VALUE='<%if tcWebApi_get("Wan_PVC4","Active","h") = "Yes" then tcWebApi_get("Wan_PVC4","VPI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VPI5" VALUE='<%if tcWebApi_get("Wan_PVC5","Active","h") = "Yes" then tcWebApi_get("Wan_PVC5","VPI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VPI6" VALUE='<%if tcWebApi_get("Wan_PVC6","Active","h") = "Yes" then tcWebApi_get("Wan_PVC6","VPI","s") else asp_write("0") end if%>'>
<INPUT TYPE="HIDDEN" NAME="VPI7" VALUE='<%if tcWebApi_get("Wan_PVC7","Active","h") = "Yes" then tcWebApi_get("Wan_PVC7","VPI","s") else asp_write("0") end if%>'>
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
<INPUT TYPE="HIDDEN" NAME="wan_IPv6Addr" VALUE="">	
<INPUT TYPE="HIDDEN" NAME="wan_IPv6Prefix" VALUE="">	
<INPUT TYPE="HIDDEN" NAME="wan_IPv6DefGw" VALUE="">	
<INPUT TYPE="HIDDEN" NAME="wan_MLD1" VALUE="Yes">	
<INPUT TYPE="HIDDEN" NAME="PPPDHCPv6Mode_Flag" VALUE="1">	
<INPUT TYPE="HIDDEN" NAME="PPPIPv6PDRadio2" VALUE="Yes">	
<% end if %>
<INPUT TYPE="HIDDEN" NAME="wan_dot1q" VALUE="No">	
<INPUT TYPE="HIDDEN" NAME="wan_BridgeInterface1" VALUE="No">	
<INPUT TYPE="HIDDEN" NAME="PPPAuthen" VALUE="AUTO">	
<INPUT TYPE="HIDDEN" NAME="wan_RIP1" VALUE="RIP1">	
<INPUT TYPE="HIDDEN" NAME="wan_RIP_Dir1" VALUE="None">	
<INPUT TYPE="HIDDEN" NAME="wan_IGMP1" VALUE="No">	
<table width="500" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="500" height="5" valign="baseline" class="orange"></td>
  </tr>
</table>

<table border="0" width="500" align="center" cellspacing="0" cellpadding="0">
  <tr>
    <td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
    <td width="300" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
	  <%If tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" Then%>
	  xPON Router
	  <%Else%>
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%>ADSL Router
	  <%End If%>
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
	PPPoE<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>/PPPoA<%end if%>
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
    Enter the PPPoE<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>/PPPoA<%end if%> information provided to you by your ISP. Click <B>NEXT</B> 
	to continue.
    
    </td>
  </tr>
  <tr>
    <td height="10"></td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    Username:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="text" value="<%If tcWebApi_get("Wan_PVC","USERNAME","h") <> "N/A" then tcWebApi_get("Wan_PVC","USERNAME","s") end if%>" size="14" maxlength="32" class="" name="uiViewUserNameMark" id="uiViewUserName">
        
    </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    Password:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="password" value="" size="14" maxlength="32" class="" name="uiViewPasswordMark" id="uiViewPassword">
        
    </td>
  </tr>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    VPI:
    
    </td>
    <td class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<%If tcWebApi_get("Wan_PVC","VPI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VPI","s") end if%>" name="wzDHCP_VPI" id="uiViewPvcVpi" title="VPI parameter 0..255">
        
        (0~255)
    </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    VCI:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<%If tcWebApi_get("Wan_PVC","VCI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VCI","s") end if%>" name="wzDHCP_VCI" id="uiViewPvcVci" title="VCI parameter 6..65535">
        
        (1~65535)
    </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    Connection Type: 
    
    </td>
    <td class="tabdata" align="left">
    	
        <select id="uiViewEncaps" name="uiViewEncaps">
			<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE LLC" then asp_Write("selected") end if %>>PPPoE LLC
      <option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE VC-Mux" then asp_Write("selected") end if %>>PPPoE VC-Mux
      <option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA LLC" then asp_Write("selected") end if %>>PPPoA LLC
      <option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA VC-Mux" then asp_Write("selected") end if %>>PPPoA VC-Mux
        </select>
        
    </td>
  </tr>
<%end if%>

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
          <input name="BackBtn" type="button" width="50" value="BACK" class="tabdata" onClick="<% if tcWebApi_get("WebCustom_Entry","isC5VOLLCOMMSupport","h") = "Yes" then asp_write("javascript:window.location='/cgi-bin/wizardpwd.asp'") else asp_write("javascript:window.location='/cgi-bin/wizardConType.asp'") end if %>">
         	  <input name="NextBtn" type="button" width="50" value="NEXT" class="tabdata" onClick="uiSave()">
	 <input name="ExitBtn" type="button" width="50" value="EXIT" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>


</form>
</body>
</html>        
