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
	if request_Form("backupflg") = "1" then
		tcWebApi_set("usbRestore_Entry", "dev", "ClDevType")
		tcWebApi_set("usbRestore_Entry", "autorestore", "autorestoreflag")
		tcWebApi_set("WebCurSet_Entry", "opt", "opt")
		tcWebApi_Commit("WebCurSet_Entry")
		tcWebApi_Commit("usbRestore_Entry")
		tcWebApi_Save()
	elseif request_Form("backupflg") = "2" then
		tcWebApi_set("WebCurSet_Entry", "opt", "opt")
		tcWebApi_Commit("WebCurSet_Entry")
		tcWebApi_Commit("usbRestore_Entry")
	elseif request_Form("backupflg") = "3" then
		tcWebApi_set("WebCurSet_Entry", "opt", "opt")
		tcWebApi_set("usbRestore_Entry", "autorestore", "autorestoreflag")
		tcWebApi_Commit("WebCurSet_Entry")
		tcWebApi_Commit("usbRestore_Entry")
		tcWebApi_Save()
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
	document.writeln("�豸�����У���ȴ�����");
<% end if %>
<% if Request_Form("defaultflag") = "1" then %>
<% if Request_Form("isCUCSupport") = "1" then %>
	setTimeout("doRefresh()",80*1000);
<% else %>
	setTimeout("doRefresh()",35*1000);
<% end if %>
	document.writeln("�豸�����У��Ա�ָ��������ã���ȴ�����");
<% end if %>
function doRefresh()
{
	//alert("������ɣ�"); 
 	window.top.location.href = "../";    
}
</SCRIPT>
<%if tcWebApi_get("System_Entry","reboot_type","h") = "0" then%>
<HEAD><TITLE>�豸����</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
<% if TCWebApi_get("WebCustom_Entry","isCT2PORTSSupported","h" ) <> "Yes" then %>
<% if TCWebApi_get("WebCustom_Entry","isCT1PORTSupported","h" ) <> "Yes" then %>
<SCRIPT language=JavaScript type=text/javascript src="/cgi-bin/showusb.cgi"></SCRIPT>
<% end if %>
<% end if %>
<SCRIPT language=JavaScript type=text/javascript>

function usbRestore()
{
	if(document.USBForm.usbRestoreFlag.checked == true){
		document.USBForm.autorestoreflag.value = "1";
	}else{
		document.USBForm.autorestoreflag.value = "0";
	}

	document.USBForm.backupflg.value = "3";
	document.USBForm.opt.value = "0";
	document.USBForm.submit();
}

function usbConfResult()
{
	document.USBForm.backupflg.value = "0";
	
	if(document.USBForm.autorestoreflag.value =="1")
	{
		document.USBForm.usbRestoreFlag.checked = true;
	}else
	{
		document.USBForm.usbRestoreFlag.checked = false;
}

	BackupEn();
	
	var str, commit;
	
	commit = false;
	
	if (document.USBForm.rstresult.value == "1")
{
		str = '�����ļ�����ɹ�!';
		alert(str);
		commit = true;
	}
	else if (document.USBForm.rstresult.value == "2")
	{
		str = '�����ļ�����ʧ�ܣ�����USB�豸�����Ƿ�����!';
		alert(str);
		commit = true;
	}
	else if(document.USBForm.rstresult.value == "3")
	{
		str = 'USB�豸�ռ䲻��!';
		alert(str);
		commit = true;
	}
	else if(document.USBForm.rstresult.value == "4")
	{
		var bchoise = 1;
		bchoise=confirm("����ͬ���ļ����ڣ�����ȷ��ɾ���󱸷�,ȡ���˳�����");
		if(!bchoise)
		{
			commit = true;
		}
		else
		{
			document.USBForm.opt.value = "2";
			document.USBForm.backupflg.value = "2";
			document.USBForm.submit();
		}
		}
	else if(document.USBForm.rstresult.value == "7")
	{
		str = '�����ļ�����!';
		alert(str);
		commit = true;
	}
	else if(document.USBForm.rstresult.value == "8")
	{
		str = '�����ļ�����ʧ��!';
		alert(str);
		commit = true;
	}

	if(commit){
		document.USBForm.opt.value = "3";
		document.USBForm.backupflg.value = "2";
		document.USBForm.submit();
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
	if(document.USBForm.ClDevType.value == "N/A")
	{
		document.USBForm.usbConfBtn.disabled = true;
	}
	else if (document.USBForm.ClDevType.value == "��USB�豸")
	{
		document.USBForm.usbConfBtn.disabled = true;
	}
	else
	{
		document.USBForm.usbConfBtn.disabled = false;
	}
}
</SCRIPT>
      <SCRIPT language=JavaScript type=text/javascript>
function LoadFrame()
{
	document.ResetForm.rebootflag.value = "0";
	<% if TCWebApi_get("WebCustom_Entry","isCT2PORTSSupported","h" ) <> "Yes" then %>
	<% if TCWebApi_get("WebCustom_Entry","isCT1PORTSupported","h" ) <> "Yes" then %>
	usbConfResult();
	<% end if %>
	<% end if %>
}

function backupToUSB()
{
	
	document.USBForm.backupflg.value = "1";
	document.USBForm.opt.value = "1";
	document.USBForm.submit();
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
	if(confirm("��ȷ��Ҫ�ָ��������ã�"))
	{
		document.DefaultForm.defaultflag.value = "1";
<% if TCWebApi_get("WebCustom_Entry","isCTGDSupported","h" ) = "Yes" then %>
		document.DefaultForm.restoreflag2.value = "4";
<%else%>
		document.DefaultForm.restoreflag2.value = "2";
<% end if %>
		document.DefaultForm.submit();
		getElById('buttondefaultbak').disabled = true;
		setTimeout("doRefresh()",30*1000);
	}
	//ʮ����
}

function Reboot()
{
	if(confirm("ȷ��������"))
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
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% end if %> 
        <TBODY>
        <TR>
          <TD>&nbsp;</TD>
          <TD vAlign=bottom align=right width=358>
            <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR>
                <TD vAlign=bottom align=right><SPAN class=curUserName>&nbsp; 
                  </SPAN></TD>
                <TD class=welcom vAlign=bottom align=middle width=120>��ӭ���� </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>�˳� 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <tr>
        	<TD width=808 bgColor=#d8d8d8 height=2 colSpan=3></td>
        	</tr>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#d8d8d8 rowSpan=3></TD>
          <TD width=434 bgColor=#0059A0 height=33>
            <P align=right><FONT face=���� color=#ffffff><B><FONT face=���� 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="����->�豸����" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT 
            language=javascript>
			document.write(top.gateWayName);
			</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#d8d8d8 height=33>
            <P class=GatewayType align=center>�ͺ�:
            <SCRIPT language=javascript>
			document.write(top.ModelName);
			</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#d8d8d8 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#0059A0 colSpan=2 
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
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>�豸����</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
              <TD width=474>&nbsp; </TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=15><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#�豸����" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0> </A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
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
                      <TD align=middle><INPUT type="button" value="    �豸����    "  name="btnReboot" onclick="Reboot();" >
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
				<% if tcWebApi_get("WebCurSet_Entry","CurrentAccess","h") = "0" then %>
				<% if TCWebApi_get("WebCustom_Entry","isCT2PORTSSupported","h" ) <> "Yes" then %>
				<% if TCWebApi_get("WebCustom_Entry","isCT1PORTSupported","h" ) <> "Yes" then %>
        <DIV id=usbconf>
        <TR id=usbconf0>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>USB��������</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
          <TD width=474>��</TD></TR>
        <TR id=usbconf1>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR id=usbconf2>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
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
			<INPUT TYPE="HIDDEN" NAME="autorestoreflag" VALUE="<%TCWebApi_get("usbRestore_Entry","autorestore","s")%>">
			<INPUT TYPE="HIDDEN" NAME="backupflg" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="rstresult" VALUE="<%TCWebApi_get("WebCurSet_Entry","rstresult","s")%>">
			<INPUT TYPE="HIDDEN" NAME="opt" VALUE="0">
                      <TD align=middle>��ѡ��USB�豸����: <SELECT id=ClDevType onChange="BackupEn()"
                        name=ClDevType> 
                        <SCRIPT language=JavaScript type=text/javascript>
				var i;
				for(i=0;i < usbvalue.length;i++)
				{
					var t1 =usbvalue[i].split("usb");  
					var t2 =t1[1].split("_");
					document.write("<option value=\""+usbvalue[i]+"\">USB"+t2[0]+"-"+t2[1]+"&nbsp;����</option>");
				}
					if(usbvalue.length == 0)
						document.write("<option value=\"N/A\">��USB�豸</option>");
			</SCRIPT>
                        </SELECT> &nbsp; <INPUT id="usbConfBtn" onclick="backupToUSB()" type="button" value="��������" name="usbConfBtn"> 
                        <BR><BR>
                        <CENTER>����USB�Զ��ָ������ļ�: 
                        	<INPUT id="usbRestoreFlag" onclick="usbRestore()" type="checkbox" name="usbRestoreFlag"> 
                        </CENTER>                       
                                <input type="HIDDEN" name="login_init" value="0">
	 			<INPUT TYPE="HIDDEN" NAME="restoreFlag" VALUE="0">		                                
                                <input type="HIDDEN" name="postflag" value="0"> 
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
        <% end if %>
        <% end if %>
        <DIV></DIV>
        <DIV id=clearlog>
        <TR id=clearlog0  style="display:none">
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>�����־��¼</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
          <TD width=474>��</TD></TR>
        <TR id=clearlog1  style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR id=clearlog2  style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
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
                      <TD align=middle><INPUT onclick='if(confirm("��ȷ��Ҫ���������־��¼��")) {location.replace("clearsyslog.cgi");}' type=button value="    �����־��¼    " name=button> 
                      </TD>
                      <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR></DIV>
		<% end if %>
				<% if tcWebApi_get("WebCurSet_Entry","CurrentAccess","h") = "0" then %>
        <TR id=restoredefault0 name="restoredefault0">
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>�ָ���������</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
          <TD width=474>��</TD></TR>
        <TR id=restoredefault1 name="restoredefault1">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR id=restoredefault2 name="restoredefault2">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
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
                              <TD align=middle><input type="button" value="  �ָ���������  "  name="buttondefaultbak"  onClick="ReDefaultboot();" >
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
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD><script language="JavaScript" type="text/JavaScript">if ( getElement('usbconf') )
	getElement('usbconf').style.display = "none";if ( getElement('clearlog') )
	getElement('clearlog').style.display = "none";
</script></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height="100%"></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#d8d8d8>��</TD>
          <TD width=278 bgColor=#0059A0>��</TD>
          <TD width=196 bgColor=#0059A0>

            <P align=center><!--IMG id=btnOK onclick="RefreshPage()" height=23 

            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 

            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 

            width=80 border=0--> </P></TD>

          <TD width=170 

bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>

</BODY>
<% end if %>
</HTML>

