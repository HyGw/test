<script LANGUAGE="JavaScript">
var psw_error = 0;
function page304()
{
	top.window.location.href = '/cgi-bin/reset.asp';
}
<%
if Request_Form("psw_check") = "1" then
		if Request_Form("rest_psw") = "SCDX189" then
			tcWebApi_constSet("System_Entry", "reboot_type", "4")
			tcWebApi_constSet("Account_Entry0", "Logged", "0")
			tcWebApi_constSet("WebCurSet_Entry", "ResetPSWST", "Reset")
			tcWebApi_commit("System_Entry")
		else
			tcWebApi_constSet("WebCurSet_Entry", "ResetPSWST", "Error")
		end if
		asp_write("page304();");
else
	if tcWebApi_get("WebCurSet_Entry", "ResetPSWST", "h" ) = "Error" then
		tcWebApi_constSet("WebCurSet_Entry", "ResetPSWST", "")
		asp_write("psw_error = 1;");
	end if
end if
%>
</script>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>恢复出厂</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script LANGUAGE="JavaScript">
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
	if (key == 13 && document.activeElement.id != 'btnOK' )
	{
		return confirmBtn();
	}
}

function pageLoad()
{
	with (getElById('ConfigForm'))
	{
<% if TCWebApi_get("WebCurSet_Entry", "ResetPSWST", "h" ) = "Reset" then %>
		setTimeout("top.window.location.href = '/';", 80*1000);
<% else %>
		document.onkeypress = onHandleKeyDown;
		if ( 1 == psw_error )
			showresult.innerHTML = '密码错误，请重试输入！';
<% end if %>
	}
}

function goBack()
{
	top.window.location.href = '/cgi-bin/register.asp';
}

function confirmBtn()
{
	with (getElById('ConfigForm'))
	{
		if ( 0 == rest_psw.value.length )
		{
			alert('请输入密码！');	
			return false;
		}
		btnCancel.disabled = true;
		btnOK.disabled = true;
		psw_check.value = '1';
		submit();
	}

	return true;	
}
</script>
</head>
<body onload='pageLoad()'>
<FORM id="ConfigForm" name="ConfigForm" action="/cgi-bin/reset.asp" method="post">
<% if tcWebApi_get("WebCurSet_Entry", "ResetPSWST", "h" ) = "Reset" then %>
<font size='4'>设备重启中，以便恢复出厂设置，请等待……</font>
<% else %>
<TABLE  width='50%' borderColor="#e7e7e7"  cellSpacing="0" cellPadding="0" align="center" bgColor="#e7e7e7" border="0">
  <TBODY>
  <TR>
    <TD align=center colSpan=3 width='80%' height=50 style='padding-top:30px;'>
    	<table>
    		<tr >
    			<td align=center >
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					</td>
				</tr>
    		<tr>
    			<td align=center style='width:53%; border-style: solid;border-width: 1px;border-color:black;padding-top:10px;padding-bottom:10px;'>
					&nbsp;请输入密码：
					<INPUT id="rest_psw" style="WIDTH: 53%;"  type="password" name="rest_psw" value="" maxlength='128'>
					&nbsp;
					</td>
				</tr>
    		<tr id='showmsg' >
    			<td align=center style='padding-top:10px;'>
					<font color='red'><label id='showresult'></label></font>
					</td>
				</tr>
			</table>
		<input id='psw_check' name='psw_check' type='hidden' value='0' >
		</TD>
    </TR>
  <TR>
    <TD align=center colSpan=3 height=60>
    <INPUT id="btnCancel" onclick='goBack();' type="button" value="返回" name="btnCancel">&nbsp; 
	 	<INPUT id="btnOK" onclick='confirmBtn();' type="button" value="确定" name="btnOK">&nbsp; 
	</TD></TR> </TBODY>
	</TABLE>
<% end if %>
</FORM>
</body>
</html>
