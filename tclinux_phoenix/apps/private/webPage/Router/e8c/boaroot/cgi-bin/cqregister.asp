<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!-- saved from url=(0039)http://192.168.1.1/html/AutobandReg.asp -->
<HTML><HEAD><TITLE>ҵ��ע��</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="JavaScript" type="text/JavaScript">
var	statusFlag = 0;
var isHasRetryTimesSubmit = 0;
var	registerend = false;
<%if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then%>
var	uplimitcount = 24;//count of refresh
var uplimitcount_2 = 24; // 2 miniutes
<%else%>
var	uplimitcount = 120;//count of refresh
var uplimitcount_2 = 36; // 3 miniutes
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
	top.window.location.href = 'http://' + lanip_v6 + '/cgi-bin/cqregister.asp';
else if ( '1' == isIPv4 && top.window.location.href.indexOf(lanip_v4) < 0 )
	top.window.location.href = 'http://' + lanip_v4 + '/cgi-bin/cqregister.asp';

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
	document.cookie = "RegProgressPercent=-1";
	document.cookie = "RegServiceFetch=0";
	document.cookie = "RegServiceNameId=0";
	document.cookie = "RegServiceIPTV=0";
	document.cookie = "RegServiceINTERNET=0";
	document.cookie = "RegServiceVOIP=0";
	document.cookie = "RegServiceOther=0";	
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
	with (getElById('ConfigForm'))
	{	
		setDisplay('mainScreen', 1);
		setDisplay('promptScreen', 0);
	}
}

function refreshPage()
{
	document.cookie = "RegPageStatus=2";
	syncRegStatus();
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

function onReturn()
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

	location.replace(httpip);
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
	var bandName = getValue("broadbandusername");
	var customer = getValue("customer_ID");
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
	if (customer.length > 128)
	{
		alert("���볤�Ȳ��ܴ���128λ!");
		return false;
	}
	return true;
}

function onReset()
{
	with (getElById('ConfigForm'))
	{
		if( !confirm("��ȷ����Ҫ�ָ�������ֵ?") )
			return false;

		rest_psw.value = 'cqreset';
		submit();
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
		document.cookie = "RegProgressPercent=-1";
		document.cookie = "RegServiceFetch=0";
		document.cookie = "RegServiceNameId=0";
		document.cookie = "RegServiceIPTV=0";
		document.cookie = "RegServiceINTERNET=0";
		document.cookie = "RegServiceVOIP=0";
		document.cookie = "RegServiceOther=0";
		document.cookie = "HasRetryTimesSubmit=0";
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
	document.ConfigForm.restoreflag2.value = "2";
	document.ConfigForm.submit();
	getElById('buttondefaultbak').disabled = true;	
}
<% end if%>
function pageHeight()
{
	return window.innerHeight != null? window.innerHeight : document.documentElement && document.documentElement.clientHeight ? document.documentElement.clientHeight : document.body != null? document.body.clientHeight : null;
}
</SCRIPT>

<FORM id="ConfigForm" name="ConfigForm" method="post" action="/cgi-bin/cqreset.asp" >
	<INPUT TYPE="HIDDEN" NAME="rest_psw" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="Register_CHANGE" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="tryTimesflg" VALUE="-1">
	<INPUT TYPE="HIDDEN" NAME="tryTimes" VALUE="-1">
  	<input type="hidden" name="devReginfoFlag" value="1">
  	<input type="hidden" name="newStatusFlag" value="0">
  <input name="result_startvalue" type="hidden" value="99">
</FORM>
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
		tdResultprogress.innerHTML = percent.toString() + "%";
	}
	
	document.cookie = "RegProgressPercent=" + percent.toString();
}
</SCRIPT>
<TR valign="top" height="40%"><TD>
<TABLE style="MARGIN-TOP: 150px" align=right>
  <TBODY>
  <TR>
    <TD style="MARGIN-TOP: 100px; FONT-SIZE: 13px">
	<script language="JavaScript" type="text/JavaScript">
	writeReturnhref();
	</script>
	</TD>
    <TD width="50%">&nbsp;</TD></TR></TBODY></TABLE></TD></TR><TR valign="top"><TD colspan="3">

<TABLE align="center">
  <TBODY>
	<TR>
    <TD width="8%">&nbsp;</TD>
   <TD style="FONT-SIZE: 17px" colSpan=2 align='center'>
   		�ù��ܽ��޵���רҵ��Աʹ�á�</TD></TR>
  <TR>
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 17px" colSpan="2" align='center' >��ͨ�û��������������ҵ���жϡ�</TD></TR>
  <TR>
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 12px" colSpan=2>&nbsp;</TD></TR>
  <TR>
    <TD style="FONT-SIZE: 12px" colSpan=3 align='center'>&nbsp;&nbsp;�豸��ע�ᣬ������ע�ᣬ��ָ��������òſ���ע�ᡣ</TD></TR>

  <TR>
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 12px" colSpan=2>&nbsp;</TD></TR>
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
					<TD width="45%" align=right ><INPUT  onclick="onReturn()" type=button value="����"></TD>
     
          <TD width="10%">&nbsp;</TD>
					<TD width="45%" align=left ><INPUT  onclick="onReset()" type="button" value="�ָ���������">
					</TD>
					</TR></TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
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
		top.window.location.href="/cgi-bin/selfcheck.asp";
}

function restore()
{
		top.window.location.href="/cgi-bin/reset.asp";
}
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
  <TR>
  	<TD>&nbsp;&nbsp;&nbsp;&nbsp;</TD>
    <TD><DIV id="loading"><strong id="loadcss"></strong></DIV>&nbsp;</TD></TR>
  <TR>
    <TD></TD>
    <TD>
      <DIV id=tdResult style="FONT-SIZE: 13px"></DIV></TD></TR>
  <TR id=trReturn1 style="DISPLAY: none">

    <TD align=middle colSpan=2><INPUT id=btnReturn1 style="WIDTH: 60px;HEIGHT: 20px" onclick=onReturn(); type=button value=����> 
    </TD></TR>
  <TR id=trTime style="DISPLAY: none">
    <TD>&nbsp;</TD>
    <TD id=tdTime 
style="FONT-SIZE: 13px">

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

	var rstatus = "<%tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
	var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";	
	var rnewStatus = "<%tcWebApi_get("deviceAccount_Entry","newStatus","s") %>";
	var rnewResult = "<%tcWebApi_get("deviceAccount_Entry","newResult","s") %>";

	var acsresult = "<%TCWebApi_get("Cwmp_Entry","Inform_Status","s")%>";
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
	var serviceName = "<%tcWebApi_get("deviceAccount_Entry","recProvisioningCode","s") %>";
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
			return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬�򲦴�10000��";
<%else%>
			return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬���߼�ID�������Ƿ���ȷ�򲦴�10000��";		
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
<%else%>
				return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬���߼�ID�������Ƿ���ȷ�򲦴�10000��";
<% end if%>	
			}
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
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					return "��ͥ����ע��ITMS��";	
<%else%>
					return "�ѻ�ù���IP����������ITMS...";
<% end if%>
				}else
				{
					statusFlag = 11;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					return "�ն˵�ITMS��ͨ����ͨ������֧�ž�����";
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
					var tmp = "�����PASSWORD�����룩����ȷ�����ʵ������ע��";
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
					return "��ITMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10000��";
				}
			}
			else if (rstatus == "2")
			{
				statusFlag = 2;
				if( lefttime >=0 )
				{					
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					var tmp = "�����PASSWORD�����룩����ȷ�����ʵ������ע��";
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
					return "��ITMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10000��";
				}
			}
			else if (rstatus == "3")
			{
				statusFlag = 3;
				if( lefttime >=0 )
				{									
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
					var tmp = "�����PASSWORD�����룩����ȷ�����ʵ������ע��";
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
					return "��ITMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10000��";
				}			
			}
			else if (rstatus == "4")
			{
				statusFlag = 4;
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
				return "ITMS�·�ҵ��ʱ��������ע�����֧�ž�����";
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
					if((serviceNum == "0") || (serviceList.length == 0)){
						infoStr = "ITMSƽ̨ҵ�������·��ɹ���";
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
						infoStr = "ע��ɹ����·�ҵ��ɹ�����ر�ҳ�档";
<% end if%>
					}
					else{
						infoStr = "ITMSƽ̨ҵ�������·��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ��";
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
						infoStr = "ע��ɹ����·�ҵ��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ����ر�ҳ�档";
<% end if%>
					}
<% end if%>
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
					return "ҵ���·�ʧ�ܣ�����ϵ10000�š�";	
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
<%else%>
					return "ע��ITMS�ɹ����ȴ�ITMSƽ̨�·�ҵ�����ݡ�";			
<% end if%>
<% end if%>
				}
				else if(rresult == "0")
				{
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
						return "ITMSƽ̨���ڽ��������·��������µ�򲦲����";
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
						return "ITMSƽ̨�����·�" + serviceNameStr.toString() + "ҵ�����ݣ�����ϵ��ι��ˡ�";		
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
							setDisplay("loading",0);
							setDisplay("trReturn", 1);
							if(statusFlag < 4 && isHasRetryTimesSubmit != 1){
								document.ConfigForm.tryTimes.value = 1 + iretrycount;
								document.ConfigForm.tryTimesflg.value = "1";
								document.cookie = "HasRetryTimesSubmit=1";
								document.cookie = "PageUpdate=1";
								document.ConfigForm.submit();
							}
							tdResultElement.innerHTML = "<P><strong><font color='#FF0000' size='2'>" + statusinfo + "</font></strong></P>";
						}
					}
					else{
						if(timeoutcnt <  uplimitcount)
						{
							document.cookie = "RegPageStatus=1";
							document.cookie = "RegPagetimeoutcount=" + (timeoutcnt+1).toString();
							//document.ConfigForm.submit();		
							showRegStatus();
						}
						else
						{
							registerend = true;
							setDisplay("trReturn", 1);
							setDisplay("loading",0);
							tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">��ITMS��ͨ����ͨ������ϵ�ͻ�����򲦴�10000</font></strong></P>';
						}
					}
				}
			}
	}
	
	var isStateGetOk = 0;
	function checkSyncPageFinish()
	{
		checkSyncPage();
	}

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
			rLinkState = ifameObj.rLinkState;
			
			rstatus = ifameObj.rstatus;
			rresult = ifameObj.rresult;
			rnewStatus = ifameObj.rnewStatus;
			rnewResult = ifameObj.rnewResult;
			
			acsresult = ifameObj.acsresult;

			serviceName = ifameObj.serviceName;
			serviceNum = ifameObj.serviceNum;
			
			pppup = ifameObj.pppup;
			isStateGetOk = 1;
		}
		
		showRegStatus();
	}

	</SCRIPT>

</BODY></HTML>
