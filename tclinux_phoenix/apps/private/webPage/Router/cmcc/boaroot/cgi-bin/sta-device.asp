<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>设备信息</TITLE>
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
            value="状态->设备信息" name=Selected_Menu> </FONT></B><SPAN 
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
      <SCRIPT language=JavaScript type=text/javascript>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
		var Nametype = 0;
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
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","ConnectionError","s") %>";
		var Wan_ConnectionError = vArrayStr.split(',');

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
		Wan_ConnectionError = SortUtil.SortMyArray( Wan_ConnectionError );
<% end if%>

<% if tcWebApi_get("WebCustom_Entry", "isCTPONYNSupported", "h") = "Yes"  then %>
<% if tcWebApi_get("WebCustom_Entry", "isCT2PORTSSupported", "h") = "Yes"  then %>
    var portNumber = "2";
<% else %>
    var portNumber = "4";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isWLanSupported", "h") = "Yes"  then %>
    var isWifiSupported = "AP";
<% else %>
    var isWifiSupported = "NoAP";
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCT1PORTSupported", "h") = "Yes"  then %> 
		var deviceType = "E8C 1 "+isWifiSupported;
<% else %>   
    var deviceType = "E8C "+portNumber+"+1 "+isWifiSupported;
<% end if%>
<% end if%>

function stDeviceInfo(domain,ProductClass,SerialNumber,HardwareVersion,SoftwareVersion,ManufacturerOUI)
{
this.domain = domain;
this.ProductClass = ProductClass;
this.ManufacturerOUI = ManufacturerOUI;
this.SerialNumber = SerialNumber;
this.HardwareVersion = HardwareVersion;
this.SoftwareVersion = SoftwareVersion;
}
temp = new Array(new stDeviceInfo("InternetGatewayDevice.DeviceInfo",top.ModelName,"19191900AABB818180","HG530RRA.VER.C","V100R001C01B010","00AABB"),null);
var DeviceInfo = temp[0];
function LoadFrame()
{
}
var BatchNumber = "RWC30P0.010.84725";
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
function PonInfoClass()
{
	this.LinkSta				= '<% tcWebApi_get("XPON_LinkCfg","LinkSta","s") %>';
	this.trafficstate			= '<% tcWebApi_get("XPON_Common","trafficStatus","s") %>';
	this.fecState				= '<% tcWebApi_get("Info_PonPhy","FecStatus","s") %>';
	this.PonState				= 'down';
	this.FECEnable				= 'disabled';
	this.WarnInfo				= 'disconnect';
	this.PonSendPkt				= '<% tcWebApi_get("Info_PonWanStats","TxFrameCnt","s") %>';
	this.PonRecvPkt				= '<% tcWebApi_get("Info_PonWanStats","RxFrameCnt","s") %>';
	this.SendPower				= '-';
	this.RecvPower				= '-';
	this.WorkVoltage			= '-';
	this.WorkElectric			= '-';
	this.WorkTemperature		= '-';
			
	if ( 'up' == this.trafficstate )
	{
		this.PonState 			= 'up';
		this.WarnInfo 			= 'no info';
	}
			
	if( '0' != this.LinkSta)
	{
		this.SendPower			= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "TxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
		this.RecvPower			= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "RxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
	}

	if ( '1' == this.fecState )
	{
		this.FECEnable			= 'enabled';
	}
}
var PonInfo = new PonInfoClass();

function ListCreate_info1()
{
	var ITV_name = '<% tcWebApi_get("WanInfo_Common","ITVInfo","s") %>';

	for(var i=0;i<pvc_counts;i++)
	{
		if(Wan_Actives[i] != "N/A")
		{
			if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
			{
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
				if(Wan_IP4[i] == '-')
				{
						Wan_IP4[i] = '/';
				}
<%end if%>
				document.write('<tr align="middle">');
					
				if(Wan_WanName[i].indexOf('TR069') >= 0)
				{
					document.write('<td>' + 'ITMS管理通道状态' + '</td>');

					if(PonInfo.PonState == "up")
					{
						if('up' == Wan_Status4[i])
						{
							document.write('<td align="center">正常&nbsp;</td>');
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}
					}
					else
					{
						document.write('<td align="center">异常&nbsp;</td>');
					}
					document.write('<td align="center">' + '路由（DHCP自动获取）' + '&nbsp;</td>');
					document.write('<td>' + Wan_IP4[i] + '&nbsp;</td>');
					document.write('<td>' + '/' + '&nbsp;</td>');
					document.write('<td>' + '/' + '&nbsp;</td>');
				}
				else if(Wan_WanName[i].indexOf('INTERNET') >= 0)
				{
					document.write('<td>' + 'INTERNET（上网业务）' + '</td>');

					if(PonInfo.PonState == "up")
					{
						if('up' == Wan_Status4[i])
						{
							document.write('<td align="center">正常&nbsp;</td>');
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}
					}
					else
					{
						document.write('<td align="center">异常&nbsp;</td>');
					}

					if( (Wan_ENCAP[i].indexOf('PPPoE') >=0 ) || (Wan_ENCAP[i].indexOf('DHCP') >=0 ) )
					{
						document.write('<td align="center">' + '路由（终端内置拨号）' + '&nbsp;</td>');
					}
					else
					{
						document.write('<td align="center">' + '桥接（电脑拨号）' + '&nbsp;</td>');
					}
					document.write('<td>' + Wan_IP4[i] + '&nbsp;</td>');
					if((PonInfo.PonState == "up")&&('up' == Wan_Status4[i]))
						document.write('<td>' + '/' + '&nbsp;</td>');
					else{
						var ErrCode=Wan_ConnectionError[i];
						var ErrDes;
						if ( ErrCode == 'ERROR_AUTHENTICATION_FAILURE' )
							ErrDes='认证失败';
						else if ( ErrCode == 'ERROR_IDLE_DISCONNECT' )
							ErrDes='空闲断开';
						else if ( ErrCode == 'ERROR_ISP_TIME_OUT' )
							ErrDes='响应超时';
						else if ( ErrCode == 'ERROR_USER_DISCONNECT' )
							ErrDes='用户断开';
						else if ( ErrCode == 'ERROR_NO_ANSWER' )
							ErrDes='无响应';
						else if ( ErrCode == 'ERROR_NOT_ENABLED_FOR_INTERNET' )
							ErrDes='无效链接';
						else
							ErrDes='其他异常';								
						document.write('<td>' + ErrDes + '&nbsp;</td>');					
					}
					document.write('<td>' + '/' + '&nbsp;</td>');
				}
				else if(Wan_WanName[i].indexOf('VOICE') >= 0)
				{
					document.write('<td>' + 'VOIP（语音业务）' + '</td>');

					if(PonInfo.PonState == "up")
					{
						if('up' == Wan_Status4[i])
						{
							document.write('<td align="center">正常&nbsp;</td>');
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}
					}
					else
					{
						document.write('<td align="center">异常&nbsp;</td>');
					}
					document.write('<td align="center">' + '路由（DHCP自动获取）' + '&nbsp;</td>');
					document.write('<td>' + Wan_IP4[i] + '&nbsp;</td>');
					document.write('<td>' + '/' + '&nbsp;</td>');
					VoipInfo.showRegisterState(1);
				}

				document.write('</tr>');
				
				if(ITV_name != '')
				{
					document.write('<tr align="middle">');

					if(Wan_WanValue[i].indexOf(ITV_name) >= 0)
					{
						document.write('<TD align=middle>' + 'ITV业务' + '&nbsp;</TD>');	

						if(PonInfo.PonState == "up")
						{
							if('up' == Wan_Status4[i])
							{
								document.write('<td align="center">正常&nbsp;</td>');
							}
							else
							{
								document.write('<td align="center">异常&nbsp;</td>');
							}
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}

						document.write('<TD align="center">' + '桥接（ITV终端自动获取）'+ '&nbsp;</TD>');
						document.write('<td>' + Wan_IP4[i] + '&nbsp;</td>');
						document.write('<TD align="center">' + '/' + '&nbsp;</TD>');
						document.write('<TD align="center">' + '/' + '&nbsp;</TD>');
					}
					document.write('</tr>');				
				}
			}
			else if(Wan_IPVERSION[i] == "IPv6")
			{
				document.write('<tr align="middle">');
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
				if(Wan_IP6[i] == '-')
				{
						Wan_IP6[i] = '/';
				}
<%end if%>
				if(Wan_WanName[i].indexOf('TR069') >= 0)
				{
					document.write('<td>' + 'ITMS管理通道状态' + '</td>');

					if(PonInfo.PonState == "up")
					{
						if('up' == Wan_Status6[i])
						{
							document.write('<td align="center">正常&nbsp;</td>');
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}
					}
					else
					{
						document.write('<td align="center">异常&nbsp;</td>');
					}
					document.write('<td align="center">' + '路由（DHCP自动获取）' + '&nbsp;</td>');
					document.write('<td>' + Wan_IP6[i] + '&nbsp;</td>');
					document.write('<td>' + '/' + '&nbsp;</td>');
					document.write('<td>' + '/' + '&nbsp;</td>');
				}
				else if(Wan_WanName[i].indexOf('INTERNET') >= 0)
				{
					document.write('<td>' + 'INTERNET（上网业务）' + '</td>');

					if(PonInfo.PonState == "up")
					{
						if('up' == Wan_Status6[i])
						{
							document.write('<td align="center">正常&nbsp;</td>');
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}
					}
					else
					{
						document.write('<td align="center">异常&nbsp;</td>');
					}

					if( (Wan_ENCAP[i].indexOf('PPPoE') >=0 ) || (Wan_ENCAP[i].indexOf('DHCP') >=0 ) )
					{
						document.write('<td align="center">' + '路由（终端内置拨号）' + '&nbsp;</td>');
					}
					else
					{
						document.write('<td align="center">' + '桥接（电脑拨号）' + '&nbsp;</td>');
					}
					document.write('<td>' + Wan_IP6[i] + '&nbsp;</td>');
					if((PonInfo.PonState == "up")&&('up' == Wan_Status6[i]))
						document.write('<td>' + '/' + '&nbsp;</td>');
					else{
						var ErrCode=Wan_ConnectionError[i];
						var ErrDes;
						if ( ErrCode == 'ERROR_AUTHENTICATION_FAILURE' )
							ErrDes='认证失败';
						else if ( ErrCode == 'ERROR_IDLE_DISCONNECT' )
							ErrDes='空闲断开';
						else if ( ErrCode == 'ERROR_ISP_TIME_OUT' )
							ErrDes='响应超时';
						else if ( ErrCode == 'ERROR_USER_DISCONNECT' )
							ErrDes='用户断开';
						else if ( ErrCode == 'ERROR_NO_ANSWER' )
							ErrDes='无响应';
						else if ( ErrCode == 'ERROR_NOT_ENABLED_FOR_INTERNET' )
							ErrDes='无效链接';
						else
							ErrDes='其他异常';								
						document.write('<td>' + ErrDes + '&nbsp;</td>');					
					}
					document.write('<td>' + '/' + '&nbsp;</td>');
				}
				else if(Wan_WanName[i].indexOf('VOICE') >= 0)
				{
					document.write('<td>' + 'VOIP（语音业务）' + '</td>');

					if(PonInfo.PonState == "up")
					{
						if('up' == Wan_Status6[i])
						{
							document.write('<td align="center">正常&nbsp;</td>');
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}
					}
					else
					{
						document.write('<td align="center">异常&nbsp;</td>');
					}
					document.write('<td align="center">' + '路由（DHCP自动获取）' + '&nbsp;</td>');
					document.write('<td>' + Wan_IP6[i] + '&nbsp;</td>');
					document.write('<td>' + '/' + '&nbsp;</td>');
					VoipInfo.showRegisterState(1);
				}

				document.write('</tr>');
				
				if(ITV_name != '')
				{				
					document.write('<tr align="middle">');

					if(Wan_WanValue[i].indexOf(ITV_name) >= 0)
					{
						document.write('<TD align=middle>' + 'ITV业务' + '&nbsp;</TD>');	

						if(PonInfo.PonState == "up")
						{
							if('up' == Wan_Status6[i])
							{
								document.write('<td align="center">正常&nbsp;</td>');
							}
							else
							{
								document.write('<td align="center">异常&nbsp;</td>');
							}
						}
						else
						{
							document.write('<td align="center">异常&nbsp;</td>');
						}

						document.write('<TD align="center">' + '桥接（ITV终端自动获取）'+ '&nbsp;</TD>');
						document.write('<td>' + Wan_IP6[i] + '&nbsp;</td>');
						document.write('<TD align="center">' + '/' + '&nbsp;</TD>');
						document.write('<TD align="center">' + '/' + '&nbsp;</TD>');
					}
					document.write('</tr>');
				}
			}
		}
	}
}

function PrintRecvPower()
{
	if(PonInfo.RecvPower == '-')
	{
		document.write('<TD width="250">' + '<font color="#ff0000">' +'光纤插接不正常，请检查光路或ODF是否有接错'+ '&nbsp;</font></TD>');
	}
	else if(PonInfo.RecvPower < -24)
	{
		document.write('<TD width="250">' + '<font color="#ff0000">' +'光功率过低，请检查光路或ODF是否有接错'+ '&nbsp;</font></TD>');
	}
	else
	{
    	document.write('<TD width="250">' + PonInfo.RecvPower + 'dBm' + '&nbsp;</TD>');
	}
}

function VoipInfoClass()
{
  this.Reg1Status  = '<%if tcWebAPI_get("InfoVoIP_Entry0", "Status", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry0", "Status", "s") else asp_Write("") end if %>';
  this.Reg2Status  = '<%if tcWebAPI_get("InfoVoIP_Entry1", "Status", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry1", "Status", "s") else asp_Write("") end if %>';
  this.Line1Number = '<%if tcWebAPI_get("VoIPBasic_Entry0", "SIPDisplayName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry0", "SIPDisplayName", "s") else asp_Write("") end if %>';
  this.Line2Number = '<%if tcWebAPI_get("VoIPBasic_Entry1", "SIPDisplayName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry1", "SIPDisplayName", "s") else asp_Write("") end if %>';
  this.showRegisterState = showRegisterState;
}
var VoipInfo = new VoipInfoClass();

function showRegisterState(port)
{
	var Status = '';

	if ( 1 == port )
	{
		Status = this.Reg1Status;
	}
	else if ( 2 == port )
	{
		Status = this.Reg2Status;
	}
	else
	{
		return;
	}

	if ( Status == 'Up' )
		document.write('<td>' +'正常注册'+ '&nbsp;</td>');
	else if ( Status == 'Initializing' )
		document.write('<td>' +'初始化中'+ '&nbsp;</td>');
	else if ( Status == 'Registering' )
		document.write('<td>' +'注册中'+ '&nbsp;</td>');
	else if ( Status == 'Unregistering' )
		document.write('<td>' +'未注册'+ '&nbsp;</td>');
	else if ( Status == 'Quiescent' )
		document.write('<td>' +'停止的'+ '&nbsp;</td>');
	else if ( Status == 'Disabled' )
		document.write('<td>' +'去使能'+ '&nbsp;</td>');
	else if ( Status == 'Error' )
		document.write('<td>' +'故障'+ '&nbsp;</td>');
	else if ( Status == 'Testing' )
		document.write('<td>' +'测试中'+ '&nbsp;</td>');
	else
		document.write('<td>' +'其他异常'+ '&nbsp;</td>');
}
<% end if%>
</SCRIPT>

      <FORM style="DISPLAY: none" name=ConfigForm></FORM>
      <SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD class=Item_L1 width=157 bgColor=#e7e7e7 height=30>
            <P>设备基本信息</P></TD>       
          <TD width=7 bgColor=#e7e7e7>　</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
          <TD width=474>　
            <LABEL>在本页面上，您可以查看设备的基本信息、ITMS注册状态和业务状态显示。
            </LABEL></TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=6>
<%else%>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=4>
<% end if%>	
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A href="/cgi-bin/help_content.asp#状态" 
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
          <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=400 border=1>
                    <TBODY>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONYNSupported", "h") = "Yes"  then %>
                    <TR>
                      <TD width=100 class=table_title>设备类型:</TD>
                      <TD width=300>
            		<SCRIPT language=javascript>
						document.write(deviceType);
					</SCRIPT>
                        &nbsp;</TD></TR>
                    </TR>
                    <TR>
                      <TD class=table_title>设备接入类型:</TD>
                      <TD width=300>
            		<SCRIPT language=javascript>
						var xponMode = '<% tcWebApi_get("XPON_Common","xponMode","s")%>';
                        if ((xponMode == "N/A") || ( xponMode == "GPON"))
                        	document.write("Gpon");
                        else
                            document.write("Epon");
					</SCRIPT>
                        &nbsp;</TD></TR>
                    </TR>
<% end if %>
                      <TD class=table_title>设备型号:</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
                      <TD width=250>
<%else%>
                      <TD width=300>
<% end if%>
            		<SCRIPT language=javascript>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
						document.write(top.ModelName_dev);
<% else %>
						document.write(top.ModelName);
<% end if %>   
					</SCRIPT>
                        &nbsp;</TD></TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
                    <TR style="display:none">
                      <TD class=table_title>描述:</TD>
                      <TD width=250>
                    <SCRIPT language=javascript>
                        document.write('<% tcWebApi_get("DeviceInfo_devParaStatic","DevDescribtion","s") %>');
                    </SCRIPT>
                        &nbsp;</TD></TR>
<% end if%>
                    <TR>
                      <TD class=table_title>设备标识号:</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
                      <TD width=250 style="word-break:break-all">
<%else%>
                      <TD width=300 style="word-break:break-all">
<% end if%>
					<%If tcWebApi_get("DeviceInfo_devParaDynamic","ManufacturerOUI","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaDynamic","ManufacturerOUI","s") asp_write("-") end if%><%If tcWebApi_get("DeviceInfo_devParaDynamic","SerialNum","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaDynamic","SerialNum","s") end if%>
                        &nbsp;
                      </TD></TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
                    <TR style="DISPLAY: none">
                      <TD class=table_title>家庭网卡状态:</TD>
                      <TD width=250>家庭网关卡插入正常</TD>
<%else%>
                    <TR>
                      <TD class=table_title>硬件版本:</TD>
                      <TD width=300>
					  <%If tcWebApi_get("DeviceInfo_devParaStatic","CustomerHWVersion","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaStatic","CustomerHWVersion","s") end if%>
<% end if%>
                        &nbsp;                      </TD></TR>
                    <TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
                      <TD class=table_title>OLT注册状态:</TD>
                      <TD width=250>
	                    <%If tcWebApi_get("XPON_Common","trafficStatus","h") = "up" Then%>
	                        <SCRIPT language=JavaScript type=text/javascript>
	                            document.write("注册正常");
	                        </SCRIPT>
	                    <%else%>
	                        <SCRIPT language=JavaScript type=text/javascript>
	                            document.write("注册失败（请检查收光功率或确认LOID是否正确）");
	                        </SCRIPT>
	                    <%end if%>					
                        &nbsp;                      </TD></TR>
                    <TR>
                      <TD class=table_title>ITMS注册状态:</TD>
                      <TD width=250>
	                        <SCRIPT language=JavaScript type=text/javascript>
	                            var Status = '<% tcWebApi_get("deviceAccount_Entry","registerStatus","s")%>';
	                            if ( Status == '0' ||  Status == '5')
	                            	document.write("注册正常");
	                            else
	                            	document.write("注册失败");
	                        </SCRIPT>
                       &nbsp;                      </TD></TR>
                   <TR>
                      <TD class=table_title>接收光功率:</TD>
	                    <SCRIPT language=javascript>				
	                            PrintRecvPower();
	                    </SCRIPT>
                        &nbsp;                      </TR>
<%else%>
                      <TD class=table_title>软件版本:</TD>
                      <TD width=300>
					  <%If tcWebApi_get("DeviceInfo_devParaStatic","CustomerSWVersion","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaStatic","CustomerSWVersion","s") end if%>
                        &nbsp;                      </TD></TR>
                    <TR id=div_BatCode>
                      <TD class=table_title>批次编码:</TD>
                      <TD width=300>
					  <%If tcWebApi_get("DeviceInfo_devParaStatic","BatCode","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaStatic","BatCode","s") end if%>
                        &nbsp;                      </TD></TR>
			<SCRIPT language=JavaScript type=text/javascript>
				getElement("div_BatCode").style.display = "none";
			</SCRIPT>
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONYNSupported", "h") = "Yes"  then %>
                    <TR>
                      <TD class=table_title>PON注册状态:</TD>
                      <TD width=250>
            		<SCRIPT language=javascript>
                        var g_authStstus = '<% tcWebApi_get("GPON_LOIDAuth","AuthStatus","s")%>';
                        var e_authStstus = '<% tcWebApi_get("EPON_LOIDAuth","AuthStatus","s")%>';
                        var trafficStstus = '<% tcWebApi_get("XPON_Common","trafficStatus","s")%>';
                        var linkStstus = '<% tcWebApi_get("XPON_LinkCfg","LinkSta","s")%>';
                        if (trafficStstus == "up")
                        {
                            document.write("注册成功");
                        }
                        else if(linkStstus != "0")       // phy up
                        {
                            if((linkStstus == "1" && g_authStstus == "2") || (linkStstus == "2" && e_authStstus == "2"))        // linkStstus 1:gpon 2:epon
                            {
                            document.write("注册失败");
                            }
                            else
                            {
                            document.write("正在注册");
                            }
                        }
                        else
                        {
                            document.write("未注册");
                        }
					</SCRIPT>
                        &nbsp;</TD></TR>
                    </TR>
<% end if %>
<% if tcWebApi_get("WebCustom_Entry", "isCTGDSupported", "h") = "Yes"  then %>
<TR>
                      <TD class=table_title>PON注册状态:</TD>
                      <TD width=300>
					<SCRIPT language=JavaScript type=text/javascript>
                var trafficStatus = '<% tcWebApi_get("XPON_Common","trafficStatus","s")%>';         
								if(trafficStatus == "up")
                            {
									document.write("成功-已注册已认证");
                            }
                            else
                            {
									var losStatus = '<% tcWebApi_get("XPON_Common","losStatus","s")%>';
                  if ( losStatus.indexOf("down") >= 0)
								{
                     document.write("失败-未注册未认证");
								}
								else
								{
									document.write("失败-已注册未认证");
								}
                            }
	                </SCRIPT>
                      </TD>
</TR>
<% end if%>
			</TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=400 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width="20%">&nbsp;</TD>
                      <TD class=table_title align=middle width="15%">通道（WAN连接）状态</TD>
                      <TD class=table_title align=middle width="20%">IP地址获取方式</TD>
                      <TD class=table_title align=middle width="15%">获取的IP地址</TD>
                      <TD class=table_title align=middle width="15%">上网拨号状态</TD>
                      <TD class=table_title align=middle width="15%">语音注册状态</TD></TR>
                        <SCRIPT language=JavaScript type=text/javascript>
                          ListCreate_info1();
                        </SCRIPT>
                    </TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR>
<% end if%>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
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
