<%
if Request_Form("Del_Flag") = "1" then
	TCWebApi_set("WebCurSet_Entry","qos_id","curTypeIdx")
	TCWebApi_set("QoS_Common", "QoSOptType","TypeRuleFlag")	
	if Request_Form("TypeDelFlag0") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type1","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max1","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min1","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag1") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type2","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max2","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min2","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag2") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type3","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max3","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min3","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag3") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type4","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max4","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min4","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag4") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type5","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max5","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min5","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag5") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type6","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max6","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min6","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag6") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type7","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max7","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min7","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag7") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type8","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max8","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min8","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag8") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type9","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max9","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min9","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if
	if Request_Form("TypeDelFlag9") = "Yes" then
		TCWebApi_set("QoS_Entry", "Type10","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Max10","DelNULLFlag")
		TCWebApi_set("QoS_Entry", "Min10","DelNULLFlag")
		TCWebApi_commit("QoS_Entry")
	end if	
	TCWebApi_save()
end if

If Request_Form("QoS_Flag") = "0" then 
	TCWebApi_set("WebCurSet_Entry","qos_id","curAppIdx")
	TCWebApi_set("QoS_Entry","Active","App_Flag")
	TCWebApi_set("QoS_Entry","AppName","AppName")
	TCWebApi_set("QoS_Entry","ActQueue","AppQueueFlag")
	TCWebApi_set("QoS_Common", "QoSOptType","AppRuleFlag")
	
	TCWebApi_commit("QoS_Entry")
	TCWebApi_save()
elseif Request_Form("QoS_Flag") = "1" then
	TCWebApi_set("WebCurSet_Entry","qos_id","curIndex")
elseif Request_Form("QoS_Flag") = "2" then
	TCWebApi_set("WebCurSet_Entry","qos_id","curTypeIdx")
	TCWebApi_set("QoS_Entry","Active","App_Flag")
	TCWebApi_set("QoS_Entry","ActQueue","ClsQueueValueFlag")
	TCWebApi_set("QoS_Entry","ActDSCPRemarking","DSCPMarkValue")
	TCWebApi_set("QoS_Entry","Act8021pRemarkingNum","v8021pValue")
	TCWebApi_set("QoS_Common", "QoSOptType","TypeRuleFlag")
	TCWebApi_commit("QoS_Entry")
	TCWebApi_save()
elseif Request_Form("QoS_Flag") = "3" then
	'tangh
	TCWebApi_set("QoS_Common", "QoSOptType","TypeRuleFlag")
	if Request_Form("ClsDelNo0") = "Yes" then
		TCWebApi_unset("QoS_Entry0")
		TCWebApi_commit("QoS_Entry0")
	end if	
	if Request_Form("ClsDelNo1") = "Yes" then
		TCWebApi_unset("QoS_Entry1")
		TCWebApi_commit("QoS_Entry1")
	end if	
	if Request_Form("ClsDelNo2") = "Yes" then
		TCWebApi_unset("QoS_Entry2")
		TCWebApi_commit("QoS_Entry2")
	end if	
	if Request_Form("ClsDelNo3") = "Yes" then
		TCWebApi_unset("QoS_Entry3")
		TCWebApi_commit("QoS_Entry3")
	end if	
	if Request_Form("ClsDelNo4") = "Yes" then
		TCWebApi_unset("QoS_Entry4")
		TCWebApi_commit("QoS_Entry4")
	end if	
	if Request_Form("ClsDelNo5") = "Yes" then
		TCWebApi_unset("QoS_Entry5")
		TCWebApi_commit("QoS_Entry5")
	end if	
	if Request_Form("ClsDelNo6") = "Yes" then
		TCWebApi_unset("QoS_Entry6")
		TCWebApi_commit("QoS_Entry6")
	end if	
	if Request_Form("ClsDelNo7") = "Yes" then
		TCWebApi_unset("QoS_Entry7")
		TCWebApi_commit("QoS_Entry7")
	end if	
	if Request_Form("ClsDelNo8") = "Yes" then
		TCWebApi_unset("QoS_Entry8")
		TCWebApi_commit("QoS_Entry8")
	end if	
	if Request_Form("ClsDelNo9") = "Yes" then
		TCWebApi_unset("QoS_Entry9")
		TCWebApi_commit("QoS_Entry9")
	end if
	TCWebApi_save()
		
elseif Request_Form("QoS_Flag") = "4" then
	TCWebApi_set("QoS_Common", "QoSOptType","AppRuleFlag")
	if Request_Form("AppDelNo10") = "Yes" then
		TCWebApi_unset("QoS_Entry10")
		TCWebApi_commit("QoS_Entry10")
	end if	
	if Request_Form("AppDelNo11") = "Yes" then
		TCWebApi_unset("QoS_Entry11")
		TCWebApi_commit("QoS_Entry11")
	end if	
	if Request_Form("AppDelNo12") = "Yes" then
		TCWebApi_unset("QoS_Entry12")
		TCWebApi_commit("QoS_Entry12")
	end if	
	if Request_Form("AppDelNo13") = "Yes" then
		TCWebApi_unset("QoS_Entry13")
		TCWebApi_commit("QoS_Entry13")
	end if	
	TCWebApi_save()
	
elseif Request_Form("QoS_Flag") = "33" then
	TCWebApi_set("WebCurSet_Entry","qos_id","curTypeIdx")
	if  TCWebApi_get("QoS_Entry","Type1","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type1","Type")
		TCWebApi_set("QoS_Entry","Max1","Max")
		TCWebApi_set("QoS_Entry","Min1","Min")
	elseif TCWebApi_get("QoS_Entry","Type2","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type2","Type")
		TCWebApi_set("QoS_Entry","Max2","Max")
		TCWebApi_set("QoS_Entry","Min2","Min")
	elseif TCWebApi_get("QoS_Entry","Type3","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type3","Type")
		TCWebApi_set("QoS_Entry","Max3","Max")
		TCWebApi_set("QoS_Entry","Min3","Min")
	elseif TCWebApi_get("QoS_Entry","Type4","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type4","Type")
		TCWebApi_set("QoS_Entry","Max4","Max")
		TCWebApi_set("QoS_Entry","Min4","Min")
	elseif TCWebApi_get("QoS_Entry","Type5","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type5","Type")
		TCWebApi_set("QoS_Entry","Max5","Max")
		TCWebApi_set("QoS_Entry","Min5","Min")
	elseif TCWebApi_get("QoS_Entry","Type6","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type6","Type")
		TCWebApi_set("QoS_Entry","Max6","Max")
		TCWebApi_set("QoS_Entry","Min6","Min")
	elseif TCWebApi_get("QoS_Entry","Type7","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type7","Type")
		TCWebApi_set("QoS_Entry","Max7","Max")
		TCWebApi_set("QoS_Entry","Min7","Min")
	elseif TCWebApi_get("QoS_Entry","Type8","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type8","Type")
		TCWebApi_set("QoS_Entry","Max8","Max")
		TCWebApi_set("QoS_Entry","Min8","Min")
	elseif TCWebApi_get("QoS_Entry","Type9","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type9","Type")
		TCWebApi_set("QoS_Entry","Max9","Max")
		TCWebApi_set("QoS_Entry","Min9","Min")
	elseif TCWebApi_get("QoS_Entry","Type10","h")  = "N/A" then
		TCWebApi_set("QoS_Entry","Type10","Type")
		TCWebApi_set("QoS_Entry","Max10","Max")
		TCWebApi_set("QoS_Entry","Min10","Min")
	end if
	TCWebApi_set("QoS_Entry", "ProtocolID","ProtocolList")
	TCWebApi_set("QoS_Common", "QoSOptType","TypeRuleFlag")
	TCWebApi_commit("QoS_Entry")
	TCWebApi_save()
		
elseif Request_Form("QoS_Flag") = "44" then
	TCWebApi_set("WebCurSet_Entry","qos_id","curTypeIdx")	
	if  Request_Form("TypeModifyID") = "0" then
		TCWebApi_set("QoS_Entry","Type1","Type")
		TCWebApi_set("QoS_Entry","Max1","Max")
		TCWebApi_set("QoS_Entry","Min1","Min")
	elseif Request_Form("TypeModifyID") = "1" then
		TCWebApi_set("QoS_Entry","Type2","Type")
		TCWebApi_set("QoS_Entry","Max2","Max")
		TCWebApi_set("QoS_Entry","Min2","Min")
	elseif Request_Form("TypeModifyID") = "2" then
		TCWebApi_set("QoS_Entry","Type3","Type")
		TCWebApi_set("QoS_Entry","Max3","Max")
		TCWebApi_set("QoS_Entry","Min3","Min")
	elseif Request_Form("TypeModifyID") = "3" then
		TCWebApi_set("QoS_Entry","Type4","Type")
		TCWebApi_set("QoS_Entry","Max4","Max")
		TCWebApi_set("QoS_Entry","Min4","Min")
	elseif Request_Form("TypeModifyID") = "4" then
		TCWebApi_set("QoS_Entry","Type5","Type")
		TCWebApi_set("QoS_Entry","Max5","Max")
		TCWebApi_set("QoS_Entry","Min5","Min")
	elseif Request_Form("TypeModifyID") = "5" then
		TCWebApi_set("QoS_Entry","Type6","Type")
		TCWebApi_set("QoS_Entry","Max6","Max")
		TCWebApi_set("QoS_Entry","Min6","Min")
	elseif Request_Form("TypeModifyID") = "6" then
		TCWebApi_set("QoS_Entry","Type7","Type")
		TCWebApi_set("QoS_Entry","Max7","Max")
		TCWebApi_set("QoS_Entry","Min7","Min")
	elseif Request_Form("TypeModifyID") = "7" then
		TCWebApi_set("QoS_Entry","Type8","Type")
		TCWebApi_set("QoS_Entry","Max8","Max")
		TCWebApi_set("QoS_Entry","Min8","Min")
	elseif Request_Form("TypeModifyID") = "8" then
		TCWebApi_set("QoS_Entry","Type9","Type")
		TCWebApi_set("QoS_Entry","Max9","Max")
		TCWebApi_set("QoS_Entry","Min9","Min")
	elseif Request_Form("TypeModifyID") = "9" then
		TCWebApi_set("QoS_Entry","Type10","Type")
		TCWebApi_set("QoS_Entry","Max10","Max")
		TCWebApi_set("QoS_Entry","Min10","Min")
	end if	
	TCWebApi_set("QoS_Entry", "ProtocolID","ProtocolList")
	TCWebApi_set("QoS_Common", "QoSOptType","TypeRuleFlag")
	TCWebApi_commit("QoS_Entry")
	TCWebApi_save()
	
End if
%>

<html>
<head>
<meta http-equiv="Content-Language" content="zh-cn">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link rel="stylesheet" href='/JS/stylemain.css' type='text/css'>
<title>QoS-Edit</title>
<style type="text/css">
	STYLE2 {font-size: 12px}
</style>
<script language="javascript" src="/JS/menu.js"></script>
<script language="javascript" src="/JS/util.js"></script>

<script language="javascript">

function WriteclsNum()
{
	var clsNums = new Array(4);
	clsNums[0] = "1";
	clsNums[1] = "2";
	clsNums[2] = "3";
	clsNums[3] = "4";
	for(var i=0; i< clsNums.length; i++)
	{
		document.writeln("<option value=" + clsNums[i] + ">" + clsNums[i] + "</option>");
	}
}
</script>

</head>
<body topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000" style="text-align: center" 
onLoad="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()"
onunload="DoUnload()">
<table width="808" height="100%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="1">
<table width="808"  height="117" border="0" cellpadding="0" cellspacing="0" background= "/img/framelogo.jpg" >
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
欢迎您！
</td>
<td width="50" valign="bottom"><a href="/cgi-bin/logout.cgi" target="_top" onClick="DoLogout()">
<span class="logout">
退出
</span>
</a>
</td>
</tr>
</table></td>
</tr>
</table>
<table border="0" cellpadding="0" cellspacing="0" width="808" height="100" id="table2">
<tr>
<td width="163" rowspan="3" align="center" bgcolor="#EF8218" class="LocationDisplay" id="LocationDisplay"></td>
<td height="33" width="434" bgcolor="#427594">
<p align="right"><font color="#FFFFFF" face="黑体"><b><font face="黑体" size="6" color="#FFFFFF">
<input name="Selected_Menu" type="hidden" id="Selected_Menu" value="网络->QoS">
</font></b><span class="GatewayName">网关名称:
<script language="javascript">
document.write(top.gateWayName);
</script>
</span></font></td>
<td height="33" width="211" bgcolor="#EF8218">
<p align="center" class="GatewayType">型号:
<script language="javascript">
document.write(top.ModelName);
</script></td>
</tr>
<tr>
<td height="43" colspan="2"  bgcolor="#EF8218" valign="bottom" id="MenuArea_L1">&nbsp;</td>
</tr>
<tr>
<td colspan="2" height="24" bgcolor="#427594" id="MenuArea_L2"></td>
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
      <table height="100%" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="157" bgcolor="#EF8218" height="29"><p class="Item_L1">QoS</p></td>
<td width="7" bgcolor="#EF8218"></td>
<td width="474">　</td>
<td width="170"  rowspan="5" valign="top" background="/img/panel4.gif"><table width="100%" border="0" cellspacing="0" cellpadding="20">
<tr>
<td><a href="../cgi-bin/help_content.asp#QoS" target="_blank"><img src="/img/help.gif" width="40" height="34" border="0"></a></td>
</tr>
</table>　      </td>
</tr>
<tr>
<td width="157" height="10" valign="top" bgcolor="#E7E7E7"></td>
<td width="7" background="/img/panel3.gif">　</td>
<td></td>
</tr>
<tr>
<td width="157" height="30" valign="top" bgcolor="#E7E7E7"><p class="Item_L2"></p></td>
<td width="7" background="/img/panel3.gif">　</td>
<td>
<form name="ConfigForm" action="/cgi-bin/qos-clsedit.asp" method="post">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="10">&nbsp;</td>
<td>
<p></p>
<div id = 'AppTable'></div>
<table width="100%" border="0">  <tr>
<td width="92">
	  <input type="hidden" name="Wanvalue" value="<% tcWebApi_get("QoS_Entry","nas0","s") %>">
	  <input type="hidden" name="Lanvalue" value="<% tcWebApi_get("QoS_Entry","eth0","s") %>">
	  <input type="hidden" name="DSCPRangevalue" value="<% tcWebApi_get("QoS_Entry","DSCPRangeEnd","s") %>"> 
	  <input type="hidden" name="Dotpvalue" value="<% tcWebApi_get("QoS_Entry","dotpEnd","s") %>">
	  <input type="hidden" name="SIPRangevalue" value="<% tcWebApi_get("QoS_Entry","SrcIPRangeEnd","s") %>">
	  <input type="hidden" name="DIPRangevalue" value="<% tcWebApi_get("QoS_Entry","DesIPRangeEnd","s") %>">
	  <input type="hidden" name="SPORTvalue" value="<% tcWebApi_get("QoS_Entry","SrcPortRangeEnd","s") %>">
	  <input type="hidden" name="DPORTvalue" value="<% tcWebApi_get("QoS_Entry","DesPortRangeEnd","s") %>">
	  <input type="hidden" name="TOSvalue" value="<% tcWebApi_get("QoS_Entry","ActToSRemarking","s") %>">
	  <input type="hidden" name="SMACvalue" value="<% tcWebApi_get("QoS_Entry","SrcMac","s") %>">
	  
	  <input type="hidden" name="TypeDelFlag0" value="No">
	  <input type="hidden" name="TypeDelFlag1" value="No">
	  <input type="hidden" name="TypeDelFlag2" value="No">
	  <input type="hidden" name="TypeDelFlag3" value="No">
	  <input type="hidden" name="TypeDelFlag4" value="No">
	  <input type="hidden" name="TypeDelFlag5" value="No">
	  <input type="hidden" name="TypeDelFlag6" value="No">
	  <input type="hidden" name="TypeDelFlag7" value="No">
	  <input type="hidden" name="TypeDelFlag8" value="No">
	  <input type="hidden" name="TypeDelFlag9" value="No">
	  <input type="hidden" name="DelNULLFlag" value="N/A">
	  <input type="hidden" name="TypeRuleFlag" value="typeRule">
	  <input type="hidden" name="AppRuleFlag" value="appRule">
	  <input type="hidden" name="TypeModifyID" value="-1">	  
<script language="JavaScript" type="text/JavaScript">
function WanIndexConstruction(domain,WanName,ServiceName,EnNAT)
{
	this.domain = domain;
	this.WanName = WanName;
}
function CheckWansActives()
{
	var nCurTemp =0;
	var	vcurLinks = new Array();
	var nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
	var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryName = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryValue = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vNASNAME = vArrayStr.split(',');
	for(var i=0; i<nEntryNum; i++)
	{
			nCurTemp++;
			var wmin1 = 0;
			var wmin2 = 0;
			var temp = vNASNAME[i].replace("nas", "");
			var nindex = temp.indexOf('_');
			wmin1 = 1 + parseInt(temp.substr(0, nindex));
			wmin2 = 1 + parseInt(temp.substr(nindex + 1));
			temp = wmin1.toString() + "," + wmin2.toString();
			vcurLinks[nCurTemp-1] = new WanIndexConstruction(temp, vEntryName[i]);
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

function QosAppConstruction(domain,AppName,ClassQueue)
{
	this.domain = domain;
	this.AppName = AppName;
	this.ClassQueue = ClassQueue;
}

function checkAppActive()
{
	var	ClsCnttemp = 0;
	var vActQueue = new Array(4);
	var vActObj = new Array(4);
	vActQueue[0] = "<% tcWebApi_get("QoS_Entry10","ActQueue","s") %>";
	vActQueue[1] = "<% tcWebApi_get("QoS_Entry11","ActQueue","s") %>";
	vActQueue[2] = "<% tcWebApi_get("QoS_Entry12","ActQueue","s") %>";
	vActQueue[3] = "<% tcWebApi_get("QoS_Entry13","ActQueue","s") %>";

	for(var i=0; i<4; i++)
	{
		switch	 (i)
		{
			case 0:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosAppConstruction("10","<% tcWebApi_get("QoS_Entry10","AppName","s") %>","<% tcWebApi_get("QoS_Entry10","ActQueue","s") %>");
					break;
			case 1:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosAppConstruction("11","<% tcWebApi_get("QoS_Entry11","AppName","s") %>","<% tcWebApi_get("QoS_Entry11","ActQueue","s") %>");
					break;
			case 2:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosAppConstruction("12","<% tcWebApi_get("QoS_Entry12","AppName","s") %>","<% tcWebApi_get("QoS_Entry12","ActQueue","s") %>");
					break;
			case 3:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosAppConstruction("13","<% tcWebApi_get("QoS_Entry13","AppName","s") %>","<% tcWebApi_get("QoS_Entry13","ActQueue","s") %>");
					break;
		}
	}
	var	vObjRet = new Array(ClsCnttemp+1);
	for(var m=0; m<ClsCnttemp; m++)
	{
		vObjRet[m] = vActObj[m];
	}
	vObjRet[ClsCnttemp] = null;
	return vObjRet;
}

var CurApp = checkAppActive();
function QosClassConstruction(domain,ClassQueue,DSCPMarkValue,Value8021P)
{
	this.domain = domain;
	this.ClassQueue = ClassQueue;
	this.DSCPMarkValue = DSCPMarkValue;
	this.Value8021P    = Value8021P;
}

function checkClassActive()
{
	var	ClsCnttemp = 0;
	var vActQueue = new Array(10);
	var vActObj = new Array(10);
	vActQueue[0] = "<% tcWebApi_get("QoS_Entry0","ActQueue","s") %>";
	vActQueue[1] = "<% tcWebApi_get("QoS_Entry1","ActQueue","s") %>";
	vActQueue[2] = "<% tcWebApi_get("QoS_Entry2","ActQueue","s") %>";
	vActQueue[3] = "<% tcWebApi_get("QoS_Entry3","ActQueue","s") %>";
	vActQueue[4] = "<% tcWebApi_get("QoS_Entry4","ActQueue","s") %>";
	vActQueue[5] = "<% tcWebApi_get("QoS_Entry5","ActQueue","s") %>";
	vActQueue[6] = "<% tcWebApi_get("QoS_Entry6","ActQueue","s") %>";
	vActQueue[7] = "<% tcWebApi_get("QoS_Entry7","ActQueue","s") %>";
	vActQueue[8] = "<% tcWebApi_get("QoS_Entry8","ActQueue","s") %>";
	vActQueue[9] = "<% tcWebApi_get("QoS_Entry9","ActQueue","s") %>";
	
	for(var i=0; i<10; i++)
	{
		switch	 (i)
		{
			case 0:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("0","<% tcWebApi_get("QoS_Entry0","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry0","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry0","Act8021pRemarkingNum","s") %>");
					break;
			case 1:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("1","<% tcWebApi_get("QoS_Entry1","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry1","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry1","Act8021pRemarkingNum","s") %>");
					break;
			case 2:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("2","<% tcWebApi_get("QoS_Entry2","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry2","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry2","Act8021pRemarkingNum","s") %>");
					break;
			case 3:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("3","<% tcWebApi_get("QoS_Entry3","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry3","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry3","Act8021pRemarkingNum","s") %>");
					break;
			case 4:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("4","<% tcWebApi_get("QoS_Entry4","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry4","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry4","Act8021pRemarkingNum","s") %>");
					break;
			case 5:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("5","<% tcWebApi_get("QoS_Entry5","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry5","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry5","Act8021pRemarkingNum","s") %>");
					break;
			case 6:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("6","<% tcWebApi_get("QoS_Entry6","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry6","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry6","Act8021pRemarkingNum","s") %>");
					break;
			case 7:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("7","<% tcWebApi_get("QoS_Entry7","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry7","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry7","Act8021pRemarkingNum","s") %>");
					break;	
			case 8:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("8","<% tcWebApi_get("QoS_Entry8","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry8","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry8","Act8021pRemarkingNum","s") %>");
					break;	
			case 9:
				if(vActQueue[i] != "N/A") 
					vActObj[ClsCnttemp++] = new QosClassConstruction("9","<% tcWebApi_get("QoS_Entry9","ActQueue","s") %>","<% tcWebApi_get("QoS_Entry9","ActDSCPRemarking","s") %>","<% tcWebApi_get("QoS_Entry9","Act8021pRemarkingNum","s") %>");
					break;	
		}
	}
	var	vObjRet = new Array(ClsCnttemp);
	for(var m=0; m<ClsCnttemp; m++)
	{
		vObjRet[m] = vActObj[m];
	}
	vObjRet[ClsCnttemp] = null;
	return vObjRet;
}

var CurClassArray =  checkClassActive();
function QosTypeConstruction(domain,Type,Max,Min,ProtocolList)
{
	this.domain = domain;
	this.Type = Type;
	this.Max  = Max;
	this.Min  = Min;
	this.ProtocolList  = ProtocolList;
}

function	checkClsTypestatus()
{
	var	ClsCnttemp = 0;
	var	vType;
	var Protocalvalue = "<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>";
	var vActive = new Array(10);
	var vActObj = new Array(100);	
	
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type1","s") %>","<% tcWebApi_get("QoS_Entry0","Max1","s") %>","<% tcWebApi_get("QoS_Entry0","Min1","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type2","s") %>","<% tcWebApi_get("QoS_Entry0","Max2","s") %>","<% tcWebApi_get("QoS_Entry0","Min2","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type3","s") %>","<% tcWebApi_get("QoS_Entry0","Max3","s") %>","<% tcWebApi_get("QoS_Entry0","Min3","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type4","s") %>","<% tcWebApi_get("QoS_Entry0","Max4","s") %>","<% tcWebApi_get("QoS_Entry0","Min4","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type5","s") %>","<% tcWebApi_get("QoS_Entry0","Max5","s") %>","<% tcWebApi_get("QoS_Entry0","Min5","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type6","s") %>","<% tcWebApi_get("QoS_Entry0","Max6","s") %>","<% tcWebApi_get("QoS_Entry0","Min6","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type7","s") %>","<% tcWebApi_get("QoS_Entry0","Max7","s") %>","<% tcWebApi_get("QoS_Entry0","Min7","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type8","s") %>","<% tcWebApi_get("QoS_Entry0","Max8","s") %>","<% tcWebApi_get("QoS_Entry0","Min8","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type9","s") %>","<% tcWebApi_get("QoS_Entry0","Max9","s") %>","<% tcWebApi_get("QoS_Entry0","Min9","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("0","<% tcWebApi_get("QoS_Entry0","Type10","s") %>","<% tcWebApi_get("QoS_Entry0","Max10","s") %>","<% tcWebApi_get("QoS_Entry0","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");
	
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type1","s") %>","<% tcWebApi_get("QoS_Entry1","Max1","s") %>","<% tcWebApi_get("QoS_Entry1","Min1","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type2","s") %>","<% tcWebApi_get("QoS_Entry1","Max2","s") %>","<% tcWebApi_get("QoS_Entry1","Min2","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type3","s") %>","<% tcWebApi_get("QoS_Entry1","Max3","s") %>","<% tcWebApi_get("QoS_Entry1","Min3","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type4","s") %>","<% tcWebApi_get("QoS_Entry1","Max4","s") %>","<% tcWebApi_get("QoS_Entry1","Min4","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type5","s") %>","<% tcWebApi_get("QoS_Entry1","Max5","s") %>","<% tcWebApi_get("QoS_Entry1","Min5","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type6","s") %>","<% tcWebApi_get("QoS_Entry1","Max6","s") %>","<% tcWebApi_get("QoS_Entry1","Min6","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type7","s") %>","<% tcWebApi_get("QoS_Entry1","Max7","s") %>","<% tcWebApi_get("QoS_Entry1","Min7","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type8","s") %>","<% tcWebApi_get("QoS_Entry1","Max8","s") %>","<% tcWebApi_get("QoS_Entry1","Min8","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type9","s") %>","<% tcWebApi_get("QoS_Entry1","Max9","s") %>","<% tcWebApi_get("QoS_Entry1","Min9","s") %>","<% tcWebApi_get("QoS_Entry1","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("1","<% tcWebApi_get("QoS_Entry1","Type10","s") %>","<% tcWebApi_get("QoS_Entry1","Max10","s") %>","<% tcWebApi_get("QoS_Entry1","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type1","s") %>","<% tcWebApi_get("QoS_Entry2","Max1","s") %>","<% tcWebApi_get("QoS_Entry2","Min1","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type2","s") %>","<% tcWebApi_get("QoS_Entry2","Max2","s") %>","<% tcWebApi_get("QoS_Entry2","Min2","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type3","s") %>","<% tcWebApi_get("QoS_Entry2","Max3","s") %>","<% tcWebApi_get("QoS_Entry2","Min3","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type4","s") %>","<% tcWebApi_get("QoS_Entry2","Max4","s") %>","<% tcWebApi_get("QoS_Entry2","Min4","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type5","s") %>","<% tcWebApi_get("QoS_Entry2","Max5","s") %>","<% tcWebApi_get("QoS_Entry2","Min5","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type6","s") %>","<% tcWebApi_get("QoS_Entry2","Max6","s") %>","<% tcWebApi_get("QoS_Entry2","Min6","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type7","s") %>","<% tcWebApi_get("QoS_Entry2","Max7","s") %>","<% tcWebApi_get("QoS_Entry2","Min7","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type8","s") %>","<% tcWebApi_get("QoS_Entry2","Max8","s") %>","<% tcWebApi_get("QoS_Entry2","Min8","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type9","s") %>","<% tcWebApi_get("QoS_Entry2","Max9","s") %>","<% tcWebApi_get("QoS_Entry2","Min9","s") %>","<% tcWebApi_get("QoS_Entry2","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("2","<% tcWebApi_get("QoS_Entry2","Type10","s") %>","<% tcWebApi_get("QoS_Entry2","Max10","s") %>","<% tcWebApi_get("QoS_Entry2","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type1","s") %>","<% tcWebApi_get("QoS_Entry3","Max1","s") %>","<% tcWebApi_get("QoS_Entry3","Min1","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type2","s") %>","<% tcWebApi_get("QoS_Entry3","Max2","s") %>","<% tcWebApi_get("QoS_Entry3","Min2","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type3","s") %>","<% tcWebApi_get("QoS_Entry3","Max3","s") %>","<% tcWebApi_get("QoS_Entry3","Min3","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type4","s") %>","<% tcWebApi_get("QoS_Entry3","Max4","s") %>","<% tcWebApi_get("QoS_Entry3","Min4","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type5","s") %>","<% tcWebApi_get("QoS_Entry3","Max5","s") %>","<% tcWebApi_get("QoS_Entry3","Min5","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type6","s") %>","<% tcWebApi_get("QoS_Entry3","Max6","s") %>","<% tcWebApi_get("QoS_Entry3","Min6","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type7","s") %>","<% tcWebApi_get("QoS_Entry3","Max7","s") %>","<% tcWebApi_get("QoS_Entry3","Min7","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type8","s") %>","<% tcWebApi_get("QoS_Entry3","Max8","s") %>","<% tcWebApi_get("QoS_Entry3","Min8","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type9","s") %>","<% tcWebApi_get("QoS_Entry3","Max9","s") %>","<% tcWebApi_get("QoS_Entry3","Min9","s") %>","<% tcWebApi_get("QoS_Entry3","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("3","<% tcWebApi_get("QoS_Entry3","Type10","s") %>","<% tcWebApi_get("QoS_Entry3","Max10","s") %>","<% tcWebApi_get("QoS_Entry3","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type1","s") %>","<% tcWebApi_get("QoS_Entry4","Max1","s") %>","<% tcWebApi_get("QoS_Entry4","Min1","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type2","s") %>","<% tcWebApi_get("QoS_Entry4","Max2","s") %>","<% tcWebApi_get("QoS_Entry4","Min2","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type3","s") %>","<% tcWebApi_get("QoS_Entry4","Max3","s") %>","<% tcWebApi_get("QoS_Entry4","Min3","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type4","s") %>","<% tcWebApi_get("QoS_Entry4","Max4","s") %>","<% tcWebApi_get("QoS_Entry4","Min4","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type5","s") %>","<% tcWebApi_get("QoS_Entry4","Max5","s") %>","<% tcWebApi_get("QoS_Entry4","Min5","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type6","s") %>","<% tcWebApi_get("QoS_Entry4","Max6","s") %>","<% tcWebApi_get("QoS_Entry4","Min6","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type7","s") %>","<% tcWebApi_get("QoS_Entry4","Max7","s") %>","<% tcWebApi_get("QoS_Entry4","Min7","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type8","s") %>","<% tcWebApi_get("QoS_Entry4","Max8","s") %>","<% tcWebApi_get("QoS_Entry4","Min8","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type9","s") %>","<% tcWebApi_get("QoS_Entry4","Max9","s") %>","<% tcWebApi_get("QoS_Entry4","Min9","s") %>","<% tcWebApi_get("QoS_Entry4","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("4","<% tcWebApi_get("QoS_Entry4","Type10","s") %>","<% tcWebApi_get("QoS_Entry4","Max10","s") %>","<% tcWebApi_get("QoS_Entry4","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type1","s") %>","<% tcWebApi_get("QoS_Entry5","Max1","s") %>","<% tcWebApi_get("QoS_Entry5","Min1","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type2","s") %>","<% tcWebApi_get("QoS_Entry5","Max2","s") %>","<% tcWebApi_get("QoS_Entry5","Min2","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type3","s") %>","<% tcWebApi_get("QoS_Entry5","Max3","s") %>","<% tcWebApi_get("QoS_Entry5","Min3","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type4","s") %>","<% tcWebApi_get("QoS_Entry5","Max4","s") %>","<% tcWebApi_get("QoS_Entry5","Min4","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type5","s") %>","<% tcWebApi_get("QoS_Entry5","Max5","s") %>","<% tcWebApi_get("QoS_Entry5","Min5","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type6","s") %>","<% tcWebApi_get("QoS_Entry5","Max6","s") %>","<% tcWebApi_get("QoS_Entry5","Min6","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type7","s") %>","<% tcWebApi_get("QoS_Entry5","Max7","s") %>","<% tcWebApi_get("QoS_Entry5","Min7","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type8","s") %>","<% tcWebApi_get("QoS_Entry5","Max8","s") %>","<% tcWebApi_get("QoS_Entry5","Min8","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type9","s") %>","<% tcWebApi_get("QoS_Entry5","Max9","s") %>","<% tcWebApi_get("QoS_Entry5","Min9","s") %>","<% tcWebApi_get("QoS_Entry5","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("5","<% tcWebApi_get("QoS_Entry5","Type10","s") %>","<% tcWebApi_get("QoS_Entry5","Max10","s") %>","<% tcWebApi_get("QoS_Entry5","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type1","s") %>","<% tcWebApi_get("QoS_Entry6","Max1","s") %>","<% tcWebApi_get("QoS_Entry6","Min1","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type2","s") %>","<% tcWebApi_get("QoS_Entry6","Max2","s") %>","<% tcWebApi_get("QoS_Entry6","Min2","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type3","s") %>","<% tcWebApi_get("QoS_Entry6","Max3","s") %>","<% tcWebApi_get("QoS_Entry6","Min3","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type4","s") %>","<% tcWebApi_get("QoS_Entry6","Max4","s") %>","<% tcWebApi_get("QoS_Entry6","Min4","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type5","s") %>","<% tcWebApi_get("QoS_Entry6","Max5","s") %>","<% tcWebApi_get("QoS_Entry6","Min5","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type6","s") %>","<% tcWebApi_get("QoS_Entry6","Max6","s") %>","<% tcWebApi_get("QoS_Entry6","Min6","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type7","s") %>","<% tcWebApi_get("QoS_Entry6","Max7","s") %>","<% tcWebApi_get("QoS_Entry6","Min7","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type8","s") %>","<% tcWebApi_get("QoS_Entry6","Max8","s") %>","<% tcWebApi_get("QoS_Entry6","Min8","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type9","s") %>","<% tcWebApi_get("QoS_Entry6","Max9","s") %>","<% tcWebApi_get("QoS_Entry6","Min9","s") %>","<% tcWebApi_get("QoS_Entry6","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("6","<% tcWebApi_get("QoS_Entry6","Type10","s") %>","<% tcWebApi_get("QoS_Entry6","Max10","s") %>","<% tcWebApi_get("QoS_Entry6","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type1","s") %>","<% tcWebApi_get("QoS_Entry7","Max1","s") %>","<% tcWebApi_get("QoS_Entry7","Min1","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type2","s") %>","<% tcWebApi_get("QoS_Entry7","Max2","s") %>","<% tcWebApi_get("QoS_Entry7","Min2","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type3","s") %>","<% tcWebApi_get("QoS_Entry7","Max3","s") %>","<% tcWebApi_get("QoS_Entry7","Min3","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type4","s") %>","<% tcWebApi_get("QoS_Entry7","Max4","s") %>","<% tcWebApi_get("QoS_Entry7","Min4","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type5","s") %>","<% tcWebApi_get("QoS_Entry7","Max5","s") %>","<% tcWebApi_get("QoS_Entry7","Min5","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type6","s") %>","<% tcWebApi_get("QoS_Entry7","Max6","s") %>","<% tcWebApi_get("QoS_Entry7","Min6","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type7","s") %>","<% tcWebApi_get("QoS_Entry7","Max7","s") %>","<% tcWebApi_get("QoS_Entry7","Min7","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type8","s") %>","<% tcWebApi_get("QoS_Entry7","Max8","s") %>","<% tcWebApi_get("QoS_Entry7","Min8","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type9","s") %>","<% tcWebApi_get("QoS_Entry7","Max9","s") %>","<% tcWebApi_get("QoS_Entry7","Min9","s") %>","<% tcWebApi_get("QoS_Entry7","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("7","<% tcWebApi_get("QoS_Entry7","Type10","s") %>","<% tcWebApi_get("QoS_Entry7","Max10","s") %>","<% tcWebApi_get("QoS_Entry7","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type1","s") %>","<% tcWebApi_get("QoS_Entry8","Max1","s") %>","<% tcWebApi_get("QoS_Entry8","Min1","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type2","s") %>","<% tcWebApi_get("QoS_Entry8","Max2","s") %>","<% tcWebApi_get("QoS_Entry8","Min2","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type3","s") %>","<% tcWebApi_get("QoS_Entry8","Max3","s") %>","<% tcWebApi_get("QoS_Entry8","Min3","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type4","s") %>","<% tcWebApi_get("QoS_Entry8","Max4","s") %>","<% tcWebApi_get("QoS_Entry8","Min4","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type5","s") %>","<% tcWebApi_get("QoS_Entry8","Max5","s") %>","<% tcWebApi_get("QoS_Entry8","Min5","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type6","s") %>","<% tcWebApi_get("QoS_Entry8","Max6","s") %>","<% tcWebApi_get("QoS_Entry8","Min6","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type7","s") %>","<% tcWebApi_get("QoS_Entry8","Max7","s") %>","<% tcWebApi_get("QoS_Entry8","Min7","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type8","s") %>","<% tcWebApi_get("QoS_Entry8","Max8","s") %>","<% tcWebApi_get("QoS_Entry8","Min8","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type9","s") %>","<% tcWebApi_get("QoS_Entry8","Max9","s") %>","<% tcWebApi_get("QoS_Entry8","Min9","s") %>","<% tcWebApi_get("QoS_Entry8","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("8","<% tcWebApi_get("QoS_Entry8","Type10","s") %>","<% tcWebApi_get("QoS_Entry8","Max10","s") %>","<% tcWebApi_get("QoS_Entry8","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type1","s") %>","<% tcWebApi_get("QoS_Entry9","Max1","s") %>","<% tcWebApi_get("QoS_Entry9","Min1","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type2","s") %>","<% tcWebApi_get("QoS_Entry9","Max2","s") %>","<% tcWebApi_get("QoS_Entry9","Min2","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type3","s") %>","<% tcWebApi_get("QoS_Entry9","Max3","s") %>","<% tcWebApi_get("QoS_Entry9","Min3","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type4","s") %>","<% tcWebApi_get("QoS_Entry9","Max4","s") %>","<% tcWebApi_get("QoS_Entry9","Min4","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type5","s") %>","<% tcWebApi_get("QoS_Entry9","Max5","s") %>","<% tcWebApi_get("QoS_Entry9","Min5","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type6","s") %>","<% tcWebApi_get("QoS_Entry9","Max6","s") %>","<% tcWebApi_get("QoS_Entry9","Min6","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type7","s") %>","<% tcWebApi_get("QoS_Entry9","Max7","s") %>","<% tcWebApi_get("QoS_Entry9","Min7","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type8","s") %>","<% tcWebApi_get("QoS_Entry9","Max8","s") %>","<% tcWebApi_get("QoS_Entry9","Min8","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type9","s") %>","<% tcWebApi_get("QoS_Entry9","Max9","s") %>","<% tcWebApi_get("QoS_Entry9","Min9","s") %>","<% tcWebApi_get("QoS_Entry9","ProtocolID","s") %>");
	vActObj[ClsCnttemp++] = new QosTypeConstruction("9","<% tcWebApi_get("QoS_Entry9","Type10","s") %>","<% tcWebApi_get("QoS_Entry9","Max10","s") %>","<% tcWebApi_get("QoS_Entry9","Min10","s") %>","<% tcWebApi_get("QoS_Entry0","ProtocolID","s") %>");

	var	vObjRet = new Array(ClsCnttemp+1);
	for(var m=0; m<ClsCnttemp; m++)
	{
		vObjRet[m] = vActObj[m];
	}
	vObjRet[ClsCnttemp] = null;
	return vObjRet;
}
var CurTypeArray =checkClsTypestatus();
						  </script>
<script language="JavaScript" type="text/javascript">
function QosModeConstruction(domain,Mode,Enable,Bandwidth,Plan,EnableForceWeight,EnableDSCPMark,Enable8021P)
{
this.domain = domain;
this.Mode = Mode;
this.Enable = Enable;
this.Bandwidth = Bandwidth;
this.Plan = Plan;
this.EnableForceWeight = EnableForceWeight;
this.EnableDSCPMark = EnableDSCPMark;
this.Enable8021P = Enable8021P;
}
var CurMode = new Array(new QosModeConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS","OTHER","1","0","<% tcWebApi_get("QoS_Common","Discipline","s") %>","0","0","0"),null);

function QosQueueConstruction(domain,Enable,Priority,Weight)
{
this.domain = domain;
this.Enable = Enable;
this.Priority = Priority;
this.Weight = Weight;
}
var CurQueue = new Array(new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.1","0","1","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.2","0","2","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.3","0","3","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.4","0","4","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.5","0","5","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.6","0","6","0"),null);
function Qos(Mode,App,Class,Queue)
{
this.Mode  = Mode;
this.App   = App;
this.Class = Class;
this.Queue = Queue;
}
var CurQoS = new Qos(CurMode,CurApp,CurClassArray,CurQueue);
var AppCnt = 0;
var ClsCnt = 0;
var ClsTypeCnt = 0;
var QueueCnt= 0;
var AddFlag  = false;
function CurQoSShow()
{
with (getElById('ConfigForm'))
{
setDisplay('AppTable',1);
setDisplay('ClsTable',1);
setDisplay('AddBtn',1);
setDisplay('Cls',0);
AppCnt = CurQoS.App.length - 1;
ClsCnt = CurQoS.Class.length - 1;
QueueCnt = CurQoS.Queue.length - 1;
ClsTypeCnt = CurTypeArray.length - 1;
writeAppTable();
writeClsTable();
}
}
function QosShowInit()
{
	if (CurQoS == null)
	{
		alert("从数据库获取数据失败");
	}
	CurQoSShow();
}
function ClsTypeChange()
{
var AppEdit = getElById('AppEdit');
var ClsEdit = getElById('ClsEdit');
with (getElById('ConfigForm'))
{
setDisplay('Cls',1);
if(AddFlag == true)
{
setText('Max' , 0);
setText('Min' , 0);
}
setDisplay('AppEdit',0);
setDisplay('ClsEdit',1);
}
}
function QoSChangeAppName()
{
}
function QoSChangeProtocolList()
{
}
function QoSChangeQueue()
{
var EditQueue = getElById('EditQueue');
var Priority = getElById('Priority');
Priority.value = EditQueue.selectedIndex + 1;
}
function QoSChangeClassType()
{
setText('Max', 0);
setText('Min', 0);
if(getValue('Type')=="WANInterface")
{
setDisplay('TypeCommon', 0);
setDisplay('lan', 0);
WriteWanInterFace();
setDisplay('wan', 1);
setDisplay('TypeTos', 0);
}
else if(getValue('Type')=="LANInterface")
{
setDisplay('TypeCommon', 0);
setDisplay('wan', 0);
WriteLanInterFace();
setDisplay('lan', 1);
setDisplay('TypeTos', 0);
}
else if(getValue('Type')=="TOS")
{
setDisplay('TypeCommon', 0);
setDisplay('wan', 0);
setDisplay('lan', 0);
setDisplay('TypeTos', 1);
}
else
{
setDisplay('TypeCommon', 1);
setDisplay('lan', 0);
setDisplay('wan', 0);
setDisplay('TypeTos', 0);
}
}
function btnAddCls()
{
AddFlag = true;
with (getElById('ConfigForm'))
{
ClassQueue[1].value = 1;
setText('DSCPMarkValue',0);
setText('v8021pValue',0);
setDisplay('Cls',1);
setDisplay('AppEdit',0);
setDisplay('ClsTypeEdit',0);
setDisplay('ClsEdit',1);
setDisplay('ClsTypeTable',0);
}
}
function btnAddApp()
{
with (getElById('ConfigForm'))
{
AddFlag  = true;
setSelect('AppName','');
ClassQueue[0].value = 1;
setDisplay('Cls',1);
setDisplay('AppEdit',1);
setDisplay('ClsTypeEdit',0);
setDisplay('ClsEdit',0);
setDisplay('ClsTypeTable',0);
}
}
var CurEditAppIndex = 0;
var CurEditClsIndex = 0;
var CurEditTypeIndex = 0;

function btnDelCls()
{
QoSClsDelSubmit();
}
function btnDelApp()
{
QoSAppDelSubmit();
}
function btnDelClsType()
{
QoSTypeDelSubmit();
}
function writeAppTable()
{
var	vForm = document.ConfigForm;
var k,loc,QIndex = 0;
if ( CurQoS.App == null )
{
	return;
}
loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
loc += '<TR align="middle">';
loc += '<TD align="center">业务名称</TD>';
loc += '<TD align="center">队列</TD>';
loc+= '<TD align="center">修改</TD>';
loc+= '<TD width=92><INPUT id=AddCls onclick=btnDelApp() type=button value=删除业务 name=DelApp></TD>'
loc+= '<TD width=92><INPUT id=AddCls onclick=btnAddApp() type=button value=添加业务 name=AddApp></TD>'
loc+= '</TR>';
for(k=0; k < AppCnt; k++)
{
	loc += '<TR align="middle">';
	if (CurQoS.App[k].AppName == "")
	{
		loc += '<TD align="center">&nbsp;</TD>';
	}
	else
	{
		loc += '<TD align="center">'+CurQoS.App[k].AppName +'</TD>';
	}
	loc += '<TD align="center">'+CurQoS.App[k].ClassQueue  +'</TD>';
	loc += '<TD align="center"><input name="EditApp" type="button" id="App_'+ k+'" onClick="btnEditApp(this.id)" value="Edit"/></td>\n';
	loc += '<TD align="center"><input type="checkbox" id="rmapp" name="rmapp" value="false"></td>\n';
	if (k == 0)
	{
		loc += '<TD align="center" rowspan='+AppCnt+'>&nbsp;</TD>';
	}
	loc += '</TR>';
}
	loc += '</TABLE >';
	getElById('AppTable').innerHTML = loc;
}
function stLan(Domain,Name)
{
this.Domain = Domain;
this.Name = Name;
}
var LanList = new Array();
LanList[0] = new stLan("1","LAN1");
LanList[1] = new stLan("2","LAN2");
LanList[2] = new stLan("5","SSID1");
LanList[3] = new stLan("6","SSID2");
LanList[4] = new stLan("7","SSID3");
LanList[5] = new stLan("8","SSID4");
function GetLanName(Domain)
{
	for(i=0;i<6;i++)
	{
		if(Domain == LanList[i].Domain)
		{
			this.Name = LanList[i].Name;
			return this;
		}
	}
}
function stWan(Domain,Name)
{
	this.Domain = Domain;
	this.Name = Name;
}
var WanPPP = new Array(new stWan("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANPPPConnection.2","1_TR069_INTERNET_R_0_35"),null);
var WanIP = new Array(null);
var WanList = new Array();
for (i = 0; i < CurWan.length-1; i++)
{
	WanList[i] = CurWan[i];
}
function GetWanName(Domain)
{
	var i;
	if(WanList!=null)
	{
		if(WanList.length > 0)
		{
			for(i=0;i<WanList.length;i++)
			{
				if(Domain == WanList[i].domain )
				{
					this.Name = WanList[i].WanName;
					return this;
				}
			}
		}
		else
		{
			if(WanList.domain == Domain)
			{
				this.Name = WanList.WanName;
				return this;
			}
		}
	}
}
function test()
{
	for (i = 0;i < WanList.length; i++)
	{
		alert(WanList[i].WanName);
		alert(WanList[i].domain);
	}
}
function GetWanDomain(Name)
{
	var i;
	if(WanList!=null)
	{
		if(WanList.length > 0)
		{
			for(i=0;i<WanList.length;i++)
			{
				if(Name == WanList[i].WanName  )
				{
					this.Domain = WanList[i].domain;
					return this;
				}
			}
		}
		else
		{
			this.Domain = WanList.domain;
			return this;
		}
	}
}
function writeClsTable()
{
	var loc,k;
	loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
	loc += '<TR align="middle">';
	loc += '<TD align="center">分类队列</TD>';
	loc += '<TD align="center">DSCP/TC值</TD>';
	loc += '<TD align="center">802.1p值</TD>';
	loc += '<TD align="center">增加类型</TD>';
	loc += '<TD align="center">修改</TD>';
	loc+= '<TD width=92><INPUT id=AddCls onclick=btnDelCls() type=button value=删除分类 name=DelCls></TD>'
	loc+= '<TD width=92><INPUT id=AddCls onclick=btnAddCls() type=button value=添加分类 name=AddCls></TD>'
	loc += '</TR>';
	if ( CurQoS.Class == null )
	{
		return;
	}
	for(k = 0; k < ClsCnt; k++)
	{
		loc += '<TR align="middle">';
		
		var tmpDSCPMarkValue,tmpValue8021P;
		if(CurQoS.Class[k].DSCPMarkValue == "N/A")
		{
			tmpDSCPMarkValue = "0";			
		}
		else
		{
			tmpDSCPMarkValue = CurQoS.Class[k].DSCPMarkValue;
		}
		
		if(CurQoS.Class[k].Value8021P == "N/A")
		{
			tmpValue8021P = "0";			
		}
		else
		{
			tmpValue8021P = CurQoS.Class[k].Value8021P;
		}
		
		loc += '<TD align="center">' + CurQoS.Class[k].ClassQueue + '</TD>';
		loc += '<TD align="center">' + tmpDSCPMarkValue + '</TD>';
		loc += '<TD align="center">' + tmpValue8021P + '</TD>';
		loc += '<TD align="center"><input name="addtype" type="button" id="Cls_'+ k
		+'" onClick="btnAddClsType(this.id)" value="Add"/></td>\n';
		loc += '<TD align="center"><input name="EditCls" type="button" id="Cls_'+ k
		+'" onClick="btnEditCls(this.id)" value="Edit"/></td>\n';
		loc += '<TD align="center"><input type="checkbox" name="rmcls" id="rmcls" value="false"></td>\n';
		if (k == 0)
		{
			loc += '<TD align="center" rowspan='+ClsCnt+'>&nbsp;</TD>';
		}
		loc += '</TR>';
	}
	loc += '</TABLE>';
	getElById('ClsTable').innerHTML = loc;
}

function writeTypeTable(domain)
{
	var loc,k;
	var TmpStr = 0;
	var Typedomain;
	var val;
	loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
	loc += '<TR align="middle">';
	loc += '<TD align="center">分类类型</TD>';
	loc += '<TD align="center">最小值</TD>';
	loc += '<TD align="center">最大值</TD>';
	loc += '<TD align="center">协议</TD>';
	loc += '<TD align="center">修改</TD>';
	loc+= '<TD width=92><INPUT id=AddCls onclick=btnDelClsType() type=button value=删除类型 name=DelApp></TD>'
	loc += '</TR>';
	if (CurQoS.Class == null)
	{
		return;
	}
	for(k = 0; k < ClsTypeCnt; k++)
	{
		
		if (CurTypeArray[k].domain != domain) continue;
		if (CurTypeArray[k].Type == "N/A") continue;
		
		loc += '<TR align="middle">';
		if(CurTypeArray[k].Type == "DSCP")
		{
			loc += '<TD align="center">' + 'DSCP/TC' + '</TD>';
		}
		else
		{
			loc += '<TD align="center">' + CurTypeArray[k].Type + '</TD>';
		}

		if(CurTypeArray[k].Type == "WANInterface")
		{
			for(i=0;i<WanList.length;i++)
			{
				if (WanList[i].domain == CurTypeArray[k].Min)
				{
					loc += '<TD align="center">' + WanList[i].WanName + '</TD>';
					loc += '<TD align="center">' + WanList[i].WanName + '</TD>';
					break;		
				}
			}
		}
		else if (CurTypeArray[k].Type == "LANInterface")
		{
			var LanMax = new GetLanName(CurTypeArray[k].Max);
			var LanMin = new GetLanName(CurTypeArray[k].Min);
			loc += '<TD align="center">' + LanMin.Name + '</TD>';
			loc += '<TD align="center">' + LanMax.Name + '</TD>';
		}
		else
		{
			loc += '<TD align="center">' + CurTypeArray[k].Min + '</TD>';
			loc += '<TD align="center">' + CurTypeArray[k].Max + '</TD>';		
		}
		
		val = k % 10 ;
		loc += '<TD align="center">' + CurTypeArray[k].ProtocolList + '</TD>';
		loc += '<TD align="center"><input name="EditCls" type="button" id="Type_'+ k
		+'" onClick="btnEditClsType(this.id)" value="Edit"/></td>\n';
		loc += '<TD align="center"><input type="checkbox" name="rmtype" id="rmtype" value="false"><input type="hidden" name="rmValue" value="' + val + '"></td>\n';
		loc += '</TR>';
	}
	loc += '</TABLE>';
	getElById('ClsTypeTable').innerHTML = loc;
}
function WriteWanInterFace()
{
var loc,k;
loc = '<table width="100%" border="0">';
loc +='<tr><td width="90">接口：</td>';
loc +='<td colspan="6">';
loc +='<select size="1" id="TypeWanInterFaceMin" name="TypeWanInterFaceMin">';
if(WanList!=null)
{
if(WanList.length > 0)
{
for(i=0;i<WanList.length;i++)
{
	if (i==0)
		loc +='<option value="'+WanList[i].domain+'" selected>'+ WanList[i].WanName +'</option>';
	else
		loc +='<option value="'+WanList[i].domain+'" >'+ WanList[i].WanName +'</option>';

}
}
}
loc +='</select></td>';
loc +='</tr>';
loc +='</table>';
getElById('wan').innerHTML = loc;
}
function WriteLanInterFace()
{
var loc,k;
loc = '<table width="100%" border="0">';
loc +='<tr><td width="90">最小值：</td>';
loc +='<td colspan="6">';
loc +='<select size="1" id="TypeLanInterFaceMin" name="TypeLanInterFaceMin">';
for(i=0;i<LanList.length;i++)
{
loc +='<option value="'+ LanList[i].Domain +'" selected>'+ LanList[i].Name +'</option>';
}
loc +='</select></td>';
loc +='</tr>';
loc +='<tr><td width="90">最大值：</td>';
loc +='<td colspan="6">';
loc +='<select size="1" id="TypeLanInterFaceMax" name="TypeLanInterFaceMax">';
for(i=0;i<LanList.length;i++)
{
loc +='<option value="'+LanList[i].Domain+'" selected>'+ LanList[i].Name +'</option>';
}
loc +='</select></td>';
loc +='</tr>';
loc +='</table>';
getElById('lan').innerHTML = loc;
}

function btnEditApp(index)
{
	CurEditAppIndex = index.substr(index.indexOf('_') + 1);
	AddFlag  = false;
	with (getElById('ConfigForm'))
	{
		setSelect('AppName',CurQoS.App[CurEditAppIndex].AppName);
		ClassQueue[0].value = CurQoS.App[CurEditAppIndex].ClassQueue;
		setDisplay('Cls',1);
		setDisplay('AppEdit',1);
		setDisplay('ClsTypeEdit',0);
		setDisplay('ClsEdit',0);
		setDisplay('ClsTypeTable',0);
		curAppIdx.value = CurQoS.App[CurEditAppIndex].domain;
	}
}

function btnEditCls(index)
{
	CurEditClsIndex = index.substr(index.indexOf('_') + 1);
	AddFlag  = false;
	with (getElById('ConfigForm'))
	{
		ClassQueue[1].value = CurQoS.Class[CurEditClsIndex].ClassQueue;
		setText('DSCPMarkValue',CurQoS.Class[CurEditClsIndex].DSCPMarkValue);
		setText('v8021pValue',CurQoS.Class[CurEditClsIndex].Value8021P);
		setDisplay('Cls',1);
		setDisplay('AppEdit',0);
		setDisplay('ClsEdit',1);
		setDisplay('ClsTypeEdit',0);
		setDisplay('ClsTypeTable',1);
		document.ConfigForm.curTypeIdx.value = CurQoS.Class[CurEditClsIndex].domain;
		writeTypeTable(CurQoS.Class[CurEditClsIndex].domain);
	}
}

function btnAddClsType(index)
{
	CurEditClsIndex = index.substr(index.indexOf('_') + 1);
	AddFlag  = true;
	with (getElById('ConfigForm'))
	{
		TypeModifyID.value = "-1";	
		setText('Max' , 0);
		setText('Min' , 0);
		setDisplay('Cls',1);
		setDisplay('AppEdit',0);
		setDisplay('ClsEdit',0);
		setSelect('ProtocolList','TCP');
		setDisplay('ClsTypeEdit',1);
		setDisplay('ClsTypeTable',1);
		writeTypeTable(CurQoS.Class[CurEditClsIndex].domain);
		setDisplay('lan', 0);
		setDisplay('wan', 0);
		setDisplay('TypeTos', 0);
		setDisplay('TypeCommon', 1);
	}
}

function btnEditClsType(index)
{
	CurEditTypeIndex = index.substr(index.indexOf('_') + 1);
	AddFlag  = false;
	with (getElById('ConfigForm'))
	{
	TypeModifyID.value = CurEditTypeIndex % 10;
	setSelect('Type',CurTypeArray[CurEditTypeIndex].Type);
	if(getValue('Type')=="WANInterface")
	{
		setDisplay('TypeCommon', 0);
		setDisplay('lan', 0);
		setDisplay('TypeTos', 0);
		WriteWanInterFace();
		setDisplay('wan', 1);
		setSelect('TypeWanInterFaceMin', CurTypeArray[CurEditTypeIndex].Min);
	}
	else if(getValue('Type')=="LANInterface")
	{
		setDisplay('TypeCommon', 0);
		setDisplay('wan', 0);
		setDisplay('TypeTos', 0);
		WriteLanInterFace();
		setDisplay('lan', 1);
		setSelect('TypeLanInterFaceMin', CurTypeArray[CurEditTypeIndex].Min);
		setSelect('TypeLanInterFaceMax', CurTypeArray[CurEditTypeIndex].Max);
	}
	else
	{
		setDisplay('lan', 0);
		setDisplay('wan', 0);
		setDisplay('TypeTos', 0);
		setDisplay('TypeCommon', 1);
		setText('Max', CurTypeArray[CurEditTypeIndex].Max);
		setText('Min', CurTypeArray[CurEditTypeIndex].Min);
	}
	setSelect('ProtocolList',CurTypeArray[CurEditTypeIndex].ProtocolList);
	setDisplay('Cls',1);
	setDisplay('AppEdit',0);
	setDisplay('ClsEdit',0);
	setDisplay('ClsTypeEdit',1);
	}
}

function LoadFrame()
{
	document.ConfigForm.Del_Flag.value = "0";
}

function QoSAppSubmit()
{
	var url;
	if(false == AppCheck())
	{
		return;
	}
	document.ConfigForm.QoS_Flag.value = "0";
	document.ConfigForm.App_Flag.value = "Yes";
	document.ConfigForm.AppQueueFlag.value = document.ConfigForm.ClassQueue[0].value;
	document.ConfigForm.submit();
}

function QoSClsSubmit()
{
	if(false == ClsCheck())
	{
		return;
	}
	document.ConfigForm.QoS_Flag.value = "2";
	document.ConfigForm.App_Flag.value = "Yes";
	document.ConfigForm.ClsQueueValueFlag.value = document.ConfigForm.ClassQueue[1].value;
	document.ConfigForm.submit();
}
function QoSTypeSubmit()
{
	var vForm = document.ConfigForm;
	vForm.curTypeIdx.value = CurQoS.Class[CurEditClsIndex].domain;
	with (getElById('ConfigForm'))
	{
			if(getValue('Type')=="WANInterface")
			{
				vForm.Min.value = vForm.TypeWanInterFaceMin.value;
				vForm.Max.value = vForm.TypeWanInterFaceMin.value;				
			}
			else if(getValue('Type')=="LANInterface")
			{
				vForm.Min.value = vForm.TypeLanInterFaceMin.value;
				vForm.Max.value = vForm.TypeLanInterFaceMax.value;				
			}
			else if(getValue('Type')=="NullType")
			{
				vForm.Min.value = "0";
				vForm.Max.value = "0";				
			}
			
			if (vForm.TypeModifyID.value == "-1")
			{
				vForm.QoS_Flag.value = "33";
			}
			else
			{
				vForm.QoS_Flag.value = "44";
			}			
	}
	if (false == ClsTypeCheck())
	{
		return;
	}
	vForm.submit();
}
function QoSClsDelSubmit()
{
	var Rmapp ;
	var Rmcls ;
	var k = 0;
	var DelClsCount = 0;
	var Domainstr  = 0;
	var indexstr;
	with (getElById('ConfigForm'))
	{
		var vForm = document.ConfigForm;
		Rmcls = getElById('rmcls');//rmcls checkbox control
		if(Rmcls!=null)
		{
			if(Rmcls.length > 0)
			{
				for(k=0; k < Rmcls.length ; k++ )
				{
					if(Rmcls[k].checked == true )
					{
						if(CurQoS.Class[k].domain=="0")
							{ClsDelNo0.value = "Yes";}
						else if(CurQoS.Class[k].domain=="1")
							{ClsDelNo1.value = "Yes";}
						else if(CurQoS.Class[k].domain=="2")
							{ClsDelNo2.value = "Yes";}
						else if(CurQoS.Class[k].domain=="3")
							{ClsDelNo3.value = "Yes";}
						else if(CurQoS.Class[k].domain=="4")
							{ClsDelNo4.value = "Yes";}
						else if(CurQoS.Class[k].domain=="5")
							{ClsDelNo5.value = "Yes";}
						else if(CurQoS.Class[k].domain=="6")
							{ClsDelNo6.value = "Yes";}
						else if(CurQoS.Class[k].domain=="7")
							{ClsDelNo7.value = "Yes";}
						else if(CurQoS.Class[k].domain=="8")
							{ClsDelNo8.value = "Yes";}
						else if(CurQoS.Class[k].domain=="9")
							{ClsDelNo9.value = "Yes";}
						DelClsCount++;		
					}
				}
			}
			else
			{
				if(Rmcls.checked == true )
				{
						if(CurQoS.Class[0].domain=="0")
							{ClsDelNo0.value = "Yes";}
						else if(CurQoS.Class[0].domain=="1")
							{ClsDelNo1.value = "Yes";}
						else if(CurQoS.Class[0].domain=="2")
							{ClsDelNo2.value = "Yes";}
						else if(CurQoS.Class[0].domain=="3")
							{ClsDelNo3.value = "Yes";}
						else if(CurQoS.Class[0].domain=="4")
							{ClsDelNo4.value = "Yes";}
						else if(CurQoS.Class[0].domain=="5")
							{ClsDelNo5.value = "Yes";}
						else if(CurQoS.Class[0].domain=="6")
							{ClsDelNo6.value = "Yes";}
						else if(CurQoS.Class[0].domain=="7")
							{ClsDelNo7.value = "Yes";}
						else if(CurQoS.Class[0].domain=="8")
							{ClsDelNo8.value = "Yes";}
						else if(CurQoS.Class[0].domain=="9")
							{ClsDelNo9.value = "Yes";}
					DelClsCount++;
				}
			}
		}
		if(DelClsCount==0)
		{
			alert("没有选中任何分类");
			return;
		}
		if(!confirm("您确认要删除所选中的分类条件吗?"))
		{
			return;
		}
		vForm.curTypeIdx.value = Domainstr;
		vForm.QoS_Flag.value = "3";
		vForm.App_Flag.value = "No";
		vForm.submit();
	}
}
function QoSAppDelSubmit()
{
	var Rmapp ;
	var Rmcls ;
	var k = 0;
	var DelClsCount = 0;
	var Domainstr ;
	var indexstr;
	with (getElById('ConfigForm'))
	{
		Rmapp = getElById('rmapp');
		if(Rmapp!=null)
		{
			if(Rmapp.length > 0)
			{
				for(k=0; k < Rmapp.length ; k++ )
				{
					if(Rmapp[k].checked == true )
					{
						if(CurQoS.App[k].domain=="10")
							{AppDelNo10.value = "Yes";}
						else if(CurQoS.App[k].domain=="11")
							{AppDelNo11.value = "Yes";}
						else if(CurQoS.App[k].domain=="12")
							{AppDelNo12.value = "Yes";}
						else if(CurQoS.App[k].domain=="13")
							{AppDelNo13.value = "Yes";}
						DelClsCount++;
					}
				}
			}
			else
			{
				if(Rmapp.checked == true )
				{
					if(CurQoS.App[0].domain=="10")
						{AppDelNo10.value = "Yes";}							
					else if(CurQoS.App[0].domain=="11")
						{AppDelNo11.value = "Yes";}							
					else if(CurQoS.App[0].domain=="12")
						{AppDelNo12.value = "Yes";}							
					else if(CurQoS.App[0].domain=="13")
						{AppDelNo13.value = "Yes";}
					DelClsCount++;
				}
			}
		}
		if(DelClsCount==0)
		{
			alert("没有选中任何业务");
			return;
		}
		if(!confirm("您确认要删除所选中的分类条件吗?"))
		{
			return;
		}
		QoS_Flag.value = "4";
		App_Flag.value = "No";
		document.ConfigForm.submit();	
	}
}
function QoSTypeDelSubmit()
{
	var url;
	var Rmapp ;
	var Rmtype ;
	var RmVal ;
	var k = 0;
	var DelClsCount = 0;
	var Domainstr ;
	var TypeIst = new Array();
	var i=0;
	Domainstr = CurQoS.Class[CurEditClsIndex].domain;
	with (getElById('ConfigForm'))
	{
		Rmtype = getElById("rmtype");
		RmVal = getElById("rmvalue");
		if(Rmtype!=null)
		{
			if(Rmtype.length > 0)
			{
				for(k=0; k < Rmtype.length ; k++ )
				{
					if(Rmtype[k].checked == true )
					{
						if(RmVal[k].value =="0")
							{TypeDelFlag0.value =  "Yes";}
						else if(RmVal[k].value =="1")
							{TypeDelFlag1.value =  "Yes";}
						else if(RmVal[k].value =="2")
							{TypeDelFlag2.value =  "Yes";}
						else if(RmVal[k].value =="3")
							{TypeDelFlag3.value =  "Yes";}
						else if(RmVal[k].value =="4")
							{TypeDelFlag4.value =  "Yes";}
						else if(RmVal[k].value =="5")
							{TypeDelFlag5.value =  "Yes";}
						else if(RmVal[k].value =="6")
							{TypeDelFlag6.value =  "Yes";}
						else if(RmVal[k].value =="7")
							{TypeDelFlag7.value =  "Yes";}
						else if(RmVal[k].value =="8")
							{TypeDelFlag8.value =  "Yes";}
						else if(RmVal[k].value =="9")
							{TypeDelFlag9.value =  "Yes";}
						DelClsCount++;
					}
				}
			}
			else
			{
				if(Rmtype.checked == true )
				{
						if(RmVal.value =="0")
							{TypeDelFlag0.value =  "Yes";}
						else if(RmVal.value =="1")
							{TypeDelFlag1.value =  "Yes";}
						else if(RmVal.value =="2")
							{TypeDelFlag2.value =  "Yes";}
						else if(RmVal.value =="3")
							{TypeDelFlag3.value =  "Yes";}
						else if(RmVal.value =="4")
							{TypeDelFlag4.value =  "Yes";}
						else if(RmVal.value =="5")
							{TypeDelFlag5.value =  "Yes";}
						else if(RmVal.value =="6")
							{TypeDelFlag6.value =  "Yes";}
						else if(RmVal.value =="7")
							{TypeDelFlag7.value =  "Yes";}
						else if(RmVal.value =="8")
							{TypeDelFlag8.value =  "Yes";}
						else if(RmVal.value =="9")
							{TypeDelFlag9.value =  "Yes";}
					DelClsCount++;
				}
			}
		}
		if(DelClsCount==0)
		{
			alert("没有选中任何分类");
			return;
		}
		if(!confirm("您确认要删除所选中的分类条件吗?"))
		{
			return;
		}
		Del_Flag.value = "1";
		curTypeIdx.value = Domainstr;
		document.ConfigForm.submit();
	}
}
function btnRtturnQoS()
{
	location.replace("net-qos.asp");
	return;
}
function is_integer(val)
{
	if (/^(\+|-)?\d+$/.test( val ))
	{
		return true;
	}
	else
	{
		return false;
	}
}
function cmpIpAddress(address1,address2)
{
var Lnum = 0;
var Snum = 0;
var addrParts1 = address1.split('.');
var addrParts2 = address2.split('.');
for (var i = 0; i <= 3; i++)
{
Lnum = parseInt(addrParts1[i]);
Snum = parseInt(addrParts2[i]);
if (Lnum < Snum)
{
return false;
}
}
return true;
}
function cmpMacAddress (macM,macS)
{
var Lmac = 0;
var Smac = 0;
var macParts1 = macM.split(':');
var macParts2 = macS.split(':');
for (var i = 0; i <= 3; i++)
{
Lmac = parseInt(macParts1[i], 16);
Smac = parseInt(macParts2[i], 16);
if (Lmac != Smac)
{
return false;
}
}
for (i = 4; i <= 5; i++)
{
Lmac = parseInt(macParts1[i], 16);
Smac = parseInt(macParts2[i], 16);
if (Lmac < Smac)
{
return false;
}
}
return true;
}
function AppCheck()
{
var classQ;
var Classifytype;
var max;
var min;
var dscp;
var v8021p;
var sip;
var dip;
with (getElById('ConfigForm'))
{
	if(AddFlag  == true)
	{
		if(AppCnt >= 5)
		{
			alert("最多可以配置四条APP");
			setDisplay('Cls',0);
			return false;
		}
	}
	if(getValue('AppName')=='')
	{
		return true;
	}
if(AddFlag  == true)
{
for(var i=0;i < AppCnt;i++)
{
if(getValue('AppName')==CurQoS.App[i].AppName)
{
	alert("该APP已经存在");
	return false;
}
}
}
else
{
for(var i=0;i < AppCnt;i++)
{
if(i!=CurEditAppIndex)
{
if(getValue('AppName')==CurQoS.App[i].AppName)
{
	alert("修改后和已经存在的App分类存在冲突，请调整");
return false;
}
}
else if((ClassQueue[0].value == CurQoS.App[CurEditAppIndex].ClassQueue)
&&(getValue('AppName') == CurQoS.App[CurEditAppIndex].AppName))
{
	alert("没有任何修改");
	setDisplay('Cls',0);
return false;
}
}
}
}
return true;
}
function ClsCheck()
{
var classQ;
var Classifytype;
var max;
var min;
var dscp;
var v8021p;
var sip;
var dip;
with (getElById('ConfigForm'))
{
dscp   = getValue('DSCPMarkValue');
v8021p = getValue('v8021pValue');
if (is_integer(dscp) == false ||  dscp < 0 || dscp > 63)
{
alert("DSCP/TC的有效值为0-63区间的整数，请重新输入");

return false;
}
if (is_integer(v8021p) == false ||  v8021p < 0 || v8021p > 7)
{
	alert("802.1p的有效值为0-7区间的整数，请重新输入");
	return false;
}
}
return true;
}

function getValidTypeNum(index,isadd)
{
	var cnt = 0;
	for(var i=0;i<9;i++)
	{
		if(CurTypeArray[index*9 + i].Type != "N/A")
		{
			cnt++;
		}
	}
	if(isadd)
	{
		cnt++;
	}
	return cnt;
}

function ProtocolCheck()
{
	var Typedomain;
	var k;
	var domain    = CurQoS.Class[CurEditClsIndex].domain;
	var protocol  = getSelectVal('ProtocolList');
	for (k = 0; k < ClsTypeCnt; k++)
	{
		if (AddFlag == false)
		{
			if (k == CurEditTypeIndex)
			{
				continue;
			}
		}
		Typedomain = CurTypeArray[k].domain;
		if (Typedomain.indexOf(domain) >= 0)
		{
			if(CurTypeArray[k].ProtocolList != protocol && CurTypeArray[k].ProtocolList !="N/A" )
			{
					if (getValidTypeNum(Typedomain,AddFlag) > 1)
					{
						alert("同一分类下的协议不统一，请调整");
						return false;
					}
			}
		return true;
		}
	}
	return true;
}

function TypeCommonCheck()
{
var Typedomain;
var k;
var domain = CurQoS.Class[CurEditClsIndex].domain;
var ClsType = getSelectVal('Type');
for (k = 0; k < ClsTypeCnt; k++)
{
if(AddFlag  == false)
{
if(k == CurEditTypeIndex)
{
continue;
}
}
Typedomain = CurTypeArray[k].domain;
if (Typedomain.indexOf(domain) >= 0)
{
if (CurTypeArray[k].Type == ClsType)
{
alert("同一分类组合中不允许存在相同的分类类型，请调整");
return false;
}
if ((CurTypeArray[k].Type == 'TOS' && ClsType == 'DSCP')
|| (CurTypeArray[k].Type == 'DSCP' && ClsType == 'TOS'))
{
alert("TOS和DSCP/TC流分类类型不允许组合，请调整");
return false;
}
}
}
return true;
}

function ClsTypeCheck()
{
var classQ;
var Classifytype;
var max;
var min;
var dscp;
var v8021p;
var sip;
var dip;
var k;
with (getElById('ConfigForm'))
{
classQ        = ClassQueue[1].value;
Classifytype  = getValue('Type');
max           = getValue('Max');
min           = getValue('Min');
if (max == '' ||min =='')
{
alert("输入参数不能为空");
return false;
}
if (false == ProtocolCheck())
{
return false;
}
if(false == TypeCommonCheck())
{
return false;
}
if (Classifytype =='SMAC')
{
	if (min != '' && isValidMacAddress(min) == false)
	{
		alert("最小值地址为非法MAC地址");
		return false;
	}
	else if (max != '' && isValidMacAddress(max) == false)
	{
		alert("最大值地址为非法MAC地址");
		return false;
	}
	else if (min != max)
	{
		alert("MAC地址最小值与最大值必须相同");
		return false;
}
}
else if (Classifytype =='8021P')
{
	if (max != '' && (is_integer(max) == false ||  max < 0 || max> 7))
	{
		alert("最大值请输入0至7之间的数字");
		return false;
	}
	else if (min != '' && (is_integer(min) == false ||  min < 0 || min > 7))
	{
		alert("最小值请输入0至7之间的数字");
		return false;
	}
	else if (parseInt(min) > parseInt(max))
	{
		alert("最小值大于最大值，请重新输入");
		return false;
	}
}
else if (Classifytype =='SIP')
{
	if (max != '' && (isAbcIpAddress(max) == false) && (isIpv6Address(max) == false))
	{
		var TipStr = '源IP地址 最大值"' + max + '" 是无效IP地址';
		alert(TipStr);
		return false;
	}
	else if (min != '' && (isAbcIpAddress(min) == false)	&& (isIpv6Address(min) == false))
	{
		var TipStr = '源IP地址 最小值"' + min + '" 是无效IP地址';
		alert(TipStr);
		return false;
	}
	else if ((isAbcIpAddress(min) == true) && (isAbcIpAddress(max) == true)	&& (cmpIpAddress(max ,min) == false))
	{
		var TipStr = '源IP地址 最小值"' + min + '" 大于最大值'+max;
		alert(TipStr);
		return false;
	}
	else if ((isIpv6Address(min) == true)	 || (isIpv6Address(max) == true) )
	{
		if (max != min)
		{
			alert("IPv6地址最小值与最大值必须相同");
			return false;
		}
	}
}
else if (Classifytype =='DIP')
{
	if (max != '' && (isAbcIpAddress(max) == false) && (isIpv6Address(max) == false))
	{
		var TipStr = '目的IP地址 最大值"' + max + '" 是无效IP地址';
		alert(TipStr);
		return false;
	}
	else if (min != '' && (isAbcIpAddress(min) == false) && (isIpv6Address(min) == false))
	{
		var TipStr = '目的P地址 最小值"' + min + '" 是无效IP地址。';
		alert(TipStr);
		return false;
	}
	else if ((isAbcIpAddress(min) == true) && (isAbcIpAddress(max) == true) && (cmpIpAddress(max ,min) == false))
	{
		var TipStr = '目的IP地址 最小值"' + min + '" 大于最大值'+max;
		alert(TipStr);
		return false;
	}
	else if ((isIpv6Address(min) == true)	 || (isIpv6Address(max) == true) )
	{
		if (max != min)
		{
			alert("IPv6地址最小值与最大值必须相同");
			return false;
		}
	}
}
else if ((Classifytype =='DPORT')||(Classifytype =='SPORT'))
{
	if (max != '' && isValidPort(max) == false)
	{
		alert("最大值为无效端口");
		return false;
	}
	else if (min != '' && isValidPort(min) == false)
	{
		alert("最小值为无效端口");
		return false;
	}
	else if (parseInt(min) > parseInt(max))
	{
		alert("端口的最小值大于最大值");
		return false;
	}
}
else if (Classifytype =='DSCP')
{
	if (max != '' && (is_integer(max) == false ||  max < 0 || max> 63))
	{
		alert("DSCP/TC的最大值应为0-63的数值");
		return false;
	}
	else if (min != '' && (is_integer(min) == false ||  min < 0 || min> 63))
	{
		alert("DSCP/TC的最小值应为0-63的数值");
		return false;
	}
	else if (parseInt(min) > parseInt(max))
	{
		alert("DSCP/TC的最小值大于最大值，请重新输入");
		return false;
	}
}
else if (Classifytype =='WANInterface')
{
	return true;
}
else if (Classifytype =='LANInterface')
{
if(TypeLanInterFaceMin.selectedIndex > TypeLanInterFaceMax.selectedIndex)
{
	alert("接口中的最小值比最大值大，请重新选择");
	setDisplay('Cls',0);
	return false;
}
}
}
return true;
}
function QosSubmit()
{
	with (getElById('ConfigForm'))
	{
		Del_Flag.value = "-1";
		QoS_Flag.value = "-1";
	}
	document.ConfigForm.submit();

}
</script> &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;</td>
</tr>
</table>
<div id = 'ClsTable'></div>
<table width="100%" border="0">  <tr>
<td width="92"><input type="hidden" name="QoS_Flag" value="-1">
                          <input type="hidden" name="Del_Flag" value="0">
                          <input type="hidden" name="curAppIdx" value="<% tcWebApi_get("QoS_Common","curAppIdx","s") %>">
                          <input type="hidden" name="curSubTypeIdx" value="<% tcWebApi_get("QoS_Common","curSubTypeIdx","s") %>">
						  <input type="hidden" name="curTypeIdx" value="<% tcWebApi_get("QoS_Common","curTypeIdx","s") %>">
                          <input type="hidden" name="App_Flag" value="Yes">
                          <input type="hidden" name="DiscplineFlag" value="<%if tcWebApi_get("QoS_Common","Discipline","h") <> "N/A" then tcWebApi_get("QoS_Common","Discipline","s") else asp_Write("SP") end if%>">
                          <input type="hidden" name="LanFlag0" value="No">
                          <input type="hidden" name="LanFlag1" value="No">
                          <input type="hidden" name="LanFlag2" value="No">
                          <input type="hidden" name="LanFlag3" value="No">
                          <input type="hidden" name="LanFlag4" value="No">
                          <input type="hidden" name="LanFlag5" value="No">
                          <input type="hidden" name="LanFlag6" value="No">
                          <input type="hidden" name="LanFlag7" value="No">
                          <input type="hidden" name="WanFlag0" value="No">
                          <input type="hidden" name="WanFlag1" value="No">
                          <input type="hidden" name="WanFlag2" value="No">
                          <input type="hidden" name="WanFlag3" value="No">
                          <input type="hidden" name="WanFlag4" value="No">
                          <input type="hidden" name="WanFlag5" value="No">
                          <input type="hidden" name="WanFlag6" value="No">
                          <input type="hidden" name="WanFlag7" value="No"> 
                          <script language="JavaScript" type="text/JavaScript">
						  </script>
                          &nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
</tr>
</table>
<div id = 'ClsTypeTable'></div>
<div id="AddBtn">
<table width="100%" border="0">  <tr>
</tr>
</table>
</div>
<td width="10">&nbsp;</td>
</tr>
<tr>
<td width="10">&nbsp;</td>
<td>
<table border="1" cellpadding="3" cellspacing="0" width='462'>
</table>
<div id="Cls">
<div id="AppEdit">
<table width="100%" border="0">  <tr>
<td width="92" height="30">业务名称：</td>
<td width="381" colspan="6">
<select size="1" id="AppName" name="AppName" onChange="QoSChangeAppName()">
<option value="TR069" <%if tcWebApi_get("QoS_Entry","AppName","h") = "TR069" then asp_Write("selected") end if%>>TR069</option>
<option value="IPTV" <%if tcWebApi_get("QoS_Entry","AppName","h") = "IPTV" then asp_Write("selected") end if%>>IPTV</option>
<option value="VOIP" <%if tcWebApi_get("QoS_Entry","AppName","h") = "VOIP" then asp_Write("selected") end if%>>VOIP</option>
<option value="" <%if tcWebApi_get("QoS_Entry","AppName","h") = "N/A" then asp_Write("selected") end if%>></option>
</select>
<input type="hidden" name="AppDelNo10" value="No">
<input type="hidden" name="AppDelNo11" value="No">
<input type="hidden" name="AppDelNo12" value="No">
<input type="hidden" name="AppDelNo13" value="No">
<input type="hidden" name="ClsDelNo0" value="No">
<input type="hidden" name="ClsDelNo1" value="No">
<input type="hidden" name="ClsDelNo2" value="No">
<input type="hidden" name="ClsDelNo3" value="No">
<input type="hidden" name="ClsDelNo4" value="No">
<input type="hidden" name="ClsDelNo5" value="No">
<input type="hidden" name="ClsDelNo6" value="No">
<input type="hidden" name="ClsDelNo7" value="No">
<input type="hidden" name="ClsDelNo8" value="No">
<input type="hidden" name="ClsDelNo9" value="No">

</td>
</tr>
</table>
<table id = 'CarQueue' width="100%" border="0">  <tr>
<td width="92">分类队列：</td>
<td width="381" colspan="6">
<select size="1" id="ClassQueue" name="ClassQueue">
<option value="1">1</option>
<option value="2">2</option>
<option value="3">3</option>
<option value="4">4</option>
<script language="JavaScript" type="text/javascript">
if (document.ConfigForm.DiscplineFlag.value == "CAR")
{
document.write("<option value=\"5\">5</option>");
document.write("<option value=\"6\">6</option>");
}
</script>
</select>
                              <input type="hidden" name="AppQueueFlag" value="1"> </td>
</tr>
</table>
<table width="100%" border="0">
<tr>
<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input name="AddConfirm" type="button" id="AddConfirm" onClick="QoSAppSubmit()" value="提交"/>      </td>
</tr>
</table>
</div>
<div id="ClsEdit">
<table id = 'CarQueue1' width="100%" border="0">  <tr>
<td width="92">分类队列：</td>
<td width="381" colspan="6">
<select size="1" id="ClassQueue" name="ClassQueue">
<option value="1" selected>1</option>
<option value="2">2</option>
<option value="3">3</option>
<option value="4">4</option>
<script language="JavaScript" type="text/javascript">
if (document.ConfigForm.DiscplineFlag.value == "CAR")
{
	document.write("<option value=\"5\">5</option>");
	document.write("<option value=\"6\">6</option>");
}
</script>
</select>
                              <input type="hidden" name="ClsQueueValueFlag" value="1"> </td>
</tr>
</table>
<table width="100%" border="0">  <tr>
<td width="92">DSCP/TC值：</td>
<td width="384"><input name="DSCPMarkValue" type="text" id="DSCPMarkValue" value="0" size="10" maxlength="8"> </td>
</tr>
</table>
<table width="100%" border="0">  <tr>
<td width="96">802.1P值：</td>
<td width="405"><input name="v8021pValue" type="text" id="v8021pValue" value="0" size="10" maxlength="8"> </td>
</tr>
</table>
<table width="100%" border="0">
<tr>
<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input name="AddConfirm" type="button" id="AddConfirm" onClick="QoSClsSubmit()" value="提交"/>      </td>
</tr>
</table>
</div>
<div id="ClsTypeEdit">
<table width="100%" border="0">  <tr>
<td width="93">分类类型：</td>
<td width="394" colspan="6">
<select size="1" id="Type" name="Type" onChange="QoSChangeClassType()">
<option value="NullType"></option>
<option value="SMAC">SMAC</option>
<option value="8021P">8021P</option>
<option value="SIP">SIP</option>
<option value="DIP">DIP</option>
<option value="SPORT">SPORT</option>
<option value="DPORT">DPORT</option>
<option value="DSCP">DSCP/TC</option>
<option value="WANInterface">WANInterface</option>
<option value="LANInterface">LANInterface</option>
</select>
</td>
</tr>
</table>
<div id="wan">        </div>
<div id="lan">        </div>
<div id="TypeTos">        </div>
<table width="100%" border="0" id="TypeCommon">
<tr><td width="100">最小值：</td>
<TD width=401><INPUT id="Min" maxLength="128" value="" size="40" name="Min"></TD>
</tr>
<tr>
<td width="100">最大值：</td>
<TD width=401><INPUT id="Max" maxLength="128" value="" size="40" name="Max"></TD></tr>
</table>
<table width="104%" border="0">  <tr>
<td width="94">协议类型：</td>
<td width="407" colspan="6">
<select size="1" id="ProtocolList" name="ProtocolList" onChange="QoSChangeProtocolList()">
<option value="TCP" >TCP</option>
<option value="UDP" >UDP</option>
<option value="TCP,UDP" >TCP,UDP</option>
<option value="ICMP" >ICMP</option>
<option value="rtp" >RTP</option>
<option value="all" >ALL</option>
</select>	</td>
</tr>
</table>
<table width="100%" border="0">
<tr>
<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input name="AddConfirm" type="button" id="AddConfirm" onClick="QoSTypeSubmit()" value="提交"/>      </td>
</tr>
</table>
</div>
</div>
<table width="100%" border="0">
<tr>
<td>&nbsp;</td>
</tr>
<tr>
<td width="92" height="26" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input name="RtturnQoS" type="button" id="RtturnQoS" onClick="btnRtturnQoS()" value="返回队列编辑页面"/></td>
</tr>
</table>
<SCRIPT language=JavaScript type=text/javascript>
QosShowInit();
</SCRIPT>
<td width="10">&nbsp;</td>
</tr>
</table>
</form>
</td>
</tr>
<tr>
<td width="157" height="10" valign="top" bgcolor="#E7E7E7"></td>
<td width="7" background="/img/panel3.gif">　</td>
<td></td>
</tr>
<tr>
<td width="157" height="" valign="top" bgcolor="#E7E7E7"></td>
<td width="7" background="/img/panel3.gif">　</td>
<td></td>
</tr>
</table>
</td>
</tr>
<tr>
<td height="1">
<table border="0" cellpadding="0" cellspacing="0" width="808" height="35" id="table7">
<tr>
<td width="162" bgcolor="#EF8218">　</td>
<td width="278" bgcolor="#427594">　</td>
<td width="196" bgcolor="#427594">
<p align="center"><img src="/img/ok.gif" width="80" height="23" border="0" id="btnOK" onClick="QosSubmit()">&nbsp;
<IMG id=btnCancel height=23 src="/img/cancel.gif" onclick="RefreshPage()" width=80 border=0> </td>
<td width="170" bgcolor="#313031">　</td>
</tr>
</table>
</td>
</tr>
</table>
</body>
</html>
