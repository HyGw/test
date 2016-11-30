<% 
	if request_form("WZTrue_IUP_Flag") = "1" then			
		tcWebApi_set("Wan_PVC0","USERNAME","WZTrue_IUP_UsrnameInput")
		tcWebApi_set("Wan_PVC0","PASSWORD","WZTrue_IUP_PassInput")
		if request_form("WZTrue_IUP_ConnRadio") = "Connect_Keep_Alive" then
			tcWebApi_set("Wan_PVC0","CONNECTION","WZTrue_IUP_ConnRadio")
		else
			tcWebApi_constSet("Wan_PVC0","CONNECTION","Connect_Manually")
		end if
		tcWebApi_save()
		tcWebApi_commit("Wan_PVC0")
	end if
%>
<HTML><HEAD><TITLE></TITLE>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT language=JavaScript>
// skip the wifi saving when the wifi is not enabled
<% if tcWebApi_get("Info_WLan","isExist","h") <> "On" then %>
	top.location.href = "/cgi-bin/wzWait2_true.asp?WZTrue_ExitFlag_1=0&skipWifi_Flag=1";
<% End If %>

function doLoad()
{
}

function strSSIDCheck(str) 
{
	if(str.value.match(/[^\x00-\xff]/g))
	{
		alert("Invalid SSID Input!");return true;
	}
	return false;
}

function wepkeyCheck(object) {
	var keyvalue=object.value;
	var wpapsklen=object.value.length;
	
   	if(wpapsklen >= 8 && wpapsklen < 64) {
    	if(keyvalue.match(/[^\x00-\xff]/g))
   	    {
			alert("Pre-Shared Key should be between 8 and 63 ASCII characters or 64 Hex string.");
			return true;
   	    }
	}else if(wpapsklen==64){
		for(i=0;i<64;i++){
			var c=keyvalue.charAt(i);
			if(doHexCheck(c)<0){
				alert('Pre-Shared Key Hex value error!');
				return true;
			}
		}
	}else {
    	alert('Pre-Shared Key length error!');
		return true;
	}			
	return false;
}

function onClickExitBtn() 
{
	var thisForm = document.forms[0];
	if (confirm("Quit setup wizard and discard settings ?")) 
	{
		thisForm.WZTrue_ExitFlag_1.value = 1;
		self.opener=null; 
		self.open('','_self');  
		self.close();
		setTimeout("alert('Your current browser can not close the window, please close it by yourself')", 3);
	}
}

function onClickNextBtn() 
{
	var thisForm = document.forms[0];
	if (wepkeyCheck(thisForm.wireless_lan_Wepkey)== true)
	{
		return;
	}
	
	thisForm.WZTrue_ExitFlag_1.value = 0;
	thisForm.submit();
}

function onClickBackBtn()
{
	window.location = "/cgi-bin/wzIspUserPwd_true.asp";
}
</SCRIPT>
</HEAD>
<BODY leftMargin=0 topMargin=0 onLoad="doLoad();">
<FORM name="wireless_lanForm" action="/cgi-bin/wzWait2_true.asp" method="post">
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
				It is the Wireless Lan Page title, you can see what you want to see 
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=2 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD width=40>&nbsp;</TD>
			<TD class=tabdata colSpan=2>
				Wireless LAN Setup 
			</TD>
		</TR>
		<TR>
			<TD height=10></TD>
			<TD width=150 align="right">
				Wireless:
			</TD>
			<TD>
				<input type=radio <%if tcWebApi_get("WLan_Common","APOn","h") = "0" then asp_Write("checked") end if%> value=0 name=wireless_lan_wirelessRadio> 
				Disabled 
				<input type=radio <%if tcWebApi_get("WLan_Common","APOn","h") = "1" then asp_Write("Checked") end if%> value=1 name=wireless_lan_wirelessRadio> 
				Enabled 
			</TD>
		</TR>
		<TR>
			<TD>&nbsp;</TD>
			<TD class=tabdata align="right">
				SSID:
			</TD>
			<TD class=tabdata align=left>
				<input size="32" maxlength="32" value="<%tcWebApi_get("WLan_Entry0","SSID","s")%>" name=wireless_lanSSID onBlur="strSSIDCheck(this);"> 
			</TD>
		</TR>
		<TR>
			<TD>&nbsp;</TD>
			<TD class=tabdata align=right>CH:</TD>
			<TD class=tabdata align=left>
				<select size=1 name=wireless_lan_CH style="width = 60px">
					<option value="0" <% if tcWebApi_get("WLan_Common","Channel","h") = "0" then asp_Write("selected") end if %>>Auto</option> 
					<option value="1" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "1" then asp_Write("selected") end if %>>1</option> 
					<option value="2" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "2" then asp_Write("selected") end if %>>2</option> 
					<option value="3" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "3" then asp_Write("selected") end if %>>3</option> 
					<option value="4" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>4</option> 
					<option value="5" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "5" then asp_Write("selected") end if %>>5</option> 
					<option value="6" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "6" then asp_Write("selected") end if %>>6</option> 
					<option value="7" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "7" then asp_Write("selected") end if %>>7</option> 
					<option value="8" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "8" then asp_Write("selected") end if %>>8</option> 
					<option value="9" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "9" then asp_Write("selected") end if %>>9</option> 
					<option value="10" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10</option> 
					<option value="11" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11</option> 
					<option value="12" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "12" then asp_Write("selected") end if %>>12</option> 
					<option value="13" <% if tcWebApi_staticGet("WLan_Common","Channel","h") = "13" then asp_Write("selected") end if %>>13</option> 
				</select> 
				&nbsp; 
			</TD>
		</TR>
		<TR>
			<TD height=10></TD>
			<TD width=150 align="right">
				Encryption: 
			</TD>
			<TD>
				<input type=radio <%if tcWebApi_get("WLan_Entry0","AuthMode","h") = "OPEN" then asp_Write("checked") end if%> value=0 name=wireless_lan_EncryptionRadio> 
				Disabled 
				<input type=radio <%if tcWebApi_get("WLan_Entry0","AuthMode","h") <> "OPEN" then asp_Write("checked") end if%> value=1 name=wireless_lan_EncryptionRadio> 
				Enabled 
			</TD>
		</TR>
		<TR>
			<TD>&nbsp;</TD>
			<TD class=tabdata align=right>
				Encryption Type:
			</TD>
			<TD class=tabdata align=left width=310>
				<select size=1 name=wireless_lan_KeyType style="width = 60px">
				<OPTION value="AES" <% if tcWebApi_get("WLan_Entry0","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES 
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry0","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry0","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES	
				</select>
			</TD>
		</TR>
		<TR>
			<TD height=25>&nbsp;</TD>
			<TD class=tabdata align=right>
				Pre-Shared Key:
			</TD>
			<TD class=tabdata align=left>
				<input maxlength=13 size=13 value="<%If tcWebApi_get("WLan_Entry0","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry0","WPAPSK","s") end if%>" name=wireless_lan_Wepkey onBlur="wepkeyCheck(this);">
				<span id="str1">
				(8~63 characters or 64 Hex string)
				</span></TD>
		</TR>
		<TR>
			<TD width=40>&nbsp;</TD>
			<TD class=tabdata colSpan=2></TD></TR>
			<TR>
			<TD>&nbsp;</TD>
			<TD>&nbsp;</TD>
			<TD height=15>
			<input type="hidden" name="WZTrue_ExitFlag_1" value="0">
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
</FORM>
</BODY>
</HTML>
</HTML>
