<%
If Request_Form("fwFlag")="1" then 
	tcWebApi_set("Firewall_Entry","firewall_status","firewallEnable") 
	tcWebApi_set("Firewall_Entry","spi_status","spiEnable")
	tcWebApi_commit("Firewall_Entry")
End if
%>

<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=iso-8859-1" ;>
<LINK rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT>

function fwSave(){
   	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to save these configurations?"))
			return;
	<%end if%>	
  document.fw_form.fwFlag.value=1;
  document.fw_form.submit();
}
</SCRIPT>

<META content="MSHTML 6.00.2900.3059" name=GENERATOR></HEAD>
<BODY>
<FORM name="fw_form" action="/cgi-bin/adv_firewall.asp" method=post>
<DIV align=center>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TD class=light-orange colSpan=5 height=5>&nbsp;</TD></TR>
  <TR>
    <TD class=title-main width=150 height=30><FONT 
      color=#ffffff>Firewall</FONT> </TD>
    <TD class=black width=10>&nbsp;</TD>
    <TD width=150></TD>
    <TD width=10></TD>
    <TD width=440></TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Firewall</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    <INPUT type="radio" value="1" name="firewallEnable"  <% if tcWebApi_get("Firewall_Entry","firewall_status","h") = "1" then asp_Write("checked") end if %> > <FONT color=#000000>Enabled</FONT>
    <INPUT type="radio" value="0" name="firewallEnable" <% if tcWebApi_get("Firewall_Entry","firewall_status","h") = "0" then asp_Write("checked") end if %> > <FONT color=#000000>Disabled</FONT>
     </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>SPI</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    <INPUT type="radio" value="1" name="spiEnable" <% if tcWebApi_get("Firewall_Entry","spi_status","h") = "1" then asp_Write("checked") end if %> > <FONT color=#000000>Enabled</FONT> 
    <INPUT type="radio" value="0" name="spiEnable" <% if tcWebApi_get("Firewall_Entry","spi_status","h") = "0" then asp_Write("checked") end if %> > <FONT color=#000000>Disabled</FONT> 
    </TD></TR>
    <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right></DIV></TD>
    <TD class=tabdata>
      <DIV align=center></DIV></TD>
    <TD class=tabdata>
      <DIV align=left><FONT color=#000000>(WARNING: If you enable SPI, all 
      traffics initiated from WAN would be blocked, including DMZ, Virtual 
      Server and ACL WAN side.)</FONT> </DIV></TD></TR></TBODY></TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TD class=light-orange width=150>&nbsp;</TD>
    <TD class=light-orange width=10></TD>
    <TD width=150>&nbsp;</TD>
    <TD width=10>&nbsp;</TD>
    <TD width=440>&nbsp;</TD></TR></TBODY></TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR height=42>
    <TD class=black width=160>&nbsp;</TD>
    <TD class=orange width=160>&nbsp;</TD>
    <TD class=orange width=440>
    <INPUT TYPE="HIDDEN" NAME="fwFlag" VALUE="0">
    <INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="fwSave();">	
    <INPUT onClick="javascript:window.location='adv_firewall.asp'" type=button value=CANCEL name=firewall_cancel>
    </TD></TR></TBODY></TABLE></DIV></FORM></BODY></HTML>
