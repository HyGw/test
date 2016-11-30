<%
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
<script language="JavaScript" type="text/javascript" src="/jsl.js"></script>
<script language="JavaScript" type="text/javascript" src="/val.js"></script>
<script language="JavaScript" type="text/javascript" src="/pvc.js"></script>
<script language="JavaScript" type="text/javascript" src="/ip_new.js"></script>
<style type="text/css">

</style>
<script>
function doSubnetCheck()
{
	var sIP = document.wzStaticform.wzStatic_IPaddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits = sIP[0].split(".");
	var mask = document.wzStaticform.wzStatic_SubMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");
	var gatewayIP = document.wzStaticform.wzStatic_GatewayAdd.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
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
function ExitWizard()
{
    if (confirm("Quit setup wizard and discard settings ?")) {
		window.close();
	}
}

function check_vci()
{
	var value;
	var value1;
	var pvc;
	var form=document.wzStaticform;
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

function IPoACheck() {
	if (document.wzStaticform.uiViewEncaps.selectedIndex == 2) {
		document.wzStaticform.wzStatic_SubMask.disabled = true;
		document.wzStaticform.wzStatic_SubMask.value = "0.0.0.0";
		document.wzStaticform.wzStatic_GatewayAdd.disabled = true;
		document.wzStaticform.wzStatic_GatewayAdd.value = "0.0.0.0";
	}  else {
		document.wzStaticform.wzStatic_SubMask.disabled = false;
		document.wzStaticform.wzStatic_GatewayAdd.disabled = false;
	}
	return;
}


function vpivcicheck() {
	var value;
	
	value = document.forms[0].uiViewPvcVpi.value;
	if (!isNumeric(value)) {
	   alert('Input for VPI has to be an integer');
		return false;
	}
	if(Number(value) > 255 || Number(value)<0) 
	{ 
		alert("VPI must be in the range 0-255"); 
		return false; 
  	}

	value = document.forms[0].uiViewPvcVci.value;
	if (!isNumeric(value)) {
	   alert('Input for VCI has to be an integer');
		return false;
	}
  	if(Number(value) > 65535 || Number(value)<32) 
  	{ 
		alert("VCI must be in the range 32-65535"); 
		return false; 
  	}  
	return true;
}
function uiSave() {
	var value;
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if(!check_vci())
		return;
	if (!vpivcicheck())
	  	return;
<%end if%>
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
	document.wzStaticform.submit();
}
</script>
</head>

<body topmargin="10" leftmargin="0">
<body topmargin="10" leftmargin="0">

<form name="wzStaticform" method="post" ACTION="/cgi-bin/wizardcomp.asp">
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

<INPUT TYPE="HIDDEN" NAME="TypeFlag" VALUE="1">
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
<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then %>
<INPUT TYPE="HIDDEN" NAME="wan_IPVERSION" VALUE="IPv4/IPv6">	
<% end if %>
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
	Static IP Address
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
    Enter the static IP information provided to you by your ISP. Click <B>NEXT</B> 
	to continue.
    
	 
	<input type="hidden" name="wanVCFlag">
	
	</td>
  </tr>
  <tr>
    <td height="10"></td>
    <td></td>
    <td></td>
  </tr>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    VPI:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<% If tcWebApi_get("Wan_PVC","VPI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VPI","s") end if%>"  name="wzDHCP_VPI" id="uiViewPvcVpi" title="VPI parameter 0..255">
        
        (0~255) </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    VCI:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<% If tcWebApi_get("Wan_PVC","VCI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VCI","s") end if%>"  name="wzDHCP_VCI" id="uiViewPvcVci" title="VCI parameter 6..65535">
        
        (1~65535)</td>
  </tr>
<%end if%>

  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    IP Address: 
    
    </td>
    <td class="tabdata" align="left">
        
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
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
  <tr>
    <td width="40">&#12288;</td>
    <td class="tabdata" align="right">
    
    Connection Type: 
    
    </td>
    <td class="tabdata" align="left">
    	
        <select id="uiViewEncaps" name="uiViewEncaps">
			<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if %>>1483 Bridged IP LLC
      <option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if %>>1483 Bridged IP VC-Mux
      <option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP LLC(IPoA)" then asp_Write("selected") end if %>>1483 Routed IP LLC(IPoA)
      <option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP VC-Mux" then asp_Write("selected") end if %>>1483 Routed IP VC-Mux
        </select>
        
        </td>
  </tr>
<%end if%>

  <tr>
    <td height="25"></td>
  </tr>
</table>

<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="right">
          <input name="BackBtn" type="button" width="50" value="BACK" class="tabdata" onClick="javascript:window.location='/cgi-bin/wizardConType.asp'">
      	  <input name="NextBtn" type="button" width="50" value="NEXT" class="tabdata" onClick="uiSave()">
	    <input name="ExitBtn" type="button" width="50" value="EXIT" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>


</form>
</body>
</html>        
