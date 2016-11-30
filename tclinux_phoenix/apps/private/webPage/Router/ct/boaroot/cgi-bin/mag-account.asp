<%
if Request_Form("accountflg")="1" then
	if tcWebApi_get("WebCurSet_Entry","CurrentAccess","h") = "1" then
		if Request_Form("oldPassword") = tcWebApi_get("Account_Entry1","web_passwd","h") then
			TCWebApi_set("WebCurSet_Entry","CurPage","1")
			TCWebApi_set("Account_Entry1","web_passwd","cfmPassword")
			TCWebApi_commit("Account_Entry")
			TCWebApi_save()
		else
			TCWebApi_set("WebCurSet_Entry","CurPage","2")
		end if
	else
		TCWebApi_set("WebCurSet_Entry","CurPage","1")
		TCWebApi_set("Account_Entry1","web_passwd","cfmPassword")
		TCWebApi_commit("Account_Entry")
		TCWebApi_save()
	end If
else
	TCWebApi_set("WebCurSet_Entry","CurPage","0")
end If
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>用户管理</TITLE>
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
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="管理->用户管理" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
				document.write(top.ModelName);
			</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24></TD></TR></TBODY></TABLE>
      <SCRIPT language=javascript>
			MakeMenu(getElById('Selected_Menu').value);
	  </SCRIPT>
      <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD height=15><IMG height=15 src="/img/panel1.gif" 
            width=164 border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 
            border=0> </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
<SCRIPT language=JavaScript type=text/javascript>
var signal = "<% tcWebApi_get("WebCurSet_Entry","CurPage","s") %>";

function LoadFrame()
{	
		with (getElById('ConfigForm'))
		{
			accountflg.value = "0";
			if((0 != parseInt(signal)) && (signal != "N/A"))
			{
				var istate = parseInt(signal);
				if(1 == istate)
				{
					if (curUserName == sptUserName){
						var tdResultElement = getElement('stResult');
						tdResultElement.innerHTML = '<P><strong><font color="#0000FF" size="2">密码修改成功！</font></strong></P>';
					}
					else{
						alert("密码修改成功，将跳转到登录页面！");
						window.location.href="/cgi-bin/logout.cgi";
					}
				}
				else{
					window.location.href="/cgi-bin/ErrMsg.asp";
				}
			}
		}
}
	
	function CheckForm()
	{
		if(document.ConfigForm.newPassword.value.length > 31)
		{
			alert("新密码不能大于31个字符 ！");
			return false;
		}
		if (document.ConfigForm.newPassword.value == "")
		{
			alert("新密码不能为空！");
			return false;
		}
		if (document.ConfigForm.cfmPassword.value != document.ConfigForm.newPassword.value)
		{
			alert("确认密码不匹配！");
			return false;
		}
		return true;
	}
	
	function OnOK()
	{
		if (CheckForm())
		{
			document.ConfigForm.accountflg.value = "1";
			document.ConfigForm.submit();
		}
	}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>用户管理</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474></TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#用户管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name=ConfigForm action="/cgi-bin/mag-account.asp" method="post">
			<INPUT TYPE="HIDDEN" NAME="name0" VALUE="<%TCWebApi_get("Account_Entry0","username","s")%>">
			<INPUT TYPE="HIDDEN" NAME="name1" VALUE="<%TCWebApi_get("Account_Entry1","username","s")%>">
			<INPUT TYPE="HIDDEN" NAME="name2" VALUE="<%TCWebApi_get("Account_Entry2","username","s")%>">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR id=secUsername>
                      <TD width=91 height="26">用户名：</TD>
                      <TD width=169>
					  <SELECT id=oldUsername style="WIDTH: 115px"  size=1 name=oldUsername> 
                        	<SCRIPT language=JavaScript type=text/javascript>
							/*if(document.ConfigForm.name0.value != "")
							{
								document.write('<option value="' + document.ConfigForm.name0.value + '" >' + document.ConfigForm.name0.value);
							}*/
							if(document.ConfigForm.name1.value != "")
							{
								document.write('<option value="' + document.ConfigForm.name1.value + '" selected>' + document.ConfigForm.name1.value);
							}
							/*if(document.ConfigForm.name2.value != "")
							{
								document.write('<option value="' + document.ConfigForm.name2.value + '" >' + document.ConfigForm.name2.value);
							}*/
							</SCRIPT>
                        </SELECT></TD></TR>
                    <TR id=trNewUsername name=trNewUsername >
                      <TD width=89>新用户名：</TD>
                      <TD width=169><LABEL><INPUT id=newUsername 
                        style="WIDTH: 115px" maxLength=35 size=15 
                        name=newUsername> </LABEL></TD></TR>
                    <TR id=TroldPassword name=TroldPassword >
                      <TD>旧密码：</TD>
                      <TD><INPUT id=oldPassword type=password maxLength=35 
                        size=15 name=oldPassword></TD></TR>
                    <TR>
                      <TD>新密码：</TD>
                      <TD><INPUT id=newPassword type=password maxLength=35 
                        size=15 name=newPassword>
					<script language="JavaScript" type="text/JavaScript">
					setDisplay("trNewUsername", 0);
					if (curUserName == sptUserName)
					{
						setDisplay("TroldPassword", 0);
					}
					</script></TD></TR>
                    <TR>
                      <TD>确认新密码：</TD>
                      <TD><INPUT id=cfmPassword type=password maxLength=35 
                        size=15 name=cfmPassword></TD></TR></TBODY></TABLE><BR></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD><DIV id=stResult style="FONT-SIZE: 13px"></DIV></TD>
				<INPUT TYPE="HIDDEN" NAME="accountflg" VALUE="0">
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center><IMG id="btnOK" onclick="OnOK()" height="23"
            src="/img/ok.gif" width="80" border="0">&nbsp;&nbsp;<IMG 
            id="btnCancel" onclick="RefreshPage()" height="23" src="/img/cancel.gif" 
            width="80" border="0"> </P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR>
        </TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
