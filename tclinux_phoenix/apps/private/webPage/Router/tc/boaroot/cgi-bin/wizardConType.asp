<%
if request_Form("wanTransFlag") = "1" then
	tcWebApi_set("Wan_Common","TransMode","wan_TransMode")
	tcWebApi_set("WebCurSet_Entry","wan_pvc","WAN_PVC")
	tcWebApi_set("WebCurSet_Entry","nat_pvc","WAN_PVC")
else	
	tcWebApi_set("Timezone_Entry","TZ","uiViewdateTZ")
end if
%>

<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script type='text/javascript' src="/jsl.js"></script>
<script>
function ExitWizard() {
    if (confirm("Quit setup wizard and discard settings ?")) {
		window.close();
	}
}

<%if tcWebApi_get("WebCustom_Entry","isISPWizard","h") = "Yes" then%>
function doISPList(){
	if(document.wzConTypeform.CBISPList.checked){
		document.wzConTypeform.Typeradio[0].disabled = true;
		document.wzConTypeform.Typeradio[1].disabled = true;
		document.wzConTypeform.Typeradio[2].disabled = true;
		document.wzConTypeform.Typeradio[3].disabled = true;
	}
	else{
		document.wzConTypeform.Typeradio[0].disabled = false;
		document.wzConTypeform.Typeradio[1].disabled = false;
		document.wzConTypeform.Typeradio[2].disabled = false;
		document.wzConTypeform.Typeradio[3].disabled = false;
	}
}
<%end if%>
function uiSave() {
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	document.wzConTypeform.wanMultiService.value = 1;
	document.wzConTypeform.WAN_PVC_EXT.value = 0;
	<%end if%>

	<%if tcWebApi_get("WebCustom_Entry","isISPWizard","h") = "Yes" then%>
	if(document.wzConTypeform.CBISPList.checked){
		document.wzConTypeform.action = "/cgi-bin/wizardISP.asp"
		document.wzConTypeform.TypeFlag.value=0;
	}
	else
	<%end if%>
	{
		if(document.wzConTypeform.Typeradio[0].checked){
			document.wzConTypeform.action = "/cgi-bin/wizardDHCP.asp"
			document.wzConTypeform.TypeFlag.value=0;		
		}
		else if(document.wzConTypeform.Typeradio[1].checked){
			document.wzConTypeform.action = "/cgi-bin/wizardStatic.asp"
			document.wzConTypeform.TypeFlag.value=1;
		}
		else if(document.wzConTypeform.Typeradio[2].checked){
			document.wzConTypeform.action = "/cgi-bin/wizardPPP.asp"
			document.wzConTypeform.TypeFlag.value=2;
		}
		else{
			document.wzConTypeform.action = "/cgi-bin/wizardBridge.asp"
			document.wzConTypeform.TypeFlag.value=3;
		}
	}
    document.wzConTypeform.submit();
}

<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
function doTransChange() {
	var pvc = document.wzConTypeform.wan_TransMode.selectedIndex;
	var hasAtm=0, hasPtm=0, hasWan0=0, hasPON;

<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	hasAtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	hasPtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	hasWan0 = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then%>
	hasPON = 1;
<%end if%>


if(hasAtm==1 && hasPtm==1 && hasWan0==1){
//AtmPtmEther
	if(pvc==1) //ptm
		pvc = 8;
	else if(pvc==2) //ether
		pvc = 10;
}
if(hasAtm==1 && hasPtm==1 && hasWan0==0){
//AtmPtm, no Ether
	if(pvc==1) //ptm
		pvc = 8;
}
if(hasAtm==1 && hasPtm==0 && hasWan0==1){
//AtmEther, no Ptm
	if(pvc==1) //ether
		pvc = 10;
}
if(hasAtm==0 && hasPtm==1 && hasWan0==1){
//PtmEther, no Atm
	if(pvc==0) //ptm
		pvc = 8;
	else if(pvc==1) //ether
		pvc = 10;
}
if (hasAtm==0 && hasPtm==0 && hasWan0==0 && hasPON==1){
	// PON , no AtmPtmEther
	pvc = 0;
}
	document.wzConTypeform.WAN_PVC.value = pvc;
	document.wzConTypeform.wanTransFlag.value = 1;
	document.wzConTypeform.submit();
	return;
}

<%end if%>
</script>
</head>

<body topmargin="10" leftmargin="0">

<form name="wzConTypeform" method="post">

<INPUT TYPE="HIDDEN" NAME="TypeFlag" VALUE="3">

<INPUT TYPE="HIDDEN" NAME="wanTransFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="WAN_PVC" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wanMultiService" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="WAN_PVC_EXT" VALUE="0">

<table width="500" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="500" height="5" valign="baseline" class="orange"></td>
  </tr>
</table>

<table border="0" width="500" align="center" cellspacing="0" cellpadding="0">
  <tr>
    <td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
    <td width="300" height="50" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
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
    <td width="500" bgcolor="#FFFFFF" class="headline">&nbsp; Quick Start - ISP 
	Connection Type 
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
	Select the <%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
	WAN Transfer Mode and <%end if%>
	internet connection type to connect to your ISP. Click <B>NEXT</B> 
	to continue.
	
        
        <input type="hidden" name="wzExitFlag">
        
    </td>
  </tr>

  <tr>
    <td></td>
    <td></td>
    <td></td>
  </tr>
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
  <tr>
    <td width="40" height="40">&#12288;</td>
    <td width="150" class="tabdata" align="left"><div align=left><font color="#000000"><B>WAN Transfer Modes:</B></font></div></td>
	<td class="tabdata" align=left>
	<SELECT NAME="wan_TransMode" SIZE="1" onChange="doTransChange()">

	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>		
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then asp_Write("selected") end if %>>ATM
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then asp_Write("selected") end if %>>PTM
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then asp_Write("selected") end if %>>Ethernet
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then%>
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "PON" then asp_Write("selected") end if %>>Fiber
	<%End If%>

	</SELECT></td>
  </tr>
<%end if%>

  <tr>
    <td width="40" height="40">&#12288;</td>
    <td width="150" class="tabdata" align="left">
        
        <input type="radio" name="Typeradio" id="uiViewConTypeStatus0" <% if tcWebApi_get("Wan_PVC","ISP","h") = "0" then asp_Write("checked") end if %>>
        
        
        Dynamic IP Address
        
        </td>
    <td width="310" class="tabdata" align="left"><span class="databold">
    
	Choose this option to obtain a IP address automatically from your ISP.
	
	</span></td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td  height="40" class="tabdata" align="left">
        
        <input type="radio" name="Typeradio" id="uiViewConTypeStatus1" <% if tcWebApi_get("Wan_PVC","ISP","h") = "1" then asp_Write("checked") end if %>>
        
        
        Static IP Address
        
        </td>
    <td class="tabdata" align="left"><span class="databold">
	
	Choose this option to set static IP information provided to you by your ISP.
	
	</span></td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td  height="40" class="tabdata" align="left">
        
		<input type="radio"  name="Typeradio" id="uiViewConTypeStatus2" <% if tcWebApi_get("Wan_PVC","ISP","h") = "2" then asp_Write("checked") end if %>>
        
        
        PPPoE<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>/PPPoA<%end if%>
        
        </td>
    <td class="tabdata" align="left"><span class="databold">
    
	Choose this option if your ISP uses PPPoE<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>/PPPoA<%end if%>. (For most <%if tcWebApi_get("Wan_Common","TransMode","h") = "PON" then%>PON<%else%>DSL<%End if%> users)
	
	</span></td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td  height="40" class="tabdata" align="left">
        
        <input type="radio" name="Typeradio" id="uiViewConTypeStatus3" <% if tcWebApi_get("Wan_PVC","ISP","h") = "3" then asp_Write("checked")  elseif tcWebApi_get("Wan_PVC","ISP","h") = "N/A" then asp_Write("selected") end if %>>
        Bridge Mode
        </td>
    <td class="tabdata" align="left"><span class="databold">
    
	Choose this option if your ISP uses Bridge Mode.
	
	</span></td>
  </tr>
  <%if tcWebApi_get("WebCustom_Entry","isISPWizard","h") = "Yes" then%>
  <tr>
    <td>&#12288;</td>
    <td  height="40" class="tabdata" align="left">
        
        <input type="checkbox" name="CBISPList" id="uiViewCBISPList" onClick="doISPList()">
        Auto setup by ISP list
        </td>
    <td class="tabdata" align="left"><span class="databold">
    
	Choose from ISP List to set your ISP information.
	
	</span></td>
  </tr>
  <%end if%>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">&#12288;</td>
    <td class="tabdata" align="left">&#12288;</td>
  </tr>
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
  <tr>
    <td height="25"></td>
  </tr>
<%end if%>
</table>

<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="right">
          <input name="BackBtn" type="button" width="50" value="BACK" class="tabdata" onClick="javascript:window.location='/cgi-bin/wizardTZ.asp'"  >
          	  <input name="NextBtn" type="button" width="50" value="NEXT" class="tabdata" onClick="uiSave()">
          	  <input name="ExitBtn" type="button" width="50" value="EXIT" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>
</form>
</body>
</html>        
