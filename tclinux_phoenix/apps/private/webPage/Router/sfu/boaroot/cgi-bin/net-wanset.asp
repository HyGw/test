<%
if Request_Form("SubmitFlag") = "1" then
	TCWebApi_set("WanInfo_Common","CurIFIndex","curSetIndex")
	TCWebApi_set("WanInfo_WanPVC","Action","OperatorStyle")
	TCWebApi_set("WanInfo_WanPVC","ENCAP","EncapFlag")
	TCWebApi_set("WanInfo_WanIF","LinkMode","LinkModeFlag")
	TCWebApi_set("WanInfo_WanIF","MTU","MTUFlag")
	TCWebApi_set("WanInfo_WanIF","ISP","IpModeFlag")
	if Request_Form("IpModeFlag") = "1" then
		TCWebApi_set("WanInfo_WanIF","IPADDR","wanIpAddress")
		TCWebApi_set("WanInfo_WanIF","NETMASK","wanSubnetMask")
		TCWebApi_set("WanInfo_WanIF","GATEWAY","defaultGateway")
	elseif Request_Form("IpModeFlag") = "2" then
		TCWebApi_set("WanInfo_WanIF","USERNAME","pppUserName")
		TCWebApi_set("WanInfo_WanIF","PASSWORD","pppPassword")
	end if	
	TCWebApi_commit("WanInfo_WanPVC")
	TCWebApi_save()
end if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>宽带设置</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>

<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language=JavaScript>
function LoadFrame()
{

}
</SCRIPT>

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
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->宽带设置" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43 class=CucBgColorTD_1>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24 class=CucBgColorTD_3></TD></TR></TBODY></TABLE>
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
      <SCRIPT language=JavaScript>

function IpModeChange(IpMode)
{
	document.ConfigForm.IpModeFlag.value = IpMode;
}

function checkStatic()
{
	with (getElById('ConfigForm'))
	{
		if (!isAbcIpAddress(wanIpAddress.value))
		{
			alert("IP地址非法！");
			wanIpAddress.focus();
			return false;
		}
		
		if (!isValidSubnetMask(wanSubnetMask.value))
		{
			alert("子网掩码非法！");			
			return false;
		}
		
		if (!isHostIpWithSubnetMask(wanIpAddress.value, wanSubnetMask.value))
		{
			alert("IP地址和子网掩码不匹配");
			return false;
		}
		
		if (!isAbcIpAddress(defaultGateway.value))
		{
			alert("缺省网关非法！");
			return false;
		}
	}	
	return true;
}

function checkPPP()
{
	with (getElById('ConfigForm'))
	{
		if (isValidNameEx(pppUserName.value) == false)
		{
			alert("用户名非法");
			return false;
		}
		
		if (isValidNameEx(pppPassword.value) == false)
		{
			alert("密码非法");
			return false;
		}
	}	
	return true;
}

function doSubmit()
{
	with (getElById('ConfigForm'))
	{
		if(IpModeFlag.value == 1)	//static
		{
			if(checkStatic() == false)
				return;
				
			EncapFlag.value = "1483 Bridged IP LLC";
			MTUFlag.value = "1500";
			LinkModeFlag.value = "linkIP";
		}
		else if(document.ConfigForm.IpModeFlag.value == 2)	//pppoe
		{
			if(checkPPP() == false)
				return;	

			EncapFlag.value = "PPPoE LLC";
			MTUFlag.value = "1492";
			LinkModeFlag.value = "linkPPP";
		}
		else
		{
			EncapFlag.value = "1483 Bridged IP LLC";
			MTUFlag.value = "1500";
			LinkModeFlag.value = "linkIP";
		}
		SubmitFlag.value = 1;
	}
	document.ConfigForm.submit();
}

</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 class=Item_L1 height=30>
            <P>语音连接</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#宽带设置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 　 　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM name="ConfigForm" action="/cgi-bin/net-wanset.asp" method="post">
                  <input type="hidden" name="SubmitFlag" value="0">
                  <input type="hidden" name="OperatorStyle" value="Modify">
                  <input type="hidden" name="IpModeFlag" value="<% tcWebApi_get("WanInfo_WanIF","ISP","s") %>">
                  <input type="hidden" name="EncapFlag" value="<% tcWebApi_get("WanInfo_WanPVC","ENCAP","s") %>">
                  <input type="hidden" name="MTUFlag" value="<% tcWebApi_get("WanInfo_WanIF","MTU","s") %>">
                  <input type="hidden" name="LinkModeFlag" value="<% tcWebApi_get("WanInfo_WanIF","LinkMode","s") %>">
                  <input type="hidden" name="curSetIndex" value="1">
                  <DIV id=wanset_DhcpMode>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=120><INPUT onclick="IpModeChange(0)" type=radio value="DHCP" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "0" then asp_Write("checked") end if%>> DHCP</TD>
                      <TD>从ISP处自动得到一个IP地址.</TD></TR>
											<BR>
										</TBODY>
									</TABLE>
									</DIV>			
															
                  <DIV id=wanset_StaticMode>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=120><INPUT onclick="IpModeChange(1)" type=radio value="Static" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "1" then asp_Write("checked") end if%>> Static</TD>
                      <TD>经ISP配置一个静态的IP给你.</TD></TR>
                    <TR>
                      <TD>&nbsp;&nbsp;IP 地址：</TD>
                      <TD>                                  
                      	<INPUT id=wanIpAddress maxLength=15 size=15 name=wanIpAddress value="<%if TCWebApi_get("WanInfo_WanIF","IPADDR","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","IPADDR","s" ) end if %>">
                      </TD></TR>
                    <TR>
                      <TD>&nbsp;&nbsp;子网掩码：</TD>
                      <TD>                                  
                      	<INPUT id=wanSubnetMask maxLength=15 size=15 name=wanSubnetMask value="<%if TCWebApi_get("WanInfo_WanIF","NETMASK","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","NETMASK","s" ) end if %>">
                      </TD></TR>
                    <TR>
                      <TD>&nbsp;&nbsp;缺省网关：</TD>
                      <TD>                                  
                      	<INPUT id=defaultGateway maxLength=15 size=15 name=defaultGateway value="<%if TCWebApi_get("WanInfo_WanIF","GATEWAY","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","GATEWAY","s" ) end if %>">
                      </TD></TR>
											<BR>
										</TBODY>
									</TABLE>
									</DIV>
													
                  <DIV id=wanset_PPPoEMode>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=120><INPUT onclick="IpModeChange(2)" type=radio value="PPPoE" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "2" then asp_Write("checked") end if%>> PPPoE</TD>
                      <TD>若ISP使用PPPoE请选择该项.</TD></TR>
                    <TR>
                      <TD>&nbsp;&nbsp;用户名：</TD>
                      <TD>                                  
                      	<INPUT id=pppUserName style="FONT-FAMILY: '宋体'" maxLength=63 size=15 
                        name=pppUserName value="<%if TCWebApi_get("WanInfo_WanIF","USERNAME","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","USERNAME","s" ) end if %>">
                      </TD></TR>
                    <TR>
                      <TD>&nbsp;&nbsp;密码：</TD>
                      <TD>                                  
                      	<INPUT id=pppPassword style="FONT-FAMILY: '宋体'" type=password maxLength=63 size=15 name=pppPassword value="<% if TCWebApi_get("WanInfo_WanIF","PASSWORD","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","PASSWORD","s" ) end if %>">
                      </TD></TR>
											<BR>
										</TBODY>
									</TABLE>
									</DIV>
									</FORM>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
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
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>　</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2>
         	<P align=center><IMG id=btnOK onclick=doSubmit() height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 
            width=80 border=0> </P></TD>
          <TD width=170 
bgColor=#313031 class=CucBgColorTD_2>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
