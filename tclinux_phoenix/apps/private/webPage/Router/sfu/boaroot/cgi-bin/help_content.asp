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
	manageEquip_LOG.style.display = "none";
	
if (curUserName == sysUserName)
{
statusEquip.style.display = "";
statusNetwork.style.display = "";
getElement('statusNetwork_1').innerText = '显示连接信息和PON信息。 连接信息显示网络侧连接状态，各条PVC的IP地址、子网掩码，默认网关，DNS服务器信息；PON信息显示PON的链路连接状态，链路性能统计，光模块信息。 ';
statusUser.style.display = "";
statusVoip.style.display = "none";
statusVoip_1.style.display = "none";
apptitle.style.display = "none";
networktitle.style.display = "none";
getElement('statusUser_1').innerText ='<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>显示WLAN接口信息，<% end if %>以太网接口信息<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>和USB接口信息<% end if %>。<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>WLAN接口信息显示无线网络连接状态，SSID_1的认证方式和加密状态等；<% end if %>以太网接口信息显示网关IP地址，MAC地址，每个LAN口的状态；<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>USB接口信息显示各个USB连接设备的信息。 <% end if %>';

var netwkObjs = document.getElementsByName('ntwk_cfgWan');
for (i = 0; i < netwkObjs.length; i++)
{
netwkObjs[i].style.display = "none";
}
ntwk_cfgDHCP.style.display = "none";
ntwk_cfgDHCP_1.style.display = "none";
ntwk_cfgloidset.style.display = "none";
ntwk_cfgloidset_1.style.display = "none";
ntwk_cfgSNTP.style.display = "none";
ntwk_cfgSNTP_1.style.display = "none";
ntwk_cfgSNTP_2.style.display = "none";
ntwk_cfgSNTP_3.style.display = "none";
ntwk_cfgSNTP_4.style.display = "none";

appVoip.style.display = "none";
appVoip_1.style.display = "none";
appVoip_2.style.display = "none";
appVoip_3.style.display = "none";
appVoip_4.style.display = "none";


manageUser.style.display = "";
manageUser_1.style.display = "none";
manageUser_2.style.display = "none";
manageUser_3.style.display = "none";
manageEquip_LOG.style.display = "none";
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
manageEquip_USB.style.display = "none";
<% end if %>
manageEquip_2.style.display = "none";
manageSyslog.style.display = "none";
manageSyslog_1.style.display = "none";
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
<tr id='networktitle'>
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
<li>宽带设置:对语音连接进行相关参数设置,设置完成后,界面上显示相应的状态。</li>
<ul>
<li>DHCP:设备将从ISP通过DHCP自动获得IP。 </li>
<li>Static:由ISP配置一个静态的IP地址。需要输入的信息包括:IP地址，子网掩码，缺省网关。</li>
<li>PPPoE:ISP将提供帐号，填入包括如下信息:用户名、密码。该模式系统将通过PPPoE动态获得IP。 </li>
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
<li>IPv4配置:使用该操作,对LAN相关参数进行设置。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgloidset id=ntwk_cfgloidset>
<td>&nbsp;</td>
<td class="tabdataleft">3.LOID配置管理<a name=LOID配置管理 id="LOID配置管理"></a></td>
</tr>
<tr name=ntwk_cfgloidset_1 id=ntwk_cfgloidset_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>通过设置LOID和Password，向网管服务器注册并请求业务配置。</li>
</ul></td>
</tr>
<tr name=ntwk_cfgSNTP id=ntwk_cfgSNTP>
<td>&nbsp;</td>
<td class="tabdataleft">4.时间管理<a name=时间管理 id="时间管理"></a></td>
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

<tr id="apptitle">
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>三、应用<a name=应用></a></td>
</tr>
<tr name=appVoip id=appVoip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.宽带电话设置<a name=宽带电话设置 id="宽带电话设置"></a></td>
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

<tr>
<td>&nbsp;</td>
<td class="tabdataleft"></td>
</tr>
<tr>
<script language="JavaScript" type="text/JavaScript">
if (curUserName == sysUserName)
	document.write('<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>二、管理<a name=管理></a></td>');
else
	document.write('<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>四、管理<a name=管理></a></td>');
</script>
</tr>
<tr name=manageUser id=manageUser>
<td>&nbsp;</td>
<td class="tabdataleft">1.用户管理<a name=用户管理></a></td>
</tr>
<tr name=manageUser_1 id=manageUser_1>
<td>&nbsp;</td>
<td class="tabdataleft" >&nbsp;(1)联通维护账号</td>
</tr>
<tr name=manageUser_2 id=manageUser_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>密码设置:使用联通维护帐号登录后，可以修改用户管理帐号的密码，而不需要校验原密码。</li>
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
<td class="tabdataleft"><ul><li>日志写入等级设置:可以启用或者禁用写入日志文件。在启用写入日志文件的情况下，可以设置写入和显示日志文件的等级。</li>
<li>日志:显示当前等级下的日志内容。</li>
</ul></td>
</tr>
<tr name=netDiag id=netDiag>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>五、诊断<a name=诊断></a></td>
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
</ul>	 	</td>
</tr>
</table>

</form>
</body>
</html>
