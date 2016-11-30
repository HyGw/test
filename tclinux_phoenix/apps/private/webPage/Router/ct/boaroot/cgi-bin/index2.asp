<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<HTML xmlns="http://www.w3.org/1999/xhtml"><HEAD><TITLE>Login</TITLE>
<script LANGUAGE="JavaScript">
var Logged = 0;
var regSta = "<% tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
var regRst = "<% tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";
var pppup = "<% tcWebApi_get("CwmpRoute_Entry","ifName0","s") %>"; 
var webflag = "<% tcWebApi_get("Cwmp_Entry","webpageFlag","s") %>";
var inneripflag = "<% tcWebApi_get("WebCurSet_Entry","InnerIP","s") %>";
var isIPv4 = "<% tcWebApi_get("WebCurSet_Entry","IsIPv4","s") %>";
var ServerLanIP;
if('0' == isIPv4)
	ServerLanIP = "http://[" + "<% tcWebApi_get("Lan_Entry0","IP6","s") %>" + "]";
else ServerLanIP = "http://" + "<% tcWebApi_get("Lan_Entry0","IP","s") %>";
function getCookie(Name)
{
	var search = Name + "=";
	if(document.cookie.length > 0) 
	{
		offset = document.cookie.indexOf(search);
		if(offset != -1) 
		{
			offset += search.length;
			end = document.cookie.indexOf(";", offset);
			if(end == -1) end = document.cookie.length;
			return unescape(document.cookie.substring(offset, end));
		}
		else 
			return "";
	}
}

<%
if tcWebApi_get("Info_Adsl","lineState","h") = "up" then%>
	if(((regSta == 0)&&(regRst == 99)) || ((regSta == 0)&&(regRst == 2)) || (regSta == 99)){
		if((pppup != "N/A") && (pppup != "") && (webflag==1) && (inneripflag=="0")){
			top.window.location.href=ServerLanIP + "/cgi-bin/register.asp";
		}	
	}
<%
end if
%>
<%
if Request_Form("Username") = tcWebApi_get("Account_Entry0","username","h") then
	if tcWebApi_get("Sys_Entry","IsAdmin","h") <> "0" then
		if tcWebApi_get("Account_Entry0","Logged","h") = "1" then%>
			document.write('<% tcWebApi_get("Account_Entry0","username","s") %>' + '用户已经处于登录状态！');
			Logged = 1;
		<%elseif tcWebApi_get("Account_Entry1","Logged","h") = "1" then%>
			document.write('<% tcWebApi_get("Account_Entry1","username","s") %>' + '用户已经处于登录状态！');
			Logged = 1;
		<%elseif Request_Form("Password") = tcWebApi_get("Account_Entry0","web_passwd","h") then
			TCWebApi_set("Account_Entry0","Logoff","Logoff")%>	
			function delCookie(name)
			{  
				var exp = new Date();  
				exp.setTime(exp.getTime() - 10000);  
				document.cookie = name + "=del;expires=" + exp.toGMTString();  
			}
			delCookie("LoginTimes");
			top.window.location.href="/cgi-bin/content.asp";	
		<%end if
	end if
end if
%>
<%
if Request_Form("Username") = tcWebApi_get("Account_Entry1","username","h") then
	if tcWebApi_get("Sys_Entry","IsAdmin","h") <> "1" then
		if tcWebApi_get("Account_Entry1","Logged","h") = "1" then%>
			document.write('<% tcWebApi_get("Account_Entry1","username","s") %>' + '用户已经处于登录状态！');
			Logged = 1;
		<%elseif tcWebApi_get("Account_Entry0","Logged","h") = "1" then%>
			document.write('<% tcWebApi_get("Account_Entry0","username","s") %>' + '用户已经处于登录状态！');
			Logged = 1;		
		<%elseif Request_Form("Password") = tcWebApi_get("Account_Entry1","web_passwd","h") then
			TCWebApi_set("Account_Entry1","Logoff","Logoff")%>
			function delCookie(name)
			{  
				var exp = new Date();  
				exp.setTime(exp.getTime() - 10000);  
				document.cookie = name + "=del;expires=" + exp.toGMTString();  
			}
			delCookie("LoginTimes");
			top.window.location.href="/cgi-bin/content.asp";	
		<%end if
	end if
end if
%>
<%
if Request_Form("Username") = tcWebApi_get("Account_Entry2","username","h") then
	if tcWebApi_get("Sys_Entry","IsAdmin","h") <> "1" then
		if tcWebApi_get("Account_Entry2","Logged","h") = "1" then%>
			document.write('<% tcWebApi_get("Account_Entry2","username","s") %>' + '用户已经处于登录状态！');
			Logged = 1;
		<%elseif Request_Form("Password") = tcWebApi_get("Account_Entry2","web_passwd","h") then
			TCWebApi_set("Account_Entry2","Logoff","Logoff")%>
			function delCookie(name)
			{  
				var exp = new Date();  
				exp.setTime(exp.getTime() - 10000);  
				document.cookie = name + "=del;expires=" + exp.toGMTString();  
			}
			delCookie("LoginTimes");
			top.window.location.href="/cgi-bin/content.asp";	
		<%end if
	end if
end if
%>
<%
if Request_Form("hLoginTimes") = "3" then
	TCWebApi_set("Account_Entry0","LoginTimes","hLoginTimes")
	TCWebApi_commit("Account_Entry0")
	TCWebApi_set("Account_Entry0","LoginTimes","hLoginTimes_Zero")
end if
%>
</script>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<SCRIPT language=JavaScript src="/JS/util.js"></SCRIPT>

<STYLE type=text/css>#div_visite {
	FONT-SIZE: 12px; MARGIN: 100px 50px; COLOR: #333333; FONT-FAMILY: "宋体"
}
TABLE {
	FONT-SIZE: 15px; FONT-FAMILY: "宋体"
}
</STYLE>

<SCRIPT language=JavaScript type=text/javascript>
function getsec(str)
{  
	var str1=str.substring(1,str.length)*1;    
	var str2=str.substring(0,1);  
	if (str2=="s")
	{  
		return str1*1000; 
	}
	else if (str2=="h")
	{  
		return str1*60*60*1000; 
    	}
	else if (str2=="d")
	{  
		return str1*24*60*60*1000; 
    	}  
}
	
function setCookie(name,value,time)
{  
	var strsec = getsec(time); 
	var exp = new Date();  
	exp.setTime(exp.getTime() + strsec*1);  
	document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString();  
} 
	
var LoginTimes = 0;
if((getCookie("LoginTimes") != "") && (typeof(getCookie("LoginTimes")) != "undefined"))
{
	LoginTimes = parseInt(getCookie("LoginTimes"));
}

function SubmitForm()
{
	var Username = document.getElementById('Username');
	var Password = document.getElementById('Password');
	var hLoginTimes = document.getElementById('hLoginTimes');
	
	if (Username.value == "")
	{
		alert("帐号不能为空");
		return false;
	}
	if (Password.value == "")
	{
		alert("密码不能为空");
		return false;
	}
	var cookie = "UID=" + Username.value + ";";
	document.cookie = cookie;
	var cookie = "PSW=" + Password.value;
	document.cookie = cookie;
	LoginTimes = parseInt(LoginTimes) + 1;
	setCookie("LoginTimes", LoginTimes, "s60");
	hLoginTimes.value = LoginTimes;
	document.ConfigForm.submit();
	return true;
}
function btnbandReg()
{
	var lineStatus = "<% tcWebApi_get("Info_Adsl","lineState","s") %>";

	if ((lineStatus == "up") && (webflag == "1"))
	{
		document.cookie = "RegPageStatus=0";
		document.cookie = "RegPagetimeoutcount=0";
		location.replace(ServerLanIP + "/cgi-bin/register.asp");
	}
	else
	{
		alert("未正确连接到ITMS,不能进行设备注册!");
	}

	/*var xmlhttp = CreateXMLHttp();
	xmlhttp.onreadystatechange = function()
	
	if (xmlhttp.readyState == 4)
	{
		if (xmlhttp.status == 200)
		{
			var status = xmlhttp.responseText;
			if (status.indexOf("1") >= 0)
			{
				location.replace("/html/AutobandReg.asp");
			}
			else
			{
				alert("未正确连接到ITMS,不能进行设备注册!");
			}
		}
	}

	//xmlhttp.open('post', 'queryCwmpSts.cgi', true);
	//xmlhttp.send(null);*/
	return true;
}
function onHandleKeyDown(e)
{
	var key = 0;
	if (window.event)
	{
		key = window.event.keyCode;
	}
	else if (e)
	{
		key = e.which;
	}
	if (key == 13 && document.activeElement.id != 'btnSubmit' && document.activeElement.id != 'bandReg')
	{
		SubmitForm();
	}
}
document.onkeypress = onHandleKeyDown;
</SCRIPT>

<META content="Microsoft FrontPage 5.0" name="GENERATOR"></HEAD>
<BODY>
<SCRIPT language=JavaScript type=text/javascript>
if (LoginTimes >= 3)
{
	document.write('您已经连续三次登录失败，请1分钟后再登录！');
}
</SCRIPT>

<DIV id=div_visite>
<FORM id="ConfigForm" name="ConfigForm" action="/cgi-bin/index2.asp" method="post">		
<TABLE style="POSITION: relative" borderColor="#e7e7e7" height="45%" cellSpacing="0" cellPadding="0" align="center" bgColor="#e7e7e7" border="0">
  <TBODY>
  <TR>
    <TD align="middle" bgColor="#ffffff" colSpan="3" height="10"><LABEL><IMG height="117" src="/img/logo.jpg" width="337"></LABEL></TD></TR>
  <TR>
    <TD align=right width="30%" height=50>
	<script language="JavaScript" type="text/JavaScript">
	<%if tcWebApi_get("Sys_Entry","IsAdmin","h") = "1" then %>
		document.writeln("管理员帐号：");
	<% else %>
		document.writeln("用户帐号：");
	<% end if %>
	</script>
	</TD>
    <TD width=30>　</TD>
    <TD><LABEL>
    <INPUT id="Username" style="WIDTH: 140px; FONT-FAMILY: Arial" name="Username" size="20"> </LABEL></TD></TR>
  <TR>
    <TD align=right height=30>密码：</TD>
    <TD>　</TD>
    <TD>
	<input id="Logoff" type="hidden" name="Logoff" value="0">    
	<input id="hLoginTimes" type="hidden" name="hLoginTimes" value="0">
	<input id="hLoginTimes_Zero" type="hidden" name="hLoginTimes_Zero" value="0">
    <INPUT id="Password" style="WIDTH: 140px; FONT-FAMILY: Arial" type="password" name="Password" size="20"></TD></TR>
  <TR>
    <TD>&nbsp; </TD>
    <TD>&nbsp; </TD>
    <TD style="COLOR: #ff0000" align="center" colspan="3">
      <SCRIPT language=javascript>    
			if (LoginTimes > 0 && LoginTimes < 3)
			{
				var str = '您已经' +  LoginTimes + '次用户名或密码输入错误！';
				document.write(str);
			}			
      </SCRIPT>
    </TD></TR>
  <TR>
    <TD align=center colSpan=3>
	 <INPUT id="btnSubmit" onclick="SubmitForm()" type="button" value="用户登录" name="btnSubmit">&nbsp;
	<%if tcwebApi_get("Info_WLan","isCUCSupport","h")="Yes" then%>	
	<% else %>
	<INPUT id="bandReg" onclick="btnbandReg()" type="button" value="设备注册" name="bandReg">&nbsp; 
	<% end if %> 
	</TD></TR> </TBODY></TABLE></FORM></DIV>
		<SCRIPT language=JavaScript type=text/javascript>
		if ((LoginTimes >= 3) || (Logged == "1"))
		{
			getElById('div_visite').style.display = 'none';
		}
		else
		{
			getElById('Username').focus();
		}
		</SCRIPT>
</BODY></html>
