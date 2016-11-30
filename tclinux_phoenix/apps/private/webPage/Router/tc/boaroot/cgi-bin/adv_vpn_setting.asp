<%
if Request_form("delnum") <> "" then
		tcWebApi_set("WebCurSet_Entry","vpn_id","delnum")
		tcWebApi_unset("VPN_Entry")
		tcWebApi_commit("VPN_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<SCRIPT language=javascript>

function totalConnCheck()
{
	if(document.VpnTable_form.TotalConnNum.value >= 8)
	{
		alert("Users can only configure 8 VPN connections!\n");
		return false;
	}
	
	window.location='/cgi-bin/adv_vpn_connection.asp?add_num='+document.VpnTable_form.add_num.value
	return true;
}

function doDelete(i)
{
 	document.VpnTable_form.delnum.value=i;
	document.VpnTable_form.submit();
}

function doedit(i)
{
 	document.VpnTable_form.action="/cgi-bin/adv_vpn_connection.asp";
	document.VpnTable_form.editnum.value=i;
	document.VpnTable_form.submit();
}
</SCRIPT>

</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/adv_vpn_setting.asp" name="VpnTable_form">
<div align="center">
<INPUT TYPE="HIDDEN" NAME="delnum">
<INPUT TYPE="HIDDEN" NAME="editnum">
<INPUT TYPE="HIDDEN" NAME="TotalConnNum" value="<%TcWebApi_get("VPN","Total_Conn_Num","s")%>">
<INPUT TYPE="HIDDEN" NAME="add_num" value="<%TcWebApi_get("VPN","add_num","s")%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
  <tr>
    <td width="150" height="30" class="title-main">
      <font color="#FFFFFF">VPN Connection List</font></td>
	<td width="10" class="black">&nbsp;</td><td width="150"></td>
	<td width="10" ></td><td width="440"></td></tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
	<td colspan="3" align=center>
      <table width="580" border="1" align="center" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
		<tr height="30">
		  <td width="40" align=center class="tabdata"><strong>#</strong></td>
		  <td width="120" align=center class="tabdata"><strong><font color="#000000">Connection Name</font></strong></td>
		  <td width="100" align=center class="tabdata"><strong><font color="#000000">Remote Gateway</font></strong></td>
		  <td width="100" align=center class="tabdata"><strong><font color="#000000">Local Address</font></strong></td>
		  <td width="100"  align=center class="tabdata"><strong><font color="#000000">Remote Address</font></strong></td>
		  <td align=center class="tabdata"><strong><font color="#000000">Edit</font></strong></td>
		  <td align=center class="tabdata"><strong><font color="#000000">Drop</font></strong></td>
          </tr>
	<%if tcWebApi_get("GUITemp_Entry0","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry0","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry0","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry0","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry0","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry0","Drop","s") %></td>
			</tr>
	<%end if%>
	<%if tcWebApi_staticGet("GUITemp_Entry1","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry1","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry1","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry1","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry1","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry1","Drop","s") %></td>
			</tr>
	<%end if%>
	<%if tcWebApi_staticGet("GUITemp_Entry2","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry2","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry2","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry2","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry2","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry2","Drop","s") %></td>
			</tr>
	<%end if%>
	<%if tcWebApi_staticGet("GUITemp_Entry3","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry3","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry3","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry3","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry3","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry3","Drop","s") %></td>
			</tr>
	<%end if%>
	<%if tcWebApi_staticGet("GUITemp_Entry4","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry4","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry4","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry4","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry4","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry4","Drop","s") %></td>
			</tr>
	<%end if%>
	<%if tcWebApi_staticGet("GUITemp_Entry5","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry5","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry5","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry5","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry5","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry5","Drop","s") %></td>
			</tr>
	<%end if%>
	<%if tcWebApi_staticGet("GUITemp_Entry6","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry6","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry6","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry6","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry6","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry6","Drop","s") %></td>
			</tr>
	<%end if%>
	<%if tcWebApi_staticGet("GUITemp_Entry7","vpnTRLine","h") <> "" then%>
		<tr>
			<%tcWebApi_staticGet("GUITemp_Entry7","Index_Name","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry7","GW_Local","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry7","Remote","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry7","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry7","Drop","s") %></td>
			</tr>
	<%end if%>
		
		</table></td></tr>
		
  <tr>
    <td class="light-orange"></td>
	<td class="light-orange"></td>
	<td>&nbsp;</td><td>&nbsp;</td>
	<td>&nbsp;</td></tr>
	
</table>

<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr height="42">
    <td width="160" class="black">&nbsp;</td>
	<td width="160" class="orange">&nbsp;</td>
	<td width="440" class="orange">
	  <input name="Add New Connection" type="button" value="Add New Connection" onClick="totalConnCheck()">
  	</tr>
</table>

</div>
</form>
</body>
</html>
