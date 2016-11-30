<%
if Request_Form("Save_Flag") = "1" then 
	TCWebApi_set("WebCurSet_Entry","ipfilter_id","curNum")
	TCWebApi_set("IpMacFilter_Common","Action","Actionflag")
	TCWebApi_set("IpMacFilter_Common","IpMacType","IpMacType_Flag")
	TCWebApi_set("IpMacFilter_Entry","Active","EnableMac_Flag")
	TCWebApi_set("IpMacFilter_Entry","RuleType","RuleType_Flag")
	TCWebApi_set("IpMacFilter_Entry","MacName","Name")
	TCWebApi_set("IpMacFilter_Entry","MacAddr","SourceMACAddress")
	TCWebAPI_set("IpMacFilter_Entry","Interface","Interface_Flag")
	TCWebAPI_set("IpMacFilter_Entry","Direction","Direction_Flag")
	
	TCWebApi_commit("IpMacFilter_Entry")
	TCWebApi_save()
end if
%>
<html>
<head>
<meta http-equiv="Content-Language" content="zh-cn">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link rel="stylesheet" href='/JS/stylemain.css' type='text/css'>
<title>添加MAC过滤</title>
<script language="javascript" src="/JS/menu.js"></script>
<script language="javascript" src="/JS/util.js"></script>
<script language="JavaScript" type="text/JavaScript">
<% if Request_Form("Save_Flag") = "1" then %>
	location.replace("/cgi-bin/sec-macfilter.asp");
<%end if%>
</script>
</head>
<body topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000" style="text-align: center" onLoad="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()"
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
<input name="Selected_Menu" type="hidden" id="Selected_Menu" value="安全->MAC过滤">
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
<script language="JavaScript" type="text/javascript">
	
var MacFilter = [null];
function LoadFrame()
{
	with (getElById('ConfigForm'))
	{
		Save_Flag.value = "0";
	}
}
function CheckForm(type)
{
with (getElById('ConfigForm'))
{
if (Name.length > 31)
{
alert("过滤名称不能超过31个字符!")
return false;
}
/*for (i = 0; i < MacFilter.length - 1; i++)
{
if (MacFilter[i][1] == Name.value)
{
alert('过滤名称不能重复！');
return false;
}
}*/
if (Name.value == '')
{
alert("过滤名称不能为空");
return false;
}
if (isValidName(Name.value) == false)
{
alert("过滤名称不正确");
return false;
}
if (SourceMACAddress.value == '')
{
alert("MAC地址不正确");
return false;
}
if (isValidMacAddress(SourceMACAddress.value) == false )
{
alert('MAC地址 "' + SourceMACAddress.value + '" 不正确,正确格式为(AA:BB:CC:DD:EE:FF)');
return false;
}
}
return true;
}
function AddSubmitParam(SubmitForm,type)
{
SubmitForm.addForm('ConfigForm','x');
SubmitForm.setAction('addcfg.cgi?x=InternetGatewayDevice.X_ATP_Security.MacFilter'
+ '&RequestFile=html/security/macfilter.asp');
}
function Cancel()
{
	location.replace('sec-macfilter.asp');
}
</script>
<script language="javascript" for="document" event="onkeydown">
if(event.keyCode==13)
{
SubmitForm();
return false;
}
</script>
<table height="100%" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="157" bgcolor="#EF8218" height="30"><p class="Item_L1">MAC过滤</p></td>
<td width="7" bgcolor="#EF8218">　</td>
<td width="474">　</td>
<td width="170" rowspan="5" valign="top" background="/img/panel4.gif"><table width="100%" border="0" cellspacing="0" cellpadding="20">
<tr>
<td><a href="/cgi-bin/help_content.asp#MAC过滤" target="_blank"><img src="/img/help.gif" width="40" height="34" border="0"></a></td>
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
<form name="ConfigForm" action="/cgi-bin/sec-addmacfilter.asp" method="post">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="10">&nbsp;</td>
<td>
<b>MAC地址接入控制</b><br>

                    <input type="hidden" name="Save_Flag" value="0">
                    <input type="hidden" name="EnableMac_Flag" value="Yes">
                    <input type="hidden" name="curNum" value="<% tcWebApi_get("IpMacFilter","add_macnum","s") %>">
                    <input type="hidden" name="RuleType_Flag" value="MAC">
					<input type="hidden" name="Direction_Flag" value="Incoming">
                    <input type="hidden" name="IpMacType_Flag" value="Mac">
                    <input type="hidden" name="Actionflag" value="Add">
                    <input type="hidden" name="Interface_Flag" value="br0"> 
                    <script language="JavaScript" type="text/JavaScript">

function btnSubmit()
{
	var Form = document.ConfigForm;
	if(CheckForm(0) == false)
		return;
	Form.Save_Flag.value = "1";
	Form.submit();
}		
</script>
                    <br>
                    <br>
<table border="0" cellpadding="0" cellspacing="0">
<tr>
<td>过滤规则名称：</td>
<td><input type='text' name="Name" id="Name" maxlength='31'></td>
<td>&nbsp;</td>
</tr>
<tr>
<td>MAC地址：</td>
<td><input type='text' name="SourceMACAddress" id="SourceMACAddress" maxlength='17'></td>
<td>（格式：AA:BB:CC:DD:EE:FF）</td>
</tr>
<tr>
<td style="display:none"><br>
使能：
<input type='checkbox' id="Enable" name="Enable" checked></td>
</tr>
</table>
<br><br>
</td>
<td width="10">&nbsp;</td>
</tr>
<tr>
<td width="10">&nbsp;</td>
<td></td>
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
<p align="center"><img src="/img/ok.gif" width="80" height="23" border="0" id="btnOK" onClick="btnSubmit()">&nbsp;&nbsp;<img src="/img/cancel.gif" width="80" height="23" border="0" id="btnCancel" onClick="RefreshPage()"> </td>
<td width="170" bgcolor="#313031">　</td>
</tr>
</table>
</td>
</tr>
</table>
</body>
</html>
