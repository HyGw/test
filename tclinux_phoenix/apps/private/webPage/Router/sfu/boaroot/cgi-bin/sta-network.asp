<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>�������Ϣ</TITLE>
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
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=���� color=#ffffff><B><FONT face=���� 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="״̬->�������Ϣ" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
            <P class=GatewayType align=center>�ͺ�:
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
          <TD height=15><IMG height=15 src="/img/panel1.gif" 
            width=164 border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 
            border=0> </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
      <SCRIPT language=JavaScript src="/img/printtable.js"></SCRIPT>

      <SCRIPT language=JavaScript type=text/javascript>
	 	var userState = 1;
	 	if (curUserName == sptUserName)
			userState = 0;
	 	var para = 0;
		var pvc_counts=<% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
		//get all value
		// num 0
		var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanName = vArrayStr.split(',');
		// num 1
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanValue = vArrayStr.split(',');
		// num 2
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 3
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		Wan_Actives = vArrayStr.split(',');
		// num 4		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var WANEnNAT = vArrayStr.split(',');
		// num 5		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 6		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IPVERSION = vArrayStr.split(',');
		// num 7		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_Status4 = vArrayStr.split(',');
		// num 8		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IP4 = vArrayStr.split(',');
		// num 9		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_ENCAP = vArrayStr.split(',');
		// num 10		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_GateWay4 = vArrayStr.split(',');
		// num 11		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_DNS4 = vArrayStr.split(',');
		// num 12		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_SecDNS4 = vArrayStr.split(',');
		// num 13		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_Status6 = vArrayStr.split(',');
		// num 14		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IP6 = vArrayStr.split(',');
		// num 15		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_GateWay6 = vArrayStr.split(',');
		// num 16		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_DNS6 = vArrayStr.split(',');
		// num 17		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_SecDNS6 = vArrayStr.split(',');
		// num 18		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_PD6 = vArrayStr.split(',');
		// num 19		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_NetMask4 = vArrayStr.split(',');
		// num 20		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 21		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 22		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_Connection = vArrayStr.split(',');
		// num 23		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_VidPRI = vArrayStr.split(',');
		// num 24		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_MAC = vArrayStr.split(',');

		SortUtil.SetReplaceStrflag(1);
		Wan_WanName = SortUtil.UpdateWanIFName( Wan_WanName, pvc_counts );

		Wan_Actives = SortUtil.SortMyArray( Wan_Actives );
		Wan_IPVERSION = SortUtil.SortMyArray( Wan_IPVERSION );
		Wan_Status4 = SortUtil.SortMyArray( Wan_Status4 );
		Wan_IP4 = SortUtil.SortMyArray( Wan_IP4 );
		Wan_ENCAP = SortUtil.SortMyArray( Wan_ENCAP );
		Wan_GateWay4 = SortUtil.SortMyArray( Wan_GateWay4 );
		Wan_DNS4 = SortUtil.SortMyArray( Wan_DNS4 );
		Wan_SecDNS4 = SortUtil.SortMyArray( Wan_SecDNS4 );
		Wan_Status6 = SortUtil.SortMyArray( Wan_Status6 );
		Wan_IP6 = SortUtil.SortMyArray( Wan_IP6 );
		Wan_GateWay6 = SortUtil.SortMyArray( Wan_GateWay6 );
		Wan_DNS6 = SortUtil.SortMyArray( Wan_DNS6 );
		Wan_SecDNS6 = SortUtil.SortMyArray( Wan_SecDNS6 );
		Wan_PD6 = SortUtil.SortMyArray( Wan_PD6 );
		Wan_NetMask4 = SortUtil.SortMyArray( Wan_NetMask4 );
		Wan_Connection = SortUtil.SortMyArray( Wan_Connection );
		Wan_VidPRI = SortUtil.SortMyArray( Wan_VidPRI );
		Wan_MAC = SortUtil.SortMyArray( Wan_MAC );


		function transTemperature(temperature)
		{
			var temp = Number(temperature);
			if (temp >= Math.pow(2, 15)){
				return -Math.round((Math.pow(2, 16)-temp)/256);
			}else{
				return Math.round(temp/256);
			}
		}			
			
		function PonInfoClass()
		{
			this.LinkSta					= '<% tcWebApi_get("XPON_LinkCfg","LinkSta","s") %>';
			this.trafficstate				= '<% tcWebApi_get("XPON_Common","trafficStatus","s") %>';
			this.fecState					= '<% tcWebApi_get("Info_PonPhy","FecStatus","s") %>';
			this.PonState 				= 'down';
			this.FECEnable 				= 'disabled';
			this.WarnInfo 				= 'disconnect';
			this.PonSendPkt 			= '<% tcWebApi_get("Info_PonWanStats","TxFrameCnt","s") %>';
			this.PonRecvPkt 			= '<% tcWebApi_get("Info_PonWanStats","RxFrameCnt","s") %>';
			this.SendPower 				= '-';
			this.RecvPower 				= '-';
			this.WorkVoltage 			= '-';
			this.WorkElectric 		= '-';
			this.WorkTemperature 	= '-';
			
			if ( 'up' == this.trafficstate )
			{
				this.PonState 				= 'up';
				this.WarnInfo 				= 'no info';
			}
				
			if( '0' != this.LinkSta)
			{
				this.SendPower 				= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "TxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
				this.RecvPower 				= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "RxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
				this.WorkVoltage 			= (Number(<%tcWebApi_get("Info_PonPhy", "SupplyVoltage", "s")%>)/10);
				this.WorkElectric 		= (Number(<%tcWebApi_get("Info_PonPhy", "TxBiasCurrent", "s")%>)*2/1000);
				this.WorkTemperature 	= transTemperature(<%tcWebApi_get("Info_PonPhy", "Temperature", "s")%>);
			}
			
			if ( '1' == this.fecState )
				this.FECEnable 				= 'enabled';
		}
		
		var PonInfo = new PonInfoClass();

		function ListCreate_info1(para)
		{
				var index = 1;
				for(var i=0;i<pvc_counts;i++)
				{
					if(Wan_Actives[i] != "N/A")
					{
						if(para == 6)
						{

							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{

								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								if(PonInfo.PonState == "up"){
									if ('up' == Wan_Status6[i])
											document.write('<td>������&nbsp;</td>');
										else if(Wan_Actives[i]=="Yes")											
											document.write('<td>������&nbsp;</td>');
										else
											document.write('<td>δ����&nbsp;</td>');
									}
									else
											document.write('<td>δ����&nbsp;</td>');
									document.write('<td>' + Wan_IP6[i] + '&nbsp;</td>');
									document.write('<td>' + Wan_GateWay6[i] + '&nbsp;</td>');
								document.write('</tr>');
								index = index + 1;
							}
							else 
							{
								if(Wan_IPVERSION[i] == "IPv4")
								{
									index = index + 1;
								}
							}
						}
						else 
						{
							if(para == 4)
							{
								if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
								{
									document.write('<tr align="middle">');
									document.write('<td>' + Wan_WanName[i] + '</td>');
									if(PonInfo.PonState == "up"){
										if ('up' == Wan_Status4[i])
												document.write('<td>������&nbsp;</td>');
										else if(Wan_Actives[i]=="Yes")											
											document.write('<td>������&nbsp;</td>');
										else
											document.write('<td>δ����&nbsp;</td>');
									}
									else
											document.write('<td>δ����&nbsp;</td>');
										document.write('<td>' + Wan_IP4[i] + '&nbsp;</td>');
										document.write('<td>' + Wan_NetMask4[i] + '&nbsp;</td>');
									document.write('</tr>');
									index = index + 1;
								}
								else 
								{
									if(Wan_IPVERSION[i] == "IPv6")
									{
										index = index + 1;
									}
								}
							}
						}
					}
				}
			}
			
		function ListCreate_info2(para)
		{
				var index = 1;
				for(var i=0;i<pvc_counts;i++)
				{
					if(Wan_Actives[i] != "N/A")
					{
						if(para == 6)
						{
							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{
								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								if ( 'Connect_Manually' == Wan_Connection[i] )
									document.write('<td> �ֶ� </td>');
								else
									document.write('<td> �Զ� </td>');
								document.write('<td>' + Wan_ENCAP[i] + '&nbsp;</td>');
								document.write('</tr>');
								index = index + 1;
							}
							else 
							{
								if(Wan_IPVERSION[i] == "IPv4")
								{
									index = index + 1;
								}
							}
						}
						else 
						{
							if(para == 4)
							{
								if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
								{
									document.write('<tr align="middle">');
									document.write('<td>' + Wan_WanName[i] + '</td>');
									if ( 'Connect_Manually' == Wan_Connection[i] )
										document.write('<td> �ֶ� </td>');
									else
										document.write('<td> �Զ� </td>');
									document.write('<td>' + Wan_ENCAP[i] + '&nbsp;</td>');
									document.write('</tr>');
									index = index + 1;
								}
								else 
								{
									if(Wan_IPVERSION[i] == "IPv6")
									{
										index = index + 1;
									}
								}
							}
						}
					}
				}
			}
			
		function ListCreate_info2p(para)
		{
			for( var i=0; i<pvc_counts; i++ )
			{
				if( Wan_Actives[i] != 'N/A' )
				{
					if( para == 6 )
					{
						if( Wan_IPVERSION[i] == 'IPv4')
							continue;
					}
					else if ( para == 4 )
					{
						if( Wan_IPVERSION[i] == 'IPv6')
							continue;	
					}
					else
							return;

					document.write('<tr align="middle">');
					document.write('<td>' + Wan_WanName[i] + '</td>');
					document.write('<td>' + Wan_VidPRI[i] + '</td>');
					document.write('<td>' + Wan_MAC[i] + '&nbsp;</td>');
					document.write('</tr>');
				}
			}
		}
						
		function ListCreate_info3(para)
		{
				var index = 1;
				for(var i=0;i<pvc_counts;i++)
				{
					if(Wan_Actives[i] != "N/A")
					{
						if(para == 6)
						{
							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{
								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								document.write('<td>' + Wan_DNS6[i] + '&nbsp;</td>');
								document.write('<td>' + Wan_SecDNS6[i] + '&nbsp;</td>');
								document.write('<td>' + Wan_PD6[i] + '&nbsp;</td>');

								document.write('</tr>');
								index = index + 1;
							}
							else 
							{
								if(Wan_IPVERSION[i] == "IPv4")
								{
									index = index + 1;
								}
							}
						}
						else 
						{
							if(para == 4)
							{
								if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
								{
									document.write('<tr align="middle">');
									document.write('<td>' + Wan_WanName[i] + '</td>');
									document.write('<td>' + Wan_GateWay4[i] + '&nbsp;</td>');
									document.write('<td>' + Wan_DNS4[i] + '&nbsp;</td>');
									document.write('<td>' + Wan_SecDNS4[i] + '&nbsp;</td>');
									document.write('</tr>');
									index = index + 1;
								}
								else 
								{
									if(Wan_IPVERSION[i] == "IPv6")
									{
										index = index + 1;
									}
								}
							}
						}
					}
				}
			}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 class=Item_L1 height=30>
            <P>PON��·������Ϣ</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=16>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#�������Ϣ" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE></TD></TR>
        <TR style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>��������</TD>
                      <TD class=table_title align=middle>״̬</TD>
                      <TD class=table_title align=middle>IP��ַ</TD>
                      <TD class=table_title align=middle>��������</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info1(4);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=tabOptionv4 cellSpacing=0 cellPadding=3 width=406 
                  border=1 name="tabOptionv4">
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>��������</TD>
                      <TD class=table_title id=secPVC align=middle>����</TD>
                      <TD class=table_title id=secEncap 
align=middle>IP��ȡ��ʽ</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info2(4);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                    	<TD class=table_title align=middle>��������</TD>
                      <TD class=table_title align=middle>VLAN/���ȼ�</TD>
                      <TD class=table_title align=middle>MAC��ַ</TD>
                      </TR>
							        <SCRIPT language=JavaScript type=text/javascript>
											ListCreate_info2p(4);
											</SCRIPT>
                  </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=180>��������</TD>
                      <TD class=table_title align=middle width=208>Ĭ������</TD>
                      <TD class=table_title align=middle width=208>��ѡDNS</TD>
                      <TD class=table_title align=middle width=208>����DNS</TD></TR>
                    <SCRIPT language=javascript>
						ListCreate_info3(4);
					</SCRIPT>
                    </TBODY></TABLE></TD>
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
        <TR style="display:none">
          <TD width=157 class=Item_L1 height=30>
            <P>IPv6������Ϣ</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>��</TD>
          <TD width=474>��</TD></TR>
        <TR style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>��������</TD>
                      <TD class=table_title align=middle>IPv6״̬</TD>
                      <TD class=table_title align=middle>IPv6��ַ</TD>
                      <TD class=table_title align=middle>IPv6Ĭ������</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info1(6);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=tabOptionv6 cellSpacing=0 cellPadding=3 width=406 
                  border=1 name="tabOptionv6">
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>��������</TD>
                      <TD class=table_title id=secPVC align=middle>����</TD>
                      <TD class=table_title id=secEncap 
align=middle>IP��ȡ��ʽ</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info2(6);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                    
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                    	<TD class=table_title align=middle>��������</TD>
                      <TD class=table_title align=middle>VLAN/���ȼ�</TD>
                      <TD class=table_title align=middle>MAC��ַ</TD>
                      </TR>
							        <SCRIPT language=JavaScript type=text/javascript>
											ListCreate_info2p(6);
											</SCRIPT>
                  </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=180>��������</TD>
                      <TD class=table_title align=middle 
width=208>IPv6��ѡDNS</TD>
                      <TD class=table_title align=middle 
width=208>IPv6����DNS</TD>
                      <TD class=table_title align=middle width=208>��ȡǰ׺</TD></TR>
                    <SCRIPT language=javascript>
						ListCreate_info3(6);
					</SCRIPT>
                    </TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR style="display:none">
          <TD width=157 class=Item_L1 height=30>
            <P>PON��·������Ϣ</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>��</TD>
          <TD width=474>��</TD></TR>
        <TR style="display:none">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD vAlign=top>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=secPonInfo1>
                  <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                    <TBODY>
                    <TR>
                    	<TD class='table_title' colspan='2'>������·������Ϣ</TD>
                    </TR>
                    <TR>
                      <TD class=table_title width=160>PON��·����״̬:</TD>
                       <TD > 
                        <script language=JavaScript type=text/javascript>
												if( PonInfo.PonState == "up")
													document.write("����");
												else if ( PonInfo.PonState == "connecting")
													document.write("������");
												else
													document.write("����");
												</script>
                    		&nbsp;</TD>
                    </TR>
                    <TR>
                      <TD class=table_title>FECʹ��:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												if(PonInfo.FECEnable == "disabled")
													document.write("ȥʹ��");
												else
													document.write("ʹ��");
											</SCRIPT>
                        &nbsp;</TD>
                      </TR>
                    <TR>
                      <TD class=table_title>�澯��Ϣ:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
                        if(PonInfo.WarnInfo == "disconnect")
                                document.write("�����ж�");
			else
				document.write("�޸澯");
			</SCRIPT>
                        &nbsp;</TD>
                      </TR>
                    <TR>
                    	<TD class='table_title' colspan='2'>��·����ͳ��</TD>
                    </TR>
                    <TR>
                      <TD class=table_title>PON�ڷ�����:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.PonSendPkt);
												</SCRIPT>
                        &nbsp;</TD>
                     </TR>
                    <TR>
                      <TD class=table_title>PON���հ���:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.PonRecvPkt);
												</SCRIPT>
                        &nbsp;</TD>
                     </TR>
	                    <TR>
	                    	<TD class='table_title' colspan='2'>��ģ����Ϣ</TD>
	                    </TR>
                     <TR>
                      <TD class=table_title>���͹⹦��:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.SendPower);
												</SCRIPT>dBm
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>���չ⹦��:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.RecvPower);
												</SCRIPT>dBm
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>������ѹ:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.WorkVoltage);
												</SCRIPT>mV
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>��������:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.WorkElectric);
												</SCRIPT>mA
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>�����¶�:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.WorkTemperature);
												</SCRIPT>��
                        &nbsp;</TD>
                     </TR>
                      </TBODY>
                     </TABLE>
                  </DIV>
                  </TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>&nbsp;</TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR></TBODY></TABLE>
    </TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>��</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>��</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2>
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031 class=CucBgColorTD_2>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
