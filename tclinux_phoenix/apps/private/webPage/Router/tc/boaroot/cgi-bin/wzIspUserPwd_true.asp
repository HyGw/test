�?HTML><HEAD><TITLE></TITLE>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT language=JavaScript>
function onClickExitBtn() 
{
    if (confirm("Quit setup wizard and discard settings ?")) 
	{
		self.opener=null; 
		self.open('','_self');  
		self.close();
		setTimeout("alert('Your current browser can not close the window, please close it by yourself')", 3);
	}
}
function haveSpace(str)
{
	var i;
	for(i = 0; i < str.length; i++)
	{
		if(str.charAt(i) == ' ')
		{
			return true;
		}
	}
	return false;
}
function checkUsrname(usrname)
{
	if(usrname == "")
	{
		alert("User name can't be NULL!");return false;
	}
	else
	{
		if(haveSpace(usrname) == true)
		{
			alert("Username can't contain space!");return false
		}
	}
	return true;
}
function checkPassword(password)
{
	if(password == "")
	{
		alert("Password can't be NULL!");return false;
	}
	else
	{
		if(haveSpace(password) == true)
		{
			alert("Password can't contain space!");return false
		}
	}
	return true;
}
function onClickNextBtn() 
{
	var thisForm = document.forms[0];
	if((checkUsrname(thisForm.WZTrue_IUP_UsrnameInput.value) == false)||
	(checkPassword(thisForm.WZTrue_IUP_PassInput.value) == false))
	{
		return;
	}
	thisForm.WZTrue_IUP_Flag.value = 1;
	thisForm.submit();
}

function onClickBackBtn()
{
	window.location = "/cgi-bin/wzExistAccount_true.asp";
}

</SCRIPT>
<META content="MSHTML 6.00.2900.5726" name=GENERATOR></HEAD>
<BODY leftMargin=0 topMargin=0>
<form name="WZTrue_IUP_Form" method="post" action="/cgi-bin/wzWiFi_true.asp">
<TABLE cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange vAlign=baseline width=500 height=5></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD vAlign=center align=middle width=200 bgColor=#ffffff height=50>
			<DIV align=left>
			<img src="/logo.gif" tppabs="logo.gif" width="200" height="50">
			</DIV>
			</TD>
			<TD class=model vAlign=bottom align=right width=300 bgColor=#ffffff></TD>
		</TR>
		<TR>
			<TD class=orange width=500 colSpan=2></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE class=orange height=2 cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE height=35 cellSpacing=0 cellPadding=0 width=500 align=center border=0 color="#FF9933">
	<TBODY>
		<TR>
			<TD class=headline width=500 bgColor=#ffffff>
				&nbsp; Quick Start-PPPoE <%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then %>/PPPoA <%end if%>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=2 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD width=40>&nbsp;</TD>
			<TD class=tabdata colSpan=2>
				Enter the PPPoE/PPPoA information provided to you by your ISP.Click <B>NEXT</B> to continue.
			</TD>
		</TR>
		<TR>
			<TD height=10></TD>
			<TD width=150></TD>
			<TD>
			</TD>
		</TR>
		<TR>
			<TD>&nbsp;</TD>
			<TD class=tabdata align=right>Username:</TD>
			<TD class=tabdata align=left>
			<INPUT maxLength=64 size=32 name=WZTrue_IUP_UsrnameInput value="<% if tcWebApi_get("Wan_PVC0","USERNAME","h")<>"N/A" then tcWebApi_staticGet("Wan_PVC0","USERNAME","s")end if %>">
			</TD>
		</TR>
		<TR>
			<TD>&nbsp;</TD>
			<TD class=tabdata align=right>Password:</TD>
			<TD class=tabdata align=left>
			<INPUT type=password maxLength=64 size=32 name=WZTrue_IUP_PassInput value="<% if tcWebApi_get("Wan_PVC0","PASSWORD","h")<>"N/A" then tcWebApi_staticGet("Wan_PVC0","PASSWORD","s") end if %>">
			</TD>
		</TR>
		<TR>
			<TD height=10></TD>
			<TD width=150></TD>
			<TD></TD>
		</TR>
		<TR>
			<TD width=40>&nbsp;</TD>
			<TD class=tabdata colSpan=2></TD>
		</TR>
		<TR>
			<TD>&nbsp;</TD>
			<TD class=tabdata align=right>Connection</TD>
			<TD class=tabdata align=left width=310>
			<INPUT TYPE="RADIO" NAME="WZTrue_IUP_ConnRadio" VALUE="Connect_Keep_Alive" checked>Always on
			</TD>
		</TR>
		<TR>
			<TD height=25>&nbsp;</TD>
			<TD class=tabdata align=right></TD>
			<TD class=tabdata align=left>
			<INPUT TYPE="RADIO" NAME="WZTrue_IUP_ConnRadio" VALUE="Connect_Manually">Manual
			</TD>
		</TR>
		<TR>
			<TD width=40>&nbsp;</TD>
			<TD class=tabdata colSpan=2></TD></TR>
			<TR>
			<TD>&nbsp;</TD>
			<TD>&nbsp;</TD>
			<TD height=15>
			<input name="WZTrue_IUP_Flag" type="hidden" value="0">
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE class=orange height=40 cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange>
				<DIV align=right>
				<INPUT class=tabdata onclick=onClickBackBtn() type=button value=BACK name=WZTrue_BackBtn >
				<INPUT class=tabdata onclick=onClickNextBtn() type=button value=NEXT name=WZTrue_IUP_NextBtn >
				<INPUT class=tabdata onclick=onClickExitBtn() type=button value=EXIT name=WZTrue_IUP_ExitBtn >
				</DIV>
			</TD>
			<TD class=orange vAlign=baseline width=25>&nbsp;</TD>
		</TR>
	</TBODY>
</TABLE>
</FORM></BODY></HTML>
</FORM></BODY></HTML>