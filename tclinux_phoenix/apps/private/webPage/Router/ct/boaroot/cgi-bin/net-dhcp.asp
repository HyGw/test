<%
if Request_Form("IPV4_Flag") = "1" then
	TCWebApi_set("Lan_Entry","IP","ethIpAddress")
	TCWebApi_set("Lan_Entry","netmask","ethSubnetMask")
	TCWebApi_set("Lan_Dhcp","type","dhcpSrvType")
	
	if Request_Form("dhcpSrvType") = "0" then
		TCWebApi_unset("DhcpRelay_Entry")
        TCWebApi_commit("Dhcpd_Common")
	end if
	
	if Request_Form("dhcpSrvType") = "1" then
		TCWebApi_set("Dhcpd_Common","lease","dhcpLeasedTimeFrag")
		TCWebApi_set("Dhcpd_Common","start","dhcpEthStart")
		TCWebApi_set("Dhcpd_Common","pool_count","EthendIPcount")
		TCWebApi_set("Dhcpd_Option60","DeviceStyle","IpPoolIndex")
		TCWebApi_set("Dhcpd_Option60","lease","dhcpLeasedTimeFrag")
		if Request_Form("IpPoolIndex") = "STB" then
			TCWebApi_set("Dhcpd_Option60","startSTB","dhcpEthStartFrag")
			TCWebApi_set("Dhcpd_Option60","pool_countSTB","DeviceendIPcount")
		elseif Request_Form("IpPoolIndex") = "Phone" then
			TCWebApi_set("Dhcpd_Option60","startPhone","dhcpEthStartFrag")
			TCWebApi_set("Dhcpd_Option60","pool_countPhone","DeviceendIPcount")
		elseif Request_Form("IpPoolIndex") = "Camera" then
			TCWebApi_set("Dhcpd_Option60","startCamera","dhcpEthStartFrag")
			TCWebApi_set("Dhcpd_Option60","pool_countCamera","DeviceendIPcount")
		else
			TCWebApi_set("Dhcpd_Option60","start","dhcpEthStartFrag")
			TCWebApi_set("Dhcpd_Option60","pool_count","DeviceendIPcount")
		end if
		
		TCWebApi_unset("DhcpRelay_Entry")
		TCWebApi_commit("Dhcpd_Common")
		TCWebApi_commit("Dhcpd_Option60")
	end if
	
	if Request_Form("dhcpSrvType") = "2" then
		TCWebApi_set("DhcpRelay_Entry","Server","dhcpRelayServerIP")
		TCWebApi_commit("DhcpRelay_Entry")
        TCWebApi_commit("Dhcpd_Common")
	end if
	
	TCWebApi_commit("Lan")
	TCWebApi_save()
end if

if Request_Form("RA_Flag") = "1" then
	TCWebApi_set("Radvd_Entry","Enable","RAEnable_Flag")
	TCWebApi_set("Radvd_Entry","Mode","RAMode")
	if Request_Form("RAMode") = "1" then
		TCWebApi_set("Radvd_Entry","PrefixIPv6","radvdPrefix")
		TCWebApi_set("Radvd_Entry","Prefixv6Len","RAPrefixLen")
		TCWebApi_set("Radvd_Entry","PreferredLifetime","RAt1")
		TCWebApi_set("Radvd_Entry","ValidLifetime","RAt2")
		TCWebApi_set("Radvd_Entry","ManagedEnable","RAManagedEn_Flag")
		TCWebApi_set("Radvd_Entry","OtherEnable","RAOtherEn_Flag")
		TCWebApi_set("Radvd_Entry","MinRAInterval","RACycMin")
		TCWebApi_set("Radvd_Entry","MaxRAInterval","RACycMax")
	end if
	
	TCWebApi_commit("Radvd_Entry")
	TCWebApi_save()
end if

if Request_Form("Dhcp1_Flag") = "1" then
	TCWebApi_set("Dhcp6s_Entry","Enable","EnDHCPServerFlag")
	TCWebApi_set("Dhcp6s_Entry","Mode","DHCPSetTypeFlag")
	if Request_Form("DHCPSetTypeFlag") = "1" then
		TCWebApi_set("Dhcp6s_Entry","PrefixIPv6","dhcpPrefix")
		TCWebApi_set("Dhcp6s_Entry","Prefixv6Len","PrefixLen")
		TCWebApi_set("Dhcp6s_Entry","PreferredLifetime","t1")
		TCWebApi_set("Dhcp6s_Entry","ValidLifetime","t2")
		TCWebApi_set("Dhcp6s_Entry","DNSserver","DnsSrvOne")
		TCWebApi_set("Dhcp6s_Entry","SecDNSserver","DnsSrvTwo")
	end if
	
	TCWebApi_commit("Dhcp6s_Entry")
	TCWebApi_save("Dhcp6s_Entry")
end if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>DHCP配置</TITLE>
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
            value="网络->LAN侧地址配置" name=Selected_Menu> </FONT></B><SPAN 
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
	  function	getendetherIPforCount(start, count)
	  {
	  	var vret = "";
		var S = start.split('.');
		if(S.length > 3)
		{
			var temp = parseInt(count) + parseInt(S[3]) - 1;
			vret = S[0] + '.' + S[1] + '.' + S[2] + '.' + temp.toString();
		}
		return vret;
	  }
function dhcpcnst(domain,dhcpStart,dhcpEnd,LeaseTime,Enable,STBMinAddr,STBMaxAddr,STBRelay,PhoneMinAddr,PhoneMaxAddr,PhoneRelay,CameraMinAddr,CameraMaxAddr,CameraRelay,ComputerMinAddr,ComputerMaxAddr,ComputerRelay, PortRelayEnable, PortRelayStr)
{
this.domain = domain;
this.dhcpStart = dhcpStart;
this.dhcpEnd = dhcpEnd;
this.LeaseTime = LeaseTime;
this.Enable = Enable;
this.STBMinAddr = STBMinAddr;
this.STBMaxAddr = STBMaxAddr;
this.STBRelay = STBRelay;
this.PhoneMinAddr = PhoneMinAddr;
this.PhoneMaxAddr = PhoneMaxAddr ;
this.PhoneRelay = PhoneRelay;
this.CameraMinAddr = CameraMinAddr;
this.CameraMaxAddr  = CameraMaxAddr ;
this.CameraRelay = CameraRelay;
this.ComputerMinAddr = ComputerMinAddr;
this.ComputerMaxAddr = ComputerMaxAddr ;
this.ComputerRelay = ComputerRelay ;
this.PortRelayEnable = PortRelayEnable;
this.PortRelayStr = PortRelayStr;
}
var dhcpst = new Array(new dhcpcnst("InternetGatewayDevice.LANDevice.1.LANHostConfigManagement","<% tcWebApi_get("Dhcpd_Common","start","s") %>","<% tcWebApi_get("Dhcpd_Common","pool_count","s") %>","86400","1","<% tcWebApi_get("Dhcpd_Option60","startSTB","s" ) %>","<% tcWebApi_get("Dhcpd_Option60","pool_countSTB","s" ) %>","0","<% tcWebApi_get("Dhcpd_Option60","startPhone","s" ) %>","<% tcWebApi_get("Dhcpd_Option60","pool_countPhone","s" ) %>","0","<% tcWebApi_get("Dhcpd_Option60","startCamera","s" ) %>","<% tcWebApi_get("Dhcpd_Option60","pool_countCamera","s" ) %>","0","<% tcWebApi_get("Dhcpd_Option60","start","s" ) %>","<% tcWebApi_get("Dhcpd_Option60","pool_count","s" ) %>","0","0",""),null);
poolIndex = top.lanDevIndex;
var proto = ''
var natEnbl = 1;
var dhcpEnbl = dhcpst[0].Enable;
var dhcpStart = "";
if(dhcpst[0].dhcpStart != "N/A")
	dhcpStart = dhcpst[0].dhcpStart;
var dhcpEnd = getendetherIPforCount(dhcpStart,dhcpst[0].dhcpEnd);
var dhcpLease = dhcpst[0].LeaseTime;
var dhcpStart1_1 = dhcpst[0].STBMinAddr;
var dhcpEnd1_1 = getendetherIPforCount(dhcpStart1_1, dhcpst[0].STBMaxAddr);
var enblrelay_stb = dhcpst[0].STBRelay;
var dhcpStart1_2 = dhcpst[0].PhoneMinAddr;
var dhcpEnd1_2 = getendetherIPforCount(dhcpStart1_2, dhcpst[0].PhoneMaxAddr);
var enblrelay_phone = dhcpst[0].PhoneRelay;
var dhcpStart1_3 = dhcpst[0].CameraMinAddr
var dhcpEnd1_3 = getendetherIPforCount(dhcpStart1_3, dhcpst[0].CameraMaxAddr);
var enblrelay_camera = dhcpst[0].CameraRelay;
var dhcpStart1_4 = dhcpst[0].ComputerMinAddr;
var dhcpEnd1_4 = getendetherIPforCount(dhcpStart1_4, dhcpst[0].ComputerMaxAddr);
var enblrelay_computer = dhcpst[0].ComputerRelay;
var dhcpPortRelayEnable = dhcpst[0].PortRelayEnable;
var host = [["InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1","192.168.1.1","255.255.255.0"],null];
var oldAddrPrimary;
var oldMaskPrimary;
if (host.length > 1)
{
oldAddrPrimary = host[0][1];
oldMaskPrimary = host[0][2];
}
function dhcpv6cnst(domain,Enable,AllocateWan,Prefix,PrefixLength,Preferredlifetime,Validlifetime,DnsSrver)
{
this.domain            = domain;
this.Enable            = Enable;
this.allocateWan       = AllocateWan;
this.Prefix            = Prefix;
this.PrefixLength      = PrefixLength;
this.Preferredlifetime = Preferredlifetime;
this.Validlifetime     = Validlifetime;
var dns           = DnsSrver.split(',');
this.DnsSrvOne    = dns[0];
if ( dns.length > 1 )
{
this.DnsSrvTwo = dns[1];
}
else
{
this.DnsSrvTwo = '';
}
}
var dhcpv6st = new Array(new dhcpv6cnst("InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_CT-COM_DHCPv6","1","UseAllocatedSubnet","","64","3600","7200",""),null);
if (dhcpv6st.length > 0)
{
var dhcpEnb6l = dhcpv6st[0].Enable;
var UseAllc  = dhcpv6st[0].allocateWan;
var DnsOne   = dhcpv6st[0].DnsSrvOne;
var DnsTwo   = dhcpv6st[0].DnsSrvTwo;
if (DnsOne == '')
{
DnsOne = "fe80::1";
}
var Prefix   = dhcpv6st[0].Prefix;
var Prelen   = dhcpv6st[0].PrefixLength;
var Domana   = "";
var Time1    = dhcpv6st[0].Preferredlifetime;
var Time2    = dhcpv6st[0].Validlifetime;
}
else
{
var dhcpEnb6l = "1";
var UseAllc  = "UseAllocatedSubnet";
var DnsOne   = "fe80::1";
var DnsTwo   = "";
var Prefix   = "";
var Prelen   = "64";
var Domana   = "";
var Time1    = "3600";
var Time2    = "7200";
}
function radvdcnst(domain,Enable,AllocateWan,AssConn,Prefix,PrefixLength,Preferredlifetime,Validlifetime)
{
this.domain            = domain;
this.allocateWan       = AllocateWan;
this.AssConn           = AssConn;
this.Prefix            = Prefix;
this.PrefixLength      = PrefixLength;
this.Enable            = Enable;
this.Preferredlifetime = Preferredlifetime;
this.Validlifetime     = Validlifetime;
}
var radvdst = new Array(new radvdcnst("InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_CT-COM_SLAAC","1","Normal","1111:db8:1:0::","1111:db8:1:0::","64","3600","7200"),null);
if (radvdst.length > 1)
{
var radvdEnbl = radvdst[0].Enable;
var RAUseAllc  = radvdst[0].allocateWan;
var RAAssCon   = radvdst[0].AssConn;
var RAPrefix   = radvdst[0].Prefix;
var RAPrelen   = radvdst[0].PrefixLength;
var RATime1    = radvdst[0].Preferredlifetime;
var RATime2    = radvdst[0].Validlifetime;
}
else
{
var radvdEnbl = "1";
var RAUseAllc  = "UseAllocatedSubnet";
var RAAssCon   = "";
var RAPrefix   = "";
var RAPrelen   = "64";
var RATime1    = "3600";
var RATime2    = "7200";
}
var changeAddrFlag = false;
function disableDhcp6Srv()
{
with ( document.forms[2] )
{
setCheck("dhcp6SrvType" ,0);
setDisplay('AssConnection',0);
setDisplay('DHCP6SPrefixCfg',0);
setDisable('dhcpPrefix',1);
setDisable('PrefixLen' ,1);
setDisable('t1',1);
setDisable('t2',1);
setDisable('DnsSrvOne',1);
setDisable('DnsSrvTwo',1);
setDisable('DomainNameText'    ,1);
}
}
function enableDhcp6Srv(formLoad)
{
with ( document.forms[2] )
{
setCheck("dhcp6SrvType",1);
setDisplay('AssConnection',1);
if ( "Normal" == UseWan.value )
{
setDisplay('DHCP6SPrefixCfg', 1);
}
setDisable('dhcpPrefix',0);
setDisable('PrefixLen' ,0);
setDisable('t1',0);
setDisable('t2',0);
setDisable('DnsSrvOne',0);
setDisable('DnsSrvTwo',0);
setDisable('DomainNameText'    ,1);
}
}

function EnablePrefixParameter()
{
with (document.forms[2])
{
if ( 0 != getCheckVal('dhcp6SrvType'))
{
setDisable('dhcpPrefix',0);
setDisable('NamePrefix',0);
setDisable('PrefixLen'    ,0);
setDisable('NamePrefixLen',0);
}
}
}
function DisablePrefixParameter()
{
with (document.forms[2])
{
if ( 0 != getCheckVal('dhcp6SrvType'))
{
setDisable('dhcpPrefix',1);
setDisable('NamePrefix',1);
setDisable('PrefixLen'    ,1);
setDisable('NamePrefixLen',1);
}
}
}
function EnableIPPoolParameter()
{
with (document.forms[2])
{
}
}
function DisableIPPoolParameter()
{
with (document.forms[2])
{
}
}
function IPPoolGenerateOperation()
{
with (document.forms[2])
{
if ( "UsePrefix" == UsePrefixOrIPPool.value )
{
DisableIPPoolParameter();
EnablePrefixParameter();
}
else if ( "UseIPPool" == UsePrefixOrIPPool.value )
{
DisablePrefixParameter();
EnableIPPoolParameter();
}
else
{
DisablePrefixParameter();
DisableIPPoolParameter();
}
}
}
function CheckPrefix(Prefix,type)
{
var IpAddress1 = Prefix.split(':');
var IpAddress3 = Prefix;
if ( 1 == type && '' == IpAddress3)
{
return true;
}
if ( true != isUnicastIpv6Address(IpAddress3) )
{
return false;
}
switch (type)
{
case 0 :
if ( 15 == parseInt(IpAddress1[0].substring(0, 1),16) )
{
switch ( parseInt(IpAddress1[0].substring(1, 2),16) )
{
case 15 :
return false;
case 14 :
if ( 8 <= parseInt(IpAddress1[0].substring(2, 3),16) && 12 > parseInt(IpAddress1[0].substring(2, 3),16) )
{
return false;
}
break;
default:
break;
}
}
break;
case 1 :
if ( 15 == parseInt(IpAddress1[0].substring(0, 1),16) )
{
switch ( parseInt(IpAddress1[0].substring(1, 2),16) )
{
case 15 :
return false;
default:
break;
}
}
break;
default:
return false;
}
return true;
}
function CheckLength(Length)
{
var TemLen = parseInt(Length);
if ( true == isNaN(TemLen) || TemLen > 64 || TemLen < 16)
{
return false;
}
return true;
}
function CheckHex(Hex)
{
var TemHex  = Hex;
var Loopj   = 0;
var Tempi   = 0;
var Tempj   = 1;
var section = 0;
var num     = 0;
for (Loopj = 0 ; Loopj < TemHex.length ; Loopj++)
{
section = TemHex.substring(Tempi, Tempj);
num = parseInt(section, 16);
if ( true == isNaN(num) )
{
return false;
}
Tempi++;
Tempj++;
}
return true;
}
function CheckTime(Time1,Time2)
{
var TemTime1 = Time1;
var TemTime2 = Time2;
if ( TemTime1.length > 10 || '' == TemTime1 )
{
return 1;
}
if ( TemTime2.length > 10 || '' == TemTime2 )
{
return 2;
}
if ( true != isPlusInteger(TemTime1))
{
return 1;
}
if ( true != isPlusInteger(TemTime2))
{
return 2;
}
TemTime1 = parseInt(Time1);
TemTime2 = parseInt(Time2);
if ( TemTime1 > 4294967295 || TemTime1 < 60 )
{
return 1;
}
if ( TemTime2 > 4294967295 || TemTime2 < 60 )
{
return 2;
}
if ( TemTime2 < TemTime1 )
{
return 3;
}
return true;
}
function disableRASrv()
{
with (document.forms[1])
{
setCheck('radvdSrvType', 0);
setDisplay('RAAssConnection', 0);
setDisplay('RadvdPrefixCfg', 0);
}
}
function enableRASrv(formLoad)
{
with (document.forms[1])
{
setCheck('radvdSrvType', 1);
setDisplay('RAAssConnection', 1);
if ("Normal" == RAUseWan.value)
{
setDisplay('RadvdPrefixCfg', 1);
}
}
}

function CheckRAPrefix(Prefix, type)
{
var IpAddress1 = Prefix.split(':');
var IpAddress3 = Prefix;
if (1 == type && '' == IpAddress3)
{
return true;
}
if (true != isGlobalIpv6Address(IpAddress3))
{
return false;
}
return true;
}
function CheckRATime(Time1,Time2)
{
var TemTime1 = Time1;
var TemTime2 = Time2;
if (TemTime1.length > 10 || '' == TemTime1)
{
return 1;
}
if (TemTime2.length > 10 || '' == TemTime2)
{
return 2;
}
if (true != isPlusInteger(TemTime1))
{
return 1;
}
if (true != isPlusInteger(TemTime2))
{
return 2;
}
TemTime1 = parseInt(Time1);
TemTime2 = parseInt(Time2);
if (TemTime1 > 4294967295 || TemTime1 < 600)
{
return 1;
}
if (TemTime2 > 4294967295 || TemTime2 < 600)
{
return 2;
}
if (TemTime2 < TemTime1)
{
return 3;
}
return true;
}
function RAIntervalValue(vMin, vMax)
{
	var MinInterval = vMin;
	var MaxInterval = vMax;
	if ('' == MinInterval)
	{
		return 1;
	}
	if ('' == MaxInterval)
	{
		return 2;
	}
	MinInterval = parseFloat(vMin);
	MaxInterval = parseFloat(vMax);
	if (MinInterval > (MaxInterval*0.75))
	{	
		var temp = MaxInterval*0.75;
		alert("RA最小间隔:" + vMin + "不应大于0.75倍RA最大间隔:" + temp.toString());
		return 1;
	}
	if(MinInterval < 3)
		return 1;
	if (MaxInterval > 1800 || MaxInterval < 4)
	{
		return 2;
	}
	if (MaxInterval < MinInterval)
	{
		return 3;
	}
	return true;
}

function RACheckForm(type)
{
	with (document.forms[1])
	{
		if (1 != getCheckVal('radvdSrvType') || "Normal" != RAUseWan.value)
		{
			return true;
		}
		if (true != CheckRAPrefix(radvdPrefix.value,0))
		{
			alert("前缀\"" + radvdPrefix.value + "\"不是合法的IPv6地址");
			return false;
		}
		if (true != CheckLength(RAPrefixLen.value))
		{
			alert("前缀长度:\"" + RAPrefixLen.value + "\"不合法");
			return false;
		}
		switch (CheckRATime(RAt1.value,RAt2.value))
		{
		case 1 :
		{
			alert("时间:\"" + RAt1.value + "\"不合法");
			return false;
		}
		case 2 :
		{
			alert("时间:\"" + RAt2.value + "\"不合法");
			return false;
		}
		case 3 :
		{
			alert("有效寿命:" + RAt2.value + "小于首选寿命:" + RAt1.value);
			return false;
		}
		default:
		break;
		}
		switch (RAIntervalValue(RACycMin.value, RACycMax.value)){
			case 1 :
			{
				alert("RA最小间隔:\"" + RACycMin.value + "\"不合法");
				return false;
			}
			case 2 :
			{
				alert("RA最大间隔:\"" + RACycMax.value + "\"不合法");
				return false;
			}
			case 3 :
			{
				alert("RA最大间隔:" + RACycMax.value + "小于RA最小间隔:" + RACycMin.value);
				return false;
			}
			default:
				break;
		}
	}
	return true;
}
function AddRASubmitParam(Form)
{
with (document.forms[1])
{
/*Form.addParameter('y.RouterAdvertisementEnable', getCheckVal('radvdSrvType'));
if (1 == getCheckVal('radvdSrvType'))
{
Form.addParameter('y.UseAllocatedWAN', getValue('RAUseWan'));
Form.addParameter('y.AssociatedConnection', getValue('radvdPrefix'));
}
if (1 == getCheckVal('radvdSrvType') && "Normal" == RAUseWan.value)
{
Form.addParameter('y.Prefix', getValue('radvdPrefix'));
Form.addParameter('y.PrefixLength', getValue('RAPrefixLen'));
Form.addParameter('y.Preferredlifetime', getValue('RAt1'));
Form.addParameter('y.Validlifetime', getValue('RAt2'));
}
var RequestFile = '/html/network/dhcp.asp';
var url = 'setcfg.cgi?'
+ 'y=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_CT-COM_SLAAC'
+ '&RequestFile=' + RequestFile;
Form.setAction(url);*/
}
}
function RASubmit()
{
if (RACheckForm() == true)
{
//var Form = new webSubmitForm();
//AddRASubmitParam(Form);
//Form.submit();
document.forms[1].RA_Flag.value = "1";
document.forms[1].submit();

}
}
function disableDhcpSrv()
{
with (document.forms[0])
{
dhcpSrvType[1].checked = false;
dhcpEthStart.disabled = 1;
dhcpEthEnd.disabled = 1;
enableRelay.disabled = 1;
IpPoolIndex.disabled = 1;
dhcpEthStartFrag.disabled = 1;
dhcpEthEndFrag.disabled = 1;
dhcpLeasedTimeFrag.disabled = 1;
setDisplay("DhcprelayIP", 0);
setDisplay("dhcpstartaddr",0);
setDisplay("dhcpendaddr",0);
setDisplay("dhcplease",0);
}
}
function enableDhcpSrv(formLoad)
{
with (document.forms[0])
{
//dhcpSrvType[1].checked = true;
dhcpEthStart.disabled = 0;
dhcpEthEnd.disabled = 0;
/*if (formLoad == 0)
setDhcpAddresses(ethIpAddress.value);
else
{*/
dhcpEthStart.value = dhcpStart;
dhcpEthEnd.value = dhcpEnd;
//}

setDisable('dhcpLeasedTimeFrag', 0);
setDisable('IpPoolIndex',0);
setDisable('dhcpEthStartFrag',0);
setDisable('dhcpEthEndFrag',0);
setDisable('enableRelay',0);
setDisplay("DhcprelayIP", 0);
setDisplay("dhcpstartaddr",1);
setDisplay("dhcpendaddr",1);
setDisplay("dhcplease",1);
}
}
function disableDhcpSrvRelay()
{
with (document.forms[0])
{
dhcpSrvAddr.disabled = 1;
}
}
function enableDhcpSrvRelay()
{
with ( document.forms[0] )
{
if (dhcpEthStart.value != "")
dhcpSrvAddr.value = dhcpEthStart.value;
else
dhcpSrvAddr.value = dhcpStart;
dhcpSrvAddr.disabled = 0;
}
}
function typeClick()
{
with (document.forms[0])
{
	if ( natEnbl == '1' )
	{
		if ( dhcpSrvType[0].checked == true )
		{
			disableDhcpSrv();
		}
		else if( dhcpSrvType[1].checked == true )
		{
			enableDhcpSrv(0);
		}
		else
		{
			disableDhcpSrv();
			setDisplay("DhcprelayIP",1);
			getElById('dhcpRelayServerIP').focus();
		}
	}
	else
	{
		if (dhcpSrvType[0].checked == true)
		{
			disableDhcpSrv();
			disableDhcpSrvRelay();
		}
		else if (dhcpSrvType[1].checked == true)
		{
			enableDhcpSrv(0);
			disableDhcpSrvRelay();
		}
	}
}
}
function LoadFrame()
{
with (document.forms[0])
{
	dhcpSecPortRelay();
	IPV4_Flag.value = "0";
	typeClick();
	IpPoolIndexChange();
/*ethIpAddress.value = oldAddrPrimary;
ethSubnetMask.value = oldMaskPrimary;
if (proto == 'Bridge')
{
setVisible('dhcpInfo',0);
}
else
{
setVisible('dhcpInfo',1);
if (natEnbl == '1')
{
if (dhcpEnbl == '1')
{
enableDhcpSrv(1);
}
else
{
dhcpSrvType[0].checked = true;
disableDhcpSrv();
}
setCheck('enablePortRelay', dhcpPortRelayEnable);
if (dhcpPortRelayEnable == 1)
{
setDisplay('portrelayInfo', 1);
secPortRelayStr();
}
else
{
setDisplay('portrelayInfo', 0);
}
}
else
{
if (dhcpEnbl == '1')
{
enableDhcpSrv(1);
disableDhcpSrvRelay();
}
else
{
dhcpSrvType[0].checked = true;
disableDhcpSrv();
disableDhcpSrvRelay();
}
}
IpPoolIndex.value = poolIndex;
if (poolIndex == '1')
{
dhcpEthStartFrag.value = dhcpStart1_1;
dhcpEthEndFrag.value = dhcpEnd1_1;
setCheck('enableRelay',enblrelay_stb);
}
else if (poolIndex == '2')
{
dhcpEthStartFrag.value = dhcpStart1_2;
dhcpEthEndFrag.value = dhcpEnd1_2;
setCheck('enableRelay',enblrelay_phone);
}
else if (poolIndex == '3')
{
dhcpEthStartFrag.value = dhcpStart1_3;
dhcpEthEndFrag.value = dhcpEnd1_3;
setCheck('enableRelay',enblrelay_camera);
}
else if(poolIndex == '4')
{
dhcpEthStartFrag.value = dhcpStart1_4;
dhcpEthEndFrag.value = dhcpEnd1_4;
setCheck('enableRelay',enblrelay_computer);
}
if (dhcpLease != 60 && dhcpLease != 3600
&& dhcpLease != 86400 && dhcpLease != 604800)
{
var opt = addOption('dhcpLeasedTimeFrag', '' , dhcpLease,
parseInt(dhcpLease/60) + '分钟');
}
setSelect('dhcpLeasedTimeFrag', dhcpLease);;
}*/
}
with (document.forms[1])
{
	RA_Flag.value = "0";
	RAUseWanChange();
	RATypeClick();
/*if (1 == radvdEnbl)
{
enableRASrv(1);
}
else
{
setCheck("radvdSrvType",0);
disableRASrv();
}
setSelect('RAUseWan', RAUseAllc);
RAUseWanChange();
radvdPrefix.value = RAPrefix;
RAPrefixLen.value  = RAPrelen;
RAt1.value = RATime1;
RAt2.value = RATime2;*/
}
with (document.forms[2])
{
	Dhcp1_Flag.value = "0";
	typev6Click();
/*if (1 == dhcpEnb6l)
{
setCheck("dhcp6SrvType",1);
setDisplay('AssConnection',1);
}
else
{
setCheck("dhcp6SrvType",0);
setDisplay('AssConnection',0);
}*/
//setSelect('UseWan', UseAllc);
	UseWanChange();
/*if (0 != Prefix)
{
dhcpPrefix.value = Prefix;
}
if (0 != Prelen)
{
PrefixLen.value  = Prelen;
}
if (0 != Time1)
{
t1.value = Time1;
}
if (0 != Time2)
{
t2.value = Time2;
}
DnsSrvOne.value = DnsOne;
DnsSrvTwo.value = DnsTwo;
DomainNameText.value = Domana;*/
}
}
function isEndGTEStart(EndIp, StartIp)
{
addrEnd = EndIp.split('.');
addrStart = StartIp.split('.');
E = parseInt(addrEnd[3]) + 1;
S = parseInt(addrStart[3]) + 1;
if (E < S)
return false;
return true;
}
function ipIsInIP(subStartIp, subEndIp, StartIp, EndIp)
{
addrEnd = EndIp.split('.');
addrStart = StartIp.split('.');
subAddrEnd = subEndIp.split('.');
subAddrStart = subStartIp.split('.');
for (i = 0; i < 4; i++)
{
seg = parseInt(addrEnd[i]);
subSeg = parseInt(subAddrEnd[i]);
if (subSeg > seg)
return false;
seg = parseInt(addrStart[i]);
subSeg = parseInt(subAddrStart[i]);
if ((subSeg > 0) && (subSeg < seg))
return false;
}
return true;
}
function  ipConfilt( StartIp1 , EndIp1, StartIp2, EndIp2)
{
if (!( isValidIpAddress(StartIp1)
&& isValidIpAddress(EndIp1)
&&isValidIpAddress(StartIp2)
&&isValidIpAddress(EndIp2)))
{
return false;
}
var StartIp1A = StartIp1.split('.');
var EndIp1A= EndIp1.split('.');
var StartIp2A = StartIp2.split('.');
var EndIp2A = EndIp2.split('.');
var StarIp1Value = parseInt(StartIp1A[3]);
var  EndIp1Value = parseInt(EndIp1A[3]);
var StarIp2Value = parseInt(StartIp2A[3]);
var EndIp2Value = parseInt(EndIp2A[3]);
if ((StarIp1Value >EndIp2Value)  || (EndIp1Value <StarIp2Value))
{
return false;
}
return true;
}
function CheckForm(type)
{
with (document.forms[0])
{
if (isValidIpAddress(ethIpAddress.value) == false)
{
alert('地址 "' + ethIpAddress.value + '" 无效。');
return false;
}
if (isValidSubnetMask(ethSubnetMask.value) == false)
{
alert('子网掩码 "' + ethSubnetMask.value + '" 无效。');
return false;
}
if (isBroadcastIp(ethIpAddress.value, ethSubnetMask.value) == true)
{
alert('地址 "' + ethIpAddress.value + '" 无效。');
return false;
}
if (dhcpSrvType[1].checked == true)
{
if (dhcpEthStart.value == '0.0.0.0')
{
alert('起始IP地址 "' + dhcpEthStart.value + '" 是无效IP地址。');
return false;
}
if (dhcpEthEnd.value == '0.0.0.0')
{
alert('结束IP地址  "' + dhcpEthEnd.value + '" 是无效IP地址。');
return false;
}
if ((dhcpEthStart.value != '0.0.0.0')&&(dhcpEthEnd.value != '0.0.0.0'))
{
if (isValidIpAddress(dhcpEthStart.value) == false ||
!(isSameSubNet(ethIpAddress.value, ethSubnetMask.value, dhcpEthStart.value, ethSubnetMask.value)))
{
alert('起始IP地址 "' + dhcpEthStart.value + '" 是无效IP地址。');
return false;
}
if (isBroadcastIp(dhcpEthStart.value, ethSubnetMask.value) == true)
{
alert('错误! 起始IP地址 "' + dhcpEthStart.value + '" 无效。');
return false;
}
if (isValidIpAddress(dhcpEthEnd.value) == false ||
!(isSameSubNet(ethIpAddress.value, ethSubnetMask.value, dhcpEthEnd.value, ethSubnetMask.value)))
{
alert('结束IP地址 "' + dhcpEthEnd.value + '" 是无效IP地址。');
return false;
}
if (isBroadcastIp(dhcpEthEnd.value, ethSubnetMask.value) == true)
{
alert('错误!结束IP地址 "' + dhcpEthEnd.value + '" 无效。');
return false;
}
if (!(isEndGTEStart(dhcpEthEnd.value, dhcpEthStart.value)))
{
alert("结束IP地址必须等于或大于起始IP地址。");
return false;
}
if (dhcpEthStart.value == ethIpAddress.value)
{
alert("起始IP地址不能与路由IP地址相同。");
return false;
}
if (dhcpEthEnd.value == ethIpAddress.value)
{
alert("结束IP地址不能与路由IP地址相同。");
return false;
}
}
if ((dhcpEthStartFrag.value == '')
&&(dhcpEthEndFrag.value == '')
&&(dhcpLeasedTimeFrag.value ==''))
{
dhcpEthStartFrag.value = '0.0.0.0';
dhcpEthEndFrag.value = '0.0.0.0';
dhcpLeasedTimeFrag.value ='86400';
}
else
{
if ((dhcpEthStartFrag.value != '0.0.0.0')&&(dhcpEthEndFrag.value != '0.0.0.0'))
{
if (isValidIpAddress(dhcpEthStartFrag.value) == false ||
!(isSameSubNet(ethIpAddress.value, ethSubnetMask.value, dhcpEthStartFrag.value, ethSubnetMask.value)))
{
alert('IP池的起始IP地址 "' + dhcpEthStartFrag.value + '"  是无效IP地址。');
return false;
}
if (isBroadcastIp(dhcpEthStartFrag.value, ethSubnetMask.value) == true)
{
alert('错误!IP池的 起始IP地址 "' + dhcpEthStartFrag.value + '" 无效。');
return false;
}
if (isValidIpAddress(dhcpEthEndFrag.value) == false ||
!(isSameSubNet(ethIpAddress.value, ethSubnetMask.value, dhcpEthEndFrag.value, ethSubnetMask.value)))
{
alert('IP池的结束IP地址 "' + dhcpEthEndFrag.value + '"  是无效IP地址。');
return false;
}
if (isBroadcastIp(dhcpEthEndFrag.value, ethSubnetMask.value) == true)
{
alert('错误!IP池的结束IP地址 "' + dhcpEthEndFrag.value + '"  无效。');
return false;
}
if (!(isEndGTEStart(dhcpEthEndFrag.value, dhcpEthStartFrag.value)))
{
alert("IP池的结束IP地址必须大于或等于IP池的起始IP地址。");
return false;
}
if (dhcpEthStartFrag.value == ethIpAddress.value)
{
alert("IP池的起始IP地址不能与路由IP地址相同。");
return false;
}
if (dhcpEthEndFrag.value == ethIpAddress.value)
{
alert("IP池的结束IP地址不能与路由IP地址相同。");
return false;
}
}
else if((dhcpEthStartFrag.value == '0.0.0.0')&&(dhcpEthEndFrag.value == '0.0.0.0'))
{
}
else
{
alert('IP池的起始IP地址和结束IP地址当其中一个不为0时另一个也不能为0。');
return false;
}
}
if (IpPoolIndex.value == "STB")
{
/*if (!ipIsInIP(dhcpEthStartFrag.value, dhcpEthEndFrag.value, dhcpEthStart.value , dhcpEthEnd.value))
{
alert("IP段STB在主要的IP池之外。");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_2, dhcpEnd1_2))
{
alert("IP段STB与IP 段Phone 冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_3, dhcpEnd1_3))
{
alert("IP段STB与IP 段Camera 冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_4, dhcpEnd1_4))
{
alert("IP段STB与IP 段Computer 冲突!");
return false;
}*/
}
else if (IpPoolIndex.value == "Phone")
{
/*if (!ipIsInIP(dhcpEthStartFrag.value, dhcpEthEndFrag.value, dhcpEthStart.value , dhcpEthEnd.value))
{
alert("IP段Phone在主要的IP池之外。");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_1, dhcpEnd1_1))
{
alert("IP段Phone 与IP 段STB 冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_3, dhcpEnd1_3))
{
alert("IP段Phone 与IP 段Camera冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_4, dhcpEnd1_4))
{
alert("IP段Phone 与IP 段Computer 冲突!");
return false;
}*/
}
else if(IpPoolIndex.value == "Camera")
{
/*if(!ipIsInIP(dhcpEthStartFrag.value, dhcpEthEndFrag.value, dhcpEthStart.value , dhcpEthEnd.value))
{
alert("IP段Camera在主要的IP池之外。");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_1, dhcpEnd1_1))
{
alert("IP段Camera 与IP 段STB 冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_2, dhcpEnd1_2))
{
alert("IP段Camera 与IP 段Phone 冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_4, dhcpEnd1_4))
{
alert("IP段Camera 与IP 段Computer 冲突!");
return false;
}*/
}
else if(IpPoolIndex.value == "Computer")
{
/*if(!ipIsInIP(dhcpEthStartFrag.value, dhcpEthEndFrag.value, dhcpEthStart.value , dhcpEthEnd.value))
{
alert("IP段Computer在主要的IP池之外。");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_1, dhcpEnd1_1))
{
alert("IP段Computer 与IP 段STB 冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_2, dhcpEnd1_2))
{
alert("IP段Computer 与IP 段Phone 冲突!");
return false;
}
else if (ipConfilt (dhcpEthStartFrag.value, dhcpEthEndFrag.value,dhcpStart1_3, dhcpEnd1_3))
{
alert("IP段Computer 与IP 段Camera 冲突!");
return false;
}*/
}
}
else
{
if ( natEnbl == '1' )
{
}
else
{
}
}
oldAddrPrimary = ethIpAddress.value;
oldMaskPrimary = ethSubnetMask.value;
}
with (document.forms[2])
{
switch (type)
{
case 3:
{
if (1 != getCheckVal('dhcp6SrvType') || "Normal" != UseWan.value)
{
return true;
}
if ("" != dhcpPrefix.value)
{
if (true != CheckPrefix(dhcpPrefix.value,0))
{
alert('前缀 : "' + dhcpPrefix.value +'" 不是合法的IPv6地址 !');
return false;
}
}
if ("" != dhcpPrefix.value || ("" != PrefixLen.value && 0 != PrefixLen.value))
{
if (true != CheckLength(PrefixLen.value))
{
alert('前缀长度 : "' + PrefixLen.value + '" 不合法 !');
return false;
}
}
if ("" != dhcpPrefix.value || ("" != t1.value && 0 != t1.value) || ("" != t2.value && 0 != t2.value))
{
switch (CheckTime(t1.value,t2.value))
{
case 1 :
{
alert('时间 : "' + t1.value + '" 不合法 !');
return false;
}
case 2 :
{
alert('时间 : "' + t2.value + '" 不合法 !');
return false;
}
case 3 :
{
alert('有效寿命 : ' + t2.value + '小于' + '首选寿命 : ' + t1.value);
return false;
}
default:
break;
}
}
if ("" == DnsSrvOne.value && "" != DnsSrvTwo.value)
{
alert('前缀 : "' + DnsSrvOne.value + '" 不是合法的IPv6地址 !');
return false;
}
if (true != CheckPrefix(DnsSrvOne.value,1))
{
alert('前缀 : "' + DnsSrvOne.value + '" 不是合法的IPv6地址 !');
return false;
}
if (true != CheckPrefix(DnsSrvTwo.value,1))
{
alert('前缀 : "' + DnsSrvTwo.value + '" 不是合法的IPv6地址 !');
return false;
}
break;
}
case 1:
{
}
break;
case 2:
{
}
break;
default:
break;
}
}
return true;
}
function setDhcpAddresses(lanIp)
{
with (document.forms[0])
{
if (isValidIpAddress(lanIp) == false)
{
alert('地址 "' + lanIp + '" 是无效IP地址。');
return;
}
var lanMask = ethSubnetMask.value;
var Start = dhcpEthStart.value;
var End = dhcpEthEnd.value;
addrParts = lanIp.split('.');
maskParts = lanMask.split('.');
if ( addrParts.length != 4 )
return false;
if ( maskParts.length != 4 )
return false;
t1 = parseInt(addrParts[3]) + 1;
t2 = 254 - parseInt(maskParts[3]);
if (255 <= t2)
{
t2 = 254;
}
if (dhcpEthStart.value != '' && t1 >= 255)
{
alert("要提供DHCP服务IP地址的最后一位必须小于254。");
return false;
}
dhcpEthStart.value = dhcpEthEnd.value = "";
for (i = 0; i < 3; i++)
{
dhcpEthStart.value = dhcpEthStart.value + addrParts[i] + ".";
dhcpEthEnd.value = dhcpEthEnd.value + addrParts[i] + ".";
}
dhcpEthStart.value = dhcpEthStart.value + t1;
dhcpEthEnd.value = dhcpEthEnd.value + t2;
if ( dhcpSrvType[1].checked == true
&& ((oldAddrPrimary != ethIpAddress.value)||(oldMaskPrimary != ethSubnetMask.value)))
{
alert('警告：该操作将改变IP地址池!');
changeAddrFlag = true;
IpPoolIndex.value = "STB";
dhcpEthStartFrag.value = '0.0.0.0';
dhcpEthEndFrag.value= '0.0.0.0';
poolIndex = '1';
dhcpStart1_1 = '0.0.0.0';
dhcpEnd1_1 = '0.0.0.0';
dhcpStart1_2 = '0.0.0.0';
dhcpEnd1_2 = '0.0.0.0';
dhcpStart1_3 = '0.0.0.0';
dhcpEnd1_3 = '0.0.0.0';
dhcpStart1_4 = '0.0.0.0';
dhcpEnd1_4 = '0.0.0.0';
}
}
}
function IpPoolIndexChange()
{
with ( document.forms[0] ) {
var index=IpPoolIndex.value;
if(index == 'STB')
{
dhcpEthStartFrag.value = dhcpStart1_1;
dhcpEthEndFrag.value = dhcpEnd1_1;
setCheck('enableRelay',enblrelay_stb);
}
else if(index == 'Phone')
{
dhcpEthStartFrag.value = dhcpStart1_2;
dhcpEthEndFrag.value = dhcpEnd1_2;
setCheck('enableRelay',enblrelay_phone);
}
else if(index == 'Camera')
{
dhcpEthStartFrag.value = dhcpStart1_3;
dhcpEthEndFrag.value = dhcpEnd1_3;
setCheck('enableRelay',enblrelay_camera);
}
else if(index == 'Computer')
{
dhcpEthStartFrag.value = dhcpStart1_4;
dhcpEthEndFrag.value = dhcpEnd1_4;
setCheck('enableRelay',enblrelay_computer);
}
}
}

function getPortRelayStr(relayinfstr, pr_str)
{
if (getCheckVal(pr_str) == 1)
{
if (relayinfstr == '')
{
return  getValue(pr_str);
}
else
{
return  ',' + getValue(pr_str);
}
}
return '';
}
function secPortRelayStr()
{
/*var PortRelayArray = dhcpst[0].PortRelayStr.split(',');
var InsId;
var handle;
for (i = 0; i < PortRelayArray.length; i++)
{
InsId = PortRelayArray[i].substr(PortRelayArray[i].lastIndexOf('.') + 1)
if (PortRelayArray[i].indexOf('LANEthernetInterfaceConfig') > 0)
{
handle =  'setCheck(' + '\'PortRelay_lan' + InsId + '\'' + ',1)';
}
else if (PortRelayArray[i].indexOf('WLANConfiguration') > 0)
{
handle =  'setCheck(' + '\'PortRelay_ssid' + InsId + '\'' + ',1)';
}
else
{
break;
}
eval(handle);
}*/
}
function AddSubmitParam(Form,type)
{
/*if (type == 1 || type == 2 || type == 3)
{
with (document.forms[2])
{
switch(type)
{
case 3:
{
Form.addParameter('y.DHCPServerEnable',getCheckVal('dhcp6SrvType'));
if ( 1 == getCheckVal('dhcp6SrvType') )
{
Form.addParameter('y.UseAllocatedWAN',getValue('UseWan'));
}
if ( 1 == getCheckVal('dhcp6SrvType') && "Normal" == UseWan.value )
{
Form.addParameter('y.Prefix',getValue('dhcpPrefix'));
if ( '' == getValue('PrefixLen') )
{
Form.addParameter('y.PrefixLength',0);
}
else
{
Form.addParameter('y.PrefixLength',getValue('PrefixLen'));
}
if ( '' == getValue('t1') )
{
Form.addParameter('y.Preferredlifetime',0);
}
else
{
Form.addParameter('y.Preferredlifetime',getValue('t1'));
}
if ( '' == getValue('t2') )
{
Form.addParameter('y.Validlifetime',0);
}
else
{
Form.addParameter('y.Validlifetime',getValue('t2'));
}
var DnsStr = '';
DnsStr = '';
if (DnsSrvOne.value != '')
{
DnsStr = getValue('DnsSrvOne');
if (DnsSrvTwo.value != '')
{
DnsStr = getValue('DnsSrvOne') + ',' + getValue('DnsSrvTwo');
}
}
else if (DnsSrvTwo.value != '') 
{
DnsStr = getValue('DnsSrvTwo');
}
Form.addParameter('y.DNSServer',DnsStr);
Form.addParameter('y.DomainName',getValue('DomainNameText'));
}
var RequestFile = '/html/network/dhcp.asp';
var url = 'setcfg.cgi?'
+ 'y=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_CT-COM_DHCPv6'
+ '&RequestFile=' + RequestFile;
Form.setAction(url);
break;
}
case 1:
break;
case 2:
break;
default:
return;
}
}
}
else
{
Form.addParameter('x.IPInterfaceIPAddress',getValue('ethIpAddress'));
Form.addParameter('x.IPInterfaceSubnetMask',getValue('ethSubnetMask'));
Form.usingPrefix('y');
with (document.forms[0])
{
var isRelay = getCheckVal('enableRelay');
Form.addParameter('DHCPServerEnable',getRadioVal('dhcpSrvType'));
if (1 == getRadioVal('dhcpSrvType'))
{
if (IpPoolIndex.value == '1')
{
Form.addParameter('X_CT-COM_STB-MinAddress',dhcpEthStartFrag.value);
Form.addParameter('X_CT-COM_STB-MaxAddress',dhcpEthEndFrag.value);
Form.addParameter('X_CT-COM_STB-DHCPRelay',isRelay);
if (changeAddrFlag == true)
{
Form.addParameter('X_CT-COM_Phone-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Phone-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Camera-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Camera-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Computer-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Computer-MaxAddress','0.0.0.0');
}
}
else if (IpPoolIndex.value == '2')
{
Form.addParameter('X_CT-COM_Phone-MinAddress',dhcpEthStartFrag.value);
Form.addParameter('X_CT-COM_Phone-MaxAddress',dhcpEthEndFrag.value);
Form.addParameter('X_CT-COM_Phone-DHCPRelay',isRelay);
if (changeAddrFlag == true)
{
Form.addParameter('X_CT-COM_STB-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_STB-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Camera-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Camera-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Computer-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Computer-MaxAddress','0.0.0.0');
}
}
else if (IpPoolIndex.value == '3')
{
Form.addParameter('X_CT-COM_Camera-MinAddress',dhcpEthStartFrag.value);
Form.addParameter('X_CT-COM_Camera-MaxAddress',dhcpEthEndFrag.value);
Form.addParameter('X_CT-COM_Camera-DHCPRelay',isRelay);
if (changeAddrFlag == true)
{
Form.addParameter('X_CT-COM_STB-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_STB-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Phone-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Phone-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Computer-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Computer-MaxAddress','0.0.0.0');
}
}
else if (IpPoolIndex.value == '4')
{
Form.addParameter('X_CT-COM_Computer-MinAddress',dhcpEthStartFrag.value);
Form.addParameter('X_CT-COM_Computer-MaxAddress',dhcpEthEndFrag.value);
Form.addParameter('X_CT-COM_Computer-DHCPRelay',isRelay);
if (changeAddrFlag == true)
{
Form.addParameter('X_CT-COM_STB-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_STB-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Phone-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Phone-MaxAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Camera-MinAddress','0.0.0.0');
Form.addParameter('X_CT-COM_Camera-MaxAddress','0.0.0.0');
}
}
Form.addParameter('MinAddress',getValue('dhcpEthStart'));
Form.addParameter('MaxAddress',getValue('dhcpEthEnd'));
Form.addParameter('DHCPLeaseTime',getValue('dhcpLeasedTimeFrag'));
Form.addParameter('DHCPServerEnable',getRadioVal('dhcpSrvType'));
}
Form.addParameter('X_CT_COM_Port-DHCPRelayEnable',getCheckVal('enablePortRelay'));
var relayinfstr = '';
if (getCheckVal('enablePortRelay') == 1)
{
for (i = 1; i <= 4; i++)
{
relayinfstr = relayinfstr + getPortRelayStr(relayinfstr,'PortRelay_lan'+i);
relayinfstr = relayinfstr + getPortRelayStr(relayinfstr,'PortRelay_ssid'+i);
}
}
Form.addParameter('X_CT_COM_RelayLanInterface', relayinfstr);
Form.endPrefix();
top.lanDevIndex = IpPoolIndex.value;
}
var RequestFile = 'html/network/dhcp.asp';
if (getValue('ethIpAddress') != host[0][1])
{
RequestFile = '';
}
var url = 'setcfg.cgi?'
+ 'y=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement'
+ '&x=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1'
+ '&RequestFile=' + RequestFile;
Form.setAction(url);
}*/
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>IPv4配置</P></TD>
          <TD width=8 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=8>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#LAN侧地址配置" 
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
            <FORM name="ConfigForm" action="/cgi-bin/net-dhcp.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD><B>本地网络配置</B><BR><BR>用户侧的DHCP打开和关闭、租约时间（1分钟、1小时、1天、1周）。<BR>DHCP地址区间设置及与设备类型的映射。<BR>DHCP工作模式（DHCP 
                  Server和DHCP relay）设置。 <BR>
                      <input type="hidden" name="portDHCPRelayFlag" value="<% tcWebApi_get("Dhcpd_Common","PortDHCPRelay","s") %>">
                      <input type="hidden" name="portLan1" value="<% tcWebApi_get("Dhcpd_Common","PortLan1","s") %>">
                      <input type="hidden" name="portLan2" value="<% tcWebApi_get("Dhcpd_Common","PortLan2","s") %>">
                      <input type="hidden" name="portLan3" value="<% tcWebApi_get("Dhcpd_Common","PortLan3","s") %>">
                      <input type="hidden" name="portLan4" value="<% tcWebApi_get("Dhcpd_Common","PortLan4","s") %>">
                      <input type="hidden" name="portSSID1" value="<% tcWebApi_get("Dhcpd_Common","PortSSID1","s") %>">
                      <input type="hidden" name="portSSID2" value="<% tcWebApi_get("Dhcpd_Common","PortSSID2","s") %>">
                      <input type="hidden" name="portSSID3" value="<% tcWebApi_get("Dhcpd_Common","PortSSID3","s") %>">
                      <input type="hidden" name="portSSID4" value="<% tcWebApi_get("Dhcpd_Common","PortSSID4","s") %>">
                      <input type="hidden" name="DeviceDHCPRelayFlag" value="<% tcWebApi_get("Dhcpd_Common","DeviceDHCPRelay","s") %>">
                      <input type="hidden" name="IPV4_Flag" value="0">
                      <input type="hidden" name="EthendIPcount" value="100">
                      <input type="hidden" name="DeviceendIPcount" value="80"> <BR>
                      <script language="JavaScript" type="text/JavaScript">
					  function dhcpSecPortRelay()
					{
						with (document.forms[0])
						{
							if(getCheckVal('enablePortRelay') == 1)
								{
								setDisplay('portrelayInfo', 1);			
								document.ConfigForm.portDHCPRelayFlag.value="Yes";
								}
							else
							{
									setDisplay('portrelayInfo', 0);
									document.ConfigForm.portDHCPRelayFlag.value="No";
							}
						}
					}
					  function checkportcheckboxstate()
					  {
					  	var	vForm = document.ConfigForm;
						
						if(vForm.PortRelay_lan1.checked)
							vForm.portLan1.value = "Yes";
						else vForm.portLan1.value = "No";
						if(vForm.PortRelay_lan2.checked)
							vForm.portLan2.value = "Yes";
						else vForm.portLan2.value = "No";
						if(vForm.PortRelay_lan3.checked)
							vForm.portLan3.value = "Yes";
						else vForm.portLan3.value = "No";
						if(vForm.PortRelay_lan4.checked)
							vForm.portLan4.value = "Yes";
						else vForm.portLan4.value = "No";
						if(vForm.PortRelay_ssid1.checked)
							vForm.portSSID1.value = "Yes";
						else vForm.portSSID1.value = "No";
						if(vForm.PortRelay_ssid2.checked)
							vForm.portSSID2.value = "Yes";
						else vForm.portSSID2.value = "No";
						if(vForm.PortRelay_ssid3.checked)
							vForm.portSSID3.value = "Yes";
						else vForm.portSSID3.value = "No";
						if(vForm.PortRelay_ssid4.checked)
							vForm.portSSID4.value = "Yes";
						else vForm.portSSID4.value = "No";
					  }
					  function GetEndSubStartCount(EndIp, StartIp)
					  {
					  	var vret;
						addrEnd = EndIp.split('.');
						addrStart = StartIp.split('.');
						E = parseInt(addrEnd[3]) + 1;
						S = parseInt(addrStart[3]);
						vret = E - S;
						return vret;
						
					  }
					  function btnSaveIPv4()
					  {
					  	if(CheckForm(1) == false)
							return false;
						var	vForm = document.ConfigForm;
						vForm.EthendIPcount.value = GetEndSubStartCount(vForm.dhcpEthEnd.value, vForm.dhcpEthStart.value);
						vForm.DeviceendIPcount.value = GetEndSubStartCount(vForm.dhcpEthEndFrag.value, vForm.dhcpEthStartFrag.value);
						
					 	checkportcheckboxstate();
					  
						vForm.IPV4_Flag.value = "1";
						vForm.submit();		
					  }
					  function devicedhcpRelay()
					  {
					  	var	vForm = document.ConfigForm;
						if(vForm.enableRelay.checked)
							vForm.DeviceDHCPRelayFlag.value = "Yes";
						else vForm.DeviceDHCPRelayFlag.value = "No";
					  }
					  </script>
                      <BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=150 height="24">IP地址:</TD>
                      <TD><INPUT maxLength=15 onchange=setDhcpAddresses(this.value) 
                      name="ethIpAddress" value="<% if tcWebApi_get("Lan_Entry","IP","h" ) <> "N/A" then tcWebApi_get("Lan_Entry","IP","s" ) end if %>"></TD></TR>
                    <TR>
                      <TD>子网掩码:</TD>
                      <TD><INPUT maxLength=15 onchange=setDhcpAddresses(ethIpAddress.value) 
                        name="ethSubnetMask" value="<% if tcWebApi_get("Lan_Entry","netmask","h" ) <> "N/A" then tcWebApi_get("Lan_Entry","netmask","s" ) end if %>"></TD></TR></TBODY></TABLE><BR>
                  <DIV id=upnpInfo>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY></TBODY></TABLE></DIV>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD colSpan=2><INPUT id=dhcpSrvType onclick="typeClick()" type=radio value="0" name="dhcpSrvType" <%if tcWebApi_get("Lan_Dhcp","type","h") = "0" then asp_Write("checked") end if %>> &nbsp;&nbsp;禁用DHCP 
                        Server </TD></TR>
                    <TR>
                      <TD><INPUT id="dhcpSrvType" onclick="typeClick()"  type=radio  value="1" name="dhcpSrvType" <%if tcWebApi_get("Lan_Dhcp","type","h") = "1" then asp_Write("checked") end if %>> <NOBR>&nbsp;&nbsp;开启DHCP 
                        Server</NOBR></TD>
                              <TD><INPUT id="dhcpSrvType" onclick="typeClick()"  type=radio  value="2" name="dhcpSrvType" <%if tcWebApi_get("Lan_Dhcp","type","h") = "2" then asp_Write("checked") end if %>> <NOBR>&nbsp;&nbsp;开启DHCP 
                        Relay</NOBR></TD>
                      <TD>&nbsp;&nbsp;&nbsp;</TD>
                      <TD style="display:none"><NOBR>设备类型:&nbsp; <SELECT onchange="IpPoolIndexChange()" size=1 name="IpPoolIndex"> 
                          <OPTION value="STB" <%if tcWebApi_get("Dhcpd_Option60","DeviceStyle","h") = "STB" then asp_Write("selected") end if%>>STB
						  <OPTION value="Phone" <%if tcWebApi_get("Dhcpd_Option60","DeviceStyle","h") = "Phone" then asp_Write("selected") end if%>>Phone
						  <OPTION value="Camera" <%if tcWebApi_get("Dhcpd_Option60","DeviceStyle","h") = "Camera" then asp_Write("selected") end if%>>Camera
						  <OPTION value="Computer" <%if tcWebApi_get("Dhcpd_Option60","DeviceStyle","h") = "Computer" then asp_Write("selected") end if%>>Computer</OPTION>
						  </SELECT></NOBR></TD></TR>
                    <TR id="dhcpstartaddr">
                      <TD>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;起始IP地址:</TD>
                      <TD><INPUT maxLength=15 name="dhcpEthStart" value="<%if tcWebApi_get("Dhcpd_Common","start","h" ) <> "N/A" then tcWebApi_get("Dhcpd_Common","start","s") end if %>"></TD>
                      <TD>&nbsp;&nbsp;&nbsp;</TD>
                      <TD style="display:none"><INPUT maxLength=15 name="dhcpEthStartFrag" value="<%if tcWebApi_get("Dhcpd_Option60","start","h" ) <> "N/A" then tcWebApi_get("Dhcpd_Option60","start","s") end if %>"></TD></TR>
                    <TR id="dhcpendaddr">
                      <TD>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;结束IP地址:</TD>
                      <TD><INPUT maxLength=15 name="dhcpEthEnd" value="<%if tcWebApi_get("Dhcpd_Common","pool_count","h" ) <> "N/A" then tcWebApi_get("Dhcpd_Common","pool_count","s") end if %>"></TD>
                      <TD>&nbsp;&nbsp;&nbsp;</TD>
                      <TD style="display:none"><INPUT maxLength=15 name="dhcpEthEndFrag" value="<%if tcWebApi_get("Dhcpd_Option60","pool_count","h" ) <> "N/A" then tcWebApi_get("Dhcpd_Option60","pool_count","s") end if %>"></TD></TR>
                    <TR id="dhcplease">
                      <TD><NOBR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;租期:</NOBR></TD>
                      <TD><SELECT size=1 name=dhcpLeasedTimeFrag> 
					  <OPTION value=60 <%if tcWebApi_get("Dhcpd_Common","lease","h") = "60" then asp_Write("selected") elseif tcWebApi_get("Dhcpd_Entry","lease","h") = "N/A" then asp_Write("selected") end if%>>1分钟
					  <OPTION value=3600 <%if tcWebApi_get("Dhcpd_Common","lease","h") = "3600" then asp_Write("selected") end if%>>1小时
					  <OPTION value=86400 <%if tcWebApi_get("Dhcpd_Common","lease","h") = "86400" then asp_Write("selected") end if%>>1天
					  <OPTION value=604800 <%if tcWebApi_get("Dhcpd_Common","lease","h") = "604800" then asp_Write("selected") end if%>>1周
					  </SELECT></TD>
                      <TD>&nbsp;&nbsp;&nbsp;</TD>
                      <TD style="display:none">设备DHCP relay: <INPUT type=checkbox onclick=devicedhcpRelay() name=enableRelay <%if tcWebApi_get("Dhcpd_Common","DeviceDHCPRelay","h") = "Yes" then asp_Write("checked") end if %>>
					  </TD></TR>
					  </TBODY></TABLE>
                  <DIV id=portrelayEnable>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                              <TD style="display:none"> <BR>
                                端口DHCP relay 
					  <INPUT id=enablePortRelay onclick="dhcpSecPortRelay()" type=checkbox name="enablePortRelay" <%if tcWebApi_get("Dhcpd_Common","PortDHCPRelay","h") = "Yes" then asp_Write("checked") end if%>> 
					  </TD></TR></TBODY></TABLE></DIV>
                  <DIV id=portrelayInfo>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD>
                        <DIV id=sec_lan1><INPUT id=PortRelay_lan1 type=checkbox name="PortRelay_lan1" <%if tcWebApi_get("Dhcpd_Common","PortLan1","h") = "Yes" then asp_Write("checked") end if %>> LAN 1 &nbsp; </DIV>
                        <DIV id=sec_ssid1><INPUT id=PortRelay_ssid1 
                        type=checkbox name="PortRelay_ssid1" <%if tcWebApi_get("Dhcpd_Common","PortSSID1","h") = "Yes" then asp_Write("checked") end if %>> SSID 1 </DIV></TD>
                      <TD>
                        <DIV id=sec_lan2><INPUT id=PortRelay_lan2 type=checkbox name="PortRelay_lan2" <%if tcWebApi_get("Dhcpd_Common","PortLan2","h") = "Yes" then asp_Write("checked") end if %>> LAN 2 &nbsp; </DIV>
                        <DIV id=sec_ssid2><INPUT id=PortRelay_ssid2 
                        type=checkbox name="PortRelay_ssid2" <%if tcWebApi_get("Dhcpd_Common","PortSSID2","h") = "Yes" then asp_Write("checked") end if %>> SSID 2 </DIV></TD>
                      <TD>
                        <DIV id=sec_lan3><INPUT id=PortRelay_lan3 type=checkbox name="PortRelay_lan3" <%if tcWebApi_get("Dhcpd_Common","PortLan3","h") = "Yes" then asp_Write("checked") end if %>> LAN 3 &nbsp; </DIV>
                        <DIV id=sec_ssid3><INPUT id=PortRelay_ssid3 
                        type=checkbox name="PortRelay_ssid3" <%if tcWebApi_get("Dhcpd_Common","PortSSID3","h") = "Yes" then asp_Write("checked") end if %>> SSID 3 </DIV></TD>
                      <TD>
                        <DIV id=sec_lan4><INPUT id=PortRelay_lan4 type=checkbox name="PortRelay_lan4" <%if tcWebApi_get("Dhcpd_Common","PortLan4","h") = "Yes" then asp_Write("checked") end if %>> LAN 4 &nbsp; </DIV>
                        <DIV id=sec_ssid4><INPUT id=PortRelay_ssid4 type=checkbox name="PortRelay_ssid4" <%if tcWebApi_get("Dhcpd_Common","PortSSID4","h") = "Yes" then asp_Write("checked") end if %>> SSID 4 
                  </DIV></TD></TR></TBODY></TABLE>
                      </DIV>
                      <P align=center> 
                        <INPUT style="DISPLAY: none" onclick=btnSaveIPv4() type=button value=保存 name=button>
                        <INPUT style="DISPLAY: none" onclick=btnSaveIPv4() type=button value=保存/重启 name=button>
                      </P>
                      <DIV id=DhcprelayIP><NOBR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;远端服务器IP:
                        <INPUT maxLength=15 name="dhcpRelayServerIP" value="<%if tcWebApi_get("DhcpRelay_Entry","Server","h" ) <> "N/A" then tcWebApi_get("DhcpRelay_Entry","Server","s" ) end if %>"></NOBR></P>
                      </DIV></TD>
                <TD width=10>&nbsp;</TD></TR><BR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD align=middle width=307>
				<BUTTON onclick="btnSaveIPv4()"; name=btnApply1 value="Apply">保存</BUTTON></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>IPv6配置</P></TD>
          <TD width=8 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=8 background=/img/panel3.gif>　</TD>
          <TD>
              <FORM name="RAConfigForm" action="/cgi-bin/net-dhcp.asp" method="post">
        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P>如果一个IPv6地址的首选寿命到期了，一般都禁止使用这个地址来启动新的连接，<BR>但这个地址仍可以继续用于现存的连接和输入连接，直到其有效寿命耗尽为止。</P>
                      <BR></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD width=147><B>RA配置</B><BR>
                      <input type="hidden" name="RA_Flag" value="0">
                      <input type="hidden" name="RAEnable_Flag" value="<% tcWebApi_get("Radvd_Entry","Enable","s") %>">
                      <input type="hidden" name="RAMode" value="<% tcWebApi_get("Radvd_Entry","Mode","s") %>">
                      <input type="hidden" name="RAManagedEn_Flag" value="<% tcWebApi_get("Radvd_Entry","ManagedEnable","s") %>">
			<input type="hidden" name="RAOtherEn_Flag" value="<% tcWebApi_get("Radvd_Entry","OtherEnable","s") %>">
                      <script language="JavaScript" type="text/JavaScript">
function RAUseWanChange()
{
	with (document.forms[1])
	{
		if ("Normal" == RAUseWan.value && 1 == getCheckVal('radvdSrvType'))
		{
			setDisplay('RadvdPrefixCfg', 1);
			RAMode.value = "1";
		}
		else
		{
			setDisplay('RadvdPrefixCfg', 0);
			RAMode.value = "0";
		}
	}
}
function RATypeClick()
{
	with (document.forms[1])
	{
		if (getCheckVal('radvdSrvType') == 0)
		{
			disableRASrv();
			RAEnable_Flag.value = "0";
		}
		else
		{
			enableRASrv(1);
			RAEnable_Flag.value = "1";
		}
	}
}
function RAMOClick(flag)
{
	with (document.forms[1])
	{
		if(0 == flag){
			if (getCheckVal('radvdmanageden') == 0)
			{
				RAManagedEn_Flag.value = "0";
			}
			else
			{
				RAManagedEn_Flag.value = "1";
			}
		}
		else if(1 == flag){
			if (getCheckVal('radvdotheren') == 0)
			{
				RAOtherEn_Flag.value = "0";
			}
			else
			{
				RAOtherEn_Flag.value = "1";
			}
		}
	}
}
					  </script></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE>
                    <TBODY>
                    <TR>
                      <TD align=right width=120>SLAAC:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT id=radvdSrvType onclick="RATypeClick();" type=checkbox name="radvdSrvType" <%if tcWebApi_get("Radvd_Entry","Enable","h") = "1" then asp_Write("checked") end if%>>使能
					  </TD></TR></TBODY></TABLE></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=RAAssConnection style="DISPLAY: none">
                  <TABLE>
                    <TBODY>
                    <TR>
                      <TD align=right width=120>配置模式:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><SELECT id=RAUseWan onchange="RAUseWanChange()" name="RAUseWan"> 
				<OPTION value="Normal" <%if tcWebApi_get("Radvd_Entry","Mode","h") = "1" then asp_Write("selected") end if%>>手动配置</OPTION> 
						<OPTION value="UseAllocatedSubnet" <%if tcWebApi_get("Radvd_Entry","Mode","h") = "0" then asp_Write("selected") end if%>>自动配置</OPTION>
						</SELECT></TD></TR></TBODY></TABLE></DIV></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=RadvdPrefixCfg style="DISPLAY: none">
                  <TABLE>
                    <TBODY>
                    <TR id=NamePrefix>
                      <TD align=right width=120>前缀:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=39 onchange="this.value = this.value.toLowerCase( )" name="radvdPrefix" value="<%if tcWebApi_get("Radvd_Entry","PrefixIPv6","h") <> "N/A" then tcWebApi_get("Radvd_Entry","PrefixIPv6","s") end if%>"> <STRONG 
                        style="COLOR: #ff0033">*</STRONG></TD></TR>
                    <TR id=NamePrefixLen>
                      <TD width=120 height="26" align=right>前缀长度:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=2 name="RAPrefixLen" value="<%if tcWebApi_get("Radvd_Entry","Prefixv6Len","h") <> "N/A" then tcWebApi_get("Radvd_Entry","Prefixv6Len","s") end if%>"> <STRONG 
                        style="COLOR: #ff0033">*</STRONG>[16 - 64]</TD></TR>
                    <TR>
                      <TD align=right width=120>首选寿命:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=10 name="RAt1" value="<%if tcWebApi_get("Radvd_Entry","PreferredLifetime","h") <> "N/A" then tcWebApi_get("Radvd_Entry","PreferredLifetime","s") end if%>"> 
                        <STRONG style="COLOR: #ff0033">*</STRONG>[600 - 4294967295 S]</TD></TR>
                    <TR>
                      <TD align=right width=120>有效寿命:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=10 name="RAt2" value="<%if tcWebApi_get("Radvd_Entry","ValidLifetime","h") <> "N/A" then tcWebApi_get("Radvd_Entry","ValidLifetime","s") end if%>"> 
                        <STRONG style="COLOR: #ff0033">*</STRONG>[600 - 4294967295 S]</TD></TR>
					<TR>
                              <TD align=right width=120>&nbsp;&nbsp;&nbsp;</TD>
                              <TD align=left width=307>
							  	<input type="checkbox" onClick="RAMOClick(0);" name="radvdmanageden" id="radvdmanageden" <%if tcWebApi_get("Radvd_Entry","ManagedEnable","h") = "1" then asp_Write("checked") end if%>>开启Managed标志
								<input type="checkbox" onClick="RAMOClick(1);" name="radvdotheren" id="radvdotheren" <%if tcWebApi_get("Radvd_Entry","OtherEnable","h") = "1" then asp_Write("checked") end if%>>开启Other标志 </TD>
                            </TR>
					<TR>
                              <TD align=right width=120>RA最小间隔:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=4 name="RACycMin" value="<%if tcWebApi_get("Radvd_Entry","MinRAInterval","h") <> "N/A" then tcWebApi_get("Radvd_Entry","MinRAInterval","s") end if%>">
                                <STRONG style="COLOR: #ff0033">*</STRONG>[3 - 
                                1350 S]</TD>
                            </TR>
					<TR>
                              <TD align=right width=120>RA最大间隔:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=4 name="RACycMax" value="<%if tcWebApi_get("Radvd_Entry","MaxRAInterval","h") <> "N/A" then tcWebApi_get("Radvd_Entry","MaxRAInterval","s") end if%>">
                                <STRONG style="COLOR: #ff0033">*</STRONG>[4 - 
                                1800 S]</TD>
                            </TR></TBODY></TABLE></DIV></TD></TR>
              <TR>
                <TD colSpan=2><BR></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD align=middle width=307><BUTTON onclick=RASubmit(); 
                  name="btnRASubmit" value="Apply">保存</BUTTON></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=8 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM id=ConfigForm1 name="ConfigForm1" action="/cgi-bin/net-dhcp.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD width=147><B>DHCP配置</B><BR>
                      <input type="hidden" name="Dhcp1_Flag" value="0"> 
                      <input type="hidden" name="EnDHCPServerFlag" value="<% tcWebApi_get("Dhcp6s_Entry","Enable","s") %>">
                      <input type="hidden" name="DHCPSetTypeFlag" value="<% tcWebApi_get("Dhcp6s_Entry","Mode","s") %>">
                      <script language="JavaScript" type="text/JavaScript">
					  function typev6Click()
{
with ( document.forms[2] )
{
if (getCheckVal('dhcp6SrvType') == 0)//unchecked
{
disableDhcp6Srv();
EnDHCPServerFlag.value = "0";
}
else
{
enableDhcp6Srv(1);
EnDHCPServerFlag.value = "1";
}
}
}
 function UseWanChange()
{
	with (document.forms[2])
	{
		if ( ("Normal" == UseWan.value) && (1 == getCheckVal('dhcp6SrvType')) )
		{
			setDisplay('DHCP6SPrefixCfg', 1);
			DHCPSetTypeFlag.value = "1";
		}
		else
		{
			if( "UseAllocatedSubnet" == UseWan.value)
				DHCPSetTypeFlag.value = "0";
			else if( "UseAllocatedOther" == UseWan.value)
				DHCPSetTypeFlag.value = "2";
			setDisplay('DHCP6SPrefixCfg', 0);
		}
	}
}
function btnDhcp1Save()
{
	document.ConfigForm1.Dhcp1_Flag.value = "1";
	document.ConfigForm1.submit();
}
					  </script>
                    </TD>
                  </TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE>
                    <TBODY>
                    <TR>
                      <TD align=right width=120>服务器:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT id=dhcp6SrvType onclick="typev6Click()" type=checkbox name="dhcp6SrvType" <%if tcWebApi_get("Dhcp6s_Entry","Enable","h") = "1" then asp_Write("checked") end if%>>使能</TD></TR></TBODY></TABLE></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=AssConnection style="DISPLAY: none">
                  <TABLE>
                    <TBODY>
                    <TR>
                      <TD align=right width=120>配置模式:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><SELECT id=UseWan  onchange="UseWanChange()" name="UseWan"> 
						<OPTION value="Normal" <%if tcWebApi_get("Dhcp6s_Entry","Mode","h") = "1" then asp_Write("selected") end if%>>手动配置</OPTION> 
						<OPTION value="UseAllocatedSubnet" <%if tcWebApi_get("Dhcp6s_Entry","Mode","h") = "0" then asp_Write("selected") end if%>>自动配置前缀和域名服务器</OPTION> 
						<OPTION  value="UseAllocatedOther" <%if tcWebApi_get("Dhcp6s_Entry","Mode","h") = "2" then asp_Write("selected") end if%>>自动配置域名服务器</OPTION>
						  </SELECT></TD></TR></TBODY></TABLE></DIV></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=DHCP6SPrefixCfg style="DISPLAY: none">
                  <TABLE>
                    <TBODY>
                    <TR>
                      <TD align=right width=120>前缀:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=39 onchange="this.value = this.value.toLowerCase( )" name="dhcpPrefix" value="<%if tcWebApi_get("Dhcp6s_Entry","PrefixIPv6", "h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","PrefixIPv6","s") end if%>"></TD></TR>
                    <TR>
                      <TD align=right width=120>前缀长度:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=2 name="PrefixLen" value="<%if tcWebApi_get("Dhcp6s_Entry","Prefixv6Len", "h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","Prefixv6Len", "s") end if%>">[16 - 64]</TD></TR>
                    <TR>
                      <TD align=right width=120>首选寿命:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=10 name="t1" value="<%if tcWebApi_get("Dhcp6s_Entry","PreferredLifetime", "h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","PreferredLifetime", "s") end if%>">[60 - 4294967295 S]</TD></TR>
                    <TR>
                      <TD align=right width=120>有效寿命:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=10 name="t2" value="<%if tcWebApi_get("Dhcp6s_Entry","ValidLifetime", "h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","ValidLifetime", "s") end if%>">[60 
                        - 4294967295 S]</TD></TR>
                    <TR>
                      <TD align=right width=120>首选DNS服务器:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=39 name="DnsSrvOne" value="<%if tcWebApi_get("Dhcp6s_Entry","DNSserver","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","DNSserver","s") end if%>"></TD></TR>
                    <TR>
                      <TD align=right width=120>备用DNS服务器:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307><INPUT maxLength=39 name="DnsSrvTwo" value="<%if tcWebApi_get("Dhcp6s_Entry","SecDNSserver","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","SecDNSserver","s") end if%>"></TD></TR></TBODY></TABLE></DIV></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=DHCP6SDomainName style="DISPLAY: none">
                  <TABLE>
                    <TBODY>
                    <TR>
                      <TD align=right width=120>Domain 
                      name:&nbsp;&nbsp;&nbsp;</TD>
                      <TD align=left width=307>
					  <INPUT maxLength=16 name=DomainNameText value="<%if tcWebApi_get("Ddns_Entry","MYHOST","h") <> "N/A" then tcWebApi_get("Ddns_Entry","MYHOST","s") end if%>">
					  </TD></TR></TBODY></TABLE></DIV></TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD align=middle width=307><BUTTON onclick="btnDhcp1Save()"; name=btnApply2 value="Apply">保存</BUTTON>
				</TD></TR></TBODY></TABLE></FORM></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center>&nbsp;&nbsp;<!--img src="/img/cancel.gif" width="80" height="23" border="0" id="btnCancel" onClick=""--> 
<!-- InstanceEndEditable --></P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE><BR></BODY></HTML>
