
<%
If Request_Form("Cwmpflag")="1" then 
	TCWebApi_set("Cwmp_Entry", "Active", "CWMP_Active")
	TCWebApi_set("Cwmp_Entry", "acsUrl", "CWMP_ACSURL")
	TCWebApi_set("Cwmp_Entry", "acsUserName", "CWMP_ACSUserName")
	If tcWebApi_get("WebCustom_Entry","isCZOTESupported","h") <> "Yes" Then
	TCWebApi_set("Cwmp_Entry", "acsPassword", "CWMP_ACSPassword")
	else
		if Request_Form("CWMP_ACSPassword") <> "******" Then
			TCWebApi_set("Cwmp_Entry", "acsPassword", "CWMP_ACSPassword")
		end if
	end if
	TCWebApi_set("Cwmp_Entry", "conReqPath", "CWMP_ConnectionRequestPath")
	TCWebApi_set("Cwmp_Entry", "conReqUserName", "CWMP_ConnectionRequestUserName")
	If tcWebApi_get("WebCustom_Entry","isCZOTESupported","h") <> "Yes" Then
		TCWebApi_set("Cwmp_Entry", "conReqPassword", "CWMP_ConnectionRequestPassword")
	else
		if Request_Form("CWMP_ConnectionRequestPassword") <> "******" Then
	TCWebApi_set("Cwmp_Entry", "conReqPassword", "CWMP_ConnectionRequestPassword")
		end if
	end if
	TCWebApi_set("Cwmp_Entry", "periodActive", "CWMP_PeriodActive")
	TCWebApi_set("Cwmp_Entry", "periodInterval", "CWMP_PeriodInterval")
	if tcwebApi_get("WebCustom_Entry","isTR69BindPVC","h") = "Yes" then
	TCWebApi_set("Cwmp_Entry", "BindActive", "CWMP_BindActive")
	IF Request_Form("CWMP_BindActive")="Yes" then
	TCWebApi_set("Cwmp_Entry", "wan_PVC", "bind_PVC")
	end if
	end if
	tcWebApi_commit("Cwmp_Entry")
End if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<script language="JavaScript" src="OutVariant.asp"></script>
<script language="javascript" src="/general.js"></script>
<script language="JavaScript">
function cwmpinit(){
	if(document.CWMP_form.elements[0].checked) 
		cwmpSwitch(1);
	else
		cwmpSwitch(0);	
		
	if(document.CWMP_form.CWMPLockFlag.value == 1)
		doCWMPLock();

<% If tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" Then%>
	if(document.CWMP_form.CWMP_ACSPassword != null)
		document.CWMP_form.CWMP_ACSPassword.value = acsPwd;
	if(document.CWMP_form.CWMP_ConnectionRequestPassword != null)
		document.CWMP_form.CWMP_ConnectionRequestPassword.value = conReqPwd;
<%end if%>
}

function cwmpSwitch(on_off)
{
	if(on_off == 0){
		for(i = 2; i < 11; i++){
			document.CWMP_form.elements[i].disabled = true;}
		<%if tcwebApi_get("WebCustom_Entry","isTR69BindPVC","h") = "Yes" then%>
			for(i = 11; i < 14; i++)
			document.CWMP_form.elements[i].disabled = true;
		<%end if%>	
	}		
	else{
		for(i = 2; i < 11; i++){
			document.CWMP_form.elements[i].disabled = false;
}
		if(document.CWMP_form.CWMP_PeriodActive[1].checked)
			document.CWMP_form.elements[10].disabled = true;
		<%if tcwebApi_get("WebCustom_Entry","isTR69BindPVC","h") = "Yes" then%>
			for(i = 11; i < 14; i++)
				document.CWMP_form.elements[i].disabled = false;
			if(document.CWMP_form.CWMP_BindActive[1].checked) 
				document.CWMP_form.elements[13].disabled = true;
		<%end if%>
	}
}
function cwmpPeriodSwitch(on_off)
{
	if(on_off == 0)
			document.CWMP_form.elements[10].disabled = true;
	else
			document.CWMP_form.elements[10].disabled = false;
}

function isNumeric(s)
{
  var len= s.length;
  var ch;
  if(len==0)
    return false;
  for( i=0; i< len; i++)
  {
    ch= s.charAt(i);
    if( ch > '9' || ch < '0')
    {
      return false;
    }
  }
  return true;
}

function cwmpNumValidCheck()
{
	var value1;
/*	
	value1 = document.CWMP_form.CWMP_ConnectionRequestPort.value;
	if(!isNumeric(value1)){
		alert("The connection request port should be interger!");
		return false;
	}
	else{
		if(Number(value1) > 65535 || Number(value1) < 1){
			alert("The connection request port should be 1-65535!");
			return false;
		}
	}
*/
	value1 = document.CWMP_form.CWMP_PeriodInterval.value;
	if(!document.CWMP_form.elements[8].checked)
		return true;
	if(!isNumeric(value1)){
		<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
			alert("The periodic interval should be positive interger!");
		<%else%>
		alert("The periodic interval should be interger!");
		<%end if%>
		return false;
	}
	else{
		if(Number(value1) > 999999 || Number(value1) < 1){
			alert("The inform interval should be 1-999999!");
			return false;
		}
	}
	
	return true;
}

function StringCheck(val)
{
    re = /^[^\s]+$/;
    if( re.test(val) )
        return true;
    else
        return false;
}

function cwmpStringValidCheck()
{
	var value1;
	var value2 = null;
	var url = /^https*:\/\/\w+/;
	var path = /^\/\w+/;

	value1 = document.CWMP_form.CWMP_ACSURL.value;
	if(value1 == "") {
		alert("ACS URL should not be empty!");
		return false;
	}

	if(!StringCheck(value1)){
		alert("ACS URL should not be have blank character!");
		return false;
	}
	else{	
		if(StringCheck(value1)){
			value2 = value1.match(url);
			if(value2 == null){
				alert("The format of ACS URL is wrong!");
				return false;
			}
			if(chineseCheck(document.CWMP_form.CWMP_ACSURL, "ACSURLId"))
			{
				return false;
			}
		}
	}
	
	if(chineseCheck(document.CWMP_form.CWMP_ACSUserName, "ACSUserNameId"))
	{
		return false;
	}
	if(chineseCheck(document.CWMP_form.CWMP_ACSPassword, "ACSPasswordId"))
	{
		return false;
	}
	if(chineseCheck(document.CWMP_form.CWMP_ConnectionRequestUserName, "CWMPConnectionRequestUserNameId"))
	{
		return false;
	}
	if(chineseCheck(document.CWMP_form.CWMP_ConnectionRequestPassword, "CWMPConnectionRequestPasswordId"))
	{
		return false;
	}
	/* value1 = document.CWMP_form.CWMP_ACSUserName.value;
	if(!StringCheck(value1)){
		alert("Username for logining ACS should not be empty!");
		return false;
	}
	
	value1 = document.CWMP_form.CWMP_ACSPassword.value;
	if(!StringCheck(value1)){
		alert("Password for logining ACS should not be empty!");
		return false;
	}
	*/
	
	value1 = document.CWMP_form.CWMP_ConnectionRequestPath.value;
    /*	
	if(!StringCheck(value1)){
		alert("Connection request path should not be empty!");
		return false;
	}
	else{
	*/
	if(StringCheck(value1)){
		value2 = value1.match(path);
		if(value2 == null){
			alert("The format of connection request path is wrong!");
			return false;
		}
	}
	/*
	value1 = document.CWMP_form.CWMP_ConnectionRequestUserName.value;
	if(!StringCheck(value1)){
		alert("Username for Connection request should not be empty!");
		return false;
	}
	
	value1 = document.CWMP_form.CWMP_ConnectionRequestPassword.value;
	if(!StringCheck(value1)){
		alert("Password for Connection request should not be empty!");
		return false;
	}
	*/
	return true;
}

function cwmpSave()
{
	
  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to save these configurations?"))
			return;
	<%end if%>	

	if(document.CWMP_form.elements[0].checked){
		if(!cwmpNumValidCheck())
			return false;
		if(!cwmpStringValidCheck())
			return false;
		<%if tcwebApi_get("WebCustom_Entry","isTR69BindPVC","h") = "Yes" then%>
		if(!cwmpBindPVCCheck())
			return false;
		<%end if%>
	}
	
	document.CWMP_form.Cwmpflag.value=1;	
	document.CWMP_form.submit();

}
function doCWMPLock()
{
	for(i = 0; i < document.forms[0].elements.length; i++)
	{	
		document.forms[0].elements[i].disabled = true;
	}
}
<%if tcwebApi_get("WebCustom_Entry","isTR69BindPVC","h") = "Yes" then%>
function cwmpBindSwitch(onOff){
	 		
	if(onOff == 0)
		document.CWMP_form.bind_PVC.disabled = true;
	else
		document.CWMP_form.bind_PVC.disabled = false;
}

function cwmpBindPVCCheck()
{
var form=document.CWMP_form; 
if(form.CWMP_BindActive[1].checked)
	return true;
 var activePVC = ["<% tcWebApi_staticGet("Wan_PVC0","Active","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC1","Active","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC2","Active","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC3","Active","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC4","Active","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC5","Active","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC6","Active","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC7","Active","s") %>"];
 var ispPVC = ["<% tcWebApi_staticGet("Wan_PVC0","ISP","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC1","ISP","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC2","ISP","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC3","ISP","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC4","ISP","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC5","ISP","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC6","ISP","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC7","ISP","s") %>"];
	var pvc_index = parseInt(document.CWMP_form.bind_PVC.value);
	if(activePVC[pvc_index]!="Yes")
	{
		alert("The PVC is not active");
		return false;
	}
	if(ispPVC[pvc_index]=="3")
	{
		alert("The PVC is bridge");
		return false;
	}
	return true;
}

<%end if%>

</script>
</head>
<body onload="cwmpinit()">

<form name="CWMP_form" method="post" ACTION="/cgi-bin/access_cwmp.asp" >
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
      <td width="150" height="30" class="title-main"> 
        CWMP Setup 
      </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="150"></td>
    <td width="9" ></td>
    <td width="441"></td>
  </tr>
  <tr>
    <td height="25" class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    CWMP
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
      <input name="CWMP_Active" type="radio" value="Yes" <%if tcWebApi_get("Cwmp_Entry", "Active", "h") = "Yes" then asp_Write("checked") end if%> onclick="cwmpSwitch(1)">
      <font color="#000000">Activated</font>
	  <input name="CWMP_Active" type="radio" value="No" <%if tcWebApi_get("Cwmp_Entry", "Active", "h") = "No" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry", "Active", "h") = "N/A" then asp_Write("checked") end if%> onclick="cwmpSwitch(0)">
   	  <font color="#000000">Deactivated</font>
     </td>
  </tr>
  <tr>
      <td height="25" class="title-sub"> 
        ACS Login Information 
      </td>
    <td class="light-orange"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td class="tabdata"><div align="right" id="ACSURLId"> 
          URL 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ACSURL" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsUrl", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "acsUrl", "s") end if%>" size="64" maxlength="256">
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="ACSUserNameId"> 
          User Name
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ACSUserName" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsUserName", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry",  "acsUserName", "s") end if%>" size="32" maxlength="256">
       	</td>
  </tr>
  </tr>
  <tr>
    <td class="light-orange"><hr noshade class="light-orange-line"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="ACSPasswordId"> 
          Password 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
<% If tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" Then%>
	<input name="CWMP_ACSPassword" type="PASSWORD" value="" size="32" maxlength="256">
<%else%>
        <input name="CWMP_ACSPassword" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "acsPassword", "s") end if%>" size="32" maxlength="256">
<% end if %>
	 </td>
  </tr>
   <tr>
      <td height="25" class="title-sub"> 
        Connection Request Information 
      </td>
    <td class="light-orange"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
          Path 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ConnectionRequestPath" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqPath", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqPath", "s") end if%>" size="63" maxlength="256">
	</td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="CWMPConnectionRequestUserNameId"> 
          UserName 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ConnectionRequestUserName" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqUserName", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqUserName", "s") end if%>" size="32" maxlength="256">
	 </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="CWMPConnectionRequestPasswordId"> 
          Password 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
<% If tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" Then%>
        <input name="CWMP_ConnectionRequestPassword" type="PASSWORD" value="" size="32" maxlength="256">
<%else%>
        <input name="CWMP_ConnectionRequestPassword" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqPassword" ,"s") end if%>" size="32" maxlength="256">
<% end if %>
	 </td>
	</td>
  </tr>
  <tr>
      <td height="25" class="title-sub"> 
        Periodic Inform Config 
      </td>
    <td class="light-orange"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
          Periodic Inform 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
      <input name="CWMP_PeriodActive" value="Yes" type="radio" <%if tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "Yes" then asp_Write("checked") end if%> onclick="cwmpPeriodSwitch(1)">
  		Activated
 	  <input name="CWMP_PeriodActive" value="No" type="radio" <%if tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "No" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "N/A" then asp_Write("checked") end if%> onclick="cwmpPeriodSwitch(0)">
 		Deactivated
    </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
          Interval 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
		<input type="text" name="CWMP_PeriodInterval" value="<%if tcWebApi_get("Cwmp_Entry", "periodInterval", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "periodInterval", "s") end if%>" size="9" maxlength="9" >
     </td>
  </tr>

  </tr>
<%if tcwebApi_get("WebCustom_Entry","isTR69BindPVC","h") = "Yes" then%>
<tr>
      <td height="25" class="title-sub"> 
        Cwmp Binding 
      </td>
    <td class="light-orange"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
 </tr>
 <tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
          Status 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
      <input name="CWMP_BindActive" value="Yes" type="radio" <%if tcWebApi_get("Cwmp_Entry", "BindActive", "h") = "Yes" then asp_Write("checked") end if%> onclick="cwmpBindSwitch(1)">
  		Activated
 	  <input name="CWMP_BindActive" value="No" type="radio" <%if tcWebApi_get("Cwmp_Entry", "BindActive", "h") = "No" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry", "BindActive", "h") = "N/A" then asp_Write("checked") end if%> onclick="cwmpBindSwitch(0)">
 		Deactivated
    </td>
  </tr>
  <tr>
    <td height="40" class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
           PVC Index
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
      <td class="tabdata"> <select name="bind_PVC" size="1" >
          <option value="0" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "0" then asp_Write("selected") end if %>>PVC_0 
          <option value="1" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "1" then asp_Write("selected") end if %>>PVC_1 
          <option value="2" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "2" then asp_Write("selected") end if %>>PVC_2 
          <option value="3" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "3" then asp_Write("selected") end if %>>PVC_3 
          <option value="4" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "4" then asp_Write("selected") end if %>>PVC_4 
          <option value="5" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "5" then asp_Write("selected") end if %>>PVC_5 
          <option value="6" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "6" then asp_Write("selected") end if %>>PVC_6 
          <option value="7" <% if tcWebApi_get("Cwmp_Entry","wan_PVC","h") = "7" then asp_Write("selected") end if %>>PVC_7 
        </select></td>
	
  </tr>
<%end if%>
 </table>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >
	<input name="CWMPLockFlag" type="HIDDEN" value="<%if tcWebApi_get("Cwmp_Entry", "CWMPLockFlag", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "CWMPLockFlag", "s") else asp_write("0") end if%>">
	</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="Cwmpflag" VALUE="0">
		<input name="cwmp_apply" type="button" value="APPLY" onClick="cwmpSave()" class="sbutton">
      	<input type="reset"  name="cwmp_cancel" value="CANCEL">
   	</td>
  </tr>
</table>
</div>
</form>
</body>
</html>        
