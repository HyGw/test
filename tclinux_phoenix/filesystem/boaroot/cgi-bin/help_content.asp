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
getElement('statusNetwork_1').innerText = '��ʾ������Ϣ��PON��Ϣ�� ������Ϣ��ʾ���������״̬������PVC��IP��ַ���������룬Ĭ�����أ�DNS��������Ϣ��PON��Ϣ��ʾPON����·����״̬����·����ͳ�ƣ���ģ����Ϣ�� ';
statusUser.style.display = "";
getElement('statusUser_1').innerText ='<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>��ʾWLAN�ӿ���Ϣ��<% end if %>��̫���ӿ���Ϣ<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>��USB�ӿ���Ϣ<% end if %>��<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>WLAN�ӿ���Ϣ��ʾ������������״̬��SSID_1����֤��ʽ�ͼ���״̬�ȣ�<% end if %>��̫���ӿ���Ϣ��ʾ����IP��ַ��MAC��ַ��ÿ��LAN�ڵ�״̬��<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>USB�ӿ���Ϣ��ʾ����USB�����豸����Ϣ�� <% end if %>';
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
<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
var v_cfgRemote = '<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>';
if ( v_cfgRemote == '1' )
{
	var item_idx = 1;
	<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %> 
	item_idx ++;
	<% end if %>
	ntwk.style.display = '';
	ntwk_cfgRemoteManage.style.display = '';
	ntwk_cfgRM01.innerHTML = item_idx + '.Զ�̹���<a name=Զ�̹��� id="Զ�̹���"></a>';
	ntwk_cfgRemoteManage_3.style.display = '';
	ntwk_cfgRM03.innerHTML = '&nbsp;(1).����';
	ntwk_cfgRemoteManage_4.style.display = '';
	ntwk_cfgRM04.innerHTML = '<ul><li>OLT��֤ע��</li></ul>';
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
securityURL.style.display = "";
securityFirewall.style.display = "";
getElement('securityFirewall_1').innerHTML ='����ǽ����:��ȫ������';
securityMAC.style.display = "";
securityPort.style.display = "none";
securityPort_1.style.display = "none";
securityPort_2.style.display = "none";
securityPort_3.style.display = "none";
securityPort_4.style.display = "none";
securityProtocolFilter.style.display = "none";
securityProtocolFilter_1.style.display = "none";
securityProtocolFilter_2.style.display = "none";
securityProtocolFilter_3.style.display = "none";
securityProtocolFilter_4.style.display = "none";
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
manageEquip_2.style.display = "none";
manageSyslog.style.display = "none";
manageSyslog_1.style.display = "none";
netDiag.style.display = "none";
networkDiag.style.display = "none";
networkDiag_1.style.display = "none";
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>
	getElement("secuiText").innerHTML="������ȫ";     
	<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) <> "Yes" then %>
		getElement("manText").innerHTML="��������";
	<%else%>
		getElement("appText").innerHTML="����Ӧ��";
		getElement("manText").innerHTML="�ġ�����";
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
<tr><td class="topHelpTitle" colspan="2">��&nbsp;��&nbsp;��&nbsp;��</td></tr>
<tr><td colspan="2">&nbsp;</td></tr>
<tr><td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>һ��״̬<a name=״̬></a></td></tr>
<tr name=statusEquip id=statusEquip >
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.�豸��Ϣ<a name=�豸��Ϣ></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>��ʾ�豸�ͺţ��豸��ʶ�ţ�Ӳ���汾������汾����Ϣ��</li>
</ul>
</td>
</tr>
<tr name=statusNetwork id=statusNetwork >
<td width=20>&nbsp;</td>
<td class="tabdataleft">2.�������Ϣ<a name=�������Ϣ id="�������Ϣ"></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusNetwork_1>��ʾ������Ϣ��PON��Ϣ�� ������Ϣ��ʾ���������״̬������PVC��IP��ַ���������룬Ĭ�����أ�DNS��������Ϣ��PON��Ϣ��ʾPON����·����״̬����·����ͳ�ƣ���ģ����Ϣ�� </li>
</ul>
</td>
</tr>
<tr name=statusUser id=statusUser>
<td width=20>&nbsp;</td>
<td class="tabdataleft">3.�û�����Ϣ<a name=�û�����Ϣ id="�û�����Ϣ"></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusUser_1><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>��ʾWLAN�ӿ���Ϣ����̫���ӿ���Ϣ��  WLAN�ӿ���Ϣ��ʾ������������״̬���ŵ�������SSID��ͳ����Ϣ��SSID����֤��ʽ�ͼ���״̬�ȣ���̫���ӿ���Ϣ��ʾ����IP��ַ��MAC��ַ��ÿ��LAN�ڵ�״̬���շ������ֽ�����<% else %>��̫���ӿ���Ϣ��ʾ����IP��ַ��MAC��ַ��ÿ��LAN�ڵ�״̬���շ������ֽ�����<% end if %></li>
</ul>
</td>
</tr>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
<tr name=statusVoip id=statusVoip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">4.���������Ϣ<a name=���������Ϣ id="���������Ϣ"></a></td>
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusVoip_1>���������Ϣ����ҵ��ע��״̬�͵绰���룻ҵ��״̬������ע���δע���״̬���绰������ʾע��ĵ绰���롣</li>
</ul>
</td>
</tr>
<% end if %>
<tr name=statusACS id=statusACS>
<td width=20>&nbsp;</td>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
<td class="tabdataleft">4.Զ�̹���״̬<a name=Զ�̹���״̬ id="Զ�̹���״̬"></a></td>
<% else %>
<td class="tabdataleft">5.Զ�̹���״̬<a name=Զ�̹���״̬ id="Զ�̹���״̬"></a></td>
<% end if %> 
</tr>
<tr>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li id=statusACS_1>��ʾ�������������ҵ�������·�״̬�������������������ϱ�Inform����Լ�����ITMS�������������</li>
</ul>
</td>
</tr>
<tr name=ntwk id=ntwk>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>��������<a name=����></a></td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft">1.�������<a name=�������></a></td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>Internet����</li>
</ul></td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=ntwk_cfgWan id=ntwk_cfgWan>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>�������:�Կ�����ӽ�����ز�������,������ɺ�,��������ʾ��Ӧ��״̬��</li>
<ul>
<li>WAN����:�������½����ӡ����һ��WAN���ӣ�������ɾ�����ӡ�ɾ�������ӡ�</li>
<li>����:�ø���PVC��Ч��</li>
<li id=ntwk_cfgWan_1>���ð�:���԰Ѹ���LAN�ڡ�����SSID�Ͷ�Ӧ��WAN�ڰ󶨡�</li>
<li>ģʽ:�ֳ�Route��Bridge����ģʽ��Routeģʽ������������ģʽDHCP����ISP���õ�һ��IP��ַ����Static����ISP����һ����̬��IP���㣩��PPPoE��Bridgeģʽ���Խ��豸���óɽ���LAN��ISP֮��������豸��������ʹ���������������ͨ�ž�����ͬһLAN���������ϡ�</li>
<li>���ӷ�ʽ:����ͨ��IP��ʽ�������Ӻ�ͨ��PPP��ʽ�����������֡�����ʵ�ʵ�������Ҫѡ�������ӵķ�ʽ��</li>
<li>IPЭ��汾:��������ʹ�õ�IP�汾��ͨ��ʹ��IPv4�汾��Ҳ����ͨ��ѡ��IPv6��֧��IPv6�����ѡ��IPv4/v6��ʽ����ͬʱʹ��IPv4��IPv6����IP�汾��</li>
<li>PPPoE:ISP���ṩ�ʺţ��������������Ϣ:�û��������롢�������ƣ������ò��ŷ�ʽ����ģʽϵͳ��ͨ��PPPoE��̬���IP�� </li>
<li>DHCP:�豸����ISPͨ��DHCP�Զ����IP�� </li>
<li>Static:��ISP����һ����̬��IP��ַ����Ҫ�������Ϣ����:IP��ַ���������룬��ѡDNS������������DNS�������ȣ�IP��ַ����������ȷ�ĸ�ʽ��4��IP�ֽ�֮����&quot;.&quot;�ŷֿ�(x.x.x.x)������:192.168.0.100��</li>
<li id=ntwk_cfgWan_2>֧��802.1q��802.1p��NAT��IGMP Proxy��</li>
<li id=ntwk_cfgWan_3>�������:UBR Without PCR��UBR With PCR��CBR��Non-Real Time VBR��Realtime VBR��</li>
<li>IPv6 WAN��Ϣ��ȡ��ʽ:����ͨ��DHCP����SLAAC���ַ�ʽ��̬�Ļ�ȡIPv6�ĵ�ַ��Ҳ���Ծ�̬����IPv6�ĵ�ַ�����ݾ������������������á�</li>
</ul>
</ul></td>
</tr>
<tr name=ntwk_cfgBind id=ntwk_cfgBind>
<td>&nbsp;</td>
<td class="tabdataleft">2.������<a name=������ id="������"></a></td>
</tr>
<tr name=ntwk_cfgBind_1 id=ntwk_cfgBind_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>�����ÿ��������û�LAN�ں�WAN�ڵİ󶨷�ʽ��֧�ֶ˿ڰ󶨺Ͷ˿�+VLAN�����ַ�ʽ������Ƕ˿ڰ󶨣���Ӧ�˿ڵ����ݴӰ󶨵�WAN�ڳ�ȥ������Ƕ˿�
	+VLAN�󶨣���Ӧ�˿��Ҵ���VLAN�����ݲŻ�Ӱ󶨵�WAN�ڳ�ȥ���˿ڼ�VLAN����m1/n1 VLAN�Է�ʽ���ã�����m1�����û����VLAN��n1������ӿڵ�VLAN��
	����VLAN���Էֺŷֿ���</li>
</ul></td>
</tr>
<tr name=ntwk_cfgDHCP id=ntwk_cfgDHCP>
<td>&nbsp;</td>
<td class="tabdataleft">3.LAN���ַ����<a name=LAN���ַ���� id="LAN���ַ����"></a></td>
</tr>
<tr name=ntwk_cfgDHCP_1 id=ntwk_cfgDHCP_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IPv4����:Ĭ��Ϊ&quot;����DHCP&quot;��ʹ�øò���,��DHCP��ز�����������,������ɺ󣬽�������ʾ�µ�DHCP״̬������������Լʱ�䣨1���ӡ�1Сʱ��1�졢1�ܣ���DHCP����ģʽ��DHCP 
          Server��DHCP relay����</li>
<li>IPv6����:���������Ƿ�����LAN���RA������LAN���豸����ͨ��RA��ʽ��ȡIPv6�ĵ�ַ��ͬʱҲ����������LAN��ͨ��DHCP��ʽ��ȡIPv6�ĵ�ַ��</li>
</ul></td>
</tr>
<tr name=ntwk_cfgWLAN id=ntwk_cfgWLAN <% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>style="display:none"<% end if %>>
<td>&nbsp; </td>
<script language="JavaScript" type="text/javascript">
if (curUserName == sptUserName)
{
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>
document.write( '<td class="tabdataleft" >4.WLAN����<a name=WLAN���� id="WLAN����"></a></td>');
<% end if %>
}
else if(curUserName == sysUserName)
{
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>
document.write( '<td class="tabdataleft" >1.WLAN����<a name=WLAN���� id="WLAN����"></a></td>');
<% end if %>
}
</script>
</tr>
<tr <% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>style="display:none"<% end if %>>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li id=ntwk_cfgWLAN_1>ʹ������:������ر�WLAN</li>
<li id=ntwk_cfgWLAN_2>ģʽѡ��:��ѡ��<i><b>802.11b</b></i>,<i><b>802.11g</b></i>,<i><b>802.11n</b></i>,<i><b>802.11b/g���</b></i>�Լ�<i><b>802.11b/g/n���</b></i>ģʽ</li>
<li id=ntwk_cfgWLAN_3>�ŵ�ѡ��:�����������ô��б���ѡ��ǡ�����ŵ���Ϊ�����źŸ���,Ӧ��ÿ��<strong><em>AP</em></strong>���䲻ͬ���ŵ�������ʹ���Զ��ŵ�ѡ��ģʽ��</li>
<li id=ntwk_cfgWLAN_4>���͹���ǿ��:�趨������Ƶģ��ʹ�õķ��书�ʡ� </li>
<li id=ntwk_cfgWLAN_5>SSID����: �趨��ǰ�����õ�SSID����š�</li>
<li id=ntwk_cfgWLAN_6>SSID: ����SSID���ơ��������������ַ�,���Ȳ��ܳ���32���ַ�,���ִ�Сд��</li>
<li id=ntwk_cfgWLAN_7>����: ѡ���������ӹ������ʣ����е��Զ����ʸ����ŵ��������Զ�ѡ����ʵ�������ʡ�</li>
<li id=ntwk_cfgWLAN_8>Ƶ�����:ѡ������������802.11nģʽ�µ�Ƶ������</li>
<li id=ntwk_cfgWLAN_9>�������:ѡ������������802.11nģʽ�µı���ʱ������</li>
<li id=ntwk_cfgWLAN_10>SSIDʹ��:ѡ���Ƿ�ʹ�ܵ�ǰ��SSID��</li>
<li id=ntwk_cfgWLAN_11>�㲥ȡ��:�Ƿ�ʹ�ܶ�ӦSSID�Ĺ㲥ȡ�����ܣ����ʹ�ܣ������ز������㲥SSID��</li>
<li id=ntwk_cfgWLAN_12>��ȫ����:�ɶ�SSID���ð�ȫ����������֧�ֿ���ϵͳ��WEP��WPA-PSK��WPA2-PSK��WPA-PSK/WPA2-PSK����֤��ʽ�Ͷ�Ӧ�ļ��ܷ�ʽ��</li>
<li id=ntwk_cfgWLAN_13>WPA Ԥ��֤������Կ:���ù�����Կ��</li>
<li id=ntwk_cfgWLAN_14>WPA ����:ѡ����ܷ�����</li>
</ul></td>
</tr>
<tr name=ntwk_cfgRemoteManage id=ntwk_cfgRemoteManage>
<td>&nbsp;</td>
<td class="tabdataleft" name='ntwk_cfgRM01' id='ntwk_cfgRM01'><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>5<% else %>4<% end if%>.Զ�̹���<a name=Զ�̹��� id="Զ�̹���"></a></td>
</tr>
<tr name=ntwk_cfgRemoteManage_1 id=ntwk_cfgRemoteManage_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=ntwk_cfgRemoteManage_2 id=ntwk_cfgRemoteManage_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>ITMS����������</li>
<li>OLT��֤</li>
</ul></td>
</tr>
<tr name=ntwk_cfgRemoteManage_3 id=ntwk_cfgRemoteManage_3>
<td>&nbsp;</td>
<td class="tabdataleft" name='ntwk_cfgRM03' id='ntwk_cfgRM03'>&nbsp;(2).����</td>
</tr>
<tr name=ntwk_cfgRemoteManage_4 id=ntwk_cfgRemoteManage_4>
<td>&nbsp;</td>
<td class="tabdataleft" name='ntwk_cfgRM04' id='ntwk_cfgRM04'><ul>
<li>��������ITMS��������URL����IP��ַ��ITMS��֤���ص��û��������룬�Լ�������֤ITMS���û��������롣ͬʱ���������������Ƿ����������ϱ����ģ��Լ��ϱ����ĵ����ڡ����⻹���Խ���֤����֤�����û���á�</li>
<li>OLT��֤ע��</li>
</ul></td>
</tr>
<tr name=ntwk_cfgQoS id=ntwk_cfgQoS>
<td>&nbsp;</td>
<td class="tabdataleft"><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>6<% else %>5<% end if%>.QoS<a name=QoS id="QoS"></a></td>
</tr>
<tr name=ntwk_cfgQoS_1 id=ntwk_cfgQoS_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>QoS ���ã�������ά���ʺſ�ʹ�ã�:����ʱ��Ԥ�ú�ҵ����ص�����ģ�壬ʹ��ʱ���ݾ��忪ͨҵ�����ѡ������á�</li>
</ul></td>
</tr>
<tr name=ntwk_cfgSNTP id=ntwk_cfgSNTP>
<td>&nbsp;</td>
<td class="tabdataleft"><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>7<% else %>6<% end if%>.ʱ�����<a name=ʱ����� id="ʱ�����"></a></td>
</tr>
<tr name=ntwk_cfgSNTP_1 id=ntwk_cfgSNTP_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=ntwk_cfgSNTP_2 id=ntwk_cfgSNTP_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>�Ƿ�����</li>
</ul></td>
</tr>
<tr name=ntwk_cfgSNTP_3 id=ntwk_cfgSNTP_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=ntwk_cfgSNTP_4 id=ntwk_cfgSNTP_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul><li>SNTP ������ʱ��Э�飩 </li>
<ul>
<li>SNTP����:Ĭ��Ϊ&quot;<strong>������SNTP</strong>&quot;������SNTP����SNTP����ز����������ã�������ɺ󣬽�������ʾ�µ�SNTP���á�</li>
<li>������������������Ҳ������IP��ַ�������ѡ���б�������SNTP�����������б����ѡ��Other�����ں�������SNTP��������������IP��ַ�� </li>
<li>ʱ�������б��������ѡ�����ڵص�ʱ������</li>
</ul>
</ul></td>
</tr>
<tr name=ntwk_cfgRoute id=ntwk_cfgRoute>
<td>&nbsp;</td>
<td class="tabdataleft"><% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>8<% else %>7<% end if%>.·������<a name=·������ id="·������"></a></td>
</tr>
<tr name=ntwk_cfgRoute_1 id=ntwk_cfgRoute_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=ntwk_cfgRoute_2 id=ntwk_cfgRoute_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>��̬·����ʾ</li>
</ul></td>
</tr>
<tr name=ntwk_cfgRoute_3 id=ntwk_cfgRoute_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=ntwk_cfgRoute_4 id=ntwk_cfgRoute_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>��̬·��:����Ŀ�������ַ���������롢���ص�ַ�ͽӿڡ��������ص�ַ�ͽӿ�����ѡ��һ�</li>
</ul></td>
</tr>
<tr>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC id="secuiText">������ȫ<a name=��ȫ></a></td>
</tr>
<tr name=securityURL id=securityURL>
<td>&nbsp;</td>
<td class="tabdataleft">1.��������������<a name=�������������� id="��������������"></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>�Ƿ�����</li>
<li>��ǰ�Ĺ���ģʽ</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>����</li>
<ul>
<li>���ù���������:������رչ��������ʹ��ܣ�Ĭ��Ϊ��<strong>�ر�</strong>����������ɺ󣬽�������ʾ�µĹ���������״̬�� </li>
<li>����ģʽ:��<b>��������</b>��<b>��������</b>������������ģʽ���޸Ĺ��˵�ǰ�Ĺ���ģʽ��</li>
<li>URL:������ȷ��URL�����һ��URL�������б��С�</li>
</ul>
<li>�����б�:��ʾ�Ѿ����˵�URL�б�</li>
<li>��ǰ�Ĺ����б�Ϊ������ʱ�������б��е�URL���󽫱����ˣ���ǰ�����б�Ϊ������ʱ��ֻ�й����б��е�URL���󱻽��ܡ�</li>
</ul></td>
</tr>
<tr name=securityFirewall id=securityFirewall>
<td>&nbsp;</td>
<td class="tabdataleft">2.����ǽ<a name=����ǽ></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li id=securityFirewall_1>����ǽ����:��ȫ��������</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>��ȫ��������:ʹ�øò��������Խ�����ǽ�ȼ�����Ϊ"<b>��</b>"��"<b>��</b>"��"<b>��</b>"��������ɺ󣬽�������ʾ�µķ���ǽ״̬��</li>
</ul></td>
</tr>
<tr name=securityMAC id=securityMAC>
<td>&nbsp;</td>
<td class="tabdataleft">3.MAC����<a name=MAC����></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>�Ƿ�����</li>
<li>��ǰ�Ĺ���ģʽ</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>����</li>
<ul>
<li>���ù�����MAC����:������رչ��������ʹ��ܣ�Ĭ��Ϊ��<strong>�ر�</strong>����������ɺ󣬽�������ʾ�µĹ���������״̬�� </li>
<li>����ģʽ:��<b>��������</b>��<b>��������</b>���˲����޸ĵ�ǰ�Ĺ���ģʽ��</li>
<li>MAC��ַ:������ȷ��MAC��ַ�����һ��MAC��ַ�������б��С�</li>
</ul>
<li>�����б�:��ʾ�Ѿ����˵�MAC��ַ�б�</li>
<li>��ǰ�Ĺ����б�Ϊ������ʱ�����й����б��е�MAC��ַ��PC���޷�����Internet���磻��ǰ����ģʽΪ������ʱ�����й����б��е�MAC��ַ��PC�ſ��Է���Internet���磻�����Ǻ��������ǰ�������LAN���PC�����Է������ء�</li>
</ul></td>
</tr>
<tr name=securityPort id=securityPort>
<td>&nbsp;</td>
<td class="tabdataleft">4.�˿ڹ���<a name=�˿ڹ���></a></td>
</tr>
<tr name=securityPort_1 id=securityPort_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=securityPort_2 id=securityPort_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>�Ƿ�����</li>
<li>��ǰ�Ĺ���ģʽ</li>
</ul></td>
</tr>
<tr name=securityPort_3 id=securityPort_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=securityPort_4 id=securityPort_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>����</li>
<ul>
<li>���ù������˿ڹ���:������رչ��������ʹ��ܣ�Ĭ��Ϊ��<strong>�ر�</strong>����������ɺ󣬽�������ʾ�µĹ���������״̬�� </li>
<li>����ģʽ:��<b>��������</b>��<b>��������</b>���˲����޸ĵ�ǰ�Ĺ���ģʽ��</li>
<li>����ӡ�:���һ�����˹��򵽹�����������ɾ������ɾ��һ�������ѡ�еĹ��˹���</li>
</ul>
<li>�����б�:��ʾ��ǰ�Ĺ�������б�</li>
<li>��ǰ�Ĺ���ģʽΪ������ʱ�����Ϲ����б�����һ����������ݰ����޷�ͨ�����أ���ǰ����ģʽΪ������ʱ�����Ϲ����б��й�������ݰ��ſ���ͨ�����أ������Ǻ��������ǰ�����,LAN���PC�����Է������ء�</li>
<li>���˹�������á��������ơ����ɽ���Э�顱����ԴIP��ʼ��ַ������ԴIP������ַ������Դ�˿ڡ�����Ŀ��IP��ʼ��ַ������Ŀ��IP������ַ������Ŀ�Ķ˿ڡ���Ϊ���������������á� </li>
</ul></td>
</tr>
<tr name=securityProtocolFilter id=securityProtocolFilter>
	<td>&nbsp;</td>
	<td class="tabdataleft">5.Э�����<a name=Э�����></a></td>
	</tr>
	<tr name=securityProtocolFilter_1 id=securityProtocolFilter_1>
	<td>&nbsp;</td>
	<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
	</tr>
	<tr name=securityProtocolFilter_2 id=securityProtocolFilter_2>
	<td>&nbsp;</td>
	<td class="tabdataleft"><ul>
	<li>��ǰ�˿ڶ�Ӧ�Ĺ���Э��</li>
	</ul></td>
	</tr>
	<tr name=securityProtocolFilter_3 id=securityProtocolFilter_3>
	<td>&nbsp;</td>
	<td class="tabdataleft">&nbsp;(2).����</td>
	</tr>
	<tr name=securityProtocolFilter_4 id=securityProtocolFilter_4>
	<td>&nbsp;</td>
	<td class="tabdataleft"><ul>
	<li>������Ҫ���˵�Э�����ͣ�ƥ���Э�����ͱ��Ľ�������</li>
	</ul></td>
</tr>
<tr id="apptitle">
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC id="appText">�ġ�Ӧ��<a name=Ӧ��></a></td>
</tr>
<tr name=appDDNS id=appDDNS>
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.DDNS����<a name=DDNS���� id="DDNS����"></a></td>
</tr>
<tr name=appDDNS_1 id=appDDNS_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=appDDNS_2 id=appDDNS_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>��̬����</li>
<ul>
<li>DDNS����</li>
</ul>
</ul>	 		</td>
</tr>
<tr name=appDDNS_3 id=appDDNS_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=appDDNS_4 id=appDDNS_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>���ÿ���:ʹ�øò���,������ر�DDNS����,������ɺ�,��������ʾ�µ�DDNS״̬.</li>
<li>DDNS����:ʹ�øò���,��DDNS��ز�����������,������ɺ�,��������ʾ�µ�DDNS״̬.</li>
</ul>	 	</td>
</tr>
<tr name=appAdnat id=appAdnat>
<td width=20>&nbsp;</td>
<td class="tabdataleft">2.�߼�NAT����<a name=�߼�NAT���� id="�߼�NAT����"></a></td>
</tr>
<tr name=appAdnat_1 id=appAdnat_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=appAdnat_2 id=appAdnat_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>ALG����</li>
<ul>
<li>L2TP ALG״̬</li>
<li>IPSec ALG״̬</li>
<li>H.323 ALG״̬</li>
<li>RTSP ALG״̬</li>
<li>SIP ALG״̬</li>
<li>FTP ALG״̬</li>
<li>PPTP ALG״̬</li>
</ul>
<li>DMZ����</li>
<ul type="circle">
<li>DMZ����״̬</li>
</ul>
</ul>	 	</td>
</tr>
<tr name=appAdnat_3 id=appAdnat_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=appAdnat_4 id=appAdnat_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>ALG���ÿ���:ʹ�øò�����������رո���ALG���ܣ�������ɺ󣬽�������ʾ�µĸ���ALG״̬��</li>
<li>DMZ����:ʹ�øò�������DMZ��ز����������ã�������ɺ󣬽�������ʾ�µ�DMZ״̬��</li>
</ul>	 	</td>
</tr>
<tr name=appUPNP id=appUPNP>
<td width=20>&nbsp;</td>
<td class="tabdataleft">3.UPNP����<a name=UPNP���� id="UPNP����"></a></td>
</tr>
<tr name=appUPNP_1 id=appUPNP_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=appUPNP_2 id=appUPNP_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>��ʾ������UPnP�Ƿ����á�</li>
</ul>	 	</td>
</tr>
<tr name=appUPNP_3 id=appUPNP_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=appUPNP_4 id=appUPNP_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>ͨ����ѡ������UPnP��ʹ��״̬��ѡ�и�ѡ����ʹUPnPҵ����Ч������UPnPҵ�񽫽��á�</li>
</ul>	 	</td>
</tr>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
<tr name=appVoip id=appVoip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">4.����绰����<a name=����绰���� id="����绰����"></a></td>
</tr>
<tr name=appVoip_1 id=appVoip_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=appVoip_2 id=appVoip_2>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>����绰�������á�</li>
<li>����绰�߼����á�</li>
</ul>	 	</td>
</tr>
<tr name=appVoip_3 id=appVoip_3>
<td width=20>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=appVoip_4 id=appVoip_4>
<td width=20>&nbsp;</td>
<td class="tabdataleft">
<ul>
<li>�޸�SIP�����������Ӧ�á���ť����������á� </li>
<li>Outbound��������ַ��ISP�ṩ���ⷢ����������ĵ�ַ�����Ϊ�ջ���Ϊ0.0.0.0���ⷢ����������������á�</li>
<li>Outbound�������˿ںţ�SIP�ⷢ�����������UDP�˿ڣ��������Ϊ�ջ���0����ʹ��Ĭ��ֵ5060��</li>
<li>����Outbound��������ַ��ISP�ṩ�ı����ⷢ����������ĵ�ַ�����Ϊ�ջ���Ϊ0.0.0.0�������ⷢ����������������á�</li>
<li>����Outbound�������˿ںţ�SIP�����ⷢ�����������UDP�˿ڣ��������Ϊ�ջ���0����ʹ��Ĭ��ֵ5060��</li>
<li>���÷�������ַ��ISP�ṩ��SIP����������ĵ�ַ�����Ϊ�ջ���Ϊ0.0.0.0��SIP����������������á�</li>
<li>���ö˿ںţ�SIP�����������UDP�˿ڣ��������Ϊ�ջ���0����ʹ��Ĭ��ֵ5060��</li>
<li>���÷�������ַ��ISP�ṩ��SIP���ô���������ĵ�ַ�����Ϊ�ջ���Ϊ0.0.0.0��SIP���ô���������������á�</li>
<li>���ö˿ںţ�SIP���ô����������UDP�˿ڣ��������Ϊ�ջ���0����ʹ��Ĭ��ֵ5060��</li>
<li>���ض˿ڣ� ����SIP��UDP�˿ںţ�Ĭ��Ϊ5060��</li>
<li>��ͼ�� ָʾ����ҵ��Ĳ��ŷ�����</li>
<li>��ͼƥ��ģʽ���������ƥ�����Сƥ�䡣</li>
<li>���Ҽ�������ָʾ��ǰ����ҵ��Ĺ��Һ͵�����</li>
<li>����˿ڣ�ָʾ�ں�������ѡ�������˿ڡ�</li>
<li>ý��˿ڣ�ָʾ�ں���ý��ѡ�������˿ڡ�</li>
</ul>	 	</td>
</tr>
<% end if %>

<tr name=appIGMP id=appIGMP>
<td>&nbsp;</td>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
<td class="tabdataleft">4.IGMP/MLD����<a name=IGMP���� id="IGMP����"></a></td>
<% else %>
<td class="tabdataleft">5.IGMP/MLD����<a name=IGMP���� id="IGMP����"></a></td>
<% end if %> 
</tr>
<tr name=appIGMP_1 id=appIGMP_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=appIGMP_2 id=appIGMP_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IGMP/MLD Snooping״̬</li>
<li>IGMP/MLD Proxy״̬</li>
</ul></td>
</tr>
<tr name=appIGMP_3 id=appIGMP_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=appIGMP_4 id=appIGMP_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IGMP/MLD Snooping����:ʹ�øò��������û���ͣ��IGMP/MLD Snooping���ܣ�������ɺ󣬽�������ʾIGMP/MLD Snooping����״̬��</li>
<li>IGMP/MLD Proxyʹ������:ʹ�øò��������û���ͣ��IGMP/MLD Proxy���ܣ�������ɺ󣬽�������ʾIGMP/MLD Proxyʹ��״̬��</li>
<li>IGMP/MLD Proxy����:ʹ�øò��������û���ͣ��ÿ��WAN����IGMP/MLD Proxy���ܣ�������ɺ󣬽�������ʾÿ��WAN���ӵ�IGMP/MLD Proxy����״̬��</li>
</ul></td>
</tr>
<tr name=appDaily id=appDaily>
<td>&nbsp;</td>
<script language="JavaScript" type="text/javascript">
if (curUserName == sptUserName)
{
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
document.write( '<td class="tabdataleft" >5.�ճ�Ӧ��<a name=�ճ�Ӧ�� id="�ճ�Ӧ��"></a></td>');
<% else %>
document.write( '<td class="tabdataleft" >6.�ճ�Ӧ��<a name=�ճ�Ӧ�� id="�ճ�Ӧ��"></a></td>');
<% end if %> 
}
else if(curUserName == sysUserName)
{
document.write( '<td class="tabdataleft" >1.�ճ�Ӧ��<a name=�ճ�Ӧ�� id="�ճ�Ӧ��"></a></td>');
}
</script>
</tr>
<tr name=appDaily_1 id=appDaily_1>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(1).״̬��ʾ</td>
</tr>
<tr name=appDaily_2 id=appDaily_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
<li>FTP�ͷ���������Ϣ</li>
<% end if %>
<li id=appDaily_5>IPTV������Ϣ</li>
</ul></td>
</tr>
<tr name=appDaily_3 id=appDaily_3>
<td>&nbsp;</td>
<td class="tabdataleft">&nbsp;(2).����</td>
</tr>
<tr name=appDaily_4 id=appDaily_4>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
<li>ͨ��FTP�ͷ��˹��ܣ����������������ʲ����ն˵�USB�豸�е��ļ���ͬʱ�����û�����ָ�����ļ����ն˵�USB�豸�С�</li>
<% end if %>
<li id=appDaily_6>��������WAN���鲥VLAN��</li>
</ul></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"></td>
</tr>
<tr>
<script language="JavaScript" type="text/JavaScript">
if (curUserName == sysUserName)
	document.write('<td id=manText class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>�塢����<a name=����></a></td>');
else
	document.write('<td id=manText class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>�塢����<a name=����></a></td>');
</script>
</tr>
<tr name=manageUser id=manageUser>
<td>&nbsp;</td>
<td class="tabdataleft">1.�û�����<a name=�û�����></a></td>
</tr>
<tr name=manageUser_1 id=manageUser_1>
<td>&nbsp;</td>
<td class="tabdataleft" >&nbsp;(1)����ά���˺�</td>
</tr>
<tr name=manageUser_2 id=manageUser_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>��������:ʹ�õ���ά���ʺŵ�¼�󣬿����޸��û������ʺŵ����룬������ҪУ��ԭ���롣</li>
</ul></td>
</tr>
<tr name=manageUser_3 id=manageUser_3>
<td>&nbsp;</td>
      <td class="tabdataleft">&nbsp;(2)�û������ʺ�</td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
          <li>�û������ʺź���������:ʹ���û������ʺŵ�¼��,���޸��û������ʺŵ����룬��ҪУ��ԭ���롣 </li>
        </ul></td>
</tr>
<tr name=manageEquip id=manageEquip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">2.�豸����<a name=�豸���� id="�豸����"></a></td>
</tr>
<tr>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>�豸����:ʹ�øò���,�����������ء�</li>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
<li id=manageEquip_USB>USB���������ļ�:����USB�豸���ӵ��ն���,���"��������"�Ϳ��Խ���ǰ�����ļ����ݵ�USB�豸�С������������USB�Զ��ָ������ļ����ܣ���USB�豸�����е������ļ����뵽�ն�ϵͳ�С�</li>
<% end if %>
<li id=manageEquip_LOG style="display:none">�����־��¼:ʹ�øò���,�����ǰ������־��</li>
<li id=manageEquip_2>�ָ���������:ʹ�øò���,�����ػָ�������ʱ��Ĭ�����á�</li>
</ul></td>
</tr>
<tr name=manageSyslog id=manageSyslog>
<td>&nbsp;</td>
<td class="tabdataleft">3.��־�ļ�����<a name=��־�ļ����� id="��־�ļ�����"></a></td>
</tr>
<tr name=manageSyslog_1 id=manageSyslog_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul><li>��־д��ȼ�����:�������û��߽���д����־�ļ���������д����־�ļ�������£���������д�����ʾ��־�ļ��ĵȼ���</li>
<li>��־:��ʾ��ǰ�ȼ��µ���־���ݡ�</li>
<li>ά�������ϱ�:������ά����Ա��ά����ʹ���˵���ά���ʺ�ʱ��ͨ���ý������֪ͨITMS���µ���ά���ʺŵ����롣</li>
</ul></td>
</tr>
<tr name=netDiag id=netDiag>
<td id=diagText class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>�������<a name=���></a></td>
</tr>
<tr name=networkDiag id=networkDiag>
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.�������<a name=������� id="�������"></a></td>
</tr>
<tr name=networkDiag_1 id=networkDiag_1>
<td width=20>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>PING����:֧����ѡ����WAN������ping���ԣ�Ŀ�ĵ�ַ֧��IP��ַ���������ַ�ʽ��</li>
<li>Tracert����:֧����ѡ����WAN������Tracert���ԣ�Ŀ�ĵ�ַ֧��IP��ַ���������ַ�ʽ��</li>
<li>Inform�ֶ��ϱ�:֧��Inform�ֶ��ϱ���������ȷ��ʾ�ֶ�Inform�ϱ���״̬��</li>
</ul>	 	</td>
</tr>
</table>




</form>
</body>
</html>
