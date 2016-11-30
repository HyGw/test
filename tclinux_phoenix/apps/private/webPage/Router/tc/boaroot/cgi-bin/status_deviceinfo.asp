<%
	tcWebApi_set("WebCurSet_Entry","dev_pvc","DvInfo_PVC")
	tcWebApi_set("WebCurSet_Entry","dev_pvc_ext","DvInfo_PVC_Ext")
	If Request_Form("Saveflag")="1" then
		tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
		If Request_Form("Dipflag")="1" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="2" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="3" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_set("DeviceInfo_PVC","IP6DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="4" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_set("DeviceInfo_PVC","IP6DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		END If 
		If Request_Form("DipConnFlag")="1" then
			tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
			tcWebApi_commit("DeviceInfo_PVC");
		elseif Request_Form("DipConnFlag")="2" then
			tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
			tcWebApi_commit("DeviceInfo_PVC");
		END If 

		tcWebApi_set("Info_Dongle","ManualDialUp","DongleConnFlag")	
		If Request_Form("DongleConnFlag")="1" then
			tcWebApi_set("Info_Dongle","ManualDialUp","DongleConnFlag")
			tcWebApi_commit("Dongle_Entry");
		elseif Request_Form("DongleConnFlag")="2" then
			tcWebApi_set("Info_Dongle","ManualDialUp","DongleConnFlag")
			tcWebApi_commit("Dongle_Entry");
		END If 	

	END If 	  	
%>
<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<TITLE>ASP Example</TITLE>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language="JavaScript">
function doSave() {
	document.DvInfo_Form.submit();
}
function renewrelease(ip){
  document.DvInfo_Form.Dipflag.value = ip;
  document.DvInfo_Form.Saveflag.value = 1;
  document.DvInfo_Form.DipConnFlag.value = 0;
  document.DvInfo_Form.submit();  
}
function reconnect(flag){
  document.DvInfo_Form.DipConnFlag.value = flag;
  document.DvInfo_Form.Saveflag.value = 1;
  document.DvInfo_Form.submit();  
}
function dongle_reconnect(flag){
  document.DvInfo_Form.DongleConnFlag.value = flag;
  document.DvInfo_Form.Saveflag.value = 1;
  document.DvInfo_Form.submit();  
}

<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" then%>
function transTemperature(temperature){
	var temp = Number(temperature);
	if (temp >= Math.pow(2, 15)){
		return -Math.round((Math.pow(2, 16)-temp)/256);
	}else{
		return Math.round(temp/256);
	}
}
<%end if%>

<%if tcwebApi_get("WebCustom_Entry", "isC2TrueSupported", "h")="Yes" Then %>
self.setTimeout("self.location.reload(true);",10000);
<%end if%>
<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then %>
self.setTimeout("window.location='/cgi-bin/status_deviceinfo.asp';",10000);
<%end if%>
</script>
</HEAD>
<BODY>
<FORM METHOD="POST" ACTION="/cgi-bin/status_deviceinfo.asp" name="DvInfo_Form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<INPUT TYPE="HIDDEN" NAME="Saveflag" VALUE="0">
<tr>
<td height="5" class="light-orange" colspan="5">
	<INPUT TYPE="HIDDEN" NAME="Dipflag" VALUE="0">
</td></tr><tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF">Device Information</font></td><td width="10" class="black">&nbsp;</td><td width="150">&nbsp;</td><td width="10" >&nbsp;</td><td width="440">&nbsp;</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"> <div align=right>
<font color="#000000">Firmware Version</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_get("DeviceInfo","FwVer","h") <> "N/A" Then tcWebApi_staticGet("DeviceInfo","FwVer","s") end if%>     
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">MAC Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then%>
	<% if tcWebApi_staticGet("DeviceInfo_PVC","WanMac","h") <> "N/A" then tcWebApi_staticGet("DeviceInfo_PVC","WanMac","s") 
	elseif tcWebApi_staticGet("Info_Ether","mac","h") <> "N/A" then tcWebApi_staticGet("Info_Ether","mac","s") end if%>  
<% elseif tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "9" then%>	
	<% if tcWebApi_staticGet("DeviceInfo_PVC","WanMac","h") <> "N/A" then tcWebApi_staticGet("DeviceInfo_PVC","WanMac","s") 
	elseif tcWebApi_staticGet("Info_Ether","mac","h") <> "N/A" then tcWebApi_staticGet("Info_Ether","mac","s") end if%>  
<% elseif tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then%>	
	<% if tcWebApi_staticGet("DeviceInfo_PVC","WanMac","h") <> "N/A" then tcWebApi_staticGet("DeviceInfo_PVC","WanMac","s") 
	elseif tcWebApi_staticGet("Info_Ether","mac","h") <> "N/A" then tcWebApi_staticGet("Info_Ether","mac","s") end if%>  
	<%else%>
	<%If tcWebApi_get("Info_Ether","mac","h") <> "N/A" Then tcWebApi_staticGet("Info_Ether","mac","s") end if%>   
<%end if%>	
<%else%>
<%If tcWebApi_get("Info_Ether","mac","h") <> "N/A" Then tcWebApi_staticGet("Info_Ether","mac","s") end if%>   
<%end if%>
</td></tr><tr>
<td class="title-main">
<font color="#FFFFFF">LAN</font></td><td class="black">&nbsp;</td><td><hr noshade></td><td><hr noshade></td><td><hr noshade></td></tr>
<tr>
		<td class="title-sub"><font color="#000000">IPv4</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("Info_Ether","ip","h") <> "N/A" Then tcWebApi_staticGet("Info_Ether","ip","s") end if%>       
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Subnet Mask</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_get("Lan_Entry","netmask","h") <> "N/A" Then tcWebApi_staticGet("Lan_Entry","netmask","s") end if%>  	  
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">DHCP Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("Lan_Dhcp","type","h") = "1" Then asp_Write("Enable") elseif tcWebApi_staticGet("Lan_Dhcp","type","h") = "2" Then asp_Write("Relay") else asp_Write("Disabled") end if%>
</td></tr>
<%if tcWebApi_get("DeviceInfo", "isSFU", "h") <> "Yes" then%>
<% if tcWebApi_staticGet("Info_Ether","isIPv6Supported","h") = "Yes" then %>
<tr>
		<td class="title-sub"><font color="#000000">IPv6</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">Link local IP</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("DeviceInfo_LanIP6","LinkLocal","h") <> "N/A" Then tcWebApi_Get("DeviceInfo_LanIP6","LinkLocal","s") end if%> 
</td></tr>

<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">Manual Global IP</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("DeviceInfo_LanIP6","ManualGlobal","h") <> "N/A" Then tcWebApi_Get("DeviceInfo_LanIP6","ManualGlobal","s") end if%>  
</td></tr>

<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">Dynamic Global IP</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("DeviceInfo_LanIP6","DynamicGlobal0","h") <> "N/A" Then tcWebApi_Get("DeviceInfo_LanIP6","DynamicGlobal0","s") end if%>     
</td></tr>

<% If tcWebApi_staticGet("DeviceInfo_LanIP6","DynamicGlobal1","h") <> "N/A" Then %>
<tr>   
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000"> </font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<% tcWebApi_staticGet("DeviceInfo_LanIP6","DynamicGlobal1","s") %>
</td></tr>
<% end if %>

<tr>   
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">DHCP Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_get("Dhcp6s_Entry","Enable","h") = "1" Then asp_Write("Enable") else asp_Write("Disabled") end if%>
</td></tr>
<% end if %>
<% end if %>
<%if tcwebApi_get("WebCustom_Entry","is3GDongleSupport","h")="Yes" then%>
</table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF">3G</font></td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td width="440"><hr noshade>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<INPUT TYPE="HIDDEN" NAME="DongleConnFlag" VALUE="0">
<font color="#000000">USB Device</font>    </div></td><td class="tabdata" ><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_get("Info_Dongle","Device","s")%>
        &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp 
		<%If tcWebApi_get("Dongle_Entry","CONNECTION","h") = "Connect_Manually" then%>
        		<%If tcWebApi_staticGet("Info_Dongle","IP","h") <> "N/A" Then%>
        			<input type="button" name="DongleDisconnect" value="Disconnect" onClick="dongle_reconnect(2)" >
        		<%else %>
        			<INPUT TYPE="button" NAME="DongleConnect" VALUE="Connect" onClick="dongle_reconnect(1)" >  		
			<%End if%>
		<%End if%>
</td></tr>
	<tr>
		<td class="title-sub"><font color="#000000">IPv4</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Status</font>    </div></td><td class="tabdata" ><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("Info_Dongle","Status","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("Info_Dongle","IP","s")%>       
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">Subnet Mask</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("Info_Dongle","SubMask","s")%>  	  
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Default Gateway</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("Info_Dongle","DefGW","s")%>	
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">DNS Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<% if tcWebApi_get("Dproxy_Entry","type","h") = "1" then %>
<%tcWebApi_staticGet("Dproxy_Entry","Primary_DNS","s")%>
<%else%>
<%tcWebApi_staticGet("Info_Dongle","DNSIP","s")%>
<%end if%>
</tr>
<%end if%>



</table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<INPUT TYPE="HIDDEN" NAME="IPv6PrivacyAddrsSupportedFlag" value="<%tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","s")%>" >
<tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF">WAN</font></td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td width="440"><hr noshade>
</td></tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000"><%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>Interface<%else%>Virtual Circuit<%end if%></font></div></td><td class="tabdata"><div align=center>:</div></td>
<td class="tabdata">
<SELECT NAME="DvInfo_PVC" SIZE="1" onChange="doSave()">
<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" then%>
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "0" then asp_Write("selected") end if %>>WAN0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "1" then asp_Write("selected") end if %>>WAN1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "2" then asp_Write("selected") end if %>>WAN2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "3" then asp_Write("selected") end if %>>WAN3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "4" then asp_Write("selected") end if %>>WAN4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "5" then asp_Write("selected") end if %>>WAN5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "6" then asp_Write("selected") end if %>>WAN6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "7" then asp_Write("selected") end if %>>WAN7
<%else%>
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "0" then asp_Write("selected") end if %>>PVC0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "1" then asp_Write("selected") end if %>>PVC1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "2" then asp_Write("selected") end if %>>PVC2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "3" then asp_Write("selected") end if %>>PVC3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "4" then asp_Write("selected") end if %>>PVC4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "5" then asp_Write("selected") end if %>>PVC5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "6" then asp_Write("selected") end if %>>PVC6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "7" then asp_Write("selected") end if %>>PVC7

<%else%>
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "0" then asp_Write("selected") end if %>>PVC0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "1" then asp_Write("selected") end if %>>PVC1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "2" then asp_Write("selected") end if %>>PVC2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "3" then asp_Write("selected") end if %>>PVC3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "4" then asp_Write("selected") end if %>>PVC4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "5" then asp_Write("selected") end if %>>PVC5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "6" then asp_Write("selected") end if %>>PVC6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "7" then asp_Write("selected") end if %>>PVC7
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		<OPTION value="8" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then asp_Write("selected") end if %>>PTM0
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		<OPTION value="10" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then asp_Write("selected") end if %>>WAN0
	<%end if%>

<%end if%>
<%end if%>

</SELECT></td>
</tr>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then%>
	<tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
	<font color="#000000">Service</font></div></td><td class="tabdata"><div align=center>:</div></td>
	<td class="tabdata">
	<SELECT NAME="DvInfo_PVC_Ext" SIZE="1" onChange="doSave()">
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
	</SELECT></td></tr></tr>
<%end if%>

<%end if%>
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then%>
	<tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
	<font color="#000000">Service</font></div></td><td class="tabdata"><div align=center>:</div></td>
	<td class="tabdata">
	<SELECT NAME="DvInfo_PVC_Ext" SIZE="1" onChange="doSave()">
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
	</SELECT></td></tr></tr>
<%end if%>
<%end if%>
<%end if%>
<tr><td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Connection Type</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<INPUT TYPE="HIDDEN" NAME="DipConnFlag" VALUE="0">
        <% if tcWebApi_get("DeviceInfo_PVC","connType","h") <> "" then tcWebApi_staticGet("DeviceInfo_PVC","connType","s")  end if %>
        &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp 
		<%If tcWebApi_staticGet("DeviceInfo_PVC","DispConnBtnType","h") <> "0" then%>
        	<%If tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "1" Then%>
        	<input type="button" name="Disconnect" value="Disconnect" onClick="reconnect(2)" >
        	<%elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "1" Then%>
        	<input type="button" name="Disconnect" value="Disconnect" onClick="reconnect(2)" > 
        	<%else %>
        	<INPUT TYPE="button" NAME="Connect" VALUE="Connect" onClick="reconnect(1)" >  		
			<%End if%>
		<%End if%>
</td></tr>
<%If tcWebApi_staticGet("DeviceInfo_PVC","DispPPPconn","h") <> "0" then%>
<tr><td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">PPP connection time</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","pppConnTime","s")%>	
</td></tr>
<%end if%>
<tr>
		<td class="title-sub"><font color="#000000">IPv4</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>

<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Status</font>    </div></td><td class="tabdata" ><div align=center>:</div></td><td class="tabdata">
<%
if tcWebApi_get("GUI_Entry0","Custom","h") = "C9" then
    If tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "3" Then
	asp_Write("ADSL CONNECTION FAILURE")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "5" Then 
	asp_Write("PPP up") 
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "4" Then  
	asp_Write("PPP down")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "2" Then
	asp_Write("PPP USERNAME/PASSWORD INVALID")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "1" Then
	asp_Write("Connected")
    else asp_Write("Not Connected") 
    end if
else
    If tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "1" Then asp_Write("Connected") else asp_Write("Not Connected") end if
end if
%>
</td></tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<table cellpadding="0" cellspacing="0" border="0"><tr><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","WanIP","s")%>  &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp </td>		
<td><%If tcWebApi_staticGet("DeviceInfo_PVC","DispBtnType","h") <> "0" then%>
<INPUT TYPE="button" NAME="renewIP" VALUE="renewIP" onClick="renewrelease(1)" >
<INPUT TYPE="button" NAME="releaseIP" VALUE="releaseIP" onClick="renewrelease(2)" >  		
<%End if%></td></tr></table>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Subnet Mask</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","WanSubMask","s")%>	    
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<%if tcWebApi_get("WebCustom_Entry","isWanDNSEachPVC","h") = "Yes" then%>
<font color="#000000">Gateway</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%  tcWebApi_staticGet("DeviceInfo_PVC","gateway","s") %>
<%else%>
<font color="#000000">Default Gateway</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%  tcWebApi_staticGet("DeviceInfo_PVC","WanDefGW","s") %> 
<%end if%>
</td></tr>
<%if tcwebApi_get("WebCustom_Entry","isWanDNSEachPVC","h") = "Yes" then%>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Primary DNS</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%  tcWebApi_staticGet("DeviceInfo_PVC","DNSIP","s") %>	
</td></tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Secondary DNS</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<% tcWebApi_staticGet("DeviceInfo_PVC","SECDNSIP","s") %>	
</td></tr>
<% else %>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">
<%if tcwebApi_get("WebCustom_Entry","isShow2DNS","h") = "Yes" then%>
Pri DNS Server
<% else %> 
DNS Server
<% end if %>
</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<script language="JavaScript" type="text/JavaScript">
var artIpver = ["<% tcWebApi_staticGet("Wan_PVC0","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC1","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC2","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC3","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC4","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC5","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC6","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC7","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC8","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC9","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC10","IPVERSION","s") %>"];
	var pvc_index = parseInt(document.DvInfo_Form.DvInfo_PVC.value);
	var strtype = "<% tcWebApi_get("Dproxy_Entry","type","s") %>";
	var strPriDNS = "<% tcWebApi_staticGet("Dproxy_Entry","Primary_DNS","s") %>";
	var strDevDNS = "<% tcWebApi_staticGet("DeviceInfo_PVC","DNSIP","s") %>";
	
	if (("IPv6" == artIpver[pvc_index]) || ("N/A" == artIpver[pvc_index]))
		document.writeln("N/A");
	else{
		if (strtype == "1") {
			document.writeln(strPriDNS);
		}
		else{
			document.writeln(strDevDNS);
		}	
	}	
</script>
</td></tr>
<%if tcwebApi_get("WebCustom_Entry","isShow2DNS","h") = "Yes" then%>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">SEC DNS Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<script language="JavaScript" type="text/JavaScript">

	var strSecDNS = "<% tcWebApi_staticGet("Dproxy_Entry","Secondary_DNS","s") %>";
	var strSecDevDNS = "<% tcWebApi_staticGet("DeviceInfo_PVC","SECDNSIP","s") %>";
	
	if (("IPv6" == artIpver[pvc_index]) || ("N/A" == artIpver[pvc_index]))
		document.writeln("N/A");
	else{
		if (strtype == "1") {
			document.writeln(strSecDNS);
		}
		else{
			document.writeln(strSecDevDNS);
		}	
	}	
</script>
</td></tr>
<%end if%>
<%end if%>
<%if tcWebApi_get("DeviceInfo", "isSFU", "h") <> "Yes" then%>
<% if tcWebApi_staticGet("Info_Ether","isIPv6Supported","h") = "Yes" then %>
<tr>
		<td class="title-sub"><font color="#000000">IPv6</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>

<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Status</font>    </div></td><td class="tabdata" ><div align=center>:</div></td><td class="tabdata">
<%
if tcWebApi_staticGet("GUI_Entry0","Custom","h") = "C9" then
    If tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "3" Then
	asp_Write("ADSL CONNECTION FAILURE")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "5" Then 
	asp_Write("PPP up") 
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "4" Then  
	asp_Write("PPP down")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "2" Then
	asp_Write("PPP USERNAME/PASSWORD INVALID")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "1" Then
	asp_Write("Connected")
    else asp_Write("Not Connected") 
    end if
else
    If tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "1" Then asp_Write("Connected") else asp_Write("Not Connected") end if
end if
%>
</td>
</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<table cellpadding="0" cellspacing="0" border="0"><tr><td class="tabdata">
<script language="JavaScript" type="text/JavaScript">
	var str_IP6 = "<%tcWebApi_get("DeviceInfo_PVC","IP6WanIP","s")%>";
	if("N/A" != str_IP6){
		var str_ip6value = str_IP6;
		var vlen = str_IP6.indexOf('/');
		if(vlen != -1){
			str_ip6value = str_IP6.substring(0, vlen);
		}
		document.writeln(str_ip6value);
	}
	else{
		document.writeln(str_IP6);
	}
</script>  &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp </td>		
<td><%If tcWebApi_staticGet("DeviceInfo_PVC","IP6DispBtnType","h") <> "0" then%>
<INPUT TYPE="button" NAME="renewIP6" VALUE="renewIP6" onClick="renewrelease(3)" >
<INPUT TYPE="button" NAME="releaseIP6" VALUE="releaseIP6" onClick="renewrelease(4)">  		
<%End if%></td></tr></table>
</td></tr>
<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
<% If tcWebApi_staticGet("DeviceInfo_PVC","IP6SecWanIP","h") <> "N/A" Then %>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">IP Second Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<script language="JavaScript" type="text/JavaScript">
	var str_IP6 = "<%tcWebApi_get("DeviceInfo_PVC","IP6SecWanIP","s")%>";
	if("N/A" != str_IP6){
		var str_ip6value = str_IP6;
		var vlen = str_IP6.indexOf('/');
		if(vlen != -1){
			str_ip6value = str_IP6.substring(0, vlen);
		}
		document.writeln(str_ip6value);
	}
	else{
		document.writeln(str_IP6);
	}
</script>  &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp </td></tr>
<%end if%>
<%End if%>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Prefix Length</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<script language="JavaScript" type="text/JavaScript">
	if("N/A" != str_IP6){
		var str_prelen = "64";
		var plen = str_IP6.indexOf('/');
		if(plen != -1){
			str_prelen = str_IP6.substring(1+plen, 3+plen);
		}
		document.writeln(str_prelen);
	}
	else{
		document.writeln(str_IP6);
	}
</script>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Default Gateway</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6WanDefGW","s")%>	
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">DNS Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6DNSIP","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Prefix Delegation</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6PreDelegation","s")%>	
</td></tr>
<% end if %>
</table>
<%end if%>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF" class="tabdata">
<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" then%>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">xPON</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td>
		<td colspan="3"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
			<font color="#000000">xPON Firmware Ver</font></div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3">
			<%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "1" Then  tcWebApi_staticGet("GPON_ONU","Version","s")
			elseif tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" then  tcWebApi_staticGet("EPON_ONU","FWVer","s")
			Else%> N/A
			<%end if%>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Line State</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3"><%tcWebApi_staticGet("XPON_Common","trafficStatus","s")%></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000">xPON Mode</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3">
			<%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "1" Then%>GPON
			<%elseif tcWebApi_get("XPON_LinkCfg","LinkSta","h") = "2" Then%>EPON
			<%Else%> Auto Detect
			<%end if%>
		</td>
	</tr>
	<tr>
		<td class="title-sub"><font color="#000000">Transceiver</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000">Rx Power</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3">
		<script language="JavaScript">
			<%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") <> "0"  Then%>
				document.write((Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "RxPower", "s")%>))/10000)/(Math.log(10))*100)/10)+" dBm");
			<%else %>document.write("N/A");
			<%End If%>
		</script>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000">Tx Power</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3">
		<script language="JavaScript">
			<%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") <> "0"  Then%>
				document.write((Math.round(Math.log((Number(<%tcWebApi_get("Info_PonPhy", "TxPower", "s")%>))/10000)/(Math.log(10))*100)/10)+" dBm");
			<%else %>document.write("N/A");
			<%End If%>
		</script>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000">TxBias</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3">
		<script language="JavaScript">
			<%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") <> "0"  Then%>
				document.write((Number(<%tcWebApi_get("Info_PonPhy", "TxBiasCurrent", "s")%>)*2/1000)+" mA");
			<%else %>document.write("N/A");
			<%End If%>
		</script>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000">SupplyVoltage</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3">
		<script language="JavaScript">
			<%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") <> "0"  Then%>
				document.write((Number(<%tcWebApi_get("Info_PonPhy", "SupplyVoltage", "s")%>)/10000)+" V");
			<%else %>document.write("N/A");
			<%End If%>
		</script>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000">Temperature</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata" colspan="3">
			<script language="JavaScript">
				<%if tcWebApi_get("XPON_LinkCfg","LinkSta","h") <> "0"  Then %>
					document.write(transTemperature(<%tcWebApi_get("Info_PonPhy", "Temperature", "s")%>)+" C");					
				<%else%>document.write("N/A")
				<%End If%>
			</script>
		</td>
	</tr>
<%else%>
<% if tcWebApi_get("WebCurSet_Entry","dev_pvc","h") <> "10" then%>
<tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF"><%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>x<%else%>A<%end if%>DSL</font></td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td colspan="3"><hr noshade></td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000"><%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>x<%else%>A<%end if%>DSL Firmware Ver</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%If tcWebApi_get("Info_Adsl","fwVer","h") <> "N/A" then tcWebApi_staticGet("Info_Adsl","fwVer","s") end if%>     
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Line State</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Modulation</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%tcWebApi_staticGet("Info_Adsl","Opmode","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Annex Mode</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%tcWebApi_get("Info_Adsl","AdslType","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata" colspan="3"><hr noshade class="light-orange-line"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right></div></td><td class="tabdata"></td><td width="80" align=center class="tabdata">
<font color="#000000">Downstream</font></td><td width="80" align=center class="tabdata">
<font color="#000000">Upstream</font></td><td width="280">&nbsp;</td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">SNR Margin</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","SNRMarginDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","SNRMarginUp","s")%>
</td><td class="tabdata"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Line Attenuation</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","AttenDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","AttenUp","s")%>
</td><td width="280" class="tabdata"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Data Rate</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","DataRateDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","DataRateUp","s")%>
</td><td width="280" class="tabdata"></td></tr><tr>
<% if tcWebApi_staticGet("WebCustom_Entry","isAdslShowMaxRate","h") = "Yes" then %>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">MAX Rate</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","AttainDown","s")%>&nbsp;kbps
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","AttainUp","s")%>&nbsp;kbps
</td><td width="280" class="tabdata"></td></tr>
<%end if%>
<% if tcWebApi_staticGet("WebCustom_Entry","isAdslShowTxpower","h") = "Yes" then %>
<tr><td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">POWER</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","PowerDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","PowerUp","s")%>
</td><td width="280" class="tabdata"></td></tr>
<%end if%>
<% if tcWebApi_staticGet("WebCustom_Entry","isAdslShowCRC","h") = "Yes" then %>
<tr><td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">CRC</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","CRCDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","CRCUp","s")%>
</td><td width="280" class="tabdata"></td></tr>
<%end if%>
<% if tcWebApi_get("WebCustom_Entry","isSNMPSupported","h") = "Yes" then %>
<tr><td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">ES</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_get("Info_Showtime","ESDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_get("Info_Showtime","ESUp","s")%>
</td><td class="tabdata"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">SES</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Showtime","SESDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Showtime","SESUp","s")%>
</td><td class="tabdata"></td></tr>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") <> "Yes" then%>
<tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">UAS</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Showtime","UASDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Showtime","UASUp","s")%>
</td><td class="tabdata"></td></tr>
<%end if%>
<% end if %>
<%end if%>
<%end if%>
<tr><td class="light-orange"></td><td class="light-orange"></td><td width="150">&nbsp;</td><td width="10">&nbsp;</td><td colspan="3" class="tabdata">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<tr>
<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">&nbsp;</td></tr></table></form>    
</BODY>
</HTML>
