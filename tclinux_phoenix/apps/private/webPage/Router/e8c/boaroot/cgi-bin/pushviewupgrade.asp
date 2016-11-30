<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<HTML xmlns="http://www.w3.org/1999/xhtml">
<HEAD>
<TITLE>网关系统更新</TITLE>
<script LANGUAGE="JavaScript">
var	isIPv4 = "<% tcWebApi_get("WebCurSet_Entry","IsIPv4","s") %>";
var	lanip;
var lanip_v4 = "<% tcWebApi_get("Lan_Entry0","IP","s") %>";
var lanip_v6 = "[" + "<% tcWebApi_get("Lan_Entry0","IP6","s") %>" + "]";
if ( 'N/A' == lanip_v4 )
	lanip_v4 = '192.168.1.1';
if ( 'N/A' == lanip_v6 )
	lanip_v6 = '[fe80::1]';
if ( '0' == isIPv4 && top.window.location.href.indexOf(lanip_v6) < 0 )
	top.window.location.href = 'http://' + lanip_v6 + '/cgi-bin/pushviewupgrade.asp';
else if ( '1' == isIPv4 && top.window.location.href.indexOf(lanip_v4) < 0 )
	top.window.location.href = 'http://' + lanip_v4 + '/cgi-bin/pushviewupgrade.asp';
	
function LoadFrame()
{
}

function SubmitForm( sel )
{
	with ( getElById('ConfigForm') )
	{
		switch ( sel )
		{
			case 1:
				pushviewsel.value = 'upgrade';
			break;
			case 2:
				pushviewsel.value = 'ignore';
			break;
			case 3:
				pushviewsel.value = 'later';
			break;
			default:
			return false;
		}

		pushviewcommit.value = '1';
		submit();
	}
}
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

<META content="Microsoft FrontPage 5.0" name="GENERATOR"></HEAD>
<BODY onload="LoadFrame()">
<DIV id=div_visite>
<% if tcWebApi_get("OSGIUpgrade_Entry", "pushview", "h") = "Yes" then %>
<FORM id="ConfigForm" name="ConfigForm" action="/cgi-bin/pushviewfinish.asp" method="post">		
<TABLE style="POSITION: relative" borderColor="#e7e7e7" height="45%" cellSpacing="0" cellPadding="0" align="center" bgColor="#e7e7e7" border="0">
  <TBODY>
  <TR>
    <TD align="middle" bgColor="#ffffff" colSpan="3" height="10"><LABEL>
			<IMG height="117" src="/img/logo.jpg" width="337">
    </LABEL></TD></TR>
  <TR>
    <TD colspan='3' align='center' style='padding-top:20px'>&nbsp;&nbsp;网关<% if tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "0" then %>固件<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "1" then %>OSGI framework<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "2" then %>JAVA machine<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "5" then %>Mobile manager<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "6" then %>Interface modules<% end if %>存在更新，是否立刻升级?&nbsp;&nbsp;</TD>
    </TR>
  <TR>
    <TD align=right height=30>
		&nbsp;
    </TD>
    <TD></TD>
    <TD></TD>
	</TR>
  <TR>
    <TD>&nbsp; </TD>
    <TD>&nbsp; </TD>
    <TD style="COLOR: #ff0000" align="center" colspan="3">
    </TD></TR>
  <TR>
    <TD align=center colSpan=3>
    	<input type="hidden" name="pushviewcommit" value="0">
    	<input type="hidden" name="pushviewsel" value="unset">
	 <INPUT id="btnSubmit" onclick="SubmitForm(1)" type="button" value="升 级" name="btnSubmit">&nbsp;
	 
	 <INPUT id="btnSubmit" onclick="SubmitForm(2)" type="button" value="不升级" name="btnSubmit">&nbsp;
	 
	 <INPUT id="btnSubmit" onclick="SubmitForm(3)" type="button" value="暂不升级" name="btnSubmit">&nbsp;
	</TD></TR> </TBODY></TABLE></FORM>
<% end if %>
</DIV>
</BODY></html>
