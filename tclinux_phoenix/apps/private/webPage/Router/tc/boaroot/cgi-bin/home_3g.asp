<%
If request_Form("dongle_save_flag") = "1" Then
	tcWebApi_set("Dongle_Entry","Active","dongle_status")
	tcWebApi_set("Dongle_Entry","BACKUP","dongle_backup")
	tcWebApi_set("Dongle_Entry","LOCATION","dongle_location")
	tcWebApi_set("Dongle_Entry","ISP","dongle_isp")
	tcWebApi_set("Dongle_Entry","DEFAULTAPN","dongle_default_cfg")
	tcWebApi_set("Dongle_Entry","APN","dongle_APN_hidden")
	tcWebApi_set("Dongle_Entry","TELEPHONE","dongle_TELEPHONE_hidden")
	tcWebApi_set("Dongle_Entry","USERNAME","dongle_username_hidden")
	tcWebApi_set("Dongle_Entry","PASSWORD","dongle_password_hidden")
	if tcwebApi_get("WebCustom_Entry","isUSB3GDonglePINSupport","h") = "Yes" then
		tcWebApi_set("Dongle_Entry","PIN","dongle_PIN")
	end if	
	tcWebApi_set("Dongle_Entry","CONNECTION","dongle_connect_type")
	tcWebApi_set("Dongle_Entry","CLOSEIFIDLE","dongle_idle_time")
	tcWebApi_set("Dongle_Entry","DEFAULTROUTE","dongle_default_route")
	tcWebApi_set("Dongle_Entry","NATENABLE","dongle_NAT_enable")
	tcWebApi_set("Dongle_Entry","MTU","dongle_TCP_MTU")
	
	tcWebApi_set("Dongle_Entry", "AUTHEN", "PPPAuthen")

	tcWebApi_commit("Dongle_Entry")
End If
%>
<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language="JavaScript" src="/val.js"></script>
<script language="JavaScript" src="/mac.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
</head>
<script language="JavaScript">
function isInteger(value)
{   	
	if (/^\d+$/.test(value)) 
	{
	   return true;
	} 
	else 
	{
	    return false;
	}
}
function doLoad()
{
	init_default_cfg();
	default_cfg_chg();
	connect_type_chg();
}

function init_default_cfg()
{
	document.Alpha_3G.dongle_TELEPHONE_hidden.value = document.Alpha_3G.dongle_TELEPHONE_def_hidden.value;	//"*99#"
	document.Alpha_3G.dongle_APN_hidden.value = document.Alpha_3G.dongle_APN_def_hidden.value;	//"3gnet"
	document.Alpha_3G.dongle_username_hidden.value = document.Alpha_3G.dongle_username_def_hidden.value;	// ""
	document.Alpha_3G.dongle_password_hidden.value = document.Alpha_3G.dongle_password_def_hidden.value;	// ""	
}
function cp2Hidden()
{
	document.Alpha_3G.dongle_TELEPHONE_hidden.value = document.Alpha_3G.dongle_TELEPHONE.value;
	document.Alpha_3G.dongle_APN_hidden.value = document.Alpha_3G.dongle_APN.value;
	document.Alpha_3G.dongle_username_hidden.value = document.Alpha_3G.dongle_username.value;
	document.Alpha_3G.dongle_password_hidden.value = document.Alpha_3G.dongle_password.value;
}
function cp2Text()
{
	document.Alpha_3G.dongle_TELEPHONE.value = document.Alpha_3G.dongle_TELEPHONE_hidden.value;
	document.Alpha_3G.dongle_APN.value = document.Alpha_3G.dongle_APN_hidden.value;
	document.Alpha_3G.dongle_username.value = document.Alpha_3G.dongle_username_hidden.value;
	document.Alpha_3G.dongle_password.value = document.Alpha_3G.dongle_password_hidden.value;
}

function default_cfg_chg()
{
	var value = document.Alpha_3G.dongle_default_cfg[0].checked;
	if (value)
	{
		document.Alpha_3G.dongle_TELEPHONE.disabled = true;
		document.Alpha_3G.dongle_APN.disabled = true;
		document.Alpha_3G.dongle_username.disabled = true;
		document.Alpha_3G.dongle_password.disabled = true;
		init_default_cfg();
		cp2Text();
	}
	else
	{
		document.Alpha_3G.dongle_TELEPHONE.disabled = false;
		document.Alpha_3G.dongle_APN.disabled = false;
		document.Alpha_3G.dongle_username.disabled = false;
		document.Alpha_3G.dongle_password.disabled = false;
		cp2Hidden();
	}
}

function connect_type_chg()
{
	var value = document.Alpha_3G.dongle_connect_type[1].checked;
    if (value)
        document.Alpha_3G.dongle_idle_time.disabled = false;
    else
    	document.Alpha_3G.dongle_idle_time.disabled = true;
}

function uiSave()
{
	var form=document.Alpha_3G;
	var MTU = parseInt(form.dongle_TCP_MTU.value);
	
  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to save these configurations?"))
			return;
	<%end if%>	
	
	<%if tcwebApi_get("WebCustom_Entry","isUSB3GDonglePINSupport","h") = "Yes" then%>
	var PIN = form.dongle_PIN.value;
	var PINConfirm = form.dongle_PINConfirm.value;
	var PINLen = PIN.length;
	var i;
	
	if( PINLen < 4 || PINLen > 8 )
	{
		alert("The length of PIN Code should be between 4 and 8.");
		return false;		
	}
	for( i=0; i<PINLen; i++ )
	{
		if( PIN.charAt(i) < '0' || PIN.charAt(i) > '9' )	
		{
			alert("The PIN Code should be composed of digits.");
			return false;
		}	
	}
	if (PIN != PINConfirm) 
	{
    alert("Your PIN and Confirm PIN must match before you can apply. ");
    return false;
  }
  <%end if%>
  if (document.Alpha_3G.dongle_connect_type[1].checked)
	{       
  	var Idletime = form.dongle_idle_time.value;
  	if(!isInteger(Idletime))
		{
			alert('Idletime must be a Non-negative Interger');
			return false;
		}
	}	
	if((!isInteger(form.dongle_TCP_MTU.value))||((MTU > 1492 || MTU < 100) && MTU!=0))
	{
		alert("The range of TCP MTU: 100~1492 or 0 as default value");
		return false;
	}
	form.dongle_save_flag.value = 1;
	/*before click save btn, fill correct hidden value*/
	default_cfg_chg();
	form.submit();
}
</script>

<body>
<FORM METHOD="POST" ACTION="/cgi-bin/home_3g.asp" name="Alpha_3G">
<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">3G Status</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"></td>
		<td width="10" ></td>
		<td width="440"></td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Status</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="HIDDEN" NAME="dongle_save_flag" VALUE="0">
			<INPUT TYPE="RADIO" NAME="dongle_status" VALUE="Yes" <% if tcWebApi_get("Dongle_Entry","Active","h") = "Yes" then asp_Write("checked") end if %> ><font color="#000000">Activated</font>
			<INPUT TYPE="RADIO" NAME="dongle_status" VALUE="No" <% if tcWebApi_get("Dongle_Entry","Active","h") = "No" then asp_Write("checked") end if if tcWebApi_get("Dongle_Entry","Active","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font>
		</td>
	</tr>
	<%if tcWebApi_get("WebCustom_Entry","is3GBackupButtonSupport","h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">3G BackUp</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="dongle_backup" VALUE="Yes" <% if tcWebApi_get("Dongle_Entry","BACKUP","h") = "Yes" then asp_Write("checked") end if %> ><font color="#000000">Enabled</font>
			<INPUT TYPE="RADIO" NAME="dongle_backup" VALUE="No" <% if tcWebApi_get("Dongle_Entry","BACKUP","h") = "No" then asp_Write("checked") end if if tcWebApi_get("Dongle_Entry","BACKUP","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Disabled</font>
		</td>
	</tr>
	<%end if%>	
	<tr>
		<td class="title-sub"><font color="#000000">ISP Setting</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	
	
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Default APN</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="dongle_default_cfg" VALUE="Yes" 	<%if tcWebApi_get("Dongle_Entry","DEFAULTAPN","h") = "Yes" then asp_Write("checked") elseif tcWebApi_get("Dongle_Entry","DEFAULTAPN","h") = "N/A" then asp_Write("checked") end if%> onClick="default_cfg_chg()"><font color="#000000">Yes</font>
			<INPUT TYPE="RADIO" NAME="dongle_default_cfg" VALUE="No" <%if tcWebApi_get("Dongle_Entry","DEFAULTAPN","h") = "No" then asp_Write("checked") end if%> onClick="default_cfg_chg()"><font color="#000000">No</font>
		</td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">TELEPHONE</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="HIDDEN" NAME="dongle_TELEPHONE_hidden">
			<INPUT TYPE="HIDDEN" NAME="dongle_TELEPHONE_def_hidden"  VALUE= <% if tcWebApi_get("Dongle_Entry","TELEPHONE_DEF","h") <> "N/A" then tcWebApi_get("Dongle_Entry","TELEPHONE_DEF","s") else asp_Write("") end if%> >
			<INPUT TYPE="TEXT" NAME="dongle_TELEPHONE" SIZE="32" MAXLENGTH="31"  VALUE="<% if tcWebApi_get("Dongle_Entry","TELEPHONE","h") <> "N/A" then tcWebApi_get("Dongle_Entry","TELEPHONE","s") end if%>" ></td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">APN</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="HIDDEN" NAME="dongle_APN_hidden">
			<INPUT TYPE="HIDDEN" NAME="dongle_APN_def_hidden" VALUE= <% if tcWebApi_get("Dongle_Entry","APN_DEF","h") <> "N/A" then tcWebApi_get("Dongle_Entry","APN_DEF","s") else asp_Write("")  end if%> >
			<INPUT TYPE="TEXT" NAME="dongle_APN" SIZE="32" MAXLENGTH="31" VALUE="<% if tcWebApi_get("Dongle_Entry","APN","h") <> "N/A" then tcWebApi_get("Dongle_Entry","APN","s") end if%>" >
		</td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">UserName</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="HIDDEN" NAME="dongle_username_hidden">
			<INPUT TYPE="HIDDEN" NAME="dongle_username_def_hidden" VALUE= <% if tcWebApi_get("Dongle_Entry","USERNAME_DEF","h") <> "N/A" then tcWebApi_get("Dongle_Entry","USERNAME_DEF","s") else asp_Write("")  end if%> >
			<INPUT TYPE="TEXT" NAME="dongle_username" SIZE="32" MAXLENGTH="31" VALUE="<% if tcWebApi_get("Dongle_Entry","USERNAME","h") <> "N/A" then tcWebApi_get("Dongle_Entry","USERNAME","s") end if%>" >
		</td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Password</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="HIDDEN" NAME="dongle_password_hidden">
			<INPUT TYPE="HIDDEN" NAME="dongle_password_def_hidden" VALUE= <% if tcWebApi_get("Dongle_Entry","PASSWORD_DEF","h") <> "N/A" then tcWebApi_get("Dongle_Entry","PASSWORD_DEF","s")  else asp_Write("")  end if%> >
			<INPUT TYPE="TEXT" NAME="dongle_password" SIZE="32" MAXLENGTH="31" VALUE="<% if tcWebApi_get("Dongle_Entry","PASSWORD","h") <> "N/A" then tcWebApi_get("Dongle_Entry","PASSWORD","s") end if%>" >
		</td>
	</tr>
	
	<%if tcWebApi_get("WebCustom_Entry","isUSB3GDonglePINSupport","h") = "Yes" then%>
	<tr>
		<td class="title-sub"><font color="#000000">PIN Setting</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>

	<tr>
	    <td class="light-orange">&nbsp;</td>
	    <td class="light-orange"></td>
	    <td class="tabdata"><div align=right><font color="#000000">PIN</font></div></td>
	    <td class="tabdata"><div align=center>:</div></td>
	    <td class="tabdata">
	        <INPUT TYPE="PASSWORD" NAME="dongle_PIN" SIZE="32" MAXLENGTH="8" VALUE="<%if tcWebApi_get("Dongle_Entry","PIN","h") <> "N/A" then tcWebApi_get("Dongle_Entry","PIN","s") else asp_Write("") end if%>">   
	    </td>
	</tr>

	<tr>
	    <td class="light-orange">&nbsp;</td>
	    <td class="light-orange"></td>
	    <td class="tabdata"><div align=right><font color="#000000">Confirm PIN</font></div></td>
	    <td class="tabdata"><div align=center>:</div></td>
	    <td class="tabdata">
	        <INPUT TYPE="PASSWORD" NAME="dongle_PINConfirm" SIZE="32" MAXLENGTH="8" VALUE="<%if tcWebApi_get("Dongle_Entry","PIN","h") <> "N/A" then tcWebApi_get("Dongle_Entry","PIN","s") else asp_Write("") end if%>"> 
	    </td>
	</tr>	
	<%end if%>
		
	<tr>
		<td class="title-sub"><font color="#000000">Connection Setting</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Connection </font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="dongle_connect_type" VALUE="Connect_Keep_Alive"  <%if tcWebApi_get("Dongle_Entry","CONNECTION","h") = "Connect_Keep_Alive" then asp_Write("checked") elseif tcWebApi_get("Dongle_Entry","CONNECTION","h") = "N/A" then asp_Write("checked") end if%> onClick="connect_type_chg();"><font color="#000000">Always On (Recommended)</font>
		</td>
	</tr>
	<tr id="connondemand_info">
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="dongle_connect_type" VALUE="Connect_on_Demand"   <%if tcWebApi_get("Dongle_Entry","CONNECTION","h") = "Connect_on_Demand" then asp_Write("checked") end if%> onClick="connect_type_chg();"><font color="#000000">Connect On-Demand (Close if idle for</font>        
			<INPUT TYPE="TEXT" NAME="dongle_idle_time" SIZE="5" MAXLENGTH="3" VALUE=<% if tcWebApi_get("Dongle_Entry","CLOSEIFIDLE","h") = "N/A" then asp_Write("0") else tcWebApi_get("Dongle_Entry","CLOSEIFIDLE","s") end if%> ><font color="#000000">minutes)</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="dongle_connect_type" VALUE="Connect_Manually"  <%if tcWebApi_get("Dongle_Entry","CONNECTION","h") = "Connect_Manually" then asp_Write("checked")  end if%> onClick="connect_type_chg();"><font color="#000000">Connect Manually</font>
		</td>
	</tr>
	
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">PPP Authentication</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		
      <td class="tabdata"> <select name="PPPAuthen" size="1" >
	  	  <option <% if tcWebApi_get("Dongle_Entry","AUTHEN","h") = "AUTO" then asp_Write("selected") elseif tcWebApi_get("Dongle_Entry","AUTHEN","h") = "N/A" then asp_Write("selected") end if %> value="AUTO">AUTO
		  <option <% if tcWebApi_get("Dongle_Entry","AUTHEN","h") = "PAP" then asp_Write("selected") end if %> value="PAP">PAP
          <option <% if tcWebApi_get("Dongle_Entry","AUTHEN","h") = "CHAP" then asp_Write("selected") end if %> value="CHAP">CHAP 
        </select></td>
	</tr>
	
		
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Default Route</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="dongle_default_route" VALUE="Yes" <% if tcWebApi_get("Dongle_Entry","DEFAULTROUTE","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000">Yes</font>
			<INPUT TYPE="RADIO" name="dongle_default_route" VALUE="No"  <% if tcWebApi_get("Dongle_Entry","DEFAULTROUTE","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Dongle_Entry","DEFAULTROUTE","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">No</font>
		</td>
	</tr>
	
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">NAT</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<INPUT TYPE="RADIO" NAME="dongle_NAT_enable" VALUE="Enable" <% if tcWebApi_get("Dongle_Entry","NATENABLE","h") = "Enable" then asp_Write("checked")end if %> ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" name="dongle_NAT_enable" VALUE="Disable"  <% if tcWebApi_get("Dongle_Entry","NATENABLE","h") = "Disable" then asp_Write("checked") elseif tcWebApi_get("Dongle_Entry","NATENABLE","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Disable</font>
		</td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">TCP MTU Option</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">TCP MTU(0 means use default:1492)</font>
			<INPUT TYPE="TEXT" NAME="dongle_TCP_MTU" SIZE="5" MAXLENGTH="4" VALUE=<% if tcWebApi_get("Dongle_Entry","MTU","h") = "N/A" then asp_Write("0") else tcWebApi_get("Dongle_Entry","MTU","s") end if%> ><font color="#000000">bytes</font>
		</td>
	</tr>
	
	
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" height="42" class="black">&nbsp;</td>
		<td width="160" class="orange"></td>
		<td width="440" class="orange">
			<INPUT TYPE="button" NAME="SaveBtn" VALUE="SAVE" onClick="uiSave()" class="sbutton">
			<INPUT TYPE="reset" NAME="dongle_cancel" VALUE="CANCEL" >
		</td>
	</tr>      
</table>
</table></div></form></body>
<script language="JavaScript">
doLoad();
</script>
</html>
