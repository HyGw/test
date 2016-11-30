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
function FormLoad()
{
with (document.forms[0])
{
	appDDNS.style.display = "none";
	appDDNS_1.style.display = "none";
	appDDNS_2.style.display = "none";
	appDDNS_3.style.display = "none";
	appDDNS_4.style.display = "none";
	appUPNP.style.display = "none";
	appUPNP_1.style.display = "none";
	appUPNP_2.style.display = "none";
	appUPNP_3.style.display = "none";
	appUPNP_4.style.display = "none";
	appDaily.style.display = "none";
	appDaily_2.style.display = "none";
	appDaily_3.style.display = "none";
	appDaily_4.style.display = "none";
	manageEquip_LOG.style.display = "none";
	manageEquip_USB.style.display = "none";
	
if (curUserName == sysUserName)
{
statusEquip.style.display = "";
statusNetwork.style.display = "";
getElement('statusNetwork_1').innerText = '显示连接信息和DSL信息。 连接信息显示网络侧连接状态，各条PVC的IP地址、子网掩码，默认网关，DNS服务器信息；DSL信息显示DSL的当前同步状态，连接状态。 ';
statusUser.style.display = "";
getElement('statusUser_1').innerText ='显示WLAN接口信息，以太网接口信息和USB接口信息。WLAN接口信息显示无线网络连接状态，SSID_1的认证方式和加密状态等；以太网接口信息显示网关IP地址，MAC地址，每个LAN口的状态；USB接口信息显示各个USB连接设备的信息。 ';
statusACS.style.display = "none";
statusACS_1.style.display = "none";
for (i = 0; i < ntwk_cfgWan.length; i++)
{
ntwk_cfgWan[i].style.display = "none";
}
ntwk_cfgDHCP.style.display = "none";
ntwk_cfgDHCP_1.style.display = "none";
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
securityURL.style.display = "";
securityFirewall.style.display = "";
getElement('securityFirewall_1').innerText ='防火墙设置:安全级设置';
securityMAC.style.display = "";
securityPort.style.display = "none";
securityPort_1.style.display = "none";
securityPort_2.style.display = "none";
securityPort_3.style.display = "none";
securityPort_4.style.display = "none";
apptitle.style.display = "none";
appDDNS.style.display = "none";
appDDNS_1.style.display = "none";
appDDNS_2.style.display = "none";
appDDNS_3.style.display = "none";
appDDNS_4.style.display = "none";
appAdnat.style.display = "none";
appAdnat_1.style.display = "none";
appAdnat_2.style.display = "none";
appAdnat_3.style.display = "none";
appAdnat_4.style.display = "none";
appUPNP.style.display = "none";
appUPNP_1.style.display = "none";
appUPNP_2.style.display = "none";
appUPNP_3.style.display = "none";
appUPNP_4.style.display = "none";
appIGMP.style.display = "none";
appIGMP_1.style.display = "none";
appIGMP_2.style.display = "none";
appIGMP_3.style.display = "none";
appIGMP_4.style.display = "none";
appDaily.style.display = "";
manageUser.style.display = "";
manageUser_1.style.display = "none";
manageUser_2.style.display = "none";
manageUser_3.style.display = "none";
manageEquip_LOG.style.display = "none";
manageEquip_2.style.display = "none";
manageSyslog.style.display = "none";
manageSyslog_1.style.display = "none";
manageDiag.style.display = "none";
manageDiag_1.style.display = "none";
netDiag.style.display = "none";
networkDiag.style.display = "none";
networkDiag_1.style.display = "none";
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
<li id=statusNetwork_1>显示连接信息和DSL信息。 连接信息显示网络侧连接状态，各条PVC的IP地址、子网掩码，默认网关，DNS服务器信息，PVC及封装信息；DSL信息显示DSL的当前同步状态，连接状态，上下行速率，噪声裕量，交织深度，线路协议，线路衰减，输出功率，信道类型等。 </li>
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
<li id=statusUser_1>显示WLAN接口信息，以太网接口信息。  WLAN接口信息显示无线网络连接状态，信道，各个SSID的统计信息、SSID、认证方式和加密状态等；以太网接口信息显示网关IP地址，MAC地址，每个LAN口的状态、收发包和字节数。</li>
</ul>
</td>
</tr>
<tr name=statusACS id=statusACS>
<td width=20>&nbsp;</td>
<td class="tabdataleft">4.远程管理状态<a name=远程管理状态 id="远程管理状态"></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusACS_1>显示交互建立情况、业务配置下发状态，交互建立包括主动上报Inform情况以及接受ITMS连接请求情况。</li>
</ul>
</td>
</tr>
<tr>
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
<li>VPI/VCI:设置链接使用的VPI和VCI。</li>
<li id=ntwk_cfgWan_1>启用绑定:可以把各个LAN口、各个SSID和对应的WAN口绑定。</li>
<li>模式:分成Route、Bridge两种模式。Route模式下有三种连接模式DHCP（从ISP处得到一个IP地址）、Static（经ISP配置一个静态的IP给你）、PPPoE。Bridge模式可以将设备配置成介于LAN和ISP之间的网桥设备，它可以使得两个或多个网络的通信就像处在同一LAN物理连接上。</li>
<li>链接方式:包括通过IP方式建立链接和通过PPP方式建立链接两种。根据实际的网络需要选择建立链接的方式。</li>
<li>IP协议版本:设置链接使用的IP版本，通常使用IPv4版本，也可以通过选择IPv6来支持IPv6。如果选择IPv4/v6方式，则同时使用IPv4和IPv6两种IP版本。</li>
<li>PPPoE:ISP将提供帐号，填入包括如下信息:用户名、密码、服务名称，并配置拨号方式。该模式系统将通过PPPoE动态获得IP。 </li>
<li>DHCP:设备将从ISP通过DHCP自动获得IP。 </li>
<li>Static:由ISP配置一个静态的IP地址。需要输入的信息包括:IP地址，子网掩码，首选DNS服务器及备用DNS服务器等，IP地址必须输入正确的格式，4个IP字节之间由&quot;.&quot;号分开(x.x.x.x)，例如:192.168.0.100。</li>
<li id=ntwk_cfgWan_2>支持802.1q、802.1p、NAT、IGMP Proxy。</li>
<li id=ntwk_cfgWan_3>服务类别:UBR Without PCR、UBR With PCR、CBR、Non-Real Time VBR、Realtime VBR。</li>
<li id=ntwk_cfgWan_4>PVC封装模式:LLC和VCMUX。</li>
<li>IPv6 WAN信息获取方式:可以通过DHCP或者SLAAC两种方式动态的获取IPv6的地址，也可以静态设置IPv6的地址，根据具体的网络情况进行设置。</li>
</ul>
</ul></td>
</tr>
<tr name=ntwk_cfgDHCP id=ntwk_cfgDHCP>
<td>&nbsp;</td>
<td class="tabdataleft">2.LAN侧地址配置<a name=LAN侧地址配置 id="LAN侧地址配置"></a></td>
</tr>
<tr name=ntwk_cfgDHCP_1 id=ntwk_cfgDHCP_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IPv4配置:默认为&quot;启用DHCP&quot;。使用该操作,对DHCP相关参数进行设置,设置完成后，界面上显示新的DHCP状态。可以配置租约时间（1分钟、1小时、1天、1周），DHCP工作模式（DHCP 
          Server和DHCP relay）。</li>
<li>IPv6配置:可以配置是否开启对LAN侧的RA服务，让LAN侧设备可以通过RA方式获取IPv6的地址，同时也可以设置让LAN侧通过DHCP方式获取IPv6的地址。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgWLAN id=ntwk_cfgWLAN>
<td>&nbsp; </td>
<script language="JavaScript" type="text/javascript">
if (curUserName == sptUserName)
{
document.write( '<td class="tabdataleft" >3.WLAN配置<a name=WLAN配置 id="WLAN配置"></a></td>');
}
else if(curUserName == sysUserName)
{
document.write( '<td class="tabdataleft" >1.WLAN配置<a name=WLAN配置 id="WLAN配置"></a></td>');
}
</script>
</tr>
<tr>
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
<td class="tabdataleft">4.远程管理<a name=远程管理 id="远程管理"></a></td>
</tr>
<tr name=ntwk_cfgRemoteManage_1 id=ntwk_cfgRemoteManage_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=ntwk_cfgRemoteManage_2 id=ntwk_cfgRemoteManage_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>ITMS服务器设置</li>
</ul></td>
</tr>
<tr name=ntwk_cfgRemoteManage_3 id=ntwk_cfgRemoteManage_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=ntwk_cfgRemoteManage_4 id=ntwk_cfgRemoteManage_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>用于设置ITMS服务器的URL或者IP地址，ITMS认证网关的用户名和密码，以及网关认证ITMS的用户名和密码。同时还可以配置网关是否发送周期性上报报文，以及上报报文的周期。另外还可以进行证书认证的启用或禁用。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgQoS id=ntwk_cfgQoS>
<td>&nbsp;</td>
<td class="tabdataleft">5.QoS<a name=QoS id="QoS"></a></td>
</tr>
<tr name=ntwk_cfgQoS_1 id=ntwk_cfgQoS_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>QoS 设置（仅电信维护帐号可使用）:出厂时可预置和业务相关的配置模板，使用时根据具体开通业务进行选择和配置。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgSNTP id=ntwk_cfgSNTP>
<td>&nbsp;</td>
<td class="tabdataleft">6.时间管理<a name=时间管理 id="时间管理"></a></td>
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
<td class="tabdataleft">7.路由配置<a name=路由配置 id="路由配置"></a></td>
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
<tr>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>三、安全<a name=安全></a></td>
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
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>四、应用<a name=应用></a></td>
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
<td class="tabdataleft">1.高级NAT配置<a name=高级NAT配置 id="高级NAT配置"></a></td>
</tr>
<tr name=appAdnat_1 id=appAdnat_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appAdnat_2 id=appAdnat_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>ALG设置</li>
<ul>
<li>L2TP ALG状态</li>
<li>IPSec ALG状态</li>
<li>H.323 ALG状态</li>
<li>RTSP ALG状态</li>
<li>SIP ALG状态</li>
</ul>
<li>DMZ设置</li>
<ul type="circle">
<li>DMZ设置状态</li>
</ul>
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
<li>ALG启用开关:使用该操作，启动或关闭各个ALG功能，设置完成后，界面上显示新的各个ALG状态。</li>
<li>DMZ设置:使用该操作，对DMZ相关参数进行设置，设置完成后，界面上显示新的DMZ状态。</li>
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
<tr name=appIGMP id=appIGMP>
<td>&nbsp;</td>
<td class="tabdataleft">2.IGMP设置<a name=IGMP设置 id="IGMP设置"></a></td>
</tr>
<tr name=appIGMP_1 id=appIGMP_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appIGMP_2 id=appIGMP_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IGMP Snooping状态</li>
<li>IGMP Proxy状态</li>
</ul></td>
</tr>
<tr name=appIGMP_3 id=appIGMP_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr name=appIGMP_4 id=appIGMP_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IGMP Snooping设置:使用该操作，启用或者停用IGMP Snooping功能，设置完成后，界面上显示IGMP  Snooping设置状态。</li>
<li>IGMP Proxy使能设置:使用该操作，启用或者停用IGMP Proxy功能，设置完成后，界面上显示IGMP Proxy使能状态。</li>
<li>IGMP Proxy设置:使用该操作，启用或者停用每条WAN连接IGMP Proxy功能，设置完成后，界面上显示每条WAN连接的IGMP Proxy设置状态。</li>
</ul></td>
</tr>
<tr name=appDaily id=appDaily>
<td>&nbsp;</td>
<script language="JavaScript" type="text/javascript">
/*if (curUserName == sptUserName)
{
document.write( '<td class="tabdataleft" >5.日常应用<a name=日常应用 id="日常应用"></a></td>');
}
else if(curUserName == sysUserName)
{
document.write( '<td class="tabdataleft" >1.日常应用<a name=日常应用 id="日常应用"></a></td>');
}*/
</script>
</tr>
<tr name=appDaily_2 id=appDaily_2>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).状态显示</td>
</tr>
<tr name=appDaily_3 id=appDaily_3>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>FTP客服端配置信息</li>
</ul></td>
</tr>
<tr name=appDaily_4 id=appDaily_4>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).操作</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"></td>
</tr>
<tr>
<script language="JavaScript" type="text/JavaScript">
if (curUserName == sysUserName)
	document.write('<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>四、管理<a name=管理></a></td>');
else
	document.write('<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>五、管理<a name=管理></a></td>');
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
<li id=manageEquip_USB>USB备份配置文件:当有USB设备连接到终端上,点击"备份配置"就可以将当前配置文件备份到USB设备中。另外可以启用USB自动恢复配置文件功能，将USB设备中已有的配置文件导入到终端系统中。</li>
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
<td class="tabdataleft"><ul><li>日志写入等级设置:可以启用或者禁用写入日志文件。在启用写入日志文件的情况下，可以设置写入和显示日志文件的等级。</li>
<li>日志:显示当前等级下的日志内容。</li>
</ul></td>
</tr>
<tr name=manageDiag id=manageDiag>
<td width=20>&nbsp;</td>
<td class="tabdataleft">4.维护<a name=维护 id="维护"></a></td>
</tr>
<tr name=manageDiag_1 id=manageDiag_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>维护结束上报:当电信维护人员在维护中使用了电信维护帐号时，通过该界面操作通知ITMS更新电信维护帐号的密码。</li>
</ul>	 	</td>
</tr>
<tr name=netDiag id=netDiag>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>六、诊断<a name=诊断></a></td>
</tr>
<tr name=networkDiag id=networkDiag>
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.网络诊断<a name=网络诊断 id="网络诊断"></a></td>
</tr>
<tr name=networkDiag_1 id=networkDiag_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>线路测试:可以验证DSL环路是否连通。</li>
<li>PING测试:支持在选定的WAN连接下ping测试，目的地址支持IP地址和域名两种方式。</li>
<li>Tracert测试:支持在选定的WAN连接下Tracert测试，目的地址支持IP地址和域名两种方式。</li>
<li>Inform手动上报:支持Inform手动上报，并且正确显示手动Inform上报的状态。</li>
</ul>	 	</td>
</tr>
</table>




</form>
</body>
</html>
