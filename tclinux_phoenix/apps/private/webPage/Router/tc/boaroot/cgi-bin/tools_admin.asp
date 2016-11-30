<%
If Request_Form("adminFlag") = "1" Then
	TCWebApi_set("Account_Entry0","web_passwd","uiViewTools_Password")
	TCWebApi_set("Account_Entry0","console_passwd","uiViewTools_Password")
	if tcwebApi_get("WebCustom_Entry","isThreeLevelChangePassWord","h") = "Yes" Then
	TCWebApi_set("Account_Entry1","web_passwd","Password2")
	TCWebApi_set("Account_Entry2","web_passwd","Password3")
	end if
	tcWebApi_commit("Account_Entry0")
	if  tcwebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" then
		TCWebApi_set("Account_Entry0","PWChangeFlag","adminFlag")
	end if
End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" src="OutVariant.asp"></script>
<script language="JavaScript" src="/jsl.js"></script>
<script language="JavaScript" src="/ip.js"></script>

<script language="JavaScript">
function fresh()
{
<%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" then%>
	<%if Request_Form("adminFlag")="1" then%>
		window.location = "/cgi-bin/tools_admin.asp";
	<%end if%>
<%end if%>
<%if tcwebApi_get("WebCustom_Entry","isThreeLevelChangePassWord","h") = "Yes" then%>
<%if TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") = "0" then %>
	if(document.tool_admin.uiViewTools_Password != null)
		document.tool_admin.uiViewTools_Password.value = adminPwd;
	if(document.tool_admin.uiViewTools_PasswordConfirm != null)
		document.tool_admin.uiViewTools_PasswordConfirm.value = adminPwd;
<%end if%>
<%if TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") <> "2" then%>
	if(document.tool_admin.Password2 != null)
		document.tool_admin.Password2.value = userPwd;
	if(document.tool_admin.PasswordConfirm2 != null)
		document.tool_admin.PasswordConfirm2.value = userPwd;
<%end if%>
	if(document.tool_admin.Password3 != null)
		document.tool_admin.Password3.value = user2Pwd;
	if(document.tool_admin.PasswordConfirm3 != null)
		document.tool_admin.PasswordConfirm3.value = user2Pwd;
<%end if%>	
}
function uiSave() {
	
  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to save these configurations?"))
			return;
	<%end if%>	
		
 <%if tcwebApi_get("WebCustom_Entry","isThreeLevelChangePassWord","h") = "Yes" then%>   
if(document.tool_admin.CurrentAccess.value == "0")
{
<%end if%>
    if (document.tool_admin.uiViewTools_Password.value.length == 0) {
    	alert("Empty Password Invaild. ");
    	return;
    }
	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
	var password = document.tool_admin.uiViewTools_Password.value;
	if (password.length < 8 || password.length > 30) {
    	alert("Password Invalid.The length of password should be 8~30 character(s)!");
		return -1;
	}
	
	for(var i = 0; i < password.length; i ++ )
	{
	   var ascNum = password.charCodeAt(i);
	   if (ascNum < 33 || ascNum > 126 || ascNum == 59){
   			alert("Password Invalid.");
			return -1;
		}
	}
	<%end if%>
    if (document.tool_admin.uiViewTools_Password.value != document.tool_admin.uiViewTools_PasswordConfirm.value) {
        alert("Your Password and Confirm Password must match before you can apply. ");
        return;
    }
    if(quotationCheck(document.tool_admin.uiViewTools_Password, 30) ) 
			return;
	
 <%if tcwebApi_get("WebCustom_Entry","isThreeLevelChangePassWord","h") = "Yes" then%>   
}

	if(document.tool_admin.CurrentAccess.value != "2")
	{
		if (document.tool_admin.Password2.value.length == 0) {
    		alert("Empty Password Invaild. ");
    		return;
   		}
	 	if (document.tool_admin.Password2.value != document.tool_admin.PasswordConfirm2.value) {
        	alert("Your Password and Confirm Password must match before you can apply. ");
       		 return;
   		}
    	if(quotationCheck(document.tool_admin.Password2, 30) ) 
			return;
	
	}

	if (document.tool_admin.Password3.value.length == 0) {
   		alert("Empty Password Invaild. ");
   		return;
  	}
	if (document.tool_admin.Password3.value != document.tool_admin.PasswordConfirm3.value) {
      	alert("Your Password and Confirm Password must match before you can apply. ");
      	 return;
   	}
    if(quotationCheck(document.tool_admin.Password3, 30) ) 
		return;
 <%end if%>
 	document.tool_admin.adminFlag.value=1;
    document.tool_admin.submit();
    return;
}

function quotationCheck(object, limit_len) {
	var len = object.value.length;
	var c;
	var i, j = 0;
    for (i = 0; i < len; i++)
    {
	 	var c = object.value.charAt(i);
      
	  	if (c == '"')
		{
			j += 6;
		}
	<%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" then%>
		else if(c == ';')
		{
			alert("Password can not contain ';'!!");  
			return true;
		}
	<%end if%> 
		else
			j++;
    }
   	if (j > limit_len)
	{
   	alert('Input too many character!!');      	  								    	    	   		
		return true;
	}	
	return false;
}
</script>
</head><body onLoad="fresh()">
<FORM METHOD="POST" ACTION="/cgi-bin/tools_admin.asp" name="tool_admin">
<INPUT TYPE="HIDDEN" NAME="adminFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="CurrentAccess" VALUE=<%TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "s")%>>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
 </tr>
 <tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">Administrator</font></td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"> </td><td width="440"></td>
</tr>

<%if tcwebApi_get("WebCustom_Entry","isThreeLevelChangePassWord","h") = "Yes" then%>
<%if TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") = "0" then %>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Username</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" style="word-wrap:break-word;word-break:break-all;"><strong>
    <font color="#000000">
	<% tcWebApi_get("Account_Entry0","username","s") %>
	</font>    </strong></td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">New Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="uiViewTools_Password" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Confirm Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="uiViewTools_PasswordConfirm" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>


<%end if %>
<%else%>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Username</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" style="word-wrap:break-word;word-break:break-all;"><strong>
    <font color="#000000">
	<% tcWebApi_get("Account_Entry0","username","s") %>
	</font>    </strong></td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">New Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="uiViewTools_Password" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Confirm Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="uiViewTools_PasswordConfirm" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>
<%end if%>
<%if tcwebApi_get("WebCustom_Entry","isThreeLevelChangePassWord","h") = "Yes" then%>
<%if TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") <> "2" then%>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Username</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" style="word-wrap:break-word;word-break:break-all;"><strong>
    <font color="#000000">
	<% tcWebApi_get("Account_Entry1","username","s") %>
	</font>    </strong></td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">New Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="Password2" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Confirm Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="PasswordConfirm2" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>

<%end if%>

<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Username</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" style="word-wrap:break-word;word-break:break-all;"><strong>
    <font color="#000000">
	<% tcWebApi_get("Account_Entry2","username","s") %>
	</font>    </strong></td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">New Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="Password3" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Confirm Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="PasswordConfirm3" SIZE="32" MAXLENGTH="30" VALUE="">    </td>
</tr>

<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>

<%end if%>


</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="uiSave()">        <INPUT TYPE="BUTTON" NAME="CancelBtn" VALUE="CANCEL" onClick="javascript:window.location='/cgi-bin/tools_admin.asp'">        </td></tr></table></div></form>
</body></html>
