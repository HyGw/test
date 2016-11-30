<html>
<head>
<meta http-equiv="Content-Language" content="zh-cn">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<meta http-equiv=Content-Script-Type content=text/javascript>
<script language="javascript" src="/JS/util.js"></script>
<link rel="stylesheet" href="/JS/stylemain.css" type="text/css">
<script language="JavaScript" type="text/javascript">
var sptUserName = '0'; 
var sysUserName = '1';
var curUserName = "<%tcWebApi_get("WebCurSet_Entry","CurrentAccess","s")%>";
var isCZGDSupport = "<%tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "s")%>";
function FormLoad()
{
with (document.forms[0])
{
	manageEquip_LOG.style.display = "none";
	
if (curUserName == sysUserName)
{
statusEquip.style.display = "";
statusNetwork.style.display = "";
if(isCZGDSupport == 'Yes')
	getElement('statusNetwork_1').innerText = '显示连接信息和PON信息。 连接信息显示网络侧连接状态，各条PVC的IP地址、子网掩码，默认网关，DNS服务器信息。PON信息显示PON的链路连接状态。';
else
getElement('statusNetwork_1').innerText = '显示连接信息和PON信息。 连接信息显示网络侧连接状态，各条PVC的IP地址、子网掩码，默认网关，DNS服务器信息；PON信息显示PON的链路连接状态，链路性能统计，光模块信息。 ';
statusUser.style.display = "";
getElement('statusUser_1').innerText ='<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>显示WLAN接口信息，<% end if %>以太网接口信息<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>和USB接口信息<% end if %>。<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>WLAN接口信息显示无线网络连接状态，SSID_1的认证方式和加密状态等；<% end if %>以太网接口信息显示网关IP地址，MAC地址，每个LAN口的状态；<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>USB接口信息显示各个USB连接设备的信息。 <% end if %>';
statusACS.style.display = "none";
statusACS_1.style.display = "none";
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>
ntwk.style.display = "none";
<% end if %>
var network = getElementByName("ntwk_cfgWan");
for (i = 0; i < network.length; i++)
{
network[i].style.display = "none";
}
ntwk_cfgBind.style.display = "none";
ntwk_cfgBind_1.style.display = "none";
if(isCZGDSupport != 'Yes')
{
ntwk_cfgDHCP.style.display = "none";
ntwk_cfgDHCP_1.style.display = "none";
}
ntwk_cfgWLAN.style.display = "";
ntwk_cfgWLAN_2.style.display = "none";
ntwk_cfgWLAN_3.style.display = "none";
ntwk_cfgWLAN_4.style.display = "none";
ntwk_cfgWLAN_5.style.display = "none";
ntwk_cfgWLAN_7.style.display = "none";
ntwk_cfgWLAN_8.style.display = "none";
ntwk_cfgWLAN_9.style.display = "none";
ntwk_cfgRemoteManage.style.display = "none";
ntwk_cfgRemoteManage_1.style.display = "none";
ntwk_cfgRemoteManage_2.style.display = "none";
ntwk_cfgRemoteManage_3.style.display = "none";
ntwk_cfgRemoteManage_4.style.display = "none";
<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
var v_cfgRemote = '<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>';
if ( v_cfgRemote == '1' )
{
	var item_idx = 1;
	<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %> 
	item_idx ++;
	<% end if %>
	if(isCZGDSupport == 'Yes')
		item_idx ++;
	ntwk.style.display = '';
	ntwk_cfgRemoteManage.style.display = '';
	ntwk_cfgRM01.innerHTML = item_idx + '.远程管理<a name=远程管理 id="远程管理"></a>';
	ntwk_cfgRemoteManage_3.style.display = '';
	ntwk_cfgRM03.innerHTML = '&nbsp;(1).操作';
	ntwk_cfgRemoteManage_4.style.display = '';
	ntwk_cfgRM04.innerHTML = '<ul><li>OLT认证注册</li></ul>';
}
ntwk_cfgQoS.style.display = "none";
ntwk_cfgQoS_1.style.display = "none";
ntwk_cfgSNTP.style.display = "none";
ntwk_cfgSNTP_1.style.display = "none";
ntwk_cfgSNTP_2.style.display = "none";
ntwk_cfgSNTP_3.style.display = "none";
ntwk_cfgSNTP_4.style.display = "none";
ntwk_cfgRoute.style.display = "none";
ntwk_cfgRoute_1.style.display = "none";
ntwk_cfgRoute_2.style.display = "none";
ntwk_cfgRoute_3.style.display = "none";
ntwk_cfgRoute_4.style.display = "none";
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYJXSupported","h") = "Yes" then %>
ntwk_dslimit.style.display = "none";
ntwk_dslimit_1.style.display = "none";
ntwk_dslimit_2.style.display = "none";
<% end if %>
securityURL.style.display = "";
securityFirewall.style.display = "";
getElement('securityFirewall_1').innerHTML ='防火墙设置:安全级设置';
securityMAC.style.display = "";
securityPort.style.display = "none";
securityPort_1.style.display = "none";
securityPort_2.style.display = "none";
securityPort_3.style.display = "none";
securityPort_4.style.display = "none";
appDDNS.style.display = "none";
appDDNS_1.style.display = "none";
appDDNS_2.style.display = "none";
appDDNS_3.style.display = "none";
appDDNS_4.style.display = "none";
if(isCZGDSupport != 'Yes')
{
appAdnat.style.display = "none";
appAdnat_1.style.display = "none";
appAdnat_2.style.display = "none";
appAdnat_3.style.display = "none";
appAdnat_4.style.display = "none";
}
else
{
	appAdnat_2_alg.style.display = "none";
	appAdnat_2_alg_s.style.display = "none";
	appAdnat_2_dmz.style.display = "none";
	appAdnat_2_dmz_s.style.display = "none";
	appAdnat_4_alg.style.display = "none";
	appAdnat_4_dmz.style.display = "none";
}
appUPNP.style.display = "none";
appUPNP_1.style.display = "none";
appUPNP_2.style.display = "none";
appUPNP_3.style.display = "none";
appUPNP_4.style.display = "none";
appVoip.style.display = "none";
appVoip_1.style.display = "none";
appVoip_2.style.display = "none";
appVoip_3.style.display = "none";
appVoip_4.style.display = "none";
appIGMP.style.display = "none";
appIGMP_1.style.display = "none";
appIGMP_2.style.display = "none";
appIGMP_3.style.display = "none";
appIGMP_4.style.display = "none";
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) <> "Yes" then %>
apptitle.style.display = "none";
appDaily.style.display = "none";
appDaily_1.style.display = "none";
appDaily_3.style.display = "none";
<% end if %>
appDaily_5.style.display = "none";
appDaily_6.style.display = "none";
manageUser.style.display = "";
manageUser_1.style.display = "none";
manageUser_2.style.display = "none";
manageUser_3.style.display = "none";
manageEquip_LOG.style.display = "none";
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
manageEquip_USB.style.display = "none";
<% end if %>
if(isCZGDSupport != 'Yes')
{
manageEquip_2.style.display = "none";
}
if(isCZGDSupport == 'Yes')
{
manageSyslog_1_1.style.display = "none";
manageSyslog_1_3.style.display = "none";	
}
else
{
manageSyslog.style.display = "none";
manageSyslog_1.style.display = "none";
}
netDiag.style.display = "none";
networkDiag.style.display = "none";
networkDiag_1.style.display = "none";
<% if TCWebApi_get("WebCustom_Entry","isCTJOYMESupported","h" ) = "Yes" then %>
serviceDiag.style.display = "none";
serviceDiag_1.style.display = "none";
<% end if %>
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>
	getElement("secuiText").innerHTML="二、安全";     
	<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) <> "Yes" then %>
		getElement("manText").innerHTML="三、管理";
	<%else%>
		getElement("appText").innerHTML="三、应用";
		getElement("manText").innerHTML="四、管理";
	<% end if %>
<% end if %>
}
}
}
</script>
</head>
<body onLoad="FormLoad()">
<form name=test>
<table width="100%" border="0" cellspacing="0" cellpadding="0" id=tab name=tab>
<tr><td class="topHelpTitle" colspan="2">帮&nbsp;助&nbsp;文&nbsp;档</td></tr>
<tr><td colspan="2">&nbsp;</td></tr>
<tr><td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>一、状态<a name=状态></a></td></tr>
<tr name=statusEquip id=statusEquip >
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.设备信息<a name=设备信息></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>显示设备型号，设备标识号，硬件版本，软件版本等信息。</li>
</ul>
</td>
</tr>
<tr name=statusNetwork id=statusNetwork >
<td width=20>&nbsp;</td>
<td class="tabdataleft">2.网络侧信息<a name=网络侧信息 id="网络侧信息"></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusNetwork_1>显示连接信息和PON信息。 连接信息显示网络侧连接状态，各条PVC的IP地址、子网掩码，默认网关，DNS服务器信息；PON信息显示PON的链路连接状态，链路性能统计，光模块信息。 </li>
</ul>
</td>
</tr>
<tr name=statusUser id=statusUser>
<td width=20>&nbsp;</td>
<td class="tabdataleft">3.用户侧信息<a name=用户侧信息 id="用户侧信息"></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusUser_1><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>显示WLAN接口信息，以太网接口信息。  WLAN接口信息显示无线网络连接状态，信道，各个SSID的统计信息、SSID、认证方式和加密状态等；以太网接口信息显示网关IP地址，MAC地址，每个LAN口的状态、收发包和字节数。<% else %>以太网接口信息显示网关IP地址，MAC地址，每个LAN口的状态、收发包和字节数。<% end if %></li>
</ul>
</td>
</tr>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
<tr name=statusVoip id=statusVoip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">4.宽带语音信息<a name=宽带语音信息 id="宽带语音信息"></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusVoip_1>宽带语音信息包括业务注册状态和电话号码；业务状态包括已注册和未注册等状态；电话号码显示注册的电话号码。</li>
</ul>
</td>
</tr>
<% end if %>
<tr name=statusACS id=statusACS>
<td width=20>&nbsp;</td>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
<td class="tabdataleft">4.远程管理状态<a name=远程管理状态 id="远程管理状态"></a></td>
<% else %>
<td class="tabdataleft">5.远程管理状态<a name=远程管理状态 id="远程管理状态"></a></td>
<% end if %> 
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusACS_1>显示交互建立情况、业务配置下发状态，交互建立包括主动上报Inform情况以及接受ITMS连接请求情况。</li>
</ul>
</td>
</tr>
<tr name=ntwk id=ntwk>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>二、网络<a name=网络></a></td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft">1.宽带设置<a name=宽带设置></a></td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>Internet连接</li>
</ul></td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>宽带设置:对宽带连接进行相关参数设置,设置完成后,界面上显示相应的状态。</li>
<ul>
<li>WAN连接:单击“新建连接”添加一条WAN连接；单击“删除连接”删除本连接。</li>
<li>启用:让该条PVC生效。</li>
<li id=ntwk_cfgWan_1>启用绑定:可以把各个LAN口、各个SSID和对应的WAN口绑定。</li>
<li>模式:分成Route、Bridge两种模式。Route模式下有三种连接模式DHCP（从ISP处得到一个IP地址）、Static（经ISP配置一个静态的IP给你）、PPPoE。Bridge模式可以将设备配置成介于LAN和ISP之间的网桥设备，它可以使得两个或多个网络的通信就像处在同一LAN物理连接上。</li>
<li>链接方式:包括通过IP方式建立链接和通过PPP方式建立链接两种。根据实际的网络需要选择建立链接的方式。</li>
<li>IP协议版本:设置链接使用的IP版本，通常使用IPv4版本，也可以通过选择IPv6来支持IPv6。如果选择IPv4/v6方式，则同时使用IPv4和IPv6两种IP版本。</li>
<li>PPPoE:ISP将提供帐号，填入包括如下信息:用户名、密码、服务名称，并配置拨号方式。该模式系统将通过PPPoE动态获得IP。 </li>
<li>DHCP:设备将从ISP通过DHCP自动获得IP。 </li>
<li>Static:由ISP配置一个静态的IP地址。需要输入的信息包括:IP地址，子网掩码，首选DNS服务器及备用DNS服务器等，IP地址必须输入正确的格式，4个IP字节之间由&quot;.&quot;号分开(x.x.x.x)，例如:192.168.0.100。</li>
<li id=ntwk_cfgWan_2>支持802.1q、802.1p、NAT、IGMP Proxy。</li>
<li id=ntwk_cfgWan_3>服务类别:UBR Without PCR、UBR With PCR、CBR、Non-Real Time VBR、Realtime VBR。</li>
<li>IPv6 WAN信息获取方式:可以通过DHCP或者SLAAC两种方式动态的获取IPv6的地址，也可以静态设置IPv6的地址，根据具体的网络情况进行设置。</li>
</ul>
</ul></td>
</tr>
<tr name=ntwk_cfgBind id=ntwk_cfgBind>
<td>&nbsp;</td>
<td class="tabdataleft">2.绑定设置<a name=绑定设置 id="绑定设置"></a></td>
</tr>
<tr name=ntwk_cfgBind_1 id=ntwk_cfgBind_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>绑定设置可以设置用户LAN口和WAN口的绑定方式，支持端口绑定和端口+VLAN绑定两种方式。如果是端口绑定，对应端口的数据从绑定的WAN口出去；如果是端口
	+VLAN绑定，对应端口且带绑定VLAN的数据才会从绑定的WAN口出去。端口加VLAN绑定以m1/n1 VLAN对方式配置，其中m1代表用户侧的VLAN，n1代表出接口的VLAN，
	多组VLAN对以分号分开。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgDHCP id=ntwk_cfgDHCP>
<td>&nbsp;</td>
<script language="JavaScript" type="text/javascript">
if((curUserName == sysUserName) && (isCZGDSupport == 'Yes'))
document.write( '<td class="tabdataleft">1.LAN侧地址配置<a name=LAN侧地址配置 id="LAN侧地址配置"></a></td>');
else
document.write( '<td class="tabdataleft">3.LAN侧地址配置<a name=LAN侧地址配置 id="LAN侧地址配置"></a></td>');
</script>
</tr>
<tr name=ntwk_cfgDHCP_1 id=ntwk_cfgDHCP_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IPv4配置:默认为&quot;启用DHCP&quot;。使用该操作,对DHCP相关参数进行设置,设置完成后，界面上显示新的DHCP状态。可以配置租约时间（1分钟、1小时、1天、1周），DHCP工作模式（DHCP 
          Server和DHCP relay）。</li>
<li>IPv6配置:可以配置是否开启对LAN侧的RA服务，让LAN侧设备可以通过RA方式获取IPv6的地址，同时也可以设置让LAN侧通过DHCP方式获取IPv6的地址。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgWLAN id=ntwk_cfgWLAN <% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>style="display:none"<% end if %>>
<td>&nbsp; </td>
<script language="JavaScript" type="text/javascript">
if (curUserName == sptUserName)
{
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>
document.write( '<td class="tabdataleft" >4.WLAN配置<a name=WLAN配置 id="WLAN配置"></a></td>');
<% end if %>
}
else if(curUserName == sysUserName)
{
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>
if(isCZGDSupport == 'Yes')
document.write( '<td class="tabdataleft" >2.WLAN配置<a name=WLAN配置 id="WLAN配置"></a></td>');
else
document.write( '<td class="tabdataleft" >1.WLAN配置<a name=WLAN配置 id="WLAN配置"></a></td>');
<% end if %>
}
</script>
</tr>
<tr <% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>style="display:none"<% end if %>>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li id=ntwk_cfgWLAN_1>使能无线:开启或关闭WLAN</li>
<li id=ntwk_cfgWLAN_2>模式选择:可选择<i><b>802.11b</b></i>,<i><b>802.11g</b></i>,<i><b>802.11n</b></i>,<i><b>802.11b/g混合</b></i>以及<i><b>802.11b/g/n混合</b></i>模式</li>
<li id=ntwk_cfgWLAN_3>信道选择:根据网络设置从列表中选择恰当的信道。为避免信号干扰,应对每个<strong><em>AP</em></strong>分配不同的信道，建议使用自动信道选择模式。</li>
<li id=ntwk_cfgWLAN_4>发送功率强度:设定无线射频模块使用的发射功率。 </li>
<li id=ntwk_cfgWLAN_5>SSID索引: 设定当前所配置的SSID的序号。</li>
<li id=ntwk_cfgWLAN_6>SSID: 设置SSID名称。可以输入任意字符,长度不能超过32个字符,区分大小写。</li>
<li id=ntwk_cfgWLAN_7>速率: 选择无线连接工作速率，其中的自动速率根据信道环境，自动选择合适的最大速率。</li>
<li id=ntwk_cfgWLAN_8>频带宽度:选择无线网络在802.11n模式下的频道带宽。</li>
<li id=ntwk_cfgWLAN_9>保护间隔:选择无线网络在802.11n模式下的保护时间间隔。</li>
<li id=ntwk_cfgWLAN_10>SSID使能:选择是否使能当前的SSID。</li>
<li id=ntwk_cfgWLAN_11>广播取消:是否使能对应SSID的广播取消功能，如果使能，则网关不会对外广播SSID。</li>
<li id=ntwk_cfgWLAN_12>安全设置:可对SSID设置安全参数，网关支持开放系统、WEP、WPA-PSK、WPA2-PSK、WPA-PSK/WPA2-PSK等认证方式和对应的加密方式。</li>
<li id=ntwk_cfgWLAN_13>WPA 预认证共享密钥:设置共享密钥。</li>
<li id=ntwk_cfgWLAN_14>WPA 加密:选择加密方法。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgRemoteManage id=ntwk_cfgRemoteManage>
<td>&nbsp;</td>
<td class="tabdataleft" name='ntwk_cfgRM01' id='ntwk_cfgRM01'><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>5<% else %>4<% end if%>.远程管理<a name=远程管理 id="远程管理"></a></td>
</tr>
<tr name=ntwk_cfgRemoteManage_1 id=ntwk_cfgRemoteManage_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=ntwk_cfgRemoteManage_2 id=ntwk_cfgRemoteManage_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>ITMS服务器设置</li>
<li>OLT认证</li>
</ul></td>
</tr>
<tr name=ntwk_cfgRemoteManage_3 id=ntwk_cfgRemoteManage_3>
<td>&nbsp;</td>
<td class="tabdataleft" name='ntwk_cfgRM03' id='ntwk_cfgRM03'>&nbsp;(2).操作</td>
</tr>
<tr name=ntwk_cfgRemoteManage_4 id=ntwk_cfgRemoteManage_4>
<td>&nbsp;</td>
<td class="tabdataleft" name='ntwk_cfgRM04' id='ntwk_cfgRM04'><ul>
<li>用于设置ITMS服务器的URL或者IP地址，ITMS认证网关的用户名和密码，以及网关认证ITMS的用户名和密码。同时还可以配置网关是否发送周期性上报报文，以及上报报文的周期。另外还可以进行证书认证的启用或禁用。</li>
<li>OLT认证注册</li>
</ul></td>
</tr>
<tr name=ntwk_cfgQoS id=ntwk_cfgQoS>
<td>&nbsp;</td>
<td class="tabdataleft"><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>6<% else %>5<% end if%>.QoS<a name=QoS id="QoS"></a></td>
</tr>
<tr name=ntwk_cfgQoS_1 id=ntwk_cfgQoS_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>QoS 设置（仅电信维护帐号可使用）:出厂时可预置和业务相关的配置模板，使用时根据具体开通业务进行选择和配置。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgSNTP id=ntwk_cfgSNTP>
<td>&nbsp;</td>
<td class="tabdataleft"><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>7<% else %>6<% end if%>.时间管理<a name=时间管理 id="时间管理"></a></td>
</tr>
<tr name=ntwk_cfgSNTP_1 id=ntwk_cfgSNTP_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=ntwk_cfgSNTP_2 id=ntwk_cfgSNTP_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>是否启用</li>
</ul></td>
</tr>
<tr name=ntwk_cfgSNTP_3 id=ntwk_cfgSNTP_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=ntwk_cfgSNTP_4 id=ntwk_cfgSNTP_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul><li>SNTP （网络时间协议） </li>
<ul>
<li>SNTP设置:默认为&quot;<strong>不启用SNTP</strong>&quot;。启用SNTP，对SNTP的相关参数进行设置，设置完成后，界面上显示新的SNTP配置。</li>
<li>服务器即可设置域名也可设置IP地址，如果想选择列表框以外的SNTP服务器，在列表框中选择“Other”，在后面输入SNTP服务器的域名或IP地址。 </li>
<li>时间区域列表框允许你选择所在地的时间区域。</li>
</ul>
</ul></td>
</tr>
<tr name=ntwk_cfgRoute id=ntwk_cfgRoute>
<td>&nbsp;</td>
<td class="tabdataleft"><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>8<% else %>7<% end if%>.路由配置<a name=路由配置 id="路由配置"></a></td>
</tr>
<tr name=ntwk_cfgRoute_1 id=ntwk_cfgRoute_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=ntwk_cfgRoute_2 id=ntwk_cfgRoute_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>静态路由显示</li>
</ul></td>
</tr>
<tr name=ntwk_cfgRoute_3 id=ntwk_cfgRoute_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=ntwk_cfgRoute_4 id=ntwk_cfgRoute_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>静态路由:设置目的网络地址、子网掩码、网关地址和接口。其中网关地址和接口至少选择一项。</li>
</ul></td>
</tr>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYJXSupported","h") = "Yes" then %>
<tr name=ntwk_dslimit id=ntwk_dslimit>
<td>&nbsp;</td>
<td class="tabdataleft"><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>9<% else %>8<% end if%>.QoS带宽管理<a name='QoS带宽管理' id='QoS带宽管理'></a></td>
</tr>
<tr name=ntwk_dslimit_1 id=ntwk_dslimit_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).支持上行数据限速模式和下行数据限速模式</td>
</tr>
<tr name=ntwk_dslimit_2 id=ntwk_dslimit_2>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).支持基于用户端口和用户IP地址段进行限速</td>
</tr>
<% end if %>
<tr>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC id="secuiText">三、安全<a name=安全></a></td>
</tr>
<tr name=securityURL id=securityURL>
<td>&nbsp;</td>
<td class="tabdataleft">1.广域网访问设置<a name=广域网访问设置 id="广域网访问设置"></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>是否启用</li>
<li>当前的过滤模式</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>设置</li>
<ul>
<li>启用广域网过滤:启动或关闭广域网访问功能，默认为“<strong>关闭</strong>”。设置完成后，界面上显示新的广域网访问状态。 </li>
<li>过滤模式:“<b>黑名单”</b>或“<b>白名单”</b>；单击“更改模式”修改过滤当前的过滤模式。</li>
<li>URL:输入正确的URL。添加一条URL到过滤列表中。</li>
</ul>
<li>过滤列表:显示已经过滤的URL列表。</li>
<li>当前的过滤列表为黑名单时，过滤列表中的URL请求将被过滤；当前过滤列表为白名单时，只有过滤列表中的URL请求被接受。</li>
</ul></td>
</tr>
<tr name=securityFirewall id=securityFirewall>
<td>&nbsp;</td>
<td class="tabdataleft">2.防火墙<a name=防火墙></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li id=securityFirewall_1>防火墙设置:安全级别设置</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>安全级别设置:使用该操作，可以将防火墙等级设置为"<b>高</b>"，"<b>中</b>"或"<b>低</b>"。设置完成后，界面上显示新的防火墙状态。</li>
</ul></td>
</tr>
<tr name=securityMAC id=securityMAC>
<td>&nbsp;</td>
<td class="tabdataleft">3.MAC过滤<a name=MAC过滤></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>是否启用</li>
<li>当前的过滤模式</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>设置</li>
<ul>
<li>启用广域网MAC过滤:启动或关闭广域网访问功能，默认为“<strong>关闭</strong>”。设置完成后，界面上显示新的广域网访问状态。 </li>
<li>过滤模式:“<b>黑名单”</b>或“<b>白名单”</b>；此操作修改当前的过滤模式。</li>
<li>MAC地址:输入正确的MAC地址，添加一条MAC地址到过滤列表中。</li>
</ul>
<li>过滤列表:显示已经过滤的MAC地址列表。</li>
<li>当前的过滤列表为黑名单时，具有过滤列表中的MAC地址的PC将无法访问Internet网络；当前过滤模式为白名单时，具有过滤列表中的MAC地址的PC才可以访问Internet网络；不管是黑名单还是白名单，LAN侧的PC都可以访问网关。</li>
</ul></td>
</tr>
<tr name=securityPort id=securityPort>
<td>&nbsp;</td>
<td class="tabdataleft">4.端口过滤<a name=端口过滤></a></td>
</tr>
<tr name=securityPort_1 id=securityPort_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=securityPort_2 id=securityPort_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>是否启用</li>
<li>当前的过滤模式</li>
</ul></td>
</tr>
<tr name=securityPort_3 id=securityPort_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=securityPort_4 id=securityPort_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>设置</li>
<ul>
<li>启用广域网端口过滤:启动或关闭广域网访问功能，默认为“<strong>关闭</strong>”。设置完成后，界面上显示新的广域网访问状态。 </li>
<li>过滤模式:“<b>黑名单”</b>或“<b>白名单”</b>；此操作修改当前的过滤模式。</li>
<li>“添加”:添加一条过滤规则到过滤名单；“删除”：删除一条或多条选中的过滤规则。</li>
</ul>
<li>过滤列表:显示当前的规则过滤列表。</li>
<li>当前的过滤模式为黑名单时，符合过滤列表任意一条规则的数据包将无法通过网关；当前过滤模式为白名单时，符合过滤列表中规则的数据包才可以通过网关；不管是黑名单还是白名单,LAN侧的PC都可以访问网关。</li>
<li>过滤规则可设置“过滤名称”，可将“协议”、“源IP起始地址”、“源IP结束地址”、“源端口”、“目的IP起始地址”、“目的IP结束地址”、“目的端口”作为过滤条件进行设置。 </li>
</ul></td>
</tr>
<tr id="apptitle">
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC id="appText">四、应用<a name=应用></a></td>
</tr>
<tr name=appDDNS id=appDDNS>
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.DDNS配置<a name=DDNS配置 id="DDNS配置"></a></td>
</tr>
<tr name=appDDNS_1 id=appDDNS_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appDDNS_2 id=appDDNS_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>动态域名</li>
<ul>
<li>DDNS设置</li>
</ul>
</ul>	 		</td>
</tr>
<tr name=appDDNS_3 id=appDDNS_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=appDDNS_4 id=appDDNS_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>启用开关:使用该操作,启动或关闭DDNS功能,设置完成后,界面上显示新的DDNS状态.</li>
<li>DDNS设置:使用该操作,对DDNS相关参数进行设置,设置完成后,界面上显示新的DDNS状态.</li>
</ul>	 	</td>
</tr>
<tr name=appAdnat id=appAdnat>
<td width=20>&nbsp;</td>
<script language="JavaScript" type="text/javascript">
if((curUserName == sysUserName) && (isCZGDSupport == 'Yes'))
document.write( '<td class="tabdataleft">1.高级NAT配置<a name=高级NAT配置 id="高级NAT配置"></a></td>');
else
document.write( '<td class="tabdataleft">2.高级NAT配置<a name=高级NAT配置 id="高级NAT配置"></a></td>');
</script>
</tr>
<tr name=appAdnat_1 id=appAdnat_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appAdnat_2 id=appAdnat_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id='appAdnat_2_alg'>ALG设置</li>
<ul id='appAdnat_2_alg_s'>
<li>L2TP ALG状态</li>
<li>IPSec ALG状态</li>
<li>H.323 ALG状态</li>
<li>RTSP ALG状态</li>
<li>SIP ALG状态</li>
<li>FTP ALG状态</li>
<li>PPTP ALG状态</li>
</ul>
<li id='appAdnat_2_dmz'>DMZ设置</li>
<ul type="circle" id='appAdnat_2_dmz_s'>
<li>DMZ设置状态</li>
</ul>
<%if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then%>
<li>虚拟主机设置</li>
<ul type="circle">
<li>虚拟主机设置状态</li>
</ul>
<% end if %>
</ul>	 	</td>
</tr>
<tr name=appAdnat_3 id=appAdnat_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=appAdnat_4 id=appAdnat_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id='appAdnat_4_alg'>ALG启用开关:使用该操作，启动或关闭各个ALG功能，设置完成后，界面上显示新的各个ALG状态。</li>
<li id='appAdnat_4_dmz'>DMZ设置:使用该操作，对DMZ相关参数进行设置，设置完成后，界面上显示新的DMZ状态。</li>
<%if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then%>
<li>虚拟主机设置:使用该操作，支持外部端口的访问映射到家庭内部不同的IP地址，设置完成后，界面上显示已设置的映射规则。</li>
<% end if %>
</ul>	 	</td>
</tr>
<tr name=appUPNP id=appUPNP>
<td width=20>&nbsp;</td>
<td class="tabdataleft">3.UPNP配置<a name=UPNP配置 id="UPNP配置"></a></td>
</tr>
<tr name=appUPNP_1 id=appUPNP_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appUPNP_2 id=appUPNP_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>显示和配置UPnP是否启用。</li>
</ul>	 	</td>
</tr>
<tr name=appUPNP_3 id=appUPNP_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=appUPNP_4 id=appUPNP_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>通过复选框配置UPnP的使能状态。选中复选框即能使UPnP业务生效，否则UPnP业务将禁用。</li>
</ul>	 	</td>
</tr>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
<tr name=appVoip id=appVoip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">4.宽带电话设置<a name=宽带电话设置 id="宽带电话设置"></a></td>
</tr>
<tr name=appVoip_1 id=appVoip_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appVoip_2 id=appVoip_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>宽带电话基本设置。</li>
<li>宽带电话高级设置。</li>
</ul>	 	</td>
</tr>
<tr name=appVoip_3 id=appVoip_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=appVoip_4 id=appVoip_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>修改SIP参数：点击“应用”按钮保存参数设置。 </li>
<li>Outbound服务器地址：ISP提供的外发代理服务器的地址，如果为空或者为0.0.0.0，外发代理服务器将不启用。</li>
<li>Outbound服务器端口号：SIP外发代理服务器的UDP端口，如果设置为空或者0，将使用默认值5060。</li>
<li>备用Outbound服务器地址：ISP提供的备用外发代理服务器的地址，如果为空或者为0.0.0.0，备用外发代理服务器将不启用。</li>
<li>备用Outbound服务器端口号：SIP备用外发代理服务器的UDP端口，如果设置为空或者0，将使用默认值5060。</li>
<li>主用服务器地址：ISP提供的SIP代理服务器的地址，如果为空或者为0.0.0.0，SIP代理服务器将不启用。</li>
<li>主用端口号：SIP代理服务器的UDP端口，如果设置为空或者0，将使用默认值5060。</li>
<li>备用服务器地址：ISP提供的SIP备用代理服务器的地址，如果为空或者为0.0.0.0，SIP备用代理服务器将不启用。</li>
<li>备用端口号：SIP备用代理服务器的UDP端口，如果设置为空或者0，将使用默认值5060。</li>
<li>本地端口： 本地SIP的UDP端口号，默认为5060。</li>
<li>数图： 指示语音业务的拨号方案。</li>
<li>数图匹配模式：包含最大匹配和最小匹配。</li>
<li>国家及地区：指示当前语音业务的国家和地区。</li>
<li>信令端口：指示在呼叫信令选择的网络端口。</li>
<li>媒体端口：指示在呼叫媒体选择的网络端口。</li>
</ul>	 	</td>
</tr>
<% end if %>

<tr name=appIGMP id=appIGMP>
<td>&nbsp;</td>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
<td class="tabdataleft">
<% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
	4.IGMP/MLD设置
<% else %>
	4.IGMP设置
<% end if %>
	<a name=IGMP设置 id="IGMP设置"></a></td>
<% else %>
<td class="tabdataleft">
<% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
	5.IGMP/MLD设置
<% else %>
	5.IGMP设置
<% end if %>
	<a name=IGMP设置 id="IGMP设置"></a></td>
<% end if %> 
</tr>
<tr name=appIGMP_1 id=appIGMP_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appIGMP_2 id=appIGMP_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
<li>IGMP/MLD Snooping状态</li>
<li>IGMP/MLD Proxy状态</li>
<% else %>
<li>IGMP Snooping状态</li>
<li>IGMP Proxy状态</li>
<% end if %>
</ul></td>
</tr>
<tr name=appIGMP_3 id=appIGMP_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=appIGMP_4 id=appIGMP_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
<li>IGMP/MLD Snooping设置:使用该操作，启用或者停用IGMP/MLD Snooping功能，设置完成后，界面上显示IGMP/MLD Snooping设置状态。</li>
<li>IGMP/MLD Proxy使能设置:使用该操作，启用或者停用IGMP/MLD Proxy功能，设置完成后，界面上显示IGMP/MLD Proxy使能状态。</li>
<li>IGMP/MLD Proxy设置:使用该操作，启用或者停用每条WAN连接IGMP/MLD Proxy功能，设置完成后，界面上显示每条WAN连接的IGMP/MLD Proxy设置状态。</li>
<% else %>
<li>IGMP Snooping设置:使用该操作，启用或者停用IGMP Snooping功能，设置完成后，界面上显示IGMP Snooping设置状态。</li>
<li>IGMP Proxy使能设置:使用该操作，启用或者停用IGMP Proxy功能，设置完成后，界面上显示IGMP Proxy使能状态。</li>
<li>IGMP Proxy设置:使用该操作，启用或者停用每条WAN连接IGMP Proxy功能，设置完成后，界面上显示每条WAN连接的IGMP Proxy设置状态。</li>
<% end if %>
</ul></td>
</tr>
<tr name=appDaily id=appDaily>
<td>&nbsp;</td>
<script language="JavaScript" type="text/javascript">
if (curUserName == sptUserName)
{
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
document.write( '<td class="tabdataleft" >5.日常应用<a name=日常应用 id="日常应用"></a></td>');
<% else %>
document.write( '<td class="tabdataleft" >6.日常应用<a name=日常应用 id="日常应用"></a></td>');
<% end if %> 
}
else if(curUserName == sysUserName)
{
if(isCZGDSupport == 'Yes')
document.write( '<td class="tabdataleft" >2.日常应用<a name=日常应用 id="日常应用"></a></td>');
else
document.write( '<td class="tabdataleft" >1.日常应用<a name=日常应用 id="日常应用"></a></td>');
}
</script>
</tr>
<tr name=appDaily_1 id=appDaily_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appDaily_2 id=appDaily_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
<li>FTP客服端配置信息</li>
<% end if %>
<li id=appDaily_5>IPTV配置信息</li>
</ul></td>
</tr>
<tr name=appDaily_3 id=appDaily_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=appDaily_4 id=appDaily_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
<li>通过FTP客服端功能，在内网或外网访问插在终端的USB设备中的文件，同时允许用户下载指定的文件到终端的USB设备中。</li>
<% end if %>
<li id=appDaily_6>可以设置WAN口组播VLAN。</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"></td>
</tr>
<tr>
<script language="JavaScript" type="text/JavaScript">
if (curUserName == sysUserName)
	document.write('<td id=manText class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>五、管理<a name=管理></a></td>');
else
	document.write('<td id=manText class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>五、管理<a name=管理></a></td>');
</script>
</tr>
<tr name=manageUser id=manageUser>
<td>&nbsp;</td>
<td class="tabdataleft">1.用户管理<a name=用户管理></a></td>
</tr>
<tr name=manageUser_1 id=manageUser_1>
<td>&nbsp;</td>
<td class="tabdataleft" >&nbsp;(1)电信维护账号</td>
</tr>
<tr name=manageUser_2 id=manageUser_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>密码设置:使用电信维护帐号登录后，可以修改用户管理帐号的密码，而不需要校验原密码。</li>
</ul></td>
</tr>
<tr name=manageUser_3 id=manageUser_3>
<td>&nbsp;</td>
      <td class="tabdataleft">&nbsp;(2)用户管理帐号</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
          <li>用户管理帐号和密码设置:使用用户管理帐号登录后,可修改用户管理帐号的密码，需要校验原密码。 </li>
        </ul></td>
</tr>
<tr name=manageEquip id=manageEquip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">2.设备管理<a name=设备管理 id="设备管理"></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>设备重启:使用该操作,重新启动网关。</li>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
<li id=manageEquip_USB>USB备份配置文件:当有USB设备连接到终端上,点击"备份配置"就可以将当前配置文件备份到USB设备中。另外可以启用USB自动恢复配置文件功能，将USB设备中已有的配置文件导入到终端系统中。</li>
<% end if %>
<li id=manageEquip_LOG style="display:none">清除日志记录:使用该操作,清除当前所有日志。</li>
<li id=manageEquip_2>恢复出厂设置:使用该操作,将网关恢复到出厂时的默认配置。</li>
</ul></td>
</tr>
<tr name=manageSyslog id=manageSyslog>
<td>&nbsp;</td>
<td class="tabdataleft">3.日志文件管理<a name=日志文件管理 id="日志文件管理"></a></td>
</tr>
<tr name=manageSyslog_1 id=manageSyslog_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul><li id="manageSyslog_1_1">日志写入等级设置:可以启用或者禁用写入日志文件。在启用写入日志文件的情况下，可以设置写入和显示日志文件的等级。</li>
<li id="manageSyslog_1_2">日志:显示当前等级下的日志内容。</li>
<li id="manageSyslog_1_3">维护结束上报:当电信维护人员在维护中使用了电信维护帐号时，通过该界面操作通知ITMS更新电信维护帐号的密码。</li>
</ul></td>
</tr>
<tr name=netDiag id=netDiag>
<td id=diagText class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>六、诊断<a name=诊断></a></td>
</tr>
<tr name=networkDiag id=networkDiag>
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.网络诊断<a name=网络诊断 id="网络诊断"></a></td>
</tr>
<tr name=networkDiag_1 id=networkDiag_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>PING测试:支持在选定的WAN连接下ping测试，目的地址支持IP地址和域名两种方式。</li>
<li>Tracert测试:支持在选定的WAN连接下Tracert测试，目的地址支持IP地址和域名两种方式。</li>
<li>Inform手动上报:支持Inform手动上报，并且正确显示手动Inform上报的状态。</li>
</ul>	 	</td>
</tr>
<% if tcwebApi_get("WebCustom_Entry","isCTJOYMESupported","h") = "Yes" then %>
<tr name=serviceDiag id=serviceDiag>
<td width=20>&nbsp;</td>
<td class="tabdataleft">2.业务诊断<a name=业务诊断 id="业务诊断"></a></td>
</tr>
<tr name=serviceDiag_1 id=serviceDiag_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>外线电压测试:支持页面下发外线电压测试命令，并且正确显示测试结果。</li>
<li>振铃阻抗测试:支持页面下发振铃阻抗测试命令，并且正确显示测试结果。</li>
<li>模拟主叫测试:支持输入特定测试号码，并且正确显示线路状态和测试结果。</li>
</ul>	 	</td>
</tr>
<% end if %>
</table>




</form>
</body>
</html>
