<%
If Request_Form("Register_CHANGE")="1" then 
	TCWebApi_set("Cwmp_Entry","devregInform","devReginfoFlag")
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
<HTML><HEAD><TITLE>业务注册</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript type=text/javascript>var curUser="<% tcWebApi_get("Account_Entry0","curUserName","s") %>";</SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="JavaScript" type="text/JavaScript">
var	statusFlag = 0;
var	registerend = false;
var	uplimitcount = 48;//count of refresh
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
	document.write('<A href="' + httpip + '" onclick="resetCookie();">返回登录页面</A>');
}
function resetCookie() {
	document.cookie = "RegPageStatus=0";
	document.cookie = "RegPagetimeoutcount=0";
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
marginwidth="0" background="/img/bg.jpg">
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
				tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">您的注册尝试已经超过限制，请三分钟后再试！</font></strong></P>';
				
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
		alert("宽带帐号不能为空!");
		return false;
	}
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") <> "Yes"  then %>
	if (customer == "")
	{
		alert("请输入证件号码!");
		return false;
	}
	<% end if%>
	if (bandName.length > 128)
	{
		alert("宽带帐号长度不能大于128位!");
		return false;
	}
	if (customer.length > 128)
	{
		alert("证件号码长度不能大于128位!");
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
  <input name="result_startvalue" type="hidden" value="99">
  <DIV id=mainScreen style="DISPLAY: none">
<SCRIPT language=javascript type=text/javascript>
	var percent = 0;
var winHeight = pageHeight();
if (winHeight > 494)
{
winHeight = (winHeight - 494)/2;
}
else
{
winHeight = 80;
}
document.write('<table width="808" height="494" align="center" background="../images/bg.jpg" style="margin-top:'+winHeight+'px">');
function updateprogress()
{
	with (getElById('ConfigForm')){
		var tdResultprogress = getElement('loadcss');
		tdResultprogress.style.width = percent.toString() + "%";
		tdResultprogress.innerHTML = percent.toString() + "%";
		if(percent < 100) percent++;
	}
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
    <TD colSpan=3>&nbsp;</TD></TR>
  <TR>
    <TD width="2%">&nbsp;</TD>
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") = "Yes" then %>
              <TD style="FONT-SIZE: 13px" colSpan=2>注：请您依次输入LOID和PASSWORD</TD>
	<% else %>
			  <TD style="FONT-SIZE: 13px" colSpan=2>注：请您依次输入宽带帐号和证件号码</TD>
	<%end if%>
  </TR>
  <TR>
    <TD colSpan=3>&nbsp;</TD></TR>
  <TR id="useraccount">
    <TD width="2%">&nbsp;</TD>
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") = "Yes" then%>
              <TD width="38%" align="right" style="FONT-SIZE: 13px">LOID:</TD>
	<%else%>
			  <TD width="38%" align="right" style="FONT-SIZE: 13px">宽带帐号：</TD>
	<%end if%>
    <TD width="60%"><INPUT id="broadbandusername" style="WIDTH: 120px; FONT-FAMILY: '宋体'" 
      name="broadbandusername" height="20" value="<% If TCWebApi_get("deviceAccount_Entry","userName","h") <> "N/A" then
						TCWebApi_get("deviceAccount_Entry","userName","s")
						end if%>">&nbsp;<FONT color=red>*</FONT></TD></TR>
  <TR id="userpassword">
    <TD width="2%">&nbsp;</TD>
	<% if tcWebApi_get("WebCustom_Entry", "isE8TYSupported", "h") = "Yes" then%>
              <TD style="FONT-SIZE: 13px" align="right">PASSWORD:</TD>
	<%else%>
			  <TD style="FONT-SIZE: 13px" align="right">证件号码：</TD>
	<%end if%>
    <TD><INPUT id="customer_ID" style="WIDTH: 120px; FONT-FAMILY: '宋体'" 
      type="password" name="customer_ID" height="20" value="<% If TCWebApi_get("deviceAccount_Entry","userPasswordDEV","h") <> "N/A" then
						TCWebApi_get("deviceAccount_Entry","userPasswordDEV","s")
						end if%>"> </TD></TR>
  <TR id="msgwait">
    <TD width="2%">&nbsp;</TD>
    <TD style="FONT-SIZE: 13px" align=right></TD>
              <TD>注册中,请稍等...</TD>
            </TR>
  <TR id="msgbutton">
    <TD width="2%">&nbsp;</TD>
    <TD colSpan=2>
      <TABLE height="15%" width="100%" border=0>
        <TBODY>
        <TR>
        <TR>
          <TD width="20%" height=25>&nbsp;</TD>
          <TD width="33%"><INPUT id="btnOK" style="WIDTH: 60px" onclick="onOK()" type=button value="确定"></TD>
          <TD width="47%"><INPUT id="btnCancel" style="WIDTH: 60px" onclick="onReset()" type="button" value="重置"></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR></TABLE></DIV>
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
if (winHeight > 494)
{
winHeight = (winHeight - 494)/2;
}
else
{
winHeight = 80;
}
document.write('<table width="808" height="494" align="center" background="../images/bg.jpg" style="margin-top:'+winHeight+'px">');
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
    <TD colSpan=2><DIV id="loading"><strong id="loadcss"></strong></DIV>&nbsp;</TD></TR>
  <TR>
    <TD></TD>
    <TD>
      <DIV id=tdResult style="FONT-SIZE: 13px"></DIV></TD></TR>
  <TR id=trReturn style="DISPLAY: none">
    <TD align=middle colSpan=2><INPUT id=btnReturn style="WIDTH: 60px" onclick=onReturn(0); type=button value=返回> 
    </TD></TR>
  <TR id=trReturn1 style="DISPLAY: none">
    <TD align=middle colSpan=2><INPUT id=btnReturn1 style="WIDTH: 60px" onclick=onReturn(1); type=button value=返回> 
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
	function showMsg()
	{
		var rstatus = "<%tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
		var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";
		if (retryTimes != "N/A")
		{
			iretrycount = parseInt(retryTimes);
		}
		if (limitTimes != "N/A")
		{
			ilimitcount = parseInt(limitTimes);
		}
		
		var lefttime = ilimitcount - iretrycount - 1;
		
		if(rstatus == "N/A" || rstatus == "99")
		{
			statusFlag = 0;
			return "N/A";
		}		
		else if (rstatus == "1")
		{
			statusFlag = 1;
			if(iretrycount < (ilimitcount-1))
			{
				var tmp= "身份证不存在！请重试（剩余尝试次数：" + lefttime.toString() + "）";
				return tmp;
			}
			else
			{			
				return "身份证不存在！注册失败，请联系10000号";
			}
		}
		else if (rstatus == "2")
		{
			statusFlag = 2;
			if(iretrycount < (ilimitcount-1))
			{					
				var tmp = "宽带账号不存在！请重试（剩余尝试次数：" + lefttime.toString() + "）";
				return tmp;
			}
			else
			{			
				return "宽带账号不存在！注册失败，请联系10000号";
			}
		}
		else if (rstatus == "3")
		{
			statusFlag = 3;
			if(iretrycount < (ilimitcount-1))
			{									
				var tmp= "身份证与宽带账号不匹配！请重试（剩余尝试次数：" + lefttime.toString() + "）";
				return tmp;
			}
			else
			{
				return "身份证与宽带账号不匹配！注册失败，请联系10000号";
			}			
		}
		else if (rstatus == "4")
		{
			statusFlag = 4;
			return "注册超时，请检查线路后重试";
		}
		else if (rstatus == "5")
		{
			statusFlag = 5;
			isRegSuccess = true;
			return "已经注册成功，无需再注册";
		}	
		else if (rstatus == "0")
		{
			if(rresult == "1")
			{
				statusFlag = 7;
				isRegSuccess = true;
				return "注册成功，下发业务成功，网关需要重启，请等待";
			}
			else if(rresult == "2")
			{
				statusFlag = 8;
				isRegSuccess = true;
				return "注册成功，下发业务失败，请联系10000号";			
			}
			else if(rresult == "99")
			{
				statusFlag = 9;
				isRegSuccess = true;
				return "注册成功";			
			}
			else if(rresult == "0")
			{
				statusFlag = 10;
				isRegSuccess = true;
				return "注册成功，正在下发业务，请等待";			
			}
		}
		return "未收到远程业务配置";			
	}
	var timeoutcnt=gettimeoutcnt();
	var statusinfo = showMsg();
	if(getCookie("RegStatusChanged_Flag") != statusFlag){
			document.cookie = "RegStatusHasChanged=1";
	}
	var tdResultElement = getElement('tdResult');
	checkQuitstatus();
	var rs=getPageStatus();
	if(rs == "1")
	{
		if(0 == timeoutcnt){
			document.cookie = "RegStatusChanged_Flag=" + statusFlag.toString();
			document.cookie = "RegStatusHasChanged=0";
		}
		var isReshasChanged = getCookie("RegStatusHasChanged");	
		if(null != intervalfunFlag)
			clearInterval(intervalfunFlag);
		percent = parseInt((timeoutcnt/(1+uplimitcount))*100);
		updateprogress();
		intervalfunFlag = setInterval(updateprogress, ((1+uplimitcount)*refreshtimer)/100);
		setDisplay("loading",1);
		if(((9 == statusFlag) || (10 == statusFlag)) && isReshasChanged){	
			tdResultElement.innerHTML = "<P><strong><font color='#FF0000' size='2'>" + statusinfo + "</font></strong></P>";
		}
		else{
			tdResultElement.innerHTML = '<div align="center"><font style="FONT-SIZE: 13px" align="center">注册中,请稍等...</font></div>';
		}
		setTimeout(refreshPage,refreshtimer);
	}
	if(rs == "2")
	{
		var isReshasChanged = getCookie("RegStatusHasChanged");
		if((0 != statusFlag) && (9 != statusFlag) && (10 != statusFlag) && isReshasChanged){
			with (getElById('ConfigForm'))
			{
				registerend = true;
				if(null != intervalfunFlag)
					clearInterval(intervalfunFlag);
				setDisplay("loading",0);
				setDisplay("trReturn", 1);
				if(statusFlag < 4){
					document.ConfigForm.tryTimes.value = 1 + iretrycount;
					document.ConfigForm.tryTimesflg.value = "1";
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
				if(statusFlag < 4){
					document.ConfigForm.tryTimes.value = 1 + iretrycount;
					document.ConfigForm.tryTimesflg.value = "1";
				}	
				setDisplay("trReturn", 1);
				setDisplay("loading",0);
				if(null != intervalfunFlag){
					clearInterval(intervalfunFlag);
				}
				if(0 == statusFlag){
					tdResultElement.innerHTML = '<P><img src="/img/iconFail.jpg"><strong><font color="#FF0000" size="2">注册超时!</font></strong></P>';
				}
				else
					tdResultElement.innerHTML = "<P><strong><font color='#FF0000' size='2'>" + statusinfo + "</font></strong></P>";			
			}
		}
	}
	</SCRIPT>

</FORM></BODY></HTML>
