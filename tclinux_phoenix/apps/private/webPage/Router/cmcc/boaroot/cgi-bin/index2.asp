<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<HTML xmlns="http://www.w3.org/1999/xhtml"><HEAD><TITLE>Login</TITLE>
<script LANGUAGE="JavaScript">
var Logged = 0;
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

function rmCookie(name)
{  
	var exp = new Date();  
	exp.setTime(exp.getTime() - 10000);  
	document.cookie = name + "=del;expires=" + exp.toGMTString();  
	document.cookie = name + "=del;expires=" + exp.toGMTString()+";path=/;";  
}

function gotoSimCardMsg()
{
	rmCookie("UID");rmCookie("PSW");rmCookie("LoginTimes");
	top.window.location.href='/cgi-bin/InsertSimcardMsg.cgi';
}

var Logged = 0;
var pswST = 0;
var gotoMainSet = 0;
function userlogin( login )
{
	Logged = login;
}
function userPSWST( st)
{
	pswST = st;
}

function gotomain_set()
{
	gotoMainSet = 1;
}

function gotomain()
{
	if ( 1 != gotoMainSet )
		return -1;

	if ( 2 == pswST )
		return -2 ;
	else if ( 1 == pswST )
	{
		if ( 0 == Logged )
		{
			rmCookie("LoginTimes");
			top.window.location.href = '/cgi-bin/content.asp';
			return 1;
		}
		else
		{
			rmCookie("UID");rmCookie("PSW");rmCookie("LoginTimes");
			if ( 1 == Logged )
				document.write('电信维护账号已经处于登录状态！');
			else if ( 2 == Logged )
				document.write('用户账号已经处于登录状态！');
		}
	}
	return 0;
}

function refreshself()
{
	top.window.location.href ='/cgi-bin/index2.asp';
}
<%
if Request_Form("Username") <> "" then
if tcWebApi_get("WebCurSet_Entry","isLockState","h") <> "1" then
		tcWebApi_set("WebCurSet_Entry","WebPSW","Password")
		if tcWebApi_get("Account_Entry0","Logged","h") = "1" then
			asp_write("userlogin(1);")
		elseif tcWebApi_get("Account_Entry1","Logged","h") = "1" then
			asp_write("userlogin(2);")
		end if

		if tcWebApi_get("SimCard_Entry", "cpeFlag", "h") = "1"  then
			if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "0"  then
				asp_write("gotoSimCardMsg();")
				tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "2")
				asp_write("userPSWST(2);")
			elseif tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
				if Request_Form("Password1") = tcWebApi_get("Account_Entry1","web_passwd","h") then
					tcWebApi_set("Account_Entry1","Logoff","Logoff")
					tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "1")
					asp_write("userPSWST(1);")
				elseif tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "1"  then
						if tcWebApi_get("Info_SIMWebAuth","webAuthRet","h") = "1" then
							tcWebApi_set("Account_Entry0","Logoff","Logoff")
							tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "1")
							asp_write("userPSWST(1);")
						end if
				elseif Request_Form("Password2") = tcWebApi_get("Account_Entry0","web_passwd","h") then
						tcWebApi_set("Account_Entry0","Logoff","Logoff")
						tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "1")
						asp_write("userPSWST(1);")
				end if
			end if
		else
				if tcWebApi_get("WebCustom_Entry", "isCTPONJXSupported", "h") = "Yes" then
						if Request_Form("logintype") = "adm" then
								if Request_Form("Password1") = tcWebApi_get("Account_Entry0","web_passwd","h") then
										tcWebApi_set("Account_Entry0", "Logoff", "Logoff")
										tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "1")
										asp_write("userPSWST(1);")
								end if
						else
								if Request_Form("Password1") = tcWebApi_get("Account_Entry1","web_passwd","h") then
										tcWebApi_set("Account_Entry1", "Logoff", "Logoff")
										tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "1")
										asp_write("userPSWST(1);")
								end if
						end if
				else
						if Request_Form("Password1") = tcWebApi_get("Account_Entry1","web_passwd","h") then
							tcWebApi_set("Account_Entry1","Logoff","Logoff")
							tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "1")
							asp_write("userPSWST(1);")
						elseif Request_Form("Password2") = tcWebApi_get("Account_Entry0","web_passwd","h") then
								tcWebApi_set("Account_Entry0","Logoff","Logoff")
								tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "1")
								asp_write("userPSWST(1);")
						end if
				end if
		end if
		
		if tcWebApi_get("WebCurSet_Entry", "WebPSWST", "h") = "1" then
			asp_write("gotomain_set();")
		end if
else
	asp_write("refreshself();")
end if
end if

if Request_Form("hLoginTimes") = "3" then
	if tcWebApi_get("WebCurSet_Entry", "WebPSWST", "h") <> "1" then
		if tcWebApi_get("WebCurSet_Entry", "WebPSWST", "h") <> "2" then
			tcWebApi_set("Account_Entry0","LoginTimes","hLoginTimes")
			tcWebApi_commit("Account_Entry0")
			tcWebApi_set("Account_Entry0","LoginTimes","hLoginTimes_Zero")
			tcWebApi_set("WebCurSet_Entry","isLockState","value_one")
			asp_write("refreshself();")
		end if
	end if
end if
tcWebApi_constSet("WebCurSet_Entry", "WebPSW", "")
tcWebApi_constSet("WebCurSet_Entry", "WebPSWST", "")
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

var rstatus = "<%tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";	

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
	var Password1 = document.getElementById('Password1');
	var Password2 = document.getElementById('Password2');
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
	Password1.value = Password.value;
	Password2.value = Password.value;
	var cookie = "UID=" + Username.value + ";path=/;";
	document.cookie = cookie;
	var cookie = "PSW=" + Password.value + ";path=/;";
	document.cookie = cookie;
	LoginTimes = parseInt(LoginTimes) + 1;
	setCookie("LoginTimes", LoginTimes, "s60");
	hLoginTimes.value = LoginTimes;
	document.ConfigForm.submit();
	return true;
}
function btnbandReg()
{
<% if tcWebApi_get("WebCustom_Entry", "isCTGDSupported", "h") = "Yes" then%>
	if((0==rstatus) && (1==rresult))
	{
		alert("已注册成功，无需再注册");
		return false;
	}
<% end if%>
	document.cookie = "RegPageStatus=0";
	document.cookie = "RegPagetimeoutcount=0";
	location.replace(ServerLanIP + "/cgi-bin/register.asp");

	return true;
}

<% if tcWebApi_get("WebCustom_Entry", "isCTPONJXSupported", "h") = "Yes" then%>
var sLoginType = 'usr';
if( (getCookie("logintype") != "") && (typeof(getCookie("logintype")) != "undefined") )
{
	sLoginType = getCookie("logintype");
}
<% end if%>

<%if tcWebApi_get("WebCurSet_Entry","isLockState","h") <> "1" then %>
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
if ( !(0 != Logged && pswST) )
	document.onkeypress = onHandleKeyDown;
<% end if %>
function LoadFrame()
{
	if ( 1 == gotomain() )
		return;
<% if tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes" then%>
	<%if tcWebApi_get("LogicID_Entry","isRegistered","h") = "1" then%>
		<%if tcWebApi_get("XPON_Common","trafficStatus","h") = "up" then%>
			document.getElementById('bandReg').disabled = true;
		<%else%>
			document.getElementById('bandReg').disabled = false;
		<%end if%>
	<%else%>
		document.getElementById('bandReg').disabled = false;
	<%end if%>
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2Supported", "h") = "Yes" then%>
	<%if tcWebApi_get("LogicID_Entry","isRegistered","h") = "1" then%>
		document.getElementById('bandReg').disabled = true;
	<%else%>
		document.getElementById('bandReg').disabled = false;
	<%end if%>
<% end if %>
<% end if %>

<% if tcWebApi_get("WebCustom_Entry", "isCTPONJXSupported", "h") = "Yes" then%>
	var logintypeObj = document.getElementById('logintype');

	if ( logintypeObj )
		logintypeObj.value = sLoginType;
<% end if %>
}
</SCRIPT>

<META content="Microsoft FrontPage 5.0" name="GENERATOR"></HEAD>
<BODY onload="LoadFrame()">
<SCRIPT language=JavaScript type=text/javascript>
<%if tcWebApi_get("WebCurSet_Entry","isLockState","h") = "1" then %>
LoginTimes = 3
<% end if %>
if (LoginTimes >= 3 && !(1==gotoMainSet&&1==pswST&&0==Logged) )
{
	document.write('连续三次登录失败，WEB登录被锁定，请1分钟后再登录！');
}
</SCRIPT>

<DIV id=div_visite>
<FORM id="ConfigForm" name="ConfigForm" action="/cgi-bin/index2.asp" method="post">		
<TABLE style="POSITION: relative" borderColor="#e7e7e7" height="45%" cellSpacing="0" cellPadding="0" align="center" bgColor="#e7e7e7" border="0">
  <TBODY>
  <TR>
    <TD align="middle" bgColor="#e7e7e7" colSpan="3" height="117" >
<% if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes"  then%> 
    	<IMG src="/img/logo.jpg">
<%else%>
			<IMG height="100%" src="/img/logo.jpg" width="100%">
<% end if%>
		</TD></TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONJXSupported", "h") <> "Yes" then%>
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
<% end if %>
  <TR>
    <TD align=right height=30>
	<% if tcWebApi_get("WebCustom_Entry", "isCTPONJXSupported", "h") = "Yes" then%>
	<INPUT id="Username" style="display:none" name="Username" size="1">
	<script language="JavaScript" type="text/JavaScript">
	if ( 'adm' == sLoginType )
		document.writeln("维护密码：");
	else
		document.writeln("用户密码：");
	</script>
	<% else %>
		密码：
	<% end if %>
    </TD>
    <TD>　</TD>
    <TD>
	<input id="Logoff" type="hidden" name="Logoff" value="0">    
	<input id="hLoginTimes" type="hidden" name="hLoginTimes" value="0">
	<input id="hLoginTimes_Zero" type="hidden" name="hLoginTimes_Zero" value="0">
	<input id="value_one" type="hidden" name="value_one" value="1"> 
	<input id="Password1" type="hidden" name="Password1" value="">
	<input id="Password2" type="hidden" name="Password2" value=""> 
	<input id="logintype" type="hidden" name="logintype" value="usr">
    <INPUT id="Password" style="WIDTH: 140px; FONT-FAMILY: Arial" type="password" name="Password" size="20"></TD></TR>
  <TR>
    <TD>&nbsp; </TD>
    <TD>&nbsp; </TD>
    <TD style="COLOR: #ff0000" align="center" colspan="3">
      <SCRIPT language=javascript>    
			if (LoginTimes > 0 && LoginTimes < 3 && !(1==gotoMainSet&&1==pswST&&0==Logged))
			{
				var str = '您已经' +  LoginTimes + '次用户名或密码输入错误！';
				document.write(str);
<% if tcWebApi_get("WebCustom_Entry", "isCTPONJXSupported", "h") = "Yes" then%>
				rmCookie("UID");rmCookie("PSW");
<% end if %>
			}			
      </SCRIPT>
    </TD></TR>
  <TR>
    <TD align=center colSpan=3>
	 <INPUT id="btnSubmit" onclick="SubmitForm()" type="button" value="用户登录" name="btnSubmit">&nbsp;
	 
	 <%if tcwebApi_get("WebCustom_Entry","isCTSFUC9Supported","h")="Yes" then %>
	 <INPUT id="bandReg" onclick="btnbandReg()" type="button" value="设备注册" name="bandReg">&nbsp; 
	 <%else%>
	<%if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h")="Yes" then %>	
	<%elseif tcwebApi_get("WebCustom_Entry","isCTPONNMGSupported","h") = "Yes" then %>
	<%else%>
			<INPUT id="bandReg" onclick="btnbandReg()" type="button" value="设备注册" name="bandReg">&nbsp; 
	<% end if %> 
	<% end if %> 
	</TD></TR> </TBODY></TABLE></FORM></DIV>
		<SCRIPT language=JavaScript type=text/javascript>
		if ((LoginTimes >= 3) || (Logged&&pswST) || (1==gotoMainSet&&1==pswST&&0==Logged))
		{
			getElById('div_visite').style.display = 'none';
		}
		else
		{
			<% if tcWebApi_get("WebCustom_Entry", "isCTPONJXSupported", "h") <> "Yes" then%>
			getElById('Username').focus();
			<% end if %>
		}
		<% if tcWebApi_get("WebCustom_Entry", "isCTPONSupported", "h") = "Yes"  then %>
		getElById('Username').value="useradmin";
		<% end if %>
		</SCRIPT>
</BODY></html>
