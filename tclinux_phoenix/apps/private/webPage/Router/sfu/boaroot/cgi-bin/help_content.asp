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
statusVoip.style.display = "none";
statusVoip_1.style.display = "none";
apptitle.style.display = "none";
networktitle.style.display = "none";
getElement('statusUser_1').innerText ='<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>��ʾWLAN�ӿ���Ϣ��<% end if %>��̫���ӿ���Ϣ<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>��USB�ӿ���Ϣ<% end if %>��<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>WLAN�ӿ���Ϣ��ʾ������������״̬��SSID_1����֤��ʽ�ͼ���״̬�ȣ�<% end if %>��̫���ӿ���Ϣ��ʾ����IP��ַ��MAC��ַ��ÿ��LAN�ڵ�״̬��<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>USB�ӿ���Ϣ��ʾ����USB�����豸����Ϣ�� <% end if %>';

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
<tr id='networktitle'>
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
<li>�������:���������ӽ�����ز�������,������ɺ�,��������ʾ��Ӧ��״̬��</li>
<ul>
<li>DHCP:�豸����ISPͨ��DHCP�Զ����IP�� </li>
<li>Static:��ISP����һ����̬��IP��ַ����Ҫ�������Ϣ����:IP��ַ���������룬ȱʡ���ء�</li>
<li>PPPoE:ISP���ṩ�ʺţ��������������Ϣ:�û��������롣��ģʽϵͳ��ͨ��PPPoE��̬���IP�� </li>
</ul>
</ul></td>
</tr>

<tr name=ntwk_cfgDHCP id=ntwk_cfgDHCP>
<td>&nbsp;</td>
<td class="tabdataleft">2.LAN���ַ����<a name=LAN���ַ���� id="LAN���ַ����"></a></td>
</tr>
<tr name=ntwk_cfgDHCP_1 id=ntwk_cfgDHCP_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>IPv4����:ʹ�øò���,��LAN��ز����������á�</li>
</ul></td>
</tr>
<tr name=ntwk_cfgloidset id=ntwk_cfgloidset>
<td>&nbsp;</td>
<td class="tabdataleft">3.LOID���ù���<a name=LOID���ù��� id="LOID���ù���"></a></td>
</tr>
<tr name=ntwk_cfgloidset_1 id=ntwk_cfgloidset_1>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>ͨ������LOID��Password�������ܷ�����ע�Ტ����ҵ�����á�</li>
</ul></td>
</tr>
<tr name=ntwk_cfgSNTP id=ntwk_cfgSNTP>
<td>&nbsp;</td>
<td class="tabdataleft">4.ʱ�����<a name=ʱ����� id="ʱ�����"></a></td>
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

<tr id="apptitle">
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>����Ӧ��<a name=Ӧ��></a></td>
</tr>
<tr name=appVoip id=appVoip>
<td width=20>&nbsp;</td>
<td class="tabdataleft">1.����绰����<a name=����绰���� id="����绰����"></a></td>
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

<tr>
<td>&nbsp;</td>
<td class="tabdataleft"></td>
</tr>
<tr>
<script language="JavaScript" type="text/JavaScript">
if (curUserName == sysUserName)
	document.write('<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>��������<a name=����></a></td>');
else
	document.write('<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>�ġ�����<a name=����></a></td>');
</script>
</tr>
<tr name=manageUser id=manageUser>
<td>&nbsp;</td>
<td class="tabdataleft">1.�û�����<a name=�û�����></a></td>
</tr>
<tr name=manageUser_1 id=manageUser_1>
<td>&nbsp;</td>
<td class="tabdataleft" >&nbsp;(1)��ͨά���˺�</td>
</tr>
<tr name=manageUser_2 id=manageUser_2>
<td>&nbsp;</td>
<td class="tabdataleft"><ul>
<li>��������:ʹ����ͨά���ʺŵ�¼�󣬿����޸��û������ʺŵ����룬������ҪУ��ԭ���롣</li>
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
</ul></td>
</tr>
<tr name=netDiag id=netDiag>
<td class="tabHelpdata" colspan=2 bgcolor=#CCCCCC>�塢���<a name=���></a></td>
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
</ul>	 	</td>
</tr>
</table>

</form>
</body>
</html>
