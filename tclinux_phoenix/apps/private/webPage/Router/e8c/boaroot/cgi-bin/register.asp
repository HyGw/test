<%
If Request_Form("Register_CHANGE")="1" then 
if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes" then
	TCWebApi_set("GPON_ONU","Password","customer_ID")
else
	TCWebApi_set("EPON_LOIDAuth","LOID0","broadbandusername")
	TCWebApi_set("EPON_LOIDAuth","Password0","customer_ID")
if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then 
	tcWebApi_constSet("EPON_LOIDAuth","AuthStatus","0")
	tcWebApi_constSet("GPON_LOIDAuth","AuthStatus","0")
end if
	TCWebApi_set("GPON_LOIDAuth","LOID","broadbandusername")
	TCWebApi_set("GPON_LOIDAuth","Password","customer_ID")
end if
	TCWebApi_save()
if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes" then
	TCWebApi_commit("GPON_ONU")
else
	TCWebApi_commit("EPON_LOIDAuth")
	TCWebApi_commit("GPON_LOIDAuth")
end if
 
	TCWebApi_set("Cwmp_Entry","devregInform","devReginfoFlag")
	TCWebApi_set("deviceAccount_Entry","newStatus","newStatusFlag")
	TCWebApi_set("deviceAccount_Entry","newResult","newStatusFlag")
if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") <> "Yes" then
	TCWebApi_set("deviceAccount_Entry","userName","broadbandusername")
end if
	TCWebApi_set("deviceAccount_Entry","userPasswordDEV","customer_ID")

	if tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes" then
		TCWebApi_set("LogicID_Entry","isRegistered","Register_CHANGE")
	end if
	if tcWebApi_get("deviceAccount_Entry","registerStatus","h") <> "0" then
		TCWebApi_set("deviceAccount_Entry","registerResult","result_startvalue")
	end if
	if tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes" then
		tcWebApi_constSet("WebCurSet_Entry","regPageSubmit","1")
	end if
	if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") <> "Yes" then
	if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2Supported", "h") = "Yes" then
		TCWebApi_set("LogicID_Entry","isRegistered","Register_CHANGE")
		tcWebApi_constSet("WebCurSet_Entry","regPageSubmit","1")
	end if
	end if
	TCWebApi_commit("deviceAccount_Entry")
	TCWebApi_save()
elseif Request_Form("Register_CHANGE") = "2" then
	if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes" then
		TCWebApi_set("System_Entry","newRomfileSelect","romfileselect")
		TCWebApi_commit("System_Entry")
	end if
elseif Request_Form("simOLTVerify") = "1" then
	if Request_Form("simUSERID") = "0" then
			tcWebApi_constSet("Account_Entry0","Logged","0")
	else
			tcWebApi_constSet("Account_Entry1","Logged","0")
	end if
End If
If Request_Form("tryTimesflg")="1" then 
	TCWebApi_set("deviceAccount_Entry","retryTimes","tryTimes")
	TCWebApi_commit("deviceAccount_Entry")
	TCWebApi_save()
End If

%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!-- saved from url=(0039)http://192.168.1.1/html/AutobandReg.asp -->
<HTML><HEAD><TITLE>业务注册</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="JavaScript" type="text/JavaScript">
<%if tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes" then%>
function showmsg_info()
{
	alert('逻辑ID保存成功，设备正在注册，登录页面后可查看注册状态！');
	top.window.location.href='/cgi-bin/index2.asp';
}
<% else %>
<%if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2Supported", "h") = "Yes" then%>
function showmsg_info()
{
	alert('逻辑ID保存成功，设备正在注册，登录页面后可查看注册状态！');
	top.window.location.href='/cgi-bin/index2.asp';
}
<%end if%>
<%end if%>
<%
	if tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes" then
		if tcWebApi_get("WebCurSet_Entry", "regPageSubmit", "h") = "1"  then
				tcWebApi_constSet("WebCurSet_Entry","regPageSubmit","0")
				asp_write("showmsg_info()")
		end if
	else
		if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") <> "Yes" then
		if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2Supported", "h") = "Yes" then
			if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") <> "Yes" then
				if tcWebApi_get("WebCurSet_Entry", "regPageSubmit", "h") = "1"  then
						tcWebApi_constSet("WebCurSet_Entry","regPageSubmit","0")
						asp_write("showmsg_info()")
				end if
			end if
		end if
		end if
	end if
	if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
	   if tcWebApi_get("SimCard_Entry", "cpeFlag", "h") = "1"  then
		   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "0"  then%>
		   	top.window.location.href="/cgi-bin/InsertSimcardMsg.cgi";
		   <%end if
	   end if
	end if
%>
var	statusFlag = 0;
var isHasRetryTimesSubmit = 0;
var	registerend = false;
var regITMSlimitcount = 24;
<%if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then%>
var	uplimitcount = 48;//count of refresh
var uplimitcount_2 = 36; // 2 miniutes
<%else%>
var	uplimitcount = 120;//count of refresh
var uplimitcount_2 = 36; // 3 miniutes
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
var uplimitcount_3 = 48;
<%end if%>
<%end if%>
<%if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes"  then%>
var service_timeout = 24; // 2 miniutes
<%end if%>
var	refreshtimer = 5000;//unit is millisecond
var	isIPv4 = "<% tcWebApi_get("WebCurSet_Entry","IsIPv4","s") %>";
var	lanip;
var lanip_v4 = "<% tcWebApi_get("Lan_Entry0","IP","s") %>";
var lanip_v6 = "[" + "<% tcWebApi_get("Lan_Entry0","IP6","s") %>" + "]";
if ( 'N/A' == lanip_v4 )
	lanip_v4 = '192.168.1.1';
if ( 'N/A' == lanip_v6 )
	lanip_v6 = '[fe80::1]';
if ( '0' == isIPv4 && top.window.location.href.indexOf(lanip_v6) < 0 )
	top.window.location.href = 'http://' + lanip_v6 + '/cgi-bin/register.asp';
else if ( '1' == isIPv4 && top.window.location.href.indexOf(lanip_v4) < 0 )
	top.window.location.href = 'http://' + lanip_v4 + '/cgi-bin/register.asp';

function writeReturnhref()
{
	var httpip;
	if("0" == isIPv4){
		lanip = "[" + "<% tcWebApi_get("Lan_Entry0","IP6","s") %>" + "]";
		if(lanip == "N/A")
			httpip = "http://[fe80::1]";
		else
			httpip = "http://" + lanip;
	}
	else{
		lanip = "<% tcWebApi_get("Lan_Entry0","IP","s") %>";
		if(lanip == "N/A")	
			httpip = "http://192.168.1.1";
		else
			httpip = "http://" + lanip;
	}
	document.write('<A href="' + httpip + '" onclick="resetCookie();">返回登录页面</A>');
}
function resetCookie() {
	document.cookie = "RegPageStatus=0";
	document.cookie = "RegPagetimeoutcount=0";
	document.cookie = "RegPagetimeoutcount_2=0";
	document.cookie = "RegProgressPercent=-1";
	document.cookie = "RegServiceFetch=0";
	document.cookie = "RegServiceNameId=0";
	document.cookie = "RegServiceIPTV=0";
	document.cookie = "RegServiceINTERNET=0";
	document.cookie = "RegServiceVOIP=0";
	document.cookie = "RegServiceOther=0";	
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes" then%>
	document.cookie = "RegServicBIND2ITMSTimeout=0";
	document.cookie = "RegServicBIND2WebStartTime=0";
<% end if%>
	return true;
}
function checkQuitstatus() {
	var retry = "<%tcWebApi_get("deviceAccount_Entry","retryTimes","s") %>";
        var limit = "<%tcWebApi_get("deviceAccount_Entry","retryLimit","s") %>";
        if (retry != "N/A" && limit != "N/A")
        {
                if(parseInt(retry) > parseInt(limit))
		{
			registerend = true;
			resetCookie();
			setDisplay("msgwait", 0);	
		}
	}
}
</script>

<META content="MSHTML 6.00.2900.3698" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center; valign: middle" vLink="#000000" aLink="#000000" 
link="#000000" leftMargin="0" topMargin="0" onload="LoadFrame()" marginheight="0" 
marginwidth="0">
<SCRIPT language=JavaScript type=text/javascript>

function LoadFrame()
{
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes" then %>					
	var promptError = 0;
	<% if Request_Form("ReDefaultbootflg") = "1" then %>
		<% if Request_Form("TestCode") = tcWebApi_get("Account_Entry1","web_passwd","h") then
			tcWebApi_set("WebCurSet_Entry", "isRegFactoryDef", "restoreflag2") %>
			promptError = 2;
		<% else %>
			promptError = 1;
		<% end if %>
	<% elseif tcWebApi_get("deviceAccount_Entry","registerResult","h") = "1" then %>
		var tdResultElement = getElement('tdResult');
		var titles = "<div align='center' style='padding-bottom:5px'><font style='FONT-SIZE: 12px' align='center'><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>上行E8-C终端业务注册提示：</font></div>";
		tdResultElement.innerHTML = titles + "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + "设备已注册、无需再注册，需恢复出厂配置才可再注册。" + "</font></div>";
		setDisplay('mainScreen', 0);
		setDisplay('promptScreen', 1);
		setDisplay("loading",0);
		setDisplay("trReturn1", 1);
		return;
	<%end if%>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
	getElement("hint2Text").innerHTML="“LOID”和“Password”";  
	getElement("loidText").innerHTML="LOID:";  
	getElement("passwordText").innerHTML="Password:";  
<% end if%>
	var tryTimes = "<%tcWebApi_get("deviceAccount_Entry","retryTimes","s") %>";
	var limitTimes = "<%tcWebApi_get("deviceAccount_Entry","retryLimit","s") %>";
	if (tryTimes != "N/A")
	{
		tryTimes = parseInt("<%tcWebApi_get("deviceAccount_Entry","retryTimes","s") %>");
	}
	else
	{
		tryTimes = 0;
	}
	if (limitTimes != "N/A")
	{
		limitTimes = parseInt("<%tcWebApi_get("deviceAccount_Entry","retryLimit","s") %>");
	}
	else
	{
		limitTimes = 0;
	}
	var lefttime = limitTimes - tryTimes;
	var isPageUpdate = getCookie('PageUpdate');
	
	with (getElById('ConfigForm'))
	{
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		if(promptError == 1)
		{
			setDisplay("mainScreen",0);
			setDisplay("promptScreen", 1);
			setDisplay("loading",0);
			setDisplay("trReDefaultboot", 1);
			setDisplay("submitbutton", 1);
			setDisplay("trErrorTestCode", 1);
			setDisplay("trErrorTestCodeTitle", 1);
			setDisplay("trReturn1", 0);
		}
		else if ( 2 == promptError )
		{
			top.window.location.href ='/cgi-bin/resetscreen.asp';		
		}
		else{
<% end if%>		
		if(parseInt(lefttime) > 0
			|| (isPageUpdate == 1 && parseInt(lefttime) == 0))
		{
			if (isPageUpdate == 1 && parseInt(lefttime) == 0)
				document.cookie = "PageUpdate=0";
			var rs=getPageStatus();
			if((rs == "2") || (rs == "1"))
			{
				setDisplay('mainScreen', 0);
				setDisplay('promptScreen', 1);
			}
			else
			{
				setDisplay('mainScreen', 1);
				setDisplay('promptScreen', 0);
			}
			if (registerend){
				resetCookie();
			}
		}
		else
		{
				var tdResultElement = getElement('tdResult');
				setDisplay("trReturn", 1);
				setDisplay('mainScreen', 0);
				setDisplay('promptScreen', 1);
				setDisplay("loading",0);
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
				hidePercent();
<% end if%>
				isRegSuccess = true;
<% if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>	
				tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">在RMS上注册失败！请3分钟后重试或联系客户经理或拨打10086</font></strong></P>';
<%else%>			
				tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">在ITMS上注册失败！请3分钟后重试或联系客户经理或拨打10000</font></strong></P>';
<% end if %>
		}
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>				
		}
<% end if%>			
<%if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes" then%>
<% if Request_Form("Register_CHANGE") = "2" then %>
<% if Request_Form("romfileselect") <> Request_Form("ori_romfileselect") then %>
		setDisplay('mainScreen', 0);
		setDisplay('promptScreen', 0);
		setDisplay('loading', 0);
		setTimeout("doRefresh()",40*1000);
		document.writeln("Version Type发生变更，设备重启中，请等待……");
<% end if %>
<% end if %>
<%end if%>		
	}
}

<%if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes" then%>
function doRefresh()
{
 	window.top.location.href = "../";    
}
<%end if%>

function refreshPage()
{
	document.cookie = "RegPageStatus=2";
	syncRegStatus();
	//document.ConfigForm.submit();
}

function getPageStatus()
{
	if(document.cookie.length > 0)
	{
		tstart = document.cookie.indexOf("RegPageStatus=");
		if( tstart != -1)
		{
			tstart += 14;
			tend = document.cookie.indexOf(";",tstart);
			if(tend == -1) tend = document.cookie.length;
			return document.cookie.substring(tstart,tend);
		}
		else return "N/A";
	}
	else return "N/A";
}
function gettimeoutcnt()
{
	if(document.cookie.length > 0)
	{
		tstart = document.cookie.indexOf("RegPagetimeoutcount=");
		if( tstart != -1)
		{
			tstart += 20;
			tend = document.cookie.indexOf(";",tstart);
			if(tend == -1) tend = document.cookie.length;
			return parseInt(document.cookie.substring(tstart,tend));
		}
		else return 0;
	}
	else return 0;
}

function gettimeoutcnt_2()
{
	if(document.cookie.length > 0)
	{
		tstart = document.cookie.indexOf("RegPagetimeoutcount_2=");
		if( tstart != -1)
		{
			tstart += 22;
			tend = document.cookie.indexOf(";",tstart);
			if(tend == -1) tend = document.cookie.length;
			return parseInt(document.cookie.substring(tstart,tend));
		}
		else return 0;
	}
	else return 0;
}

function getCookie(Name)
{
	var searched = Name + "=";
	if(document.cookie.length > 0) 
	{
		offset = document.cookie.indexOf(searched);
		if(offset != -1) 
		{
			offset += searched.length;
			end = document.cookie.indexOf(";", offset);
			if(end == -1) end = document.cookie.length;
			return parseInt(document.cookie.substring(offset, end));
		}
		else 
			return 0;
	}
	else return 0;
}

function onReturn(index)
{
	resetCookie();
	if (1 == index)
	{
		with (getElById('ConfigForm'))
		{
			setDisplay('mainScreen', 1);
			setDisplay('promptScreen', 0);
		}
	}
	else
	{
<% if tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes" then%>
		location.replace("http://" + lanip.toString());	
<%else%>
	<% if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") = "Yes" then %>
		if (isRegSuccess != true && gLefttimes > 0 )
		{
			document.ConfigForm.submit();
		}
		else
		{
			location.replace("http://" + lanip.toString());
		}
	<% else %>
	<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2Supported", "h") = "Yes" then%>
	<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes" then %>
		if (isRegSuccess != true && gLefttimes > 0 )
		{
			document.ConfigForm.submit();
		}
		else
		{
			location.replace("http://" + lanip.toString());
		}
	<% else %>
		location.replace("http://" + lanip.toString());	
	<% end if%>
	<%else%>	
		if (isRegSuccess != true && gLefttimes > 0 )
		{
			document.ConfigForm.submit();
		}
		else
		{
			location.replace("http://" + lanip.toString());
		}
	<% end if%>
	<% end if%>
<% end if%>
	}
}
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
function ReDefaultboot()
{
	var tdResultElement = getElement('tdResult');
	tdResultElement.innerHTML = "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + "输入正确验证码，可以帮助恢复默认的出厂配置" + "</font></div>";
	setDisplay("trReturn1", 0);
	setDisplay("trReDefaultboot", 1);
	setDisplay("submitbutton", 1);
}
<% end if%>
function CheckForm()
{
	var customer = getValue("customer_ID");
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") <> "Yes"  then %>
	var bandName = getValue("broadbandusername");
	if (bandName == "")
	{
		alert("逻辑ID不能为空!");
		return false;
	}

	if (bandName.length > 128)
	{
		alert("逻辑ID长度不能大于128位!");
		return false;
	}
<% end if %>
	if (customer.length > 128)
	{
		alert("密码长度不能大于128位!");
		return false;
	}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>
	var reg = /^[a-zA-Z0-9][\x21-\x73]*[a-zA-Z0-9]$|^[a-zA-Z0-9]+$/;						
	
	if (reg.test(bandName) == false)
	{
   	alert("LOID只能以字母数字开头和结尾，中间不能含有空格。");
		return;
	} 
	  
	if (customer.length>0 && reg.test(customer)== false)
	{
		alert("Password只能以字母数字开头和结尾，中间不能含有空格。");
		return;
	}	
<%end if%>
	return true;
}

function onReset()
{
	with (getElById('ConfigForm'))
	{
		setText('broadbandusername', "");
		setText('customer_ID', "");
	}
}

function onOK()
{
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
   <%else%>
	if(CheckForm())
	{
   <% end if
else%>
	if(CheckForm())
	{
<% end if%>
		document.ConfigForm.Register_CHANGE.value = "1";
		document.cookie = "RegPageStatus=1";
		document.cookie = "RegPagetimeoutcount=0";
		document.cookie = "RegPagetimeoutcount_2=0";
		document.cookie = "RegProgressPercent=-1";
		document.cookie = "RegServiceFetch=0";
		document.cookie = "RegServiceNameId=0";
		document.cookie = "RegServiceIPTV=0";
		document.cookie = "RegServiceINTERNET=0";
		document.cookie = "RegServiceVOIP=0";
		document.cookie = "RegServiceOther=0";
		document.cookie = "HasRetryTimesSubmit=0";
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
		var dateT = new Date();
		document.cookie = "RegStartTime="+dateT.getTime();
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes" then%>
		document.cookie = "RegServicBIND2ITMSTimeout=0";
		document.cookie = "RegServicBIND2WebStartTime=0";
<% end if%>

		document.ConfigForm.submit();
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
   <%else%>
	}
   <% end if
else%>
	}
<% end if%>
}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes"  then %>
function onSetting()
{
	top.window.location.href="/cgi-bin/cwmpsetting.asp";
}
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
function onCancel()
{
	setDisplay("trErrorTestCode", 0);

	with (getElById('ConfigForm'))
	{
		setText('TestCode', "");
	}
}

function onSubmit()
{
	var testcode = getValue("TestCode");
	if (testcode == "")
	{
		alert("验证密码不能为空!");
		return false;
	}

	document.ConfigForm.ReDefaultbootflg.value = "1";
	document.ConfigForm.restoreflag2.value = "Yes";
	document.ConfigForm.submit();
	getElById('buttondefaultbak').disabled = true;	
}
<% end if%>
function pageHeight()
{
	return window.innerHeight != null? window.innerHeight : document.documentElement && document.documentElement.clientHeight ? document.documentElement.clientHeight : document.body != null? document.body.clientHeight : null;
}
</SCRIPT>
<FORM id="SelfCheckForm" name="SelfCheckForm" method="post" action="/cgi-bin/selfcheck.asp">
	<input type="hidden" name="stb_test" value="0">
</FORM>

<FORM id="ConfigForm" name="ConfigForm" method="post">
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
	<INPUT TYPE="HIDDEN" NAME="ReDefaultbootflg" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="restoreflag2" VALUE="">
<% end if%>	
	<INPUT TYPE="HIDDEN" NAME="Register_CHANGE" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="tryTimesflg" VALUE="-1">
	<INPUT TYPE="HIDDEN" NAME="tryTimes" VALUE="-1">
  	<input type="hidden" name="devReginfoFlag" value="1">
  	<input type="hidden" name="newStatusFlag" value="0">
  <input name="result_startvalue" type="hidden" value="99">
  <DIV id=mainScreen style="DISPLAY: none">
<SCRIPT language=javascript type=text/javascript>
	var percent = 0;
var winHeight = pageHeight();
if (winHeight > 540)
{
winHeight = (winHeight - 540)/2;
}
else
{
winHeight = 80;
}
document.write('<table width="808" height="540" align="center" background="../img/bg.jpg" style="margin-top:'+winHeight+'px">');
function updateprogress()
{
	with (getElById('ConfigForm')){
		var tdResultprogress = getElement('loadcss');
		tdResultprogress.style.width = percent.toString() + "%";
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
		getElement('percentage').innerHTML = percent.toString() + "%";
<%else%>
		tdResultprogress.innerHTML = percent.toString() + "%";
<% end if%>
	}
	
	document.cookie = "RegProgressPercent=" + percent.toString();
}
</SCRIPT>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
<TR valign="top" height="40%">
<TD width="20%"></TD>
<TD width="50%"></TD>
<TD width="30%">
<%else%>
<TR valign="top" height="40%"><TD>
<% end if%>	
<TABLE style="MARGIN-TOP: 150px" align=right>
  <TBODY>
  <TR>
    <TD style="MARGIN-TOP: 100px; FONT-SIZE: 13px">
	<script language="JavaScript" type="text/JavaScript">
	writeReturnhref();
	</script>
	</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD width="50%">&nbsp;</TD></TR></TBODY></TABLE></TD></TR><TR valign="top">
    <TD	width="20%"></TD>
    <TD width="59%">
<%else%>
    <TD width="50%">&nbsp;</TD></TR></TBODY></TABLE></TD></TR><TR valign="top"><TD colspan="3">
<% end if%>
<TABLE align="center">
  <TBODY>
	<TR>
    <TD width="8%">&nbsp;</TD>
<% if tcwebApi_get("WebCustom_Entry","isCTPONCYSupported","h") = "Yes" then %>
    <% if tcwebApi_get("WebCustom_Entry","isCTPONYNSupported","h") = "Yes" then %>
        <TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>上行<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>终端业务注册提示：</TD></TR>
    <% else %>
    	  <TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>上行E8-C终端业务注册提示：</TD></TR>
    <% end if %>
<% else %>
    <% if tcwebApi_get("WebCustom_Entry","isCTPONYNSupported","h") = "Yes" then %>
        <TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>上行<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>终端业务注册提示：</TD></TR>
    <% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
    		<TD style="FONT-SIZE: 12px" colSpan=2>GPON上行E8-C终端业务注册提示：</TD></TR>
    <% elseif tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
    		<TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>上行<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>终端业务注册提示：</TD></TR>
		<% elseif tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>
				<TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>上行<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>终端业务注册提示：</TD></TR>
    <% else %>
        <TD style="FONT-SIZE: 12px" colSpan=2>EPON上行E8-C终端业务注册提示：</TD></TR>
    <% end if %>
<% end if %>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>1.按要求插紧光纤后再确认终端光信号灯熄灭。</TD></TR>
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
		<TD style="FONT-SIZE: 12px" colSpan=2>1.请插紧光纤并确认光信号灯已处于熄灭状态</TD></TR>
<% elseif tcWebApi_get("WebCustom_Entry", "isCTPONCYSupported", "h") = "Yes"  then %>
		<TD style="FONT-SIZE: 12px" colSpan=2>1.请插紧“光纤E/G”接口的光纤，检查并确认光信号灯已处于熄灭状态</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>1.请插紧“网络E/G”接口的光纤，检查并确认光信号灯已处于熄灭状态</TD></TR>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then %>
  <% if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
    <TR style="display:none">
  <%else%>
    <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.光信号常亮后输入LOID和PASSWORD（密码）进行注册。密码可以为宽带帐号（不带@后缀）、电话号码（不加区号）或LOID（纯ITV接入）。</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.准确输入<a id="hint2Text">“逻辑ID”和“密码”</a>，点击“确定”进行注册或插入家庭<br/>&nbsp; 网关卡后重启设备</TD></TR>
<% end if%>
  <% end if %>
<%else%>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.光信号常亮后输入LOID和PASSWORD（密码）进行注册。密码可以为宽带帐号（不带@后缀）、电话号码（不加区号）或LOID（纯ITV接入）。</TD></TR>
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
		<TD style="FONT-SIZE: 12px" colSpan=2>2.准确输入“Password”，点击“确定”进行注册</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.准确输入<a id="hint2Text">“逻辑ID”和“密码”</a>，点击“确定”进行注册</TD></TR>
<% end if%>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
		  <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>2.在注册过程中不要随意拔插光纤（10分钟内）。</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>2.在注册及业务下发过程中（10 分钟内）不要断电、不要拔光纤</TD></TR>
		<% end if%>
		  <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.本注册功能仅用于新设备的认证及业务下发，已正常在用设备请勿重新注册。</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.本注册功能仅用于新设备的认证及业务下发，已正常在用设备请勿重新注册</TD></TR>
		<% end if%>
		  </TR>
   <%else%>
		 <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.在注册过程中不要随意拔插光纤（10分钟内）。</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.在注册及业务下发过程中（10 分钟内）不要断电、不要拔光纤</TD></TR>
		<% end if%>
		  <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>4.本注册功能仅用于新设备的认证及业务下发，已正常在用设备请勿重新注册。</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>4.本注册功能仅用于新设备的认证及业务下发，已正常在用设备请勿重新注册</TD></TR>
		<% end if%>
		  </TR>
   <% end if
else%>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>3.在注册过程中不要随意拔插光纤（10分钟内）。</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>3.在注册及业务下发过程中（10 分钟内）不要断电、不要拔光纤</TD></TR>
<% end if%>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>4.该终端是<% if tcwebApi_get("XPON_Common","xponMode","h") = "GPON" then %>GPON<% else %>EPON<% end if %>的E8-C终端。</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>4.本注册功能仅用于新设备的认证及业务下发，已正常在用设备请勿重新注册</TD></TR>
<% end if%>
  </TR>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes"  then %>
  <TR id="tr_romfileselect">  	
  	<TD width="8%">&nbsp;<input type="hidden" name="romfileselect" value=""><input type="hidden" name="ori_romfileselect" value="<%TCWebApi_get("System_Entry","romfileSelect","s")%>"></TD>
  	<TD width="38%" align="right" style="FONT-SIZE: 13px" id="romfileselectText">Version Type:</TD>
  	<TD width="60%"><iframe id="romfileselectfrm" name="romfileselectfrm" src="/cgi-bin/getRomfileInfo.cgi" frameborder="0" width="240" height="26"></iframe></TD>
  </TR>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") <> "Yes"  then %>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
  	<TR id="useraccount" style="display:none">
   <%else%>
  	<TR id="useraccount">
   <% end if
else%>
  <TR id="useraccount">
<% end if%>
    <TD width="8%">&nbsp;<P></TD>
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") = "Yes" then%>
              <TD width="38%" align="right" style="FONT-SIZE: 13px">LOID:</TD>
	<%else%>
			  <TD width="38%" align="right" style="FONT-SIZE: 13px" id="loidText">逻辑ID:</TD>
	<%end if%>
    <TD width="60%"><INPUT id="broadbandusername" style="WIDTH: 120px; FONT-FAMILY: '宋体'" <% if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>maxLength=24<%end if%>
      name="broadbandusername" height="20" value="<% If TCWebApi_get("deviceAccount_Entry","userName","h") <> "N/A" then
						TCWebApi_get("deviceAccount_Entry","userName","s")
						end if%>">&nbsp;<FONT color=red>*</FONT></TD></TR>
<%end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
		<TR id="userpassword" style="display:none">
   <%else%>
		<TR id="userpassword">
   <% end if
else%>
  <TR id="userpassword">
<% end if %>
    <TD width="8%">&nbsp;</TD>
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") = "Yes" then%>
              <TD style="FONT-SIZE: 13px" align="right">PASSWORD:</TD>
	<%elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
				<TD width="38%" style="FONT-SIZE: 13px" align="right">Password:</TD>
	<%else%>
			  <TD style="FONT-SIZE: 13px" align="right" id="passwordText">密码:</TD>
	<%end if%>
    <TD><INPUT id="customer_ID" style="WIDTH: 120px; FONT-FAMILY: '宋体'" <% if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>maxLength=12<%end if%> 
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") <> "Yes"  then %>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") <> "Yes"  then %>
      type="password"
<% end if%>
<% end if%>
       name="customer_ID" height="20" value="<% If TCWebApi_get("deviceAccount_Entry","userPasswordDEV","h") <> "N/A" then
						TCWebApi_get("deviceAccount_Entry","userPasswordDEV","s")
						end if%>"> </TD></TR>
  <TR id="msgwait">
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 13px" align=right></TD>
              <TD>注册中,请稍等...</TD>
            </TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
    <TR id="msgbutton" style="display:none">
   <%else%>
    <TR id="msgbutton">
   <% end if
else%>
  <TR id="msgbutton">
<% end if%>
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
        <TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>   
          <TD width="20%" align=right ><INPUT id="btnOK" style="WIDTH: 60px;HEIGHT: 20px" onclick="onOK()" type=button value="确定"></TD>
<%else%>
					<TD width="45%" align=right ><INPUT id="btnOK" style="WIDTH: 60px;HEIGHT: 20px" onclick="onOK()" type=button value="确定"></TD>
<% end if%>         
<% if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes"  then%> 
					<TD width="10%"><INPUT id="btnSetting" style="WIDTH: 70px;HEIGHT: 20px" onclick="onSetting()" type=button value="高级设置"></TD>
<%else%>
          <TD width="10%">&nbsp;</TD>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then 
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
          <TD width="45%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="重置"></TD></TR></TBODY></TABLE></TD></TR>
          <DIV id=ServiceConfiguration>
            <TR align=center>
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2><INPUT id=SerConfig onclick="onOK()" type=button value=业务配置 name=SerConfig></TD></TR></DIV>
	</TBODY></TABLE></TD></TR></TABLE></DIV>
   <%else%>
          <TD width="45%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="重置"></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
   <% end if
else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>  
          <TD width="20%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="重置">
<%else%>
					<TD width="45%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="重置">
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>    
          <TD width="20%" align=left ><INPUT id="btnCancel" style="WIDTH: 65px;HEIGHT: 20px" onclick="btnselfCheck()" type="button" value="终端自检">       	
<% end if%></TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					<TD width="20%" align=left ><INPUT id="btnCancel" style="WIDTH: 65px;HEIGHT: 20px" onclick="restore()" type="button" value="恢复出厂"></TD>
<% end if%> 
					</TR></TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
<% end if%>
<DIV id=promptScreen style="DISPLAY: none">
<SCRIPT language=javascript type=text/javascript>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
	if((getPageStatus() == "1") && !registerend)
<%else%>
	if(getPageStatus() == "1")
<% end if%>
	{
		setDisplay("msgwait", 1);
	}
	else
	{
		setDisplay("msgwait", 0);
	}

var winHeight = pageHeight();
if (winHeight > 540)
{
winHeight = (winHeight - 540)/2;
}
else
{
winHeight = 80;
}
document.write('<table width="808" height="540" align="center" background="../img/bg.jpg" style="margin-top:'+winHeight+'px">');
function btnselfCheck()
{
		document.SelfCheckForm.stb_test.value  = "1";
		document.SelfCheckForm.submit();
}

function restore()
{
		top.window.location.href="/cgi-bin/reset.asp";
}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
function hidePercent()
{
    setDisplay("prog0", 0);
    setDisplay("prog1", 0);
}
<% end if%>
</SCRIPT>
<TR valign="top" height="50%"><TD>
<TABLE style="MARGIN-TOP: 150px" align=right>
  <TBODY>
  <TR>
    <TD style="MARGIN-TOP: 100px; FONT-SIZE: 13px">
	<script language="JavaScript" type="text/JavaScript">
	writeReturnhref();
	</script>
	</TD>
    <TD width="50%">&nbsp;</TD></TR></TBODY></TABLE></TD></TR><TR valign="top"><TD>
<TABLE width="40%" align=center>
  <TBODY>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
  <TR>
  	<TD id='prog0'>&nbsp;&nbsp;&nbsp;&nbsp;</TD>
  <TD align=middle id='prog1'><DIV id="percentage"></DIV></TD>
  </TR>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
  <TR>
    <TD></TD>
    <TD>
      <DIV id=tdShow style="FONT-SIZE: 13px"></DIV></TD></TR>
  <TR>
    <TR>
    <TD></TD>
    <TD>
      <DIV id=tdShowPon style="FONT-SIZE: 13px"></DIV></TD></TR>
<% end if%>
  	<TD>&nbsp;&nbsp;&nbsp;&nbsp;</TD>
    <TD><DIV id="loading"><strong id="loadcss"><% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%><img height=16 width=0><% end if%></strong></DIV>&nbsp;</TD></TR>
  <TR>
    <TD></TD>
    <TD>
      <DIV id=tdResult style="FONT-SIZE: 13px"></DIV></TD></TR>
  <TR id=trReturn style="DISPLAY: none">
    <TD align=middle colSpan=2><INPUT id=btnReturn style="WIDTH: 60px;HEIGHT: 20px" onclick=onReturn(0); type=button value=返回> 
    </TD></TR>
  <TR id=trReturn1 style="DISPLAY: none">
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
        <TR>
          <TD width="45%" align=right><INPUT id=btnReturn1 onclick=onReturn(0); type=button value='返 回'> </TD>
          <TD width="10%">&nbsp;</TD>
          <TD width="45%" align=middle><INPUT type="button" value="恢复出厂设置"  name="buttondefaultbak"  onClick="ReDefaultboot();" ></TD>
        </TR></TBODY></TABLE></TD>
  </TR>
  <TR id=trReDefaultboot style="DISPLAY: none">
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
			  <TR id=trErrorTestCodeTitle style="DISPLAY: none">
			    <TD colSpan=3 align=middle><font style='FONT-SIZE: 13px' align='center'>输入正确验证码，可以帮助恢复默认的出厂配置</font></TD>
			  </TR>
        <TR>
          <TD width="30%" align=right>验证密码;</TD>
          <TD width="8%">&nbsp;</TD>
          <TD width="62%" align=left><INPUT id="TestCode" style="WIDTH: 120px; FONT-FAMILY: '宋体'" type="password" name="TestCode" height="20" value="">&nbsp;<FONT color=red>*</FONT></TD>
        </TR></TBODY></TABLE></TD>
  </TR>
  <TR id=trErrorTestCode style="DISPLAY: none">
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2 align=middle>提示：输入的验证密码不正确，请找支撑经理从ITMS平台获取</TD>
  </TR>
  <TR id="submitbutton" style="DISPLAY: none">
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
        <TR>
          <TD width="45%" align=right ><INPUT id="btnSubmit" style="WIDTH: 60px;HEIGHT: 20px" onclick="onSubmit()" type=button value="提交"></TD>
          <TD width="10%">&nbsp;</TD>
          <TD width="45%" align=left><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onCancel()" type="button" value="取消"></TD></TR></TBODY></TABLE></TD></TR>
<%else%>
    <TD align=middle colSpan=2><INPUT id=btnReturn1 style="WIDTH: 60px;HEIGHT: 20px" onclick=onReturn(1); type=button value=返回> 
    </TD></TR>
<% end if%>
  <TR id=trTime style="DISPLAY: none">
    <TD>&nbsp;</TD>
    <TD id=tdTime 
style="FONT-SIZE: 13px">
<DIV id="regStatus" style="DISPLAY: none"><iframe src="/cgi-bin/regstatus.asp" onload='ifame_load_finish()' id="statusfrm" name="statusfrm" frameborder="0" width="0" height="0"></iframe></DIV>
</TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
	<SCRIPT language=javascript type=text/javascript>
	var isRegSuccess = false;
	var intervalfunFlag = null;
	var limitTimes = "<%tcWebApi_get("deviceAccount_Entry","retryLimit","s") %>";
	var retryTimes = "<%tcWebApi_get("deviceAccount_Entry","retryTimes","s") %>";
	var iretrycount = 0;
	var ilimitcount = 0;
	function getServiceNameId(ServiceName)
	{
		if(ServiceName == "" || ServiceName == "N/A")
			return 0;
			
		switch(ServiceName)
		{
			case "IPTV":
				return 1;
			case "INTERNET":
				return 2;
			case "VOIP":
				return 3;
			default:	//Other Service
				return 9;
		}
	}
	

	var rPhyStatus = "<%tcWebApi_get("XPON_Common","losStatus","s") %>";
	var rLinkState = "<%tcWebApi_get("XPON_Common","trafficStatus","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
	rPhyStatus = 'up'
<% end if%>
	var rstatus = "<%tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
	var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";	
	var rnewStatus = "<%tcWebApi_get("deviceAccount_Entry","newStatus","s") %>";
	var rnewResult = "<%tcWebApi_get("deviceAccount_Entry","newResult","s") %>";

	var acsresult = "<%TCWebApi_get("Cwmp_Entry","Inform_Status","s")%>";
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
	var serviceName = "<%tcWebApi_get("deviceAccount_Entry","ProvisioningCode","s") %>";
<%else%>
	var serviceName = "<%tcWebApi_get("deviceAccount_Entry","serviceName","s") %>";
<% end if%>
	var serviceNum = parseInt("<%tcWebApi_get("deviceAccount_Entry","serviceNum","s") %>");	
	
	var pppup = "<% tcWebApi_get("CwmpRoute_Entry","ifName0","s") %>"; 
	
	var isOLTReg = 0;
	var cwmpRetry = 0;
	var gLefttimes = 0;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
	var gOLTuptime = 0;
	var gITMSRegTime = 0;
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") = "Yes"  then %>
	var gConnectITMS = 0;
	var gITMSStep = 0;
	var gITMSMsg = new Array("注册ITMS成功，等待ITMS平台下发业务数据。"
													, "ITMS平台正在下发业务数据，请勿断电或拔光纤。"
													, "ITMS平台正在下发业务数据，请勿断电或拔光纤。"
													, "ITMS平台正在下发业务数据，请勿断电或拔光纤。"
													, "ITMS平台正在下发业务数据，请勿断电或拔光纤。");
	var gITMSMsgSize = gITMSMsg.length;
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
	var gOLTStep = 0;
	var gOLTMsg = new Array("正在注册OLT..."
													, "正在注册OLT..."
													, "正在注册OLT..."
													, "正在注册OLT..."
													, "正在注册OLT...");
	var gOLTMsgSize = gOLTMsg.length;
	var authStatus="";
	var regsta = true;
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes"  then %>
	var gCwmpWANUP = 0;
	var gConnectITMS = 0;
	var isVoipSet = '';
<% end if%>

	function showMsg()
	{
		percent = getCookie("RegProgressPercent");	
		var rs = getPageStatus();
		
		if (retryTimes != "N/A")
		{
			iretrycount = parseInt(retryTimes);
		}
		if (limitTimes != "N/A")
		{
			ilimitcount = parseInt(limitTimes);
		}
		
		var lefttime = 0;
		
		if(isHasRetryTimesSubmit == 1)
			lefttime = ilimitcount - iretrycount;	
		else
			lefttime = ilimitcount - iretrycount - 1;	
		gLefttimes = lefttime;
		
		if(percent < 1)
		{
			statusFlag = 0;
			percent++;
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
			return "正在注册，请等待。";	
<%else%>
			return "注册中，请等待...";		
<% end if%>
		}
		
		if(rPhyStatus.indexOf('up') < 0)	//phy no link
		{
			statusFlag = 12;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
			return "终端在OLT上注册失败，请检查光纤是否插接正常、或收光功率是否正常、或LOID是否输入正确。";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
			if (percent >= 20)
			return "ITMS注册成功,业务下发失败。";
		else 
			return "光信号异常，请检查光路是否正常或拨打10000。";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
			return "在OLT上注册失败，请检查光信号灯是否处于熄灭状态、Password是否正确。";
<%else%>
			return "在OLT上注册失败，请检查光信号灯是否处于熄灭状态、逻辑ID和密码是否正确或拨打10000。";		
<% end if%>
<% end if%>
<% end if%>
		}
		else if(rLinkState != 'up')	// link no up
		{
			statusFlag = 0;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
			percent = 30; //change to 30%
			isOLTReg = 1;
			if( gettimeoutcnt() >=  uplimitcount_2 )
			{
				statusFlag = 12;
				return "终端在OLT上注册失败，请检查光纤是否插接正常、或收光功率是否正常、或LOID是否输入正确。";
			}
			return "终端正在向OLT发起注册";
<%else%>
			percent = 20; //change to 20%
			isOLTReg = 1;
			if( gettimeoutcnt() >=  uplimitcount_2 )
			{
				statusFlag = 12;
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
				return "在OLT上注册失败，请检查逻辑ID和密码是否正确或拨打10000。";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
				return "在OLT上注册失败，请检查光信号灯是否处于熄灭状态、Password是否正确。";
<%else%>
				return "在OLT上注册失败，请检查光信号灯是否处于熄灭状态、逻辑ID和密码是否正确或拨打10000。";
<% end if%>	
			}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
			else if ((authStatus == 2) || (authStatus == 3) || (authStatus == 4))                           // auth fail
      {
         percent = 40;
         registerend = true;
         regsta = false;
         return "注册OLT失败，LOID不存在，请重试。";
      }
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
			return "家庭网关注册网络。";	
<%else%>
			return "正在注册OLT...";
<% end if%>
<% end if%>
		}
		else	//link up
		{			
			if(rnewStatus != "1")
				rstatus = "99";
			if(rnewResult != "1")
				rresult = "99";
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
			var rs_p = getPageStatus();
			var msg_show = 0;
			refreshtimer = 1000;
			var msg_tmp = gOLTMsg[gOLTStep];
			if ( "1" == rs_p )
			{
				percent += 15;
				if ( gOLTStep < gOLTMsgSize )
				{
					msg_show = 1;
					gOLTStep ++;
				}
			}
			if ( gOLTStep < gOLTMsgSize || 1 == msg_show )
			{
				statusFlag = 0;
				return msg_tmp;
			}
			rstatus = "0";
			rresult = "1";
<% end if%>		
<% if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") = "Yes"  then %>
			var rs_p = getPageStatus();
			var msg_show = 0;
			if ( 1 == gConnectITMS )
			{
				refreshtimer = 1000;
				var msg_tmp = gITMSMsg[gITMSStep];
				if ( "1" == rs_p )
				{
					percent += 10;
					if ( gITMSStep < gITMSMsgSize )
					{
						msg_show = 1;
						gITMSStep ++;
					}
				}
				if ( gITMSStep < gITMSMsgSize
						|| 1 == msg_show )
				{
					statusFlag = 0;
					return msg_tmp;
				}

				rstatus = "0";
				rresult = "1";
			}
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes"  then %>
			var rs_p = getPageStatus();
			if ( '0' == rstatus )
			{
				if ( 1 == gConnectITMS )
				{
					rresult = '0';
					// check timeout
					var web_startTime = getCookie('RegServicBIND2WebStartTime');
					if( (gettimeoutcnt() - web_startTime) >=  service_timeout )
						rresult = '1';
					else
					{
						// check itms voip node status
						if ( 'Yes' == isVoipSet )
							rresult = '1';
					}
				}
				else
				{
					if ('1' == rs_p )
					{
						gConnectITMS = 1;
						var curTimeC = gettimeoutcnt();
						document.cookie = "RegServicBIND2WebStartTime=" + curTimeC.toString();
					}
				}
			}
<% end if%>
			if(rstatus == "N/A" || rstatus == "99")
			{
				if(percent < 30 || pppup == 'N/A'
					|| (30 == percent && '1' == rs && pppup != 'N/A' ))
				{
					statusFlag = 0;
					percent = 30;//change to 30%
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					if ( 0 == gOLTuptime )
						gOLTuptime = gettimeoutcnt();
					if ( ( gettimeoutcnt() - gOLTuptime ) >=  uplimitcount_2 )
					{
						statusFlag = 12;
						return "终端到ITMS的通道不通，请找支撑经理处理。";
					}
					return "终端在OLT已注册成功，下一步是终端向ITMS平台发起注册";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "家庭网关获取管理地址。";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "正在获取管理IP...";	
<%else%>
					return "注册OLT成功，正在获取管理IP...";		
<% end if%>
<% end if%>
				}
			
				if(acsresult != "1" && acsresult != "2"
					|| (isOLTReg = 1 && ++cwmpRetry <= 72 ))
				{
					statusFlag = 0;
					percent = 40; //change to 40%
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes"  then %>
					if ( acsresult != "1" && acsresult != "2" && "2" == rs_p )
					{
						if ( 0 == gCwmpWANUP )
						{
							gCwmpWANUP = 1;
							var curTime = gettimeoutcnt();
							document.cookie = "RegServicBIND2ITMSTimeout=" + curTime.toString();
						}
						else
						{
							// check timeout
							var wanup_startTime = getCookie('RegServicBIND2ITMSTimeout');
							if( (gettimeoutcnt() - wanup_startTime) >=  service_timeout )
							{
								statusFlag = 11;
								return "到ITMS的通道不通，请联系客户经理或拨打10000。";
							}
						}
					}
<% end if%>

<% if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") = "Yes"  then %>
					if ( acsresult != "1" && acsresult != "2" && "2" == rs_p )
						gConnectITMS = 1;
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "家庭网关注册ITMS。";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "已获得管理IP，正在连接RMS...";	
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					if ( 0 == gITMSRegTime )
						gITMSRegTime = gettimeoutcnt();
					if ( ( gettimeoutcnt() - gITMSRegTime ) >=  uplimitcount_3 )
					{
						statusFlag = 13;
						return "终端向ITMS注册超时，请找支撑经理处理。";
					}
<% end if%>
					return "已获得管理IP，正在连接ITMS...";
<% end if%>
				}else
				{
					statusFlag = 11;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					return "终端到ITMS的通道不通，请找支撑经理处理。";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "终端到RMS的通道不通，请联系客户经理或拨打10086。";
<%else%>
					return "到ITMS的通道不通，请联系客户经理或拨打10000。";				
<% end if%>
				}
			}		
			else if (rstatus == "1")
			{
				statusFlag = 1;
				if( lefttime >=0 )
				{
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					if( iretrycount >=5 )
					{
						var tmp = "输入的PASSWORD（密码）不正确，注册失败，请找支撑经理处理";
					}
					else
					{
						var tmp = "输入的PASSWORD（密码）不正确，请核实后重新注册";
					}
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					var tmp = "在RMS上注册失败！正在重试。";					
<% else %>
					var tmp = "在ITMS上注册失败！请检查逻辑ID和密码是否正确或联系客户经理或拨打10000。（剩余尝试次数：" + lefttime.toString() + "）";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					tmp = "LOID或Password不正确，请确认后重新输入（剩余尝试次数：" + lefttime.toString() + "）";
<% end if%>
					return tmp;
				}
				else
				{			
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>		
					return "在RMS上注册失败！请联系客户经理或拨打10086。";
<%else%>
					return "在ITMS上注册失败，请联系客户经理或拨打10000。";
<%end if%>
				}
			}
			else if (rstatus == "2")
			{
				statusFlag = 2;
				if( lefttime >=0 )
				{					
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					if( iretrycount >=5 )
					{
						var tmp = "输入的LOID在ITMS平台不存在，注册失败，请找支撑经理处理";
					}
					else
					{
						var tmp = "输入的LOID在ITMS平台不存在，注册失败，请核实后重新注册";
					}
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					var tmp = "在RMS上注册失败！正在重试。";						
<%else%>
					var tmp = "在ITMS上注册失败！请检查逻辑ID和密码是否正确或联系客户经理或拨打10000。（剩余尝试次数：" + lefttime.toString() + "）";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					tmp = "LOID或Password不正确，请确认后重新输入（剩余尝试次数：" + lefttime.toString() + "）";
<% end if%>
					return tmp;
				}
				else
				{			
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>		
					return "在RMS上注册失败！请联系客户经理或拨打10086。";
<%else%>
					return "在ITMS上注册失败，请联系客户经理或拨打10000。";
<%end if%>
				}
			}
			else if (rstatus == "3")
			{
				statusFlag = 3;
				if( lefttime >=0 )
				{									
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					var tmp = "输入的PASSWORD（密码）不正确，请核实后重新注册";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					var tmp = "在RMS上注册失败！正在重试。";						
<%else%>
					var tmp = "在ITMS上注册失败！请检查逻辑ID和密码是否正确或联系客户经理或拨打10000。（剩余尝试次数：" + lefttime.toString() + "）";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					tmp = "LOID或Password不正确，请确认后重新输入（剩余尝试次数：" + lefttime.toString() + "）";
<% end if%>
					return tmp;
				}
				else
				{
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>		
					return "在RMS上注册失败！请联系客户经理或拨打10086。";
<%else%>
					return "在ITMS上注册失败，请联系客户经理或拨打10000。";
<%end if%>
				}			
			}
			else if (rstatus == "4")
			{
				statusFlag = 4;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
				return "ITMS下发业务超时，请重新注册或找支撑经理处理。";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
				return "注册超时！请检查线路后重试或联系客户经理或拨打10086";
<%else%>
				return "注册超时！请检查线路后重试或联系客户经理或拨打10000";
<% end if%>
			}
			else if (rstatus == "5")
			{
				statusFlag = 5;
				isRegSuccess = true;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
				return "该终端已在ITMS平台成功注册过，请找支撑经理处理后再重新注册。";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
				return "已经在RMS注册成功，无需再注册。";				
<%else%>
				return "已经在ITMS注册成功，无需再注册。";
<% end if%>
			}	
			else if (rstatus == "0")
			{
				if(rresult == "1")
				{
					statusFlag = 7;
					isRegSuccess = true;
					percent = 100;//change to 100%
					var serviceList = "";
					
					if(getCookie("RegServiceIPTV") == 1)
					{
						if(serviceList == "")
							serviceList = serviceList + "iTV";
						else
							serviceList = serviceList + ",iTV";
					}
					
					if(getCookie("RegServiceINTERNET") == 1)
					{
						if(serviceList == "")
							serviceList = serviceList + "上网";
						else
							serviceList = serviceList + ",上网";
					}
						
					if(getCookie("RegServiceVOIP") == 1)
					{
						if(serviceList == "")
							serviceList = serviceList + "语音";
						else
							serviceList = serviceList + ",语音";
					}
					
					if(getCookie("RegServiceOther") == 1)
					{
						if(serviceList == "")
							serviceList = serviceList + "其他";
						else
							serviceList = serviceList + ",其他";
					}
					
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					var infoStr = "ITMS平台数据下发成功，共下发了" + serviceList.toString() + " " + serviceNum.toString() + "个业务。";
<%else%>
					var infoStr = "";
<% end if%>					
					if((serviceNum == "0") || (serviceList.length == 0)){
						infoStr = "ITMS平台业务数据下发成功。";
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
						infoStr = "注册成功，下发业务成功。请关闭页面。";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
						infoStr = "RMS平台业务数据下发成功。";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
						var dateT = new Date();
						var difDateT = (dateT.getTime() - getCookie('RegStartTime')) / 1000;
						var showDateT;
						if ( difDateT >= 60 )
							showDateT = parseInt(difDateT/60) + '分' + parseInt(difDateT%60) + '秒';
						else
							showDateT = parseInt(difDateT%60) + '秒';
						infoStr = "ITMS平台业务数据下发成功，注册总时长为: " + showDateT;
<% end if%>
					}
					else{
						infoStr = "ITMS平台业务数据下发成功，共下发了" + serviceList.toString() + " " + serviceNum.toString() + "个业务。";
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
						infoStr = "注册成功，下发业务成功，共下发了" + serviceList.toString() + " " + serviceNum.toString() + "个业务。请关闭页面。";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
						infoStr = "RMS平台业务数据下发成功，共下发了" + serviceList.toString() + " " + serviceNum.toString() + "个业务。";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
						var dateT = new Date();
						var difDateT = (dateT.getTime() - getCookie('RegStartTime')) / 1000;
						var showDateT;
						if ( difDateT >= 60 )
							showDateT = parseInt(difDateT/60) + '分' + parseInt(difDateT%60) + '秒';
						else
							showDateT = parseInt(difDateT%60) + '秒';
						infoStr = "ITMS平台业务数据下发成功，共下发了" + serviceList.toString() + " " + serviceNum.toString() + "个业务，注册总时长为: " + showDateT;
<% end if%>
					}

<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
					regsta = true;
					var infoStr =  "注册成功。";
<%end if%>						
					return infoStr;
				}
				else if(rresult == "2")
				{
					statusFlag = 8;
					isRegSuccess = true;
					percent = 100;//change to 100%
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					return "ITMS平台业务未下发或业务下发异常，请找支撑经理处理";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "注册成功，业务下发失败，请联系10000号。";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "RMS下发业务异常！请联系客户经理或拨打10086。";							
<%else%>
					return "ITMS下发业务异常！请联系客户经理或拨打10000。";			
<% end if%>
<% end if%>
				}
				else if(rresult == "99")
				{
					statusFlag = 9;
					percent = 50;//change to 50%
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					return "终端在ITMS平台注册成功、下一步是ITMS平台下发数据";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "ITMS业务下发。";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "等待RMS平台下发业务数据。";	
<%else%>
					return "注册ITMS成功，等待ITMS平台下发业务数据。";			
<% end if%>
<% end if%>
				}
				else if(rresult == "0")
				{
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
						if ( percent <= 50 && 9 != statusFlag )
						{
							if ( '1' == rs )
								statusFlag = 9;
							percent = 50;//change to 50%
							return "注册ITMS成功，等待ITMS平台下发业务数据。";
						}
<% end if%>
					var fetchStart = getCookie("RegServiceFetch");	
					var serviceNameId = getServiceNameId(serviceName);
					statusFlag = 10;
					isRegSuccess = true;	
					
					if(fetchStart != 1 || serviceNum <= 0)
					{
						percent = 60;//change to 60%
						if(serviceNameId != 0)
							document.cookie = "RegServiceFetch=1";						
							document.cookie = "RegServiceNameId=0";
							document.cookie = "RegServiceIPTV=0";
							document.cookie = "RegServiceINTERNET=0";
							document.cookie = "RegServiceVOIP=0";
							document.cookie = "RegServiceOther=0";
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
						return "ITMS平台正在进行数据下发、请勿下电或拔插光纤";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "RMS平台正在下发业务数据，请勿断电或拔光纤。";	
<%else%>
						return "ITMS平台正在下发业务数据，请勿断电或拔光纤。";			
<% end if%>
					}else
					{
						var last_ServiceNameId = getCookie("RegServiceNameId");	
						var perServiceStep = parseInt(40 / serviceNum);
						var serviceNameStr = "";
					
						if(percent < 60)
							percent = 60;
	
						document.cookie = "RegServiceNameId=" + serviceNameId.toString();
						switch(serviceNameId)
						{
							case 1:
								document.cookie = "RegServiceIPTV=1";
								serviceNameStr = "iTV";
								break;
							case 2:
								document.cookie = "RegServiceINTERNET=1";
								serviceNameStr = "上网";
								break;			
							case 3:
								document.cookie = "RegServiceVOIP=1";
								serviceNameStr = "语音";
								break;
							default:
								document.cookie = "RegServiceOther=1";
								serviceNameStr = "其他";
								break;										
						}								
						
						if(last_ServiceNameId != serviceNameId)
						{
							percent = percent + perServiceStep;
							if(percent > 99)
								percent = 99;
						}
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>	
						return "RMS平台正在下发" + serviceNameStr.toString() + "业务数据，请勿断电或拔光纤。";	
<% else %>				
						return "ITMS平台正在下发" + serviceNameStr.toString() + "业务数据，请勿断电或拔光纤。";		
<% end if %>
					}
				}
			}
		}
		return "未收到远程业务配置";			
	}
	
	function showRegStatus()
	{
			var rs=getPageStatus();
			isHasRetryTimesSubmit = getCookie("HasRetryTimesSubmit");
			if(((rs == "2") || (rs == "1")) && !registerend)
			{				
				var timeoutcnt=gettimeoutcnt();
				var timeoutcnt_2=gettimeoutcnt_2();
				var statusinfo = showMsg();
				updateprogress();
				if(getCookie("RegStatusChanged_Flag") != statusFlag){
						document.cookie = "RegStatusHasChanged=1";
				}
				var tdResultElement = getElement('tdResult');
				checkQuitstatus();	
				if(rs == "1")
				{
					if(0 == timeoutcnt){
						document.cookie = "RegStatusChanged_Flag=" + statusFlag.toString();
						document.cookie = "RegStatusHasChanged=0";
					}
					setDisplay("loading",1);
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					var HintMsg = "提示: 注册过程中不要随意插拔光纤(10分钟内)";
					var PonMsg = "该终端是<% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>的E8-C终端";
					var tdShowElement = getElement('tdShow');
					tdShowElement.innerHTML = "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + HintMsg + "</font></div>";
					var tdShowPonElement = getElement('tdShowPon');
					tdShowPonElement.innerHTML = "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + PonMsg + "</font></div>";
<%end if%>
					tdResultElement.innerHTML = "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + statusinfo + "</font></div>";
					setTimeout(refreshPage,refreshtimer);
				}
				if(rs == "2")
				{
					var isReshasChanged = getCookie("RegStatusHasChanged");
					if((0 != statusFlag) && (9 != statusFlag) && (10 != statusFlag) && isReshasChanged){
						with (getElById('ConfigForm'))
						{
							registerend = true;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
							setDisplay("tdShow", 0);
							setDisplay("tdShowPon", 0);
<%end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYJXSupported", "h") = "Yes"  then %>
							setDisplay("loading",1);
<%else%>
							setDisplay("loading",0);
<%end if%>
							setDisplay("trReturn", 1);
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
							hidePercent();
<% end if%>
							if(statusFlag < 4 && isHasRetryTimesSubmit != 1){
								document.ConfigForm.tryTimes.value = 1 + iretrycount;
								document.ConfigForm.tryTimesflg.value = "1";
								document.cookie = "HasRetryTimesSubmit=1";
								document.cookie = "PageUpdate=1";
								document.ConfigForm.submit();
							}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>							
							tdResultElement.innerHTML = "<P>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong><font color='#FF0000' size='2'>" + statusinfo + "</font></strong></P>";
<% else %>						
							tdResultElement.innerHTML = "<P><strong><font color='#FF0000' size='2'>" + statusinfo + "</font></strong></P>";
<% end if %>
						}
					}
					else{
						if(timeoutcnt <  uplimitcount)
						{
							if(percent == 40)
							{
								if(timeoutcnt_2 <  regITMSlimitcount)
								{
										document.cookie = "RegPagetimeoutcount_2=" + (timeoutcnt_2+1).toString();
								}						
								else
								{
									registerend = true;
									setDisplay("trReturn", 1);
									setDisplay("loading",0);
									tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">在ITMS上注册超时!请检查线路后重试或联系客户经理或拨打10000</font></strong></P>';
								}
							}				
							document.cookie = "RegPageStatus=1";
							document.cookie = "RegPagetimeoutcount=" + (timeoutcnt+1).toString();
							//document.ConfigForm.submit();		
							showRegStatus();
						}
						else
						{
							registerend = true;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
							setDisplay("tdShow", 0);
							setDisplay("tdShowPon", 0);
<%end if%>
							setDisplay("trReturn", 1);
							setDisplay("loading",0);
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
							hidePercent();
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>	
						  tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">到RMS的通道不通，请联系客户经理或拨打10086</font></strong></P>';
<% else %>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
							tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">ITMS业务下发超时，请重新注册或找支撑经理处理</font></strong></P>';
<% elseif tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes" then%>
							tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">注册ITMS失败，请联系客户经理或拨打10000</font></strong></P>';
<% else %>
							tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">到ITMS的通道不通，请联系客户经理或拨打10000</font></strong></P>';
<% end if %>
<%end if%>
						}
					}
				}
			}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>			
			else{
				if(regsta == false){
					var statusinfo = showMsg();
					var tdResultElement = getElement('tdResult');
					updateprogress();
					setDisplay("trReturn", 1);
					tdResultElement.innerHTML = "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + statusinfo + "</font></div>";
				}
			}
<% end if %>			
	}
	
	var isStateGetOk = 0;
	function checkSyncPageFinish()
	{
		checkSyncPage();
	}
	setTimeout("checkSyncPageFinish()", 10000);

	function checkSyncPage()
	{
		var stobj = window.frames['statusfrm'].location;

		if ( stobj )
		{
			if ( '/cgi-bin/regstatus.asp' != stobj.pathname )
			{
				window.frames['statusfrm'].location.href = '/cgi-bin/regstatus.asp';
				setTimeout(checkSyncPage, 10000);
				return 0;
			}
			else if ( 0 == isStateGetOk )
			{
				syncRegStatus();
				setTimeout(checkSyncPage, 10000);
			}
		}

		return 1;
	}
	
	function syncRegStatus()
	{
		var stobj = window.frames['statusfrm'].location;
		if ( stobj )
			stobj.reload();
	}
	
	var isFirstLoad = 1;
	function ifame_load_finish()
	{
		var ifameObj = window.frames['statusfrm'];

		if ( isFirstLoad || 0 == checkSyncPage() )
		{
			isFirstLoad = 0;
			return;	
		}
		
		if ( null != ifameObj && null != ifameObj.rPhyStatus )
		{
			rPhyStatus = ifameObj.rPhyStatus;
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
			rPhyStatus = 'up'
<% end if%>
			rLinkState = ifameObj.rLinkState;
			
			rstatus = ifameObj.rstatus;
			rresult = ifameObj.rresult;
			rnewStatus = ifameObj.rnewStatus;
			rnewResult = ifameObj.rnewResult;
			
			acsresult = ifameObj.acsresult;

			serviceName = ifameObj.serviceName;
			serviceNum = ifameObj.serviceNum;
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>			
			authStatus = ifameObj.authStatus;
<% end if%>			
			pppup = ifameObj.pppup;
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes"  then %>
			isVoipSet = ifameObj.isVoipSet;
<% end if%>
			isStateGetOk = 1;
		}
		
		showRegStatus();
	}
	
	showRegStatus();
	</SCRIPT>

</FORM></BODY></HTML>
