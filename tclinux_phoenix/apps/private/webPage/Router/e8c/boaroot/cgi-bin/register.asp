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
<HTML><HEAD><TITLE>ҵ��ע��</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="JavaScript" type="text/JavaScript">
<%if tcWebApi_get("WebCustom_Entry", "isCTPONJSSupported", "h") = "Yes" then%>
function showmsg_info()
{
	alert('�߼�ID����ɹ����豸����ע�ᣬ��¼ҳ���ɲ鿴ע��״̬��');
	top.window.location.href='/cgi-bin/index2.asp';
}
<% else %>
<%if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2Supported", "h") = "Yes" then%>
function showmsg_info()
{
	alert('�߼�ID����ɹ����豸����ע�ᣬ��¼ҳ���ɲ鿴ע��״̬��');
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
	document.write('<A href="' + httpip + '" onclick="resetCookie();">���ص�¼ҳ��</A>');
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
		var titles = "<div align='center' style='padding-bottom:5px'><font style='FONT-SIZE: 12px' align='center'><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>����E8-C�ն�ҵ��ע����ʾ��</font></div>";
		tdResultElement.innerHTML = titles + "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + "�豸��ע�ᡢ������ע�ᣬ��ָ��������òſ���ע�ᡣ" + "</font></div>";
		setDisplay('mainScreen', 0);
		setDisplay('promptScreen', 1);
		setDisplay("loading",0);
		setDisplay("trReturn1", 1);
		return;
	<%end if%>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
	getElement("hint2Text").innerHTML="��LOID���͡�Password��";  
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
				tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">��RMS��ע��ʧ�ܣ���3���Ӻ����Ի���ϵ�ͻ�����򲦴�10086</font></strong></P>';
<%else%>			
				tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">��ITMS��ע��ʧ�ܣ���3���Ӻ����Ի���ϵ�ͻ�����򲦴�10000</font></strong></P>';
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
		document.writeln("Version Type����������豸�����У���ȴ�����");
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
	tdResultElement.innerHTML = "<div align='center'><font style='FONT-SIZE: 13px' align='center'>" + "������ȷ��֤�룬���԰����ָ�Ĭ�ϵĳ�������" + "</font></div>";
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
		alert("�߼�ID����Ϊ��!");
		return false;
	}

	if (bandName.length > 128)
	{
		alert("�߼�ID���Ȳ��ܴ���128λ!");
		return false;
	}
<% end if %>
	if (customer.length > 128)
	{
		alert("���볤�Ȳ��ܴ���128λ!");
		return false;
	}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>
	var reg = /^[a-zA-Z0-9][\x21-\x73]*[a-zA-Z0-9]$|^[a-zA-Z0-9]+$/;						
	
	if (reg.test(bandName) == false)
	{
   	alert("LOIDֻ������ĸ���ֿ�ͷ�ͽ�β���м䲻�ܺ��пո�");
		return;
	} 
	  
	if (customer.length>0 && reg.test(customer)== false)
	{
		alert("Passwordֻ������ĸ���ֿ�ͷ�ͽ�β���м䲻�ܺ��пո�");
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
		alert("��֤���벻��Ϊ��!");
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
        <TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>����<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>�ն�ҵ��ע����ʾ��</TD></TR>
    <% else %>
    	  <TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>����E8-C�ն�ҵ��ע����ʾ��</TD></TR>
    <% end if %>
<% else %>
    <% if tcwebApi_get("WebCustom_Entry","isCTPONYNSupported","h") = "Yes" then %>
        <TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>����<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>�ն�ҵ��ע����ʾ��</TD></TR>
    <% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
    		<TD style="FONT-SIZE: 12px" colSpan=2>GPON����E8-C�ն�ҵ��ע����ʾ��</TD></TR>
    <% elseif tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
    		<TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>����<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>�ն�ҵ��ע����ʾ��</TD></TR>
		<% elseif tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>
				<TD style="FONT-SIZE: 12px" colSpan=2><% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>����<% if tcwebApi_get("WebCustom_Entry","isVOIPSupported","h") = "Yes" then %>E8-C<% else %>E8-B<% end if %>�ն�ҵ��ע����ʾ��</TD></TR>
    <% else %>
        <TD style="FONT-SIZE: 12px" colSpan=2>EPON����E8-C�ն�ҵ��ע����ʾ��</TD></TR>
    <% end if %>
<% end if %>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>1.��Ҫ�������˺���ȷ���ն˹��źŵ�Ϩ��</TD></TR>
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
		<TD style="FONT-SIZE: 12px" colSpan=2>1.�������˲�ȷ�Ϲ��źŵ��Ѵ���Ϩ��״̬</TD></TR>
<% elseif tcWebApi_get("WebCustom_Entry", "isCTPONCYSupported", "h") = "Yes"  then %>
		<TD style="FONT-SIZE: 12px" colSpan=2>1.����������E/G���ӿڵĹ��ˣ���鲢ȷ�Ϲ��źŵ��Ѵ���Ϩ��״̬</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>1.����������E/G���ӿڵĹ��ˣ���鲢ȷ�Ϲ��źŵ��Ѵ���Ϩ��״̬</TD></TR>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then %>
  <% if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
    <TR style="display:none">
  <%else%>
    <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.���źų���������LOID��PASSWORD�����룩����ע�ᡣ�������Ϊ����ʺţ�����@��׺�����绰���루�������ţ���LOID����ITV���룩��</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.׼ȷ����<a id="hint2Text">���߼�ID���͡����롱</a>�������ȷ��������ע�������ͥ<br/>&nbsp; ���ؿ��������豸</TD></TR>
<% end if%>
  <% end if %>
<%else%>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.���źų���������LOID��PASSWORD�����룩����ע�ᡣ�������Ϊ����ʺţ�����@��׺�����绰���루�������ţ���LOID����ITV���룩��</TD></TR>
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
		<TD style="FONT-SIZE: 12px" colSpan=2>2.׼ȷ���롰Password���������ȷ��������ע��</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.׼ȷ����<a id="hint2Text">���߼�ID���͡����롱</a>�������ȷ��������ע��</TD></TR>
<% end if%>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
		  <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>2.��ע������в�Ҫ����β���ˣ�10�����ڣ���</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>2.��ע�ἰҵ���·������У�10 �����ڣ���Ҫ�ϵ硢��Ҫ�ι���</TD></TR>
		<% end if%>
		  <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.��ע�Ṧ�ܽ��������豸����֤��ҵ���·��������������豸��������ע�ᡣ</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.��ע�Ṧ�ܽ��������豸����֤��ҵ���·��������������豸��������ע��</TD></TR>
		<% end if%>
		  </TR>
   <%else%>
		 <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.��ע������в�Ҫ����β���ˣ�10�����ڣ���</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>3.��ע�ἰҵ���·������У�10 �����ڣ���Ҫ�ϵ硢��Ҫ�ι���</TD></TR>
		<% end if%>
		  <TR>
		    <TD width="8%">&nbsp;</TD>
		<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		    <TD style="FONT-SIZE: 12px" colSpan=2>4.��ע�Ṧ�ܽ��������豸����֤��ҵ���·��������������豸��������ע�ᡣ</TD></TR>
		<%else%>
		    <TD style="FONT-SIZE: 12px" colSpan=2>4.��ע�Ṧ�ܽ��������豸����֤��ҵ���·��������������豸��������ע��</TD></TR>
		<% end if%>
		  </TR>
   <% end if
else%>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>3.��ע������в�Ҫ����β���ˣ�10�����ڣ���</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>3.��ע�ἰҵ���·������У�10 �����ڣ���Ҫ�ϵ硢��Ҫ�ι���</TD></TR>
<% end if%>
  <TR>
    <TD width="8%">&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD style="FONT-SIZE: 12px" colSpan=2>4.���ն���<% if tcwebApi_get("XPON_Common","xponMode","h") = "GPON" then %>GPON<% else %>EPON<% end if %>��E8-C�նˡ�</TD></TR>
<%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>4.��ע�Ṧ�ܽ��������豸����֤��ҵ���·��������������豸��������ע��</TD></TR>
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
			  <TD width="38%" align="right" style="FONT-SIZE: 13px" id="loidText">�߼�ID:</TD>
	<%end if%>
    <TD width="60%"><INPUT id="broadbandusername" style="WIDTH: 120px; FONT-FAMILY: '����'" <% if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>maxLength=24<%end if%>
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
			  <TD style="FONT-SIZE: 13px" align="right" id="passwordText">����:</TD>
	<%end if%>
    <TD><INPUT id="customer_ID" style="WIDTH: 120px; FONT-FAMILY: '����'" <% if tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "h") = "Yes"  then %>maxLength=12<%end if%> 
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
              <TD>ע����,���Ե�...</TD>
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
          <TD width="20%" align=right ><INPUT id="btnOK" style="WIDTH: 60px;HEIGHT: 20px" onclick="onOK()" type=button value="ȷ��"></TD>
<%else%>
					<TD width="45%" align=right ><INPUT id="btnOK" style="WIDTH: 60px;HEIGHT: 20px" onclick="onOK()" type=button value="ȷ��"></TD>
<% end if%>         
<% if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes"  then%> 
					<TD width="10%"><INPUT id="btnSetting" style="WIDTH: 70px;HEIGHT: 20px" onclick="onSetting()" type=button value="�߼�����"></TD>
<%else%>
          <TD width="10%">&nbsp;</TD>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then 
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then %>
          <TD width="45%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="����"></TD></TR></TBODY></TABLE></TD></TR>
          <DIV id=ServiceConfiguration>
            <TR align=center>
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2><INPUT id=SerConfig onclick="onOK()" type=button value=ҵ������ name=SerConfig></TD></TR></DIV>
	</TBODY></TABLE></TD></TR></TABLE></DIV>
   <%else%>
          <TD width="45%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="����"></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
   <% end if
else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>  
          <TD width="20%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="����">
<%else%>
					<TD width="45%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="����">
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>    
          <TD width="20%" align=left ><INPUT id="btnCancel" style="WIDTH: 65px;HEIGHT: 20px" onclick="btnselfCheck()" type="button" value="�ն��Լ�">       	
<% end if%></TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					<TD width="20%" align=left ><INPUT id="btnCancel" style="WIDTH: 65px;HEIGHT: 20px" onclick="restore()" type="button" value="�ָ�����"></TD>
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
    <TD align=middle colSpan=2><INPUT id=btnReturn style="WIDTH: 60px;HEIGHT: 20px" onclick=onReturn(0); type=button value=����> 
    </TD></TR>
  <TR id=trReturn1 style="DISPLAY: none">
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
        <TR>
          <TD width="45%" align=right><INPUT id=btnReturn1 onclick=onReturn(0); type=button value='�� ��'> </TD>
          <TD width="10%">&nbsp;</TD>
          <TD width="45%" align=middle><INPUT type="button" value="�ָ���������"  name="buttondefaultbak"  onClick="ReDefaultboot();" ></TD>
        </TR></TBODY></TABLE></TD>
  </TR>
  <TR id=trReDefaultboot style="DISPLAY: none">
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
			  <TR id=trErrorTestCodeTitle style="DISPLAY: none">
			    <TD colSpan=3 align=middle><font style='FONT-SIZE: 13px' align='center'>������ȷ��֤�룬���԰����ָ�Ĭ�ϵĳ�������</font></TD>
			  </TR>
        <TR>
          <TD width="30%" align=right>��֤����;</TD>
          <TD width="8%">&nbsp;</TD>
          <TD width="62%" align=left><INPUT id="TestCode" style="WIDTH: 120px; FONT-FAMILY: '����'" type="password" name="TestCode" height="20" value="">&nbsp;<FONT color=red>*</FONT></TD>
        </TR></TBODY></TABLE></TD>
  </TR>
  <TR id=trErrorTestCode style="DISPLAY: none">
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2 align=middle>��ʾ���������֤���벻��ȷ������֧�ž����ITMSƽ̨��ȡ</TD>
  </TR>
  <TR id="submitbutton" style="DISPLAY: none">
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
        <TR>
          <TD width="45%" align=right ><INPUT id="btnSubmit" style="WIDTH: 60px;HEIGHT: 20px" onclick="onSubmit()" type=button value="�ύ"></TD>
          <TD width="10%">&nbsp;</TD>
          <TD width="45%" align=left><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onCancel()" type="button" value="ȡ��"></TD></TR></TBODY></TABLE></TD></TR>
<%else%>
    <TD align=middle colSpan=2><INPUT id=btnReturn1 style="WIDTH: 60px;HEIGHT: 20px" onclick=onReturn(1); type=button value=����> 
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
	var gITMSMsg = new Array("ע��ITMS�ɹ����ȴ�ITMSƽ̨�·�ҵ�����ݡ�"
													, "ITMSƽ̨�����·�ҵ�����ݣ�����ϵ��ι��ˡ�"
													, "ITMSƽ̨�����·�ҵ�����ݣ�����ϵ��ι��ˡ�"
													, "ITMSƽ̨�����·�ҵ�����ݣ�����ϵ��ι��ˡ�"
													, "ITMSƽ̨�����·�ҵ�����ݣ�����ϵ��ι��ˡ�");
	var gITMSMsgSize = gITMSMsg.length;
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
	var gOLTStep = 0;
	var gOLTMsg = new Array("����ע��OLT..."
													, "����ע��OLT..."
													, "����ע��OLT..."
													, "����ע��OLT..."
													, "����ע��OLT...");
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
			return "����ע�ᣬ��ȴ���";	
<%else%>
			return "ע���У���ȴ�...";		
<% end if%>
		}
		
		if(rPhyStatus.indexOf('up') < 0)	//phy no link
		{
			statusFlag = 12;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
			return "�ն���OLT��ע��ʧ�ܣ���������Ƿ������������չ⹦���Ƿ���������LOID�Ƿ�������ȷ��";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
			if (percent >= 20)
			return "ITMSע��ɹ�,ҵ���·�ʧ�ܡ�";
		else 
			return "���ź��쳣�������·�Ƿ������򲦴�10000��";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
			return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬��Password�Ƿ���ȷ��";
<%else%>
			return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬���߼�ID�������Ƿ���ȷ�򲦴�10000��";		
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
				return "�ն���OLT��ע��ʧ�ܣ���������Ƿ������������չ⹦���Ƿ���������LOID�Ƿ�������ȷ��";
			}
			return "�ն�������OLT����ע��";
<%else%>
			percent = 20; //change to 20%
			isOLTReg = 1;
			if( gettimeoutcnt() >=  uplimitcount_2 )
			{
				statusFlag = 12;
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
				return "��OLT��ע��ʧ�ܣ������߼�ID�������Ƿ���ȷ�򲦴�10000��";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
				return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬��Password�Ƿ���ȷ��";
<%else%>
				return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬���߼�ID�������Ƿ���ȷ�򲦴�10000��";
<% end if%>	
			}
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
			else if ((authStatus == 2) || (authStatus == 3) || (authStatus == 4))                           // auth fail
      {
         percent = 40;
         registerend = true;
         regsta = false;
         return "ע��OLTʧ�ܣ�LOID�����ڣ������ԡ�";
      }
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
			return "��ͥ����ע�����硣";	
<%else%>
			return "����ע��OLT...";
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
						return "�ն˵�ITMS��ͨ����ͨ������֧�ž�����";
					}
					return "�ն���OLT��ע��ɹ�����һ�����ն���ITMSƽ̨����ע��";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "��ͥ���ػ�ȡ�����ַ��";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "���ڻ�ȡ����IP...";	
<%else%>
					return "ע��OLT�ɹ������ڻ�ȡ����IP...";		
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
								return "��ITMS��ͨ����ͨ������ϵ�ͻ�����򲦴�10000��";
							}
						}
					}
<% end if%>

<% if tcWebApi_get("WebCustom_Entry", "isCTPONHLJSupported", "h") = "Yes"  then %>
					if ( acsresult != "1" && acsresult != "2" && "2" == rs_p )
						gConnectITMS = 1;
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "��ͥ����ע��ITMS��";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "�ѻ�ù���IP����������RMS...";	
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					if ( 0 == gITMSRegTime )
						gITMSRegTime = gettimeoutcnt();
					if ( ( gettimeoutcnt() - gITMSRegTime ) >=  uplimitcount_3 )
					{
						statusFlag = 13;
						return "�ն���ITMSע�ᳬʱ������֧�ž�����";
					}
<% end if%>
					return "�ѻ�ù���IP����������ITMS...";
<% end if%>
				}else
				{
					statusFlag = 11;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					return "�ն˵�ITMS��ͨ����ͨ������֧�ž�����";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "�ն˵�RMS��ͨ����ͨ������ϵ�ͻ�����򲦴�10086��";
<%else%>
					return "��ITMS��ͨ����ͨ������ϵ�ͻ�����򲦴�10000��";				
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
						var tmp = "�����PASSWORD�����룩����ȷ��ע��ʧ�ܣ�����֧�ž�����";
					}
					else
					{
						var tmp = "�����PASSWORD�����룩����ȷ�����ʵ������ע��";
					}
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					var tmp = "��RMS��ע��ʧ�ܣ��������ԡ�";					
<% else %>
					var tmp = "��ITMS��ע��ʧ�ܣ������߼�ID�������Ƿ���ȷ����ϵ�ͻ�����򲦴�10000����ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					tmp = "LOID��Password����ȷ����ȷ�Ϻ��������루ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
<% end if%>
					return tmp;
				}
				else
				{			
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>		
					return "��RMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10086��";
<%else%>
					return "��ITMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10000��";
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
						var tmp = "�����LOID��ITMSƽ̨�����ڣ�ע��ʧ�ܣ�����֧�ž�����";
					}
					else
					{
						var tmp = "�����LOID��ITMSƽ̨�����ڣ�ע��ʧ�ܣ����ʵ������ע��";
					}
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					var tmp = "��RMS��ע��ʧ�ܣ��������ԡ�";						
<%else%>
					var tmp = "��ITMS��ע��ʧ�ܣ������߼�ID�������Ƿ���ȷ����ϵ�ͻ�����򲦴�10000����ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					tmp = "LOID��Password����ȷ����ȷ�Ϻ��������루ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
<% end if%>
					return tmp;
				}
				else
				{			
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>		
					return "��RMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10086��";
<%else%>
					return "��ITMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10000��";
<%end if%>
				}
			}
			else if (rstatus == "3")
			{
				statusFlag = 3;
				if( lefttime >=0 )
				{									
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					var tmp = "�����PASSWORD�����룩����ȷ�����ʵ������ע��";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					var tmp = "��RMS��ע��ʧ�ܣ��������ԡ�";						
<%else%>
					var tmp = "��ITMS��ע��ʧ�ܣ������߼�ID�������Ƿ���ȷ����ϵ�ͻ�����򲦴�10000����ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					tmp = "LOID��Password����ȷ����ȷ�Ϻ��������루ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
<% end if%>
					return tmp;
				}
				else
				{
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>		
					return "��RMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10086��";
<%else%>
					return "��ITMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10000��";
<%end if%>
				}			
			}
			else if (rstatus == "4")
			{
				statusFlag = 4;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
				return "ITMS�·�ҵ��ʱ��������ע�����֧�ž�����";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
				return "ע�ᳬʱ��������·�����Ի���ϵ�ͻ�����򲦴�10086";
<%else%>
				return "ע�ᳬʱ��������·�����Ի���ϵ�ͻ�����򲦴�10000";
<% end if%>
			}
			else if (rstatus == "5")
			{
				statusFlag = 5;
				isRegSuccess = true;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
				return "���ն�����ITMSƽ̨�ɹ�ע���������֧�ž������������ע�ᡣ";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
				return "�Ѿ���RMSע��ɹ���������ע�ᡣ";				
<%else%>
				return "�Ѿ���ITMSע��ɹ���������ע�ᡣ";
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
							serviceList = serviceList + "����";
						else
							serviceList = serviceList + ",����";
					}
						
					if(getCookie("RegServiceVOIP") == 1)
					{
						if(serviceList == "")
							serviceList = serviceList + "����";
						else
							serviceList = serviceList + ",����";
					}
					
					if(getCookie("RegServiceOther") == 1)
					{
						if(serviceList == "")
							serviceList = serviceList + "����";
						else
							serviceList = serviceList + ",����";
					}
					
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					var infoStr = "ITMSƽ̨�����·��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ��";
<%else%>
					var infoStr = "";
<% end if%>					
					if((serviceNum == "0") || (serviceList.length == 0)){
						infoStr = "ITMSƽ̨ҵ�������·��ɹ���";
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
						infoStr = "ע��ɹ����·�ҵ��ɹ�����ر�ҳ�档";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
						infoStr = "RMSƽ̨ҵ�������·��ɹ���";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
						var dateT = new Date();
						var difDateT = (dateT.getTime() - getCookie('RegStartTime')) / 1000;
						var showDateT;
						if ( difDateT >= 60 )
							showDateT = parseInt(difDateT/60) + '��' + parseInt(difDateT%60) + '��';
						else
							showDateT = parseInt(difDateT%60) + '��';
						infoStr = "ITMSƽ̨ҵ�������·��ɹ���ע����ʱ��Ϊ: " + showDateT;
<% end if%>
					}
					else{
						infoStr = "ITMSƽ̨ҵ�������·��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ��";
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
						infoStr = "ע��ɹ����·�ҵ��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ����ر�ҳ�档";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
						infoStr = "RMSƽ̨ҵ�������·��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ��";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYCQSupported", "h") = "Yes" then%>
						var dateT = new Date();
						var difDateT = (dateT.getTime() - getCookie('RegStartTime')) / 1000;
						var showDateT;
						if ( difDateT >= 60 )
							showDateT = parseInt(difDateT/60) + '��' + parseInt(difDateT%60) + '��';
						else
							showDateT = parseInt(difDateT%60) + '��';
						infoStr = "ITMSƽ̨ҵ�������·��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ��ע����ʱ��Ϊ: " + showDateT;
<% end if%>
					}

<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes"  then %>
					regsta = true;
					var infoStr =  "ע��ɹ���";
<%end if%>						
					return infoStr;
				}
				else if(rresult == "2")
				{
					statusFlag = 8;
					isRegSuccess = true;
					percent = 100;//change to 100%
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					return "ITMSƽ̨ҵ��δ�·���ҵ���·��쳣������֧�ž�����";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "ע��ɹ���ҵ���·�ʧ�ܣ�����ϵ10000�š�";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "RMS�·�ҵ���쳣������ϵ�ͻ�����򲦴�10086��";							
<%else%>
					return "ITMS�·�ҵ���쳣������ϵ�ͻ�����򲦴�10000��";			
<% end if%>
<% end if%>
				}
				else if(rresult == "99")
				{
					statusFlag = 9;
					percent = 50;//change to 50%
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					return "�ն���ITMSƽ̨ע��ɹ�����һ����ITMSƽ̨�·�����";
<%else%>
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "ITMSҵ���·���";	
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "�ȴ�RMSƽ̨�·�ҵ�����ݡ�";	
<%else%>
					return "ע��ITMS�ɹ����ȴ�ITMSƽ̨�·�ҵ�����ݡ�";			
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
							return "ע��ITMS�ɹ����ȴ�ITMSƽ̨�·�ҵ�����ݡ�";
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
						return "ITMSƽ̨���ڽ��������·��������µ��β����";
<% elseif tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>
					return "RMSƽ̨�����·�ҵ�����ݣ�����ϵ��ι��ˡ�";	
<%else%>
						return "ITMSƽ̨�����·�ҵ�����ݣ�����ϵ��ι��ˡ�";			
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
								serviceNameStr = "����";
								break;			
							case 3:
								document.cookie = "RegServiceVOIP=1";
								serviceNameStr = "����";
								break;
							default:
								document.cookie = "RegServiceOther=1";
								serviceNameStr = "����";
								break;										
						}								
						
						if(last_ServiceNameId != serviceNameId)
						{
							percent = percent + perServiceStep;
							if(percent > 99)
								percent = 99;
						}
<%if tcWebApi_get("WebCustom_Entry", "isCMCCSupported", "h") = "Yes"  then %>	
						return "RMSƽ̨�����·�" + serviceNameStr.toString() + "ҵ�����ݣ�����ϵ��ι��ˡ�";	
<% else %>				
						return "ITMSƽ̨�����·�" + serviceNameStr.toString() + "ҵ�����ݣ�����ϵ��ι��ˡ�";		
<% end if %>
					}
				}
			}
		}
		return "δ�յ�Զ��ҵ������";			
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
					var HintMsg = "��ʾ: ע������в�Ҫ�����ι���(10������)";
					var PonMsg = "���ն���<% if tcwebApi_get("XPON_Common","xponMode","h") = "EPON" then %>EPON<% else %>GPON<% end if %>��E8-C�ն�";
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
									tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">��ITMS��ע�ᳬʱ!������·�����Ի���ϵ�ͻ�����򲦴�10000</font></strong></P>';
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
						  tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">��RMS��ͨ����ͨ������ϵ�ͻ�����򲦴�10086</font></strong></P>';
<% else %>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
							tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">ITMSҵ���·���ʱ��������ע�����֧�ž�����</font></strong></P>';
<% elseif tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes" then%>
							tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">ע��ITMSʧ�ܣ�����ϵ�ͻ�����򲦴�10000</font></strong></P>';
<% else %>
							tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">��ITMS��ͨ����ͨ������ϵ�ͻ�����򲦴�10000</font></strong></P>';
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
