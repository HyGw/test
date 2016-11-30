<%
if Request_Form("Changed_Flag") = "1" then
	TCWebApi_set("WebCurSet_Entry","qos_id","curIndex")
	TCWebApi_set("QoS_Entry","Active","VLanActive")
	TCWebApi_set("QoS_Entry","pvcIndex","pvcindex")
	TCWebApi_set("QoS_Entry","vlanid","nameCOM_VLAN")
	TCWebApi_set("QoS_Entry","EntryIndex","curIndexshow")
	TCWebApi_set("QoS_Common", "QoSOptType","vlanRuleFlag")	
	TCWebApi_commit("QoS_Entry")
	TCWebApi_commit("QoS_Common")
	TCWebApi_save()
elseif Request_Form("Changed_Flag") = "2" then
	TCWebApi_set("WebCurSet_Entry","qos_id","curIndex")
	TCWebApi_set("QoS_Entry","EntryIndex","curIndexshow")
	
end if
%>

<html>
<head>
<meta http-equiv="Content-Language" content="zh-cn">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link rel="stylesheet" href='/JS/stylemain.css' type='text/css'>
<title>QoS</title>
<script language="javascript" src="/JS/menu.js"></script>
<script language="javascript" src="/JS/util.js"></script>

      <SCRIPT language=JavaScript type=text/javascript>
	 	var para = 0;
		var pvc_counts=<% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
		//get all value
		// num 0
		var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanName = vArrayStr.split(',');
		// num 1
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanValue = vArrayStr.split(',');
		// num 2
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanIndex = vArrayStr.split(',');
		// num 3
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		Wan_Actives = vArrayStr.split(',');
		// num 4		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var WANEnNAT = vArrayStr.split(',');
		// num 5		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 6		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IPVERSION = vArrayStr.split(',');
		// num 7		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_Status4 = vArrayStr.split(',');
		// num 8		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IP4 = vArrayStr.split(',');
		// num 9		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_ENCAP = vArrayStr.split(',');
		// num 10		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_GateWay4 = vArrayStr.split(',');
		// num 11		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_DNS4 = vArrayStr.split(',');
		// num 12		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_SecDNS4 = vArrayStr.split(',');
		// num 13		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_Status6 = vArrayStr.split(',');
		// num 14		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IP6 = vArrayStr.split(',');
		// num 15		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_GateWay6 = vArrayStr.split(',');
		// num 16		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_DNS6 = vArrayStr.split(',');
		// num 17		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_SecDNS6 = vArrayStr.split(',');
		// num 18		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_PD6 = vArrayStr.split(',');
		// num 19		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_NetMask4 = vArrayStr.split(',');
		
		Wan_WanName = SortUtil.UpdateWanIFName( Wan_WanName, pvc_counts );

		Wan_WanValue = SortUtil.SortMyArray( Wan_WanValue );
		Wan_WanIndex = SortUtil.SortMyArray( Wan_WanIndex );		
		Wan_Actives = SortUtil.SortMyArray( Wan_Actives );
		WANEnNAT = SortUtil.SortMyArray( WANEnNAT );
		Wan_IPVERSION = SortUtil.SortMyArray( Wan_IPVERSION );
		Wan_Status4 = SortUtil.SortMyArray( Wan_Status4 );
		Wan_IP4 = SortUtil.SortMyArray( Wan_IP4 );
		Wan_ENCAP = SortUtil.SortMyArray( Wan_ENCAP );
		Wan_GateWay4 = SortUtil.SortMyArray( Wan_GateWay4 );
		Wan_DNS4 = SortUtil.SortMyArray( Wan_DNS4 );
		Wan_SecDNS4 = SortUtil.SortMyArray( Wan_SecDNS4 );
		Wan_Status6 = SortUtil.SortMyArray( Wan_Status6 );
		Wan_IP6 = SortUtil.SortMyArray( Wan_IP6 );
		Wan_GateWay6 = SortUtil.SortMyArray( Wan_GateWay6 );
		Wan_DNS6 = SortUtil.SortMyArray( Wan_DNS6 );
		Wan_SecDNS6 = SortUtil.SortMyArray( Wan_SecDNS6 );
		Wan_PD6 = SortUtil.SortMyArray( Wan_PD6 );
		Wan_NetMask4 = SortUtil.SortMyArray( Wan_NetMask4 );

</script>

</head>
<body topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000" style="text-align: center" onLoad="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()"
onunload="DoUnload()">
<table width="808" height="100%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="1">
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
<table height=80 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg border=0>
<% else %>
<table width="808"  height="117" border="0" cellpadding="0" cellspacing="0" background=          "/img/framelogo.jpg" >
<% end if %> 
<tr>
<td>&nbsp;</td>
<td width="358" align="right" valign="bottom">
<table border="0" cellpadding="0" cellspacing="0" id="table8">
<tr>
<td align="right" valign="bottom">
<span class="curUserName">
&nbsp;
</span>
</td>
<td width="120" align="center" valign="bottom" class="welcom">
��ӭ����
</td>
<td width="50" valign="bottom"><a href="/cgi-bin/logout.cgi" target="_top" onClick="DoLogout()">
<span class="logout">
�˳�
</span>
</a>
</td>
</tr>
</table></td>
</tr>
</table>
<table border="0" cellpadding="0" cellspacing="0" width="808" height="100" id="table2">
<tr>
<TD width=808 bgColor=#d8d8d8 height=2 colSpan=3></td>
</tr>
<tr>
<td width="163" rowspan="3" align="center" bgcolor="#d8d8d8" class="LocationDisplay" id="LocationDisplay"></td>
<td height="33" width="434" bgcolor="#0059A0">
<p align="right"><font color="#FFFFFF" face="����"><b><font face="����" size="6" color="#FFFFFF">
<input name="Selected_Menu" type="hidden" id="Selected_Menu" value="����->QoS">
</font></b><span class="GatewayName">��������:
<script language="javascript">
document.write(top.gateWayName);
</script>
</span></font></td>
<td height="33" width="211" bgcolor="#d8d8d8">
<p align="center" class="GatewayType">�ͺ�:
<script language="javascript">
document.write(top.ModelName);
</script></td>
</tr>
<tr>
<td height="43" colspan="2"  bgcolor="#d8d8d8" valign="bottom" id="MenuArea_L1">&nbsp;</td>
</tr>
<tr>
<td colspan="2" height="24" bgcolor="#0059A0" id="MenuArea_L2"></td>
</tr>
</table>
<script language="javascript">
MakeMenu(getElById ('Selected_Menu').value);
</script>
<table border="0" cellpadding="0" cellspacing="0" width="808" height="15" id="table3">
<tr>
<td height="15">
<img height=15 src="/img/panel1.gif" width=164 border=0>
</td>
<td>
<img height=15 src="/img/panel2.gif" width=645 border=0>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td valign="top">
<script language="JavaScript" src="/JS/printbox.js"></script>
<script language="JavaScript" type="text/javascript">
function stWan(domain,Name,Operation,Protocol)
{
var list = domain.split('.');
this.domain = domain;
this.WcdDomain = list[0] + '.' + list[1] + '.' + list[2] + '.' + list[3] + '.' + list[4];
this.key = domain.substr(domain.indexOf('WANDevice'));
this.Name = Name;
this.Operation = Operation;
this.Protocol = Protocol;
this.inst = list[4];
}
function stDsl(domain,COM_VLAN)
{
this.domain = domain;
var list = domain.split('.');
this.key = 	list[4];
this.WcdDomain = list[0] + '.' + list[1] + '.' + list[2] + '.' + list[3] + '.' + list[4];
this.inst = list[4];
this.COM_VLAN = COM_VLAN;
}
var CntPvc = new Array(new stDsl("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANDSLLinkConfig","10000"),null);
var WanIP = new Array(null);
var WanPPP = new Array(new stWan("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANPPPConnection.2","1_TR069_INTERNET_R_0_35","Active","Off"),null);
var ii = 0;
var VlanDomain;
var VlanAttr = '|Subscriber,';
var VlanList = VlanAttr.split(',');
for (i = 0; i < VlanList.length-1; i++)
{
CntPvc[i].VlanAttr = VlanList[i];
}
var Wan = new Array();
for (i = 0; i < WanIP.length-1; i++)
{
Wan[i] = WanIP[i];
for (ii = 0; ii < CntPvc.length-1; ii++)
{
if (WanIP[i].inst == CntPvc[ii].inst)
{
Wan[i].COM_VLAN = CntPvc[ii].COM_VLAN;
Wan[i].VlanAttr = CntPvc[ii].VlanAttr;
}
}
}
for (j = 0; j < WanPPP.length-1; i++,j++)
{
Wan[i] =  WanPPP[j];
for (ii = 0; ii < CntPvc.length-1; ii++)
{
if (WanPPP[j].inst == CntPvc[ii].inst)
{
Wan[i].COM_VLAN = CntPvc[ii].COM_VLAN;
Wan[i].VlanAttr = CntPvc[ii].VlanAttr;
}
}
}
function ShowIfName(val,arrMsgInfo)
{
for (i = 0; i < Wan.length; i++)
{
if (Wan[i].domain == val)
{
return Wan[i].Name;
}
else if ('br0' == val)
{
return 'br0';
}
}
return '&nbsp;';
}
function LoadFrame()
{	
}
function CheckForm()
{
if (getElById('sel_Interface').length == 0)
{
alert('��ǰû��WAN���ӣ����½��µ�WAN���Ӻ��ٽ���RIP���ã�');
return false;
}
var v = getValue('idCOM_VLAN');
if(isPlusInteger(v) == false)
{
alert("VLAN ID�Ƿ���");
return false;
}
else
{
if (((v == "") || (v < 0) || (v > 4094)) && (v != 10000))
{
alert("VLAN ID�Ƿ���");
return false;
}
}
return true;
}
function AddSubmitParam(SubmitForm,type)
{
var WcdDomain;
SubmitForm.addParameter('x.X_CT-COM_VLAN', getValue('idCOM_VLAN')) ;
for (i = 0; i < Wan.length; i++)
{
if (Wan[i].domain == getSelectVal('sel_Interface'))
{
WcdDomain = Wan[i].WcdDomain;
break;
}
}
SubmitForm.setAction('setcfg.cgi?x=' + WcdDomain + '.WANDSLLinkConfig' + '&RequestFile=html/network/comvlan.asp');
}

function btnRtturnQoS()
{
//var Form = new webSubmitForm();
//var	url ;
//with (getElById('ConfigForm'))
//{
//url = 'setcfg.cgi?RequestFile=html/network/qos.asp';
//Form.setAction(url);
	location.replace("net-qos.asp");
	Form.submit();
	return;
//}
}
</script>
<table height="100%" border="0" cellpadding="0" cellspacing="0">
<tr>
<TD width=157 bgColor=#e7e7e7 height=30>
<p class="Item_L1">QoS</p></td>          	
<TD width=7 bgColor=#e7e7e7>��</TD>
<td width="474">��</td>
<td width="170" rowspan="9" valign="top" background="/img/panel4.gif"><table width="100%" border="0" cellspacing="0" cellpadding="20" height='100%'>
<tr>
<td valign='top'><a href="../cgi-bin/help_content.asp#QoS" target="_blank"><img src="/img/help.gif" width="40" height="34" border="0"></a></td>
</tr>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
<tr>
  <td valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></td></tr>
<% end if %>
</table>��      ������      ������</td>
</tr>
<tr>
<td width="157" height="10" valign="top" bgcolor="#E7E7E7"></td>
<td width="7" background="/img/panel3.gif">��</td>
<td></td>
</tr>
<tr>
<td width="157" height="30" valign="top" bgcolor="#E7E7E7"><p class="Item_L2"></p></td>
<td width="7" background="/img/panel3.gif">��</td>
<td>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="10">&nbsp;</td>
<td><p><br>
<br><br>
<form id="ConfigForm" name="ConfigForm">
<table border="1" cellpadding="3" cellspacing="0" width="400">
<tr align='center'>
<td class="table_title">�ӿ�</td>
<td class="table_title">VLAN</td>
</tr>
<tr align="middle">
<td>
					<input type="hidden" name="Changed_Flag" value="0">
                    <input type="hidden" name="VLanActive" value="<% tcWebApi_get("QoS_Entry","Active","s") %>">
                    <input type="hidden" name="curIndex" value="<% tcWebApi_get("WebCurSet_Entry","qos_id","s") %>">
                    <input type="hidden" name="curIndexshow" value="<% tcWebApi_get("QoS_Entry","EntryIndex","s") %>">
                     <input type="hidden" name="vlanRuleFlag" value="vlanidRule">
					  <input type="hidden" name="pvcindex" value=""> 
						  
                          <script language="JavaScript" type="text/JavaScript">

function WriteWanNameSelected()
{	
	for (i = 0; i < pvc_counts; i++)
	{
		document.write('<option value=' + Wan_WanIndex[i] + '>' + Wan_WanName[i] +  '</option>');
	}	
	var Form = document.ConfigForm;
	Form.sel_Interface.value = Form.curIndexshow.value;		
}

function ChangeInterface(Interface)
{
	var Form = document.ConfigForm;
	Form.curIndex.value = parseInt( parseInt(Wan_WanIndex[Form.sel_Interface.selectedIndex]) / 8) + 22;
	Form.curIndexshow.value = parseInt(Form.sel_Interface.value);	
	Form.Changed_Flag.value = "2";
	Form.submit();	
}

function	BtnSave()
{
	var Form = document.ConfigForm;
	Form.curIndex.value = parseInt( parseInt(Wan_WanIndex[Form.sel_Interface.selectedIndex]) / 8) + 22;
	Form.pvcindex.value = parseInt( parseInt(Wan_WanIndex[Form.sel_Interface.selectedIndex]) / 8);
	Form.curIndexshow.value = parseInt(Form.sel_Interface.value);
	if(parseInt(Form.nameCOM_VLAN.value) <0 ||  parseInt(Form.nameCOM_VLAN.value) >4094)
	{
		if(Form.nameCOM_VLAN.value != "10000")
		{
			alert("VLANֵ����ȷ��");
			return;
		}
	}
	
	Form.Changed_Flag.value = "1";
	if((Form.nameCOM_VLAN.value == "10000") || (Form.nameCOM_VLAN.value == ""))
		Form.VLanActive.value = "No";
	else Form.VLanActive.value = "Yes";	
	Form.submit();
}
</script> 
                          <select id="sel_Interface" name="sel_Interface" onChange="ChangeInterface(this)">
<script language="JavaScript" type="text/javascript">
WriteWanNameSelected();

</script>
</select>
</td>
<td>
<INPUT name="nameCOM_VLAN" id="idCOM_VLAN" size="5" maxlength="5" value="">

</td>
</tr>
<script language="JavaScript" type="text/javascript">
	var myvlanid='<%tcWebApi_get("QoS_Entry","vlanid","s")%>';
	var myForm = document.ConfigForm;
	if(myvlanid != "N/A")
	{
		myForm.idCOM_VLAN.value = myvlanid;
	}
	else
	{
		var entryNum = parseInt( parseInt(Wan_WanIndex[myForm.sel_Interface.selectedIndex]) / 8) + 22;
		var idVal;
		if( entryNum == 22)
		{
			idVal = '<%tcWebApi_get("QoS_Entry22","vlanid","s")%>';
		}
		if( entryNum == 23)
		{
			idVal = '<%tcWebApi_get("QoS_Entry23","vlanid","s")%>';
		}
		if( entryNum == 24)
		{
			idVal = '<%tcWebApi_get("QoS_Entry24","vlanid","s")%>';
		}
		if( entryNum == 25)
		{
			idVal = '<%tcWebApi_get("QoS_Entry25","vlanid","s")%>';
		}
		if( entryNum == 26)
		{
			idVal = '<%tcWebApi_get("QoS_Entry26","vlanid","s")%>';
		}
		if( entryNum == 27)
		{
			idVal = '<%tcWebApi_get("QoS_Entry27","vlanid","s")%>';
		}
		if( entryNum == 28)
		{
			idVal = '<%tcWebApi_get("QoS_Entry28","vlanid","s")%>';
		}
		if( entryNum == 29)
		{
			idVal = '<%tcWebApi_get("QoS_Entry29","vlanid","s")%>';
		}
		if(idVal != "N/A")
		{
			myForm.idCOM_VLAN.value = idVal;
		}
		else
		{
			myForm.idCOM_VLAN.value="";
		}
	}



if (getElById('sel_Interface').length != 0)
{
	var obj = getElById('sel_Interface');
}
</script>
</table>
                  </form>
<br>
<input type='button' onClick='btnRtturnQoS()' value='���ض��б༭ҳ��'>
</p>
</td>
<td width="10">&nbsp;</td>
</tr>
<tr>
<td width="10">&nbsp;</td>
<td></td>
<td width="10">&nbsp;</td>
</tr>
</table>    </td>
</tr>
</div>
<tr>
<td width="157" height="10" valign="top" bgcolor="#E7E7E7"></td>
<td width="7" background="/img/panel3.gif">��</td>
<td></td>
</tr>
<tr>
<td width="157" height="" valign="top" bgcolor="#E7E7E7"></td>
<td width="7" background="/img/panel3.gif">��</td>
<td></td>
</tr>
</table>
</td>
</tr>
<tr>
<td height="1">
<table border="0" cellpadding="0" cellspacing="0" width="808" height="35" id="table7">
<tr>
<TD width=162 bgColor=#d8d8d8>��</TD>
<TD width=278 bgColor=#0059A0>��</TD>
<TD width=196 bgColor=#0059A0>
<p align="center"><img src="/img/ok.gif" width="80" height="23" border="0" id="btnOK" onClick="SubmitForm()">&nbsp;&nbsp;<img src="/img/cancel.gif" width="80" height="23" border="0" id="btnCancel" onClick="Cancle()"> </td>
<td width="170" bgcolor="#313031">��</td>
</tr>
</table>
</td>
</tr>
</table>
</body>
</html>
