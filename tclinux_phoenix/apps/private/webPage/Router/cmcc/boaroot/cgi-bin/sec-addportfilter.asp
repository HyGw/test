<%
if Request_Form("PortFilterType") <> "" then
	TCWebApi_set("WebCurSet_Entry","ipfilter_id","PortFilterType")
end if

if Request_Form("Save_Flag") = "1" then 
	TCWebApi_set("WebCurSet_Entry","ipfilter_id","curNum")
	TCWebApi_set("IpMacFilter_Common","IpMacType","IpMacType_Flag")
	TCWebApi_set("IpMacFilter_Common","Action","Actionflag")
	TCWebApi_set("IpMacFilter_Entry","Interface","WanInterfaceFlag")	
	TCWebApi_set("IpMacFilter_Entry","Direction","Direction_Flag")
	TCWebApi_set("IpMacFilter_Entry","Active","EnableIP_Flag")
	TCWebApi_set("IpMacFilter_Entry","RuleType","RuleType_Flag")
	TCWebApi_set("IpMacFilter_Entry","IPName","Name")
	TCWebApi_set("IpMacFilter_Entry","Protocol","Protocol")
	
	TCWebApi_set("IpMacFilter_Entry","SrcIPAddr","SourceIPStart")
	TCWebApi_set("IpMacFilter_Entry","SrcIPMask","SourceIpMask")
	TCWebApi_set("IpMacFilter_Entry","SrcPort","SourcePortStart")

	TCWebApi_set("IpMacFilter_Entry","DesIPAddr","DestIPStart")
	TCWebApi_set("IpMacFilter_Entry","DesIPMask","DestIpMask")
	TCWebApi_set("IpMacFilter_Entry","DesPort","DestPortStart")
	if TCWebApi_get("WebCustom_Entry","isCMCCSupported","h" ) = "Yes" then
		TCWebApi_set("IpMacFilter_Entry","SrcIPEndAddr","SourceIPEnd")
		TCWebApi_set("IpMacFilter_Entry","DesIPEndAddr","DestIPEnd")
		TCWebApi_set("IpMacFilter_Entry","SrcPortEnd","SourcePortEnd")
		TCWebApi_set("IpMacFilter_Entry","DesPortEnd","DestPortEnd")
	end if
	TCWebApi_commit("IpMacFilter_Entry")
	TCWebApi_save()
	
end if
%>
<html>
<head>
<meta http-equiv="Content-Language" content="zh-cn">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link rel="stylesheet" href="/JS/stylemain.css" type='text/css'>
<title>��Ӷ˿ڹ���</title>

<script language="javascript" src="/JS/menu.js"></script>
<script language="javascript" src="/JS/util.js"></script>
<script language="JavaScript" type="text/JavaScript">
<% if Request_Form("Save_Flag") = "1" then %>
	location.replace("/cgi-bin/sec-portfilter.asp");
<%end if%>
</script>
</head>
<body topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000" style="text-align: center" onLoad="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()"
onunload="DoUnload()">
<table width="808" height="100%" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr> 
    <td height="1"> 
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
    	<table width="808"  height="80" border="0" cellpadding="0" cellspacing="0" background="/img/framelogo.jpg" >
<% else %>
    	<table width="808"  height="117" border="0" cellpadding="0" cellspacing="0" background="/img/framelogo.jpg" >
<% end if %>
        <tr> 
          <td>&nbsp;</td>
          <td width="358" align="right" valign="bottom"> <table border="0" cellpadding="0" cellspacing="0" id="table8">
              <tr> 
                <td align="right" valign="bottom"> <span class="curUserName"> 
                  &nbsp; </span> </td>
                <td width="120" align="center" valign="bottom" class="welcom"> 
                  ��ӭ���� </td>
                <td width="50" valign="bottom"><a href="/cgi-bin/logout.cgi" target="_top" onClick="DoLogout()"> 
                  <span class="logout"> �˳� </span> </a> </td>
              </tr>
            </table></td>
        </tr>
      </table>
      <table border="0" cellpadding="0" cellspacing="0" width="808" height="100" id="table2">
      	<tr><TD width=808 bgColor=#d8d8d8 height=2 colSpan=3></td></tr>	
        <tr> 
          <td width="163" rowspan="3" align="center" bgcolor="#d8d8d8" class="LocationDisplay" id="LocationDisplay"></td>
          <td height="33" width="434" bgcolor="#0059A0"> <p align="right"><font color="#FFFFFF" face="����"><b><font face="����" size="6" color="#FFFFFF"> 
              <input name="Selected_Menu" type="hidden" id="Selected_Menu" value="��ȫ->�˿ڹ���">
              </font></b><span class="GatewayName">��������: 
              <script language="javascript">
document.write(top.gateWayName);
</script>
              </span></font></td>
          <td height="33" width="211" bgcolor="#d8d8d8"> <p align="center" class="GatewayType">�ͺ�: 
              <script language="javascript">
document.write(top.ModelName);
</script>
          </td>
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
</script> <table border="0" cellpadding="0" cellspacing="0" width="808" height="15" id="table3">
        <tr> 
          <td height="15"> <img height=15 src="/img/panel1.gif" width=164 border=0> 
          </td>
          <td> <img height=15 src="/img/panel2.gif" width=645 border=0> </td>
        </tr>
      </table></td>
  </tr>
  <tr> 
    <td valign="top"> <table height="100%" border="0" cellpadding="0" cellspacing="0">
        <tr> 
          <td width="157" bgcolor="#e7e7e7" height="30"><p class="Item_L1">�˿ڹ���</p></td>
          <td width="7" bgcolor="#e7e7e7">��</td>
          <td width="474">��</td>
          <td width="170" rowspan="5" valign="top" background="/img/panel4.gif"><table width="100%" border="0" cellspacing="0" cellpadding="20" height='100%'>
              <tr> 
                <td valign='top'><a href="/cgi-bin/help_content.asp#�˿ڹ���" target="_blank"><img src="/img/help.gif" width="40" height="34" border="0"></a></td>
              </tr>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
            </table>
            �� �� ��</td>
        </tr>
        <tr> 
          <td width="157" height="10" valign="top" bgcolor="#E7E7E7"></td>
          <td width="7" background="/img/panel3.gif">��</td>
          <td></td>
        </tr>
        <tr> 
          <td width="157" height="30" valign="top" bgcolor="#E7E7E7"><p class="Item_L2"></p></td>
          <td width="7" background="/img/panel3.gif">��</td>
          <td> <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr> 
                <td width="10">&nbsp;</td>
                <td> <form name="ConfigForm" id="ConfigForm" action="/cgi-bin/sec-addportfilter.asp" method="post">
                    <p> 
                      <input type="hidden" name="PortFilterType">
                      <input type="hidden" name="IpMacType_Flag" value="IpUp">
                      <input type="hidden" name="Save_Flag" value="0">
                      <input type="hidden" name="curNum" value="<% tcWebApi_get("WebCurSet_Entry","ipfilter_id","s") %>">
                      <input type="hidden" name="Direction_Flag" value="<% tcWebApi_get("IpMacFilter_Entry","Direction","s") %>">
                      <input type="hidden" name="EnableIP_Flag" value="Yes">
                      <input type="hidden" name="RuleType_Flag" value="IP">
                      <input type="hidden" name="Current_Flag" value="0">
                      <input type="hidden" name="WanInterfaceFlag" value="br0">
                      <input type="hidden" name="IpType_Flag" value="IpUp">
                      <input type="hidden" name="Actionflag" value="Add">
                      <script language="JavaScript" type="text/javascript">
var DomainName = document.ConfigForm.curNum.value;
/*if (top.preAddDomain != '')
{
DomainName = top.preAddDomain;
top.preAddDomain = '';
}
else
{
DomainName = 'InternetGatewayDevice.X_ATP_Security.IpFilterOut';
}*/
var portFilterInIndex = 0;
var portFilterOutIndex = 0;
var portFilterIn = new Array()
var portFilterOut = new Array()
function stWanInfo(domain,Name,Enable,NATEnabled,ConnectionType,CntStatus,IPv6Enabled)
{
this.domain = domain;
this.Name = Name;
this.Enable = Enable;
this.NATEnabled = NATEnabled;
this.ConnectionType = ConnectionType;
this.CntStatus = CntStatus;
this.IPv6Enabled = IPv6Enabled;
}
var WanIP  = new Array(new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1","1_TR069_R_8_46","1","0","IP_Routed","PendingDisconnect","0"),null);
var WanPPP = new Array(null);
var WanInfo = new Array();
var i;
for (i = 0; i < WanIP.length-1; i++)
{
WanInfo[i] = WanIP[i];
}
for (j = 0; j < WanPPP.length-1; i++,j++)
{
WanInfo[i] =  WanPPP[j];
}
var FliterType;
if (DomainName <= 79)
{
	FliterType = 'out';//up filter
}
else
{
	FliterType = 'in';//down filter
}
function LoadFrame()
{
	var typeFLAG = GetQueryString('typeFlag');
	document.ConfigForm.Save_Flag.value = "0";
	if (FliterType== "out")
	{
		document.ConfigForm.Current_Flag.value = "out";
		setDisplay('interface',0);
		ChangeInface();		
	}
	else if (FliterType== "in")
	{
		document.ConfigForm.Current_Flag.value = "in";
		setDisplay('interface',1);
		ChangeInface();
	}
	if ( typeFLAG )
		document.ConfigForm.IpMacType_Flag.value = typeFLAG;
}

function checkForm()
{
	//var	IPNames = new Array(8);
	var vnumber = 0;
	/*IPNames[0] = "<% tcWebApi_get("IpMacFilter_Entry0","IPName","s") %>";
	IPNames[1] = "<% tcWebApi_get("IpMacFilter_Entry1","IPName","s") %>";
	IPNames[2] = "<% tcWebApi_get("IpMacFilter_Entry2","IPName","s") %>";
	IPNames[3] = "<% tcWebApi_get("IpMacFilter_Entry3","IPName","s") %>";
	IPNames[4] = "<% tcWebApi_get("IpMacFilter_Entry4","IPName","s") %>";
	IPNames[5] = "<% tcWebApi_get("IpMacFilter_Entry5","IPName","s") %>";
	IPNames[6] = "<% tcWebApi_get("IpMacFilter_Entry6","IPName","s") %>";
	IPNames[7] = "<% tcWebApi_get("IpMacFilter_Entry7","IPName","s") %>";*/
	
	if(document.ConfigForm.Current_Flag.value == "out")
	{
		/*
		if(parseInt(vnumber)>=40)
		{
			alert("���˹������ﵽ���ޣ�");
			return false;	
		}

		if (document.ConfigForm.Name.value == IPNames[0] || document.ConfigForm.Name.value == IPNames[1] || document.ConfigForm.Name.value == IPNames[2] || document.ConfigForm.Name.value == IPNames[3] )
		{
			 alert("�������Ʋ����ظ���");
			 return false;			 	
		}*/
		
		if (document.ConfigForm.Protocol.value == "" && document.ConfigForm.SourceIPStart.value == "" && document.ConfigForm.SourceIpMask.value == "" && document.ConfigForm.SourcePortStart.value == "" &&
			document.ConfigForm.DestIPStart.value == "" && document.ConfigForm.DestIpMask.value == "" && document.ConfigForm.DestPortStart.value == "")
		{
			 alert("���˹�������Ҫ����һ�������");
			 return false;			 	
		}
	}
		
	if(document.ConfigForm.Current_Flag.value == "in")
	{
		/*
		if(parseInt(vnumber)>=40)
		{
			alert("���˹������ﵽ���ޣ�");
			return false;		
		}
	
		if (document.ConfigForm.Name.value == IPNames[4] || document.ConfigForm.Name.value == IPNames[5] || document.ConfigForm.Name.value == IPNames[6] || document.ConfigForm.Name.value == IPNames[7] )
		{
			 alert("�������Ʋ����ظ���");
			 return false;			 	
		}*/
		
		if (document.ConfigForm.Protocol.value == "" && document.ConfigForm.SourceIPStart.value == "" && document.ConfigForm.SourceIpMask.value == "" && document.ConfigForm.SourcePortStart.value == "" &&
			document.ConfigForm.DestPortStart.value == "")
		{
			 alert("���˹�������Ҫ���ó�Ŀ�ĵ�ַ�������������");
			 return false;			 	
		}
		
		if (document.ConfigForm.WanPath.value.length == 0)
		{
			alert('��ǰû�п�ʹ�õ�WAN���ӣ�');
			return false;
		}	
	}
	
	if (document.ConfigForm.Name.value == '' || document.ConfigForm.Name.value.length > 31 	)
	{
		alert("�������Ʋ���ȷ");
		return false;
	}
	if (isValidName(document.ConfigForm.Name.value) == false)
	{
		alert("�������Ʋ���ȷ");
		return false;
	}
	
	if (document.ConfigForm.SourceIPStart.value != ""  && document.ConfigForm.SourceIPStart.value != "0.0.0.0"  && (isAbcIpAddress(document.ConfigForm.SourceIPStart.value) == false))
	{
		if (isGlobalIpv6Address(document.ConfigForm.SourceIPStart.value) == false)
		{
			alert('Դ��ʼ��ַ "' + document.ConfigForm.SourceIPStart.value + '" ����ȷ');
			return false;
		}
	}
<% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>	
	if (document.ConfigForm.SourceIPStart.value == "" && document.ConfigForm.SourceIPEnd.value != "" )
	{
		msg = '����Դ��ַ��Ϊ��ʱ����������Դ��ʼ��ַ';
		alert(msg);
		return false;
	}
	if (document.ConfigForm.SourceIPEnd.value != ""  && document.ConfigForm.SourceIPEnd.value != "0.0.0.0"  && (isAbcIpAddress(document.ConfigForm.SourceIPEnd.value) == false))
	{
		if (isGlobalIpv6Address(document.ConfigForm.SourceIPEnd.value) == false)
		{
			alert('Դ������ַ "' + document.ConfigForm.SourceIPEnd.value + '" ����ȷ');
			return false;
		}
	}
<% end if %>	
	if (document.ConfigForm.SourceIpMask.value != "" && document.ConfigForm.SourceIpMask.value != "0.0.0.0" && (isValidSubnetMask(document.ConfigForm.SourceIpMask.value) == false))
	{
			alert('Դ��������"' + document.ConfigForm.SourceIpMask.value + '" ����ȷ');
			return false;
	}
	
	if (document.ConfigForm.DestIPStart.value != ""  && document.ConfigForm.DestIPStart.value != "0.0.0.0"  && (isAbcIpAddress(document.ConfigForm.DestIPStart.value) == false))
	{
		if (isGlobalIpv6Address(document.ConfigForm.DestIPStart.value) == false)
		{
			alert('Ŀ����ʼ��ַ "' + document.ConfigForm.DestIPStart.value + '" ����ȷ');
			return false;
		}
	}
<% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>	
	if (document.ConfigForm.DestIPStart.value == "" && document.ConfigForm.DestIPEnd.value != "" )
	{
		msg = 'Ŀ�Ľ�����ַ��Ϊ��ʱ����������Ŀ����ʼ��ַ';
		alert(msg);
		return false;
	}
	if (document.ConfigForm.DestIPEnd.value != ""  && document.ConfigForm.DestIPEnd.value != "0.0.0.0"  && (isAbcIpAddress(document.ConfigForm.DestIPEnd.value) == false))
	{
		if (isGlobalIpv6Address(document.ConfigForm.DestIPEnd.value) == false)
		{
			alert('Ŀ�Ľ�����ַ "' + document.ConfigForm.DestIPEnd.value + '" ����ȷ');
			return false;
		}
	}
<% end if %>		
	if (document.ConfigForm.DestIpMask.value != "" && document.ConfigForm.DestIpMask.value != "0.0.0.0" && (isValidSubnetMask(document.ConfigForm.DestIpMask.value) == false))
	{
		alert('Ŀ����������"' + document.ConfigForm.DestIpMask.value + '" ����ȷ');
		return false;
	}
	
	if (document.ConfigForm.SourceIPStart.value == "" && document.ConfigForm.SourceIpMask.value != "" )
	{
		msg = 'Դ�������벻Ϊ��ʱ����������Դ��ַ';
		alert(msg);
		return false;
	}
	
	if (document.ConfigForm.DestIPStart.value == "" && document.ConfigForm.DestIpMask.value != "" )
	{
		msg = 'Ŀ���������벻Ϊ��ʱ����������Ŀ�ĵ�ַ';
		alert(msg);
		return false;
	}
	
	if (document.ConfigForm.SourcePortStart.value != "" || document.ConfigForm.DestPortStart.value != "")
	{
			if (document.ConfigForm.Protocol.selectedIndex == 0 || document.ConfigForm.Protocol.selectedIndex == 4)
			{		
				msg = 'ֻ��TCP��UDPЭ��������ö˿�';
				alert(msg);
				return false;
			}
	}
	
	if (document.ConfigForm.SourcePortStart.value != "" && document.ConfigForm.SourcePortStart.value != "0" && isValidPort(document.ConfigForm.SourcePortStart.value) == false )
	{
		msg = 'Դ�˿� "' + document.ConfigForm.SourcePortStart.value + '" ����ȷ';
		alert(msg);
		return false;
	}
<% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>	
	if (document.ConfigForm.SourcePortStart.value == "" && document.ConfigForm.SourcePortEnd.value != "" )
	{
		msg = '����Դ�˿ڲ�Ϊ��ʱ������������ʼԴ�˿�';
		alert(msg);
		return false;
	}
	if (document.ConfigForm.SourcePortEnd.value != "" && document.ConfigForm.SourcePortEnd.value != "0" && isValidPort(document.ConfigForm.SourcePortEnd.value) == false )
	{
		msg = '����Դ�˿� "' + document.ConfigForm.SourcePortEnd.value + '" ����ȷ';
		alert(msg);
		return false;
	}
	if(document.ConfigForm.SourcePortStart.value != "" && document.ConfigForm.SourcePortEnd.value != "" && parseInt(document.ConfigForm.SourcePortEnd.value) < parseInt(document.ConfigForm.SourcePortStart.value)){
		msg = '����Դ�˿ڲ���С����ʼ�˿�';
		alert(msg);
		return false;		
	}
<% end if %>
	if (document.ConfigForm.DestPortStart.value != "" && document.ConfigForm.DestPortStart.value != "0" && isValidPort(document.ConfigForm.DestPortStart.value) == false )
	{
		msg = 'Ŀ�Ķ˿� "' + document.ConfigForm.DestPortStart.value + '" ����ȷ';
		alert(msg);
		return false;
	}
<% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>	
	if (document.ConfigForm.DestPortStart.value == "" && document.ConfigForm.DestPortEnd.value != "" )
	{
		msg = '����Ŀ�Ķ˿ڲ�Ϊ��ʱ������������ʼĿ�Ķ˿�';
		alert(msg);
		return false;
	}
	if (document.ConfigForm.DestPortEnd.value != "" && document.ConfigForm.DestPortEnd.value != "0" && isValidPort(document.ConfigForm.DestPortEnd.value) == false )
	{
		msg = '����Ŀ�Ķ˿� "' + document.ConfigForm.DestPortEnd.value + '" ����ȷ';
		alert(msg);
		return false;
	}
	if(document.ConfigForm.DestPortStart.value != "" && document.ConfigForm.DestPortEnd.value != "" && parseInt(document.ConfigForm.DestPortEnd.value) < parseInt(document.ConfigForm.DestPortStart.value)){
		msg = '����Ŀ�Ķ˿ڲ���С����ʼ�˿�';
		alert(msg);
		return false;		
	}
<% end if %>
	if(document.ConfigForm.Current_Flag.value == "in")
	{
		for(var i=0;i<portFilterInIndex;i++)
		{
			/*
			if (document.ConfigForm.Name.value == portFilterIn[i].IPName)
			{
				alert("��ͬ�Ĺ��������Ѵ��ڣ�");
				return false;
			}*/
			if (document.ConfigForm.Protocol.value == portFilterIn[i].Protocol &&
			document.ConfigForm.SourceIPStart.value == portFilterIn[i].SrcIPAddr &&
			document.ConfigForm.SourceIpMask.value == portFilterIn[i].SrcIPMask &&
			document.ConfigForm.SourcePortStart.value == portFilterIn[i].SrcPort &&
			document.ConfigForm.DestIPStart.value == portFilterIn[i].DesIPAddr &&
			document.ConfigForm.DestIpMask.value == portFilterIn[i].DesIPMask &&
			document.ConfigForm.DestPortStart.value == portFilterIn[i].DesPort &&
			document.ConfigForm.WanPath.value == portFilterIn[i].Interface)
			{
				alert("��ͬ�Ĺ��˹����Ѵ��ڣ�");
				return false;
			}
		}
	}
	else	//out
	{
		for(var i=0;i<portFilterOutIndex;i++)
		{
			/*
			if (document.ConfigForm.Name.value == portFilterOut[i].IPName)
			{
				alert("��ͬ�Ĺ��������Ѵ��ڣ�");
				return false;
			}*/
			if (document.ConfigForm.Protocol.value == portFilterOut[i].Protocol &&
			document.ConfigForm.SourceIPStart.value == portFilterOut[i].SrcIPAddr &&
			document.ConfigForm.SourceIpMask.value == portFilterOut[i].SrcIPMask &&
			document.ConfigForm.SourcePortStart.value == portFilterOut[i].SrcPort &&
			document.ConfigForm.DestIPStart.value == portFilterOut[i].DesIPAddr &&
			document.ConfigForm.DestIpMask.value == portFilterOut[i].DesIPMask &&
			document.ConfigForm.DestPortStart.value == portFilterOut[i].DesPort)		
			{
				alert("��ͬ�Ĺ��˹����Ѵ��ڣ�");
				return false;
			}
		}
	}
	
	return true;
}

function btnSaveIP()
{
	var Form = document.ConfigForm;	

	if (checkForm() != false)
	{
		if(Form.Current_Flag.value == "in")
		{
		Form.curNum.value = "<% tcWebApi_get("IpMacFilter","add_ipdownnum","s") %>";
		Form.IpType_Flag.value = "IpDown";
		Form.Direction_Flag.value = "Incoming";
		Form.WanInterfaceFlag.value =  Form.WanPath.value;
		}
		else
		{
		Form.curNum.value = "<% tcWebApi_get("IpMacFilter","add_ipupnum","s") %>";
		Form.Direction_Flag.value = "Outgoing";
		Form.IpType_Flag.value = "IpUp";
		//Form.WanInterfaceFlag.value =  Form.WanPath.value;		
		}
		Form.Save_Flag.value = "1";
		Form.submit();
	}
}
function Cancel()
{
	location.replace('/cgi-bin/sec-portfilter.asp');
}
</script>
                      <script language="JavaScript" type="text/JavaScript">
var nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
var vEntryName = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
var vEntryValue = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
var WANEnNAT = vArrayStr.split(',');

		vEntryName = SortUtil.UpdateWanIFName( vEntryName, nEntryNum );

		vEntryValue = SortUtil.SortMyArray( vEntryValue );
		WANEnNAT = SortUtil.SortMyArray( WANEnNAT );

function ChangeInface()
{
	var index = getElById('WanPath').selectedIndex;
	if (index > -1)
	{
		for (i = 0; i < WANEnNAT.length-1; i++)
		{
			if (vEntryValue[i] == getSelectVal('WanPath'))
			{
				break;
			}
		}
		if (WANEnNAT[i] == "Disabled")
		{
			getElById('NatState').innerHTML = '����';
		}
		else
		{
			getElById('NatState').innerHTML = '����';
		}
	}
}
</script>
                      <br>
                      <b>IP�����</b><br>
                      <br>
                    <table border="0" cellpadding="0" cellspacing="0">
                      <tr> 
                        <td>��������:</td>
                        <td><input type='text' size="18" name="Name" maxLength=31></td>
                      </tr>
                      <tr style="display:none"> 
                        <td>ʹ��:</td>
                        <td> <input type='checkbox' size="18" name="Enable" checked maxLength=16></td>
                      </tr>
                      <tr> 
                        <td>Э��:</td>
                        <td><select name='Protocol' size="1">
                            <option value=""> 
                            <option value="TCP/UDP">TCP/UDP 
                            <option value="TCP">TCP 
                            <option value="UDP">UDP 
                            <option value="ICMP">ICMP </select></td>
                      </tr>
                      <tr> 
                        <td>ԴIP��ַ:</td>
                        <td><input type='text' size="20" name="SourceIPStart" maxLength=15></td>
                      </tr>
                      <tr <% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>style="display:none"<% end if %>> 
                        <td>Դ��������:</td>
                        <td><input type='text' size="20" name="SourceIpMask" maxLength=15></td>
                      </tr>
                      <% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>
                      <tr> 
                        <td>����ԴIP��ַ:</td>
                        <td><input type='text' size="20" name="SourceIPEnd" maxLength=15></td>
                      </tr>                      
                      <% end if %>
                      <tr> 
                        <td>Դ�˿�:</td>
                        <td> <input type='text' size="20" name="SourcePortStart" maxLength=11></td>
                      </tr>
                      <% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>
                      <tr> 
                        <td>����Դ�˿�:</td>
                        <td><input type='text' size="20" name="SourcePortEnd" maxLength=15></td>
                      </tr>                      
                      <% end if %>
                      <tr> 
                        <td>Ŀ��IP��ַ:</td>
                        <td> <input type='text' size="20" name="DestIPStart" maxLength=15></td>
                      </tr>
                      <tr <% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>style="display:none"<% end if %>> 
                        <td>Ŀ����������:</td>
                        <td><input type='text' size="20" name="DestIpMask" maxLength=15></td>
                      </tr>
                      <% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>
                      <tr> 
                        <td>����Ŀ��IP��ַ:</td>
                        <td><input type='text' size="20" name="DestIPEnd" maxLength=15></td>
                      </tr>                      
                      <% end if %>
                      <tr> 
                        <td>Ŀ�Ķ˿�:</td>
                        <td><input type='text' size="20" name="DestPortStart" maxLength=11></td>
                      </tr>
                      <% if tcwebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then %>
                      <tr> 
                        <td>����Ŀ�Ķ˿�:</td>
                        <td><input type='text' size="20" name="DestPortEnd" maxLength=15></td>
                      </tr>                      
                      <% end if %>
                    </table>
                    <div id='interface' style="display:none;"> 
                      <table width="400"  border="0" cellpadding="0" cellspacing="0">
                        <tr> 
                          <td width="79">�ӿ�:</td>
                          <td width="321"> <select name="WanPath" size="1" onChange="ChangeInface()">
                              <script language="JavaScript" type="text/javascript">
							for (i = 0; i < nEntryNum; i++)
							{
								document.write('<option value=' + vEntryValue[i] + '>' + vEntryName[i] + '</option>');
							}
							  
							</script>
                            </select></td>
                        </tr>
                        <tr> 
                          <td  width="79" height="23">NAT״̬:</td>
                          <td><div id="NatState">&nbsp;</div></td>
                        </tr>
                      </table>
                    </div>
                    <br></p>
                    </form>
                    <script language="JavaScript" type="text/javascript">
                    	document.write('<iframe style="display:none" src="/cgi-bin/sec_portfilterinlist.cgi" frameborder="0" width="450" height="220" ></iframe>');
                    	document.write('<iframe style="display:none" src="/cgi-bin/sec_portfilteroutlist.cgi" frameborder="0" width="450" height="220" ></iframe>');
                   	</script>
                            </td>
                <td width="10">&nbsp;</td>
              </tr>
              <tr> 
                <td width="10">&nbsp;</td>
                <td></td>
                <td width="10">&nbsp;</td>
              </tr>
            </table></td>
        </tr>
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
      </table></form>
      </td>
  </tr>
  <tr> 
    <td height="1"> <table border="0" cellpadding="0" cellspacing="0" width="808" height="35" id="table7">
        <tr> 
          <td width="162" bgcolor="#d8d8d8">��</td>
          <td width="278" bgcolor="#0059A0">��</td>
          <td width="196" bgcolor="#0059A0"> <p align="center"><img src="/img/ok.gif" width="80" height="23" border="0" id="btnOK" onClick="btnSaveIP()">&nbsp;&nbsp;<a href="#"><img src="/img/cancel.gif" width="80" height="23" border="0" id="btnCancel" onClick="RefreshPage()"> 
          </td>
          <td width="170" bgcolor="#313031">��</td>
        </tr>
      </table></td>
  </tr>
</table>
</body>
</html>
