<%		  
if Request_Form("alg_Flag") = "1" then
	TCWebApi_set("ALGSwitch_Entry","L2TPSW","L2TPSW_Flag")
	TCWebApi_set("ALGSwitch_Entry","IPSECSW","IPSecSW_Flag")
	TCWebApi_set("ALGSwitch_Entry","H323SW","H323SW_Flag")
	TCWebApi_set("ALGSwitch_Entry","RTSPSW","RTSPSW_Flag")
	TCWebApi_set("ALGSwitch_Entry","SIPSW","SIPSW_Flag")
	TCWebApi_commit("ALGSwitch_Entry")
	tcWebApi_save()
end if

if Request_Form("DMZ_Flag") = "1" then
	TCWebApi_set("WebCurSet_Entry","nat_pvc","DMZEntryCurIndex")
	TCWebApi_commit("WebCurSet_Entry")
	TCWebApi_set("Dmz_Entry","IFName","DMZInterface")
	TCWebApi_set("Dmz_Entry","Active","EnableDMZ_Flag")
	TCWebApi_set("Dmz_Entry","DMZ_IP","DMZHostIPAddress")
	TCWebApi_commit("Dmz_Entry")
	tcWebApi_save()
end if

if Request_Form("VirServer_Flag") = "1" then
	TCWebApi_set("WebCurSet_Entry","nat_pvc","VirServEntryCurIndex")
	TCWebApi_set("WebCurSet_Entry","virServ_id","VirServCurIndex")
	TCWebApi_commit("WebCurSet_Entry")
	TCWebApi_unset("VirServer_Entry_Entry")
	TCWebApi_commit("VirServer_Entry_Entry")	
	TCWebApi_set("VirServer_Entry_Entry","Active","EnableVirServer_Flag")
	TCWebApi_set("VirServer_Entry_Entry","Protocal","PortMappingProtocol")
	TCWebApi_set("VirServer_Entry_Entry","SrcIP","RemoteHost")
	TCWebApi_set("VirServer_Entry_Entry","STARTPORT","ExternalPort")
	TCWebApi_set("VirServer_Entry_Entry","ENDPORT","InternalPort")
	TCWebApi_set("VirServer_Entry_Entry","LOCALIP","InternalClient")
	TCWebApi_set("VirServer_Entry_Entry","MapName","PortMappingDescription")	
	TCWebApi_commit("VirServer_Entry_Entry")
	tcWebApi_save()
elseif Request_Form("VirServer_Flag") = "2" then
	TCWebApi_set("WebCurSet_Entry","nat_pvc","VirServEntryCurIndex")	
	if Request_Form("hVirServEntry0") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry0")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if
	if Request_Form("hVirServEntry1") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry1")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if
	if Request_Form("hVirServEntry2") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry2")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if
	if Request_Form("hVirServEntry3") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry3")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if
	if Request_Form("hVirServEntry4") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry4")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if
	if Request_Form("hVirServEntry5") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry5")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if
	if Request_Form("hVirServEntry6") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry6")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if
	if Request_Form("hVirServEntry7") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry7")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if	
	if Request_Form("hVirServEntry8") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry8")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if	
	if Request_Form("hVirServEntry9") <> "99" then
		TCWebApi_set("WebCurSet_Entry","virServ_id","hVirServEntry9")
		TCWebApi_unset("VirServer_Entry_Entry")		
	end if											
	TCWebApi_commit("WebCurSet_Entry")
	TCWebApi_commit("VirServer_Entry_Entry")
	tcWebApi_save()
elseif Request_Form("VirServer_Flag") = "3" then
	TCWebApi_set("WebCurSet_Entry","nat_pvc","VirServEntryCurIndex")								
	TCWebApi_commit("WebCurSet_Entry")
end if
%>
<HTML><HEAD><TITLE>�豸״̬</TITLE>
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
                <TD class=welcom vAlign=bottom align=middle width=120>��ӭ���� </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>�˳� 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=���� color=#ffffff><B><FONT face=���� 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="Ӧ��->�߼�NAT����" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33>
            <P class=GatewayType align=center>�ͺ�:
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
	var VirServEntryIndex = 0;
	var WanInfo = new Array();
	var nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
	var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryName = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryValue = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryIndex = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vActive = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vNATENABLE = vArrayStr.split(',');

	nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","DmzCycleValue","s") %>";
	var vDMZActive = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","DmzCycleValue","s") %>";
	var vDMZIP = vArrayStr.split(',');
		
	for (i = 0; i < nEntryNum; i++)
	{
		WanInfo[i] = new stWanInfo(vEntryValue[i],vEntryName[i],vActive[i],vNATENABLE[i],vEntryName[i],vEntryName[i],vDMZActive[i],vDMZIP[i]);
	}
	
function stAlg(domain,L2TPEnable,IPSECEnable,H323Enable,RTSPEnable,SIPEnable,SIPPort,SIPWanPath)
{
	this.domain = domain;
	if(L2TPEnable == "on")
		this.L2TPEnable = 1;
	else
		this.L2TPEnable = 0;
	if(IPSECEnable == "on")
		this.IPSECEnable = 1;
	else
		this.IPSECEnable = 0;
	if(H323Enable == "on")
		this.H323Enable = 1;
	else
		this.H323Enable = 0;
	if(RTSPEnable == "on")
		this.RTSPEnable = 1;
	else
		this.RTSPEnable = 0;
	if(SIPEnable == "on")
		this.SIPEnable = 1;
	else
		this.SIPEnable = 0;
	this.SIPPort = SIPPort;
	if(SIPWanPath == "N/A")
		this.SIPWanPath = "";
	else
		this.SIPWanPath = SIPWanPath;	
}
var algArray = 
new Array
(
	new stAlg("domain","<% tcWebApi_get("ALGSwitch_Entry","L2TPSW","s") %>","<% tcWebApi_get("ALGSwitch_Entry","IPSECSW","s") %>","<% tcWebApi_get("ALGSwitch_Entry","H323SW","s") %>","<% tcWebApi_get("ALGSwitch_Entry","RTSPSW","s") %>","<% tcWebApi_get("ALGSwitch_Entry","SIPSW","s") %>","5060","N/A"),
	null
);
var alg = algArray[0];
var WanIndex = -1;
function stWanInfo(domain,Name,Enable,NATEnabled,ConnectionType,ServiceList,DMZEnable,DMZIP)
{
	this.domain = domain;
	this.key = this.domain;
	this.Name = Name;
	this.ConnectionType = ConnectionType;
	this.ServiceList = ServiceList;
	if(DMZEnable == "Yes")
		this.DMZEnable = 1;
	else
		this.DMZEnable = 0;
	this.DMZIP = DMZIP;
	if(Enable == "Yes")
		this.Enable = 1;
	else
		this.Enable = 0;
	if(NATEnabled == "Enable")
		this.NATEnabled = 1;
	else
		this.NATEnabled = 0;		
	this.CntStatus = '';
}

function stPortMap(domain,ProtMapEnabled,Protocol,RemoteHost,ExPort,InPort,InClient,Description)
{
	domain = ""+domain;	
	this.domain = domain;
	this.key = this.domain;
	this.ProtMapEnabled = ProtMapEnabled;
	this.Protocol = Protocol;
	this.RemoteHost = RemoteHost;
	this.ExPort = ExPort;
	this.InPort = InPort;
	this.InClient = InClient;
	this.Description = Description;
	var index = domain.lastIndexOf('PortMapping');
	this.interface = domain.substr(0,index - 1);
	this.interface = domain;
	this.NATEnabled = '';
	this.Enable = '';
}

var PortMapping = new Array();
var indexPortMapping = 0;
var MAX_VIRSERVER_NUM = 10;

<%if Request_Form("VirServer_Flag") = "3" then %>
nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
vArrayStr = "<% tcWebApi_get("WanInfo_Common","VirServerCycleValue","s") %>";
var vNatActive = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","VirServerCycleValue","s") %>";
var vNatProtocal = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","VirServerCycleValue","s") %>";
var vNatSrcIP = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","VirServerCycleValue","s") %>";
var vNatSTARTPORT = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","VirServerCycleValue","s") %>";
var vNatENDPORT = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","VirServerCycleValue","s") %>";
var vNatLOCALIP = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","VirServerCycleValue","s") %>";
var vNatMapName = vArrayStr.split(',');

for (j = 0; j < MAX_VIRSERVER_NUM; j++)
{
	if(vNatActive[j] == "Yes")
	{
		PortMapping[indexPortMapping] = new stPortMap(vEntryValue[<% tcWebApi_get("WebCurSet_Entry","nat_pvc","s") %>],"1",vNatProtocal[j],vNatSrcIP[j],vNatSTARTPORT[j],vNatENDPORT[j],vNatLOCALIP[j],vNatMapName[j]);
		indexPortMapping++;
	}
}
<%end if%>

if (PortMapping.length > 0)
{
	AssociateParam('PortMapping','WanInfo','NATEnabled|Enable|id');
}
function AddSubmitParam(SubmitForm,type)
{
	switch (type)
	{
	case 1:
		SubmitForm.addForm('algForm',alg.domain);
		SubmitForm.setAction('set.cgi?RequestFile=html/application/adnat.asp');
		break;
	case 2:
		var Interface = getElById('DMZInterface');
		var index = Interface.selectedIndex;
		var DMZId = Interface.options[index].id;
		index = DMZId.substr(DMZId.indexOf('_') + 1);
		setDisable('DMZInterface',1);
		SubmitForm.addForm('dmzForm','x');
		SubmitForm.setAction('setcfg.cgi?x=' + WanInfo[index].domain + '.X_ATP_DMZ'
		+ '&RequestFile=html/application/adnat.asp');
		break;
	case 3:
		var interface = getSelectVal('PortMappingInterface');
		setDisable('PortMappingInterface',1);
		SubmitForm.addForm('PortMappingForm','x');
		SubmitForm.setAction('addcfg.cgi?x=' + interface + '.PortMapping'
		+'&RequestFile=html/application/adnat.asp');
		break;
	case 4:
		RemovePortMapping(SubmitForm,'rml');
		SubmitForm.setAction('del.cgi?RequestFile=html/application/adnat.asp');
		break;
	}
}
function hidesipPortInfo(hide)
{
	var status = 'visible';
	if ( hide == 1 )
	status = 'hidden';
	if (document.getElementById)  
	{
		document.getElementById('sipproxyport').style.visibility = status;
	}
	else
	{
		if (document.layers == false) 
		getElement('sipproxyport').style.visibility = status;
	}
}
function hideSiproxdInfo(hide) {
	var status = 'visible';
	if ( hide == 1 )
	status = 'hidden';
	if (document.getElementById)  
	{
		document.getElementById('sipproxdInfo').style.visibility = status;
		document.getElementById('sipproxyport').style.visibility = status;
		document.getElementById('applybutton').style.visibility = status;
	}
	else {
		if (document.layers) { 
			if ( hide == 1 )
				form1.Siproxd.value = '';
			else
				form1.Siproxd.value = enblSiproxd;
		}
		else  
		{
			getElement('sipproxdInfo').style.visibility = status;
			getElement('sipproxyport').style.visibility = status;
			getElement('applybutton').style.visibility = status;
		}
	}
}
function dmzClick()
{
	if(getCheckVal('DMZEnable') == 0)
	{
		setText('DMZHostIPAddress', "");
	}
}
function sipClick()
{
	//setVisible('sipproxyport',getCheckVal('SIPEnable'));
	setVisible('sipproxyport',0);
}
function isOptionValid(portValue)
{
	if(isPlusInteger(portValue) == false)
	{
		alert('�˿ڲ���ȷ');
		return false;
	}
	if(isValidPort(portValue) == false)
	{
		alert('�˿ڲ���ȷ');
		return false;
	}
	return true;
}
function CheckForm(type)
{
	switch (type)
	{
	case 1:
		return CheckAlg();
		break;
	case 2:
		return CheckDMZ();
		break;
	case 3:
		return CheckPortMapping();
		break;
	case 4:
		with(getElById('PortMappingForm'))
		{
			var rml = getElById('rml');
			if (rml == null)
			{
				return false;
			}
		}
	}
	return true;
}
function CheckAlg()
{
	for (var i = 0; i < WanInfo.length; i++)
	{
		if (getSelectVal('SIPWanPath') == WanInfo[i].key)
		{
			break;
		}
	}
	if (isOptionValid(getValue('SIPPort')) == false)
	{
		alert('SIP�˿ڲ���ȷ��');
		return false;
	}
	if (getCheckVal('SIPEnable') == 1 && WanIndex > -1)
	{
		if (WanInfo[WanIndex].NATEnabled == 0 
		|| WanInfo[WanIndex].ConnectionType.indexOf('_B_')>-1)		
		{
			if (confirm('ע�⣺����WAN������ʧЧ���޸�SIP���ò�������Ч�����Ƿ�ȷ��Ҫ�޸ĸ�����?') == false)
			return false;
		}
	}
	return true;
}
function CheckDMZ()
{
	var i = 0;
	var bWANinterface = false;
	for (i = 0; i < WanInfo.length; i++)
	{
		if((WanInfo[i].ServiceList.indexOf('_TR069_')<0) || (WanInfo[i].ServiceList.indexOf('_INTERNET_')>-1))
		{
			bWANinterface = true;
		}
	}
	if (bWANinterface == false)
	{
		alert('��ǰ�޿��õ�WAN���ӣ������WAN���Ӻ��ٽ������ã�');
		return false;
	}
	with (getElById('dmzForm'))
	{
		if (getCheckVal('DMZEnable') == 1)
		{
			if (getValue('DMZHostIPAddress') == "")
			{
				alert('������DMZ��ַ��');
				return false;
			}
			else if ((isAbcIpAddress(getValue('DMZHostIPAddress'))== false)||(isBroadcastIpAddress(getValue('DMZHostIPAddress'))==true)
			||(isNtwkSgmtIpAddress(getValue('DMZHostIPAddress'))==true))
			{
				alert('DMZ��ַ����ȷ��');
				return false;
			}
		}
		else if (getCheckVal('DMZEnable') == 0 && getValue('DMZHostIPAddress') != "")
		{
			if ((isAbcIpAddress(getValue('DMZHostIPAddress'))== false)||(isBroadcastIpAddress(getValue('DMZHostIPAddress'))==true)
			||(isNtwkSgmtIpAddress(getValue('DMZHostIPAddress'))==true))
			{
				alert('DMZ��ַ����ȷ��');
				return false;
			}
		}
	}
	return true;
}
function CheckPortMapping()
{
	if(VirServEntryIndex >= 10)
	{
		alert('ÿ���ӿ������ɴ洢10��������������!')
		return false;
	}
	if(PortMapping.length >= 32)
	{
		alert('���ɴ洢32��������������!')
		return false;
	}
	if (WanInfo.length == 0)
	{
		alert('��ǰ�޿��õ�WAN���ӣ������WAN���Ӻ��ٽ������ã�');
		return false;
	}
	with (getElById('PortMappingForm') )
	{
		if (RemoteHost.value != "")
		{
			if (isValidIpAddress(RemoteHost.value) == false)
			{
				alert('ԴIP��ַ��ʽ����ȷ��');
				return false;
			}
			else if ((isLoopIpAddress(getValue('RemoteHost'))==true)||(isBroadcastIpAddress(getValue('RemoteHost'))==true)
			||(isNtwkSgmtIpAddress(getValue('RemoteHost'))==true))
			{
				alert('ԴIP��ַ����ȷ��');
				return false;
			}
		}
		if (ExternalPort.value == "")
		{
			alert('�ⲿ��ʼ�˿ںŲ���ȷ��' );
			return false;
		}
		if (InternalPort.value == "")
		{
			alert('�ڲ���ʼ�˿ںŲ���ȷ��' );
			return false;
		}
		if (InternalClient.value == "")
		{
			alert('������ַ����ȷ��' );
			return false;
		}
		if (PortMappingDescription.value == "")
		{
			alert('�������Ʋ���ȷ������������!' );
			return false;
		}
		if (isAbcIpAddress(InternalClient.value) == false ||(isBroadcastIpAddress(getValue('InternalClient'))==true)
		||(isNtwkSgmtIpAddress(getValue('InternalClient'))==true))
		{
			alert('������ַ����ȷ��');
			return false;
		}
		if (isValidPort(ExternalPort.value) == false )
		{
			alert('�ⲿ��ʼ�˿ںŲ���ȷ��' );
			return false;
		}
		if (isValidPort(InternalPort.value) == false )
		{
			alert('�ڲ���ʼ�˿ںŲ���ȷ��' );
			return false;
		}
		if (isValidName(PortMappingDescription.value) == false)
		{
			alert('���ֲ���ȷ��');
			return false;
		}
		for (i = 0; i < PortMapping.length; i++)
		{
			if (PortMapping[i].Description == PortMappingDescription.value)
			{
				alert('�˹��������ѱ�ʹ�á�');
				return false;
			}
			if (PortMapping[i].interface ==  getSelectVal('PortMappingInterface')
			&& PortMapping[i].ExPort == ExternalPort.value
			&& (PortMapping[i].Protocol == PortMappingProtocol.value
			||PortMappingProtocol.value == 'TCP/UDP'
			||PortMapping[i].Protocol == 'TCP/UDP'))
			{
				alert('���ⲿ�˿ں��ѱ�ʹ�ã�');
				return false;
			}
		}
		if ( "21" == getValue("ExternalPort"))
		{
			return confirm('����:������ص�ǰFTP����˿�Ϊ(21)�����øö˿�ӳ����򽫸ı����ص�FTP����˿�Ϊ(2121),��ȷ��Ҫ���?');
		}
		else if ( "23" == getValue("ExternalPort") )
		{
			return confirm('����:������ص�ǰTELNET����˿�Ϊ(23)�����øö˿�ӳ�������ܻ�ı����ص�TELNET�˿�Ϊ(2323),��ȷ��Ҫ���?');
		}
		else if ( "80" == getValue("ExternalPort") )
		{
			return confirm('����:������ص�ǰHTTP����˿�Ϊ(80). ���øö˿�ӳ�������ܻ�ı����ص�web�˿�Ϊ8080,��ȷ��Ҫ���?');
		}
	}
	return true;
}
function RemovePortMapping(Form, CheckTag)
{
	var rml = getElById(CheckTag);
	var i;
	if (rml.length > 0)
	{
		for (i = 0; i < rml.length; i++)
		{
			if (rml[i].checked == true)
			{
				//Form.addParameter(rml[i].value,'');
				var ctlName = "hVirServEntry"+i;
				setText(ctlName,i);
			}
		}
	}
	else if (rml.checked == true)
	{
		//Form.addParameter(rml.value,'');
		var ctlName = "hVirServEntry0";
		setText(ctlName,'0');		
	}
}
function LoadFrame()
{
	setCheck('L2TPEnable',alg.L2TPEnable);
	setCheck('IPSECEnable',alg.IPSECEnable);
	setCheck('H323Enable',alg.H323Enable);
	setCheck('RTSPEnable',alg.RTSPEnable);
	setCheck('SIPEnable',alg.SIPEnable);
	setText('SIPPort',alg.SIPPort);
	setSelect('SIPWanPath',alg.SIPWanPath);
	sipClick();
	ShowDMZ();
	<%if Request_Form("VirServer_Flag") = "3" then %>
	setSelect('PortMappingInterface',getEntryValue(<% tcWebApi_get("WebCurSet_Entry","nat_pvc","s") %>));
	<%end if%>	
	ShowPortMapping();
}
function ShowDMZ()
{
	var Interface = getElById('DMZInterface');
	var index = Interface.selectedIndex;
	if (index >=0)
	{
		var DMZId = Interface.options[index].id;
		index = DMZId.substr(DMZId.indexOf('_') + 1);
		if ((index < 0) || (index >= WanInfo.length))
		{
			return;
		}
		setCheck('DMZEnable',WanInfo[index].DMZEnable);
		setText('DMZHostIPAddress',WanInfo[index].DMZIP);
		if (WanInfo[index].Enable == 0 || WanInfo[index].NATEnabled == 0)
		{
			setDisable('DMZEnable',1);
			setDisable('DMZHostIPAddress',1);
		}
		else
		{
			setDisable('DMZEnable',0);
			setDisable('DMZHostIPAddress',0);
		}
	}
}
function ShowPortMapping()
{
	var Interface = getElById('PortMappingInterface');
	var index = Interface.selectedIndex-1;
	if (index >=0)
	{
		//var PortMpId = Interface.options[index].id;
		//index = PortMpId.substr(PortMpId.indexOf('_') + 1);
		if (Interface.length > 1)
		{
			if (WanInfo[index].Enable == 0 || WanInfo[index].NATEnabled == 0)
			{
				setDisable('btn1',1);
			}
			else
			{
				setDisable('btn1',0);
			}
		}
		var html = '<table border="1" cellpadding="0" cellspacing="0" width="460">'
		+ '<tr align="middle">'
		+  '<td width="9%" class="table_title">Э��</td>'
		+  '<td width="13%" class="table_title">ԴIP��ַ</td>'
		+  '<td width="16%" class="table_title">�ⲿ�˿ں�</td>'
		+  '<td width="16%" class="table_title">�ڲ��˿ں�</td>'
		+  '<td width="13%" class="table_title">�ڲ�����</td>'
		+  '<td width="15%" class="table_title">ӳ������</td>'
		+  '<td width="10%" class="table_title">ʹ��</td>'
		+  '<td width="10%" class="table_title">ɾ��</td>'
		+ '</tr>'
		VirServEntryIndex = 0;
		for (i = 0; i < PortMapping.length; i++)
		{
			//if (PortMapping[i].domain.indexOf(getSelectVal('PortMappingInterface'))	> -1)
			{
				html += '<TR align="middle">'
				html +=  '<TD align="center">' + getDisplayText(PortMapping[i].Protocol,10) + '&nbsp;</TD>';
				html += '<TD align="center">' + getDisplayText(PortMapping[i].RemoteHost,8) + '&nbsp;</TD>';
				html += '<TD align="center">' + getDisplayText(PortMapping[i].ExPort,5) + '&nbsp;</TD>';
				html += '<TD align="center">' + getDisplayText(PortMapping[i].InPort,5) + '&nbsp;</TD>';
				html += '<TD align="center">' + getDisplayText(PortMapping[i].InClient,10) + '&nbsp;</TD>';
				html += '<TD align="center">' + getDisplayText(PortMapping[i].Description,10) + '&nbsp;</TD>';
				if (PortMapping[i].ProtMapEnabled == 1)
				{
					html += '<TD align="center">' + '����' + '&nbsp;</TD>';
				}
				else
				{
					html += '<TD align="center">' + '�ر�' + '&nbsp;</TD>';
				}
				html += '<TD align="center"><input type="checkbox" name="rml" value="'
				+ PortMapping[i].domain + '"></TD>';
				html += '</TR>';
				VirServEntryIndex++;
			}
		}
		html += '</table>'
		getElById('portMappingInfo').innerHTML = html;
	}
}
TOTAL_APP = 14;
FIRST_APP = "ѡ��...";
var v = new Array(TOTAL_APP);
v[0] = new cV("Domain Name Server (DNS)",1);
v[0].e[0] = new iVe("53", "53", "2", "53", "53");
v[1] = new cV("FTP Server",1);
v[1].e[0] = new iVe("21", "21", "1", "21", "21");
v[2] = new cV("IPSEC",1);
v[2].e[0] = new iVe("500", "500", "2", "500", "500");
v[3] = new cV("Mail (POP3)",1);
v[3].e[0] = new iVe("110", "110", "1", "110", "110");
v[4] = new cV("Mail (SMTP)",1);
v[4].e[0] = new iVe("25", "25", "1", "25", "25");
v[5] = new cV("PPTP",1);
v[5].e[0] = new iVe("1723", "1723", "1", "1723", "1723");
v[6] = new cV("Real Player 8 Plus",1);
v[6].e[0] = new iVe("7070", "7070", "2", "7070", "7070");
v[7] = new cV("Secure Shell Server (SSH)",1);
v[7].e[0] = new iVe("22", "22", "1", "22", "22");
v[8] = new cV("Secure Web Server (HTTPS)",1);
v[8].e[0] = new iVe("443", "443", "1", "443", "443");
v[9] = new cV("SNMP",1);
v[9].e[0] = new iVe("161", "161", "2", "161", "161");
v[10] = new cV("SNMP Trap",1);
v[10].e[0] = new iVe("162", "162", "2", "162", "162");
v[11] = new cV("Telnet Server",1);
v[11].e[0] = new iVe("23", "23", "1", "23", "23");
v[12] = new cV("TFTP",1);
v[12].e[0] = new iVe("69", "69", "2", "69", "69");
v[13] = new cV("Web Server (HTTP)",1);
v[13].e[0] = new iVe("80", "80", "1", "80", "80");
function cV(name, entryNum)
{
	this.name = name;
	this.eNum = entryNum;
	this.e = new Array(5);
}
function iVe(eStart, eEnd, proto, iStart, iEnd)
{
	this.eStart = eStart;
	this.eEnd = eEnd;
	this.proto = proto;
	this.iStart = iStart;
	this.iEnd = iEnd;
}
function clearAll()
{
}
function trimspace(str)
{
	var strTemp = new String(str);
	while (-1 != strTemp.search(" "))
	{
		strTemp = strTemp.replace(" ", "");
	}
	return strTemp;
}
function appSelect(sName)
{
	clearAll();
	with (getElById('PortMappingForm'))
	{
		if (sName == FIRST_APP)
		{
			return;
		}
		for(i = 0; i < TOTAL_APP; i++)
		{
			if(v[i].name == sName)
			{
				switch (v[i].e[0].proto)
				{
				case '0':
					setSelect('PortMappingProtocol','TCP/UDP');
					break;
				case '1':
					setSelect('PortMappingProtocol','TCP');
					break;
				case '2':
					setSelect('PortMappingProtocol','UDP');
					break;
				}
				RemoteHost.value = "";
				ExternalPort.value = v[i].e[0].eStart;
				InternalPort.value = v[i].e[0].iStart;
				PortMappingDescription.value = trimspace(sName);
			}
		}
	}
}
function radioClick()
{
	if (document.formext.radiosrv[0].checked == true)
	{
		with(document.formext)
		{
			constsrvName.disabled = true;
		}
	}
	else
	{
		with(document.formext)
		{
			constsrvName.disabled = false;
		}
	}
}

function setALGSwitch()
{
	var Form = document.algForm;
	if(Form.L2TPEnable.checked)
		Form.L2TPSW_Flag.value = "on";
	else 
		Form.L2TPSW_Flag.value = "off";
	
	if(Form.IPSECEnable.checked)
		Form.IPSecSW_Flag.value = "on";
	else 
		Form.IPSecSW_Flag.value = "off";
	
	if(Form.H323Enable.checked)
		Form.H323SW_Flag.value = "on";
	else 
		Form.H323SW_Flag.value = "off";
	
	if(Form.RTSPEnable.checked)
		Form.RTSPSW_Flag.value = "on";
	else 
		Form.RTSPSW_Flag.value = "off";
	
	if(Form.SIPEnable.checked)
		Form.SIPSW_Flag.value = "on";
	else 
		Form.SIPSW_Flag.value = "off";
}
function getEntryIndex(ctl)
{
	for (i = 0; i < nEntryNum; i++)
	{
		if (getSelectVal(ctl) == vEntryValue[i])
			return vEntryIndex[i];
	}
	return 0;
}
function getEntryValue(idx)
{
	for (i = 0; i < nEntryNum; i++)
	{
		if (idx == vEntryIndex[i])
			return vEntryValue[i];
	}
	return 0;
}
function btnSubmit(iMode)
{
	if(CheckForm(iMode) == false)
		return;
	if(iMode == 1)//alg
	{
		setALGSwitch();
		document.algForm.alg_Flag.value = "1";
		document.algForm.submit();
	}
	else if(iMode == 2)//dmz
	{
		if(getCheckVal('DMZEnable') == 0)
			document.dmzForm.EnableDMZ_Flag.value = "No";
		else 
			document.dmzForm.EnableDMZ_Flag.value = "Yes";
		document.dmzForm.DMZ_Flag.value = "1";
		document.dmzForm.DMZEntryCurIndex.value = getEntryIndex('DMZInterface');
		document.dmzForm.submit();
	}
	else if(iMode == 3)//add
	{
		document.PortMappingForm.VirServCurIndex.value = VirServEntryIndex;
		document.PortMappingForm.VirServer_Flag.value = "1";
		document.PortMappingForm.VirServEntryCurIndex.value = getEntryIndex('PortMappingInterface');
		document.PortMappingForm.submit();
	}
	else if(iMode == 4)//del
	{
		RemovePortMapping(PortMappingForm,'rml');
		document.PortMappingForm.VirServer_Flag.value = "2";
		document.PortMappingForm.VirServEntryCurIndex.value = getEntryIndex('PortMappingInterface');
		document.PortMappingForm.submit();
	}
	else if(iMode == 5)//changeIF
	{
		document.PortMappingForm.VirServer_Flag.value = "3";
		document.PortMappingForm.VirServEntryCurIndex.value = getEntryIndex('PortMappingInterface');
		document.PortMappingForm.submit();
	}	
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>ALG����</P></TD>
          <TD width=7 bgColor=#ef8218>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=13><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#�߼�NAT����" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0> </A></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id=algForm name="algForm" action="/cgi-bin/app-natset.asp" method="post">
                  <DIV id=pptp>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD><INPUT type=checkbox name=L2TPEnable> ����L2TP 
                    ALG</TD></TR><BR></TBODY></TABLE></DIV>
                  <DIV id=ipsec>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD><INPUT type=checkbox name=IPSECEnable> ����IPSec 
                    ALG</TD></TR><BR></TBODY></TABLE></DIV>
                  <DIV id=h323>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD><INPUT type=checkbox name=H323Enable> ����H.323 
                    ALG</TD></TR><BR></TBODY></TABLE></DIV>
                  <DIV id=rtsp>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD><INPUT type=checkbox name=RTSPEnable> ����RTSP 
                    ALG</TD></TR><BR></TBODY></TABLE></DIV>
                  <DIV id=sipproxdInfo>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD><INPUT onclick=sipClick() type=checkbox 
                        name=SIPEnable> ����SIP 
                  ALG</TD></TR><BR></TBODY></TABLE><BR></DIV>
                      <input type="hidden" name="L2TPSW_Flag" value="0">
                      <input type="hidden" name="IPSecSW_Flag" value="0">
                      <input type="hidden" name="H323SW_Flag" value="0">
                      <input type="hidden" name="RTSPSW_Flag" value="0">
                      <input type="hidden" name="SIPSW_Flag" value="0">
                      <input type="hidden" name="alg_Flag" value="0">
                  <DIV id=sipproxyport>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=120>SIP�˿ڣ� </TD>
                      <TD><INPUT maxLength=5 name=SIPPort></TD></TR></TBODY></TABLE>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=120>WAN�ӿڣ� </TD>
                      <TD><SELECT id=SIPWanPath name=SIPWanPath 
                          maxlength="15"> 
		<SCRIPT language=JavaScript type=text/javascript>
		document.write('<option value="">' + 'Auto' + '</option>');	
		for (i = 0; i < nEntryNum; i++)
		{
			if((vEntryName[i].indexOf('_TR069_')<0) || (vEntryName[i].indexOf('_INTERNET_')>-1))
			{
				if (vEntryValue[i] == alg.SIPWanPath)
				{
					WanIndex = i;
					document.write('<option value=' + vEntryValue[i] + '>'
					+ vEntryName[i] + '</option>');
				}
				else if (vEntryName[i].indexOf('_R_')>-1
				&& vNATENABLE[i] == 1)
				{
					document.write('<option value=' + vEntryValue[i] + '>'
					+ vEntryName[i] + '</option>');
				}
			}
		}
		</SCRIPT>
                        </SELECT> </TD></TR></TBODY></TABLE></DIV></FORM>
                  <CENTER>
                  <DIV id=applybutton><INPUT onclick="btnSubmit(1)" type=button value=����/Ӧ�� name=button3> 
                  </DIV></CENTER>
                  <P></P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>DMZ����</P></TD>
          <TD width=7 bgColor=#ef8218>��</TD>
          <TD width=474>��</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id=dmzForm name="dmzForm" action="/cgi-bin/app-natset.asp" method="post">
                      ���������ı������û�ж˿�ӳ�䣬�ᱻת����DMZ�����ϡ� 
                      <input type="hidden" name="EnableDMZ_Flag" value="0">
                      <input type="hidden" name="DMZ_Flag" value="0">
                      <input type="hidden" name="DMZEntryCurIndex" value="0">
                      <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=120>�ӿ����ƣ�</TD>
                      <TD><SELECT id=DMZInterface onchange=ShowDMZ() 
                        name=DMZInterface maxlength="15"> 
			<SCRIPT language=JavaScript type=text/javascript>
			for (i = 0; i < nEntryNum; i++)
			{
				if((vEntryName[i].indexOf('_TR069_')<0) || (vEntryName[i].indexOf('_INTERNET_')>-1))
				{
					document.write('<option value=' + vEntryValue[i] + ' id=dmz_' + i + '>'
					+ vEntryName[i] + '</option>');
				}
			}			
			</SCRIPT>
                        </SELECT> </TD></TR>
                    <TR>
                      <TD width=120>ʹ�ܣ�</TD>
                      <TD><INPUT onclick=dmzClick() type=checkbox 
                        name=DMZEnable></TD></TR>
                    <TR>
                      <TD width=120>LAN��IP��ַ��</TD>
                      <TD><INPUT maxLength=15 
                    name=DMZHostIPAddress></TD></TR></TBODY></TABLE></FORM><BR>
                  <CENTER><INPUT onclick="btnSubmit(2)" type=button value=����/Ӧ�� name=button> 
                  </CENTER>
                  <P></P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>������������</P></TD>
          <TD width=7 bgColor=#ef8218>��</TD>
          <TD width=474>��</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                <FORM id=formext name=formext>
                <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD><INPUT onclick=radioClick() type=radio CHECKED 
                        name=radiosrv>&nbsp;�Զ��壺&nbsp; </TD></TR>
                    <TR>
                      <TD><INPUT onclick=radioClick() type=radio 
                        name=radiosrv>&nbsp;ѡ��Ӧ�ã�&nbsp; </TD>
                      <TD><SELECT disabled onchange=appSelect(this.value) 
                        size=1 name=constsrvName> <OPTION value=FIRST_APP 
                          selected>ѡ��...</OPTION> <OPTION 
                          value="Domain Name Server (DNS)">Domain Name Server 
                          (DNS)</OPTION> <OPTION value="FTP Server">FTP 
                          Server</OPTION> <OPTION value=IPSEC>IPSEC</OPTION> 
                          <OPTION value="Mail (POP3)">Mail (POP3)</OPTION> 
                          <OPTION value="Mail (SMTP)">Mail (SMTP)</OPTION> 
                          <OPTION value=PPTP>PPTP</OPTION> <OPTION 
                          value="Real Player 8 Plus">Real Player 8 Plus</OPTION> 
                          <OPTION value="Secure Shell Server (SSH)">Secure Shell 
                          Server (SSH)</OPTION> <OPTION 
                          value="Secure Web Server (HTTPS)">Secure Web Server 
                          (HTTPS)</OPTION> <OPTION value=SNMP>SNMP</OPTION> 
                          <OPTION value="SNMP Trap">SNMP Trap</OPTION> <OPTION 
                          value="Telnet Server">Telnet Server</OPTION> <OPTION 
                          value=TFTP>TFTP</OPTION> <OPTION 
                          value="Web Server (HTTP)">Web Server 
                        (HTTP)</OPTION></SELECT> 
              </TD></TR></TBODY></TABLE></FORM></TD></TR>
              <TR>
                <TD>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id=PortMappingForm name="PortMappingForm" action="/cgi-bin/app-natset.asp" method="post">
                  <DIV id=natpm>
	                <input type="hidden" name="VirServer_Flag" value="0"> 
	                <input type="hidden" name="EnableVirServer_Flag" value="Yes">
	                <input type="hidden" name="VirServEntryCurIndex" value="0">
	                <input type="hidden" name="VirServCurIndex" value="0">
	                <input type="hidden" name="hVirServEntry0" value="99">
	                <input type="hidden" name="hVirServEntry1" value="99">
	                <input type="hidden" name="hVirServEntry2" value="99">
	                <input type="hidden" name="hVirServEntry3" value="99">
	                <input type="hidden" name="hVirServEntry4" value="99">
	                <input type="hidden" name="hVirServEntry5" value="99">
	                <input type="hidden" name="hVirServEntry6" value="99">
	                <input type="hidden" name="hVirServEntry7" value="99">
	                <input type="hidden" name="hVirServEntry8" value="99">
	                <input type="hidden" name="hVirServEntry9" value="99">	                
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                              <TD width=120>�ӿڣ�</TD>
                      <TD><SELECT onchange=btnSubmit(5) size=1 
                        name=PortMappingInterface> 
			<SCRIPT language=JavaScript type=text/javascript>
			document.write('<option value="">' + '' + '</option>');
			for (i = 0; i < nEntryNum; i++)
			{
				if((vEntryName[i].indexOf('_TR069_')<0 || vEntryName[i].indexOf('_INTERNET_')>-1) && (vEntryName[i].indexOf('_R_')>-1))
				{
					document.write('<option value=' + vEntryValue[i] + ' id=portmp_' + i + '>'
					+ vEntryName[i] + '</option>');
				}
			}
			</SCRIPT>
                        </SELECT> </TD></TR>
                    <TR style="DISPLAY: none">
                      <TD width=120>ʹ�ܣ�</TD>
                      <TD><SELECT size=1 name=PortMappingEnabled> <OPTION 
                          value=1 selected>����</OPTION> <OPTION 
                          value=0>����</OPTION></SELECT> </TD></TR>
                    <TR>
                      <TD width=120>Э�飺</TD>
                      <TD><SELECT size=1 name=PortMappingProtocol> <OPTION 
                          value=TCP selected>TCP</OPTION> <OPTION 
                          value=UDP>UDP</OPTION> <OPTION 
                          value=TCP/UDP>TCP/UDP</OPTION></SELECT> </TD></TR>
                    <TR>
                      <TD width=120>ԴIP��ַ��</TD>
                      <TD><INPUT maxLength=15 name=RemoteHost></TD></TR>
                    <TR>
                      <TD width=120>�ⲿ�˿ںţ�</TD>
                      <TD><INPUT maxLength=5 name=ExternalPort></TD></TR>
                    <TR>
                      <TD width=120>�ڲ��˿ںţ�</TD>
                      <TD><INPUT maxLength=5 name=InternalPort></TD></TR>
                    <TR>
                      <TD width=120>�ڲ�������</TD>
                      <TD><INPUT maxLength=32 name=InternalClient></TD></TR>
                    <TR>
                      <TD width=120>ӳ�����֣�</TD>
                      <TD><INPUT maxLength=31 
                    name=PortMappingDescription></TD></TR></TBODY></TABLE></DIV></FORM><BR><LEFT><INPUT onclick=btnSubmit(3) type=button value=��� name=btn1> 
                  </LEFT><BR><BR><BR>
                  <DIV id=portMappingInfo></DIV><BR><LEFT><INPUT onclick=btnSubmit(4) type=button value=ɾ��> 
                  </LEFT></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>&nbsp;</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR></TBODY></TABLE>
      <FORM style="DISPLAY: none" name=ConfigForm action=""></FORM></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>��</TD>
          <TD width=278 bgColor=#427594>��</TD>
          <TD width=196 bgColor=#427594><!-- InstanceEndEditable --></TD>
          <TD width=170 
bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
