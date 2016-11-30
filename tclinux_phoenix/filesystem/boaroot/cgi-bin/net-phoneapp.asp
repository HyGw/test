<%
if Request_Form("SaveFlag") = "1" then
	  TCWebApi_set("mobile_Entry","MgtURL","DistributionAddress")
		TCWebApi_set("mobile_Entry","Port","DistributionPort" )
		TCWebApi_set("mobile_Entry","Heartbeat","HeartbeatCycle" )
		TCWebApi_set("mobile_Entry","Ability","LocalAuthority" )
		TCWebApi_set("mobile_Entry","LocatePort","TCPPort" )
	  TCWebApi_commit("mobile_Entry")
	  TCWebApi_save()
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>手机连接管理</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name="GENERATOR"></HEAD>
<BODY style="TEXT-ALIGN: center" vlink="#000000" alink="#000000" link="#000000"
 leftmargin="0" topmargin="0" 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()"
onunload="DoUnload()" marginheight="0" marginwidth="0">
<table height="100%" cellspacing="0" cellpadding="0" width="808" align="center" border="0">
  <tbody>
  <tr>
    <td height="1">
      <table height="117" cellspacing="0" cellpadding="0" width="808" background="/img/framelogo.jpg" border="0">
        <tbody>
        <tr>
          <td>&nbsp;</td>
          <td valign="bottom" align="right" width="358">
            <table id="table8" cellspacing="0" cellpadding="0" border="0">
              <tbody>
              <tr>
                <td valign="bottom" align="right"><span class="curUserName">&nbsp; 
                  </span></td>
                <td class="welcom" valign="bottom" align="middle" width="120">欢迎您！ </td>
                <td valign="bottom" width="50"><a onclick="DoLogout()" href="/cgi-bin/logout.cgi" target="_top"><span class="logout">退出 
      </span></a></td></tr></tbody></table></td></tr></tbody></table>
      <table id="table2" height="100" cellspacing="0" cellpadding="0" width="808" border="0">
        <tbody>
        <tr>
          <td class="LocationDisplay" id="LocationDisplay" align="middle" width="163" bgcolor="#ef8218" rowspan="3">网络</td>
          <td width="434" bgcolor="#427594" height="33">
            <p align="right"><font face="黑体" color="#ffffff"><b><font face="黑体" color="#ffffff" size="6"><input id="Selected_Menu" type="hidden" value="网络->手机连接管理" name="Selected_Menu" /> </font></b><span class="GatewayName">网关名称:
<script language="javascript">
document.write(top.gateWayName);
</script>家庭网关
             </span></font></p></td>
          <td width="211" bgcolor="#ef8218" height="33">
            <p class="GatewayType" align="center">型号:
            <script language="javascript">
				document.write(top.ModelName);
			</script>000A21
             </p></td></tr>
        <tr>
          <td id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43>&nbsp;</td></tr>
        <tr>
          <td id=MenuArea_L2 bgColor=#427594 colSpan=2 
          height=24></td></tr></tbody></table>
      <script language="javascript">
			MakeMenu(getElById('Selected_Menu').value);
	  </script>
      <table id="table3" height="15" cellspacing="0" cellpadding="0" width="808" border="0"><tbody>
        <tr>
          <td height="15"><img height="15" src="/img/panel1.gif" width="164" border="0"> </td>
          <td><img height="15" src="/img/panel2.gif" width="645" border="0"> </td></tr></tbody></table></td></tr>
  <tr>
    <td valign="top">
<script language="JavaScript" type="text/javascript">
function LoadFrame()
{
	document.ConfigForm.SaveFlag.value = "0";
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

function CheckForm()
{
	with (getElById ("ConfigForm"))
	{
		if (!isSafeStringSN(DistributionAddress.value))
		{
			alert("MgtURL必须是英文字母、数字和下面字符的组合: \":, /, -, .\"");
			return false;
		}

		if(isPlusInteger(DistributionPort.value) == false)
		{
			alert("UDP端口非法！");
			return false;
		}

		if(isPlusInteger(HeartbeatCycle.value) == false)
		{
			alert("心跳周期非法！");
			return false;
		}

		if(isPlusInteger(TCPPort.value) == false)
		{
			alert("TCP端口非法！");
			return false;
		}
	}

	return true;
}
	
function OnOK()
{
	if(CheckForm() == false)
		return;
	
	var Form = document.ConfigForm;
	Form.SaveFlag.value = "1";
	Form.submit();
}
</script>

      <table height="100%" cellspacing="0" cellpadding="0" border="0">
        <tbody>
        <tr>
          <td width="157" bgcolor="#ef8218" height="30">
            <p class="Item_L1">手机连接管理</p></td>
          <td width="7" bgcolor="#ef8218">　</td>
          <td width="474"></td>
          <td valign="top" width="170" background="/img/panel4.gif" rowspan="5">
            <table cellspacing="0" cellpadding="20" width="100%" border="0" height="100%">
              <tbody>
              <tr>
                <td valign="top"><a href="/cgi-bin/help_content.asp#手机连接管理" target="_blank"><img height="34" src="/img/help.gif" width="40" border="0"></a></td>
                </tr></tbody></table></td></tr>
        <tr>
          <td valign="top" width="157" bgcolor="#e7e7e7" height="10"></td>
          <td width="7" background="/img/panel3.gif">　</td>
          <td></td></tr>
        <tr>
          <td valign="top" width="157" bgcolor="#e7e7e7" height="30">
            <p class="Item_L2"></p></td>
          <td width="7" background="/img/panel3.gif">　</td>
          <td>
            <form name="ConfigForm" action="/cgi-bin/net-phoneapp.asp" method="post">
            <input type="hidden" name="SaveFlag" value="0">
            <table cellspacing="0" cellpadding="0" width="100%" border="0">
              <tbody>
              <tr>
                <td width="10">&nbsp;</td>
                <td>
                  <table cellspacing="0" cellpadding="0" border="0">
                    <tbody>
                    <tr id="MgtURL">
		                  <td width="90">分发平台</td>
		                  <td width="31">地址:</td>
		                  <td width="163"><input id="DistributionAddress" maxlength="256" size="15" name="DistributionAddress" value="<%if tcWebApi_get("mobile_Entry","MgtURL","h") <> "N/A" then tcWebApi_get("mobile_Entry","MgtURL","s") end if %>"> </td>
		                  <td width="46">端口号:</td>
		                  <td width="62"><input id="DistributionPort" size="8" name="DistributionPort" value="<%if tcWebApi_get("mobile_Entry","Port","h") <> "N/A" then tcWebApi_get("mobile_Entry","Port","s") end if %>"></td></tr></tbody></table>
                  <table cellspacing="0" cellpadding="0" border="0">
                    <tbody>
                    <tr id="Heartbeat">
                      <td width="120">心跳周期：</td>
                      <td><input id="HeartbeatCycle" size="15" name="HeartbeatCycle" value="<%if tcWebApi_get("mobile_Entry","Heartbeat","h") <> "N/A" then tcWebApi_get("mobile_Entry","Heartbeat","s") end if %>"></td></tr>
                    <tr id="LocatePort">
                      <td width="120">本地访问TCP端口：</td>
                      <td><input id="TCPPort" size="15" name="TCPPort" value="<%if tcWebApi_get("mobile_Entry","LocatePort","h") <> "N/A" then tcWebApi_get("mobile_Entry","LocatePort","s") end if %>"></td></tr>
                    <tr id="Ability">
                      <td width="120">能力接口本地权限</td>
                      <td><input id="LocalAuthority" type="radio" value="0" name="LocalAuthority" <%if TCWebApi_get("mobile_Entry","Ability","h") = "0" then asp_Write("checked") elseif TCWebApi_get("mobile_Entry","Ability","h") = "N/A" then asp_Write("checked") end if %>> 开放</td>
                      <td><input id="LocalAuthority" type="radio" value="1" name="LocalAuthority" <%if TCWebApi_get("mobile_Entry","Ability","h") = "1" then asp_Write("checked") end if %>>不开放</td></tr>
                    </tbody></table><br></td>
                <td width="10">&nbsp;</td></tr>
              <tr>
                <td width="10">&nbsp;</td>
                <td><div id="stResult" style="FONT-SIZE: 13px"></div></td>
                <td width="10">&nbsp;</td></tr></tbody></table></form></td></tr>
        <tr>
          <td valign="top" width="157" bgcolor="#e7e7e7" height="10"></td>
          <td width="7" background="/img/panel3.gif">　</td>
          <td></td></tr>
        <tr>
          <td valign="top" width="157" bgcolor="#e7e7e7"></td>
          <td width="7" background="/img/panel3.gif">　</td>
          <td></td></tr></tbody></table></td></tr>
  <tr>
    <td height="1">
      <table id="table7" height="35" cellspacing="0" cellpadding="0" width="808" border="0"><tbody>
        <tr>
          <td width="162" bgcolor="#ef8218">　</td>
          <td width="278" bgcolor="#427594">　</td>
          <td width="196" bgcolor="#427594">
            <p align="center"><img id="btnOK" onclick="OnOK()" height="23" src="/img/ok.gif" width="80" border="0">&nbsp;&nbsp;<img id="btnCancel" onclick="RefreshPage()" height="23" src="/img/cancel.gif" width="80" border="0"> </p></td>
          <td width="170" bgcolor="#313031">　</td></tr>
        </tbody></table></td></tr></tbody></table>
</body></html>