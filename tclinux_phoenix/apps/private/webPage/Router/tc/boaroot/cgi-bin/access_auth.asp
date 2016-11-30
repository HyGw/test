<%
If Request_Form("AuthFlag")="1" then 
	TCWebApi_set("EPON_LOIDAuth","LOID0","Auth_LOID")
	TCWebApi_set("EPON_LOIDAuth","Password0","Auth_Password")
	tcWebApi_commit("EPON_LOIDAuth")
	
	TCWebApi_set("GPON_LOIDAuth","LOID","Auth_LOID")
	TCWebApi_set("GPON_LOIDAuth","Password","Auth_Password")
	tcWebApi_commit("GPON_LOIDAuth")
	TCWebApi_save();
	TCWebApi_set("System_Entry","reboot_type","AuthFlag") 
	tcWebApi_CommitWithoutSave("System_Entry")
End If
%>

<!--
If you execute doRefresh() and then reload webpage, doRefresh() will be disabled.
Therefore, execute doRefresh() after webpage reloads.
-->
<%If Request_Form("SaveBtn") = "APPLY"  Then%>
<script language="JavaScript">
	setTimeout("doRefresh()",70000);
</script>
<%End If%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">

<script language="JavaScript" src="/val.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript">

function doNonSympolCheck(c)
{
	if ((c >= "0")&&(c <= "9"))
	{
		return 1;
	}
	else if ((c >= "A")&&(c <= "Z"))
	{
		return 1;
	}
	else if ((c >= "a")&&(c <= "z"))
	{
		return 1;
	}

  return -1;
}

function PasswdCheck(object)
{
	var len = object.value.length;
	var c;
	var i = 0;
	for(i=0; i<len; i++)
	{
		var c = object.value.charAt(i);
		if(doNonSympolCheck(c)==-1)
		{
			alert("passwd include invalid character:  " + c);
			return true;
		}
	}
	return false;
}

function AuthSave() 
{

	if(document.AUTH_form.Auth_LOID.value.length <= 0 || document.AUTH_form.Auth_Password.value.length <= 0 ){
		alert("Both of the fields should be filled");
		return false;
	}

	if (document.AUTH_form.Auth_LOID.value.length >= 24){
		alert("EPON LOID length can't more than 24!");
		return false;
	}
	if (document.AUTH_form.Auth_Password.value.length >= 12){
		alert("EPON Password length can't more than 12!");
		return false;
	}
	if (PasswdCheck(document.AUTH_form.Auth_Password)){
		alert("EPON Password contain invalid character!");
		return false;
	}

	alert("<%tcWebApi_get("String_Entry","PonAuthJS6Text","s")%>");
	document.AUTH_form.AuthFlag.value=1;
	document.AUTH_form.submit();
}

function doRefresh()
{
	alert("<%tcWebApi_get("String_Entry","PonAuthJS7Text","s")%>");
	var mydate = new Date();
	mydate.setTime(mydate.getTime() - 1);
	document.cookie = "SESSIONID=; expires=" + mydate.toGMTString(); 
	window.top.location.href = "../";    
}

</script>
</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/access_auth.asp" name="AUTH_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">LOID</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"></td>
		<td width="10" ></td>
		<td width="440"></td>
	</tr>
	<tr>		
		<td width="150" height="25" class="light-orange"></td>
		<td width="10" class="light-orange"></td>
		<td class="tabdata"><div align=right><img src="/exclamation.gif"></div></td>
		<td>&nbsp;</td>
		<td class="tabdata">
		<font color="#000000"><%tcWebApi_get("String_Entry","PonAuthWarningText","s")%></font>
		</td>
	</tr>
	<tr>		
		<td width="150" height="25" class="light-orange"></td>
		<td width="10" class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">LOID</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="Auth_LOID" SIZE="30" MAXLENGTH="15" VALUE="<% If TCWebApi_get("EPON_LOIDAuth","LOID0","h") <> "N/A" then TCWebApi_get("EPON_LOIDAuth","LOID0","s") end if%>" >
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Password</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="Auth_Password" SIZE="30" MAXLENGTH="15" VALUE="<% If TCWebApi_get("EPON_LOIDAuth","Password0","h") <> "N/A" then TCWebApi_get("EPON_LOIDAuth","Password0","s") end if%>" >
		</td>
	</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
	<td width="160" class="light-orange">&nbsp;</td><td width="160">&nbsp;</td><td width="440"></td></tr>
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="AuthFlag" VALUE="0">
		<INPUT TYPE="SUBMIT" NAME="SaveBtn" VALUE="SAVE" onClick="AuthSave();">	</td></tr></table></form>
</body></html>
