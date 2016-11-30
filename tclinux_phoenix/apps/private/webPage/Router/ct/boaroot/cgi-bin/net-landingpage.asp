<%
If Request_Form("landingPageSubmitFlag") = "1" then
	TCWebApi_set("Portal_Entry","Enable","enableValue")	
	TCWebApi_commit("Portal_Entry")
	TCWebApi_save()
End if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>Landing Page</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()" 
onunload=DoUnload() marginheight="0" marginwidth="0">
<TABLE height="100%" cellSpacing=0 cellPadding=0 width=808 align=center 
border=0>
  <TBODY>
  <TR>
    <TD height=1>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
        <TBODY>
        <TR>
          <TD>&nbsp;</TD>
          <TD vAlign=bottom align=right width=358>
            <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR>
                <TD vAlign=bottom align=right><SPAN class=curUserName>&nbsp; 
                  </SPAN></TD>
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->门户控制" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24></TD></TR></TBODY></TABLE>
      <SCRIPT 
      language=javascript>
MakeMenu(getElById ('Selected_Menu').value);
</SCRIPT>

      <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD height=15><IMG height=15 src="/img/panel1.gif" width=164 
            border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 border=0> 
          </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
<SCRIPT language=JavaScript type=text/javascript>
function landingPageEnblChange()
{
	if(document.LandingPage.landingPageEnbl.checked)
	{	
		document.LandingPage.enableValue.value="1";
	}
	else
	{
		document.LandingPage.enableValue.value="0";
	}

	return;
}
function uiSave()
{
	document.LandingPage.landingPageSubmitFlag.value = 1;
    document.LandingPage.submit();
    return;
}

</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>门户控制</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#门户控制" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name="LandingPage" action="/cgi-bin/net-landingpage.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD> 
					<INPUT onclick=landingPageEnblChange() type="checkbox" name="landingPageEnbl" <%if TCWebApi_get("Portal_Entry","Enable", "h") = "1" then asp_Write("checked") end if%>>
					<font color="#000000">使能</font>
					<INPUT type="hidden" name="landingPageSubmitFlag" value="0">
					<INPUT type="hidden" name="enableValue" value="<%if TCWebApi_get("Portal_Entry","Enable", "h")="1" then asp_write("1") else asp_write("0") end if%>">
				</TD>
			   </TR>
			   <TR>
                	<TD width=10>&nbsp;</TD>
                	<TD>&nbsp; 	</TD>
			   </TR>
			   <TR>
                	<TD width=10>&nbsp;</TD>
                	<TD>&nbsp; 	</TD>
			   </TR>
			   <TR>
                	<TD width=10>&nbsp;</TD>
                	<TD>&nbsp; 	</TD>
			   </TR>
			   <TR>
                	<TD width=10>&nbsp;</TD>
                	<TD>&nbsp; 	</TD>
			   </TR>
			   <TR>
                	<TD width=10>&nbsp;</TD>
                	<TD>&nbsp; 	</TD>
			   </TR>
			   <TR>
                	<TD width=10>&nbsp;</TD>
                	<TD> 
					
					
					</TD>
			   </TR>
			   </TBODY>
			</TABLE>
			
			</FORM></TD></TR>
			</TBODY></TABLE>
			
			<TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=100% 
        border=0><TBODY>
        <TR>
          <TD width=164 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
           <P align=center>
					<IMG id=btnOK onclick=uiSave() height=23 src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;
					<IMG id=btnCancel onclick="javascript:window.location='/cgi-bin/net-landingpage.asp'" height=23 src="/img/cancel.gif" width=80 border=0> 
					</P></TD>
          <TD width=170 bgColor=#313031></TD></TR></TBODY></TABLE></TD>
  
</BODY></HTML>
