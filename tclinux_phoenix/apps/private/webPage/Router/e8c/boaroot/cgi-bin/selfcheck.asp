<%
	If Request_Form("stb_test")="1" then 
		tcWebApi_constSet("StbTest_Entry", "WorkStatus", "Start")
	end if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>设备自检信息</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<% if tcWebApi_get("StbTest_Entry", "WorkStatus", "h") = "Start"  then %>
<META HTTP-EQUIV="refresh" CONTENT="3; URL=./selfcheck.asp">
<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center; valign: middle" vLink="#000000" aLink="#000000" 
link="#000000" leftMargin="0" topMargin="0" onload="LoadFrame()" marginheight="0" 
marginwidth="0">
<table>
	<tbody>
         <TABLE>
          <TBODY>
             <TR>
              <TD><font style="font-size: 50px; margin: 50px 50px; margin-top: 50px">
              自检中, 请稍等...
              </TD>
             </TR>
           </TBODY>
         </TABLE>
   </tbody>
</table>
</BODY>
<% elseif tcWebApi_get("StbTest_Entry", "WorkStatus", "h") = "Running"  then %>
<META HTTP-EQUIV="refresh" CONTENT="3; URL=./selfcheck.asp">
<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center; valign: middle" vLink="#000000" aLink="#000000" 
link="#000000" leftMargin="0" topMargin="0" onload="LoadFrame()" marginheight="0" 
marginwidth="0">
<table>
	<tbody>
         <TABLE>
          <TBODY>
             <TR>
              <TD><font style="font-size: 50px; margin: 50px 50px; margin-top: 50px">
              自检中, 请稍等...
              </TD>
             </TR>
           </TBODY>
         </TABLE>
   </tbody>
</table>
</BODY>
<% else %>
<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center; valign: middle" vLink="#000000" aLink="#000000" 
link="#000000" leftMargin="0" topMargin="0" onload="LoadFrame()" marginheight="0" 
marginwidth="0">
<SCRIPT language=JavaScript type=text/javascript>
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


function pageHeight()
{
	return window.innerHeight != null? window.innerHeight : document.documentElement && document.documentElement.clientHeight ? document.documentElement.clientHeight : document.body != null? document.body.clientHeight : null;
}
var percent = 0;
var winHeight = pageHeight();
temp = new Array(new stDeviceInfo("InternetGatewayDevice.DeviceInfo",top.ModelName,"19191900AABB818180","HG530RRA.VER.C","V100R001C01B010","00AABB"),null);
var DeviceInfo = temp[0];
if (winHeight > 540)
{
winHeight = (winHeight - 540)/2;
}
else
{
winHeight = 80;
}
document.write('<table width="808" height="0" align="center"  style="margin-top:'+winHeight+'px">');
function LoadFrame()
{
}
function stDeviceInfo(domain,ProductClass,SerialNumber,HardwareVersion,SoftwareVersion,ManufacturerOUI)
{
this.domain = domain;
this.ProductClass = ProductClass;
this.ManufacturerOUI = ManufacturerOUI;
this.SerialNumber = SerialNumber;
this.HardwareVersion = HardwareVersion;
this.SoftwareVersion = SoftwareVersion;
}
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
	this.RecvPowerStats;
	this.SendPowerStats;
	this.VoIPipaddr;
	this.wanip;
	this.wanstatus;
	this.tr69ip;
	this.getVoIPipaddr = getVoIPipaddr;
	this.getwanip = getwanip;
	this.getwanstatus = getwanstatus;
	this.gettr069ip = gettr069ip;
	this.wanipindex;
	this.tr69ipindex;
	this.wanstaindex;
	function transTemperature(temperature)
	{
			var temp = Number(temperature);
			if (temp >= Math.pow(2, 15)){
				return -Math.round(((Math.pow(2, 16)-temp)/256)*10)/10;
			}else{
				return Math.round((temp/256)*10)/10;
			}
	}		
	if ( 'up' == this.trafficstate )
	{
		this.PonState 			= 'up';
		this.WarnInfo 			= 'no info';
	}
			
	if( '0' != this.LinkSta)
	{
		this.SendPower			= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "TxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
		this.RecvPower			= (Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "RxPower", "s")%>))/10000)/(Math.log(10))*100)/10);
		this.WorkTemperature 	= transTemperature(<%tcWebApi_get("Info_PonPhy", "Temperature", "s")%>);
	}

	if ( '1' == this.fecState )
	{
		this.FECEnable			= 'enabled';
	}
	
	if( '-' == this.RecvPower)
	{
		this.RecvPowerStats = '收光无光';
	}
	else
	{
		if ( this.RecvPower > -8 )
			this.RecvPowerStats = '收光过强';
		else if( this.RecvPower >= -27 && this.RecvPower <= -8 )
			this.RecvPowerStats = '收光正常';
		else
			this.RecvPowerStats = '收光过低';
	}

	if(this.SendPower == '-')
		this.SendPowerStats = '发光异常';
	else
		this.SendPowerStats = '发光正常';


function getVoIPipaddr()
{
	for(var i=0;i<pvc_counts;i++)
	{
		if(Wan_WanName[i].indexOf('VOICE') >= 0)
		{
			if(("up" == this.PonState ) && ('up' == Wan_Status4[i]))
			{
				this.VoIPipaddr = Wan_IP4[i];
				return (this.VoIPipaddr);
			}
			else if(("up" == this.PonState )&&('up' == Wan_Status6[i]))
			{
				this.VoIPipaddr = Wan_IP6[i];
				return (this.VoIPipaddr);
			}
			else
			{
				this.VoIPipaddr = "未获取宽带地址";
			}
		}
		else
		{
			this.VoIPipaddr = "未获取宽带地址";
		}
	}
	return this.VoIPipaddr;
}

function getwanip()
{
	var index = 1;
	var ipaddr = new Array();
	var j=0;
	for( var i=0;i<pvc_counts;i++)
	{
		if(Wan_Actives[i] != "N/A")
		{
			if(Wan_WanName[i].indexOf('INTERNET') >= 0)
			{
				if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
				{
					if(this.PonState == "up")
					{
							this.wanip = Wan_IP4[i];
							index++;
							ipaddr[index] = this.wanip;
					}
					else
					{
							this.wanip = "未获取宽带地址";	
					}
				}
				else if(Wan_IPVERSION[i] == "IPv6")
				{
					if(this.PonState == "up")
					{
							this.wanip = Wan_IP6[i];
							index++;
							ipaddr[index] = this.wanip;
					}
					else
				  {
							this.wanip = "未获取宽带地址";	
				  }	
				}
				else
				{
					this.wanip = '未获取宽带地址';	
				}
			}
			else
			{
				this.wanip = '未获取宽带地址';
			}	
		}
		else
		{
			this.wanip = '未获取宽带地址';
		}	
	}
	if(index == 1)
		return "未获取宽带地址";
	else
	{
		for(j=2;j<=index;j++)
		{	
			if(ipaddr[j]!= '-')
			{
				this.wanipindex = j;
				return 	ipaddr[j];
			}
		}
		if(ipaddr[index]== '-')
		{
			this.wanipindex = index;
			return "未获取宽带地址";
		}
	}
}
function gettr069ip()
{
	var index = 1;
	var ipaddr = new Array();
	for( var i=0;i<pvc_counts;i++)
	{
		if(Wan_Actives[i] != "N/A")
		{
			if(Wan_WanName[i].indexOf('TR069') >= 0)
			{
				if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
				{
					if(this.PonState == "up")
					{
							this.tr69ip = Wan_IP4[i];
							index++;
							ipaddr[index] = this.tr69ip;
					}
					else
					{
							this.tr69ip = "未获取管理地址";	
					}
				}
				else if(Wan_IPVERSION[i] == "IPv6")
				{
					if(this.PonState == "up")
					{
							this.tr69ip = Wan_IP6[i];
							index++;
							ipaddr[index] = this.tr69ip;
					}
					else
				  {
							this.tr69ip = "未获取管理地址";	
				  }	
				}
				else
				{
					this.tr69ip = '未获取管理地址';	
				}
			}
			else
			{
				this.tr69ip = '未获取管理地址';
			}	
		}
		else
		{
			this.tr69ip = '未获取管理地址';
		}	
	}
	if(index == 1)
		return "未获取管理地址";
	else
	{
		for(j=2;j<=index;j++)
		{	
			if(ipaddr[j]!= '-')
			{
				this.tr69ipindex = j;
				return 	ipaddr[j];
			}
		}
		if(ipaddr[index]== '-')
		{
			this.tr69ipindex = index;
			return "未获取宽带地址";
		}
	}
}
	function getwanstatus()
	{
		var Wan_encap;
		var index = 1;
		var wanstate = new Array();
		for(var i=0;i<pvc_counts;i++)
		{
			if(Wan_Actives[i] != "N/A")
			{
				if(Wan_WanName[i].indexOf('INTERNET') >= 0)
				{
				if(Wan_IPVERSION[i] == "IPv6")
				{
					if((Wan_ENCAP[i].indexOf('PPPoE') >=0 ) || (Wan_ENCAP[i].indexOf('DHCP') >=0 )) 
					{
							Wan_encap = "路由模式-";
							
							if((this.PonState == "up")&&('up' == Wan_Status6[i]) )
							{
									this.wanstatus = "拨号成功";
									this.wanstatus = Wan_encap + this.wanstatus;
									index++;
									wanstate[index] = this.wanstatus;
							}
							else{
								var ErrCode=Wan_ConnectionError[i];
								var ErrDes;
								if ( ErrCode == 'ERROR_AUTHENTICATION_FAILURE' )
										ErrDes="拨号失败";
								else if ( ErrCode == 'ERROR_IDLE_DISCONNECT' )
										ErrDes="空闲未拨号";
								else if ( ErrCode == 'ERROR_ISP_TIME_OUT' )
										ErrDes="拨号失败";
								else if ( ErrCode == 'ERROR_USER_DISCONNECT' )
										ErrDes="拨号失败";
								else if ( ErrCode == 'ERROR_NO_ANSWER' )
										ErrDes="拨号失败";
								else if ( ErrCode == 'ERROR_NOT_ENABLED_FOR_INTERNET' )
										ErrDes="拨号失败";
								else
										ErrDes="拨号失败";								
								this.wanstatus = ErrDes+Wan_encap;	
								index++;
								wanstate[index] = this.wanstatus;	
							}
					}
					else
					{
							Wan_encap = "桥接模式";
							index++;
							wanstate[index] = Wan_encap;
					}
					
				}
				else if(Wan_IPVERSION[i] == "IPv4")
				{
							if((Wan_ENCAP[i].indexOf('PPPoE') >=0 ) || (Wan_ENCAP[i].indexOf('DHCP') >=0 )) 
							{
									Wan_encap = "路由模式-";
									
									if((this.PonState == "up")&&('up' == Wan_Status4[i]))
									{
											this.wanstatus = "拨号成功";
											this.wanstatus = Wan_encap + this.wanstatus;
											index++;
											wanstate[index] = this.wanstatus;
									}
									else{
											var ErrCode=Wan_ConnectionError[i];
											var ErrDes;
									if ( ErrCode == 'ERROR_AUTHENTICATION_FAILURE' )
											ErrDes="拨号失败";
									else if ( ErrCode == 'ERROR_IDLE_DISCONNECT' )
											ErrDes="空闲未拨号";
									else if ( ErrCode == 'ERROR_ISP_TIME_OUT' )
											ErrDes="拨号失败";
									else if ( ErrCode == 'ERROR_USER_DISCONNECT' )
											ErrDes="拨号失败";
									else if ( ErrCode == 'ERROR_NO_ANSWER' )
											ErrDes="拨号失败";
									else if ( ErrCode == 'ERROR_NOT_ENABLED_FOR_INTERNET' )
											ErrDes="拨号失败";
									else
											ErrDes="拨号失败";								
									this.wanstatus =Wan_encap + ErrDes;
									index++;
									wanstate[index] = this.wanstatus;					
									}
							}
							else
							{
									Wan_encap = "桥接模式";
									index++;
									wanstate[index] = Wan_encap;	
							}
						
				}
				else
				{
					this.wanstatus = "拨号失败";
				}
				}
			}
			else
			{
					this.wanstatus = "未配置";
			}
		}
		
			var wanip  = PonInfo.getwanip();
			this.wanstaindex = index;
			if(this.wanstaindex == 1)
				return "未配置";
			else
				return wanstate[this.wanstaindex];
	}
}
var PonInfo = new PonInfoClass();
function VoipInfoClass()
{
  this.Reg1Status  = '<%if tcWebAPI_get("InfoVoIP_Entry0", "Status", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry0", "Status", "s") else asp_Write("") end if %>';
  this.Reg2Status  = '<%if tcWebAPI_get("InfoVoIP_Entry1", "Status", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry1", "Status", "s") else asp_Write("") end if %>';
  this.Reg1PCMErr = '<%if tcWebAPI_get("InfoVoIP_Entry0", "RegFailReason", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry0", "RegFailReason", "s") else asp_Write("0") end if %>';
  this.Reg2PCMErr = '<%if tcWebAPI_get("InfoVoIP_Entry1", "RegFailReason", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry1", "RegFailReason", "s") else asp_Write("0") end if %>';
  this.Line1Number = '<%if tcWebAPI_get("VoIPBasic_Entry0", "SIPDisplayName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry0", "SIPDisplayName", "s") else asp_Write("") end if %>';
  this.Line2Number = '<%if tcWebAPI_get("VoIPBasic_Entry1", "SIPDisplayName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry1", "SIPDisplayName", "s") else asp_Write("") end if %>';
  this.showRegisterState = showRegisterState;
}
var VoipInfo = new VoipInfoClass();
<% if tcWebApi_get("WebCustom_Entry", "isCT1FXSSupported", "h") <> "Yes"  then %>
function showRegisterState()
{
	var Status1 = this.Reg1Status;
	var Status2 = this.Reg2Status;
	var RegErr1 = this.Reg1PCMErr;
	var RegErr2 = this.Reg2PCMErr;
	var voiptatus = '';

	if ( 'Up' == Status1 )
		voiptatus = "1 注册正常; ";
	else
	{
		switch ( parseInt(RegErr1) )
		{
			case 2:
			case 3:
					voiptatus = "1 注册超时; ";
				break;
			case 4:
					voiptatus = "1 账号密码错误; ";
				break;
			default:
					voiptatus = "1 注册失败; ";
				break;
		}
	}

	if ( 'Up' == Status2 )
		voiptatus = voiptatus + "2 注册正常";
	else
	{
		switch ( parseInt(RegErr2) )
		{
			case 2:
			case 3:
					voiptatus = voiptatus + "2 注册超时";
				break;
			case 4:
					voiptatus = voiptatus + "2 账号密码错误";
				break;
			default:
					voiptatus = voiptatus + "2 注册失败";
				break;
		}
	}

	document.write(voiptatus);
}

<%else%>
function showRegisterState(port)
{
	var Status = '';
	var RegErr = '';

	if ( 1 == port )
	{
		Status = this.Reg1Status;
		RegErr = this.Reg1PCMErr;
	}
	else if ( 2 == port )
	{
		Status = this.Reg2Status;
		RegErr = this.Reg2PCMErr;
	}
	else
	{
		return;
	}

	if ( 'Up' == Status )
		voiptatus = "注册正常";
	else
	{
		switch ( parseInt(RegErr) )
		{
			case 2:
			case 3:
					voiptatus = "注册超时";
				break;
			case 4:
					voiptatus = "账号密码错误";
				break;
			default:
					voiptatus = "注册失败";
				break;
		}
	}

	document.write(voiptatus);
}
<%end if%>
var	isIPv4 = "<% tcWebApi_get("WebCurSet_Entry","IsIPv4","s") %>";
function writeReturnhref()
{
	var httpip;
	if("0" == isIPv4){
		lanip = "[" + "<% tcWebApi_get("Lan_Entry0","IP6","s") %>" + "]";
		if(lanip == "N/A")
			httpip = "http://[fe80::1]";
		else
			httpip = "http://" + lanip;
	}
	else{
		lanip = "<% tcWebApi_get("Lan_Entry0","IP","s") %>";
		if(lanip == "N/A")	
			httpip = "http://192.168.1.1/cgi-bin/register.asp";
		else
			httpip = "http://" + lanip + "/cgi-bin/register.asp";
	}
	document.write('<A href="' + httpip + '" onclick="resetCookie();">返回注册页面</A>');
}
</SCRIPT>
<DIV id=mainScreen style="DISPLAY: none">
<table>
<TR>
<TD style="MARGIN-TOP: 100px; FONT-SIZE: 13px">
	<script language="JavaScript" type="text/JavaScript">
	writeReturnhref();
	</script>
	</TD>
</TR>
</table>
<table>
	<tbody>
		<DIV id=secPonInfo1 style="TEXT-ALIGN: center; valign: middle">
                  <TABLE cellSpacing=0 cellPadding=3 width="50%" border=1 style=" margin-left:25%; margin-right:25%">
                    <TBODY>
                    <TR>
                    	<TD class='table_title' colspan='2'>硬件信息 </TD>
                    </TR>
                    <TR>
                      <TD class=table_title width=160>终端厂家信息:</TD>
                       <TD > 
												<%If tcWebApi_get("DeviceInfo_devParaStatic","Manufacturer","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaStatic","Manufacturer","s") end if%>  
												&nbsp;</TD>
                    </TR>
                    <TR>
                      <TD class=table_title width=160>硬件型号:</TD>
                       <TD > 
												<%If tcWebApi_get("DeviceInfo_devParaStatic","ModelName","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaStatic","ModelName","s") end if%>  
												&nbsp;</TD>
                    </TR>
                    <TR>
                      <TD class=table_title width=160>硬件版本:</TD>
                       <TD>
                        <%If tcWebApi_get("DeviceInfo_devParaStatic","CustomerHWVersion","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaStatic","CustomerHWVersion","s") end if%>
                         &nbsp;</TD>
                    </TR>
                    <TR>
                      <TD class=table_title width=160>终端CPU:</TD>
                      <TD>
                      <SCRIPT language=javascript>
                      		document.write("正常");
                      </SCRIPT>
                      	 &nbsp;</TD>
                    </TR>
                    <TR>
                      <TD class=table_title>终端内存:</TD>
                      <TD>
                      <SCRIPT language=javascript>
                      		document.write("正常");
                      </SCRIPT>
                        &nbsp;</TD>
                      </TR>
                    <TR>
                      <TD class=table_title>终端无线功能:</TD>
                      <TD>
                      <% if tcWebApi_get("WebCustom_Entry", "isWLanSupported", "h") = "Yes" then
								          		if tcWebApi_get("WLan_Common","APOn","h") = "1" Then
								          			 asp_Write("开启")
								          		else
								          			asp_Write("未开启")
								          		end if
                      		else
                      				asp_Write("无WiFi功能")
                      		end if
                       %>
                        &nbsp;</TD>
                    </TR>
                    <TR>
                      <TD class=table_title>终端网口状态:</TD>
                      <TD>
                      	<script language=JavaScript type=text/javascript>
													var PortConnStatus = new Array();
													PortConnStatus[0] = "<% tcWebApi_get("Info_Ether","Port1Status","s") %>";
													<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
													PortConnStatus[1] = "<% tcWebApi_get("Info_Ether","Port2Status","s") %>";
													<% end if %>
													<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
													PortConnStatus[2] = "<% tcWebApi_get("Info_Ether","Port3Status","s") %>";
													PortConnStatus[3] = "<% tcWebApi_get("Info_Ether","Port4Status","s") %>";
													<% end if %>

													var idx = 0;
													for ( idx = 0; idx < PortConnStatus.length; idx ++ )
													{
														document.write('LAN'+(idx+1)+':');
														if ( '1' == PortConnStatus[idx] )
															document.write('连接');
														else
															document.write('未连接');
															
														document.write('&nbsp;&nbsp;');
													}
	
												</script>
                        &nbsp;</TD>
                      </TR>
                    <TR>
                      <TD class=table_title>终端语音端口状态:</TD>
                      <TD>
                      <SCRIPT language=javascript>
                      		<% if tcWebApi_get("WebCustom_Entry", "isCT1FXSSupported", "h") = "Yes" then %>
                      		var status1 = '<% tcWebApi_get("VoIPSysParam_Entry0","SC_LINE_INFO_STATUS","s") %>';
                      		if ( 'Idle' == status1 )
                      			document.write('空闲');
                      		else
                      			document.write('占用');
                      		<% else %> 
                      		var status1 = '<% tcWebApi_get("VoIPSysParam_Entry0","SC_LINE_INFO_STATUS","s") %>';
                      		var status2 = '<% tcWebApi_get("VoIPSysParam_Entry1","SC_LINE_INFO_STATUS","s") %>';
                      		if(status1 == 'Idle' && status2 == 'Idle')
                      				document.write("1 空闲; 2 空闲");
                      		else if(status1 == 'Idle' && status2 != 'Idle')
                      				document.write("1 空闲; 2 占用");
                      		else if(status1 != 'Idle' && status2 == 'Idle')
                      				document.write("1 占用; 2 空闲");
                      		else
                      				document.write("1 占用; 2 占用");
                      		<% end if %>
                        </SCRIPT>
                      	&nbsp;</TD>
                     </TR>
                    <TR>
                      <TD class=table_title>收光状态:</TD>
                      <TD>
                      <SCRIPT language=javascript>				
	                    	document.write(PonInfo.RecvPowerStats);
	                    </SCRIPT>
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>发光状态:</TD>
                      <TD>
                      <SCRIPT language=javascript>				
	                    	document.write(PonInfo.SendPowerStats);
	                    </SCRIPT>	
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>光模块温度:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
											 document.write(PonInfo.WorkTemperature);
											</SCRIPT>℃
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>家庭网络二层丢帧率:</TD>
                      <TD>    
                      <% if tcWebApi_get("Info_Ether","Port2Status","h") <> "1" then %>
                      	LAN2未检测到机顶盒
                      <% else %>
                      	<%If tcWebApi_get("StbTest_Entry","DstMAC","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","DstMAC","s") end if%> 
                      						&nbsp;
											<%If tcWebApi_get("StbTest_Entry","LostRate","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","LostRate","s") end if%> 
					  	&nbsp;
					  <%end if%>

					  <% if TCWebApi_get("WebCustom_Entry","isCT2PORTSSupported","h" ) = "Yes" then %>
					  
					  <% elseif tcWebApi_get("Info_Ether","Port3Status","h") <> "1" then %>
                      	LAN3未检测到机顶盒
                      <% else %>
                      	- <%If tcWebApi_get("StbTest_Entry","DstMAC2","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","DstMAC2","s") end if%> 
                      						&nbsp;
											<%If tcWebApi_get("StbTest_Entry","LostRate2","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","LostRate2","s") end if%> 
					  	&nbsp;
					  <%end if%>
					  
					  	</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>家庭网络二层时延:</TD>
                      <TD>
                      <% if tcWebApi_get("Info_Ether","Port2Status","h") <> "1" then %>
                      	LAN2未检测到机顶盒
                      <% else %>
                      <%If tcWebApi_get("StbTest_Entry","DstMAC","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","DstMAC","s") end if%> 
                      						&nbsp;
                      <%If tcWebApi_get("StbTest_Entry","ResponseDelay","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","ResponseDelay","s") end if%>
											毫秒
                      &nbsp;
                      <% end if %>

					<% if TCWebApi_get("WebCustom_Entry","isCT2PORTSSupported","h" ) = "Yes" then %>
                    <% elseif tcWebApi_get("Info_Ether","Port3Status","h") <> "1" then %>
                      	LAN3未检测到机顶盒
                      <% else %>
                     - <%If tcWebApi_get("StbTest_Entry","DstMAC2","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","DstMAC2","s") end if%> 
                      						&nbsp;
                      <%If tcWebApi_get("StbTest_Entry","ResponseDelay2","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","ResponseDelay2","s") end if%>
											毫秒
                      &nbsp;
                      <% end if %>
                      
                      </TD>
                     </TR>
                     <TR>
                      <TD class=table_title>家庭网络二层抖动:</TD>
                      <TD>  
                      <% if tcWebApi_get("Info_Ether","Port2Status","h") <> "1" then %>
                      	LAN2未检测到机顶盒
                      <% else %>
                      <%If tcWebApi_get("StbTest_Entry","DstMAC","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","DstMAC","s") end if%> 
                      						&nbsp;
						<%If tcWebApi_get("StbTest_Entry","ResponseShake","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","ResponseShake","s") end if%>
						毫秒
                      &nbsp;
                      <% end if %>

					  <% if TCWebApi_get("WebCustom_Entry","isCT2PORTSSupported","h" ) = "Yes" then %>
                      <% elseif tcWebApi_get("Info_Ether","Port3Status","h") <> "1" then %>
                      	LAN3未检测到机顶盒
                      <% else %>
                     - <%If tcWebApi_get("StbTest_Entry","DstMAC2","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","DstMAC2","s") end if%> 
                      						&nbsp;
						<%If tcWebApi_get("StbTest_Entry","ResponseShake2","h") <> "N/A" Then tcWebApi_get("StbTest_Entry","ResponseShake2","s") end if%>
						毫秒
                      &nbsp;
                      <% end if %>
                      </TD>
                     </TR>
                     <TR>
                    	<TD class='table_title' colspan='2' >业务信息</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>终端软件信息:</TD>
                      <TD>
                      <%If tcWebApi_get("DeviceInfo_devParaStatic","CustomerSWVersion","h") <> "N/A" Then tcWebApi_get("DeviceInfo_devParaStatic","CustomerSWVersion","s") end if%>
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>OLT注册状态:</TD>
                      <TD>
                      <%If tcWebApi_get("XPON_Common","trafficStatus","h") = "up" Then%>
	                        <SCRIPT language=JavaScript type=text/javascript>
	                            document.write("已注册");
	                        </SCRIPT>
	                    <%else%>
	                        <SCRIPT language=JavaScript type=text/javascript>
	                            document.write("未注册");
	                        </SCRIPT>
	                    <%end if%>		
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>ITMS注册状态:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
	                         var Status = '<% tcWebApi_get("deviceAccount_Entry","registerStatus","s")%>';
	                         if ( Status == '0' ||  Status == '5')
	                            document.write("已注册");
	                         else
	                            document.write("未注册");
	                    </SCRIPT>
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>ITMS下发状态:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
                      		var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";	
                      		if (rresult == '1')
                      				document.write("已下发");
                      		else if(rresult == '99')
                      				document.write("未下发");
                      		else
                      				document.write("下发失败");
                      		
	                    </SCRIPT>
                        &nbsp;</TD>
                     </TR
                     <TR>
                      <TD class=table_title>管理地址:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
                      			document.write(PonInfo.gettr069ip());
                      </SCRIPT>
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>宽带拨号状态:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
                      			document.write(PonInfo.getwanstatus());
                    	</SCRIPT>
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>宽带IP地址:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
                      			document.write(PonInfo.getwanip());
                    	</SCRIPT>
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>语音注册状态:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
                      <% if tcWebApi_get("WebCustom_Entry", "isCT1FXSSupported", "h") <> "Yes"  then %>
                      		VoipInfo.showRegisterState();
                      <%else%>
                      		VoipInfo.showRegisterState(1);
                      <%end if%>
                    	</SCRIPT>	
                        &nbsp;</TD>
                     </TR>
                     <TR>
                      <TD class=table_title>语音IP地址:</TD>
                      <TD>
                      <SCRIPT language=JavaScript type=text/javascript>
                    			document.write(PonInfo.getVoIPipaddr());
												</SCRIPT>	
                        &nbsp;</TD>
                     </TR>
                      </TBODY>
                     </TABLE>
                  </DIV>
                </tbody></table></DIV></tbody></table>
	
  </BODY>
<% end if %>
</HTML>
