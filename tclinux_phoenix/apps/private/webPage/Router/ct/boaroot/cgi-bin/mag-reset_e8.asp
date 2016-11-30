<%
if Request_Form("rebootflag") = "1" then
	TCWebApi_set("System_Entry","reboot_type","restoreFlag") 
	TCWebApi_set("Account_Entry0","Logged","login_init")
	TCWebApi_commit("System_Entry")
end if
if Request_Form("defaultflag") = "1" then
	TCWebApi_set("System_Entry","reboot_type","restoreflag2") 
	TCWebApi_set("Account_Entry0","Logged","login_init")
	TCWebApi_commit("System_Entry")
end if
	
%>
<%
	if request_Form("postflag") = "1" then
		tcWebApi_set("System_Entry","upgrade_fw","HTML_HEADER_TYPE")
		tcWebApi_Commit("System_Entry")
	end if	
%>		

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<SCRIPT language=JavaScript type=text/javascript>
<% if Request_Form("rebootflag") = "1" then %>
<% if Request_Form("isCUCSupport") = "1" then %>
	setTimeout("doRefresh()",80*1000);
<% else %>
	setTimeout("doRefresh()",35*1000);
<% end if %>
	document.writeln("设备重启中，请等待……");
<% end if %>
<% if Request_Form("defaultflag") = "1" then %>
<% if Request_Form("isCUCSupport") = "1" then %>
	setTimeout("doRefresh()",80*1000);
<% else %>
	setTimeout("doRefresh()",35*1000);
<% end if %>
	document.writeln("设备重启中，以便恢复出厂设置，请等待……");
<% end if %>
function doRefresh()
{
	//alert("重启完成！"); 
 	window.top.location.href = "../";    
}
</SCRIPT>
<%if tcWebApi_get("System_Entry","reboot_type","h") = "0" then%>
<HEAD><TITLE>设备管理</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>

<SCRIPT language=JavaScript type=text/javascript>

function usbRestore()
{
	//var string3 = document.ConfigForm.tools_FW_UploadFile.value.search(/romfile/);
	var string1 = document.USBForm.restorepath.value.search(/tclinux/);
	if(document.USBForm.usbRestoreFlag.checked == true)
	{
		if(document.USBForm.restorepath.value=="")
		{
			alert("恢复文件不存在！");
		}
		else if(string1 >= 0)
		{
			//document.uiPostUpdateForm.uiStatus.value = "Upgrade in progress, Please wait...";
			document.USBForm.postflag.value = "1";
			document.USBForm.submit();
		}
		else
		{
			alert("恢复文件错误！");
		}
	}
	else
	{
		document.USBForm.postflag.value = "0";
	}
}

function usbConfResult()
{
	if (document.USBForm.storefailtype.value == "1")
	{
		var str = '配置文件备份失败，请检查USB设备连接是否正常!';
		alert(str);
	}
	else if(document.USBForm.storefailtype.value == "2")
	{
		var str = '配置文件保存成功!';
		alert(str);
	}
	else if(document.USBForm.storefailtype.value == "3")
	{
		var bchoise = 1;
		//var url= 'cfgUSBRestore.cgi'+'?coverusbpath='+lastPath;
		bchoise=confirm("已有同名文件存在，单击确定删除后备份,取消退出备份");
		if(!bchoise)
		{
			return;
		}
		else
		{
			location.replace("");
		}
	}
	else if(document.USBForm.storefailtype.value == "4")
	{
		var str = 'USB设备空间不足!';
		alert(str);
	}
}
function SetUserType()
{
	var curUserType = top.curUser;
	//current user check 
	if (curUserType == '0') 
	{
		getElement('usbconf').style.display = "none";
		getElement('clearlog').style.display = "none";
	}
	else if (curUserType == '1') 
	{
		getElement('usbconf').style.display = "none";
		getElement('clearlog').style.display = "none";
		getElement('restoredefault0').style.display = "none";
		getElement('restoredefault1').style.display = "none";
		getElement('restoredefault2').style.display = "none";
	}
}
function BackupEn()
{
	if (document.USBForm.bakpath.value == "")
	{
		document.USBForm.usbConfBtn.disabled = true;
	}
	if(document.USBForm.autorestoreflag.value =="1")
	{
		document.USBForm.usbRestoreFlag.checked = true;
	}
}
</SCRIPT>
      <SCRIPT language=JavaScript type=text/javascript>
function LoadFrame()
{
	document.ResetForm.rebootflag.value = "0";
	usbConfResult();
}

function backupToUSB()
{
	var cfg = document.USBForm.bakpath.value;
	var code = 'location.assign("' + cfg + '")';
	eval(code);
}

function doRefresh()
{
	var mydate = new Date();
	mydate.setTime(mydate.getTime() - 1);
	document.cookie = "SESSIONID=; expires=" + mydate.toGMTString(); 
 	window.top.location.href = "../";    
}

function ReDefaultboot()
{
	if(confirm("您确认要恢复出厂设置？"))
	{
		document.DefaultForm.defaultflag.value = "1";
		document.DefaultForm.restoreflag2.value = "2";
		document.DefaultForm.submit();
		getElById('buttondefaultbak').disabled = true;
		setTimeout("doRefresh()",30*1000);
	}
	//十秒钟
}

function Reboot()
{
	if(confirm("确认重启？"))
	{
		document.ResetForm.rebootflag.value = "1";
		document.ResetForm.restoreFlag.value = "1";
		document.ResetForm.submit();
		getElById('btnReboot').disabled = true;
		setTimeout("doRefresh()",30*1000);
	}
}

</SCRIPT>
<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('USBForm') != null)LoadFrame()" 
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
            value="管理->设备管理" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
			document.write(top.gateWayName);
			</SCRIPT>
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
          <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>设备重启</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
              <TD width=474>&nbsp; </TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=15><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#设备管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0> </A></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
		  <FORM id="ResetForm" name="ResetForm" action="/cgi-bin/mag-reset.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD>&nbsp;</TD>
                      <TD width=10>&nbsp;</TD></TR></TBODY></TABLE>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=10>&nbsp;</TD>
                      <TD align=middle><INPUT type="button" value="    设备重启    "  name="btnReboot" onclick="Reboot();" >
                                  <input type="hidden" name="rebootflag" value="0"> 
                                  <input type="HIDDEN" name="restoreFlag" value="0">
				<INPUT type="hidden" NAME="isCUCSupport" value="<%if tcwebApi_get("Info_WLan","isCUCSupport","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
				  
                                </TD>
                      <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <DIV id=usbconf>
        <TR id=usbconf0 style="display:none">
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>USB备份配置</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR id=usbconf1 style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR id=usbconf2 style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
		  <FORM id="USBForm" name="USBForm" action="/cgi-bin/mag-reset.asp" method="post">	
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=10>&nbsp;</TD>
					  <INPUT TYPE="HIDDEN" NAME="devicename" VALUE="<%TCWebApi_get("Usb_Entry0","devicename","s")%>">
					  <INPUT TYPE="HIDDEN" NAME="storefailtype" VALUE="<%TCWebApi_get("Usb_Entry0","storefailtype","s")%>">
					  <INPUT TYPE="HIDDEN" NAME="restorepath" VALUE="<%TCWebApi_get("Usb_Entry0","restorepath","s")%>">
					  <INPUT TYPE="HIDDEN" NAME="restorename" VALUE="<%TCWebApi_get("Usb_Entry0","restorename","s")%>">
					  <INPUT TYPE="HIDDEN" NAME="bakpath" VALUE="<%TCWebApi_get("Usb_Entry0","bakpath","s")%>">
					  <INPUT TYPE="HIDDEN" NAME="bakname" VALUE="<%TCWebApi_get("Usb_Entry0","bakname","s")%>">
					  <INPUT TYPE="HIDDEN" NAME="autorestoreflag" VALUE="<%TCWebApi_get("Usb_Entry0","autorestoreflag","s")%>">
                      <TD align=middle>请选择USB设备分区: <SELECT id=ClDevType 
                        name=ClDevType> 
                        <SCRIPT language=JavaScript type=text/javascript>
							if(document.USBForm.devicename.value != "")
							{
								document.write('<option value="' + document.USBForm.devicename.value + '" selected>' + document.USBForm.devicename.value);
							}
							else
							{
								document.write("<option value=\"\">无USB设备</option>");
							}
						</SCRIPT>
                        </SELECT> &nbsp; <INPUT id="usbConfBtn" onclick="backupToUSB()" type="button" value="备份配置" name="usbConfBtn"> 
                        <BR><BR>
                        <CENTER>启用USB自动恢复配置文件: <INPUT id="usbRestoreFlag" 
                        onclick="usbRestore()" type="checkbox" name="usbRestoreFlag"> 
                        </CENTER>
                                <input type="HIDDEN" name="login_init" value="0">
                                <input type="HIDDEN" name="postflag" value="1"> 
                                <input type="HIDDEN" name="HTML_HEADER_TYPE" value="2"> 
                                <input type="HIDDEN" name="usertype0" value="<%TCWebApi_get("Account_Entry0","usertype0","s")%>"> 
                                <input type="HIDDEN" name="usertype1" value="<%TCWebApi_get("Account_Entry1","usertype1","s")%>"> 
                                <input type="HIDDEN" name="usertype2" value="<%TCWebApi_get("Account_Entry2","usertype2","s")%>"> 
                                <input type="HIDDEN" name="name0" value="<%TCWebApi_get("Account_Entry0","username","s")%>"> 
                                <input type="HIDDEN" name="name1" value="<%TCWebApi_get("Account_Entry1","username","s")%>"> 
                                <input type="HIDDEN" name="name2" value="<%TCWebApi_get("Account_Entry2","username","s")%>">
                                <SCRIPT language=javascript>
						{
							BackupEn();
						}
						</SCRIPT>
                      </TD>
                      <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR></DIV>
        <DIV></DIV>
        <DIV id=clearlog>
        <TR id=clearlog0  style="display:none">
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>清除日志记录</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR id=clearlog1  style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR id=clearlog2  style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=10>&nbsp;</TD>
                      <TD align=middle><INPUT onclick='if(confirm("您确认要清除所有日志记录？")) {location.replace("clearsyslog.cgi");}' type=button value="    清除日志记录    " name=button> 
                      </TD>
                      <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR></DIV>
				<% if tcWebApi_get("WebCurSet_Entry","CurrentAccess","h") = "0" then %>
        <TR id=restoredefault0 name="restoredefault0">
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>恢复出厂设置</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR id=restoredefault1 name="restoredefault1">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR id=restoredefault2 name="restoredefault2">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
		  <FORM id="DefaultForm" name="DefaultForm" action="/cgi-bin/mag-reset.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=10>&nbsp;</TD>
                              <TD align=middle><input type="button" value="  恢复出厂设置  "  name="buttondefaultbak"  onClick="ReDefaultboot();" >
                                <input type="hidden" name="defaultflag" value="0">
                                <input type="hidden" name="restoreflag2" value="0">
				<INPUT type="hidden" NAME="isCUCSupport" value="<%if tcwebApi_get("Info_WLan","isCUCSupport","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
                            </TD>
                      <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
        </TBODY></TABLE></FORM></TD></TR>
		<% end if %>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height="100%"></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD><script language="JavaScript" type="text/JavaScript">
getElement('usbconf').style.display = "none";
getElement('clearlog').style.display = "none";
</script></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height="100%"></TD>
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

            <P align=center><!--IMG id=btnOK onclick="RefreshPage()" height=23 

            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 

            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 

            width=80 border=0--> </P></TD>

          <TD width=170 

bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>

</BODY>
<% end if %>
</HTML>

