<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>使用帮助</TITLE>
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
            value="帮助->使用帮助" name=Selected_Menu> </FONT></B><SPAN 
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
      <STYLE type=text/css>DD {
	FONT-SIZE: 16px
}
</STYLE>

      <SCRIPT language=JavaScript type=text/javascript>
var ifstatus = 0;
var ifnetwork = 0;
var ifsecurity = 0;
var ifapp = 0;
var ifman = 0;
var ifdiag = 0;
function LoadFrame()
{
with (document.ConfigForm)
{
	if (curUserName == sptUserName)
		{
			diag.style.display = "block";
			diag_1.style.display = "none";
		}
	else
		{
			diag.style.display = "none";
			diag_1.style.display = "none";
		}
	}
}
//一、状态
function funstatus()
{
with (document.ConfigForm)
	{
		if (ifstatus == 0)
		{
		status_1.style.display = "block";
		status_2.style.display = "block";
		status_3.style.display = "block";
		status_4.style.display = "block";
		status_5.style.display = "block";
		if (curUserName == sptUserName)
		{
		status_6.style.display = "block";
		status_7.style.display = "block";
		}
		ifstatus = 1;
		}
		else
		{
		status_1.style.display = "none";
		status_2.style.display = "none";
		status_3.style.display = "none";
		status_4.style.display = "none";
		status_5.style.display = "none";
		status_6.style.display = "none";
		status_7.style.display = "none";
		ifstatus = 0;
		}
	}
}
function funnetwork()
{
with (document.ConfigForm)
	{
		if (ifnetwork == 0)
		{
		if (curUserName == sptUserName)
		{
		network_1.style.display = "block";
		network_2.style.display = "block";
		network_3.style.display = "block";
		network_4.style.display = "block";
		}
		network_5.style.display = "block";
		if (curUserName == sysUserName)
		{
		getElement("wlanText").innerText = "1.WLAN配置";
		}
		if (curUserName == sptUserName)
		{
		network_6.style.display = "block";
		network_7.style.display = "block";
		network_8.style.display = "block";
		network_9.style.display = "block";
		network_10.style.display = "block";
		network_11.style.display = "block";
		network_12.style.display = "block";
		network_13.style.display = "block";
		}
		ifnetwork = 1;
		}
		else
		{
		network_1.style.display = "none";
		network_2.style.display = "none";
		network_3.style.display = "none";
		network_4.style.display = "none";
		network_5.style.display = "none";
		network_6.style.display = "none";
		network_7.style.display = "none";
		network_8.style.display = "none";
		network_9.style.display = "none";
		network_10.style.display = "none";
		network_11.style.display = "none";
		network_12.style.display = "none";
		network_13.style.display = "none";
		ifnetwork = 0;
		}
	}
}
function funsecui()
{
with (document.ConfigForm)
{
if (ifsecurity == 0)
{
security_1.style.display = "block";
security_2.style.display = "block";
security_3.style.display = "block";
security_4.style.display = "block";
security_5.style.display = "block";
if (curUserName == sptUserName)
{
security_6.style.display = "block";
security_7.style.display = "block";
}
ifsecurity = 1;
}
else
{
security_1.style.display = "none";
security_2.style.display = "none";
security_3.style.display = "none";
security_4.style.display = "none";
security_5.style.display = "none";
security_6.style.display = "none";
security_7.style.display = "none";
ifsecurity = 0;
}
}
}
function funapp()
{
	with (document.ConfigForm)
	{
		if (ifapp == 0)
		{
			if (curUserName == sptUserName)
			{
				//app_1.style.display = "block";
				app_2.style.display = "block";
				app_3.style.display = "block";
				//app_4.style.display = "block";
				//app_5.style.display = "block";
				app_6.style.display = "block";
				app_7.style.display = "block";
				//app_8.style.display = "block";
				
				//app_9.style.display = "block";
				//app_10.style.display = "block";
				//app_11.style.display = "block";
				if (curUserName == sysUserName)
				{
					getElement("dailyText").innerText = "1.日常应用";
				}
			}
			ifapp = 1;
		}
		else
		{
			app_1.style.display = "none";
			app_2.style.display = "none";
			app_3.style.display = "none";
			app_4.style.display = "none";
			app_5.style.display = "none";
			app_6.style.display = "none";
			app_7.style.display = "none";
			app_8.style.display = "none";
			app_9.style.display = "none";
			app_10.style.display = "none";
			app_11.style.display = "none";
			ifapp = 0;
		}
	}
}
function funman()
{
with (document.ConfigForm)
{
	if (ifman == 0)
	{
		man_1.style.display = "block";
		man_2.style.display = "block";
		man_3.style.display = "block";
		if (curUserName == sptUserName)
		{
		man_4.style.display = "block";
		man_5.style.display = "block";
		man_6.style.display = "block";
		man_7.style.display = "block";
		}
		ifman = 1;
	}
	else
	{
		man_1.style.display = "none";
		man_2.style.display = "none";
		man_3.style.display = "none";
		man_4.style.display = "none";
		man_5.style.display = "none";
		man_6.style.display = "none";
		man_7.style.display = "none";
		ifman = 0;
	}
}
}
function fundiag()
{
	with (document.ConfigForm)
	{
		if (ifdiag == 0)
		{
			diag_1.style.display = "block";
			ifdiag = 1;
		}
		else
		{
			diag_1.style.display = "none";
			ifdiag = 0;
		}
	}
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>使用帮助</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=6>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A href="/cgi-bin/help_content.asp#帮助" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif></TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR id=AttackProtect>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P><STRONG>帮助文档</STRONG></P></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name="ConfigForm" action="">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                        <TD> <DL id=info style="FONT-SIZE: 20px" name="info">
                            <DT onclick="funstatus()"><A 
                    href="/cgi-bin/help.asp#">一、状态</A> 
                            <DD id=status_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#状态" 
                    target=_blank>1.设备信息</A> 
                            <DD id=status_2 style="DISPLAY: none"> 
                            <DD id=status_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#网络侧信息" 
                    target=_blank>2.网络侧信息</A> 
                            <DD id=status_4 style="DISPLAY: none"> 
                            <DD id=status_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#用户侧信息" 
                    target=_blank>3.用户侧信息</A> 
                            <DD id=status_6 style="DISPLAY: none"> 
                            <DD id=status_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#远程管理状态" 
                    target=_blank>4.远程管理状态</A> 
                            <DT> 
                            <DT onclick=funnetwork()><A 
                    href="/cgi-bin/help.asp#">二、网络</A> 
                            <DD id=network_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#宽带设置" 
                    target=_blank>1.宽带设置</A> 
                            <DD id=network_2 style="DISPLAY: none"> 
                            <DD id=network_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#LAN侧地址配置" 
                    target=_blank>2.LAN侧地址配置</A> 
                            <DD id=network_4 style="DISPLAY: none"> 
                            <DD id=network_5 style="DISPLAY: none"><A id=wlanText 
                    href="/cgi-bin/help_content.asp#WLAN配置" 
                    target=_blank>3.WLAN配置</A> 
                            <DD id=network_6 style="DISPLAY: none"> 
                            <DD id=network_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#远程管理" 
                    target=_blank>4.远程管理</A> 
                            <DD id=network_8 style="DISPLAY: none"> 
                            <DD id=network_9 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#QoS" 
                    target=_blank>5.QoS</A> 
                            <DD id=network_10 style="DISPLAY: none"> 
                            <DD id=network_11 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#时间管理" 
                    target=_blank>6.时间管理</A> 
                            <DD id=network_12 style="DISPLAY: none"> 
                            <DD id=network_13 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#路由配置" 
                    target=_blank>7.路由配置</A> 
                            <DT> 
                            <DT onclick=funsecui()><A 
                    href="/cgi-bin/help.asp#">三、安全</A> 
                            <DD id=security_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#广域网访问设置" 
                    target=_blank>1.广域网访问设置</A> 
                            <DD id=security_2 style="DISPLAY: none"> 
                            <DD id=security_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#防火墙" 
                    target=_blank>2.防火墙</A> 
                            <DD id=security_4 style="DISPLAY: none"> 
                            <DD id=security_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#MAC过滤" 
                    target=_blank>3.MAC过滤</A> 
                            <DD id=security_6 style="DISPLAY: none"> 
                            <DD id=security_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#端口过滤" 
                    target=_blank>4.端口过滤</A> 
                            <DT> 
                            <DT id="app_all" onclick=funapp()><A 
                    href="/cgi-bin/help.asp#">四、应用</A> 
                              <script language="JavaScript" type="text/JavaScript">
							  if(curUserName == sysUserName)
							  	app_all.style.display = "none";
							  </script>
                            <DD id=app_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#DDNS配置" 
                    target=_blank>1.DDNS配置</A> 
                            <DD id=app_2 style="DISPLAY: none"> 
                            <DD id=app_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#高级NAT配置" 
                    target=_blank>1.高级NAT配置</A> 
                            <DD id=app_4 style="DISPLAY: none"> 
                            <DD id=app_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#UPNP配置" 
                    target=_blank>3.UPNP配置</A> 
                            <DD id=app_6 style="DISPLAY: none"> 
                            <DD id=app_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#IGMP设置" 
                    target=_blank>2.IGMP设置</A> 
                            <DD id=app_8 style="DISPLAY: none"> 
                            <DD id=app_9 style="DISPLAY: none"><A id=dailyText 
                    href="/cgi-bin/help_content.asp#日常应用" 
                    target=_blank>5.日常应用</A> 
                            <DD id=app_10 style="DISPLAY: none"> 
                            <DD id=app_11 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#设备命名" 
                    target=_blank>6.设备命名</A> 
                            <DT> 
                            
                              <script language="JavaScript" type="text/JavaScript">
							  	if(curUserName == sptUserName)
							  		document.writeln('<DT onclick=funman()><A href="/cgi-bin/help.asp#">五、管理</A>');
								else
									document.writeln('<DT onclick=funman()><A href="/cgi-bin/help.asp#">四、管理</A>');
							  </script>
                            <DD id=man_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#用户管理" 
                    target=_blank>1.用户管理</A> 
                            <DD id=man_2 style="DISPLAY: none"> 
                            <DD id=man_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#设备管理" 
                    target=_blank>2.设备管理</A> 
                            <DD id=man_4 style="DISPLAY: none"> 
                            <DD id=man_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#日志文件管理" 
                    target=_blank>3.日志文件管理</A> 
                            <DD id=man_6 style="DISPLAY: none"> 
                            <DD id=man_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#维护" 
                    target=_blank>4.维护</A> 
                            <DT> 
                            <DT id=diag onclick=fundiag()><A 
                    href="/cgi-bin/help.asp#">六、诊断</A> 
                            <DD id=diag_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#网络诊断" 
                    target=_blank>1.网络诊断</A> </DD>
                          </DL></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
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
            <P align=center><!--A 
            href="/cgi-bin/help.asp#"><IMG id="btnOK" 
            onclick="" height="23" src="/img/ok.gif" width="80" 
            border="0"></A>&nbsp;&nbsp;<A 
            href="/cgi-bin/help.asp#"><IMG id="btnCancel" 
            onclick="" height="23" src="/img/cancel.gif" width="80" 
            border="0"></A--> </P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
