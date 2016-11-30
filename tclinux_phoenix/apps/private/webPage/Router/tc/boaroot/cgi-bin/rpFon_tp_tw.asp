
<html>
<head>
<title>FON</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="../control.css" type="text/css">
<link rel="stylesheet" type="text/css" href="../style.css">
<style type="text/css">
td.Title{
	TEXT-ALIGN: left; BACKGROUND-COLOR: #FFFFFF; FONT-FAMILY: Arial; COLOR: #666666; FONT-SIZE: 15pt; VERTICAL-ALIGN: middle; FONT-WEIGHT: bold
}
td.SubTitle{
	TEXT-ALIGN: left; BACKGROUND-COLOR: #FFFFFF; FONT-FAMILY: Arial; COLOR: #666666; FONT-SIZE: 9pt; VERTICAL-ALIGN: middle;
}
td.SubData{
	TEXT-ALIGN: left; BACKGROUND-COLOR: #FFFFFF; FONT-FAMILY: Arial; COLOR: #009933; FONT-SIZE: 9pt; VERTICAL-ALIGN: middle; FONT-WEIGHT: bold
}
</style>
<script language="JavaScript" src="../general.js"></script>
<script language="JavaScript">

</script>
</head>
<body marginwidth="0" marginheight="0" LINK="#000000" VLINK="#000000" ALINK="#000000" >
<div align=center>
<table width="760" height="5" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td></td></tr></table>
<table border="0" width="760" cellspacing="0" cellpadding="0" bgcolor="#FFFFFF">
  <tr>
    <td width="200" height="50" align=center valign=middle bgcolor="#FFFFFF"><div align=left>
	<img src="/logo.gif" tppabs="logo.gif" width="200" height="50">
	</div></td><td width="560" align=right valign="bottom" bgcolor="#FFFFFF" class="model">
	  <font color="#000000">
	  <%if tcWebApi_get("WebCustom_Entry","isProlineCmdAction","h") = "Yes" then %>
	  <%if tcWebApi_get("SysInfo_Entry","ProductName","h") <> "N/A" then tcWebApi_get("SysInfo_Entry","ProductName","s") else%>
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%>ADSL Router
	  <%end if%>	  
	  <%else%>
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%>ADSL Router	  
	  <%end if%> 	  
	  </font>	
	  </td></tr></table>
<table width="760" height="5" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td></td></tr></table>
<TABLE border=0 cellSpacing=0 cellPadding=0 width=760 bgColor=#ffffff>
<tr>
	<td width=200 class=Title>Fon - Configuration</td>
	<td width=560>&nbsp;</td>
</tr>
<tr>
	<td>&nbsp;</td>
	<td>&nbsp;</td>
</tr>
<tr>
	<td width=200 class=SubTitle>Display fon status.</td>
	<td width=560>&nbsp;</td>
</tr>
<tr>
	<td>&nbsp;</td>
	<td>&nbsp;</td>
</tr>
<tr>
	<td width=200 class=SubTitle>FON Hotspot:</td>
	<td width=560 class=SubData><% if tcWebApi_get("FonNet_Entry","Enable","h") = "Yes" then asp_Write("Enabled") else asp_Write("Disabled") end if %></td>
</tr>
<tr>
	<td>&nbsp;</td>
	<td>&nbsp;</td>
</tr>
<tr>
	<td width=200 class=SubTitle>FON Status:</td>
	<td width=560 class=SubData><% if tcWebApi_get("FonNet_Entry","Status","h") = "1" then asp_Write("Online") else asp_Write("Offline") end if %></td>
</tr>
<tr>
	<td width=200 class=SubTitle>Public SSID:</td>
	<td width=560 class=SubData><% if tcWebApi_get("FonNet_Entry","Status","h") = "1" then asp_Write("Enabled") else asp_Write("Disabled") end if %></td>
</tr>
<tr>
	<td width=200 class=SubTitle>SM Server:</td>
	<td width=560 class=SubData><% if tcWebApi_get("FonNet_Entry","SM_status","h") = "1" then asp_Write("OK") else asp_Write("N/A") end if %></td>
</tr>
<tr>
	<td width=200 class=SubTitle>HM Server:</td>
	<td width=560 class=SubData><% if tcWebApi_get("FonNet_Entry","HM_status","h") = "1" then asp_Write("OK") else asp_Write("N/A") end if %></td>
</tr>
<tr>
	<td width=200 class=SubTitle>Key:</td>
	<td width=560 class=SubData><% if tcWebApi_get("FonNet_Entry","Key_status","h") = "1" then asp_Write("OK") else asp_Write("ERROR") end if %></td>
</tr>
<tr>
	<td>&nbsp;</td>
	<td>&nbsp;</td>
</tr>
</Table>
<TABLE class=orange border=0 cellSpacing=0 cellPadding=0 width=760 align=center 
height=5>
  <TBODY>
  <TR>
    <TD></TD></TR></TBODY></TABLE>
</div>
</body>
</html>
