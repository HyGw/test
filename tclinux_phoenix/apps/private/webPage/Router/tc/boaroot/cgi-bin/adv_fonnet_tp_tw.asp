
<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=iso-8859-1" ;>
<LINK rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT>

function RefreshPage(){
  window.location="adv_fonnet.asp";
}
</SCRIPT>

<META content="MSHTML 6.00.2900.3059" name=GENERATOR></HEAD>
<BODY>
<FORM name="fon_form" action="/cgi-bin/adv_fonnet.asp" method=post>
<DIV align=center>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TD class=light-orange colSpan=5 height=5>&nbsp;</TD></TR>
  <TR>
    <TD class=title-main width=150 height=30><FONT 
      color=#ffffff>FON</FONT> </TD>
    </TD>
    <TD class=black width=10>&nbsp;</TD>
    <TD width=50></TD>
    <TD width=110></TD>
    <TD width=440></TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata></TD>
    <TD class=tabdata>
      <DIV align=left><B>&nbsp;Fon Spot Info</B></DIV></TD>
    <TD class=tabdata></TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata></TD>
    <TD class=tabdata>
      <DIV align=center>
	<Table width=100 border=1 cellpadding=1 cellspacing=0 bordercolor=#CCCCCC>
	<TR>
	<TD class=tabdata height=20 width=85>
      	<DIV align=left>&nbsp;Fon visitors:</DIV>
	</TD>
	<TD class=tabdata width=15>
				<DIV align=center><% tcWebApi_staticGet("FonNet_Entry","client_counter","s") %></DIV>
	</TD>
	</TR></Table></DIV></TD>
    <TD class=tabdata></TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata></TD>
    <TD class=tabdata></TD>
    <TD class=tabdata></TD></TR>
  </TBODY></TABLE>
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
    <INPUT TYPE="BUTTON" NAME="Refresh" VALUE="REFRESH" onClick="RefreshPage();">
    </TD></TR></TBODY></TABLE></DIV></FORM></BODY></HTML>
