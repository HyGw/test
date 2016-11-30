<%
tcWebApi_constSet("WebCurSet_Entry", "isLOIDLocked", "No")
if tcWebApi_get("WebCustom_Entry", "isCTPONYN1PortSupported", "h") = "Yes"  then
	tcWebApi_constSet("WebCurSet_Entry", "isLOIDLocked", "Yes")
elseif tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes"  then
	if tcwebApi_get("WebCurSet_Entry","CurrentAccess","h") <> "0" then
		tcWebApi_constSet("WebCurSet_Entry", "isLOIDLocked", "Yes")
	end if
end if

if Request_Form("SaveFlag") = "1" then

	TCWebApi_set("Cwmp_Entry","Active","ActiveCwmpFlag")
	if Request_Form("isMidwareSupported") = "1" then
		TCWebApi_set("Cwmp_Entry","Tr069Enable","EnTr069Flag")
		if Request_Form("EnTr069Flag") <> "1" then
			TCWebApi_set("Cwmp_Entry","MWMgtUrl","MiddlewareAddress" )
			TCWebApi_set("Cwmp_Entry","MWMgtPort","MiddlewarePort" )
		end if
	end if

	TCWebApi_set("Cwmp_Entry","periodActive","PeriodicInformEnable")
		TCWebApi_set("Cwmp_Entry","periodInterval","PeriodicInformInterval" )
		TCWebApi_set("Cwmp_Entry","prdInformTime","PeriodicInformTime" )
		TCWebApi_set("Cwmp_Entry","acsUrl","URL" )
		TCWebApi_set("Cwmp_Entry","acsUserName","Username" )
		TCWebApi_set("Cwmp_Entry","acsPassword","Password" )
		TCWebApi_set("Cwmp_Entry","conReqUserName","ConnectionRequestUsername" )
		TCWebApi_set("Cwmp_Entry","conReqPassword","ConnectionRequestPassword" )
		TCWebApi_set("Cwmp_Entry","EnableCert","X_ATP_SSLCertEnable")
	TCWebApi_commit("Cwmp_Entry")
	TCWebApi_save()

elseif Request_Form("postflag") = "1" then
	if tcwebApi_get("WebCustom_Entry","isCTPONNMGSupported","h") <> "Yes" then
		TCWebApi_set("SslCA_Flag","UpgradeFlag","fileIndex")
		TCWebApi_commit("SslCA")
	'tcWebApi_set("System_Entry","upgrade_fw","fileIndex")
	'tcWebApi_Commit("System_Entry")
	end if
elseif Request_Form("SubmitFlag") = "1" then
if tcWebApi_get("WebCurSet_Entry", "isLOIDLocked", "h") <> "Yes"  then
if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes" then
		TCWebApi_set("GPON_ONU","Password","LoidPassword")
else
		TCWebApi_set("EPON_LOIDAuth","LOID0","LoidUsername")
		TCWebApi_set("EPON_LOIDAuth","Password0","LoidPassword")
		TCWebApi_set("GPON_LOIDAuth","LOID","LoidUsername")
		TCWebApi_set("GPON_LOIDAuth","Password","LoidPassword")
end if
		TCWebApi_save()
if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes" then
	TCWebApi_commit("GPON_ONU")
else
		TCWebApi_commit("EPON_LOIDAuth")
		TCWebApi_commit("GPON_LOIDAuth")
end if
end if
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>远程管理</TITLE>
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
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% end if %> 
        <TBODY>
        <TR>
          <TD>&nbsp;</TD>
          <TD vAlign=bottom align=right width=358>
            <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR>
                <TD vAlign=bottom align=right><SPAN class=curUserName>&nbsp; 
                  </SPAN></TD>
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <tr>
        	<TD width=808 bgColor=#d8d8d8 height=2 colSpan=3></td>
        	</tr>	
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#d8d8d8 rowSpan=3></TD>
          <TD width=434 bgColor=#0059A0 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->远程管理" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#d8d8d8 height=33>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#d8d8d8 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#0059A0 colSpan=2 
      height=24></TD>
</TR></TBODY></TABLE>
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
function stCWMP(domain,InformEnb,Interval,URL,Username,Password,CntReqName,CntReqPwd,sslCertEnbl)
{
this.domain = domain;
this.InformEnb = InformEnb;
this.Interval = Interval;
this.URL = URL;
this.Username = Username;
this.Password = Password;
this.CntReqName = CntReqName;
this.CntReqPwd = CntReqPwd;
this.sslCertEnbl = sslCertEnbl;
}
function stmid(domain,midurl, tr069enable)
{
this.domain = domain;
this.Url = midurl;
this.Tr069enable = tr069enable;
}
var URLAccessAttr = 'Subscriber';
var UsrAccessAttr = 'Subscriber';
var PwdAccessAttr = 'Subscriber';
var conUsrAccessAttr = 'Subscriber';
var conPwdAccessAttr = 'Subscriber';
var midcom = new Array(new stmid("InternetGatewayDevice.DeviceInfo.X_CT-COM_MiddlewareMgt","0.0.0.0:0","1"),null);
var mid = midcom[0];
var temp = new Array(new stCWMP("InternetGatewayDevice.ManagementServer","1","20","http://122.193.99.166:80/comserver/node1/tr069","hgw","hgw","itms","itms","0"),null);
var cwmp = temp[0];
var msg = new Array(6);

var midLastRadioValue = "0";
<% if tcwebApi_get("WebCurSet_Entry","CurrentAccess","h") = "0" then %>
msg[0] = "上传证书文件成功！";
msg[1] = "证书文件非法,上传证书文件失败！";
msg[2] = "证书功能被禁止！";
msg[3] = "证书功能已启用！";
msg[4] = "证书文件不存在,请先上传！";
msg[5] = "证书文件文件太大，上传失败！";
<% end if %>

<%if tcWebApi_get("WebCustom_Entry", "isCTPONCYSupported", "h") = "Yes"  then%>
function trimspace(str)
{
	var strTemp = new String(str);
	while (-1 != strTemp.search(" "))
	{
		strTemp = strTemp.replace(" ", "");
	}
	return strTemp;
}
<%end if%>

function LoadFrame()
{
<% if tcwebApi_get("WebCurSet_Entry","CurrentAccess","h") = "0" then %>
	document.ConfigForm.SaveFlag.value = "0";
	document.SslForm.postflag.value = "0";
<% end if %>
	document.OLTForm.SubmitFlag.value = "0";
	<% if tcwebApi_get("WebCustom_Entry","isCTPONNMGSupported","h") <> "Yes" then %>
	var obj = getElById('ResultText');
	var certStatus = parseInt("<% tcWebApi_get("Cwmp_Entry","CertCode","s") %>");
	
	if ((certStatus >= 0) && (certStatus <= 9))
	{
		obj.innerHTML = '<font color="#FF0000">' + msg[certStatus] + '</font>';
		setDisplay('ResultText',1);
	}
	else
	{
		setDisplay('ResultText', 0);
	}
	<% end if %>
	midLastRadioValue = document.ConfigForm.EnTr069Flag.value;
	
	if(midLastRadioValue == "0")
	{
		document.ConfigForm.Tr069Enable[0].checked = true;
		setDisplay("midurl", 1);
	}
	else if(midLastRadioValue == "2")
	{
		document.ConfigForm.Tr069Enable[2].checked = true;
		setDisplay("midurl", 1);
	}
	else
	{
		document.ConfigForm.Tr069Enable[1].checked = true;
		setDisplay("midurl", 0);
	}
}
function isSafeStringSN(val)
{
if ( val == "" )
{
return false;
}
for ( var j = 0 ; j < val.length ; j++ )
{
if ( !isSafeCharSN(val.charAt(j)) )
{
return false;
}
}
return true;
}
function isSafeCharSN(val)
{
if (((val >= 'A')&&(val <= 'Z'))
||((val >= 'a')&&(val <= 'z'))
||((val >= '0')&&(val <= '9'))
||(val == ':')||(val == '/')
||(val == '-')||(val == '.')
||(val == '_'))
{
return true;
}
else
{
return false;
}
}
function isTimeFormat(str)
{
	if(str.length > 20 || str.length < 19)	//0001-01-01T00:00:00Z
		return false;

	if(str.substr(10, 1) != 'T' || str.substr(4, 1) != '-' || str.substr(7, 1) != '-' ||　str.substr(13, 1) != ':' || str.substr(16, 1) != ':' || (str.substr(19, 1) != 'Z' && str.length == 20))
		return false;
	
	if(str.substr(0, 4) < '0001' || str.substr(0, 4) > '9999')	
		return false;
			
	if(str.substr(5, 2) < '01' || str.substr(5, 2) > '12')
		return false;
			
	if(str.substr(8, 2) < '01' || str.substr(8, 2) > '31')	
		return false;
			
	if(str.substr(11, 2) < '00' || str.substr(11, 2) > '23')	
		return false;		

	if(str.substr(14, 2) < '00' || str.substr(14, 2) > '59')	
		return false;		

	if(str.substr(17, 2) < '00' || str.substr(17, 2) > '59')	
		return false;	
				
	return true;
	
}
function isSafeStringSN(val)
{
if (val == "")
{
return true;
}
for ( var j = 0 ; j < val.length ; j++ )
{
if ( !isSafeCharSN(val.charAt(j)) )
{
return false;
}
}
return true;
}
function CheckForm(type)
{
with (getElById ("ConfigForm"))
{
if (!isSafeStringSN(URL.value))
{
alert("ACS必须是英文字母、数字和下面字符的组合: \":, /, -, .\"");
return false;
}
if (PeriodicInformEnable[1].checked == true &&PeriodicInformInterval.value =='' )
{
alert("周期通知时间间隔无效");
return false;
}
if ((PeriodicInformInterval.value == '') || (isPlusInteger(PeriodicInformInterval.value) == false))
{
alert("周期通知时间间隔无效");
return false;
}
if (false)
{
if (PeriodicInformEnable[1].checked == true && PeriodicInformTime.value =='' )
{
alert("周期通知时间无效");
return false;
}
if ((PeriodicInformTime.value == '') || (isTimeFormat(PeriodicInformTime.value) == false) )
{
alert("周期通知时间无效");
return false;
}
}
var info = parseFloat(PeriodicInformInterval.value );
if (info < 0 || info > 2147483647)
{
alert("周期通知时间间隔无效");
return false;
}
if (isValidString(Username.value) == false )
{
alert("ACS用户名无效");
return false;
}
if (isValidString(Password.value) == false )
{
alert("ACS密码无效");
return false;
}
if (isValidString(ConnectionRequestUsername.value) == false )
{
alert("连接请求用户名无效");
return false;
}
if (isValidString(ConnectionRequestPassword.value) == false )
{
alert("连接请求密码无效");
return false;
}
}
with (getElById ("ConfigForm"))
{
if (Tr069Enable[1].checked == true &&Tr069Enable.value =='' )
{
alert("Tr069Enable使能无效");
return false;
}
if (mid.Tr069enable == '0' && Tr069Enable[2].checked == true)
{
alert("tr069enable状态不能从0切换到2!");
return false;
}
if (mid.Tr069enable == '2' && Tr069Enable[0].checked == true)
{
alert("tr069enable状态不能从2切换到0!");
return false;
}
if(Tr069Enable[0].checked == true || Tr069Enable[2].checked == true)
{
if (MiddlewareAddress.value == '')
{
alert("请输入中间件业务管理平台地址!");
return false;
}
if (!isValidIpAddress(MiddlewareAddress.value) && !isSafeStringSN(MiddlewareAddress.value))
{
alert("地址无效，请输入正确的IP地址或域名!");
return false;
}
if (MiddlewarePort.value == '')
{
alert("请输入中间件业务管理平台端口号!");
return false;
}
if (!isValidPort(MiddlewarePort.value))
{
alert("端口号无效，请输重新输入!");
return false;
}
}
}
return true;
}
function AddSubmitParam(SubmitForm,type)
{
/*var MiddlewareURL;
SubmitForm.addForm('ConfigForm','x');
SubmitForm.addParameter('y.Tr069Enable', getRadioVal("Tr069Enable"));
if ((0 == getRadioVal("Tr069Enable")) || (2 == getRadioVal("Tr069Enable")))
{
MiddlewareURL = getValue("MiddlewareAddress") + ":" + getValue("MiddlewarePort");
SubmitForm.addParameter('y.MiddlewareURL', MiddlewareURL);
}
SubmitForm.setAction('setcfg.cgi?x=InternetGatewayDevice.ManagementServer&y=InternetGatewayDevice.DeviceInfo.X_CT-COM_MiddlewareMgt&'
+ 'RequestFile=html/network/tr069.asp');*/
}
function SubmitCert()
{
	var SslForm =document.SslForm;
	var string = SslForm.filename.value.search(/pem/);
	
		if(SslForm.filename.value=="")
		{
			alert("请选择证书文件上传！");
		}
		else
		{
			SslForm.postflag.value = "1";
			SslForm.submit();
		}

}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
<% if tcwebApi_get("WebCurSet_Entry","CurrentAccess","h") = "0" then %>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>ITMS服务器</P></TD>       
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=6><TABLE cellSpacing=0 cellPadding=20 width="100%" 
              border=0 height='100%'><TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#远程管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P>终端广域网管理协议（TR-069）允许自动配置服务器（ACS）对网关执行自动配置，<BR>下发业务，收集状态和统计信息，诊断。 
                  输入相关信息后，点击“确认”<BR>按钮配置TR-069客户端参数。</P>
                  <FORM name="ConfigForm" action="/cgi-bin/net-tr069.asp" method="post">
                  <DIV id="En069Info"><TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=149>中间件工作模式</TD>
                      <TD><INPUT onclick=ntpEnbltr069() type=radio value="0" 
                        name="Tr069Enable" <%if TCWebApi_get("Cwmp_Entry","Tr069Enable","h") = "0" then asp_Write("checked") end if %>> 启用中间件(含TR069功能)</TD></TR>
                    <TR>
                      <TD width=149></TD>
                      <TD><INPUT onclick=ntpEnbltr069() type=radio value="1" 
                        name="Tr069Enable" <%if TCWebApi_get("Cwmp_Entry","Tr069Enable","h") = "1" then asp_Write("checked") end if %>> 关闭中间件</TD></TR>
                    <TR>
                      <TD width=149></TD>
                      <TD><INPUT onclick=ntpEnbltr069() type=radio value="2" 
                        name="Tr069Enable" <%if TCWebApi_get("Cwmp_Entry","Tr069Enable","h") = "2" then asp_Write("checked") end if %>> 启用中间件(不含TR069功能)</TD></TR>
                    </TBODY></TABLE></DIV>
                          <input type="hidden" name="EnTr069Flag" value="<% tcWebApi_get("Cwmp_Entry","Tr069Enable","s") %>">
                          <input type="hidden" name="ActiveCwmpFlag" value="<% tcWebApi_get("Cwmp_Entry","Active","s") %>">
                      		<INPUT TYPE="HIDDEN" NAME="isMidwareSupported" value="<% if tcWebApi_get("WebCustom_Entry","isMidwareSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
                          <script language="JavaScript" type="text/JavaScript">
function ntpEnbltr069()
{
with (getElById ("ConfigForm"))
{
	if(midLastRadioValue == "0" && Tr069Enable[2].checked == true || midLastRadioValue == "2" && Tr069Enable[0].checked == true)
	{
		alert("启用中间件(含TR069功能) 与 启用中间件(不含TR069功能) 之间不能直接切换。");
		if(midLastRadioValue == "0")
			Tr069Enable[0].checked = true;
		else
			Tr069Enable[2].checked = true;
	}
	
	if(midLastRadioValue == "0" && Tr069Enable[1].checked == true || midLastRadioValue == "1" && Tr069Enable[0].checked == true)
		alert("关闭中间件和启用中间件(含TR069功能)之间转换时须重启。");
	
	if (Tr069Enable[1].checked == true)
	{
		setDisplay("midurl", 0);
		ActiveCwmpFlag.value = "Yes";
		EnTr069Flag.value = "1";
	}
	else
	{
		setDisplay("midurl", 1);
		if(Tr069Enable[0].checked == true)
		{
			ActiveCwmpFlag.value = "Yes";
			EnTr069Flag.value = "0";
		}
		else
		{
			ActiveCwmpFlag.value = "Yes";
			EnTr069Flag.value = "2";
		}
	}
}
}
if(document.ConfigForm.isMidwareSupported.value == 1)
	getElById("En069Info").style.display = "";
else
getElById("En069Info").style.display = "none";

</script>
                          <BR>
                  <DIV id=midurl>
                  <TABLE height=37 cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR id="hideTR">
                      <TD width=149>中间件服务器URL</TD>
                      <TD width=31>地址:</TD>
                      <TD width=163><INPUT id=MiddlewareAddress maxLength=256 
                        name="MiddlewareAddress" value="<%if tcWebApi_get("Cwmp_Entry","MWMgtUrl","h") <> "N/A" then tcWebApi_get("Cwmp_Entry","MWMgtUrl","s") end if %>"> </TD>
                      <TD width=46>端口号:</TD>
                      <TD width=62><INPUT id=MiddlewarePort maxLength=256 
                        size=8 name="MiddlewarePort" value="<% if TCWebApi_get("Cwmp_Entry","MWMgtPort","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","MWMgtPort","s") end if %>"></TD></TR></TBODY></TABLE>
                            
                          </DIV><BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=80>周期通知</TD>
                      <TD><INPUT id=PeriodicInformEnable type=radio value="No" name="PeriodicInformEnable" <%if TCWebApi_get("Cwmp_Entry","periodActive","h") = "No" then asp_Write("checked") end if %>> 禁用</TD>
                                <TD><input id=radio type=radio value="Yes" name="PeriodicInformEnable" <%if TCWebApi_get("Cwmp_Entry","periodActive","h") = "Yes" then asp_Write("checked") elseif TCWebApi_get("Cwmp_Entry","periodActive","h") = "N/A" then asp_Write("checked") end if %>>
                                  启用</TD>
                              </TR></TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=200>周期通知时间间隔[0 - 2147483647]:</TD>
                      <TD><INPUT id=PeriodicInformInterval 
                        style="WIDTH: 150px" maxLength=10 
                        name=PeriodicInformInterval value="<% if TCWebApi_get("Cwmp_Entry","periodInterval","h" ) <> "N/A" then TCWebApi_get("Cwmp_Entry","periodInterval","s" ) end if %>"></TD></TR>
                    <TR style='display:none'>
                      <TD width=200>周期通知时间[yyyy-mm-ddThh:dd:ss]:</TD>
                      <TD><INPUT id=PeriodicInformTime 
                        style="WIDTH: 150px" maxLength=20 
                        name=PeriodicInformTime value="<% TCWebApi_get("Cwmp_Entry","prdInformTime","s" ) %>"></TD></TR>
                    <TR>
                      <TD>服务器域名/IP地址及端口:</TD>
                      <TD><INPUT id=URL style="WIDTH: 150px" maxLength=256 
                        name=URL value="<% if TCWebApi_get("Cwmp_Entry","acsUrl","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","acsUrl","s") end if %>"></TD></TR>
                    <TR>
                      <TD>平台用户名:</TD>
                      <TD><INPUT id=Username style="WIDTH: 150px" 
                        maxLength=256 name=Username value="<% if TCWebApi_get("Cwmp_Entry","acsUserName","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","acsUserName","s") end if %>"></TD></TR>
                    <TR>
                      <TD>平台密码:</TD>
                      <TD>
					  	<span id="inpsw"><INPUT id=Password style="WIDTH: 150px" type=password maxLength=256 name=Password value="<% if TCWebApi_get("Cwmp_Entry","acsPassword","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","acsPassword","s") end if %>"></span>
						<%if TCWebApi_get("WebCurSet_Entry","IsSupportShowPSW","h" ) = "1" then%>
						<INPUT id="cb_enablshowpsw" onclick="doshowpswChange()" type=checkbox name="cb_enablshowpsw">显示密码
						<script language="JavaScript" type="text/JavaScript">
						
						function doshowpswChange() {
							with (getElById('ConfigForm')){
								var tempvalue1 = Password.value;
								if (cb_enablshowpsw.checked) {
									inpsw.innerHTML = "<input id=\"Password\" style=\"WIDTH: 150px\" type=\"text\" maxLength=256 name=\"Password\" value=" +document.ConfigForm.Password.value+ ">";
								}
								else {
									inpsw.innerHTML = "<input id=\"Password\" style=\"WIDTH: 150px\" type=\"password\" maxLength=256 name=\"Password\" value=" +document.ConfigForm.Password.value+ ">";
								}
								Password.value = tempvalue1;
							}
						
						}
						
						</script>
						<%end if%>
						</TD></TR>
                    <TR>
                      <TD>终端的用户名:</TD>
                      <TD><INPUT 
                        name=ConnectionRequestUsername id=ConnectionRequestUsername 
                        style="WIDTH: 150px" value="<% if TCWebApi_get("Cwmp_Entry","conReqUserName","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","conReqUserName","s") end if %>" maxLength=256></TD></TR>
                    <TR>
                      <TD>终端的密码:</TD>
                      <TD>
					  	<span id="inrapsw"><INPUT name=ConnectionRequestPassword type=password id=ConnectionRequestPassword style="WIDTH: 150px" value="<%if TCWebApi_get("Cwmp_Entry","conReqPassword","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","conReqPassword","s") end if %>" maxLength=256></span>
						<%if TCWebApi_get("WebCurSet_Entry","IsSupportShowPSW","h" ) = "1" then%>
						<INPUT id="cb_enablshowrapsw" onclick="doshowrapswChange()" type=checkbox name="cb_enablshowrapsw">显示密码
						<script language="JavaScript" type="text/JavaScript">
						
						function doshowrapswChange() {
							with (getElById('ConfigForm')){
								var tempvalue2 = ConnectionRequestPassword.value;
								if (cb_enablshowrapsw.checked) {
									inrapsw.innerHTML = "<input id=\"ConnectionRequestPassword\" style=\"WIDTH: 150px\" type=\"text\" maxLength=256 name=\"ConnectionRequestPassword\" value=" +document.ConfigForm.ConnectionRequestPassword.value+ ">";
								}
								else {
									inrapsw.innerHTML = "<input id=\"ConnectionRequestPassword\" style=\"WIDTH: 150px\" type=\"password\" maxLength=256 name=\"ConnectionRequestPassword\" value=" +document.ConfigForm.ConnectionRequestPassword.value+ ">";
								}
								ConnectionRequestPassword.value = tempvalue2;
							}
						
						}
						
						</script>
						<%end if%>
						</TD></TR></TBODY></TABLE><BR><BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0 <% if tcwebApi_get("WebCustom_Entry","isCTPONNMGSupported","h") = "Yes" then %>style="display:none"<% end if %>>
                    <TBODY>
                    <TR>
                      <TD width=80>启用证书功能</TD>
                      <TD><INPUT type=radio value="0" name="X_ATP_SSLCertEnable" <%if tcWebApi_get("Cwmp_Entry","EnableCert","h") = "0" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry","EnableCert","h") = "N/A" then asp_Write("checked") end if%>> 
                        禁用</TD>
                      <TD><INPUT type=radio value="1" name="X_ATP_SSLCertEnable" <%if tcWebApi_get("Cwmp_Entry","EnableCert","h") = "1" then asp_Write("checked") end if%>> 
                        启用</TD></TR></TBODY></TABLE>
                          <input type="hidden" name="SaveFlag" value="0">
		<SCRIPT language=JavaScript type=text/javascript>
			if(document.ConfigForm.isMidwareSupported.value == 1)
				getElById("midurl").style.display = "";
			else
			  getElById("midurl").style.display = "none";
			  
		</SCRIPT>
                          <script language="JavaScript" type="text/JavaScript">
						  
function btnSave()
{
	if(CheckForm() == false)
		return;
	
	var Form = document.ConfigForm;
	Form.SaveFlag.value = "1";
	Form.submit();
}
</script>
                        </FORM>
       <% if tcwebApi_get("WebCustom_Entry","isCTPONNMGSupported","h") <> "Yes" then %>
                  <SCRIPT language=javascript>
				  	var  cflag = "<% tcWebApi_get("SslCA_Entry0","FragNum","s") %>";//the certificate of index 1 is exist?
					if(cflag != "N/A"){
						document.write("(网关已经加载有证书文件!)");
					}
					else
				  		document.write("(网关没有加载证书文件!)");
				  </SCRIPT>
<% end if %>
                  <FORM id=SslForm name="SslForm" method="post" encType="multipart/form-data"><% if tcwebApi_get("WebCustom_Entry","isCTPONNMGSupported","h") <> "Yes" then %>文件路径[最大128个字符]: <INPUT type=file name="filename">&nbsp;<INPUT id="btnSubmit" onclick="SubmitCert()" type=button value=上传 name="btnSubmit" style="height:22px"><BR><% end if %>
                          <input type="hidden" name="postflag" value="0">
                          <input type="hidden" name="fileIndex" value="1">
                  </FORM>
                  <CENTER>
                  <DIV id=ResultText name="ResultText"></DIV> 
                  </CENTER></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD align=center>
				<BUTTON onclick="btnSave()" name=btnOK value="Apply">保存</BUTTON>&nbsp;&nbsp;
				<BUTTON onclick="RefreshPage()" name=btnCancel value="Cancel">取消</BUTTON></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>OLT认证</P></TD>          	
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD></TR>
<% else %>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>OLT认证</P></TD>          	
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=4><TABLE cellSpacing=0 cellPadding=20 width="100%" 
              border=0><TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#远程管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
<% end if %>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=8 background=/img/panel3.gif>　</TD>
          <TD vAlign=top>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
              <FORM name="OLTForm" action="/cgi-bin/register.asp" method="post" target="_top">
        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0 style="display:none">
   <%else%>
              <FORM name="OLTForm" action="/cgi-bin/net-tr069.asp" method="post">
        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
   <% end if
else%>
              <FORM name="OLTForm" action="/cgi-bin/net-tr069.asp" method="post">
        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
<% end if %>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD colspan=2>
                  <P>
                  	<% if tcWebApi_get("WebCustom_Entry", "isLOIDLocked", "h") = "Yes"  then %>
                  	您仅可以查看LOID和Password。
                  	<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then  %>
                  	您可以通过设置Password，然后点击“认证”按钮向网管服务器注册并请求业务配置。
                  	<% else %>
                  	您可以通过设置LOID和Password，然后点击“认证”按钮向网管服务器注册并请求业务配置。
                  	<% end if %>
                  	</P>
                      </TD></TR>
              <%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") <> "Yes"  then  %>
              <TR>
              	<TD>&nbsp;</TD>
                      <TD width=118>LOID:</TD>
                      <TD><INPUT id=LoidUsername style="WIDTH: 150px" 
                        maxLength=256 name=LoidUsername <% if tcWebApi_get("WebCurSet_Entry", "isLOIDLocked", "h") = "Yes"  then %>disabled<%end if %>
value="<%
if TCWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" then
 if TCWebApi_get("EPON_LOIDAuth","LOID0","h") <> "N/A" then TCWebApi_get("EPON_LOIDAuth","LOID0","s") end if
else
 if TCWebApi_get("GPON_LOIDAuth","LOID","h") <> "N/A" then TCWebApi_get("GPON_LOIDAuth","LOID","s") end if
end if %>"></TD></TR>
						<% end if %>
              <TR>
                <TD>&nbsp;</TD>
                      <TD width=118>Password:</TD>
                      <TD>
					  	<span id="inLoidpsw"><INPUT id=LoidPassword style="WIDTH: 150px" type=password maxLength=256 name=LoidPassword <% if tcWebApi_get("WebCurSet_Entry", "isLOIDLocked", "h") = "Yes"  then %>disabled<%end if %> value="<%
if TCWebApi_get("WebCustom_Entry","isCMCCSupported","h") = "Yes" then
	if TCWebApi_get("GPON_ONU","Password","h") <> "N/A" then TCWebApi_get("GPON_ONU","Password","s") end if
else	
if TCWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" then
 if TCWebApi_get("EPON_LOIDAuth","Password0","h") <> "N/A" then TCWebApi_get("EPON_LOIDAuth","Password0","s") end if
else
	 if TCWebApi_get("GPON_LOIDAuth","Password","h") <> "N/A" then TCWebApi_get("GPON_LOIDAuth","Password","s") end if
end if 
end if %>"></span>
						<%if TCWebApi_get("WebCurSet_Entry","IsSupportShowPSW","h" ) = "1" then%>
						<INPUT id="cb_enablshowpsw" onclick="doshowLOIDpswChange()" type=checkbox name="cb_enablshowpsw">显示密码
						<script language="JavaScript" type="text/JavaScript">
						
						function doshowLOIDpswChange() {
							with (getElById('OLTForm')){
								var tempvalue1 = LoidPassword.value;
								if (cb_enablshowpsw.checked) {
									inLoidpsw.innerHTML = "<input id=\"LoidPassword\" style=\"WIDTH: 150px\" type=\"text\" maxLength=256 name=\"LoidPassword\" value=" +document.OLTForm.LoidPassword.value+ ">";
								}
								else {
									inLoidpsw.innerHTML = "<input id=\"LoidPassword\" style=\"WIDTH: 150px\" type=\"password\" maxLength=256 name=\"LoidPassword\" value=" +document.OLTForm.LoidPassword.value+ ">";
								}
								LoidPassword.value = tempvalue1;
							}
						
						}
						
						</script>
						<%end if%>
						</TD></TR>
						<TR>
                <TD>&nbsp;</TD>
                <TD></TD>
                <TD>&nbsp;</TD></TR>
						<TR>
                <TD align=center colspan=3>
        <% if tcWebApi_get("WebCurSet_Entry", "isLOIDLocked", "h") <> "Yes"  then %>
				<BUTTON onclick="doSubmit()" name=btnSubmit value="Submit">认证</BUTTON>&nbsp;&nbsp;
				<BUTTON onclick="RefreshPage()" name=btnCancel value="Cancel">取消</BUTTON><% end if %><input type="hidden" name="SubmitFlag" value="0"></TD></TR>					
						<script language="JavaScript" type="text/JavaScript">
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
function rmCookie(name)
{  
	var exp = new Date();  
	exp.setTime(exp.getTime() - 10000);  
	document.cookie = name + "=del;expires=" + exp.toGMTString();  
	document.cookie = name + "=del;expires=" + exp.toGMTString()+";path=/;";  
}
<% end if
end if %>
						function doSubmit() {
							var Form = document.OLTForm;
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
   							rmCookie("UID");rmCookie("PSW");
							document.cookie = "RegPageStatus=1";
							document.cookie = "RegPagetimeoutcount=0";
							document.cookie = "RegProgressPercent=-1";
							document.cookie = "RegServiceFetch=0";
							document.cookie = "RegServiceNameId=0";
							document.cookie = "RegServiceIPTV=0";
							document.cookie = "RegServiceINTERNET=0";
							document.cookie = "RegServiceVOIP=0";
							document.cookie = "RegServiceOther=0";
							document.cookie = "HasRetryTimesSubmit=0";
   <%else%>
							if(Form.LoidUsername.value == "")
							{
								alert("LOID为空。");
								Form.LoidUsername.focus();
								return;
							}
   <% end if
else%>
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") <> "Yes" then%>
							if(Form.LoidUsername.value == "")
							{
								alert("LOID为空。");
								Form.LoidUsername.focus();
								return;
							}
<%end if%>
<%end if%>
<%if tcWebApi_get("WebCustom_Entry", "isCTPONCYSupported", "h") = "Yes"  then%>
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") <> "Yes" then%>
							var loidchg = trimspace(Form.LoidUsername.value);										
							setText('LoidUsername',loidchg);
<%end if%>						
<%end if%>

							Form.SubmitFlag.value = "1";
							Form.submit();
						}
												
						</script>					
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
						</TBODY></TABLE>
            <DIV id=ServiceConfiguration>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR align=center>
              	<TD>&nbsp;<input type="hidden" name="simOLTVerify" id="simOLTVerify" value="1"><input type="hidden" name="simUSERID" id="simUSERID" value="<%tcwebApi_get("WebCurSet_Entry","CurrentAccess","s")%>"></TD>
                <TD width=92><INPUT id=SerConfig onclick="doSubmit()" type=button value=业务配置 name=SerConfig></TD>
                <TD>&nbsp;</TD></TR></TBODY></TABLE>
            </DIV></FORM></TD></TR>
   <%else%>
						</TBODY></TABLE></FORM></TD></TR>
   <% end if
else%>
						</TBODY></TABLE></FORM></TD></TR>
<% end if%>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#d8d8d8>　</TD>
          <TD width=278 bgColor=#0059A0>　</TD>
          <TD width=196 bgColor=#0059A0>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
<script language="JavaScript">
<% if tcWebApi_get("WebCustom_Entry", "isCTGDSupported", "h") = "Yes"  then %>
var PeriodicInformIntervalOpenFlag = "<%tcWebApi_get("dynCwmpAttr_Entry","aPeriodicInformInterval","s")%>";
<% end if %>
var URLOpenFlag = "<%tcWebApi_get("dynCwmpAttr_Entry","aURL","s")%>";
var UsernameOpenFlag = "<%tcWebApi_get("dynCwmpAttr_Entry","aUsername","s")%>";
var PasswordOpenFlag = "<%tcWebApi_get("dynCwmpAttr_Entry","aPassword","s")%>";
var CRUsernameOpenFlag = "<%tcWebApi_get("dynCwmpAttr_Entry","aConnectionRequestUsername","s")%>";
var CRPasswordOpenFlag = "<%tcWebApi_get("dynCwmpAttr_Entry","aConnectionRequestPassword","s")%>";
<% if tcWebApi_get("WebCustom_Entry", "isCTGDSupported", "h") = "Yes"  then %>
if(PeriodicInformIntervalOpenFlag == "1" && getElById('PeriodicInformInterval'))
	getElById('PeriodicInformInterval').disabled = true;
<% end if %>
if(URLOpenFlag == "1" && getElById('URL') )
<% if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") = "Yes"  then %>
	getElById('URL').disabled = false;
<%else%>
	getElById('URL').disabled = true;
<%end if%>
if(UsernameOpenFlag == "1" && getElById('Username'))
	getElById('Username').disabled = true;
if(PasswordOpenFlag == "1" && getElById('Password'))
	getElById('Password').disabled = true;
if(CRUsernameOpenFlag == "1" && getElById('ConnectionRequestUsername'))
	getElById('ConnectionRequestUsername').disabled = true;
if(CRPasswordOpenFlag == "1" && getElById('ConnectionRequestPassword'))
	getElById('ConnectionRequestPassword').disabled = true;
</script>
</BODY></HTML>
