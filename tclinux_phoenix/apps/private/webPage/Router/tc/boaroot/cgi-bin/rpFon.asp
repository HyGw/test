<%
If Request_Form("FonFlag")="1" then 
	tcWebApi_set("FonNet_Entry","Enable","FonEnable") 
	tcWebApi_commit("FonNet_Entry")
End if
%>

<html>
<head>
<title>FON</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="../control.css" type="text/css">
<script language="JavaScript" src="../general.js"></script>
<script language="JavaScript">
function fonSave(){
  document.fon_form.FonFlag.value=1;
  document.fon_form.submit();
}
</script>
</head>
<body marginwidth="0" marginheight="0" LINK="#000000" VLINK="#000000" ALINK="#000000" >
<form name="fon_form" method="post">
<table width="100%" border="0" cellspacing="0" cellpadding="0" >
	<tr> 
		<td width="2%" >&nbsp;</td>
		<td width="98%" >&nbsp;</td>
	</tr>
</table>
<table width="100%" border="0" cellspacing="0" cellpadding="0" >
	<tr> 
		<td width="5%" >&nbsp;</td>
		<td width="90%" height="18"> 				
			<table border="0" cellspacing="0" cellpadding="0">
				<tr> 
					<td width="14"></td>
					<td> <div align="center"><B> FON</B></div></td>
					<td width="14"></td>
				</tr>
			</table>				
		</td>
		<td width="5%" height="18">&nbsp;</td>
	</tr>
	<tr> 
		<td width="5%" >&nbsp;</td>
		<td bgcolor="#787878"> 
			<table width="100%"  border="0" cellspacing="1" cellpadding="0">
				<tr> 
					<td bgcolor="#FFFFFF"> 
						<table width="100%"  border="0" cellspacing="0" cellpadding="0">
							<tr> 
								<td width="3" rowspan="3" background="../config_bo_l.gif"><img src="../dotspacer.gif" width="1" height="1"></td>
								<td height="3" background="../config_bo_t.gif"><img src="../dotspacer.gif" width="1" height="1"></td>
								<td width="3" rowspan="3" background="../config_bo_r.gif"><img src="../dotspacer.gif" width="1" height="1"></td>
							</tr>
							<tr> 
								<td align="center"> &nbsp;				
									
                    <table width="90%%">
                      <tr> 
                        <td>&nbsp;</td>
                        <td width="50%">&nbsp;</td>
                      </tr>
                      <tr> 
                        <td width="40%"> <FONT color=#000000>FONAP</FONT></td>
                        <td > <INPUT type="radio" value="Yes" name="FonEnable"  <% if tcWebApi_get("FonNet_Entry","Enable","h") = "Yes" then asp_Write("checked") end if %> > 
                          <FONT color=#000000>Enabled</FONT> <INPUT type="radio" value="No" name="FonEnable" <% if tcWebApi_get("FonNet_Entry","Enable","h") <> "Yes" then asp_Write("checked") end if %> > 
                          <FONT color=#000000>Disabled</FONT> </td></tr>
                      <TR bgcolor="#ffffff"> 
                      	<td width="40%" class=tabdata><div align="left"><FONT color=#000000>Status</FONT></div></td>
                        <td class=tabdata><% if tcWebApi_get("FonNet_Entry","Status","h") = "1" then asp_Write("online") else asp_Write("offline") end if %></td>               
                      </TR>
                      <tr> 
                      </tr>
                    </table>							
								<table border="0" cellspacing="0" cellpadding="0" width="300" align="center">
									<tr>
										<td>
										</td>
									</tr>
									<tr align="center">
										<td background="/wg_dotline.gif">&nbsp;</td>
									</tr>
									<tr>
										
                        <td align="center"><input type="HIDDEN" name="FonFlag" value="0"> 
                          <INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="fonSave()">	
    									<INPUT type="reset" value=CANCEL name=fon_cancel>
										</td>
									</tr>
									</table>
							</td>
						</tr>
						<tr>
							<td height="3" background="/config_bo_b.gif"><img src="../dotspacer.gif" width="1" height="1"></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
	</td>
	<td width="5%" >&nbsp;</td>
	</tr>
</table>
</form>
<p>&nbsp;</p>
</body>
</html>
