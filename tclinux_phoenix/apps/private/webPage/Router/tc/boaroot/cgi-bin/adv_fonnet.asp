<%
If Request_Form("FonFlag")="1" then 
	tcWebApi_set("FonNet_Entry","Enable","FonEnable") 
	tcWebApi_commit("FonNet_Entry")
End if
%>

<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=iso-8859-1" ;>
<LINK rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT>

function fonSave(){
  document.fon_form.FonFlag.value=1;
  document.fon_form.submit();
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
    <TD class=black width=10>&nbsp;</TD>
    <TD width=150></TD>
    <TD width=10></TD>
    <TD width=440></TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Active</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    <INPUT type="radio" value="Yes" name="FonEnable"  <% if tcWebApi_get("FonNet_Entry","Enable","h") = "Yes" then asp_Write("checked") end if %> > <FONT color=#000000>Enabled</FONT>
    <INPUT type="radio" value="No" name="FonEnable" <% if tcWebApi_get("FonNet_Entry","Enable","h") <> "Yes" then asp_Write("checked") end if %> > <FONT color=#000000>Disabled</FONT>
     </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Status</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
	<% if tcWebApi_get("FonNet_Entry","Status","h") = "1" then asp_Write("online") else asp_Write("offline") end if %>
    </TD></TR>

 <%if tcWebApi_get("WebCustom_Entry","isFonV2Support","h") <> "Yes" then %>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Register</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    <INPUT type="button" value="Register" name="RegBtn" onClick="javascript:window.open('<% tcWebApi_get("FonNet_Entry","RegURL","s") %>') " >  
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Note</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
	click the "Register" button to register the owner of CPE as a FON member !
    </TD></TR>
 <%end if%>
    <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right></DIV></TD>
    <TD class=tabdata>
      <DIV align=center></DIV></TD>
    <TD class=tabdata></TD></TR></TBODY></TABLE>
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
    <INPUT TYPE="HIDDEN" NAME="FonFlag" VALUE="0">
    <INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="fonSave();">	
    <INPUT onClick="javascript:window.location='adv_fonnet.asp'" type=button value=CANCEL name=fon_cancel>
    </TD></TR></TBODY></TABLE></DIV></FORM></BODY></HTML>
