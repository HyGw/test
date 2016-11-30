<%
if Request_Form("Wan_Flag") = "1" then

	TCWebApi_unset("WanInfo_WanIF")
	TCWebApi_set("WanInfo_WanPVC","Action","OperatorStyle")
	TCWebApi_set("WanInfo_WanPVC","VPI","atmVpi")
	TCWebApi_set("WanInfo_WanPVC","VCI","atmVci")
	TCWebApi_set("WanInfo_WanPVC","ENCAP","EnCAPFlag")
	TCWebApi_set("WanInfo_WanPVC","QOS","atmServiceCategory")
	TCWebApi_set("WanInfo_WanPVC","PCR","atmPeakCellRate")
	if Request_Form("atmServiceCategory") <> "ubr" then
		TCWebApi_set("WanInfo_WanPVC","SCR","atmSustainedCellRate")
		TCWebApi_set("WanInfo_WanPVC","MBS","atmMaxBurstSize")
	end if

	TCWebApi_set("WanInfo_WanIF","Active","WanActive")
	TCWebApi_set("WanInfo_WanIF","ServiceList","serviceList")
	TCWebApi_set("WanInfo_WanIF","BandActive","bindflag")
	TCWebApi_set("WanInfo_WanIF","LAN1","bindlan1")
	TCWebApi_set("WanInfo_WanIF","LAN2","bindlan2")
	TCWebApi_set("WanInfo_WanIF","LAN3","bindlan3")
	TCWebApi_set("WanInfo_WanIF","LAN4","bindlan4")
	TCWebApi_set("WanInfo_WanIF","SSID1","bindwireless1")
	TCWebApi_set("WanInfo_WanIF","SSID2","bindwireless2")
	TCWebApi_set("WanInfo_WanIF","SSID3","bindwireless3")
	TCWebApi_set("WanInfo_WanIF","SSID4","bindwireless4")
	TCWebApi_set("WanInfo_WanIF","WanMode","wanMode")
	TCWebApi_set("WanInfo_WanIF","LinkMode","linkMode")
	if Request_Form("wanMode") = "Route" then
		if Request_Form("linkMode") = "linkPPP" then
			TCWebApi_set("WanInfo_WanIF","PPPGETIP","PPPGetIpFlag")
			TCWebApi_set("WanInfo_WanIF","CONNECTION","ConnectionFlag")
			TCWebApi_set("WanInfo_WanIF","USERNAME","pppUserName")
			TCWebApi_set("WanInfo_WanIF","PASSWORD","pppPassword")
			TCWebApi_set("WanInfo_WanIF","PPPManualStatus","pppManualStatus_Flag")
		end if
	elseif Request_Form("wanMode") = "Bridge" then
			TCWebApi_set("WanInfo_WanIF","BridgeMode","bridgeMode")
			TCWebApi_set("WanInfo_WanIF","DHCPRealy","dhcprelay")
	end if
	
	TCWebApi_set("WanInfo_WanIF","IPVERSION","IPVersionValue")
	TCWebApi_set("WanInfo_WanIF","ISP","ISPTypeValue")
	if Request_Form("ISPTypeValue") = "0" then
		TCWebApi_set("WanInfo_WanIF","option60String","wanDhcpCode")
	end if
	
	TCWebApi_set("WanInfo_WanIF","dot1q","vlanId")
	if Request_Form("vlanId") = "Yes" then
		TCWebApi_set("WanInfo_WanIF","VLANID","vlan")
	end if
	TCWebApi_set("WanInfo_WanIF","dot1p","vlanPri")
	if Request_Form("vlanPri") = "Yes" then
		TCWebApi_set("WanInfo_WanIF","dot1pData","v8021d")
	end if
	
	TCWebApi_set("WanInfo_WanIF","NATENABLE","nat")
	TCWebApi_set("WanInfo_WanIF","IGMPproxy","enblIgmp")
	
	if Request_Form("IPVersionValue") <> "IPv6" then
		if Request_Form("linkMode") = "linkIP" then
			if Request_Form("IpMode") = "Static" then
				TCWebApi_set("WanInfo_WanIF","IPADDR","wanIpAddress")
				TCWebApi_set("WanInfo_WanIF","NETMASK","wanSubnetMask")
				TCWebApi_set("WanInfo_WanIF","GATEWAY","defaultGateway")
				TCWebApi_set("WanInfo_WanIF","DNS","dnsPrimary")
				TCWebApi_set("WanInfo_WanIF","SecDNS","dnsSecondary")
			end if
		end if
	end if

	if Request_Form("IPVersionValue") <> "IPv4" then
			TCWebApi_set("WanInfo_WanIF","DHCPv6","pppv6Mode")
			TCWebApi_set("WanInfo_WanIF","DHCPv6PD","dhcpv6pdflag")
			TCWebApi_set("WanInfo_WanIF","GW6_Manual","Disable_Flag")
		if Request_Form("IdIpv6AddrType") = "Static" then
			TCWebApi_set("WanInfo_WanIF","IPADDR6","IdIpv6Addr")
			TCWebApi_set("WanInfo_WanIF","GATEWAY6","IdIpv6Gateway")
			if Request_Form("IdIpv6Gateway") <> "" then
				TCWebApi_set("WanInfo_WanIF","GW6_Manual","Enable_Flag")
			end if
			TCWebApi_set("WanInfo_WanIF","PREFIX6","IdIpv6PrefixLen")
			TCWebApi_set("WanInfo_WanIF","DNS6","IdIpv6Dns1")
			TCWebApi_set("WanInfo_WanIF","SecDNS6","IdIpv6Dns2")
		elseif Request_Form("IdIpv6AddrType") = "DHCP" then
			TCWebApi_set("WanInfo_WanIF","GATEWAY6","IdIpv6Gateway")
			if Request_Form("IdIpv6Gateway") <> "" then
				TCWebApi_set("WanInfo_WanIF","GW6_Manual","Enable_Flag")
			end if
		end if
	end if

	TCWebApi_commit("WanInfo_WanPVC")
	TCWebApi_save()
elseif Request_Form("Wan_Flag") = "2" then
	 TCWebApi_set("WanInfo_Common","CurIFIndex","curSetIndex")
	 TCWebApi_commit("WanInfo_Common")
elseif Request_Form("Wan_Flag") = "3" then
	 TCWebApi_set("WanInfo_WanPVC","Action","OperatorStyle")
	 TCWebApi_set("WanInfo_Common","CurIFIndex","curSetIndex")
	 TCWebApi_commit("WanInfo_WanPVC")
	 TCWebApi_set("WanInfo_Common","CurIFIndex","afterdeleteFlag")
	 TCWebApi_save()
elseif Request_Form("Wan_Flag") = "4" then
	 TCWebApi_set("WanInfo_WanIF","PPPManualStatus","pppManualStatus_Flag")
	 TCWebApi_commit("WanInfo_WanIF")
	 TCWebApi_save()
else
	 TCWebApi_set("WanInfo_Common","ErrCode","Wan_Flag")
end if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>宽带设置</TITLE>
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
            value="网络->宽带设置" name=Selected_Menu> </FONT></B><SPAN 
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
	  
	var nEntryNum = "<% tcWebApi_get("WanInfo_Common","CycleNum","s") %>";
	var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryName = vArrayStr.split(','); 
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","ValidIFIndex","s") %>";
	var vEntryIndex = vArrayStr.split(',');
	var vCurrentDHCPv6 = "<% tcWebApi_get("WanInfo_WanIF","DHCPv6","s") %>";
	var vBindStatus = "<% tcWebApi_get("WanInfo_Common","BindStatus","s") %>";
	var ppp_flag = 2;
	var manual_flag = 2;
	var vcurConnect = "<% tcWebApi_get("WanInfo_WanIF","CONNECTION","s") %>";
	if(vcurConnect == "Connect_Keep_Alive")
		ppp_flag = 0;
	else if(vcurConnect == "Connect_Manually")
		ppp_flag = 1;	
	vcurConnect = "<% tcWebApi_get("WanInfo_WanIF","PPPManualStatus","s") %>";
	if(vcurConnect == "connect")
		manual_flag = 0;
	else if((vcurConnect == "disconnect"))
		manual_flag = 1;
function WanIPConstruction(domain,conName,vlanId,vlanPri,vlanEnable,bindstr,ConnectionType, nat, enblService, wanIpAddress,serviceList,dnsstr,addrType,wanSubnetMask,defaultGateway,DHCPRelay,DhcpCode,xIpv4Enable,xIpv6Enable,xIpv6Status,xIpv6AddrType,xIpv6Addr,xIpv6PrefixLen,xIpv6Gateway,xIpv6Dns)
{
this.domain = domain;
var list = domain.split('.');
this.key =     '.' + list[4] + '.I.' + list[6];
this.wanConn = 'IPCon';
this.wanId = this.domain;
this.conName = conName;
this.vlanId = vlanId;
this.vlanPri = vlanPri;
this.vlanEnable = vlanEnable;
this.bind = bindstr;
this.multMode = 0;
this.bindflag = 1;
this.ConnectionType = ConnectionType;
this.nat = getBoolValue(nat);
this.enblService = getBoolValue(enblService);
this.wanIpAddress = wanIpAddress;
this.serviceList = serviceList;
var dns = dnsstr.split(',');
this.dnsPrimary = dns[0];
this.dnsSecondary = dns[1];
this.addrType = addrType;
this.wanSubnetMask = wanSubnetMask;
this.defaultGateway = defaultGateway;
this.atmVpi = '';
this.atmVci = '';
this.LinkType = '';
this.atmServiceCategory = '';
this.atmPeakCellRate = '';
this.atmSustainedCellRate = '';
this.atmMaxBurstSize = '';
this.encapMode = '';
this.DHCPRelay = DHCPRelay;
this.ProxyEnable = '';
this.Relating = ' ';
this.DhcpCode=DhcpCode;
this.xIpv4Enable = xIpv4Enable;
this.xIpv6Enable = xIpv6Enable;
this.xIpv6Status = xIpv6Status;
this.xIpv6AddrType = xIpv6AddrType;
this.xIpv6Addr = xIpv6Addr;
this.xIpv6PrefixLen = xIpv6PrefixLen;
if (this.xIpv6PrefixLen == '0')
{
this.xIpv6PrefixLen = '';
}
this.xIpv6Gateway = xIpv6Gateway;
var Ipv6DnsServer = xIpv6Dns.split(',');
this.xIpv6Dns1 = Ipv6DnsServer[0];
if (Ipv6DnsServer.length > 1)
{
this.xIpv6Dns2 = Ipv6DnsServer[1];
}
else
{
this.xIpv6Dns2 = '';
}
}
function WanPPPConstruction(domain,conName,vlanId,vlanPri,vlanEnable,bindstr,ConnectionType, nat,enblService, wanIpAddress,serviceList,dnsstr,RemoteIPAddress,pppUserName, pppPassword,CntTrigger,ProxyEnable,pppIdleTimeout,DHCPRelay, ConnectionStatus,xIpv4Enable,xIpv6Enable,xIpv6Status,xIpv6AddrType,xIpv6Addr,xIpv6PrefixLen,xIpv6Gateway,xIpv6Dns)
{
this.domain = domain;
var list = domain.split('.');
this.key = '.' + list[4] + '.P.' + list[6];
this.wanConn = 'PPPCon';
this.wanId = this.domain;
this.conName = conName;
this.vlanId = vlanId;
this.vlanPri = vlanPri;
this.vlanEnable = vlanEnable;
this.bind = bindstr;
this.multMode = 0;
this.bindflag = 1;
this.ConnectionType = ConnectionType;
this.nat = getBoolValue(nat);
this.enblService = getBoolValue(enblService);
this.wanIpAddress = wanIpAddress;
this.serviceList = serviceList;
var dns = dnsstr.split(',');
this.dnsPrimary = dns[0];
this.dnsSecondary = dns[1];
this.defaultGateway = RemoteIPAddress;
this.pppUserName = pppUserName;
this.pppPassword = pppPassword;
this.ProxyEnable = ProxyEnable;
this.pppIdleTimeout = pppIdleTimeout;
this.atmVpi = '';
this.atmVci = '';
this.LinkType = '';
this.atmServiceCategory = '';
this.atmPeakCellRate = '';
this.atmSustainedCellRate = '';
this.atmMaxBurstSize = '';
this.encapMode = '';
this.cntMode = CntTrigger;
this.Status = ConnectionStatus;
this.Relating = ' ';
this.DHCPRelay = DHCPRelay;
this.xIpv4Enable = xIpv4Enable;
this.xIpv6Enable = xIpv6Enable;
this.xIpv6Status = xIpv6Status;
this.xIpv6AddrType = xIpv6AddrType;
this.xIpv6Addr = xIpv6Addr;
this.xIpv6PrefixLen = xIpv6PrefixLen;
if (this.xIpv6PrefixLen == '0')
{
this.xIpv6PrefixLen = '';
}
this.xIpv6Gateway = xIpv6Gateway;
var Ipv6DnsServer = xIpv6Dns.split(',');
this.xIpv6Dns1 = Ipv6DnsServer[0];
if (Ipv6DnsServer.length > 1)
{
this.xIpv6Dns2 = Ipv6DnsServer[1];
}
else
{
this.xIpv6Dns2 = '';
}
}
function trimString(destStr, cTrim)
{
var i;
var j;
var retStr = '';
for (i = 0; i < destStr.length; i++)
{
if (destStr.charAt(i) != cTrim)
{
retStr += destStr.charAt(i);
}
}
return retStr;
}
function PvcConstruction(domain,atmPvc,atmQoS,atmPeakCellRate,LinkType,atmSustainedCellRate,atmMaxBurstSize,encapMode)
{
this.domain = domain;
var list = domain.split('.');
this.key =     '.' + list[4] + '.';
var realPvc = trimString(atmPvc, ' ');
if(realPvc.charAt(0) == 'P')
{
realPvc = realPvc.substr(4);
}
var pvc = realPvc.split('/');
this.atmVpi = pvc[0];
this.atmVci = pvc[1];
this.LinkType = LinkType;    
this.atmServiceCategory = atmQoS;
this.atmPeakCellRate = atmPeakCellRate;
this.atmSustainedCellRate = atmSustainedCellRate;
this.atmMaxBurstSize = atmMaxBurstSize;
this.encapMode = encapMode;
}
var pppUsrAccess = '|Subscriber,';
var pppUsrAccessArr = pppUsrAccess.split(",");
var pppPwdAccess = '|Subscriber,';
var pppPwdAccessArr = pppPwdAccess.split(",");
var WanPPP = new Array(new WanPPPConstruction("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANPPPConnection.2","1_TR069_INTERNET_R_0_35","0","255","0","","IP_Routed","1","1","","TR069_INTERNET","","","trendchip","trendchip","AlwaysOn","0","0","1","PendingDisconnect","1","1","PendingDisconnect","DHCP","","0","",""),null);
var WanIP = new Array(null);
var CntPvc = new Array(new PvcConstruction("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANDSLLinkConfig","PVC:0/35","UBR","0","EoA","0","0","LLC"),null);
if (WanIP.length > 1)
AssociateParam('WanIP','CntPvc','atmVpi|atmVci|atmServiceCategory|atmPeakCellRate|LinkType|atmSustainedCellRate|atmMaxBurstSize|encapMode');
if (WanPPP.length > 1)
AssociateParam('WanPPP','CntPvc','atmVpi|atmVci|atmServiceCategory|atmPeakCellRate|LinkType|atmSustainedCellRate|atmMaxBurstSize|encapMode');
function ipv6mode(domain, mode)
{
this.domain = domain;
this.mode = mode;
}
var ipv6enable = new Array(new ipv6mode("InternetGatewayDevice.DeviceInfo.X_CT-COM_IPProtocolVersion","3"),null);
var ipv6version = ipv6enable[0].mode;
var Wan = Array();
for (i = 0; i < WanIP.length-1; i++)
{
Wan[i] = WanIP[i];
}
for (j = 0; j < WanPPP.length-1; j++,i++)
{
Wan[i] = WanPPP[j];
}
var upRate = parseInt('0');
//var pcrMax = 1024;
var pcrMax = 5500;
if (upRate != 0)
pcrMax = Math.floor((upRate * 1000) / (53 * 8));
var i = 0;
var AddFlag = false;
var SelWanIndex = -1;
var pvcByUseIndex = -1;
var pvcByUseCount = 0;
var wanList = '';
var changePVCFlag = true;

var msg = new Array(6);
msg[0] = "操作成功！";
msg[1] = "修改失败,一条PVC下只能创建8条Interface！";
msg[2] = "修改失败,PVC已满,只能创建8条PVC！";
msg[3] = "创建失败,此PVC已经存在8条Interface！";
msg[4] = "创建失败,PVC已满,只能创建8条PVC！";
msg[5] = "删除错误, Interface不存在！";

function LoadFrame()
{
with (getElById('ConfigForm'))
{
	Wan_Flag.value = "0";
	var wanStatus = "<% tcWebApi_get("WanInfo_Common","ErrCode","s") %>";
	if((0 != parseInt(wanStatus)) && (wanStatus != "N/A"))
	{
		if(99 == parseInt(wanStatus))
		{
			alert(msg[5]);
		}
		else{
			alert(msg[parseInt(wanStatus)]);
		}
		document.ConfigForm.submit();
	}
	//wanId.selectedIndex = 0;
	if ((CurWan.length-1) > 0)
	{
		//wanId.selectedIndex = 0;
		//WanIdChange();
		WanModeChange();
		//IpModeChange();
		if (serviceList.value == "TR069")
		{
			dhcpv6pdflag.value = "No";
			setDisplay('secBind',0);
			setDisplay('secNat',0);
			clearBindList();
		}
		else
		{
			var ipVer = getRadioVal("IpVersion");
			setDisplay('secBind',1);
			if((wanMode.value == "Bridge") || ("IPv6" == ipVer)) setDisplay('secNat',0);
			else setDisplay('secNat',1);
		}
		if(linkMode.value == "linkPPP")
			DialMethodChange();
		atmServiceCategoryChange();
		Enbl8021qChange();
		Enbl8021dChange();
	}
	else
	{
		onChangeSvrList();
	}
}
}

function IpVersionChange()
{
	with (getElById('ConfigForm'))
	{
		var ipVer = getRadioVal("IpVersion");
		var ConnType = getSelectVal('wanMode');
		var Serverlist = getSelectVal('serviceList');
		if (ConnType != 'Route')
		{
			setDisplay('divIpVersion', 0);
			setDisplay('secIPv6Div', 0);
			return;
		}
		setDisplay('divIpVersion', 1);
		setDisplay('secIPv6Div', 1);
		if ("IPv4" == ipVer)
		{
			if (Serverlist == "TR069")
			{
				setDisplay('secNat', 0);
				nat.value = "Disabled";
			}
			else
			{
				setDisplay('secNat', 1);
				nat.value = "Enable";
			}
			if ('linkIP' == getSelectVal('linkMode'))
			{
				setDisplay('secDhcp', 1);
				setDisplay('secStatic', 1);
				setDisplay('secPppoeItems', 0);
				if (SelWanIndex != -1)
				{
					if ((Wan[SelWanIndex].addrType == 'DHCP') || (Wan[SelWanIndex].wanConn == "PPPCon"))
					{
						IpMode[0].checked = true;
					}
					else
					{
						IpMode[1].checked = true;
					}
				}
				else
				{
					//IpMode[0].checked = true;
				}
			}
			else
			{
				setDisplay('secDhcp', 0);
				setDisplay('secStatic', 0);
				setDisplay('secPppoeItems', 1);
			}
			setDisplay('secPppoe', 0);
			setDisplay('secPppoa', 0);
			setDisplay('secIpoa', 0);
			//if (("linkIP" == getSelectVal('linkMode')) && ("Static" == getSelectVal('IpMode')))
			if(IpMode[1].checked && ("linkIP" == getSelectVal('linkMode')))
			{
				setDisplay('secStaticItems', 1);
			}
			else
			{
				setDisplay('secStaticItems', 0);
			}
			setDisplay('TrIpv6AddrType', 0);
			setDisplay('TrIpv6Addr', 0);
			setDisplay('TrIpv6Dns1', 0);
			setDisplay('TrIpv6Dns2', 0);
			setDisplay('TrIpv6Gateway', 0);
			setDisplay('TrIpv6GatewayInfo', 0);
		}
		else if ("IPv6" == ipVer)
		{
			setDisplay('secNat', 0);
			nat.value = "Disabled";
			setDisplay('secDhcp', 0);
			setDisplay('secStatic', 0);
			setDisplay('secPppoe', 0);
			setDisplay('secPppoa', 0);
			setDisplay('secIpoa', 0);
			setDisplay('secDhcpItems', 0);
			setDisplay('secStaticItems', 0);
			setDisplay('TrIpv6AddrType', 1);
			var linkstr = getSelectVal('linkMode');
			if(linkstr == "linkIP")
				WriteIPv6List(1);
			else
				WriteIPv6List(0);
			/*var AddrType = getSelectVal('IdIpv6AddrType');
			if (AddrType == 'SLAAC')
			{
				setDisplay('TrIpv6Addr', 0);
				setDisplay('TrIpv6Dns1', 0);
				setDisplay('TrIpv6Dns2', 0);
				setDisplay('TrIpv6Gateway', 0);
				ISPTypeValue.value = "0";
			}
			else if (AddrType == 'DHCP')
			{
				setDisplay('TrIpv6Addr', 0);
				setDisplay('TrIpv6Dns1', 0);
				setDisplay('TrIpv6Dns2', 0);
				setDisplay('TrIpv6Gateway', 1);
				ISPTypeValue.value = "0";
			}
			//else if (AddrType == 'Static')
			if(linkstr == "linkIP")
			{
				if (AddrType == 'Static')
				{
					setDisplay('TrIpv6Addr', 1);
					setDisplay('TrIpv6Dns1', 1);
					setDisplay('TrIpv6Dns2', 1);
					setDisplay('TrIpv6Gateway', 1);
					ISPTypeValue.value = "1";
				}
			}*/
		}
		else
		{
			if (Serverlist == "TR069")
			{
				setDisplay('secNat', 0);
				nat.value = "Disabled";
			}
			else
			{
				setDisplay('secNat', 1);
				nat.value = "Enable";
			}
			if ('linkIP' == getSelectVal('linkMode'))
			{
				setDisplay('secDhcp', 1);
				setDisplay('secStatic', 1);
				setDisplay('secPppoeItems', 0);
				if (SelWanIndex != -1)
				{
					if ((Wan[SelWanIndex].addrType == 'DHCP') || (Wan[SelWanIndex].wanConn == "PPPCon"))
					{
						IpMode[0].checked = true;
					}
					else
					{
						IpMode[1].checked = true;
					}
				}
				else
				{
					//IpMode[0].checked = true;
				}
			}
			else
			{
				setDisplay('secDhcp', 0);
				setDisplay('secStatic', 0);
				setDisplay('secPppoeItems', 1);
			}
				setDisplay('secPppoe', 0);
				setDisplay('secPppoa', 0);
				setDisplay('secIpoa', 0);
				if (('linkIP' == getSelectVal('linkMode')) && IpMode[1].checked)
				{
					setDisplay('secStaticItems', 1);
				}
				else
				{
					setDisplay('secStaticItems', 0);
				}
				setDisplay('TrIpv6AddrType', 1);
				if('linkIP' == getSelectVal('linkMode'))
				{
					if(IpMode[0].checked)
					{
						WriteIPv6List(0);
					}
					else if(IpMode[1].checked)
						WriteIPv6List(2);
					else
						WriteIPv6List(0);
				}
				else
					WriteIPv6List(0);
				/*var AddrType = getSelectVal('IdIpv6AddrType');
				if (AddrType == 'SLAAC')
				{
					setDisplay('TrIpv6Addr', 0);
					setDisplay('TrIpv6Dns1', 0);
					setDisplay('TrIpv6Dns2', 0);
					setDisplay('TrIpv6Gateway', 0);
				}
				else if (AddrType == 'DHCP')
				{
					setDisplay('TrIpv6Addr', 0);
					setDisplay('TrIpv6Dns1', 0);
					setDisplay('TrIpv6Dns2', 0);
					setDisplay('TrIpv6Gateway', 1);
				}
				else if (AddrType == 'Static')
				{
					setDisplay('TrIpv6Addr', 1);
					setDisplay('TrIpv6Dns1', 1);
					setDisplay('TrIpv6Dns2', 1);
					setDisplay('TrIpv6Gateway', 1);
				}*/
			}
	}
}
var changeflag = 1;
function onChangeSvrList()
{
with (getElById('ConfigForm'))
{
	if ((serviceList.value == 0) && (IpMode[2].checked == true) && (wanMode.value != "Bridge"))
	{
		//removeOption(DialMethod,1);
		//removeOption(DialMethod,'Manual');
		changeflag = 0;
		secManualDial.style.display = "none";
		secIdleTime.style.display = "none";
	}
	else if((serviceList.value != 0) && (IpMode[2].checked == true) && (wanMode.value != "Bridge"))
	{
		if(changeflag == 0)
		{
			addOption(DialMethod,1,"有流量时自动连接");
			addOption(DialMethod,'Manual',"手动拨号");
			changeflag = 1;
		}
	}
	if (serviceList.value == "TR069")
	{
		dhcpv6pdflag.value = "No";
		cb_nat.checked = false;
		nat.value = "Disabled";
		setDisplay('secBind',0);
		setDisplay('secNat',0);
		clearBindList();
	}
	else
	{
		dhcpv6pdflag.value = "Yes";
		cb_nat.checked = true;
		nat.value = "Enable";
		setDisplay('secBind',1);
		if(wanMode.value == "Bridge")
		{
			setDisplay('secNat',0);
			nat.value = "Disabled";
		}
		else
		{
			setDisplay('secNat',1);
		}
	}
	if (AddFlag == true)
	{
		if (serviceList.value == "OTHER" && wanMode.value == "Bridge")
		{
			cb_dhcprelay.checked = true;
		}
		else
		{
			cb_dhcprelay.checked = false;
		}
	}
	IpVersionChange();
}
}
function onSelectSvrList()
{
	onChangeSvrList();
with (getElById('ConfigForm'))
{
	if (serviceList.value == "OTHER" && wanMode.value == "Bridge")
	{
		cb_dhcprelay.checked = true;
	}
	else
	{
		cb_dhcprelay.checked = false;
	}
}
}
function DialMethodChange()
{
	var cntMode = getSelectVal('DialMethod');
	if (cntMode == 'AlwaysOn')
	{
		setDisplay('secIdleTime',0);
		setDisplay('secManualDial',0);
		setText('ConnectionFlag', "Connect_Keep_Alive");
	}
	else if (cntMode == 'Manual')
	{
		setDisplay('secIdleTime',0);
		setDisplay('secManualDial',1);
		setText('ConnectionFlag', "Connect_Manually");
		if(0 == ppp_flag){
			setDisable('pppDialButton',1);
			setText('pppDialButton', '手动拨号');
		}
		else if(1 == ppp_flag){
			if(0 == manual_flag){
				setDisable('pppDialButton',0);
				setText('pppDialButton', '断开连接');
				setText('pppManualStatus_Flag', 'disconnect');
			}
			else{
				setDisable('pppDialButton',0);
				setText('pppDialButton', '手动拨号');
				setText('pppManualStatus_Flag', 'connect');
			}
			
		}
		else
		{
			setDisable('pppDialButton',1);
			setText('pppDialButton', '手动拨号');
		}
		/*if (index == -1)
		{
			setDisable('pppDialButton',1);
			setText('pppDialButton', '手动拨号');
		}
		else if (Wan[index].Status == 'Connected')
		{
			setDisable('pppDialButton',0);
			setText('pppDialButton', '断开连接');
		}
		else
		{
			setDisable('pppDialButton',0);
			setText('pppDialButton', '手动拨号');
		}*/
	}
}
function cb_enblServiceChange()
{
	if(document.ConfigForm.cb_enblService.checked)
		document.ConfigForm.WanActive.value = "Yes";
	else
		document.ConfigForm.WanActive.value = "No";
}
function clearBindList()
{
for (var i = 1; i <= 4; i++)
{
document.getElementById("secLan" + i).disabled = false;
document.getElementById("cb_bindlan" + i).checked = false;
document.getElementById("secWireless" + i).disabled = false;
document.getElementById("cb_bindwireless" + i).checked = false;
}
}
function linkModeSelect()
{
with (getElById('ConfigForm'))
{
	var ipVer = getRadioVal("IpVersion");
	if (getSelectVal('linkMode') == 'linkIP')
	{
		if("IPv6" == ipVer)
			WriteIPv6List(1);
		else if("IPv4/IPv6" == ipVer)
		{
			if(IpMode[0].checked)
				WriteIPv6List(0);
			else if(IpMode[1].checked)
				WriteIPv6List(2);
			else
				WriteIPv6List(0);
		}
		setDisplay("secDhcp", 1);
		setDisplay('secStatic',1);
		if (SelWanIndex != -1)
		{
			/*if ((Wan[SelWanIndex].addrType == 'DHCP') || (Wan[SelWanIndex].wanConn == 'PPPCon'))
			{
				IpMode[0].checked = true;
			}
			else
			{
				IpMode[1].checked = true;
			}*/
		}
		else
		{
			IpMode[0].checked = true;
		}
	}
	else
	{
		if("IPv4" != ipVer)
			WriteIPv6List(0);
		setDisplay("secDhcp", 0);
		setDisplay('secStatic',0);
		IpMode[2].checked = true;
		DialMethodChange();
	}
	setDisplay('secPppoe',0);
	IpModeChange();
	IpVersionChange();
}
}
function WanModeChange()
{
with (getElById('ConfigForm'))
{
	if (wanMode.value == "Route")
	{
		setDisplay('secIpMode',1);
		setDisplay('secRouteItems',1);
		setDisplay('divLink', 1);
		setDisplay('secDhcp',1);
		setDisplay('secStatic',1);
		setDisplay('secPppoe',1);
		setDisplay('secbridgeDhcprelay',0);
		setDisplay('secBridgeType',0);
		if (serviceList.value == "TR069")
		{
			setDisplay('secNat',0);
		}
		else
		{
			setDisplay('secNat',1);
		}
		setDisplay('secIgmp',1);
		if (SelWanIndex != -1)
		{
/*if ((Wan[SelWanIndex].xIpv4Enable == '1') && (Wan[SelWanIndex].xIpv6Enable == '1'))
{
setRadio("IpVersion", "IPv4/IPv6");
}
else if ((Wan[SelWanIndex].xIpv4Enable == '1') && (Wan[SelWanIndex].xIpv6Enable == '0'))
{
setRadio("IpVersion", "IPv4");
}
else
{
setRadio("IpVersion", "IPv6");
}*/
		}
		else
		{
/*if (ipv6version == 1)
{
setRadio('IpVersion', 'IPv4');
}
else if (ipv6version == 2)
{
setRadio('IpVersion', 'IPv6');
}
else
{
setRadio('IpVersion', 'IPv4/IPv6');
}*/
		}
		//linkModeSelect();
		IpModeChange();
	}
	else if (wanMode.value == "Bridge")
	{
		setDisplay('secIpMode',0);
		setDisplay('secRouteItems',0);
		setDisplay('divLink', 0);
		setDisplay('secStaticItems',0);
		setDisplay('secPppoeItems',0);
		setDisplay('secDhcpItems',0);
		setDisplay('secDhcp',0);
		setDisplay('secStatic',0);
		setDisplay('secPppoe',0);
		setDisplay('secBridgeType',1);
		setDisplay('cb_dhcprelay',1);
		getElement('secbridgeDhcprelay').style.display = "";
		setRadio("IpVersion", "IPv4");
		setDisplay('secNat',0);
		nat.value = "Disabled";
		setDisplay('secIgmp',0);
	}
	else if (wanMode.value == "multMode")
	{
		setDisplay('secIpMode',1);
		setDisplay('secRouteItems',1);
		setDisplay('secDhcp',0);
		setDisplay('secStatic',0);
		setDisplay('secPppoe',1);
		setDisplay('secbridgeDhcprelay',0);
		IpMode[2].checked = true;
		setDisplay('secNat',1);
		setDisplay('secIgmp',1);
		IpModeChange();
	}
	if (AddFlag == true)
	{
		if (serviceList.value == "OTHER" && wanMode.value == "Bridge")
		{
			cb_dhcprelay.checked = true;
		}
		else
		{
			cb_dhcprelay.checked = false;
		}
	}
	IpVersionChange();
}
}
function WanModeSelect()
{
WanModeChange();
with (getElById('ConfigForm'))
{
if (serviceList.value == "OTHER" && wanMode.value == "Bridge")
{
cb_dhcprelay.checked = true;
}
else
{
cb_dhcprelay.checked = false;
}
}
}
function IpModeChange()
{
with (getElById('ConfigForm'))
{
	var ipVer = getRadioVal("IpVersion");
if (IpMode[0].checked == true)
{
setDisplay('secStaticItems',0);
setDisplay('secPppoeItems',0);
setDisplay('secDhcpItems',1);
document.ConfigForm.ISPTypeValue.value = "0";
	if("IPv4/IPv6" == ipVer){
		if(getSelectVal('linkMode') == 'linkIP')
			WriteIPv6List(0);
	}
		
}
else if (IpMode[1].checked == true)
{
setDisplay('secStaticItems',1);
setDisplay('secPppoeItems',0);
setDisplay('secDhcpItems',0);
document.ConfigForm.ISPTypeValue.value = "1";
	if("IPv4/IPv6" == ipVer){
		if(getSelectVal('linkMode') == 'linkIP')
			WriteIPv6List(2);
	}
}
else if (IpMode[2].checked == true)
{
setDisplay('secStaticItems',0);
setDisplay('secPppoeItems',1);
setDisplay('secDhcpItems',0);
document.ConfigForm.ISPTypeValue.value = "2";
}
else if (IpMode[3].checked == true)
{
setDisplay('secStaticItems',0);
setDisplay('secPppoeItems',1);
setDisplay('secDhcpItems',0);
document.ConfigForm.ISPTypeValue.value = "3";
}
else if (IpMode[4].checked == true)
{
setDisplay('secStaticItems',1);
setDisplay('secPppoeItems',0);
setDisplay('secDhcpItems',0);
document.ConfigForm.ISPTypeValue.value = "4";
}
//document.ConfigForm.Wan_Flag.value = "0";
//document.ConfigForm.submit();
}
}
function cb_bindflagChange()
{
with (getElById('ConfigForm'))
{
	if (cb_bindflag.checked == true)
	{
		bindflag.value = "Yes";
		setDisplay('secBind',1);
		if(wanMode.value == "Bridge")
			setDisplay('secbridgeDhcprelay',1);
		else
			setDisplay('secbridgeDhcprelay',0);
		
	}
	else
	{
		bindflag.value = "No";
		setDisplay('secBind',0);
		setDisplay('secbridgeDhcprelay',0);
	}
}
}
function Enbl8021dChange()
{
with (getElById('ConfigForm'))
{
if (enbl8021d.checked == true)
{
setDisplay('sec8021d',1);
document.ConfigForm.vlanPri.value = "Yes";
}
else
{
setDisplay('sec8021d',0);
document.ConfigForm.vlanPri.value = "No";
}
}
}
function Enbl8021qChange()
{
with (getElById('ConfigForm'))
{
if (enbl8021q.checked == true)
{
setDisplay('secVlan',1);
document.ConfigForm.vlanId.value = "Yes";
}
else
{
setDisplay('secVlan',0);
document.ConfigForm.vlanId.value = "No";
}
}
}
function	EnableNatClick()
{
	if(document.ConfigForm.cb_nat.checked)
		document.ConfigForm.nat.value = "Enable";
	else
		document.ConfigForm.nat.value = "Disabled";
}
function	EnableIGMPProxyClick()
{
	if(document.ConfigForm.cb_enblIgmp.checked)
		document.ConfigForm.enblIgmp.value = "Yes";
	else
		document.ConfigForm.enblIgmp.value = "No";
}
function EnableDHCPRealy()
{
	if(document.ConfigForm.cb_dhcprelay.checked)
		document.ConfigForm.dhcprelay.value = "Yes";
	else
		document.ConfigForm.dhcprelay.value = "No";
}
function atmServiceCategoryChange()
{
with (getElById('ConfigForm'))
{
switch (atmServiceCategory.value)
{
case "ubr":
setDisplay('secAtmPeakCellRate',0);
setDisplay('secAtmSustainedCellRate',0);
setDisplay('secAtmMaxBurstSize',0);
break;
case "ubr+":
case "cbr":
secAtmPeakCellRate.style.display = "";
secAtmSustainedCellRate.style.display = "none";
secAtmMaxBurstSize.style.display = "none";
break;
case "nrt-vbr":
case "rt-vbr":
secAtmPeakCellRate.style.display = "";
secAtmSustainedCellRate.style.display = "";
secAtmMaxBurstSize.style.display = "";
break;
}
}
}
function getWanList(list,index)
{
var temp = Wan[index].domain.split('.');
if (list == '')
{
return (temp[4] + '.' + temp[5] + '.' + temp[6]);
}
else
{
return ('|' + temp[4] + '.' + temp[5] + '.' + temp[6]);
}
}
function isDigit(val) {
if (val < '0' || val > '9')
return false;
return true;
}
function isDecimalDigit(digit)
{
if ( digit == "" )
{
return false;
}
for ( var i = 0 ; i < digit.length ; i++ )
{
if ( !isDigit(digit.charAt(i)) )
{
return false;
}
}
return true;
}
function isUseableIpAddress(address)
{
var num = 0;
var addrParts = address.split('.');
if (addrParts.length != 4)
{
return false;
}
if (isDecimalDigit(addrParts[0]) == false)
{
return false;
}
num = parseInt(addrParts[0]);
if (!(num >= 1 && num <= 223 && num != 127))
{
return false;
}
for (var i = 1; i <= 2; i++)
{
if (isDecimalDigit(addrParts[i]) == false)
{
return false;
}
num = parseInt(addrParts[i]);
if (!(num >= 0 && num <= 255))
{
return false;
}
}
if (isDecimalDigit(addrParts[3]) == false)
{
return false;
}
num = parseInt(addrParts[3]);
if (!(num >= 1 && num <= 254))
{
return false;
}
return true;
}
function CheckForm(type)
{
if (type == 0)
{
return true;
}
with (getElById('ConfigForm'))
{
ClearStatusVar();
if (wanId.length == 0)
{
alert("目前没有WAN连接，请点击添加按钮新建一条WAN连接并点击确定按钮以保存配置！");
return false;
}
if (!isInteger(atmVpi.value))
{
alert('VPI "' + atmVpi.value + '" is invalid.');
return false;
}
if (!isInteger(atmVci.value))
{
alert('VCI "' + atmVci.value + '" is invalid.');
return false;
}
var vpi = parseInt(atmVpi.value);
if (vpi < 0 || vpi > 255 || isNaN(vpi))
{
alert('VPI应配置为[0-255]之间的值。');
atmVpi.focus();
return false;
}
var vci = parseInt(atmVci.value);
if (vci < 32 || vci > 65535 || isNaN(vci))
{
alert('VCI应配置为[32-65535]之间的值。');
atmVci.focus();
return false;
}
if (enbl8021q.checked == true)
{
var v = vlan.value;
if(isPlusInteger(v) == false)
{
alert("VLAN ID非法！");
return false;
}
else
{
if ((v == "") || (v < 0) || (v > 4094))
{
alert("VLAN ID非法！");
return false;
}
}
}
if (enbl8021d.checked == true)
{
var v = v8021d.value;
if (isPlusInteger(v) == false)
{
alert("VLAN ID非法！");
return false;
}
else
{
if ((v == "") || (v < 0) || (v > 7))
{
alert("VLAN ID非法！");
return false;
}
}
}
switch (atmServiceCategory.value)
{
case "ubr+":
case "cbr":
if(isPlusInteger(atmPeakCellRate.value) == false)
{
atmPeakCellRate.focus();
alert("峰值信元速率非法！");
return false;
}
else
{
if (atmPeakCellRate.value < 0 || atmPeakCellRate.value > pcrMax)
{
atmPeakCellRate.focus();
alert("峰值信元速率非法！");
return false;
}
}
break;
case "nrt-vbr":
case "rt-vbr":
if(isPlusInteger(atmPeakCellRate.value) == false)
{
atmPeakCellRate.focus();
alert("峰值信元速率非法！");
return false;
}
else
{
if (atmPeakCellRate.value < 0 || atmPeakCellRate.value > pcrMax)
{
atmPeakCellRate.focus();
alert("峰值信元速率非法！");
return false;
}
}
if(isPlusInteger(atmSustainedCellRate.value) == false)
{
atmSustainedCellRate.focus();
alert("持续信元速率非法！");
return false;
}
else
{
if (atmSustainedCellRate.value < 0 || atmSustainedCellRate.value > pcrMax)
{
atmSustainedCellRate.focus();
alert("持续信元速率非法！");
return false;
}
}
if(parseInt(atmSustainedCellRate.value) > parseInt(atmPeakCellRate.value))
{
atmSustainedCellRate.focus();
alert("持续信元速率非法！需要小于峰值信元速率！");
return false;
}
if(isPlusInteger(atmMaxBurstSize.value) == false)
{
atmMaxBurstSize.focus();
alert("最大突发尺寸非法！");
return false;
}
else
{
if (atmMaxBurstSize.value < 0 || atmMaxBurstSize.value > 65535)
{
atmMaxBurstSize.focus();
alert("最大突发尺寸非法！");
return false;
}
}
break;
}
if (getElement('secPppoeItems').style.display != "none")
{
if (getElement('secIdleTime').style.display != "none")
{
if (isPlusInteger(pppTimeOut.value) == false)
{
alert("空闲超时非法！");
return false;
}
else
{
if ((getElement('pppTimeOut').value < 1) || (getElement('pppTimeOut').value > 4320)
|| (getElement('pppTimeOut').value == ''))
{
alert("空闲超时非法！");
return false;
}
}
}
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
if ((getElement('secStaticItems').style.display != "none")
&& ("IPv6" != getRadioVal("IpVersion")))
{
if (WanIP != null)
{
var iloop;
for (iloop = 0; iloop< WanIP.length-1; iloop++)
{
if ( (vpi != WanIP[iloop].atmVpi) && (vci != WanIP[iloop].atmVci))
{
if ( wanIpAddress.value == WanIP[iloop].wanIpAddress)
{
alert("IP地址和" + WanIP[iloop].conName + "冲突!") ;
return false;
}
}
}
}
if( WanPPP != null)
{
/*var jloop;
for (jloop = 0; jloop < WanPPP.length-1; jloop++)
{
if ( (vpi != WanPPP[jloop].atmVpi) && (vci != WanPPP[jloop].atmVci))
{
if ( wanIpAddress.value == WanPPP[jloop].wanIpAddress)
{
alert("IP地址和" + WanPPP[jloop].conName + "冲突!") ;
return false;
}
}
}*/
}
if (!isAbcIpAddress(wanIpAddress.value))
{
alert("IP地址非法！");
wanIpAddress.focus();
return false;
}
if (!isValidSubnetMask(wanSubnetMask.value))
{
alert("子网掩码非法！");
wanSubnetMask.focus();
return false;
}
if (!isHostIpWithSubnetMask(wanIpAddress.value, wanSubnetMask.value))
{
alert("IP地址和子网掩码不匹配");
wanIpAddress.focus();
return false;
}
if (!isAbcIpAddress(defaultGateway.value))
{
alert("缺省网关非法！");
defaultGateway.focus();
return false;
}
if (!isAbcIpAddress(dnsPrimary.value))
{
alert("首选DNS服务器地址非法！");
dnsPrimary.focus();
return false;
}
if (!isUseableIpAddress(dnsPrimary.value))
{
alert("首选DNS服务器地址非法！");
dnsPrimary.focus();
return false;
}
if (dnsSecondary.value != '' && !isAbcIpAddress(dnsSecondary.value))
{
alert("备用DNS服务器地址非法！");
dnsSecondary.focus();
return false;
}
if (dnsSecondary.value != '' && !isUseableIpAddress(dnsSecondary.value))
{
alert("备用DNS服务器地址非法！");
dnsSecondary.focus();
return false;
}
}
if ((secIPv6Div.style.display != "none"))
{
if ('IPv4' != getRadioVal('IpVersion'))
{
if (TrIpv6Addr.style.display != "none")
{
if (!isGlobalIpv6Address(getValue('IdIpv6Addr')))
{
alert("IPv6地址不正确!");
return false;
}
var v = getValue('IdIpv6PrefixLen');
if(isPlusInteger(v) == false)
{
alert("IPv6前缀长度不正确!");
return false;
}
else
{
if ((v == "") || (v <= 0) || (v > 128))
{
alert("IPv6前缀长度不正确!");
return false;
}
if (v.length > 1 && v.charAt(0) == '0')
{
alert("IPv6前缀长度不正确!");
return false;
}
}
if (!isUnicastIpv6Address(getValue('IdIpv6Dns1')))
{
alert("IPv6首选DNS服务器地址不正确");
return false;
}
var v1 = getValue('IdIpv6Dns2');
if (v1 != '' && !isUnicastIpv6Address(v1))
{
alert("IPv6备用DNS服务器地址不正确");
return false;
}
}
var v2 = getValue('IdIpv6Gateway');
if (v2 != '' && !isUnicastIpv6Address(v2))
{
alert("IPv6默认网关不正确");
return false;
}
}
}
}
mode = getSelectVal('wanMode');
ipMode = getRadioVal('IpMode');
brMode = getSelectVal('bridgeMode');
vpi = getValue('atmVpi')
vci = getValue('atmVci');
var type = getLinkType(mode,ipMode,brMode);
var wanType = getWanType(mode,ipMode,brMode);
var BindArray = new Array();
var j = 0;
for (var i = 1; i <= 4; i++)
{
var len = 'InternetGatewayDevice.LANDevice.1.'.length;
if (getCheckVal('cb_bindlan'+i) == 1)
BindArray[j++] = getValue('cb_bindlan'+i).substr(len);
if (getCheckVal('cb_bindwireless'+i) == 1)
BindArray[j++] = getValue('cb_bindwireless'+i).substr(len);
}
if (BindArray.length > 0)
{
for (var j = 0; j < Wan.length; j++)
{
if ((j != SelWanIndex) && (Wan[j].bind != ""))
{
if (mode == "Bridge")
{
for (i = 0; i < BindArray.length; i++)
{
if (Wan[j].bind.indexOf(BindArray[i]) >= 0)
{
alert('绑定选项与其他WAN连接的绑定选项冲突，请重新选择绑定接口，如果所有绑定选项皆被其他WAN绑定，则不能进行任何绑定！');
return false;
}
}
}
else
{
if (getRadioVal('IpVersion') == 'IPv4')
{
if ((Wan[j].ConnectionType.indexOf("Bridge") >= 0)
|| (Wan[j].xIpv4Enable == 1))
{
for (i = 0; i < BindArray.length; i++)
{
if (Wan[j].bind.indexOf(BindArray[i]) >= 0)
{
alert('绑定选项与其他WAN连接的绑定选项冲突，请重新选择绑定接口，如果所有绑定选项皆被其他WAN绑定，则不能进行任何绑定！');
return false;
}
}
}
}
else if (getRadioVal('IpVersion') == 'IPv6')
{
if ((Wan[j].ConnectionType.indexOf("Bridge") >= 0)
|| (Wan[j].xIpv6Enable == 1))
{
for (i = 0; i < BindArray.length; i++)
{
if (Wan[j].bind.indexOf(BindArray[i]) >= 0)
{
alert('绑定选项与其他WAN连接的绑定选项冲突，请重新选择绑定接口，如果所有绑定选项皆被其他WAN绑定，则不能进行任何绑定！');
return false;
}
}
}
}
else
{
for (i = 0; i < BindArray.length; i++)
{
if (Wan[j].bind.indexOf(BindArray[i]) >= 0)
{
alert('绑定选项与其他WAN连接的绑定选项冲突，请重新选择绑定接口，如果所有绑定选项皆被其他WAN绑定，则不能进行任何绑定！');
return false;
}
}
}
}
}
}
}
//if (AddFlag == true || (Wan[SelWanIndex].atmVpi != vpi || Wan[SelWanIndex].atmVci != vci ))
if (AddFlag == true)
{
var count = 0;
var i;
if (AddFlag == true)
{
for (i = 0; i < Wan.length; i++)
{
if (Wan[i].atmVpi == vpi && Wan[i].atmVci == vci)
{
count++;
}
}
if (count >= 4)
{
alert('一条PVC下最多只能添加四条WAN连接！');
return false;
}
}
count = 0;
if (wanType == 'WANIPConnection') 
{
for (i = 0; i < Wan.length; i++)
{
if (Wan[i].wanConn == 'IPCon' && Wan[i].atmVpi == vpi
&& Wan[i].atmVci == vci)
{
count++;
}
}
if (count >= 3)
{
alert('一条PVC下最多只能添加三条IPOE（或者桥）的WAN连接！');
return false;
}
}
else if (mode == 'Bridge') 
{
for (i = 0; i < Wan.length; i++)
{
if ((Wan[i].ConnectionType == 'PPP' || Wan[i].ConnectionType == 'IP' || Wan[i].wanConn == 'IPCon')
&& Wan[i].atmVpi == vpi
&& Wan[i].atmVci == vci)
{
count++;
}
}
if (count >= 3)
{
alert('一条PVC下最多只能添加三条IPOE（或者桥）的WAN连接！');
return false;
}
}
else 
{
for (i = 0; i < Wan.length; i++)
{
if ((Wan[i].wanConn == 'PPPCon' && Wan[i].ConnectionType == 'IP_Routed')
&& Wan[i].atmVpi == vpi
&& Wan[i].atmVci == vci)
{
count++;
}
}
if (count >= 4)
{
alert('一条PVC下最多只能添加四条PPPoE的WAN连接！');
return false;
}
}
}
if (CntPvc.length >= 9 && GetWanIndexPvcByUse(vpi,vci) == -1)
{
if (AddFlag == true)
{
alert("您最多只能建立8条PVC！");
return false;
}
}
return true;
}
function DisableInvisibleItems()
{
var Inputs = document.getElementsByTagName("div");
for (var i = 0; i < Inputs.length; i++)
{
if (Inputs[i].style.display == "none")
{
Inputs[i].disabled = true;
}
}
}
function ManualCntSubmit()
{
	if (AddFlag == true)
	{
		return;
	}
	//var trigger = 2; 
	//var xmlhttp = CreateXMLHttp();
	//if (Wan[SelWanIndex].Status == 'Connected')
	//{
	//	trigger = 1;
	//}
	//xmlhttp.open('post','dialcmd.cgi?cmd=' + Wan[SelWanIndex].domain + ',' + trigger, true);
	//xmlhttp.send(null);
	setDisable('btnRemoveCnt',1);
	setDisable('btnOK',1);
	setDisable('btnAddCnt',1);
	setDisable('pppDialButton',1);
	document.ConfigForm.Wan_Flag.value = "4";
	document.ConfigForm.submit();
}
function resetText()
{
var Inputs = document.getElementsByTagName("input");
for (var i = 0; i < Inputs.length; i++)
{
if (Inputs[i].type == "text" || Inputs[i].type == "password" )
{
Inputs[i].value = '';
}
}
}
function AddOption(selItem,value,text,ifSelected)
{
var option = document.createElement("option");
option.innerHTML = text;
option.value = value;
option.selected = ifSelected;
selItem.appendChild(option);
}
function RemoveOption(selItem,index)
{
	selItem.removeChild(selItem.options[index]);
}

function GetWanIndexPvcByUse(atmVpi,atmVci,exception)
{
for (i = 0; i < Wan.length; i++)
{
if (Wan[i].atmVpi == atmVpi && Wan[i].atmVci == atmVci
&& (exception == null || exception != i))
{
return i;
}
}
return -1;
}
function GetWanIndexPvcByUseEx(atmVpi, atmVci, execption)
{
for (i = 0; i < Wan.length; i++)
{
if ((Wan[i].atmVpi == atmVpi) &&
(Wan[i].atmVci == atmVci) &&
(execption != i) &&
(Wan[i].Relating.domain == Wan[execption].Relating.domain))
{
return i;
}
}
return -1;
}
function getSameWanList(index)
{
var atmVpi = Wan[index].atmVpi;
var atmVci = Wan[index].atmVci;
var list = '';
for (i = 0; i < Wan.length; i++)
{
if (i != index && Wan[i].atmVpi == atmVpi && Wan[i].atmVci == atmVci)
{
list += getWanList(list,i);
}
}
return list;
}
function getDeleteDomainName()
{
var Pvc = Wan[SelWanIndex].Relating;
var index = Pvc.domain.lastIndexOf('.');
var DslDomain = Pvc.domain.substr(0,index);    
var pos = GetWanIndexPvcByUseEx(Wan[SelWanIndex].atmVpi,
Wan[SelWanIndex].atmVci,SelWanIndex);
if (pos > -1)
{
return Wan[SelWanIndex].domain;
}
else
{
return DslDomain;
}
}

function ClearStatusVar()
{
wanList = '';
pvcByUseIndex = -1;
changePVCFlag = true;
}
function CancelAddCnt()
{
with (getElById('ConfigForm'))
{
RemoveOption(getElement('wanId'),wanId.length - 1);
AddFlag = false;
btnAddCnt.disabled = false;
}
}
function onCancel()
{
var wanIdItem = getElement('wanId');
if (wanIdItem.value == -1)
{
CancelAddCnt();
if (wanIdItem.length > 0)
{
//wanIdItem.selectedIndex = 0;
WanIdChange();
}
}
else RefreshPage();
}
function getBind(bindstr,cb_str)
{
if (getCheckVal(cb_str) == 1)
{
if (bindstr == '')
{
return  getValue(cb_str);
}
else
{
return  ',' + getValue(cb_str);
}
}
return '';
}
function getChangeWanTypeUrl(CntType)
{
var url = '';
if (pvcByUseIndex == -1)
{
pvcByUseIndex = SelWanIndex;
}
url = 'dellist=' + wanList + '&';
url += getAddWanUrl(CntType);
return url;
}
function getLinkType(mode,ipMode,brMode)
{
if (mode == "Route")
{
if (ipMode == 'PPPoA')
{
return 'PPPoA';
}
else if (ipMode == 'IPoA')
{
return 'IPoA';
}
else if (ipMode == 'CIP')
{
return 'CIP';
}
else
{
return 'EoA';
}
}
else if (mode == "Bridge")
{
return 'EoA';
}
}
function getWanType(mode,ipMode,brMode)
{
var LinkType = getLinkType(mode,ipMode,brMode);
if (mode == 'Bridge' && LinkType == 'EoA')
{
if (brMode == 'IP_Bridged')
{
return 'WANIPConnection';
}
else
{
return 'WANPPPConnection';
}
}
if ((LinkType == 'EoA' && ipMode == 'PPPoE') || (LinkType == 'PPPoA'))
{
return 'WANPPPConnection';
}
else if ((LinkType == 'EoA' && ipMode != 'PPPoE') || (LinkType == 'IPoA') || (LinkType == 'CIP'))
{
return 'WANIPConnection';
}
}
function addParam(Form,mode,ipMode,brMode)
{
var serviceList = getValue('serviceList');
var vpi = getValue('atmVpi');
var vci = getValue('atmVci');
Form.usingPrefix('y');
var wanName =  serviceList + '_' + mode.charAt(0)  + '_'
+ vpi + '_' + vci;
if ("OTHER" == serviceList)
{
wanName =  'Other' + '_' + mode.charAt(0)  + '_'
+ vpi + '_' + vci;
}
Form.addParameter('Name',wanName);
Form.addParameter('X_ATP_VLANEnabled',getCheckVal('enbl8021q'));
if (getCheckVal('enbl8021q') == 1)
{
Form.addParameter('X_ATP_VLANID',getValue('vlan'));
}
if (getCheckVal('enbl8021d') == 1)
{
Form.addParameter('X_ATP_Priority',getValue('v8021d'));
}
else
{
Form.addParameter('X_ATP_Priority',255);
}
var bindstr = '';
for (i = 1; i <= 4; i++)
{
bindstr = bindstr + getBind(bindstr,'cb_bindlan'+i);
bindstr = bindstr + getBind(bindstr,'cb_bindwireless'+i);
}
Form.addParameter('X_CT-COM_LanInterface',bindstr);
if (mode == 'Route')
{
Form.addParameter('ConnectionType','IP_Routed');
if (ipMode == 'PPPoE')
{
Form.addParameter('Username',getValue('pppUserName'));
var pwd = getValue('pppPassword');
if (pwd != '@1GV)Z<!')
{
Form.addParameter('Password',pwd);
}
if (pwd == '@1GV)Z<!')
{
var Pword = Wan[SelWanIndex].pppPassword;
Form.addParameter('Password', Pword);
}
Form.addParameter('ConnectionTrigger',getValue('DialMethod'));
Form.addParameter('DNSEnabled', "1");
Form.addParameter('DNSOverrideAllowed', "1");
Form.addParameter('DNSServers','');
}
else if (ipMode == 'Static')
{
Form.addParameter('AddressingType',ipMode);
Form.addParameter('ExternalIPAddress',getValue('wanIpAddress'));
Form.addParameter('SubnetMask',getValue('wanSubnetMask'));
Form.addParameter('DefaultGateway',getValue('defaultGateway'));
var DnsStr = getValue('dnsPrimary') + ',' + getValue('dnsSecondary');
Form.addParameter('DNSServers',DnsStr);
Form.addParameter('DNSEnabled', "1");
Form.addParameter('DNSOverrideAllowed', "0");
}
else if (ipMode == 'DHCP')
{
Form.addParameter('AddressingType',ipMode);
Form.addParameter('DNSEnabled', "1");
Form.addParameter('DNSOverrideAllowed', "1");
Form.addParameter('DNSServers','');
Form.addParameter('X_ATP_DHCPOptionCode',getValue('wanDhcpCode'));
}
else if (ipMode == 'IPoA')
{
Form.addParameter('AddressingType', "Static");
Form.addParameter('ExternalIPAddress',getValue('wanIpAddress'));
Form.addParameter('SubnetMask',getValue('wanSubnetMask'));
Form.addParameter('DefaultGateway',getValue('defaultGateway'));
var DnsStr = getValue('dnsPrimary') + ',' + getValue('dnsSecondary');
Form.addParameter('DNSServers',DnsStr);
Form.addParameter('DNSEnabled', "1");
Form.addParameter('DNSOverrideAllowed', "0");
}
else if (ipMode == 'CIP')
{
Form.addParameter('AddressingType', "Static");
Form.addParameter('ExternalIPAddress',getValue('wanIpAddress'));
Form.addParameter('SubnetMask',getValue('wanSubnetMask'));
Form.addParameter('DefaultGateway',getValue('defaultGateway'));
var DnsStr = getValue('dnsPrimary') + ',' + getValue('dnsSecondary');
Form.addParameter('DNSServers',DnsStr);
Form.addParameter('DNSEnabled', "1");
Form.addParameter('DNSOverrideAllowed', "0");
}
else if (ipMode == 'PPPoA')
{
Form.addParameter('Username',getValue('pppUserName'));
var pwd = getValue('pppPassword');
if (pwd != '@1GV)Z<!')
{
Form.addParameter('Password',pwd);
}
if (pwd == '@1GV)Z<!')
{
var Pword = Wan[SelWanIndex].pppPassword;
Form.addParameter('Password', Pword);
}
Form.addParameter('ConnectionTrigger',getValue('DialMethod'));
Form.addParameter('DNSEnabled', "1");
Form.addParameter('DNSOverrideAllowed', "1");
Form.addParameter('DNSServers','');
}
var enablNat = getCheckVal('cb_nat');
if (enablNat == 0)
{
Form.addParameter('NATEnabled', 0);
}
else
{
Form.addParameter('NATEnabled', 1);
}
}
else if (mode == 'Bridge')
{
Form.addParameter('ConnectionType',brMode);
Form.addParameter('X_CT-COM_LanInterface-DHCPEnable',(getCheckVal('cb_dhcprelay')+1)%2);
Form.addParameter('DNSEnabled', "0");
Form.addParameter('DNSServers','');
if (brMode == 'IP')
{
Form.addParameter('AddressingType','Static');
}
}
if (mode == 'Route')
{
var IpVer = getRadioVal('IpVersion');
var xIpv4Enable = 0;
var xIpv6Enable = 0;
if (IpVer == 'IPv4')
{
xIpv4Enable = 1;
xIpv6Enable = 0;
}
else if (IpVer == 'IPv6')
{
xIpv4Enable = 0;
xIpv6Enable = 1;
}
else
{
xIpv4Enable = 1;
xIpv6Enable = 1;
}
Form.addParameter('X_CT-COM_IPv4Enable', xIpv4Enable);
Form.addParameter('X_CT-COM_IPv6Enable', xIpv6Enable);
if (1 == xIpv6Enable)
{
Form.addParameter('X_CT-COM_IPv6AddressingType', getSelectVal('IdIpv6AddrType'));
if ('SLAAC' != getSelectVal('IdIpv6AddrType'))
{
Form.addParameter('X_CT-COM_IPv6DefaultGateway', getValue('IdIpv6Gateway'));
}
else
{
Form.addParameter('X_CT-COM_IPv6DefaultGateway', '');
}
if (getSelectVal('IdIpv6AddrType') == 'Static')
{
Form.addParameter('X_CT-COM_IPv6Address', getValue('IdIpv6Addr'));
Form.addParameter('X_CT-COM_IPv6PrefixLength', getValue('IdIpv6PrefixLen'));
if (getValue('IdIpv6Dns2') == '')
{
Form.addParameter('X_CT-COM_IPv6DNSServers', getValue('IdIpv6Dns1'));
}
else
{
Form.addParameter('X_CT-COM_IPv6DNSServers', getValue('IdIpv6Dns1') + ',' + getValue('IdIpv6Dns2'));
}
Form.addParameter('X_CT-COM_IPv6DNSEnabled', '1');
Form.addParameter('X_CT-COM_IPv6DNSOverrideAllowed', '0');
}
else
{
Form.addParameter('X_CT-COM_IPv6Address', "");
Form.addParameter('X_CT-COM_IPv6PrefixLength', '0');
Form.addParameter('X_CT-COM_IPv6DNSEnabled', '1');
Form.addParameter('X_CT-COM_IPv6DNSOverrideAllowed', '1');
Form.addParameter('X_CT-COM_IPv6DNSServers', "");
}
}
}
else
{
Form.addParameter('X_CT-COM_IPv6Enable', '0');
}
Form.addParameter('Enable',getCheckVal('cb_enblService'));
Form.addParameter('X_CT-COM_ServiceList',serviceList);
Form.endPrefix();
Form.usingPrefix('x');
Form.addParameter('DestinationAddress','PVC:' + vpi + '/' + vci);
var ATMQoS = getSelectVal('atmServiceCategory');
Form.addParameter('ATMQoS',ATMQoS);
Form.addParameter('Enable',1);
switch (ATMQoS)
{
case "ubr+":
case "cbr":
Form.addParameter('ATMPeakCellRate',getValue('atmPeakCellRate'));
break;
case "nrt-vbr":
case "rt-vbr":
Form.addParameter('ATMPeakCellRate',getValue('atmPeakCellRate'));
Form.addParameter('ATMSustainableCellRate',getValue('atmSustainedCellRate'));
Form.addParameter('ATMMaximumBurstSize',getValue('atmMaxBurstSize'));
break;
}
if (mode == 'Route')
{
if (ipMode == 'PPPoE')
{
Form.addParameter('LinkType','EoA');
}
else if (ipMode == 'PPPoA')
{
Form.addParameter('LinkType','PPPoA');
}
else if (ipMode == 'IPoA')
{
Form.addParameter('LinkType','IPoA');
}
else if (ipMode == 'CIP')
{
Form.addParameter('LinkType','CIP');
}
else
{
Form.addParameter('LinkType','EoA');
}
}
else if (mode == 'Bridge')
{
Form.addParameter('LinkType','EoA');
}
Form.addParameter('ATMEncapsulation',getValue('encapMode'));
Form.endPrefix();
}
function isLinkTypeDiffer(mode,ipMode,vpi,vci,exception)
{
mode = getSelectVal('wanMode');
ipMode = getRadioVal('IpMode');
vpi = getValue('atmVpi')
vci = getValue('atmVci');
var brMode = getSelectVal('bridgeMode');
var type = getWanType(mode,ipMode,brMode);
for (i = 0; i < Wan.length; i++)
{
if (Wan[i].atmVpi == vpi && Wan[i].atmVci == vci
&& (exception == null || exception != i))
{
if (type != Wan[i].LinkType)
{
return true;
}
}
}
return false;
}
function getAddWanUrl(CntType)
{
if (pvcByUseIndex == -1)
{
url =  'x=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.'
+ '&y=' + CntType;
}
else
{  
var DslDomain = Wan[pvcByUseIndex].Relating.domain;
var CntDomain = DslDomain.substr(0,DslDomain.lastIndexOf('.'));
url =  'x=' + CntDomain + '&y=' + CntType;
}
return url;
}
function AddSubmitParam(Form,type)
{
if (type == 0)
{
var DslList = "";
var cb_Dsl = getElById('cb_dslEnable');
for (i = 0; i < cb_Dsl.length; i++)
{
if (cb_Dsl[i].checked == true)
{
if(DslList == "")
{
DslList += cb_Dsl[i].value;
}
}
}
Form.addParameter('InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.ConfigMode', DslList);
Form.setAction('set.cgi?RequestFile=html/network/wan1.asp');
}
else
{
var url;
var mode = getSelectVal('wanMode');
var ipMode = getRadioVal('IpMode');
var brMode = getSelectVal('bridgeMode');
var CntType = getWanType(mode,ipMode,brMode);
vpi = getValue('atmVpi')
vci = getValue('atmVci');
pvcByUseIndex = -1;
for (i = 0; i < Wan.length; i++)
{
if (((Wan[i].atmVpi == vpi && Wan[i].atmVci == vci)) && (i != SelWanIndex))
{
pvcByUseIndex = i;
break;
}
}
if (AddFlag == true)
{
url = 'addwan.cgi?' + getAddWanUrl(CntType);
}
else
{
var temp = Wan[SelWanIndex].domain.split('.');
if ((Wan[SelWanIndex].atmVpi != vpi) || (Wan[SelWanIndex].atmVci != vci))
{
changePVCFlag = true;
}
else
{
changePVCFlag = false;
}
if (Wan[SelWanIndex].domain.indexOf(CntType) < 0)
{
if (changePVCFlag == true)
{
if (GetWanIndexPvcByUseEx(Wan[SelWanIndex].atmVpi,
Wan[SelWanIndex].atmVci,SelWanIndex) > -1)
{
wanList = temp[4] + '.' + temp[5] + '.' + temp[6];
}
else
{
wanList = temp[4];
}
url = 'changewantype.cgi?dellist=' + wanList + '&' + getAddWanUrl(CntType);
}
else
{
wanList = temp[4] + '.' + temp[5] + '.' + temp[6];
url = 'changewantype.cgi?' + getChangeWanTypeUrl(CntType)
}
}
else
{
if (changePVCFlag == true)
{
var index = GetWanIndexPvcByUse(vpi,vci);
if (index >= 0)
{
pvcByUseIndex = index;
if (GetWanIndexPvcByUseEx(Wan[SelWanIndex].atmVpi,Wan[SelWanIndex].atmVci,SelWanIndex) >=0 )
{
wanList = temp[4] + '.' + temp[5] + '.' + temp[6];
}
else
{
wanList = temp[4];
}
url = 'changewantype.cgi?' + getChangeWanTypeUrl(CntType);
}
else
{
if (GetWanIndexPvcByUseEx(Wan[SelWanIndex].atmVpi,
Wan[SelWanIndex].atmVci,SelWanIndex) > -1)
{
wanList = temp[4] + '.' + temp[5] + '.' + temp[6];
url = 'changewantype.cgi?dellist=' + wanList + '&' + getAddWanUrl(CntType);
}
else
{
url = 'setcfg.cgi?x=' + Wan[SelWanIndex].Relating.domain
+ '&y=' + Wan[SelWanIndex].domain
+ '&RequestFile=html/network/wan1.asp';
}
}
}
else
{
url = 'setcfg.cgi?x=' + Wan[SelWanIndex].Relating.domain
+ '&y=' + Wan[SelWanIndex].domain
+ '&RequestFile=html/network/wan1.asp';
}
}
}
addParam(Form,mode,ipMode,brMode);
Form.setAction(url);
setDisable('btnRemoveCnt',1);
setDisable('btnOK',1);
setDisable('btnAddCnt',1);
}
}
</SCRIPT>


      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>Internet连接</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=9>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#宽带设置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 　　　　　 　　　　　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name=ConfigForm action="/cgi-bin/net-wanset.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                        <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                                <TD width=150>连接名称： 
                                  <input type="hidden" name="curSetIndex" value="<% tcWebApi_get("WanInfo_Common","CurIFIndex","s") %>">
                                  <input type="hidden" name="WanActive" value="<% tcWebApi_get("WanInfo_WanIF","Active","s") %>">
                                  <script language="JavaScript" type="text/JavaScript">
 var			ipvChanged = 0;//flag of ip version whether changed

function CheckIpVersionState()
{
	var vForm = document.ConfigForm;
	ipvChanged = 0;
	vForm.IPVersionValue.value = "<% tcWebApi_get("WanInfo_WanIF","IPVERSION","s") %>";
	var vValue = getRadioVal("IpVersion");
	if(vForm.IPVersionValue.value != vValue){
		if(vForm.IPVersionValue.value == "IPv4")
			ipvChanged = 1;
		else if(vForm.IPVersionValue.value == "IPv6")
			ipvChanged = 2;
		else ipvChanged = 3;
	}
	vForm.IPVersionValue.value = vValue;
	
	with (getElById('ConfigForm'))
	{
		if(IdIpv6AddrType.value == "SLAAC")
			pppv6Mode.value = "No";
		else if(IdIpv6AddrType.value == "DHCP")
			pppv6Mode.value = "Yes";
		else
			pppv6Mode.value = "N/A";	
	}
	
}
	
function WanIndexConstruction(domain,WanName)
{
	this.domain = domain;
	this.WanName = WanName;
}
function CheckWansActives()
{
	var	nCurTemp = 0;
	var	vcurLinks = new Array(nEntryNum);

	for(var i=0; i<nEntryNum; i++)
	{	
		vcurLinks[nCurTemp++] = new WanIndexConstruction(vEntryIndex[i], vEntryName[i]);
	}
	
	var	vObjRet = new Array(nCurTemp+1);
	for(var m=0; m<nCurTemp; m++)
	{
		vObjRet[m] = vcurLinks[m];
	}
	vObjRet[nCurTemp] = null;
	return vObjRet;
}
var	CurWan = CheckWansActives();
function WriteWanNameSelected()
{
	var WanIDNums = CurWan;
	var nlength = WanIDNums.length-1;
	if(nlength == 1) 
		document.ConfigForm.curSetIndex.value = WanIDNums[0].domain;
	for(var i=0; i< nlength; i++)
	{
		document.writeln("<option value=" + WanIDNums[i].domain + ">" + WanIDNums[i].WanName + "</option>");
		if(WanIDNums[i].domain == document.ConfigForm.curSetIndex.value)
			document.ConfigForm.wanId.selectedIndex= i;
	}
}
//type: 
//0: Add action
//1: Modify action
function	checkBandBoxStatus(type)
{
//lan
	var strCurBind = "";
	var aCurBindFlag = new Array(8);
	aCurBindFlag[0] = "<% tcWebApi_get("WanInfo_WanIF","LAN1","s") %>";
	if(aCurBindFlag[0] != "N/A")
	{
		aCurBindFlag[1] = "<% tcWebApi_get("WanInfo_WanIF","LAN2","s") %>";
		aCurBindFlag[2] = "<% tcWebApi_get("WanInfo_WanIF","LAN3","s") %>";
		aCurBindFlag[3] = "<% tcWebApi_get("WanInfo_WanIF","LAN4","s") %>";
		aCurBindFlag[4] = "<% tcWebApi_get("WanInfo_WanIF","SSID1","s") %>";
		aCurBindFlag[5] = "<% tcWebApi_get("WanInfo_WanIF","SSID2","s") %>";
		aCurBindFlag[6] = "<% tcWebApi_get("WanInfo_WanIF","SSID3","s") %>";
		aCurBindFlag[7] = "<% tcWebApi_get("WanInfo_WanIF","SSID4","s") %>";
		for(k=0; k<8; k++)
		{
			strCurBind = strCurBind + aCurBindFlag[k] + ",";
		}
	}
	
	var strBindFlag = "";
	var nInterfaces = CurWan.length-1;
	var vForm = document.ConfigForm;
	if(vForm.cb_bindlan1.checked)
		vForm.bindlan1.value = "Yes";
	else vForm.bindlan1.value = "No";
	strBindFlag = strBindFlag + vForm.bindlan1.value + ",";
	if(vForm.cb_bindlan2.checked)
		vForm.bindlan2.value = "Yes";
	else vForm.bindlan2.value = "No";
	strBindFlag = strBindFlag + vForm.bindlan2.value + ",";
	if(vForm.cb_bindlan3.checked)
		vForm.bindlan3.value = "Yes";
	else vForm.bindlan3.value = "No";
	strBindFlag = strBindFlag + vForm.bindlan3.value + ",";
	if(vForm.cb_bindlan4.checked)
		vForm.bindlan4.value = "Yes";
	else vForm.bindlan4.value = "No";
	strBindFlag = strBindFlag + vForm.bindlan4.value + ",";
//wireless
	if(vForm.cb_bindwireless1.checked)
		vForm.bindwireless1.value = "Yes";
	else vForm.bindwireless1.value = "No";
	strBindFlag = strBindFlag + vForm.bindwireless1.value + ",";
	if(vForm.cb_bindwireless2.checked)
		vForm.bindwireless2.value = "Yes";
	else vForm.bindwireless2.value = "No";
	strBindFlag = strBindFlag + vForm.bindwireless2.value + ",";
	if(vForm.cb_bindwireless3.checked)
		vForm.bindwireless3.value = "Yes";
	else vForm.bindwireless3.value = "No";
	strBindFlag = strBindFlag + vForm.bindwireless3.value + ",";
	if(vForm.cb_bindwireless4.checked)
		vForm.bindwireless4.value = "Yes";
	else vForm.bindwireless4.value = "No";
	strBindFlag = strBindFlag + vForm.bindwireless4.value;

	var aTemp1 = new Array();
	var aTemp2 = new Array();
	var aTemp3 = new Array();
	if(vBindStatus != "N/A")
	{
		aTemp1 = vBindStatus.split(',');
		aTemp2 = strBindFlag.split(',');
		aTemp3 = strCurBind.split(',');
		//check ip version;
		var strIpversion = vForm.IPVersionValue.value;//current ip version;
		
		for(var i=0; i<8; i++)
		{
			
			if((aTemp1[i] == "Yes") && (aTemp2[i] == "Yes"))
			{
				//continue if it do modify action and the port is bind 
				if((1 == type) && (aTemp3[i] == "Yes")){
					if(0 == ipvChanged)//ip version  has not changed
						continue;
					else{
						//if("IPv4/IPv6" == strIpversion){
							if(3 == ipvChanged){//ipv4/v6->ipv4 or ipv6
								continue;
							}
							else if(1 == ipvChanged){//ipv4->ipv4/ipv6
								if(0 == aTemp1[8+2*i+1])//if ipv6 is not binded, can ok
									continue;
							}
							else if(2 == ipvChanged){//ipv6->ipv4/ipv6
								if(0 == aTemp1[8+2*i])//if ipv4 is not binded, can ok
									continue;
							}
						//}
					}
				}
				
				if((("IPv4" == strIpversion) && (0 == aTemp1[8+2*i])) || (("IPv6" == strIpversion) && (0 == aTemp1[8+2*i+1]))){
					continue;
				}
				var strindex;
				if(i < 4)
				{
					strindex = i+1;
					alert("Lan" + strindex.toString() + "已被其它Interface绑定，不能重复绑定！");
				}
				else
				{
					strindex = i - 3;
					alert("SSID" + strindex.toString() + "已被其它Interface绑定，不能重复绑定！");
				}
				return false;
			}
		}	
	}
	return true;
}

function getENCAPstatus()
{
	with (getElById('ConfigForm'))
	{
		if(wanMode.value == "Bridge")
		{
			ISPTypeValue.value = "3";
			if(encapMode.value == "LLC")
			{
				EnCAPFlag.value = "1483 Bridged IP LLC";	
			}
			else
			{
				EnCAPFlag.value = "1483 Bridged IP VC-Mux";
			}

		}
		else
		{
			if(linkMode.value == "linkPPP")
			{
				ISPTypeValue.value = "2";//pppoe mode
				if(encapMode.value == "LLC")
					EnCAPFlag.value = "PPPoE LLC";
				else
					EnCAPFlag.value = "PPPoE VC-Mux";
			}
			else
			{
				if(encapMode.value == "LLC")
				{
					//if(ISPTypeValue.value == "0")
						EnCAPFlag.value = "1483 Bridged IP LLC";
					//else EnCAPFlag.value = "1483 Routed IP LLC(IPoA)";
				}
				else
				{
					//if(ISPTypeValue.value == "0")
						EnCAPFlag.value = "1483 Bridged IP VC-Mux";
					//else EnCAPFlag.value = "1483 Routed IP VC-Mux";
				}
			}
		}
	}
}
/*function checkPVCEntryStatus()
{
	var nEntry = 0;
	var nPVC = 0;
	var pvcokflag = false;
	var	vForm = document.ConfigForm;
	var nlen = CurWan.length - 1;
	for(var i=0; i<nlen; i++)
	{
		if((CurWan[i].atmvpi == vForm.atmVpi.value) && (CurWan[i].atmvci == vForm.atmVci.value))
		{
			nEntry++;
			pvcokflag = true;
		}
		if(pvcokflag == false)
		{
			if(parseInt(CurWan[i].domainpvc) == 7)
				nPVC++;
		}
	}
	if(nEntry>8)
	{
		alert('一条PVC下最多只能添加八条WAN连接！');
		return false;
	}
	else if(nEntry == 0)
	{
		if((pvcokflag == false) && (nPVC > 0))
		{
			alert('最多只能添加八条PVC！');
			return false;
		}
	}
	return true;
	
}*/

function btnSave()
{
	if(CheckForm(1) == false)
		return false;
	getENCAPstatus();
	CheckIpVersionState();
	cb_enblServiceChange();
	EnableDHCPRealy();
	var	vForm = document.ConfigForm;

	if(vForm.linkMode.value == "linkPPP")
	{
		DialMethodChange();
		setText('pppManualStatus_Flag', 'disconnect');
	}
	vForm.Wan_Flag.value = "1";
	if(AddFlag == true){
		vForm.OperatorStyle.value = "Add";//add new
		if(checkBandBoxStatus(0) == false)
			return false;
	}
	else{
		vForm.OperatorStyle.value = "Modify";//modify
		if(checkBandBoxStatus(1) == false)
			return false;
	}
	setDisable('btnRemoveCnt',1);
	setDisable('btnOK',1);
	setDisable('btnAddCnt',1);
	vForm.submit();
}

function btnAddWanCnt()
{
	//if (CurWan.length >= 16)
	if ((CurWan.length-1) >= 64)
	{
		alert("您最多只能建立64条WAN连接！");
		return;
	}
	if (AddFlag == true)
	{
		alert("请您保存新建连接后再进行新的添加！");
		return;
	}
	AddFlag = true;
	resetText();
	with (getElById('ConfigForm'))
	{
		AddOption(getElementByName('wanId'),-1,'新建WAN连接',true);
		btnAddCnt.disabled = true;
		setSelect('serviceList','INTERNET');
		setSelect('linkMode', 'linkPPP');
		setCheck('cb_enblService',1);
		setSelect('wanMode',"Route");
		setSelect('bridgeMode', "PPPoE_Bridged");
		setDisable('pppUserName',0);
		setDisable('pppPassword',0);
		setText("atmPeakCellRate","0");
		SelWanIndex = -1;
		WanModeChange();
		onChangeSvrList();
		IpMode[2].checked = true;
		IpModeChange();
		for (var i = 0; i < 4; i++)
		{
			var checkString = 'cb_bindlan' + (i+1);
			setCheck(checkString,0);
			checkString = 'cb_bindwireless' + (i+1);
			setCheck(checkString,0);
		}
		document.getElementById("table8").focus();
	}
}
function btnRemoveWanCnt()
{
	if ((CurWan.length - 1) == 0)
	{
		alert("无法删除，您未添加任何WAN连接！");
		return;
	}
	if (AddFlag == true)
	{
		alert("新建WAN连接并未保存，不能进行删除操作！");
		return;
	}
	if (confirm("您是否确认要删除此条WAN连接？") == false)
		return;
//var Form = new webSubmitForm();
//var domain = getDeleteDomainName();
//Form.addParameter(domain,'');
//Form.setAction('del.cgi?RequestFile=html/network/wan1.asp');
//Form.submit();
	var	vForm = document.ConfigForm;
	vForm.Wan_Flag.value = "3";
	for(var i=0; i<(CurWan.length-1); i++)
	{
		if(CurWan[i].domain != vForm.curSetIndex.value)
		{
			vForm.afterdeleteFlag.value = CurWan[i].domain;
			break;
		}
	}
	setDisable('btnRemoveCnt',1);
	setDisable('btnOK',1);
	setDisable('btnAddCnt',1);
	vForm.OperatorStyle.value = "Del";
	vForm.submit();
}

function OnIPv6Changed()
{
with (getElById('ConfigForm'))
{
	var linkstr = getSelectVal('linkMode');
	var AddrType = getSelectVal('IdIpv6AddrType');
	if (AddrType == 'SLAAC')
	{
				setDisplay('TrIpv6Addr', 0);
				setDisplay('TrIpv6Dns1', 0);
				setDisplay('TrIpv6Dns2', 0);
				setDisplay('TrIpv6GatewayInfo', 0);
				setDisplay('TrIpv6Gateway', 0);
				ISPTypeValue.value = "0";
	}
	else if (AddrType == 'DHCP')
	{
				setDisplay('TrIpv6Addr', 0);
				setDisplay('TrIpv6Dns1', 0);
				setDisplay('TrIpv6Dns2', 0);
				setDisplay('TrIpv6Gateway', 1);
				setDisplay('TrIpv6GatewayInfo', 1);
				ISPTypeValue.value = "0";
	}
	else if (AddrType == 'Static')
	{
				setDisplay('TrIpv6Addr', 1);
				setDisplay('TrIpv6Dns1', 1);
				setDisplay('TrIpv6Dns2', 1);
				setDisplay('TrIpv6Gateway', 1);
				setDisplay('TrIpv6GatewayInfo', 1);
				ISPTypeValue.value = "1";
	}
}
}
function WriteIPv6List(index)
{
	var vmode = new Array("No", "Yes", "N/A");
	var ctrl = getElById('IdIpv6AddrType');
	for(var i=0; i<ctrl.options.length;)
	{
		ctrl.removeChild(ctrl.options[i]);
	}
	if(index == 0)
	{
		var aMenu = new Array("SLAAC","DHCP");
		for(i=0; i<aMenu.length; i++)
		{
			ctrl.options.add(new Option(aMenu[i],aMenu[i]));
			if(vCurrentDHCPv6 == vmode[i])
			{
				document.ConfigForm.IdIpv6AddrType.selectedIndex = i;
			}
		}
	}
	else if(index == 1)
	{
		var aMenu = new Array("SLAAC","DHCP","Static");
		for(i=0; i<aMenu.length; i++)
		{
			ctrl.options.add(new Option(aMenu[i],aMenu[i]));
			if(vCurrentDHCPv6 == vmode[i])
			{
				document.ConfigForm.IdIpv6AddrType.selectedIndex = i;
			}
		}
	}
	else if(index == 2)
	{
		var aMenu = "Static";
		ctrl.options.add(new Option(aMenu,aMenu));
	}
	OnIPv6Changed();
}

function WanIdChange()
{
	document.ConfigForm.Wan_Flag.value  = "2";
	document.ConfigForm.curSetIndex.value = CurWan[document.ConfigForm.wanId.selectedIndex].domain;
	document.ConfigForm.submit();
	/*with (getElById('ConfigForm'))
	{
		if (AddFlag == true)
		{
			CancelAddCnt();
		}
	var si = wanId.selectedIndex;
	getElement('ConfigForm').reset();
	wanId.selectedIndex = si;
	var index;
	for (var i = 0; i < Wan.length; i++)
	{
		if (Wan[i].wanId == wanId.value)
		{
			index = i;
			break;
		}
	}
	SelWanIndex = index;
	setCheck('cb_enblService',Wan[index].enblService);
	cb_enblServiceChange();
	setCheck('cb_bindflag',Wan[index].bindflag);
	clearBindList();
	var BindArray = Wan[index].bind.split(',');
	var InsId;
	var handle;
	for (i = 0; i < BindArray.length; i++)
	{
		InsId = BindArray[i].substr(BindArray[i].lastIndexOf('.') + 1)
		if (BindArray[i].indexOf('LANEthernetInterfaceConfig') > 0)
		{
			handle =  'setCheck(' + '\'cb_bindlan' + InsId + '\'' + ',1)';
		}
		else if (BindArray[i].indexOf('WLANConfiguration') > 0)
		{
			handle =  'setCheck(' + '\'cb_bindwireless' + InsId + '\'' + ',1)';
		}
		else
		{
			break;
		}
		eval(handle);
	}
	cb_bindflagChange();
	atmVpi.value = Wan[index].atmVpi;
	atmVci.value = Wan[index].atmVci;
	setSelect('encapMode',Wan[index].encapMode);
	setCheck('enbl8021q',Wan[index].vlanEnable);
	setText('vlan',Wan[index].vlanId);
	Enbl8021qChange();
	if (Wan[index].vlanPri < 0 || Wan[index].vlanPri >= 255)
	{
		setText('v8021d',"");
		setCheck('enbl8021d',0);
	}
	else
	{
		setText('v8021d',Wan[index].vlanPri);
		setCheck('enbl8021d',1);
	}
	Enbl8021dChange();
	setSelect('atmServiceCategory',Wan[index].atmServiceCategory);
	atmPeakCellRate.value = Wan[index].atmPeakCellRate;
	atmSustainedCellRate.value = Wan[index].atmSustainedCellRate;
	atmMaxBurstSize.value = Wan[index].atmMaxBurstSize;
	atmServiceCategoryChange();
	if ((Wan[index].xIpv4Enable == 1) && (Wan[index].xIpv6Enable == 1))
	{
		setRadio("IpVersion", "IPv4/IPv6");
	}
	else if ((Wan[index].xIpv4Enable == 1) && (Wan[index].xIpv6Enable == 0))
	{
		setRadio("IpVersion", "IPv4");
	}
	else if ((Wan[index].xIpv4Enable == 0) && (Wan[index].xIpv6Enable == 1))
	{
		setRadio("IpVersion", "IPv6");
	}
	setSelect('IdIpv6AddrType', Wan[index].xIpv6AddrType);
	if ('Static' == Wan[index].xIpv6AddrType)
	{
		setText('IdIpv6Addr', Wan[index].xIpv6Addr);
		setText('IdIpv6PrefixLen', Wan[index].xIpv6PrefixLen);
		setText('IdIpv6Gateway', Wan[index].xIpv6Gateway);
		setText('IdIpv6Dns1', Wan[index].xIpv6Dns1);
		setText('IdIpv6Dns2', Wan[index].xIpv6Dns2);
	}
	else if ('DHCP' == Wan[index].xIpv6AddrType)
	{
		setText('IdIpv6Gateway', Wan[index].xIpv6Gateway);
	}
	if (Wan[index].LinkType == "EoA"
	&& Wan[index].ConnectionType == 'IP_Routed'
	&& Wan[index].wanConn == 'IPCon')
	{
		setDisplay('secIdleTime',0);    
		setDisplay('secManualDial',0);  
		wanMode.value = "Route";
		WanModeChange();
		setSelect('linkMode', 'linkIP');
		linkModeSelect();
		if (Wan[index].addrType == "DHCP")
		{
			IpMode[0].checked = true;
			IpModeChange();
			wanDhcpCode.value=Wan[index].DhcpCode;
		}
		else
		{   
		IpMode[1].checked = true;
IpModeChange();
wanIpAddress.value = Wan[index].wanIpAddress;
wanSubnetMask.value = Wan[index].wanSubnetMask;
defaultGateway.value = Wan[index].defaultGateway;
dnsPrimary.value = Wan[index].dnsPrimary;
dnsSecondary.value = Wan[index].dnsSecondary;
}
}
else if (Wan[index].LinkType == "EoA"
&& Wan[index].ConnectionType == 'IP_Routed'
&& Wan[index].wanConn == 'PPPCon')
{
wanMode.value = "Route";
WanModeChange();
setSelect('linkMode', 'linkPPP');
linkModeSelect();
IpMode[2].checked = true;
IpModeChange();
pppPassword.value = "@1GV)Z<!";
if (index-WanIP.length+1<pppUsrAccessArr.length)
{
if ((pppUsrAccessArr[index-WanIP.length+1].length > 0) && (pppUsrAccessArr[index-WanIP.length+1].indexOf('Subscriber') < 0) )
{
setDisable('pppUserName',1);
}
else
{
setDisable('pppUserName',0);
}
if ((pppPwdAccessArr[index-WanIP.length+1].length>0) && (pppPwdAccessArr[index-WanIP.length+1].indexOf('Subscriber') < 0 ))
{
setDisable('pppPassword',1);
}
else
{
setDisable('pppPassword',0);
}
}
else
{
setDisable('pppUserName',0);
setDisable('pppPassword',0);
}
pppUserName.value = Wan[index].pppUserName;
setSelect('DialMethod',Wan[index].cntMode);
DialMethodChange(index);
}
else if (Wan[index].LinkType == "PPPoA")
{
pppPassword.value = "@1GV)Z<!";
wanMode.value = "Route";
WanModeChange();
IpMode[3].checked = true;
IpModeChange();
pppUserName.value = Wan[index].pppUserName;
setSelect('DialMethod',Wan[index].cntMode);
DialMethodChange(index);
}
else if (Wan[index].LinkType == "IPoA")
{
secIdleTime.style.display = "none";
secManualDial.style.display = "none";
wanMode.value = "Route";
WanModeChange();
IpMode[4].checked = true;
IpModeChange();
wanIpAddress.value = Wan[index].wanIpAddress;
wanSubnetMask.value = Wan[index].wanSubnetMask;
defaultGateway.value = Wan[index].defaultGateway;
dnsPrimary.value = Wan[index].dnsPrimary;
dnsSecondary.value = Wan[index].dnsSecondary;
}
else if (Wan[index].LinkType == "CIP")
{
secIdleTime.style.display = "none";
secManualDial.style.display = "none";
wanMode.value = "Route";
WanModeChange();
IpMode[5].checked = true;
IpModeChange();
wanIpAddress.value = Wan[index].wanIpAddress;
wanSubnetMask.value = Wan[index].wanSubnetMask;
defaultGateway.value = Wan[index].defaultGateway;
dnsPrimary.value = Wan[index].dnsPrimary;
dnsSecondary.value = Wan[index].dnsSecondary;
}
else if (Wan[index].ConnectionType.indexOf('Bridge') > -1)
{
getElement('secIdleTime').style.display = "none";
getElement('secManualDial').style.display = "none";
wanMode.value = "Bridge";
WanModeChange();
setCheck('cb_dhcprelay',(parseInt(Wan[index].DHCPRelay) + 1) % 2);
if (Wan[index].wanConn == "PPPCon")
{
setSelect('bridgeMode', "PPPoE_Bridged");
}
else
{
setSelect('bridgeMode', "IP_Bridged");
}
}
setSelect('serviceList',Wan[index].serviceList);
onChangeSvrList();
if (Wan[index].ConnectionType.indexOf('Bridge') < 0)
{
if(0 == Wan[index].nat)
{
setCheck('cb_nat', 0);
}
else
{
setCheck('cb_nat', 1);
}
}
}*/
}
								  </script></TD>
                      <TD width=200><LABEL> 
                                  <select onChange=WanIdChange()  name=wanId>
                                    <script language=JavaScript type=text/javascript>
					  WriteWanNameSelected();
//for (var i = 0; i < Wan.length; i++)
//{
//document.write('<option value="' + Wan[i].wanId + '">' + Wan[i].conName + '</option>');
//}
					</script>
                                  </select>
                                  <input type="hidden" name="Wan_Flag" value="0">
                                  <input type="hidden" name="EnCAPFlag" value="PPPoE LLC">
                                  <input type="hidden" name="PPPGetIpFlag" value="Dynamic">
                                  <input type="hidden" name="ConnectionFlag" value="<% tcWebApi_get("WanInfo_WanIF","CONNECTION","s") %>">
                                  <input type="hidden" name="Enable_Flag" value="Yes">
                                  <input type="hidden" name="Disable_Flag" value="No">
                                  <input type="hidden" name="afterdeleteFlag" value="0">
                                  <input type="hidden" name="OperatorStyle" value="Add">
                                  <input type="hidden" name="dhcpv6pdflag" value="Yes">
                                  <input type="hidden" name="pppManualStatus_Flag" value="<% tcWebApi_get("WanInfo_WanIF","PPPManualStatus","s") %>">
                                  </LABEL></TD>
                      <TD><INPUT id=btnAddCnt onclick="btnAddWanCnt()" type=button value="新建连接"> 
                      </TD></TR></TBODY></TABLE>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>承载业务：</TD>
                      <TD><LABEL><SELECT id=serviceList onchange=onSelectSvrList() name=serviceList> 
						<OPTION value="TR069" <%if tcWebApi_get("WanInfo_WanIF","ServiceList","h") = "TR069" then asp_Write("selected") elseif tcWebApi_get("WanInfo_WanIF","ServiceList","h") = "N/A" then asp_Write("selected") end if %>>TR069
						<OPTION value="INTERNET" <%if tcWebApi_get("WanInfo_WanIF","ServiceList","h") = "INTERNET" then asp_Write("selected") end if %>>INTERNET
						<OPTION value="TR069_INTERNET" <%if tcWebApi_get("WanInfo_WanIF","ServiceList","h") = "TR069_INTERNET" then asp_Write("selected") end if %>>TR069_INTERNET
						<OPTION value="OTHER" <%if tcWebApi_get("WanInfo_WanIF","ServiceList","h") = "OTHER" then asp_Write("selected") end if %>>Other
						</SELECT> </LABEL></TD>
                                <TD>启用： 
                                  <LABEL>
                                  <INPUT id=cb_enblService onclick=cb_enblServiceChange() type=checkbox name=cb_enblService <%if tcWebApi_get("WanInfo_WanIF","Active","h") = "Yes" then asp_Write("checked") end if%>>
                             		<input id=enblService type=hidden name="enblService">
                                  </LABEL></TD></TR></TBODY></TABLE>
                  <DIV id=secVpiVci>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>VPI/VCI：</TD>
                      <TD>
					  <INPUT id=atmVpi maxLength=3 size=6 name=atmVpi value="<%if tcWebApi_get("WanInfo_WanPVC","VPI","h") <> "N/A" then tcWebApi_get("WanInfo_WanPVC","VPI","s") end if%>">&nbsp;/ 
					  <INPUT id=atmVci maxLength=5 size=6 name=atmVci value="<%if tcWebApi_get("WanInfo_WanPVC","VCI","h") <> "N/A" then tcWebApi_get("WanInfo_WanPVC","VCI","s") end if%>"> </TD></TR>
                    <TR></TR></TBODY></TABLE></DIV>
                  <TABLE style="DISPLAY: none" height=32 cellSpacing=0 
                  cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150 height="32">启用绑定：</TD>
                      <TD width=306><LABEL> 
                                  <INPUT id=cb_bindflag onclick=cb_bindflagChange() type=checkbox name="cb_bindflag" <%if tcWebApi_get("WanInfo_WanIF","BandActive","h") = "Yes" then asp_Write("checked") end if%>>
                                  <INPUT id=bindflag type=hidden value="<%tcWebApi_get("WanInfo_WanIF","BandActive","s") %>" name="bindflag">
                                  </LABEL></TD></TR></TBODY></TABLE>
                  <DIV id=secBind>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150 height="20">绑定选项：</TD>
                      <TD width="75"><LABEL></LABEL>
                        <DIV id=secLan1><INPUT id=cb_bindlan1 type=checkbox name=cb_bindlan1 <%if tcWebApi_get("WanInfo_WanIF","LAN1","h") = "Yes" then asp_Write("checked") end if%>> LAN1 
						<INPUT id=bindlan1 type=hidden value=0 name=bindlan1> 
						</DIV>
                       </TD>
                      <TD width="75">
                        <DIV id=secLan2>
						<INPUT id=cb_bindlan2 type=checkbox  name=cb_bindlan2 <%if tcWebApi_get("WanInfo_WanIF","LAN2","h") = "Yes" then asp_Write("checked") end if%>> LAN2 
						<INPUT id=bindlan2 type=hidden value=0 name=bindlan2> 
						</DIV>
                       </TD>
                      <TD width="77">
                        <DIV id=secLan3><INPUT id=cb_bindlan3 type=checkbox name=cb_bindlan3 <%if tcWebApi_get("WanInfo_WanIF","LAN3","h") = "Yes" then asp_Write("checked") end if%>> LAN3 <INPUT id=bindlan3 type=hidden 
                        value=0 name=bindlan3> </DIV>
                       </TD>
                      <TD width="79">
                        <DIV id=secLan4>
						<INPUT id=cb_bindlan4 type=checkbox name=cb_bindlan4 <%if tcWebApi_get("WanInfo_WanIF","LAN4","h") = "Yes" then asp_Write("checked") end if%>> LAN4 
						<INPUT id=bindlan4 type=hidden value=0 name=bindlan4> </DIV>
                        <LABEL></LABEL></TD></TR></TBODY></TABLE>
				<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                                <TR> 
                     <TD width=150 height="20">&nbsp;</TD>
                      <TD width="75"><LABEL></LABEL>
                        <DIV id=secWireless1>
						<INPUT id=cb_bindwireless1  type=checkbox name=cb_bindwireless1 <%if tcWebApi_get("WanInfo_WanIF","SSID1","h") = "Yes" then asp_Write("checked") end if%>> SSID1
						<INPUT id=bindwireless1 type=hidden value=0 name=bindwireless1> 
						</DIV></TD>
                      <TD width="75">
                        <DIV id=secWireless2>
						<INPUT id=cb_bindwireless2 type=checkbox name=cb_bindwireless2 <%if tcWebApi_get("WanInfo_WanIF","SSID2","h") = "Yes" then asp_Write("checked") end if%>> SSID2 <INPUT id=bindwireless2 
                        type=hidden value=0 name=bindwireless2> </DIV></TD>
                      <TD width="77">
                        <DIV id=secWireless3>
						<INPUT id=cb_bindwireless3 type=checkbox name=cb_bindwireless3 <%if tcWebApi_get("WanInfo_WanIF","SSID3","h") = "Yes" then asp_Write("checked") end if%>> SSID3 
						<INPUT id=bindwireless3 type=hidden value=0 name=bindwireless3> 
						</DIV></TD>
                      <TD width="79">
                        <DIV id=secWireless4><INPUT id=cb_bindwireless4 type=checkbox name=cb_bindwireless4 <%if tcWebApi_get("WanInfo_WanIF","SSID4","h") = "Yes" then asp_Write("checked") end if%>> SSID4 
						<INPUT id=bindwireless4 type=hidden value=0 name=bindwireless4> 
                        </DIV><LABEL></LABEL></TD></TR></TBODY></TABLE>
						</DIV>
                  <TABLE height=30 cellSpacing=0 cellPadding=0 width="100%" 
                  border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>模式：</TD>
                      <TD><LABEL> 
                                  <select id=select2 onChange=WanModeSelect() name="wanMode">
                                    <option value="Route" <%if tcWebApi_get("WanInfo_WanIF","WanMode","h") = "Route" then asp_Write("selected") end if%>>Route 
                                    <option value="Bridge" <%if tcWebApi_get("WanInfo_WanIF","WanMode","h") = "Bridge" then asp_Write("selected") end if%>>Bridge 
                                  </select>
                                  <script language="JavaScript" type="text/JavaScript">
								  getElById("secLan3").style.display = "none";
								  getElById("secLan4").style.display = "none";
								  var validSSID = "<% tcWebApi_get("WLan_Common","BssidNum","s") %>";
								  if(validSSID == "1"){
								  	setDisplay('secWireless1', 1);
								  	setDisplay('secWireless2', 0);
								  	setDisplay('secWireless3', 0);
									setDisplay('secWireless4', 0);
								  }
								  else if(validSSID == "2"){
								  	setDisplay('secWireless1', 1);
								  	setDisplay('secWireless2', 1);
								  	setDisplay('secWireless3', 0);
									setDisplay('secWireless4', 0);
								  }
								  else if(validSSID == "3"){
									setDisplay('secWireless1', 1);
								  	setDisplay('secWireless2', 1);
								  	setDisplay('secWireless3', 1);
									setDisplay('secWireless4', 0);
								  }
								  else if(validSSID == "4"){
									setDisplay('secWireless1', 1);
								  	setDisplay('secWireless2', 1);
								  	setDisplay('secWireless3', 1);
									setDisplay('secWireless4', 1);
								  }
								  </script>
                                  </LABEL></TD></TR></TBODY></TABLE>
                  <DIV id=divLink>
                  <TABLE height=30 cellSpacing=0 cellPadding=0 width="100%" 
                  border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>链接方式：</TD>
                      <TD><LABEL> 
                                    <SELECT id=linkMode onchange=linkModeSelect() name="linkMode">
                                      <OPTION value="linkIP" <%if tcWebApi_get("WanInfo_WanIF","LinkMode","h") = "linkIP" then asp_Write("selected") end if%>>通过IP方式建立链接 
                                      <OPTION value="linkPPP" <%if tcWebApi_get("WanInfo_WanIF","LinkMode","h") = "linkPPP" then asp_Write("selected") end if%>>通过PPP方式建立链接 
                                    </SELECT>
                                    </LABEL></TD></TR></TBODY></TABLE></DIV>
                  <DIV 
                  id=divIpVersion>IP协议版本：&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
                  <INPUT id=IpVersion onclick="IpVersionChange()" type=radio value="IPv4" name="IpVersion" <%if tcWebApi_get("WanInfo_WanIF","IPVERSION", "h") = "IPv4" then asp_Write("checked") elseif tcWebApi_get("Wan_Entry","IPVERSION","h") = "N/A" then asp_Write("checked") end if%>>IPv4&nbsp;&nbsp; 
				  <INPUT id=IpVersion onclick="IpVersionChange()" type=radio value="IPv6" name="IpVersion" <%if tcWebApi_get("WanInfo_WanIF","IPVERSION","h") = "IPv6" then  asp_Write("checked") end if%>>IPv6&nbsp;&nbsp; 
				  <INPUT id=IpVersion onclick="IpVersionChange()" type=radio value="IPv4/IPv6" name="IpVersion" <%if tcWebApi_get("WanInfo_WanIF","IPVERSION","h") = "IPv4/IPv6" then asp_Write("checked") end if%>>IPv4/IPv6&nbsp;&nbsp; 
				  </DIV>
                          <input type="hidden" name="IPVersionValue" value="IPv4/IPv6">
                          <input type="hidden" name="ISPTypeValue" value="<% tcWebApi_get("WanInfo_WanIF","ISP","s") %>">
                          <BR>
                  <DIV id=secIpMode>
                  <DIV id=secDhcp>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150 height="20"><INPUT onclick="IpModeChange()" type=radio value="DHCP" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "0" then asp_Write("checked") end if%>> DHCP</TD>
                      <TD>从ISP处自动得到一个IP地址.</TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secStatic>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150><INPUT onclick="IpModeChange()" type=radio value="Static" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "1" then asp_Write("checked") end if%>> Static</TD>
                      <TD>经ISP配置一个静态的IP给你.</TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secPppoe>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150><INPUT onclick="IpModeChange()" type=radio value="PPPoE" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "2" then asp_Write("checked") end if%>> PPPoE</TD>
                      <TD>若ISP使用PPPoE请选择该项.</TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secPppoa>
                  <TABLE style="DISPLAY: none" cellSpacing=0 cellPadding=0 
                  width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150><INPUT onclick="IpModeChange()" type=radio value="PPPoA" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "3" then asp_Write("checked") end if%>> PPPoA</TD>
                      <TD>若ISP使用PPPoA请选择该项.</TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secIpoa>
                  <TABLE style="DISPLAY: none" cellSpacing=0 cellPadding=0 
                  width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150><INPUT onclick="IpModeChange()" type=radio  value="IPoA" name="IpMode" <%if tcWebApi_get("WanInfo_WanIF","ISP","h") = "4" then asp_Write("checked") end if%>> IPoA</TD>
                                    <TD>若ISP使用IPoA请选择该项. </TD>
                                  </TR></TBODY></TABLE></DIV><BR>
                          </DIV>
                  <DIV id=secBridgeType style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>桥类型：</TD>
                      <TD><LABEL>
					  <SELECT id=bridgeMode name="bridgeMode"> 
                          <OPTION value="PPPoE_Bridged" <%if tcWebApi_get("WanInfo_WanIF","BridgeMode","h") = "PPPoE_Bridged" then asp_Write("selected") end if%>>PPPoE_Bridged</OPTION> 
						  <OPTION value="IP_Bridged" <%if tcWebApi_get("WanInfo_WanIF","BridgeMode","h") = "IP_Bridged" then asp_Write("selected") end if%>>IP_Bridged</OPTION></SELECT> 
                    </LABEL></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secbridgeDhcprelay style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>使能DHCP透传：</TD>
                      <TD><LABEL>
                                    <INPUT id=cb_dhcprelay type=checkbox name=cb_dhcprelay onClick="EnableDHCPRealy()" <%if tcWebApi_get("WanInfo_WanIF","DHCPRealy","h") = "Yes" then asp_Write("checked") end if%>>
                                    <input type="hidden" name="dhcprelay" value="No">
                                    </LABEL></TD></TR></TBODY></TABLE></DIV>
				  <INPUT id=multMode type=hidden value=0 name=multMode>
                          <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>使能802.1q：</TD>
                      <TD><LABEL><INPUT id=enbl8021q onclick=Enbl8021qChange() type=checkbox name=enbl8021q <%if tcWebApi_get("WanInfo_WanIF","dot1q","h") = "Yes" then asp_Write("checked") end if%>> 
					  <INPUT id=vlanId type=hidden name=vlanId value="No"> </LABEL></TD>
                      <TD>
                        <DIV id=secVlan style="DISPLAY: none">
                        <TABLE cellSpacing=0 cellPadding=0 width="100%" 
border=0>
                          <TBODY>
                          <TR>
                            <TD width=120>VLAN ID[0-4094]：</TD>
                            <TD><LABEL><INPUT id=vlan maxLength=4 size=5 name=vlan value="<%if tcWebApi_get("WanInfo_WanIF","VLANID","h") <> "N/A" then tcWebApi_get("WanInfo_WanIF","VLANID","s") end if%>"> 
                    </LABEL></TD></TR></TBODY></TABLE></DIV></TD></TR></TBODY></TABLE>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>使能802.1p：</TD>
                      <TD><LABEL><INPUT id=enbl8021d onclick=Enbl8021dChange() type=checkbox name=enbl8021d <%if tcWebApi_get("WanInfo_WanIF","dot1p","h") = "Yes" then asp_Write("checked") end if%>> 
					  <INPUT id=vlanPri type=hidden name=vlanPri value="No"> </LABEL></TD>
                      <TD>
                        <DIV id=sec8021d style="DISPLAY: none">
                        <TABLE cellSpacing=0 cellPadding=0 width="100%" 
border=0>
                          <TBODY>
                          <TR>
                            <TD width=120>取值[0-7]：</TD>
                            <TD><LABEL><INPUT id=v8021d maxLength=1 size=5 name=v8021d value="<%if tcWebApi_get("WanInfo_WanIF","dot1pData","h") <> "N/A" then tcWebApi_get("WanInfo_WanIF","dot1pData","s") end if%>"> 
                    </LABEL></TD></TR></TBODY></TABLE></DIV></TD></TR></TBODY></TABLE>
                  <DIV id=secNat>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>使能NAT：</TD>
                      <TD><LABEL>
                                    <INPUT id=cb_nat type=checkbox name=cb_nat onClick="EnableNatClick()" <%if tcWebApi_get("WanInfo_WanIF","NATENABLE","h") = "Enable" then asp_Write("checked") end if%>>
                                    <INPUT id=nat type=hidden value="<% tcWebApi_get("WanInfo_WanIF","NATENABLE","s") %>" name="nat">
                                    </LABEL></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secIgmp>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR style="DISPLAY: none">
                      <TD width=150>使能IGMP Proxy：</TD>
                      <TD><LABEL>
                                    <INPUT id=cb_enblIgmp type=checkbox name="cb_enblIgmp" <%if tcWebApi_get("WanInfo_WanIF","IGMPproxy","h") = "Yes" then asp_Write("checked") end if%>>
                                    <INPUT id=enblIgmp type=hidden value="No" name=enblIgmp>
                                    </LABEL></TD></TR></TBODY></TABLE></DIV>
                          <BR>
                  <TABLE height=23 cellSpacing=0 cellPadding=0 width="100%" 
                  border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>服务类别：</TD>
                      <TD><LABEL>
                                  <select id=select onChange="atmServiceCategoryChange()" size=1 name="atmServiceCategory">
                                    <option value="ubr" <%if tcWebApi_get("WanInfo_WanPVC","QOS","h") = "ubr" then asp_Write("selected") end if%>>UBR Without PCR 
                                    <option value="ubr+"  <%if tcWebApi_get("WanInfo_WanPVC","QOS","h") = "ubr+" then asp_Write("selected") end if%>>UBR With PCR 
                                    <option value="cbr" <%if tcWebApi_get("WanInfo_WanPVC","QOS","h") = "cbr" then asp_Write("selected") end if%>>CBR 
                                    <option value="nrt-vbr" <%if tcWebApi_get("WanInfo_WanPVC","QOS","h") = "nrt-vbr" then asp_Write("selected") end if%>>Non-Real Time VBR 
                                    <option value="rt-vbr" <%if tcWebApi_get("WanInfo_WanPVC","QOS","h") = "rt-vbr" then asp_Write("selected") end if%>>Real Time VBR 
                                  </select>
                                  </LABEL></TD></TR></TBODY></TABLE>
                  <DIV id=secAtmPeakCellRate style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>峰值信元速率
                        <SCRIPT language=JavaScript type=text/javascript>
document.writeln('[0-' + pcrMax + ']');
</SCRIPT>
                         ：</TD>
                      <TD><LABEL>
                                    <INPUT id=atmPeakCellRate size=7 name="atmPeakCellRate" value="<%if TCWebApi_get("WanInfo_WanPVC","PCR","h") <> "N/A" then TCWebApi_get("WanInfo_WanPVC","PCR","s") else asp_Write("0") end if%>">
                                    [信元/秒] </LABEL></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secAtmSustainedCellRate style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>持续信元速率
                        <SCRIPT language=JavaScript type=text/javascript>
document.writeln('[0-' + pcrMax + ']');
</SCRIPT>
                         ：</TD>
                      <TD><LABEL><INPUT id=atmSustainedCellRate size=7 name=atmSustainedCellRate value="<%if TCWebApi_get("WanInfo_WanPVC","SCR","h") <> "N/A" then TCWebApi_get("WanInfo_WanPVC","SCR","s") else asp_Write("0") end if%>"> 
                  [信元/秒]</LABEL></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secAtmMaxBurstSize style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>最大突发尺寸[0-65535]：</TD>
                      <TD><LABEL><INPUT id=atmMaxBurstSize maxLength=7 size=7 name=atmMaxBurstSize value="<%if TCWebApi_get("WanInfo_WanPVC","MBS","h") <> "N/A" then TCWebApi_get("WanInfo_WanPVC","MBS","s") else asp_Write("0") end if%>"> 
                  [信元]</LABEL></TD></TR></TBODY></TABLE></DIV>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>封装模式：</TD>
                      <TD><SELECT id=encapMode size=1 name="encapMode"> 
                          <OPTION value="LLC" <%if tcWebApi_get("WanInfo_WanPVC","ENCAP","h") = "PPPoE LLC" then asp_Write("selected") elseif  tcWebApi_get("WanInfo_WanPVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if%>>LLC 
						  <OPTION value="VCMUX" <%if tcWebApi_get("WanInfo_WanPVC","ENCAP","h") = "PPPoE VC-Mux" then asp_Write("selected") elseif tcWebApi_get("WanInfo_WanPVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if%>>VCMUX
						  </SELECT>
                                  <LABEL> </LABEL></TD></TR></TBODY></TABLE>
                  <DIV id=secRouteItems>
                  <DIV id=secDhcpItems style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
					<%if TCWebApi_get("WebCurSet_Entry","isSupportDCOption60","h" ) = "1" then%>
                    <TR id="hideTR">
                      <TD width=150>DHCP Option 60:</TD>
                      <TD><LABEL><INPUT id=wanDhcpCode maxLength=15 size=15 name="wanDhcpCode" value="<%if TCWebApi_get("WanInfo_WanIF","option60String","h") <> "N/A" then TCWebApi_get("WanInfo_WanIF","option60String","s" ) end if %>"> 
					  </LABEL></TD></TR><%end if%></TBODY></TABLE></DIV>
                  <DIV id=secStaticItems style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>IP 地址：</TD>
                      <TD><LABEL><INPUT id=wanIpAddress maxLength=15 size=15 name=wanIpAddress value="<%if TCWebApi_get("WanInfo_WanIF","IPADDR","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","IPADDR","s" ) end if %>"> </LABEL></TD></TR>
                    <TR>
                      <TD>子网掩码：</TD>
                      <TD><INPUT id=wanSubnetMask maxLength=15 size=15 name=wanSubnetMask value="<%if TCWebApi_get("WanInfo_WanIF","NETMASK","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","NETMASK","s" ) end if %>">
                                    </TD>
                                  </TR>
                    <TR>
                      <TD>缺省网关：</TD>
                      <TD><INPUT id=defaultGateway maxLength=15 size=15 name=defaultGateway value="<%if TCWebApi_get("WanInfo_WanIF","GATEWAY","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","GATEWAY","s" ) end if %>"></TD></TR>
                    <TR>
                      <TD>首选DNS服务器：</TD>
                      <TD><INPUT id=dnsPrimary maxLength=15 size=15 name=dnsPrimary value="<%if tcWebApi_get("WanInfo_WanIF","DNS","h") <> "N/A" then tcWebApi_get("WanInfo_WanIF","DNS","s") end if%>"></TD></TR>
                    <TR>
                      <TD>备用DNS服务器：</TD>
                      <TD><INPUT id=dnsSecondary maxLength=15 size=15 name=dnsSecondary value="<%if tcWebApi_get("WanInfo_WanIF","SecDNS","h") <> "N/A" then tcWebApi_get("WanInfo_WanIF","SecDNS","s") end if%>"></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secPppoeItems>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>用户名：</TD>
                      <TD><LABEL><INPUT id=pppUserName style="FONT-FAMILY: '宋体'" maxLength=63 size=15 
                        name=pppUserName value="<%if TCWebApi_get("WanInfo_WanIF","USERNAME","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","USERNAME","s" ) end if %>"> </LABEL></TD></TR>
                    <TR>
                      <TD>密码：</TD>
                      <TD>
					  	<span id="inpsw"><INPUT id=pppPassword style="FONT-FAMILY: '宋体'" type=password maxLength=63 size=15 name=pppPassword value="<% if TCWebApi_get("WanInfo_WanIF","PASSWORD","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","PASSWORD","s" ) end if %>"></span>
						<%if TCWebApi_get("WebCurSet_Entry","IsSupportShowPSW","h" ) = "1" then%>
                        <INPUT id="cb_enablshowpsw" onclick="doshowpswChange()" type=checkbox name="cb_enablshowpsw">显示密码
						<script language="JavaScript" type="text/JavaScript">
						
						function doshowpswChange() {
							with (getElById('ConfigForm')){
								var tempvalue = pppPassword.value;
								if (cb_enablshowpsw.checked) {
									inpsw.innerHTML = "<input id=\"pppPassword\" style=\"FONT-FAMILY: '宋体'\" type=\"text\" maxLength=63 size=15 name=\"pppPassword\" value=" +document.ConfigForm.pppPassword.value+ ">";
								}
								else {
									inpsw.innerHTML = "<input id=\"pppPassword\" style=\"FONT-FAMILY: '宋体'\" type=\"password\" maxLength=63 size=15 name=\"pppPassword\" value=" +document.ConfigForm.pppPassword.value+ ">";
								}
								pppPassword.value = tempvalue;
							}
						
						}
						
						</script>
						<%end if%></TD>
                    </TR>
                    <TR style="DISPLAY: none">
                      <TD>服务名称：</TD>
                      <TD><INPUT id=pppServerName style="FONT-FAMILY: '宋体'"  maxLength=63 size=15 name=pppServerName>
                                    </TD>
                                  </TR>
                    <TR>
                      <TD>拨号方式：</TD>
                      <TD><SELECT id=DialMethod style="WIDTH: 117px" onchange=DialMethodChange() name=DialMethod> 
						<OPTION value="AlwaysOn" <%if TCWebApi_get("WanInfo_WanIF","CONNECTION","h" ) = "Connect_Keep_Alive" then asp_Write("selected") end if %>>自动连接</OPTION> 
						<OPTION value="Manual" <%if tcWebApi_get("WanInfo_WanIF","CONNECTION","h") = "Connect_Manually" then asp_Write("selected")  end if%>>手动拨号</OPTION></SELECT>
                                    </TD>
                                  </TR>
                    <TR id=secManualDial style="DISPLAY: none">
                      <TD>&nbsp;</TD>
                      <TD><INPUT id=pppDialButton onclick=ManualCntSubmit() type=button value=手动拨号 name=pppDialButton> 
                      </TD></TR>
                    <TR id=secIdleTime style="DISPLAY: none">
                      <TD>空闲超时（分钟）[1-4320]</TD>
                                    <TD> 
                                      <INPUT id=pppTimeOut maxLength=4 size=4 name=pppTimeOut>
                                      <input type="hidden" name="pppv6Mode" value="0">
                                    </TD>
                                  </TR></TBODY></TABLE></DIV></DIV>
                  <DIV id=secIPv6Div>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR id=TrIpv6AddrType>
                      <TD width=150>IPv6 WAN信息获取方式:</TD>
								<TD><select id="IdIpv6AddrType" style="WIDTH: 130px" onChange="OnIPv6Changed()" name="IdIpv6AddrType">
									  <option value="SLAAC" <%if TCWebApi_get("WanInfo_WanIF","DHCPv6","h" ) = "No" then asp_Write("selected") end if %>>SLAAC 
                                      <option value="DHCP" <%if TCWebApi_get("WanInfo_WanIF","DHCPv6","h" ) = "Yes" then asp_Write("selected") end if %>>DHCP
									  <option value="Static" <%if TCWebApi_get("WanInfo_WanIF","DHCPv6","h" ) = "N/A" then asp_Write("selected") end if %>>Static 
									</select>
                                 </TD>
                                </TR>
                    <TR id=TrIpv6Addr>
                      <TD>IPv6地址:</TD>
                      <TD><INPUT id=IdIpv6Addr maxLength=39 size=36 name=IdIpv6Addr value="<%if TCWebApi_get("WanInfo_WanIF","IPADDR6","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","IPADDR6","s" ) end if %>">&nbsp;/ 
					  <INPUT id=IdIpv6PrefixLen maxLength=3 size=3 name=IdIpv6PrefixLen value="<% if TCWebApi_get("WanInfo_WanIF","PREFIX6","h" ) <> "N/A" then TCWebApi_get("WanInfo_WanIF","PREFIX6","s" ) end if %>"> 
					  </TD></TR>
                    <TR id=TrIpv6Gateway>
                      <TD>IPv6默认网关:</TD>
                      <TD><INPUT id=IdIpv6Gateway maxLength=39 size=36 name=IdIpv6Gateway value="">
                                    <script language="JavaScript" type="text/JavaScript">
									var ipv6gwstr = "<% tcWebApi_get("WanInfo_WanIF","GATEWAY6","s" ) %>";
									if("N/A" == ipv6gwstr)
										setText('IdIpv6Gateway', "");
									else
										setText('IdIpv6Gateway', ipv6gwstr);
									</script></TD></TR>
					<TR id="TrIpv6GatewayInfo">
					<TD></TD>
					<TD>(IPv6默认网关不填内容，则自动获取)</TD></TR>
                    <TR id=TrIpv6Dns1>
                      <TD>IPv6首选DNS服务器:</TD>
                      <TD><INPUT id=IdIpv6Dns1 maxLength=39 size=36 name=IdIpv6Dns1 value="<%if tcWebApi_get("WanInfo_WanIF","DNS6","h") <> "N/A" then tcWebApi_get("WanInfo_WanIF","DNS6","s") end if%>"></TD></TR>
                    <TR id=TrIpv6Dns2>
                      <TD>IPv6备用DNS服务器:</TD>
                      <TD><INPUT id=IdIpv6Dns2 maxLength=39 size=36 name=IdIpv6Dns2 value="<%if tcWebApi_get("WanInfo_WanIF","SecDNS6","h") <> "N/A" then tcWebApi_get("WanInfo_WanIF","SecDNS6","s") end if%>">
					  </TD></TR></TBODY></TABLE></DIV><LABEL></LABEL><BR>
                          <LEFT> 
                          <INPUT id=btnRemoveCnt onclick="btnRemoveWanCnt()" type=button value=删除连接>
                          </LEFT></TD>
                      </TR>
              <TR>
                <TD><INPUT id=pppIdleTimeout type=hidden value=0 name=pppIdleTimeout></TD></TR></TBODY></TABLE></FORM></TD></TR>
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
            <P align=center><IMG id=btnOK onclick="btnSave();" height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick=onCancel() height=23 
            src="/img/cancel.gif" width=80 border=0> </P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
