<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>网络侧信息</TITLE>
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
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
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
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="状态->网络侧信息" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#d8d8d8 height=33>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#d8d8d8 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#0059A0 colSpan=2 
      height=24></TD>
</TR></TBODY></TABLE>
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
<% if tcwebApi_get("WebCustom_Entry","isCTSFUC9Supported","h") = "Yes" then %>		
		pvc_counts = 1;
<% end if %>
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
			this.trafficstate			= '<% tcWebApi_get("XPON_Common","trafficStatus","s") %>';
			this.fecState					= '<% tcWebApi_get("Info_PonPhy","FecStatus","s") %>';
			this.ponType					= '<% tcWebApi_get("XPON_Common","xponMode","s") %>';
			this.loidStatus				= 'init';
			this.PonState 				= 'down';
			this.FECEnable 				= '-';
			this.WarnInfo 				= 'disconnect';
			this.PonSendPkt 			= '<% tcWebApi_get("Info_PonWanStats","TxFrameCnt","s") %>';
			this.PonRecvPkt 			= '<% tcWebApi_get("Info_PonWanStats","RxFrameCnt","s") %>';
			this.SendPower 				= '-';
			this.RecvPower 				= '-';
			this.WorkVoltage 			= '-';
			this.WorkElectric 		= '-';
			this.WorkTemperature 	= '-';
			this.EncryptEnable 		= '-';
			this.encryptState			= '<% tcWebApi_get("WebCurSet_Entry","EPONEncryption","s") %>';
			this.GemPortInfo			= '<% tcWebApi_get("GPON_GEMPort","EncryptionStateALL","s") %>';
			this.DispGemPortState = DispGemPortState;
			this.phyStatus				=	'<% tcWebApi_get("XPON_Common","phyStatus","s") %>';

			if ( 'GPON' != this.ponType && 'EPON' != this.ponType )
				this.ponType = 'GPON';
			if ( 'GPON' == this.ponType )
			{
				this.loidAuth					= '<% tcWebApi_get("GPON_LOIDAuth","AuthStatus","s") %>';
				this.loidV1						= '<% tcWebApi_get("GPON_LOIDAuth","LOID","s") %>';
			}
			else
			{
				this.loidAuth					= '<% tcWebApi_get("EPON_LOIDAuth","AuthStatus","s") %>';
				this.loidV1						= '<% tcWebApi_get("EPON_LOIDAuth","LOID0","s") %>';
			}

			if ( 'up' == this.trafficstate )
			{
				this.PonState 				= 'up';
				this.WarnInfo 				= 'no info';
				this.loidStatus				= 'up';
			}
			else
			{
				if ( '0' != this.LinkSta && '2' == this.loidAuth && 0 != this.loidV1.length )
					this.loidStatus			= 'error';
			}
			
			if( '0' != this.LinkSta)
			{
				this.SendPower 				= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "TxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
				this.RecvPower 				= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "RxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
				this.WorkVoltage 			= (Number(<%tcWebApi_get("Info_PonPhy", "SupplyVoltage", "s")%>)/10);
				this.WorkElectric 		= (Number(<%tcWebApi_get("Info_PonPhy", "TxBiasCurrent", "s")%>)*2/1000);
				this.WorkTemperature 	= transTemperature(<%tcWebApi_get("Info_PonPhy", "Temperature", "s")%>);
				this.FECEnable 				= ( '1' == this.fecState ) ? 'enabled' : 'disabled';

				if ( '2' == this.LinkSta )
					this.EncryptEnable	= ( '1' == this.encryptState ) ? 'enabled' : 'disabled';
			}
			
			function DispGemPortState()
			{
				var gemportList;
				var gemportInfo;
				var gemportState;
				var i = 0;

				if ( this.GemPortInfo.indexOf('#') >=0 )
					this.GemPortInfo = this.GemPortInfo.substring(0, this.GemPortInfo.length - 1);
				else
				{
					document.write('关闭');
					return -1;
				}

				gemportList = this.GemPortInfo.split('#');
				for (i = 0; i < gemportList.length; i ++)
				{
					gemportInfo = gemportList[i].split(':');
					if ( 2 != gemportInfo.length )
						continue;

					gemportState = 'GEMPORT' + gemportInfo[0] + ':' + ( ( 1 == gemportInfo[1] ) ? '开启' : '关闭' )
												+ '; '
					document.write(gemportState);
				}

				return 0;
			}
		}
		
		var PonInfo = new PonInfoClass();

		function ListCreate_info1(para)
		{
				var index = 1;
				for(var i=0;i<pvc_counts;i++)
				{
					if(Wan_Actives[i] != "N/A")
					{
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
					if( Wan_WanName[i].indexOf('TR069') >= 0 )
					{
<% end if
end if
end if%>
						if(para == 6)
						{

							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{

								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								var isIPGWUp = 0;
								if ( 0 != Wan_IP6[i].length && '-' != Wan_IP6[i]
									&& 0 != Wan_GateWay6[i].length && '-' != Wan_GateWay6[i] )
									isIPGWUp = 1;
								if(Wan_WanName[i].indexOf('_B_') >= 0)
									isIPGWUp = 1;								
								if(PonInfo.PonState == "up"){
									if ('up' == Wan_Status6[i] && (1 == isIPGWUp))
											document.write('<td>已连接&nbsp;</td>');
<% if TCWebApi_get("WebCustom_Entry","isPPPoEOnDemandSupported","h" ) = "Yes" then %>
										else if(Wan_Actives[i]=="Yes"
													&& ( Wan_WanName[i].indexOf('_B_') >= 0
															||'PPPoE' != Wan_ENCAP[i]
															|| ('PPPoE' == Wan_ENCAP[i] && ('Connect_Keep_Alive' == Wan_Connection[i] || '-' == Wan_Connection[i]))
															)
														)
<% else %>
										else if(Wan_Actives[i]=="Yes")	
<% end if %>
											document.write('<td>连接中&nbsp;</td>');
										else
											document.write('<td>未连接&nbsp;</td>');
									}
									else
											document.write('<td>未连接&nbsp;</td>');
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
												document.write('<td>已连接&nbsp;</td>');
<% if TCWebApi_get("WebCustom_Entry","isPPPoEOnDemandSupported","h" ) = "Yes" then %>
										else if(Wan_Actives[i]=="Yes"
													&& ( Wan_WanName[i].indexOf('_B_') >= 0
																	||'PPPoE' != Wan_ENCAP[i]
																	|| ('PPPoE' == Wan_ENCAP[i] && ('Connect_Keep_Alive' == Wan_Connection[i] || '-' == Wan_Connection[i]))
																	)
														)
<% else %>
										else if(Wan_Actives[i]=="Yes")	
<% end if %>
											document.write('<td>连接中&nbsp;</td>');
										else
											document.write('<td>未连接&nbsp;</td>');
									}
									else
											document.write('<td>未连接&nbsp;</td>');
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
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
						}
<% end if
end if
end if%>
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
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
					if( Wan_WanName[i].indexOf('TR069') >= 0 )
					{
<% end if
end if
end if%>
						if(para == 6)
						{
							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{
								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								if ( 'Connect_Manually' == Wan_Connection[i] )
									document.write('<td> 手动 </td>');
								else
									document.write('<td> 自动 </td>');
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
										document.write('<td> 手动 </td>');
									else
										document.write('<td> 自动 </td>');
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
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
						}
<% end if
end if
end if%>
					}
				}
			}
			
		function ListCreate_info2p(para)
		{
			for( var i=0; i<pvc_counts; i++ )
			{
				if( Wan_Actives[i] != 'N/A' )
				{
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
				if( Wan_WanName[i].indexOf('TR069') >= 0 )
				{
<% end if
end if
end if%>
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
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
				}
<% end if
end if
end if%>
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
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
					if( Wan_WanName[i].indexOf('TR069') >= 0 )
					{
<% end if
end if
end if%>
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
<% if tcWebApi_get("WebCustom_Entry", "isCTSIMCARDSEPARATIONSupported", "h") = "Yes"  then
   if tcWebApi_get("SimCard_Entry", "cardExist", "h") = "1"  then
   if tcWebApi_get("SimCard_Entry", "simAuthRet", "h") = "0"  then %>
						}
<% end if
end if
end if%>
					}
				}
			}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD class=Item_L1 width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>IPv4连接信息</P></TD>
         <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") ="Yes" then %>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=14>
<% else %>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=16>
<%end if %>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#网络侧信息" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title align=middle>状态</TD>
                      <TD class=table_title align=middle>IP地址</TD>
                      <TD class=table_title align=middle>子网掩码</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info1(4);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=tabOptionv4 cellSpacing=0 cellPadding=3 width=406 
                  border=1 name="tabOptionv4">
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title id=secPVC align=middle>连接</TD>
                      <TD class=table_title id=secEncap 
align=middle>IP获取方式</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info2(4);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                    	<TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title align=middle>VLAN/优先级</TD>
                      <TD class=table_title align=middle>MAC地址</TD>
                      </TR>
							        <SCRIPT language=JavaScript type=text/javascript>
											ListCreate_info2p(4);
											</SCRIPT>
                  </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=180>连接名称</TD>
                      <TD class=table_title align=middle width=208>默认网关</TD>
                      <TD class=table_title align=middle width=208>首选DNS</TD>
                      <TD class=table_title align=middle width=208>备用DNS</TD></TR>
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
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") <>"Yes" then %> 
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>IPv6连接信息</P></TD>
          <TD width=7 bgColor=#e7e7e7>　</TD> 
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title align=middle>IPv6状态</TD>
                      <TD class=table_title align=middle>IPv6地址</TD>
                      <TD class=table_title align=middle>IPv6默认网关</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info1(6);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=tabOptionv6 cellSpacing=0 cellPadding=3 width=406 
                  border=1 name="tabOptionv6">
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title id=secPVC align=middle>连接</TD>
                      <TD class=table_title id=secEncap 
align=middle>IP获取方式</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info2(6);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                    
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                    	<TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title align=middle>VLAN/优先级</TD>
                      <TD class=table_title align=middle>MAC地址</TD>
                      </TR>
							        <SCRIPT language=JavaScript type=text/javascript>
											ListCreate_info2p(6);
											</SCRIPT>
                  </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=180>连接名称</TD>
                      <TD class=table_title align=middle 
width=208>IPv6首选DNS</TD>
                      <TD class=table_title align=middle 
width=208>IPv6备用DNS</TD>
                      <TD class=table_title align=middle width=208>获取前缀</TD></TR>
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
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
<% end if %>
        <TR>     	
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>PON链路连接信息</P></TD>
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=secPonInfo1>
                  <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                    <TBODY>
                    <TR>
                    	<TD class='table_title' colspan='2'>网络链路连接信息</TD>
                    </TR>
                    <TR>
                      <TD class=table_title width=160>PON链路连接状态:</TD>
                       <TD > 
                        <script language=JavaScript type=text/javascript>
												if( PonInfo.PonState == "up")
													document.write("在线");
												else if ( PonInfo.PonState == "connecting")
													document.write("连接中");
												else
													document.write("离线");
												</script>
                    		&nbsp;</TD>
                    </TR>
<% if tcwebApi_get("Account_Entry0","Logged","h") = "1" then %>                    
<% if tcwebApi_get("WebCustom_Entry","isCTPONJSSupported","h") = "Yes" then %>
                    <TR>
                      <TD class=table_title width=160>EPON连接状态:</TD>
                       <TD > 
                        <script language=JavaScript type=text/javascript>
												if( PonInfo.phyStatus.indexOf("up") > -1 && PonInfo.trafficstate == "up")
													document.write("已注册，已认证");
												else if ( PonInfo.phyStatus.indexOf("up") > -1 && PonInfo.trafficstate == "down")
													document.write("已注册，未认证");
												else
													document.write("未注册,未认证");
												</script>
                    		&nbsp;</TD>
                    </TR>
<% end if %>
<% if tcwebApi_get("WebCustom_Entry","isCTPONNMGSupported","h") = "Yes" then %>
                    <TR>
                      <TD class=table_title width=160>
                      	<script language=JavaScript type=text/javascript>
                      	document.write(PonInfo.ponType + '状态:');
                      	</script>
                      </TD>
                       <TD > 
                        <script language=JavaScript type=text/javascript>
												if( PonInfo.loidStatus == 'up')
													document.write("认证成功");
												else if ( PonInfo.loidStatus == 'error')
													document.write("LOID错误或不存在");
												else
													document.write("初始状态");
												</script>
                    		&nbsp;</TD>
                    </TR>
<% end if %>
                    <TR>
                      <TD class=table_title>FEC使能:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
                        if ( '-' == PonInfo.FECEnable )
                        	document.write(PonInfo.FECEnable);
                        else
                        {
													if(PonInfo.FECEnable == "disabled")
														document.write("去使能");
													else
														document.write("使能");
												}
											</SCRIPT>
                        &nbsp;</TD>
                      </TR>
                    <TR>
                      <TD class=table_title>加密模式:</TD>
                      <TD>
                      	<SCRIPT language=JavaScript type=text/javascript>
                      	if ( '1' == PonInfo.LinkSta )
                      	{
                      		PonInfo.DispGemPortState();
                      	}
                      	else
                      	{
	                        if ( '-' == PonInfo.EncryptEnable )
	                        	document.write(PonInfo.EncryptEnable);
	                        else
	                        {
														if(PonInfo.EncryptEnable == "disabled")
															document.write("关闭");
														else
															document.write("开启");
													}
												}
                        </SCRIPT>
                        &nbsp;</TD>
                    </TR>
                    <TR>
                      <TD class=table_title>告警信息:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
                        if(PonInfo.WarnInfo == "disconnect")
                                document.write("连接中断");
			else
				document.write("无告警");
			</SCRIPT>
                        &nbsp;</TD>
                      </TR>
                    <TR>
                    	<TD class='table_title' colspan='2'>链路性能统计</TD>
                    </TR>
                    <TR>
                      <TD class=table_title>PON口发包数:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.PonSendPkt);
												</SCRIPT>
                        &nbsp;</TD>
                     </TR>
                    <TR>
                      <TD class=table_title>PON口收包数:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.PonRecvPkt);
												</SCRIPT>
                        &nbsp;</TD>
                     </TR>
	                    <TR>
	                    	<TD class='table_title' colspan='2'>光模块信息</TD>
	                    </TR>
                     <TR>
                      <TD class=table_title>发送光功率:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.SendPower);
												</SCRIPT>dBm
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>接收光功率:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.RecvPower);
												</SCRIPT>dBm
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>工作电压:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.WorkVoltage);
												</SCRIPT>mV
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>工作电流:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.WorkElectric);
												</SCRIPT>mA
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>工作温度:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
												document.write(PonInfo.WorkTemperature);
												</SCRIPT>℃
                        &nbsp;</TD>
                     </TR>
<% end if %>
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
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE>
    </TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#d8d8d8>　</TD>
          <TD width=278 bgColor=#0059A0>　</TD>
          <TD width=196 bgColor=#0059A0>
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
