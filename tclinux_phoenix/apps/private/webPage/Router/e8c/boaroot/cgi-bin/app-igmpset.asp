<%
if Request_Form("Snoop_Flag") = "1" then
	TCWebApi_set("Lan_IgmpSnoop","igmpsnoopEnable","SnoopActive")
	TCWebApi_commit("Lan_IgmpSnoop")
	tcWebApi_save()
end if

if Request_Form("Proxy_Flag") = "1" then
	TCWebApi_set("IGMPproxy_Entry","Active","ProxyActive")	
	TCWebApi_set("IGMPproxy_Entry","UpstreamIF","WanIF")
	TCWebApi_commit("IGMPproxy_Entry")
	TCWebApi_set("MLDproxy_Entry","Active","ProxyActive")	
	TCWebApi_set("MLDproxy_Entry","UpstreamIF","WanIF")
	TCWebApi_commit("MLDproxy_Entry")	
	tcWebApi_save()
end if
%>
<HTML><HEAD><TITLE>IGMP/MLD����</TITLE>
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
            value="Ӧ��->IGMP/MLD����" name=Selected_Menu> </FONT></B><SPAN 
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
<SCRIPT language=JavaScript type=text/javascript>
	var vIgmpProxy = new Array(new stIGMPCfg("domain","<% tcWebApi_get("IGMPproxy_Entry","Active","s") %>","<% tcWebApi_get("IGMPproxy_Entry","UpstreamIF","s") %>","IGMPVersion","<% tcWebApi_get("Lan_IgmpSnoop","igmpsnoopEnable","s") %>"),null);
	var igmp = vIgmpProxy[0];
	var igmpif_find = false;
	var nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
	var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryName = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryValue = vArrayStr.split(',');
<% if tcwebApi_get("WebCustom_Entry","isCTPONCZGDSupported","h") = "Yes" then %>
	var igmpif_bridge = false;
<% end if %>   	
	vEntryName = SortUtil.UpdateWanIFName( vEntryName, nEntryNum );

	vEntryValue = SortUtil.SortMyArray( vEntryValue );	
	
	function stIGMPCfg(domain,ProxyEnable,WanIF, IGMPVersion, SnoopingEnable)
	{
		this.domain = domain;
		if(ProxyEnable == "Yes")
			this.ProxyEnable = 1;
		else
			this.ProxyEnable = 0;
		this.WanIF = WanIF;
		this.IGMPVersion = IGMPVersion;
		if(SnoopingEnable == "Yes")
			this.SnoopingEnable = 1;
		else
			this.SnoopingEnable = 0;	
	}
	function LoadFrame()
	{
		with ( document.forms[0])
		{
			setCheck('chkIgmpSnp',igmp.SnoopingEnable);
			if(igmpif_find){
				setCheck('chkIgmpProxy',igmp.ProxyEnable);
				setDisplay('IgmpProxyTab',igmp.ProxyEnable);
				if(igmp.WanIF != 'N/A')
				{
<% if tcwebApi_get("WebCustom_Entry","isCTPONCZGDSupported","h") = "Yes" then %>					
					for (i = 0; i < nEntryNum; i++)
					{				
						if((vEntryValue[i] == igmp.WanIF) && (vEntryName[i].indexOf('_B_')>-1))
						{
							igmpif_bridge = true;			
							break;
						}
					}	
					if(igmpif_bridge)
						setSelect('WanIF',"");
					else
<% end if %> 
					setSelect('WanIF',igmp.WanIF);
					
				}
			}
			else{
				setCheck('chkIgmpProxy',0);
				setDisplay('IgmpProxyTab',0);
			}
		}
	}

	function btnSubmit(iType)
	{
		if(0 == iType)
		{
			if(document.SnoopForm.chkIgmpSnp.checked)
				document.SnoopForm.SnoopActive.value = "Yes";
			else 
				document.SnoopForm.SnoopActive.value = "No";
			document.SnoopForm.Snoop_Flag.value = "1";
			document.SnoopForm.submit();
		}
		else if(1 == iType)
		{	
			if(document.ConfigForm.chkIgmpProxy.checked)
			{
				if(document.ConfigForm.WanIF.value != "")
					document.ConfigForm.ProxyActive.value = "Yes";
				else
				{
					alert("��ѡ��wan����");
					return;
				}
			}
			else 
				document.ConfigForm.ProxyActive.value = "No";
			document.ConfigForm.Proxy_Flag.value = "1";
			document.ConfigForm.submit();
		}
	}
	function ChangeEnabIgmp()
	{
		setDisplay('IgmpProxyTab',getCheckVal('chkIgmpProxy'));
	}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>
            <% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
            	IGMP/MLD SNOOPING
            <% else %>
							IGMP SNOOPING
       			<% end if %>
            </P></TD>
          <TD width=7 bgColor=#ef8218>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=9>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#IGMP����" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isWebTYLOGOSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE> </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P><BR>
		  <FORM name="SnoopForm" action="/cgi-bin/app-igmpset.asp" method="post">                          
			<input type="hidden" name="Snoop_Flag" value="0">			
			<input type="hidden" name="SnoopActive" value="0">				  
                  <TABLE cellSpacing=3 cellPadding=3 border=0>
                    <TBODY>
                    <TR>
                      <TD>
                      <% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
                      	����IGMP/MLD Snooping
                      <% else %>
												����IGMP Snooping
					       			<% end if %>
                      </TD>
                      <TD><INPUT type=checkbox 
                  name=chkIgmpSnp></TD></TR></TBODY></TABLE><BR><BR></FORM>
                  <CENTER><INPUT onclick='btnSubmit(0)' type=button value=" Ӧ�� " name=applySnp> 
                  </CENTER>
                  <P></P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>
            <% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
            	IGMP/MLD proxy
            <% else %>
							IGMP proxy
       			<% end if %>
            </P></TD>
          <TD width=7 bgColor=#ef8218>��</TD>
          <TD width=474>��</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
	<FORM name="ConfigForm" action="/cgi-bin/app-igmpset.asp" method="post">
	<input type="hidden" name="Proxy_Flag" value="0">
	<input type="hidden" name="ProxyActive" value="0">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P>
                  <TABLE cellSpacing=3 cellPadding=3 border=0>
                    <TBODY>
                    <TR>
                      <TD>
                      <% if tcwebApi_get("WebCustom_Entry","isIPv6Supported","h") ="Yes" then %>
                      	����IGMP/MLD Proxy
					            <% else %>
												����IGMP Proxy
					       			<% end if %>
                      </TD>
                      <TD><INPUT id=chkIgmpProxy onclick=ChangeEnabIgmp() 
                        type=checkbox 
                  name=chkIgmpProxy></TD></TR></TBODY></TABLE><BR><BR>
                  <CENTER>
                  <TABLE id=IgmpProxyTab cellSpacing=0 cellPadding=6 width="60%" 
                  border=1 name="IgmpProxyTab">
                    <TBODY>
                    <TR>
			<TD width=120>WAN�ӿڣ� </TD>
	                      <TD><SELECT id=WanIF name=WanIF 
	                          maxlength="15"> 
			<SCRIPT language=JavaScript type=text/javascript>
			document.write('<option value="">' + ' ' + '</option>');
			igmpif_find = true;
			for (i = 0; i < nEntryNum; i++)
			{
				if(( vEntryName[i].indexOf('_INTERNET_')<0 && vEntryName[i].indexOf('_OTHER_')<0 ) || (vEntryName[i].indexOf('_B_')>-1))
					continue;
				document.write('<option value=' + vEntryValue[i] + '>'
				+ vEntryName[i] + '</option>');
				//if(igmp.WanIF == vEntryValue[i])
				//	igmpif_find = true;
			}
			</SCRIPT>
	                </SELECT> </TD>                    
		    </TR>
                    </TBODY></TABLE></FORM></CENTER><BR>
                  <CENTER><INPUT onclick='btnSubmit(1)' type=button value=" Ӧ�� " name=applyProxy> 
                  </CENTER>
                  <P></P></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
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
          <TD width=196 bgColor=#427594><!-- InstanceEndEditable --></TD>
          <TD width=170 
bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
