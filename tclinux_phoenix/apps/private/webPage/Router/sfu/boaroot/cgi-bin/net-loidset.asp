<%
if Request_Form("SubmitFlag") = "1" then
		TCWebApi_set("EPON_LOIDAuth","LOID0","LoidUsername")
		TCWebApi_set("EPON_LOIDAuth","Password0","LoidPassword")
		TCWebApi_set("GPON_LOIDAuth","LOID","LoidUsername")
		TCWebApi_set("GPON_LOIDAuth","Password","LoidPassword")
		TCWebApi_commit("EPON_LOIDAuth")
		TCWebApi_commit("GPON_LOIDAuth")
		TCWebApi_save()
		TCWebApi_set("System_Entry","reboot_type","restoreFlag") 
		TCWebApi_commit("System_Entry")
elseif Request_Form("btnSaveFlag") = "1" then
		TCWebApi_set("GPON_ONU","SerialNumber","SNumber")
		TCWebApi_commit("GPON")
		TCWebApi_save()
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<script language="JavaScript" type="text/JavaScript">
	function doRefresh()
	{
	 	window.top.location.href = "../";   
	}
<% if Request_Form("SubmitFlag") = "1" then %>
	setTimeout("doRefresh()",30*1000);
	document.writeln("设备重启中，请等待……");
<% end if %>
</script>
<HTML>
<%if tcWebApi_get("System_Entry","reboot_type","h") = "0" then%>
<HEAD><TITLE>LOID配置管理</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()" 
onunload=DoUnload() marginheight="0" marginwidth="0">
<TABLE height="100%" cellSpacing=0 cellPadding=0 width=808 align=center 
border=0>
  <TBODY>
  <TR>
    <TD height=1>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
        <TBODY>
        <TR>
          <TD>&nbsp;</TD>
          <TD vAlign=bottom align=right width=358>
            <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR>
                <TD vAlign=bottom align=right><SPAN class=curUserName>&nbsp; 
                  </SPAN></TD>
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->LOID配置管理" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43 class=CucBgColorTD_1>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24 class=CucBgColorTD_3></TD></TR></TBODY></TABLE>
      <SCRIPT 
      language=javascript>
MakeMenu(getElById ('Selected_Menu').value);
</SCRIPT>

      <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD height=15><IMG height=15 src="/img/panel1.gif" width=164 
            border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 border=0> 
          </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
      <SCRIPT language=JavaScript type=text/javascript>
function LoadFrame()
{
	
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 class=Item_L1 height=30>
            <P>OLT认证</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=4><TABLE cellSpacing=0 cellPadding=20 width="100%" 
              border=0><TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#LOID配置管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
          	<FORM name="OLTForm" action="/cgi-bin/net-loidset.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD colspan=2>
                  <P>您可以通过设置LOID和Password，然后点击“认证”按钮向网管服务器注册并请求业务配置。</P>
                      </TD></TR>
              <TR>
              	<TD>&nbsp;</TD>
                      <TD width=118>LOID:</TD>
                      <TD><INPUT id=LoidUsername style="WIDTH: 150px" 
                        maxLength=256 name=LoidUsername
value="<%
if TCWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" then
 if TCWebApi_get("EPON_LOIDAuth","LOID0","h") <> "N/A" then TCWebApi_get("EPON_LOIDAuth","LOID0","s") end if
else
 if TCWebApi_get("GPON_LOIDAuth","LOID","h") <> "N/A" then TCWebApi_get("GPON_LOIDAuth","LOID","s") end if
end if %>"></TD></TR>
              <TR>
                <TD>&nbsp;</TD>
                      <TD width=118>Password:</TD>
                      <TD>
					  	<span id="inLoidpsw"><INPUT id=LoidPassword style="WIDTH: 150px" type=password maxLength=256 name=LoidPassword value="<%
if TCWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" then
 if TCWebApi_get("EPON_LOIDAuth","Password0","h") <> "N/A" then TCWebApi_get("EPON_LOIDAuth","Password0","s") end if
else
 if TCWebApi_get("GPON_LOIDAuth","Password","h") <> "N/A" then TCWebApi_get("GPON_LOIDAuth","Password","s") end if
end if %>"></span>
						<%if TCWebApi_get("WebCurSet_Entry","IsSupportShowPSW","h" ) = "1" then%>
						<INPUT id="cb_enablshowpsw" onclick="doshowLOIDpswChange()" type=checkbox name="cb_enablshowpsw">显示密码
						<script language="JavaScript" type="text/JavaScript">
						
						function doshowLOIDpswChange() {
							with (getElById('OLTForm')){
								var tempvalue1 = LoidPassword.value;
								if (cb_enablshowpsw.checked) {
									inLoidpsw.innerHTML = "<input id=\"LoidPassword\" style=\"WIDTH: 150px\" type=\"text\" maxLength=256 name=\"LoidPassword\" value=" +document.OLTForm.LoidPassword.value+ ">";
								}
								else {
									inLoidpsw.innerHTML = "<input id=\"LoidPassword\" style=\"WIDTH: 150px\" type=\"password\" maxLength=256 name=\"LoidPassword\" value=" +document.OLTForm.LoidPassword.value+ ">";
								}
								LoidPassword.value = tempvalue1;
							}
						
						}
						
						</script>
						<%end if%>
						</TD></TR>
						<TR>
                <TD>&nbsp;</TD>
                <TD></TD>
                <TD>&nbsp;</TD></TR>
						<TR>
                <TD align=center colspan=3>
				<BUTTON onclick="doSubmit()" name=btnSubmit value="Submit">认证</BUTTON>&nbsp;&nbsp;
				<BUTTON onclick="RefreshPage()" name=btnCancel value="Cancel">取消</BUTTON><input type="hidden" name="SubmitFlag" value="0"></TD></TR>					
				<input type="HIDDEN" name="restoreFlag" value="0">		
						<script language="JavaScript" type="text/JavaScript">
						function doSubmit() {
							var Form = document.OLTForm;
							if(Form.LoidUsername.value == "")
							{
								alert("LOID为空。");
								Form.LoidUsername.focus();
								return;
							}
	
							Form.SubmitFlag.value = "1";
							Form.restoreFlag.value = "1";
							Form.submit();
						}
												
						</script>					
						</TBODY></TABLE>
<% if TCWebApi_get("XPON_Common","xponMode","h") = "GPON" then %>						
						<BR>
						<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
							<TR>
									<TD width=118>&nbsp;GPON SN号:</TD>
									<TD><INPUT name="SNumber" id="SNumber" style="WIDTH: 150px" maxLength=256 value="<%if tcWebAPI_get("GPON_ONU", "SerialNumber", "h") <> "N/A" then tcWebAPI_get("GPON_ONU", "SerialNumber", "s") else asp_Write("") end if %>"></TD>
									<TD>(16个十六进制数，或者12个字符且最后8个必须为十六进制数)</TD>
							</TR>
							<TR>
									<TD align=center colspan=3>
									<BUTTON onclick="save()" name=btnSave value="saveSN">保存</BUTTON>
									<input type="hidden" name="btnSaveFlag" value="0">
									</TD>
							</TR>
							<script language="JavaScript" type="text/JavaScript">
								function save()
								{
									with ( document.OLTForm )
									{
										if(SNumber.value != "")
										{
											if(SNumber.value.length == 12)
											{
												for ( i = 4; i < 12; i++)
												{
													if (!SNumber.value.charAt(i).match(/^[0-9a-fA-F]$/))
													{
														alert("12个字符且最后8个必须为十六进制数");
														return ;
													}
												}
											}
											else if(SNumber.value.length == 16)
											{
												for ( i = 0; i < 16; i++)
												{
													if(!SNumber.value.charAt(i).match(/^[0-9a-fA-F]$/))
													{
														alert("16位必须全部十六进制数");
														return;
													}
												}
											}
											else
											{
												alert("SN号应为12或者16位");
												return;
											}
										}
										else
										{
											alert("SN号为空");
											return;
										}
										btnSaveFlag.value = "1";
										submit();
									}
								}
							</script>
						</TABLE>
<% end if %>
						</FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>　</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2>	</TD>
          <TD width=170 
bgColor=#313031 class=CucBgColorTD_2>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
</BODY>
<%end if%>
</HTML>
