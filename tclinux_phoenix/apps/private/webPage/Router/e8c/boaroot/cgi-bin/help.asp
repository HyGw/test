<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>ʹ�ð���</TITLE>
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
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
<% if tcwebApi_get("WebCustom_Entry","isCTPONTYLOGOSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% end if %> 
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
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=���� color=#ffffff><B><FONT face=���� 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="����->ʹ�ð���" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33>
            <P class=GatewayType align=center>�ͺ�:
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
      <STYLE type=text/css>DD {
	FONT-SIZE: 16px
}
</STYLE>

      <SCRIPT language=JavaScript type=text/javascript>
var ifstatus = 0;
var ifnetwork = 0;
var ifsecurity = 0;
var ifapp = 0;
var ifman = 0;
var ifdiag = 0;
var isCZGDSupport = "<%tcWebApi_get("WebCustom_Entry", "isCTPONCZGDSupported", "s")%>";
function LoadFrame()
{
with (document.ConfigForm)
{
	if (curUserName == sptUserName)
		{
			diag.style.display = "block";
			diag_1.style.display = "none";
			<% if tcwebApi_get("WebCustom_Entry","isCTJOYMESupported","h") = "Yes" then %>
			diag_2.style.display = "none";
			<% end if %>
		}
	else
		{
			diag.style.display = "none";
			diag_1.style.display = "none";
			<% if tcwebApi_get("WebCustom_Entry","isCTJOYMESupported","h") = "Yes" then %>
			diag_2.style.display = "none";
			<% end if %>
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %> 
			if ( vPageMap[2][5] == '1' )
			{
				networkText.style.display = '';
				<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) <> "Yes" then %>
					appText.style.display = 'none';
					getElement("manText").innerHTML="�ġ�����";
				<% end if %>
			}
			else
			{
			networkText.style.display = "none";
			getElement("secuiText").innerHTML="������ȫ";   
			<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) <> "Yes" then %>
				appText.style.display = "none";
				getElement("manText").innerHTML="��������";
			<%else%>
				getElement("appText").innerHTML="����Ӧ��";  
				getElement("manText").innerHTML="�ġ�����";
			<% end if %>
			}
<% end if %>
		}
	}
}
//һ��״̬
function funstatus()
{
with (document.ConfigForm)
	{
		if (ifstatus == 0)
		{
		status_1.style.display = "block";
		status_2.style.display = "block";
		status_3.style.display = "block";
		status_4.style.display = "block";
		status_5.style.display = "block";
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
		status_8.style.display = "block";
		status_9.style.display = "block";		
<% end if %>
		if (curUserName == sptUserName)
		{
		status_6.style.display = "block";
		status_7.style.display = "block";
		}
		ifstatus = 1;
		}
		else
		{
		status_1.style.display = "none";
		status_2.style.display = "none";
		status_3.style.display = "none";
		status_4.style.display = "none";
		status_5.style.display = "none";
		status_6.style.display = "none";
		status_7.style.display = "none";
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
		status_8.style.display = "none";
		status_9.style.display = "none";
<% end if %>
		ifstatus = 0;
		}
	}
}
function funnetwork()
{
with (document.ConfigForm)
	{
		if (ifnetwork == 0)
		{
		if (curUserName == sptUserName)
		{
		network_1.style.display = "block";
		network_2.style.display = "block";
		network_14.style.display = "block";
		network_15.style.display = "block";
		network_3.style.display = "block";
		<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %> 
		network_4.style.display = "block";
		<% end if %>
		}
		<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %> 
		network_5.style.display = "block";
		<% end if %>
		if (curUserName == sysUserName)
		{
			if(isCZGDSupport == "Yes")
				getElement("lanText").innerHTML = "1.LAN���ַ����";
		<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %> 
		if(isCZGDSupport == "Yes") 
			getElement("wlanText").innerHTML = "2.WLAN����";
		else
		getElement("wlanText").innerHTML = "1.WLAN����";
		<% end if %>
		}
		if (curUserName == sptUserName)
		{
		network_6.style.display = "block";
		network_7.style.display = "block";
		network_8.style.display = "block";
		network_9.style.display = "block";
		network_10.style.display = "block";
		network_11.style.display = "block";
		network_12.style.display = "block";
		network_13.style.display = "block";
		<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYJXSupported","h") = "Yes" then %>
		network_16.style.display = "block";
		network_17.style.display = "block";
		<% end if %>
		}
		else
		{
			if ( vPageMap[2][5] == '1' )
			{
				var item_idx = 1;
				if(isCZGDSupport == "Yes")
				{
					network_3.style.display = "block";
					item_idx ++;
				}
				<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %> 
				item_idx ++;
				<% end if %>
				network_6.style.display = '';
				network_7.style.display = '';
				network_7.innerHTML = '<A  href="/cgi-bin/help_content.asp#Զ�̹���" target=_blank>' + item_idx + '.Զ�̹���</A>';
			}
		}
		ifnetwork = 1;
		}
		else
		{
		network_1.style.display = "none";
		network_2.style.display = "none";
		network_14.style.display = "none";
		network_15.style.display = "none";
		network_3.style.display = "none";
		network_4.style.display = "none";
		network_5.style.display = "none";
		network_6.style.display = "none";
		network_7.style.display = "none";
		network_8.style.display = "none";
		network_9.style.display = "none";
		network_10.style.display = "none";
		network_11.style.display = "none";
		network_12.style.display = "none";
		network_13.style.display = "none";
		<% if tcWebApi_get("WebCustom_Entry", "isCTPONCYJXSupported","h") = "Yes" then %>
		network_16.style.display = "none";
		network_17.style.display = "none";
		<% end if %>
		ifnetwork = 0;
		}
	}
}
function funsecui()
{
with (document.ConfigForm)
{
if (ifsecurity == 0)
{
security_1.style.display = "block";
security_2.style.display = "block";
security_3.style.display = "block";
security_4.style.display = "block";
security_5.style.display = "block";
if (curUserName == sptUserName)
{
security_6.style.display = "block";
security_7.style.display = "block";
}
ifsecurity = 1;
}
else
{
security_1.style.display = "none";
security_2.style.display = "none";
security_3.style.display = "none";
security_4.style.display = "none";
security_5.style.display = "none";
security_6.style.display = "none";
security_7.style.display = "none";
ifsecurity = 0;
}
}
}
function funapp()
{
	with (document.ConfigForm)
	{
		if (ifapp == 0)
		{
				app_8.style.display = "block";				
				app_9.style.display = "block";
			if (curUserName == sptUserName)
			{
				app_1.style.display = "block";
				app_2.style.display = "block";
				app_3.style.display = "block";
				app_4.style.display = "block";
				app_5.style.display = "block";
				app_6.style.display = "block";
				app_7.style.display = "block";
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
				app_10.style.display = "block";
				app_11.style.display = "block";
<% end if %>
			}
				if (curUserName == sysUserName)
				{
					getElement("dailyText").innerHTML = "1.�ճ�Ӧ��";
					if(isCZGDSupport == "Yes")
					{
						app_3.style.display = "block";
						getElement("natText").innerHTML = "1.�߼�NAT����";
						getElement("dailyText").innerHTML = "2.�ճ�Ӧ��";
					}
				}
			ifapp = 1;
		}
		else
		{
			app_1.style.display = "none";
			app_2.style.display = "none";
			app_3.style.display = "none";
			app_4.style.display = "none";
			app_5.style.display = "none";
			app_6.style.display = "none";
			app_7.style.display = "none";
			app_8.style.display = "none";
			app_9.style.display = "none";
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
			app_10.style.display = "none";
			app_11.style.display = "none";
<% end if %>
			ifapp = 0;
		}
	}
}
function funman()
{
with (document.ConfigForm)
{
	if (ifman == 0)
	{
		man_1.style.display = "block";
		man_2.style.display = "block";
		man_3.style.display = "block";
		if (curUserName == sptUserName)
		{
		man_4.style.display = "block";
		man_5.style.display = "block";
		}
		if(isCZGDSupport == "Yes")
			man_5.style.display = "block";
		ifman = 1;
	}
	else
	{
		man_1.style.display = "none";
		man_2.style.display = "none";
		man_3.style.display = "none";
		man_4.style.display = "none";
		man_5.style.display = "none";
		ifman = 0;
	}
}
}
function fundiag()
{
	with (document.ConfigForm)
	{
		if (ifdiag == 0)
		{
			diag_1.style.display = "block";
			<% if tcwebApi_get("WebCustom_Entry","isCTJOYMESupported","h") = "Yes" then %>
			diag_2.style.display = "block";
			<% end if %>
			ifdiag = 1;
		}
		else
		{
			diag_1.style.display = "none";
			<% if tcwebApi_get("WebCustom_Entry","isCTJOYMESupported","h") = "Yes" then %>
			diag_2.style.display = "none";
			<% end if %>
			ifdiag = 0;
		}
	}
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>ʹ�ð���</P></TD>
          <TD width=7 bgColor=#ef8218>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=6>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A href="/cgi-bin/help_content.asp#����" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isWebTYLOGOSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE>�� </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif></TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR id=AttackProtect>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P><STRONG>�����ĵ�</STRONG></P></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <FORM name="ConfigForm" action="">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                        <TD> <DL id=info style="FONT-SIZE: 20px" name="info">
                            <DT onclick="funstatus()"><A 
                    href="/cgi-bin/help.asp#">һ��״̬</A> 
                            <DD id=status_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#״̬" 
                    target=_blank>1.�豸��Ϣ</A> 
                            <DD id=status_2 style="DISPLAY: none"> 
                            <DD id=status_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#�������Ϣ" 
                    target=_blank>2.�������Ϣ</A> 
                            <DD id=status_4 style="DISPLAY: none"> 
                            <DD id=status_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#�û�����Ϣ" 
                    target=_blank>3.�û�����Ϣ</A> 
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
                            <DD id=status_6 style="DISPLAY: none"> 
                            <DD id=status_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#Զ�̹���״̬" 
                    target=_blank>4.Զ�̹���״̬</A> 
<% else %>
                            <DD id=status_8 style="DISPLAY: none"> 
                            <DD id=status_9 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#���������Ϣ" 
                    target=_blank>4.���������Ϣ</A> 
                            <DD id=status_6 style="DISPLAY: none"> 
                            <DD id=status_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#Զ�̹���״̬" 
                    target=_blank>5.Զ�̹���״̬</A> 
<% end if %>
                            <DT> 
                            <DT onclick=funnetwork()><A id=networkText 
                    href="/cgi-bin/help.asp#">��������</A> 
                            <DD id=network_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#�������" 
                    target=_blank>1.�������</A> 
                    				<DD id=network_14 style="DISPLAY: none"> 
                    				<DD id=network_15 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#������" 
                    target=_blank>2.������</A> 
                            <DD id=network_2 style="DISPLAY: none"> 
                            <DD id=network_3 style="DISPLAY: none"><A id=lanText
                    href="/cgi-bin/help_content.asp#LAN���ַ����" 
                    target=_blank>3.LAN���ַ����</A> 
                            <DD id=network_4 style="DISPLAY: none"> 
                            <DD id=network_5 style="DISPLAY: none"><A id=wlanText 
                    href="/cgi-bin/help_content.asp#WLAN����" 
                    target=_blank>4.WLAN����</A> 
                    <% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>
                            <DD id=network_6 style="DISPLAY: none"> 
                            <DD id=network_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#Զ�̹���" 
                    target=_blank>5.Զ�̹���</A> 
                            <DD id=network_8 style="DISPLAY: none"> 
                            <DD id=network_9 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#QoS" 
                    target=_blank>6.QoS</A> 
                            <DD id=network_10 style="DISPLAY: none"> 
                            <DD id=network_11 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#ʱ�����" 
                    target=_blank>7.ʱ�����</A> 
                            <DD id=network_12 style="DISPLAY: none"> 
                            <DD id=network_13 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#·������" 
                    target=_blank>8.·������</A> 
                    <% if tcWebApi_get("WebCustom_Entry", "isCTPONCYJXSupported","h") = "Yes" then %>
                            <DD id=network_16 style="DISPLAY: none"> 
                            <DD id=network_17 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#QoS�������" 
                    target=_blank>9.QoS�������</A> 
                  	<% end if %>
                  	<% else %>
                            <DD id=network_6 style="DISPLAY: none"> 
                            <DD id=network_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#Զ�̹���" 
                    target=_blank>4.Զ�̹���</A> 
                            <DD id=network_8 style="DISPLAY: none"> 
                            <DD id=network_9 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#QoS" 
                    target=_blank>5.QoS</A> 
                            <DD id=network_10 style="DISPLAY: none"> 
                            <DD id=network_11 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#ʱ�����" 
                    target=_blank>6.ʱ�����</A> 
                            <DD id=network_12 style="DISPLAY: none"> 
                            <DD id=network_13 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#·������" 
                    target=_blank>7.·������</A>
                    <% if tcWebApi_get("WebCustom_Entry", "isCTPONCYJXSupported","h") = "Yes" then %>
                            <DD id=network_16 style="DISPLAY: none"> 
                            <DD id=network_17 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#QoS�������" 
                    target=_blank>8.QoS�������</A> 
                  	<% end if %>
                  	<% end if %>
                            <DT> 
                            <DT onclick=funsecui()><A id=secuiText 
                    href="/cgi-bin/help.asp#">������ȫ</A> 
                            <DD id=security_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#��������������" 
                    target=_blank>1.��������������</A> 
                            <DD id=security_2 style="DISPLAY: none"> 
                            <DD id=security_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#����ǽ" 
                    target=_blank>2.����ǽ</A> 
                            <DD id=security_4 style="DISPLAY: none"> 
                            <DD id=security_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#MAC����" 
                    target=_blank>3.MAC����</A> 
                            <DD id=security_6 style="DISPLAY: none"> 
                            <DD id=security_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#�˿ڹ���" 
                    target=_blank>4.�˿ڹ���</A> 
                            <DT> 
                            <DT id="app_all" onclick=funapp()><A id=appText 
                    href="/cgi-bin/help.asp#">�ġ�Ӧ��</A>                               
                            <DD id=app_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#DDNS����" 
                    target=_blank>1.DDNS����</A> 
                            <DD id=app_2 style="DISPLAY: none"> 
                            <DD id=app_3 style="DISPLAY: none"><A id=natText
                    href="/cgi-bin/help_content.asp#�߼�NAT����" 
                    target=_blank>2.�߼�NAT����</A> 
                            <DD id=app_4 style="DISPLAY: none"> 
                            <DD id=app_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#UPNP����" 
                    target=_blank>3.UPNP����</A> 
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
                            <DD id=app_6 style="DISPLAY: none"> 
                            <DD id=app_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#IGMP����" 
                    target=_blank>4.IGMP/MLD����</A> 
                            <DD id=app_8 style="DISPLAY: none"> 
                            <DD id=app_9 style="DISPLAY: none"><A id=dailyText 
                    href="/cgi-bin/help_content.asp#�ճ�Ӧ��" 
                    target=_blank>5.�ճ�Ӧ��</A> 
<% else %>
                    				<DD id=app_10 style="DISPLAY: none"> 
                            <DD id=app_11 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#����绰����" 
                    target=_blank>4.����绰����</A> 
                            <DD id=app_6 style="DISPLAY: none"> 
                            <DD id=app_7 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#IGMP����" 
                    target=_blank>5.IGMP/MLD����</A> 
                            <DD id=app_8 style="DISPLAY: none"> 
                            <DD id=app_9 style="DISPLAY: none"><A id=dailyText 
                    href="/cgi-bin/help_content.asp#�ճ�Ӧ��" 
                    target=_blank>6.�ճ�Ӧ��</A> 
<% end if %>
                            <DT> 
                            
                              <script language="JavaScript" type="text/JavaScript">
							  	if(curUserName == sptUserName)
							  		document.writeln('<DT onclick=funman()><A id=manText href="/cgi-bin/help.asp#">�塢����</A>');
								else
									document.writeln('<DT onclick=funman()><A id=manText href="/cgi-bin/help.asp#">�塢����</A>');
							  </script>
                            <DD id=man_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#�û�����" 
                    target=_blank>1.�û�����</A> 
                            <DD id=man_2 style="DISPLAY: none"> 
                            <DD id=man_3 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#�豸����" 
                    target=_blank>2.�豸����</A> 
                            <DD id=man_4 style="DISPLAY: none"> 
                            <DD id=man_5 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#��־�ļ�����" 
                    target=_blank>3.��־�ļ�����</A>                              
                            <DT> 
                            <DT id=diag onclick=fundiag()><A 
                    href="/cgi-bin/help.asp#">�������</A> 
                            <DD id=diag_1 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#�������" 
                    target=_blank>1.�������</A> 
<% if tcwebApi_get("WebCustom_Entry","isCTJOYMESupported","h") = "Yes" then %>
                            <DD id=diag_2 style="DISPLAY: none"><A 
                    href="/cgi-bin/help_content.asp#ҵ�����" 
                    target=_blank>2.ҵ�����</A>
<% end if %>
                    </DD></DL></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>��</TD>
          <TD width=278 bgColor=#427594>��</TD>
          <TD width=196 bgColor=#427594>
            <P align=center><!--A 
            href="/cgi-bin/help.asp#"><IMG id="btnOK" 
            onclick="" height="23" src="/img/ok.gif" width="80" 
            border="0"></A>&nbsp;&nbsp;<A 
            href="/cgi-bin/help.asp#"><IMG id="btnCancel" 
            onclick="" height="23" src="/img/cancel.gif" width="80" 
            border="0"></A--> </P></TD>
          <TD width=170 
bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
