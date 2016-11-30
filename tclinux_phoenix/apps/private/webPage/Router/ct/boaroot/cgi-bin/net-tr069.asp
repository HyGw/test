<%
if Request_Form("SaveFlag") = "1" then

	TCWebApi_set("Cwmp_Entry","Active","EnTr069Flag")
	if Request_Form("EnTr069Flag") = "Yes" then
		TCWebApi_set("Cwmp_Entry","CPHostName","MiddlewareAddress" )
		TCWebApi_set("Cwmp_Entry","CpePort","MiddlewarePort" )
	end if

	TCWebApi_set("Cwmp_Entry","periodActive","PeriodicInformEnable")
		TCWebApi_set("Cwmp_Entry","periodInterval","PeriodicInformInterval" )
		TCWebApi_set("Cwmp_Entry","acsUrl","URL" )
		TCWebApi_set("Cwmp_Entry","acsUserName","Username" )
		TCWebApi_set("Cwmp_Entry","acsPassword","Password" )
		TCWebApi_set("Cwmp_Entry","conReqUserName","ConnectionRequestUsername" )
		TCWebApi_set("Cwmp_Entry","conReqPassword","ConnectionRequestPassword" )
		TCWebApi_set("Cwmp_Entry","EnableCert","X_ATP_SSLCertEnable")
	TCWebApi_commit("Cwmp_Entry")
	TCWebApi_save()

elseif Request_Form("postflag") = "1" then
		TCWebApi_set("SslCA_Flag","UpgradeFlag","fileIndex")
		TCWebApi_commit("SslCA")
	'tcWebApi_set("System_Entry","upgrade_fw","fileIndex")
	'tcWebApi_Commit("System_Entry")
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
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->远程管理" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33>
            <P class=GatewayType align=center>型号:
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
msg[0] = "上传证书文件成功！";
msg[1] = "证书文件非法,上传证书文件失败！";
msg[2] = "证书功能被禁止！";
msg[3] = "证书功能已启用！";
msg[4] = "证书文件不存在,请先上传！";
msg[5] = "证书文件文件太大，上传失败！";
function LoadFrame()
{
	ntpEnbltr069();
	document.ConfigForm.SaveFlag.value = "0";
	document.SslForm.postflag.value = "0";
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
/*var mwURL = mid.Url;
var mwAddress;
var mwPort;
var mwAddressLen ;
setRadio('Tr069Enable',mid.Tr069enable);
if (mid.Tr069enable == '0' || mid.Tr069enable == '2')
{
setDisplay("midurl", 1);
}
else
{
setDisplay("midurl", 0);
}
setRadio('PeriodicInformEnable',cwmp.InformEnb);
setText('PeriodicInformInterval',cwmp.Interval);
setText('URL',cwmp.URL);
setText('Username',cwmp.Username);
setText('Password',cwmp.Password);
setText('ConnectionRequestUsername',cwmp.CntReqName);
setText('ConnectionRequestPassword',cwmp.CntReqPwd);
setRadio('X_ATP_SSLCertEnable',cwmp.sslCertEnbl);
mwURL = mid.Url;
mwAddressLen = mwURL.lastIndexOf(":");
if(-1==mwAddressLen)
{
mwAddress = mwURL;
mwPort    = "80"
}
else if((isValidPort(mwURL.substr(mwAddressLen + 1)) == true)
||(mwURL.substr(mwAddressLen + 1)=='0'))
{
mwAddress = mwURL.substr(0, mwAddressLen);
mwPort    = mwURL.substr(mwAddressLen + 1);
}
else
{
mwAddress = mwURL;
mwPort    = "80"
}
setText('MiddlewareAddress', mwAddress);
setText('MiddlewarePort', mwPort);

if (URLAccessAttr.indexOf('Subscriber') < 0)
{
setDisable('URL',1);
}
if (UsrAccessAttr.indexOf('Subscriber') < 0)
{
setDisable('Username',1);
}
if (PwdAccessAttr.indexOf('Subscriber') < 0)
{
setDisable('Password',1);
}
if (conUsrAccessAttr.indexOf('Subscriber') < 0)
{
setDisable('ConnectionRequestUsername',1);
}
if (conPwdAccessAttr.indexOf('Subscriber') < 0)
{
setDisable('ConnectionRequestPassword',1);
}*/
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
			alert("You must select a file to update.");
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
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>ITMS服务器</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
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
                  输入相关信息后，点击“确认”<BR>按钮配置TR-069客服端参数。</P>
                  <FORM name="ConfigForm" action="/cgi-bin/net-tr069.asp" method="post">
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=80>TR069开关</TD>
                      <TD><INPUT onclick=ntpEnbltr069() type=radio value="0" 
                        name="Tr069Enable" <%if TCWebApi_get("Cwmp_Entry","Active","h") = "Yes" then asp_Write("checked") end if %>> 启用中间件(含TR069功能)</TD></TR>
                    <TR>
                      <TD width=80></TD>
                      <TD><INPUT onclick=ntpEnbltr069() type=radio value="1" 
                        name="Tr069Enable" <%if TCWebApi_get("Cwmp_Entry","Active","h") = "No" then asp_Write("checked") end if %>> 关闭中间件</TD></TR>
                    <TR>
                      <TD width=80></TD>
                      <TD><INPUT onclick=ntpEnbltr069() type=radio value="2" 
                        name="Tr069Enable" <%if TCWebApi_get("Cwmp_Entry","Active","h") = "N/A" then asp_Write("checked") end if %>> 
                  启用中间件(不含TR069功能)</TD></TR></TBODY></TABLE>
                          <input type="hidden" name="EnTr069Flag" value="<% tcWebApi_get("Cwmp_Entry","Active","s") %>">
                          <script language="JavaScript" type="text/JavaScript">
function ntpEnbltr069(type)
{
with (getElById ("ConfigForm"))
{
	if (Tr069Enable[1].checked == true)
	{
		setDisplay("midurl", 0);
		EnTr069Flag.value = "No";
	}
	else
	{
		setDisplay("midurl", 1);
		EnTr069Flag.value = "Yes";
	}
}
}
</script>
                          <BR>
                  <DIV id=midurl>
                  <TABLE height=37 cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=149>中间件业务管理平台URL</TD>
                      <TD width=31>地址:</TD>
                      <TD width=163><INPUT id=MiddlewareAddress maxLength=256 
                        name="MiddlewareAddress" value="<%if tcWebApi_get("Cwmp_Entry","CPHostName","h") <> "N/A" then tcWebApi_get("Cwmp_Entry","CPHostName","s") end if %>"> </TD>
                      <TD width=46>端口号:</TD>
                      <TD width=62><INPUT id=MiddlewarePort maxLength=256 
                        size=8 name="MiddlewarePort" value="<% if TCWebApi_get("Cwmp_Entry","CpePort","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","CpePort","s") end if %>"></TD></TR></TBODY></TABLE>
                            
                          </DIV>
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
                      <TD><INPUT id=Password style="WIDTH: 150px" 
                        type=password maxLength=256 name=Password value="<% if TCWebApi_get("Cwmp_Entry","acsPassword","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","acsPassword","s") end if %>"></TD></TR>
                    <TR>
                      <TD>终端的用户名:</TD>
                      <TD><INPUT 
                        name=ConnectionRequestUsername id=ConnectionRequestUsername 
                        style="WIDTH: 150px" value="<% if TCWebApi_get("Cwmp_Entry","conReqUserName","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","conReqUserName","s") end if %>" maxLength=256></TD></TR>
                    <TR>
                      <TD>终端的密码:</TD>
                      <TD><INPUT 
                        name=ConnectionRequestPassword type=password id=ConnectionRequestPassword 
                        style="WIDTH: 150px" value="<%if TCWebApi_get("Cwmp_Entry","conReqPassword","h") <> "N/A" then TCWebApi_get("Cwmp_Entry","conReqPassword","s") end if %>" maxLength=256></TD></TR></TBODY></TABLE><BR><BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=80>启用证书功能</TD>
                      <TD><INPUT type=radio value="0" name="X_ATP_SSLCertEnable" <%if tcWebApi_get("Cwmp_Entry","EnableCert","h") = "0" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry","EnableCert","h") = "N/A" then asp_Write("checked") end if%>> 
                        禁用</TD>
                      <TD><INPUT type=radio value="1" name="X_ATP_SSLCertEnable" <%if tcWebApi_get("Cwmp_Entry","EnableCert","h") = "1" then asp_Write("checked") end if%>> 
                        启用</TD></TR></TBODY></TABLE>
                          <input type="hidden" name="SaveFlag" value="0">
                          <script language="JavaScript" type="text/JavaScript">
function btnSave()
{
	var Form = document.ConfigForm;
	Form.SaveFlag.value = "1";
	Form.submit();
}
</script>
                        </FORM>
                  <SCRIPT language=javascript>
				  	var  cflag = "<% tcWebApi_get("SslCA_Entry0","FragNum","s") %>";//the certificate of index 1 is exist?
					if(cflag != "N/A"){
						document.write("(网关已经加载有证书文件!)");
					}
					else
				  		document.write("(网关没有加载证书文件!)");
				  </SCRIPT>

                  <FORM id=SslForm name="SslForm" method="post" encType="multipart/form-data">文件路径[最大128个字符]: <INPUT type=file name="filename"> <BR>
                          <input type="hidden" name="postflag" value="0">
                          <input type="hidden" name="fileIndex" value="1">
                  </FORM>
                  <CENTER>
                  <DIV id=ResultText name="ResultText"></DIV><INPUT id="btnSubmit" onclick="SubmitCert()" type=button value=上传 name="btnSubmit"> 
                  </CENTER></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center><IMG id=btnOK onclick=btnSave() height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 
            width=80 border=0> </P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
