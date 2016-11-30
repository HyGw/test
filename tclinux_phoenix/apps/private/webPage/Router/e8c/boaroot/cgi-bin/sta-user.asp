<%
tcWebApi_constSet("WebCurSet_Entry", "isSSIDHidden", "No")
if tcWebApi_get("WebCustom_Entry", "isCTPONCZNXSupported", "h") = "Yes"  then
	if tcwebApi_get("WebCurSet_Entry","CurrentAccess","h") <> "0" then
		tcWebApi_constSet("WebCurSet_Entry", "isSSIDHidden", "Yes")
	end if
elseif tcWebApi_get("WebCustom_Entry", "isCTPONOnlyOneSSIDSupported", "h") = "Yes"  then
	tcWebApi_constSet("WebCurSet_Entry", "isSSIDHidden", "Yes")
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>用户侧信息</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %> 
<SCRIPT language=JavaScript type=text/javascript src="/cgi-bin/showusb.cgi"></SCRIPT>
<% end if %>
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
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
<% if tcwebApi_get("WebCustom_Entry","isCTPONTYLOGOSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% end if %> 
<% end if %>
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
            value="状态->用户侧信息" name=Selected_Menu> </FONT></B><SPAN 
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
          <TD height=15><IMG height=15 src="/img/panel1.gif" width=164 border=0></TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 border=0> 
          </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
        <SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
        <SCRIPT language=JavaScript type=text/javascript>
		var PortConnStatus = new Array(4);
		PortConnStatus[0] = "<% tcWebApi_get("Info_Ether","Port1Status","s") %>";
		PortConnStatus[1] = "<% tcWebApi_get("Info_Ether","Port2Status","s") %>";
		PortConnStatus[2] = "<% tcWebApi_get("Info_Ether","Port3Status","s") %>";
		PortConnStatus[3] = "<% tcWebApi_get("Info_Ether","Port4Status","s") %>";
		
		var aryInfoTemp = "<% tcWebApi_get("Info_Ether","PortIP","s") %>";
		var PortIp = aryInfoTemp.split(',');
		var dhcpnum = PortIp.length - 1;
		aryInfoTemp = "<% tcWebApi_get("Info_Ether","PortMac","s") %>";
		aryInfoTemp += "<% tcWebApi_get("Info_Ether","PortMac2","s") %>";
		var PortMac = aryInfoTemp.split(',');
		aryInfoTemp = "<% tcWebApi_get("Info_Ether","PortDHCP","s") %>";
		var PortDhcp = aryInfoTemp.split(',');
		aryInfoTemp = "<% tcWebApi_get("Info_Ether","PortExpire","s") %>";
		var PortExpireTime = aryInfoTemp.split(',');
		
		var wlanEnbl = "<% TCWebApi_get("WLan_Common", "APOn", "s") %>";
		var wlanacEnbl = "<% TCWebApi_get("WLan11ac_Common", "APOn", "s") %>";


function stWlan(domain,enable,ssid,BeaconType,BasicEncrypt,BasicAuth,WPAEncrypt,WPAAuth,IEEE11iEncrypt,IEEE11iAuth,Channel,ChannelsInUse)
{
this.domain = domain;
this.enable = enable;
this.ssid = ssid;
this.BeaconType = BeaconType;
this.BasicAuth = BasicAuth;
this.BasicEncrypt = BasicEncrypt;
this.WPAAuth = WPAAuth;
this.WPAEncrypt = WPAEncrypt;
this.IEEE11iAuth = IEEE11iAuth;
this.IEEE11iEncrypt = IEEE11iEncrypt;
this.Channel = Channel;
this.ChannelsInUse = ChannelsInUse;
}

var DeviceInfo =  [["0","Computer","  "," ","0"," Static "],null];

var WlanInfo = new Array(4);
WlanInfo[0] = new stWlan("0","<% tcWebApi_get("WLan_Entry0","HideSSID","s") %>","<% tcWebApi_get("WLan_Entry0","SSID","s") %>","<% tcWebApi_get("WLan_Entry0","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry0","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry0","WEPAuthType","s") %>","<% tcWebApi_get("WLan_Entry0","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry0","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
<% if tcwebApi_get("WebCurSet_Entry","isSSIDHidden","h") <> "Yes" then %>
WlanInfo[1] = new stWlan("1","<% tcWebApi_get("WLan_Entry1","HideSSID","s") %>","<% tcWebApi_get("WLan_Entry1","SSID","s") %>","<% tcWebApi_get("WLan_Entry1","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry1","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry1","WEPAuthType","s") %>","<% tcWebApi_get("WLan_Entry1","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry1","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
<% if tcwebApi_get("WebCustom_Entry","isCTPONOnlyTwoSSIDSupported","h") <> "Yes" then %>
WlanInfo[2] = new stWlan("2","<% tcWebApi_get("WLan_Entry2","HideSSID","s") %>","<% tcWebApi_get("WLan_Entry2","SSID","s") %>","<% tcWebApi_get("WLan_Entry2","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry2","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry2","WEPAuthType","s") %>","<% tcWebApi_get("WLan_Entry2","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry2","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
WlanInfo[3] = new stWlan("3","<% tcWebApi_get("WLan_Entry3","HideSSID","s") %>","<% tcWebApi_get("WLan_Entry3","SSID","s") %>","<% tcWebApi_get("WLan_Entry3","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry3","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry3","WEPAuthType","s") %>","<% tcWebApi_get("WLan_Entry3","AuthMode","s") %>","<% tcWebApi_get("WLan_Entry3","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
<% end if %>
<% end if %>

var WlanacInfo = new Array(4);
WlanacInfo[0] = new stWlan("0","<% tcWebApi_get("WLan11ac_Entry0","HideSSID","s") %>","<% tcWebApi_get("WLan11ac_Entry0","SSID","s") %>","<% tcWebApi_get("WLan11ac_Entry0","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry0","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry0","WEPAuthType","s") %>","<% tcWebApi_get("WLan11ac_Entry0","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry0","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
<% if tcwebApi_get("WebCustom_Entry","isCTPONSNSupported","h") <> "Yes" then %>
WlanacInfo[1] = new stWlan("1","<% tcWebApi_get("WLan11ac_Entry1","HideSSID","s") %>","<% tcWebApi_get("WLan11ac_Entry1","SSID","s") %>","<% tcWebApi_get("WLan11ac_Entry1","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry1","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry1","WEPAuthType","s") %>","<% tcWebApi_get("WLan11ac_Entry1","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry1","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
WlanacInfo[2] = new stWlan("2","<% tcWebApi_get("WLan11ac_Entry2","HideSSID","s") %>","<% tcWebApi_get("WLan11ac_Entry2","SSID","s") %>","<% tcWebApi_get("WLan11ac_Entry2","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry2","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry2","WEPAuthType","s") %>","<% tcWebApi_get("WLan11ac_Entry2","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry2","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
WlanacInfo[3] = new stWlan("3","<% tcWebApi_get("WLan11ac_Entry3","HideSSID","s") %>","<% tcWebApi_get("WLan11ac_Entry3","SSID","s") %>","<% tcWebApi_get("WLan11ac_Entry3","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry3","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry3","WEPAuthType","s") %>","<% tcWebApi_get("WLan11ac_Entry3","AuthMode","s") %>","<% tcWebApi_get("WLan11ac_Entry3","EncrypType","s") %>","AESEncryption","PSKAuthentication","0","1");
<% end if %>

var PacketInfo = new Array();
PacketInfo = [["0","0","0","0","0","0","0","0","0"],null];



Ethernet = [["0","Disabled","560097","4123","0","0","3096680","5897","0","0"],["InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2","Up","560097","4123","0","0","3096680","5897","0","0"],["InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.3","Disabled","560362","4124","0","0","3096680","5897","0","0"],["InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4","Disabled","560362","4124","0","0","3096680","5897","0","0"],null]

var usbstate0 = "<% tcWebApi_get("Usb_Entry0","usbstate","s") %>";
var usbstate1 = "<% tcWebApi_get("Usb_Entry1","usbstate","s") %>";

var usb = [null];
var usb1 = usb[0];
if (usb.length > 1)
var usb2 = usb[1];
var i = 0;
var LanInfo = new Array();

function stUserInfo(domain,name,level,busy)
{
	this.domain = domain;
	this.name = name;
	this.level = level;
	this.busy = busy;
}

var UserInfo = new Array(3);
	UserInfo[0] = new stUserInfo(0,"<% tcWebApi_get("Account_Entry0","username","s") %>","<% tcWebApi_get("Account_Entry1","usertype","s") %>","null");
	UserInfo[1] = new stUserInfo(1,"<% tcWebApi_get("Account_Entry1","username","s") %>","<% tcWebApi_get("Account_Entry2","usertype","s") %>","null");
	UserInfo[2] = new stUserInfo(2,"<% tcWebApi_get("Account_Entry2","username","s") %>","<% tcWebApi_get("Account_Entry3","usertype","s") %>","null");

var k = 1;
for(k=0; k < UserInfo.length-1; k++)
{
	var num = UserInfo[k].level;
	if (num == 1)
	{
		break;
	}
}
var sysUserName = UserInfo[k].name;
var addressNum = 0;
var i = 0;
function ipv6cnst(domain,ipv6address)
{
	this.domain = domain;
	this.ipv6address = ipv6address.split(',');
	addressNum = this.ipv6address.length;
}
var ipv6infost = new Array(new ipv6cnst("1","1111:db8:1::1,fe80::1"),new ipv6cnst("2","1111:db8:2::1,fe80::1"),new ipv6cnst("3","1111:db8:1::1,fe80::1"),null);


function LoadFrame()
{
	if (curUserName != sptUserName)
	{
		document.all("NetPacketInfo_WLan").style.display = "none";
		document.all("NetPacketInfo_Ethe").style.display = "none";
	}
}
function LanInfoConstruction(Device, IPAddr, MACAddr, Status)
{
	this.Device = Device;
	this.IPAddr = IPAddr;
	this.MACAddr = MACAddr;
	this.Status = Status;
}
function DhcpServerRelTime(timeVal)
{
	var timeString = "租期剩余";
	if (timeVal == 0)
	{
	return '租期已满';
	}
	if (timeVal >= 60*60*24)
	{
	timeString += parseInt(timeVal/(60*60*24)) + "天" ;
	timeVal %= 60*60*24;
	}
	if (timeVal >= 60*60)
	{
	timeString += parseInt(timeVal / (60*60)) + "小时" ;
	timeVal %= 60*60;
	}
	if (timeVal >= 60)
	{
	timeString += parseInt(timeVal / (60)) + "分钟" ;
	timeVal %= 60;
	}
	if (timeVal > 0)
	{
	timeString += timeVal + '秒';
	}
	return timeString;
}

</SCRIPT>
        <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") <>"Yes" then %>        
        <TR id=secWireless1>
          <TD width=157 bgColor=#ef8218 height=29>
            <P class=Item_L1>WLan2.4G接口信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
              <TD width=474>
			  
　 </TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
<% if tcwebApi_get("WebCustom_Entry","isCTSCSupported","h") = "Yes" then %>
            rowSpan=17
<% else %>
            rowSpan=14
<% end if %>
            ><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#用户侧信息" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isWebTYLOGOSupported","h") = "Yes" then %>                  
               <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>     
<% end if %>                                  
                  </TBODY></TABLE></TD></TR>
        <TR id=secWireless2>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR id=secWireless3>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P>
                </TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title width="29%">无线网络连接状态:</TD>
                      <TD width="71%">
					  
                        <SCRIPT language=JavaScript type=text/javascript>
							if (wlanEnbl == "0")
							{
							document.write('未启用')
							}
							else
							{
							document.write('启用')
							}
						</SCRIPT>
                        &nbsp;</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
					if (( wlanEnbl == "1") && (curUserName == sptUserName))
					document.write(   '<tr>' +
					'<td class="table_title">信道:</td>' +
					'<td>' + "<%tcWebApi_get("Info_WLan","CurrentChannel","s") %>"  +'&nbsp;</td>' +
					//'<td>' + WlanInfo[0].ChannelsInUse  +'&nbsp;</td>' +
					'</tr>');
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=NetPacketInfo_WLan cellSpacing=0 cellPadding=3 
                  width="100%" border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle>接口</TD>
                      <TD class=table_title align=middle colSpan=4>
                        <CENTER>接收 </CENTER></TD>
                      <TD class=table_title align=middle colSpan=4>
                        <CENTER>发送 </CENTER></TD></TR>
                    <TR>
                      <TD class=table_title align=middle>&nbsp;</TD>
                      <TD class=table_title align=middle>字节</TD>
                      <TD class=table_title align=middle>包</TD>
                      <TD class=table_title align=middle>错误</TD>
                      <TD class=table_title align=middle>丢弃</TD>
                      <TD class=table_title align=middle>字节</TD>
                      <TD class=table_title align=middle>包</TD>
                      <TD class=table_title align=middle>错误</TD>
                      <TD class=table_title align=middle>丢弃</TD></TR>
                    <SCRIPT language=javascript>
					if (wlanEnbl == "1")
					{
					document.writeln("  <tr>");
					document.writeln(" <td class='table_title' align='center'>" + "无线" + "</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","rxbytes","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","wlanRxFrames","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","wlanRxErrFrames","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","wlanRxDropFrames","s") %>" + "&nbsp;</td>");
										
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","txbytes","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","wlanTxFrames","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","wlanTxErrFrames","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_WLan","wlanTxDropFrames","s") %>" + "&nbsp;</td>");
					document.writeln("  </tr>");
					}
					</SCRIPT>
                    </TBODY></TABLE>
                  <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                    <TBODY>
                    <TR>
                      <TD width="20%" height="28" align=middle class=table_title>SSID索引</TD>
                      <TD class=table_title align=middle>SSID名称</TD>
                      <TD class=table_title align=middle>安全配置</TD>
                      <TD class=table_title align=middle>认证方式</TD>
                      <TD class=table_title align=middle>加密 </TD></TR>
                    <SCRIPT language=javascript>
					var ssidnum=<% tcWebApi_get("WLan_Common","BssidNum","s") %>

					if (wlanEnbl == "1")
					{
<% if tcwebApi_get("WebCustom_Entry","isCTPONCZGDSupported","h") = "Yes" then %>
					        var dispSSIDNum = 0;
						for (var i = 0; i < 4; i++)
<%else%>
						for (var i = 0; i < ssidnum; i++)
<%end if%>
						{
<% if tcWebApi_get("WebCurSet_Entry", "isSSIDHidden", "h") = "Yes"  then %>
							if ( i > 0 )
								continue;
<% end if %> 
<% if tcwebApi_get("WebCustom_Entry","isCTPONOnlyTwoSSIDSupported","h") = "Yes" then %>
							if ( i > 1 )
								continue;
<% end if %>
<% if tcwebApi_get("WebCustom_Entry","isCTPONCZGDSupported","h") = "Yes" then %>
							if(dispSSIDNum >= ssidnum)
								continue;

							if(WlanInfo[i].enable=='0' || WlanInfo[i].enable=='1'){
								dispSSIDNum++;
<% end if %>								
							var Auth = "";
							var Encrypt = "";
							var AuthSt = "已配置";
							if (WlanInfo[i].BeaconType.indexOf('WEP') > -1)
							{
								Auth = WlanInfo[i].BasicAuth;
								Encrypt = WlanInfo[i].BasicEncrypt;
							}
							else if(WlanInfo[i].BeaconType.indexOf('OPEN') > -1)
							{
								Auth = "OPENSYSTEM";
								Encrypt = "NONE";
								AuthSt = "未配置";
							}
							else 
							{
								Encrypt = WlanInfo[i].WPAAuth;
								Auth = WlanInfo[i].WPAEncrypt;
							}							
							document.writeln('<tr>' + '<td class="table_title" align="center">' + 'SSID-' + (i+1) + ':</td>' + '<td  align="center">' + WlanInfo[i].ssid + '&nbsp;</td><td align="center">' + AuthSt + '&nbsp;</td>' + '&nbsp;</td><td align="center">' + Auth.toUpperCase () + '&nbsp;</td>' + '<td' + ' align="center">' + Encrypt.toUpperCase() + '&nbsp;</td>' + '</tr>');
						}
					}
<% if tcwebApi_get("WebCustom_Entry","isCTPONCZGDSupported","h") = "Yes" then %>
					}
<% end if %>								
				</SCRIPT>
                    <BR>
                    </TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR>
              </TABLE></TD></TR>
        <TR id=secWireless4>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
          	
					<TR id=secWireless5>
					<TD width=157 bgColor=#ef8218 height=29><P class=Item_L1>WLan5G接口信息</P></TD>
					<TD width=7 bgColor=#ef8218>　</TD>
					<TD width=474></TD>
					<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>  
					<TD vAlign=top width=170 background=/img/panel4.gif 
					<% if tcwebApi_get("WebCustom_Entry","isCTSCSupported","h") = "Yes" then %>
											rowSpan=14
					<% else %>
											rowSpan=11
					<% end if %>
																>
						<TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
						<TBODY>
						<TR><TD><A href="/cgi-bin/help_content.asp#用户侧信息" target=_blank><IMG height=34 src="/img/help.gif" width=40 border=0></A></TD></TR>
						</TBODY>
						</TABLE>
					</TD>
					<% end if %>
					</TR>
					<TR id=secWireless6>
					<TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
					<TD width=7 background=/img/panel3.gif>　</TD>
					<TD></TD>
					</TR>
					<TR id=secWireless7>
					<TD vAlign=top width=157 bgColor=#e7e7e7 height=30><P class=Item_L2>&nbsp;</P></TD>
					<TD width=7 background=/img/panel3.gif>　</TD>
					<TD>
						<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
						<TBODY>
						<TR>
						<TD width=10>&nbsp;</TD>
						<TD>
							<TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
							<TBODY>
							<TR>
							<TD class=table_title width="29%">无线网络连接状态:</TD>
							<TD width="71%">
<SCRIPT language=JavaScript type=text/javascript>
if (wlanacEnbl == "0")
{
	document.write('未启用')
}
else
{
	document.write('启用')
}
</SCRIPT>
							&nbsp;</TD>
							</TR>
<SCRIPT language=JavaScript type=text/javascript>
if(( wlanacEnbl == "1") && (curUserName == sptUserName))
document.write(  '<tr>' + '<td class="table_title">信道:</td>' + '<td>' + "<%tcWebApi_get("Info_WLan11ac","CurrentChannel","s") %>"  +'&nbsp;</td>' + '</tr>');
</SCRIPT>
							</TBODY>
							</TABLE><BR>
							<TABLE id=NetPacketInfo_WLan cellSpacing=0 cellPadding=3 width="100%" border=1>
							<TBODY>
							<TR>
							<TD class=table_title align=middle>接口</TD>
							<TD class=table_title align=middle colSpan=4><CENTER>接收 </CENTER></TD>
							<TD class=table_title align=middle colSpan=4><CENTER>发送 </CENTER></TD>
							</TR>
							<TR>
							<TD class=table_title align=middle>&nbsp;</TD>
							<TD class=table_title align=middle>字节</TD>
							<TD class=table_title align=middle>包</TD>
							<TD class=table_title align=middle>错误</TD>
							<TD class=table_title align=middle>丢弃</TD>
							<TD class=table_title align=middle>字节</TD>
							<TD class=table_title align=middle>包</TD>
							<TD class=table_title align=middle>错误</TD>
							<TD class=table_title align=middle>丢弃</TD>
							</TR>
<SCRIPT language=javascript>
if (wlanacEnbl == "1")
{
	document.writeln("  <tr>");
	document.writeln(" <td class='table_title' align='center'>" + "无线" + "</td>");
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanRxBytes","s") %>" + "&nbsp;</td>");
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanRxFrames","s") %>" + "&nbsp;</td>");
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanRxErrFrames","s") %>" + "&nbsp;</td>");
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanRxDropFrames","s") %>" + "&nbsp;</td>");
					
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanTxBytes","s") %>" + "&nbsp;</td>");
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanTxFrames","s") %>" + "&nbsp;</td>");
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanTxErrFrames","s") %>" + "&nbsp;</td>");
	document.writeln("<td>" + "<% tcWebApi_get("Info_WLan11ac","wlanTxDropFrames","s") %>" + "&nbsp;</td>");
	document.writeln("  </tr>");
}
</SCRIPT>
								</TBODY>
								</TABLE>
								<TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
								<TBODY>
								<TR>
								<TD width="20%" height="28" align=middle class=table_title>SSID索引</TD>
								<TD class=table_title align=middle>SSID名称</TD>
								<TD class=table_title align=middle>安全配置</TD>
								<TD class=table_title align=middle>认证方式</TD>
								<TD class=table_title align=middle>加密 </TD>
								</TR>
<SCRIPT language=javascript>
var ssidacnum=<% tcWebApi_get("WLan11ac_Common","BssidNum","s") %>
if (wlanacEnbl == "1")
{
	for (var i = 0; i < ssidacnum; i++)
	{
<% if tcwebApi_get("WebCustom_Entry","isCTPONSNSupported","h") = "Yes" then %>
		if ( i > 0 )
		continue;
<% end if %> 
		var Auth = "";
		var Encrypt = "";
		var AuthSt = "已配置";
		if (WlanacInfo[i].BeaconType.indexOf('WEP') > -1)
		{
			Auth = WlanacInfo[i].BasicAuth;
			Encrypt = WlanacInfo[i].BasicEncrypt;
		}
		else if(WlanacInfo[i].BeaconType.indexOf('OPEN') > -1)
		{
			Auth = "OPENSYSTEM";
			Encrypt = "NONE";
			AuthSt = "未配置";
		}
		else 
		{
			Encrypt = WlanacInfo[i].WPAAuth;
			Auth = WlanacInfo[i].WPAEncrypt;
		}							
		document.writeln('<tr>' + '<td class="table_title" align="center">' + 'SSID-' + (i+1) + ':</td>' + '<td  align="center">' + WlanacInfo[i].ssid + '&nbsp;</td><td align="center">' + AuthSt + '&nbsp;</td>' + '&nbsp;</td><td align="center">' + Auth.toUpperCase () + '&nbsp;</td>' + '<td' + ' align="center">' + Encrypt.toUpperCase() + '&nbsp;</td>' + '</tr>');
	}
}
</SCRIPT>
								<BR>
								</TBODY>
								</TABLE>
							</TD>
							<TD width=10>&nbsp;</TD>
							</TR>
							<TR>
							<TD width=10>&nbsp;</TD>
							<TD><HR></TD>
							<TD width=10>&nbsp;</TD>
							</TR>
							</TBODY>
							</TABLE>
						</TD>
						</TR>
						<TR id=secWireless8>
						<TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
						<TD width=7 background=/img/panel3.gif>　</TD>
						<TD></TD>
						</TR>
<% end if %>          
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>以太网接口信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
              <TD width=474>　 
                </TD>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") ="Yes" then %>                 
                <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
                <TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#用户侧信息" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
                  </TBODY></TABLE></TD>
<% else %>
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>              
<% if TCWebApi_get("WebCustom_Entry","isWLanACSupported","h" ) <> "Yes" then %>                
								<TD vAlign=top width=170 background=/img/panel4.gif 
<% if tcwebApi_get("WebCustom_Entry","isCTSCSupported","h") = "Yes" then %>
									rowSpan=10
<% else %>
									rowSpan=7
<% end if %> 
									>
								<TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#用户侧信息" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE></TD>
<% end if %>                   
<SCRIPT language=javascript>
									  	setDisplay('secWireless1', 0);
									  	setDisplay('secWireless2', 0);
									  	setDisplay('secWireless3', 0);
											setDisplay('secWireless4', 0);
</SCRIPT>   
<% end if %>             
<% if TCWebApi_get("WebCustom_Entry","isWLanACSupported","h" ) <> "Yes" then %>     
<SCRIPT language=javascript>
									  	setDisplay('secWireless5', 0);
									  	setDisplay('secWireless6', 0);
									  	setDisplay('secWireless7', 0);
											setDisplay('secWireless8', 0);
</SCRIPT>
<% end if %>            
<% end if %>

            </TR>
        <TR>
                <TD vAlign=top width=157 bgColor=#e7e7e7 height=10> 
</TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
                </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P>
			<input type="hidden" name="dhcpIPs"> 
                  <iframe src="/cgi-bin/dhcp_ip.cgi" frameborder="0" width="0" ></iframe></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title width="24%">IP地址:</TD>
                      <TD width="76%"><% if TCWebApi_get("Lan_Entry0", "IP", "h") <> "N/A" then TCWebApi_get("Lan_Entry0", "IP", "s") end if %></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") <>"Yes" then %> 
                    <TR>
                      <TD class=table_title width="24%">IPv6地址:</TD>
                          <TD width="76%"> <% if TCWebApi_get("Lan_Entry0", "IP6", "h") <> "N/A" then TCWebApi_get("Lan_Entry0", "IP6", "s") end if %>
                            &nbsp;</TD>
                        </TR>
<% end if %>       
                    <TR>
                      <TD class=table_title>MAC地址:</TD>
                      <TD><% if TCWebApi_Get("Info_Ether", "mac", "h") <> "N/A" then TCWebApi_Get("Info_Ether", "mac", "s") end if %> </TD></TR></TBODY></TABLE>
                        <BR>
                  <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                    <TBODY>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
									<TR>
										<TD class=table_title width="24%">&nbsp; LAN &nbsp;</TD>
										<TD>
											<SCRIPT language=JavaScript type=text/javascript>
												if(PortConnStatus[0] == "1"
												|| PortConnStatus[1] == "1"
												|| PortConnStatus[2] == "1"
												|| PortConnStatus[3] == "1" )
												{
													document.write("连接上");
												}
												else
												{
													document.write("未连接");
												}
											</SCRIPT>
										&nbsp;</TD>
									</TR>
<% else %>
                    <TR>
                      <TD class=table_title align=middle>&nbsp; LAN-1 
&nbsp;</TD>
<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
                      <TD class=table_title align=middle>&nbsp; LAN-2 
&nbsp;</TD>
<% end if %>
<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
                      <TD class=table_title align=middle>&nbsp; LAN-3 
&nbsp;</TD>
                      <TD class=table_title align=middle>&nbsp; LAN-4 
&nbsp;</TD>
<% end if %>
         						</TR>
                    <TR>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						{
							if(PortConnStatus[0] == "1")
							{
								document.write("连接上");
							}
							else
							{
								document.write("未连接");
							}
						}
						</SCRIPT>
                        &nbsp;</TD>
<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
                              <TD> <script language=JavaScript type=text/javascript>
						//if (Ethernet.length > 2)
						{
							if(PortConnStatus[1] == "1")
							{
							document.write("连接上");
							}
							else
							{
							document.write("未连接");
							}
						}
												</SCRIPT>
                        &nbsp;</TD>
<% end if %>
<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>                        
<TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						//if (Ethernet.length > 3)
						{
							if(PortConnStatus[2] == "1")
							{
							document.write("连接上");
							}
							else
							{
							document.write("未连接");
							}
						}
						</SCRIPT>
                        &nbsp;</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						//if (Ethernet.length > 4)
						{
							if(PortConnStatus[3] == "1")
							{
							document.write("连接上");
							}
							else
							{
							document.write("未连接");
							}
						}
						</SCRIPT>
                                &nbsp;</TD>
<% end if %>
</TR>
<% end if %>
</TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle>&nbsp; 设备 &nbsp;</TD>
                      <TD class=table_title align=middle>&nbsp; IP地址 &nbsp;</TD>
                      <TD class=table_title align=middle>&nbsp; MAC地址&nbsp;</TD>
                      <TD class=table_title align=middle>&nbsp; 状态 
						&nbsp;</TD></TR>
                    	<SCRIPT language=JavaScript type=text/javascript>
						for(i = 0; i < dhcpnum; i++)
						{
							if(PortIp[i] != "N/A" || PortMac[i] != "N/A")
							{
								document.write('<TR align="center">');
								document.write('<TD align="center"> Computer &nbsp;</TD>');
								document.write('<TD align="center">' + PortIp[i] + '&nbsp;</TD>');
								document.write('<TD align="center">' + PortMac[i] + '&nbsp;</TD>');
								if(PortDhcp[i] == "1")
								{
									var arytemp = PortExpireTime[i].split(':');
									if("0" != arytemp[0]){
										document.write('<TD align="center">' + "租期剩余" + arytemp[0] + "天" + arytemp[1] + "小时" + arytemp[2] + "分钟" + arytemp[3] + '秒&nbsp;</TD>');
									}
									else{
										if(("0:0:0:0" == PortExpireTime[i]) || ("0" == PortExpireTime[i]))
											document.write('<TD align="center">' + "租期已满" + '&nbsp;</TD>');
										else{
												document.write('<TD align="center">' + "租期剩余" + arytemp[1] + "小时" + arytemp[2] + "分钟" + arytemp[3] + '秒&nbsp;</TD>');
										}
									}
								}
								else
								{
									document.write('<TD align="center">' + '静态分配' + '&nbsp;</TD>');
								}
								document.write('</TR>');
							}						
						}
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=NetPacketInfo_Ethe cellSpacing=0 cellPadding=3 
                  width="100%" border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle>接口</TD>
                      <TD class=table_title align=middle colSpan=4>
                        <CENTER>接收 </CENTER></TD>
                      <TD class=table_title align=middle colSpan=4>
                        <CENTER>发送 </CENTER></TD></TR>
                    <TR>
                      <TD class=table_title align=middle>&nbsp;</TD>
                      <TD class=table_title align=middle>字节</TD>
                      <TD class=table_title align=middle>包</TD>
                      <TD class=table_title align=middle>错误</TD>
                      <TD class=table_title align=middle>丢弃</TD>
                      <TD class=table_title align=middle>字节</TD>
                      <TD class=table_title align=middle>包</TD>
                      <TD class=table_title align=middle>错误</TD>
                      <TD class=table_title align=middle>丢弃</TD></TR>
                    <SCRIPT language=javascript>
					document.writeln("<tr>");
					document.writeln("<td class='table_title'>Ethernet"+"</td>");

					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","outOctets","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","txFrames","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","outErrors","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","outMulticastPkts","s") %>" + "&nbsp;</td>");
					
					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","inOctets","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","rxFrames","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","rxCrcErr","s") %>" + "&nbsp;</td>");
					document.writeln("<td>" + "<% tcWebApi_get("Info_Ether","inMulticastPkts","s") %>" + "&nbsp;</td>");
					
					document.writeln("</tr>");
					if (curUserName != sptUserName)
					{
						document.all("NetPacketInfo_WLan").style.display = "none";
						document.all("NetPacketInfo_Ethe").style.display = "none";
					}
					</SCRIPT>
                    </TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>             
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") <>"Yes" then %>
<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
<DIV id=usbinfo></DIV>
		        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>USB接口信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
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
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD align=middle>
                        <TABLE cellSpacing=0 cellPadding=3 width="100%" 
border=1>
                          <TBODY>
                          <TR>
                            <TD class=table_title width="24%">USB设备:</TD>
                            <TD width="76%">
                              <SCRIPT language=JavaScript type=text/javascript>
								if (usbvalue.length > 0)
								{
									document.write('已连接')
								}
								else
								{
									document.write('未连接')
								}
								</SCRIPT>
                            </TD></TR>
                          </TBODY></TABLE></TD></TR></TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>&nbsp;</TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
<% end if %> 
<% end if %> 
<% end if %> 
              <DIV></DIV>
<% if tcwebApi_get("WebCustom_Entry","isCTSCSupported","h") = "Yes" then %>
<script language=JavaScript type=text/javascript>
function onuObj( active, devtype, limitnum )
{
	this.Active = active;
	this.DevType = 	devtype;
	this.LimitNum = ( 'N/A' == limitnum ) ? '0' : limitnum;
	this.ActiveCHS = ( 'Yes' == active ) ? '启用' : '禁用';
}

var onuList = new Array();
onuList.push( new onuObj(
							  '<% tcwebApi_get("Accesslimit_Entry0", "Active", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry0", "devtype", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry0", "limitnum", "s") %>'
						) );
onuList.push( new onuObj(
							  '<% tcwebApi_get("Accesslimit_Entry1", "Active", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry1", "devtype", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry1", "limitnum", "s") %>'
						) );
onuList.push( new onuObj(
							  '<% tcwebApi_get("Accesslimit_Entry2", "Active", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry2", "devtype", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry2", "limitnum", "s") %>'
						) );
onuList.push( new onuObj(
							  '<% tcwebApi_get("Accesslimit_Entry3", "Active", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry3", "devtype", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry3", "limitnum", "s") %>'
						) );
onuList.push( new onuObj(
							  '<% tcwebApi_get("Accesslimit_Entry4", "Active", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry4", "devtype", "s") %>'
							, '<% tcwebApi_get("Accesslimit_Entry4", "limitnum", "s") %>'
						) );
function get_onuObj_byType( devtype )
{
	var len = onuList.length;
	var idx = 0;
	
	for ( idx = 0; idx < len; idx ++ )
	{
		if ( devtype == onuList[idx].DevType )
			return onuList[idx];
	}

	return null;
}
function get_onuObj_ActiveCHS_byType( devtype )
{
	var onuobj = get_onuObj_byType( devtype )

	if ( onuobj )
		return onuobj.ActiveCHS;

	return '';
}
function get_onuObj_LiminiNum_byType( devtype )
{
	var onuobj = get_onuObj_byType( devtype )

	if ( onuobj )
		return onuobj.LimitNum;

	return '';
}
</script>
				<TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>接入终端限制信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
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
              	<TD colspan='2'><b>接入终端限制信息说明：</b>
              		<br>
              		业务模式禁用：表示不启用限制同时接入公网的终端数量的业务;<br>
              		模式一：表示允许配置同时接入公网的最大数量模式；<br>
              		模式二：表示仅对每一种终端类型进行单独的数量限制。<br></TD>
            	</TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD align=middle>
                        <TABLE cellSpacing=0 cellPadding=3 width='100%' border=1>
                          <TBODY>
                          <TR>
                            <TD class=table_title width='40%'>业务模式</TD>
                            <TD width='64%' colspan='4'>
                             <% if tcWebApi_get("Accesslimit_Common", "mode", "h") = "1" then %>
                             模式一
                             <% elseif tcWebApi_get("Accesslimit_Common", "mode", "h") = "2" then %>
                             模式二
                             <% else %>
                             业务模式禁用
                             <% end if %>
                            </TD>
                          </TR>
                          <TR>
                            <TD class=table_title>同时接入公网的最大终端数目</TD>
                            <TD colspan='4'>
                            <% if tcWebApi_get("Accesslimit_Common", "totalnum", "h") <> "N/A" then %>
                            <% tcWebApi_get("Accesslimit_Common", "totalnum", "s") else %>
                            0
                            <% end if %>
                            </TD>
                          </TR>
                          <TR>
                            <TD class=table_title>接入公网的STB终端数目限制</TD>
                            <TD width='12%'>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_ActiveCHS_byType('STB') );
                            </script>
                            </TD>
                            <TD class=table_title width='12%'>最大数目</TD>
                            <TD width='12%'>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_LiminiNum_byType('STB') );
                            </script>
                            </TD>
                          </TR>
													<TR>
                            <TD class=table_title>接入公网的Camera终端数目限制</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_ActiveCHS_byType('Camera') );
                            </script>
                            </TD>
                            <TD class=table_title>最大数目</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_LiminiNum_byType('Camera') );
                            </script>
                            </TD>
                          </TR>
													<TR>
                            <TD class=table_title>接入公网的Computer终端数目限制</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_ActiveCHS_byType('Computer') );
                            </script>
                            </TD>
                            <TD class=table_title>最大数目</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_LiminiNum_byType('Computer') );
                            </script>
                            </TD>
                          </TR>
													<TR>
                            <TD class=table_title>接入公网的Phone终端数目限制</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_ActiveCHS_byType('Phone') );
                            </script>
                            </TD>
                            <TD class=table_title>最大数目</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_LiminiNum_byType('Phone') );
                            </script>
                            </TD>
                          </TR>
													<TR>
                            <TD class=table_title>接入公网的HGW终端数目限制</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_ActiveCHS_byType('HGW') );
                            </script>
                            </TD>
                            <TD class=table_title>最大数目</TD>
                            <TD>
                            <script language=JavaScript type=text/javascript>
                            document.write( get_onuObj_LiminiNum_byType('HGW') );
                            </script>
                            </TD>
                          </TR>
                          </TBODY></TABLE>
                     </TD>
                     </TR>
                    	</TBODY>
                    </TABLE>
                 </TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>&nbsp;</TD>
                <TD width=10>&nbsp;</TD></TR>
              </TBODY></TABLE>
        		</TD>
        </TR>         
<% end if %>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height="100%"></TD>
          <TD width=7 background=/img/panel3.gif></TD>
          <TD></TD></TR></TABLE>
    </TD></TR>   
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
