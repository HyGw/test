<%
If Request_Form("Register_CHANGE")="1" then 
	TCWebApi_set("Cwmp_Entry","devregInform","devReginfoFlag")
	TCWebApi_set("deviceAccount_Entry","newStatus","newStatusFlag")
	TCWebApi_set("deviceAccount_Entry","userName","broadbandusername")
	TCWebApi_set("deviceAccount_Entry","userPasswordDEV","customer_ID")
	if tcWebApi_get("deviceAccount_Entry","registerStatus","h") <> "0" then
		TCWebApi_set("deviceAccount_Entry","registerResult","result_startvalue")
	end if
	TCWebApi_commit("deviceAccount_Entry")
	TCWebApi_save()
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
<SCRIPT language=javascript type=text/javascript>var curUser="<% tcWebApi_get("Account_Entry0","curUserName","s") %>";</SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="JavaScript" type="text/JavaScript">
var	statusFlag = 0;
var isHasRetryTimesSubmit = 0;
var	registerend = false;
var	uplimitcount = 120;//count of refresh
var	refreshtimer = 5000;//unit is millisecond
var	isIPv4 = "<% tcWebApi_get("WebCurSet_Entry","IsIPv4","s") %>";
var	lanip;
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
	
	with (getElById('ConfigForm'))
	{
		if(parseInt(lefttime) > 0)
		{
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
				isRegSuccess = true;
				tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">����ע�᳢���Ѿ��������ƣ��������Ӻ����ԣ�</font></strong></P>';
				
		}
	}
}

function refreshPage()
{
	document.cookie = "RegPageStatus=2";
	document.ConfigForm.submit();
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
		if (isRegSuccess != true)
		{
			document.ConfigForm.submit();
		}
		else
		{
			location.replace("http://" + lanip.toString());
		}
	}
}
function CheckForm()
{
	var bandName = getValue("broadbandusername");
	var customer = getValue("customer_ID");
	if (bandName == "")
	{
		alert("�߼�ID����Ϊ��!");
		return false;
	}
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") <> "Yes"  then %>
	if (customer == "")
	{
		alert("����������!");
		return false;
	}
	<% end if%>
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
		setText('broadbandusername', "");
		setText('customer_ID', "");
	}
}

function onOK()
{
	if(CheckForm())
	{
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
	}
}
function pageHeight()
{
	return window.innerHeight != null? window.innerHeight : document.documentElement && document.documentElement.clientHeight ? document.documentElement.clientHeight : document.body != null? document.body.clientHeight : null;
}
</SCRIPT>

<FORM id="ConfigForm" name="ConfigForm" >
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
    <%if TCWebApi_get("XPON_LinkCfg","LinkSta","h") = "1" then%>
    <TD style="FONT-SIZE: 12px" colSpan=2>GPON������ͨ�ն�ҵ��ע����ʾ��</TD></TR>
    <%else%>
    <TD style="FONT-SIZE: 12px" colSpan=2>EPON������ͨ�ն�ҵ��ע����ʾ��</TD></TR>
    <%end if%>
  <TR>
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 12px" colSpan=2>1.����������E���ӿڵĹ��ˣ���鲢ȷ�Ϲ��źŵ��Ѵ���Ϩ��״̬</TD></TR>
  <TR>
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 12px" colSpan=2>2.׼ȷ���롰�߼�ID���͡����롱�������ȷ��������ע��</TD></TR>
  <TR>
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 12px" colSpan=2>3.��ע�ἰҵ���·������У�10 �����ڣ���Ҫ�ϵ硢��Ҫ�ι���</TD></TR>
  <TR>
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 12px" colSpan=2>4.��ע�Ṧ�ܽ��������豸����֤��ҵ���·��������������豸��������ע��</TD></TR>
  </TR>
  <TR id="useraccount">
    <TD width="8%">&nbsp;<P></TD>
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") = "Yes" then%>
              <TD width="38%" align="right" style="FONT-SIZE: 13px">LOID:</TD>
	<%else%>
			  <TD width="38%" align="right" style="FONT-SIZE: 13px">�߼�ID:</TD>
	<%end if%>
    <TD width="60%"><INPUT id="broadbandusername" style="WIDTH: 120px; FONT-FAMILY: '����'" 
      name="broadbandusername" height="20" value="<% If TCWebApi_get("deviceAccount_Entry","userName","h") <> "N/A" then
						TCWebApi_get("deviceAccount_Entry","userName","s")
						end if%>">&nbsp;<FONT color=red>*</FONT></TD></TR>
  <TR id="userpassword">
    <TD width="8%">&nbsp;</TD>
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") = "Yes" then%>
              <TD style="FONT-SIZE: 13px" align="right">PASSWORD:</TD>
	<%else%>
			  <TD style="FONT-SIZE: 13px" align="right">����:</TD>
	<%end if%>
    <TD><INPUT id="customer_ID" style="WIDTH: 120px; FONT-FAMILY: '����'" 
      type="password" name="customer_ID" height="20" value="<% If TCWebApi_get("deviceAccount_Entry","userPasswordDEV","h") <> "N/A" then
						TCWebApi_get("deviceAccount_Entry","userPasswordDEV","s")
						end if%>"> </TD></TR>
  <TR id="msgwait">
    <TD width="8%">&nbsp;</TD>
    <TD style="FONT-SIZE: 13px" align=right></TD>
              <TD>ע����,���Ե�...</TD>
            </TR>
  <TR id="msgbutton">
    <TD width="8%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
        <TR>
        <TR>
          <TD width="45%" align=right ><INPUT id="btnOK" style="WIDTH: 60px;HEIGHT: 20px" onclick="onOK()" type=button value="ȷ��"></TD>
          <TD width="10%">&nbsp;</TD>
          <TD width="45%" align=left ><INPUT id="btnCancel" style="WIDTH: 60px;HEIGHT: 20px" onclick="onReset()" type="button" value="����"></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
<DIV id=promptScreen style="DISPLAY: none">
<SCRIPT language=javascript type=text/javascript>
	if(getPageStatus() == "1")
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
  <TR id=trReturn style="DISPLAY: none">
    <TD align=middle colSpan=2><INPUT id=btnReturn style="WIDTH: 60px;HEIGHT: 20px" onclick=onReturn(0); type=button value=����> 
    </TD></TR>
  <TR id=trReturn1 style="DISPLAY: none">
    <TD align=middle colSpan=2><INPUT id=btnReturn1 style="WIDTH: 60px;HEIGHT: 20px" onclick=onReturn(1); type=button value=����> 
    </TD></TR>
  <TR id=trTime style="DISPLAY: none">
    <TD>&nbsp;</TD>
    <TD id=tdTime 
style="FONT-SIZE: 13px"></TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
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
	function showMsg()
	{
		percent = getCookie("RegProgressPercent");	
		var rPhyStatus = "<%tcWebApi_get("XPON_Common","phyStatus","s") %>";
		var rLinkState = "<%tcWebApi_get("XPON_LinkCfg","LinkSta","s") %>";
		
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
		
		if(percent < 1)
		{
			statusFlag = 0;
			percent++;
			return "ע���У���ȴ�...";		
		}
		
		if(rPhyStatus.indexOf('up') < 0)	//phy no link
		{
			statusFlag = 12;
			return "��OLT��ע��ʧ�ܣ�������źŵ��Ƿ���Ϩ��״̬���߼�ID�������Ƿ���ȷ�򲦴�10010��";		
		}
		else if(rLinkState != "1" && rLinkState != "2")	// link no up
		{
			statusFlag = 0;
			percent = 20; //change to 20%
			return "����ע��OLT...";
		}
		else	//link up
		{			
			var rstatus = "<%tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
			var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";	
			var rnewStatus = "<%tcWebApi_get("deviceAccount_Entry","newStatus","s") %>";
			
			if(rnewStatus != "1")
				rstatus = "99";
				
			if(rstatus == "N/A" || rstatus == "99")
			{
				if(percent < 30)
				{
					statusFlag = 0;
					percent = 30;//change to 30%
					return "���ڻ�ȡ����IP...";		
				}
			
				var acsresult = "<%TCWebApi_get("Cwmp_Entry","Inform_Status","s")%>";
				if(acsresult != "1" && acsresult != "2")
				{
					statusFlag = 0;
					percent = 40; //change to 40%
					return "�ѻ�ù���IP����������RMS...";		
				}else
				{
					statusFlag = 11;
					return "��RMS��ͨ����ͨ������ϵ�ͻ�����򲦴�10010��";				
				}
			}		
			else if (rstatus == "1")
			{
				statusFlag = 1;
				if(iretrycount < (ilimitcount-1))
				{
					var tmp = "��RMS��ע��ʧ�ܣ������߼�ID�������Ƿ���ȷ�򲦴�10010����ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
					return tmp;
				}
				else
				{			
					return "��RMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10010��";
				}
			}
			else if (rstatus == "2")
			{
				statusFlag = 2;
				if(iretrycount < (ilimitcount-1))
				{					
					var tmp = "��RMS��ע��ʧ�ܣ������߼�ID�������Ƿ���ȷ�򲦴�10010����ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
					return tmp;
				}
				else
				{			
					return "��RMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10010��";
				}
			}
			else if (rstatus == "3")
			{
				statusFlag = 3;
				if(iretrycount < (ilimitcount-1))
				{									
					var tmp = "��RMS��ע��ʧ�ܣ������߼�ID�������Ƿ���ȷ�򲦴�10010����ʣ�ೢ�Դ�����" + lefttime.toString() + "��";
					return tmp;
				}
				else
				{
					return "��RMS��ע��ʧ�ܣ�����ϵ�ͻ�����򲦴�10010��";
				}			
			}
			else if (rstatus == "4")
			{
				statusFlag = 4;
				return "ע�ᳬʱ��������·�����ԡ�";
			}
			else if (rstatus == "5")
			{
				statusFlag = 5;
				isRegSuccess = true;
				return "�Ѿ�ע��ɹ���������ע�ᡣ";
			}	
			else if (rstatus == "0")
			{
				if(rresult == "1")
				{
					statusFlag = 7;
					isRegSuccess = true;
					percent = 100;//change to 100%
					var serviceList = "";
					var serviceNum = parseInt("<%tcWebApi_get("deviceAccount_Entry","serviceNum","s") %>");	
					
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
					
					var infoStr = "RMSƽ̨ҵ�������·��ɹ������·���" + serviceList.toString() + " " + serviceNum.toString() + "��ҵ��";
										
					return infoStr;
				}
				else if(rresult == "2")
				{
					statusFlag = 8;
					isRegSuccess = true;
					percent = 100;//change to 100%
					return "RMS�·�ҵ���쳣������ϵ�ͻ�����򲦴�10010��";			
				}
				else if(rresult == "99")
				{
					statusFlag = 9;
					percent = 50;//change to 50%
					return "�ȴ�RMSƽ̨�·�ҵ�����ݡ�";			
				}
				else if(rresult == "0")
				{
					var fetchStart = getCookie("RegServiceFetch");	
					var serviceName = "<%tcWebApi_get("deviceAccount_Entry","serviceName","s") %>";
					var serviceNum = parseInt("<%tcWebApi_get("deviceAccount_Entry","serviceNum","s") %>");	
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
						return "RMSƽ̨�����·�ҵ�����ݣ�����ϵ��ι��ˡ�";			
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
						return "RMSƽ̨�����·�" + serviceNameStr.toString() + "ҵ�����ݣ�����ϵ��ι��ˡ�";		
					}
				}
			}
		}
		return "δ�յ�Զ��ҵ������";			
	}
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
					document.ConfigForm.submit();		
				}
				else
				{
					registerend = true;
					setDisplay("trReturn", 1);
					setDisplay("loading",0);
					tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">ע�ᳬʱ!</font></strong></P>';
				}
			}
		}
	}
	</SCRIPT>

</FORM></BODY></HTML>
