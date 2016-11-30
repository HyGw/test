<%
if Request_Form("Fire_Flag") = "1" then 

	TCWebApi_unset("Firewall_Entry")
	TCWebApi_set("Firewall_Entry","firewall_status","EnableFire_Flag")
	TCWebApi_set("Firewall_Entry","spi_status","SPIFW_Flag")
	
	TCWebApi_set("Firewall_Entry","firewall_level","SecurityLevel")
	
	TCWebApi_commit("Firewall_Entry")
	TCWebApi_save()
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>防火墙</TITLE>
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
            value="安全->防火墙" name=Selected_Menu> </FONT></B><SPAN 
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
      <SCRIPT language=javascript>
			MakeMenu(getElById('Selected_Menu').value);
	  </SCRIPT>
      <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808
        border=0><TBODY>
        <TR>
          <TD height=15><IMG height=15 src="/img/panel1.gif" 
            width=164 border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 
            border=0> </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top> 
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>安全级</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
              <TD width=474>
                <script language=JavaScript type=text/javascript>
var FltsecLevel = '1'
var AttackProtectEnable = '1'

function LoadFrame()
{
	with (getElById('ConfigForm'))
	{
		setAttackProtect();
		if (curUserName != sptUserName)
		{
			getElement('protectatack1').style.display = "none";
			getElement('protectatack2').style.display = "none";
			getElement('protectatack3').style.display = "none";
		}
		return;
	}
}

function SubmitForm()
{
	var	Form = document.ConfigForm;
	Form.Fire_Flag.value = "1";
	Form.submit();
}

function ChangeLevel(level)
{
}

function setAttackProtect()
{
	var attackChecked = getCheckVal('enableAttackProtect');
	if (attackChecked == '1')
	{
		document.ConfigForm.EnableFire_Flag.value = "1";
	}
	else
	{
		document.ConfigForm.EnableFire_Flag.value = "0";
	}
}
</script></TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=8>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#防火墙" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 　　　　　　 
　　　　　　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
            <TD vAlign=top width=157 bgColor=#e7e7e7 height=30></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
		  <FORM name="ConfigForm" action="/cgi-bin/sec-firewall.asp" method="post">
		  	<input type="hidden" name="Fire_Flag" value="0">
			<input type="hidden" name="SPIFW_Flag" value="0">
        	<input type="hidden" name="EnableFire_Flag" value="<% tcWebApi_get("Firewall_Entry","firewall_status","s") %>">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P>安全级配置:<SELECT onchange="ChangeLevel(this)" size=1 name="SecurityLevel"> 
				  <OPTION value="low" <%if tcWebApi_get("Firewall_Entry","firewall_level","h") = "low" then asp_Write("selected") end if%>>低级
				  <OPTION value="medium" <%if tcWebApi_get("Firewall_Entry","firewall_level","h") = "medium" then asp_Write("selected") end if%>>中级
				  <OPTION value="high" <%if tcWebApi_get("Firewall_Entry","firewall_level","h") = "high" then asp_Write("selected") end if%>>高级</OPTION>
				  </SELECT></P></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR id=protectatack1>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>攻击保护设置</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD width=0></TD></TR>
        <TR id=protectatack2>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR id=protectatack3>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif></TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR id=AttackProtect>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P>攻击保护设置: 启用<INPUT id=enableAttackProtect onclick="setAttackProtect()" type="checkbox" name="enableAttackProtect" <%if tcWebApi_get("Firewall_Entry","firewall_status","h") = "1" then asp_Write("checked") end if%>>
				  </P></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
				
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center><IMG id=btnOK onclick="SubmitForm()" height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 
            width=80 border=0> </P></TD>
          <TD width=170 bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
