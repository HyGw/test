<%

IF Request_Form("LineFlag")="1" Then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
End if


If request_Form("VoIP_advanced_flag")="1" then

	if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
	if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then
		if tcWebApi_get("WebCustom_Entry", "voip_interface", "h") = "8" then
			tcWebAPI_set("VoIPAdvanced_Common", "pvcId_serv", "voipPVC_serv")
		elseif tcWebApi_get("WebCustom_Entry", "voip_interface", "h") = "9" then
			tcWebAPI_set("VoIPAdvanced_Common", "pvcId_serv", "voipPVC_serv")
		elseif tcWebApi_get("WebCustom_Entry", "voip_interface", "h") = "10" then
			tcWebAPI_set("VoIPAdvanced_Common", "pvcId_serv", "voipPVC_serv")
		end if
	end if
	end if

	tcWebApi_set("VoIPAdvanced_Common","BindingEnable","voipBindingEnable")
	tcWebApi_set("VoIPAdvanced_Common","pvcId","voipPVC")
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
	tcWebApi_set("VoIPAdvanced_Entry","VoiceVolumeListen","VoIPVoiceVolumeListen")
	tcWebApi_set("VoIPAdvanced_Entry","VoiceVolumeSpeak","VoIPVoiceVolumeSpeak")
	tcWebApi_set("VoIPAdvanced_Common","FaxCodec","VoIPFaxCodec")
	tcWebApi_set("VoIPAdvanced_Common","DTMFTransportMode","VoIPDTMFTransportMode")
	tcWebApi_set("VoIPAdvanced_Common","VoIPRegion","Region")
	tcWebApi_set("VoIPAdvanced_Common","RegistrationExpire","VoIPRegistrationExpire")
	tcWebApi_set("VoIPSysParam_Common","SC_SYS_SIP_REREG_TIME","VoIPReRegTime")
	tcWebApi_commit("VoIPAdvanced")
end if

if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then
if request_Form("VoIP_advanced_flag")="2" then
	TCWebAPI_set("WebCustom_Entry","voip_interface","voipPVC")
end if
end if
end if

%>


<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" src="/wanfunc.js"></script>
<script language="JavaScript" src="/chksel.js"></script>
<script language="JavaScript">
function check_volume(vol){
	
	if((Number(vol) < -15 ) || (Number(vol) > 15 ) || isNaN(Number(vol))){
		alert('Exceeding the range ' + vol);
		return false;
	}
	return true;
}


function doLineChange(){
	document.VOIP_ADVANCED_form.LineFlag.value = 1;
	document.VOIP_ADVANCED_form.submit();
}

function bindingOn(on){
	var form=document.VOIP_ADVANCED_form;
	form.voipPVC.disabled=on;

}


function onLoadFun(){
	var form=document.VOIP_ADVANCED_form;
	var index = 0;

<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") = "No" then%>
		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") = "No" then%>
		

	form.VoIPVoiceVolumeListen.disabled=true;
	form.VoIPVoiceVolumeSpeak.disabled=true;
	form.Line.disabled=true;

	
<%end if%>
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
index = index+7;
<% if TCWebAPI_get("Wan_PVC0", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[0].disabled=true;
<%end if%>
<% if TCWebAPI_get("Wan_PVC1", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[1].disabled=true;
<%end if%>
<% if TCWebAPI_get("Wan_PVC2", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[2].disabled=true;
<%end if%>
<% if TCWebAPI_get("Wan_PVC3", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[3].disabled=true;
<%end if%>
<% if TCWebAPI_get("Wan_PVC4", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[4].disabled=true;
<%end if%>
<% if TCWebAPI_get("Wan_PVC5", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[5].disabled=true;
<%end if%>
<% if TCWebAPI_get("Wan_PVC6", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[6].disabled=true;
<%end if%>
<% if TCWebAPI_get("Wan_PVC7", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[7].disabled=true;
<%end if%>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
index++;
<% if TCWebAPI_get("WanExt_PVC8e0", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC8e1", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC8e2", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC8e3", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC8e4", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC8e5", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC8e6", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC8e7", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[index].disabled=true;
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
index++;
<% if TCWebAPI_get("WanExt_PVC9e0", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC9e1", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC9e2", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC9e3", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC9e4", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC9e5", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC9e6", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC9e7", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[index].disabled=true;
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
index++;
<% if TCWebAPI_get("WanExt_PVC10e0", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC10e1", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC10e2", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC10e3", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC10e4", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC10e5", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC10e6", "Active", "h") <> "Yes" then%>
<% if TCWebAPI_get("WanExt_PVC10e7", "Active", "h") <> "Yes" then%>
	form.voipPVC.options[index].disabled=true;
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>
<%end if%>

<% if TCWebAPI_get("WebCustom_Entry","voip_interface", "h") = "8" then%>

	<% if TCWebAPI_get("WanExt_PVC8e0", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[0].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC8e1", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[1].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC8e2", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[2].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC8e3", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[3].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC8e4", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[4].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC8e5", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[5].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC8e6", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[6].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC8e7", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[7].disabled=true;
	<%end if%>	

<%end if%>	

<% if TCWebAPI_get("WebCustom_Entry","voip_interface", "h") = "9" then%>

	<% if TCWebAPI_get("WanExt_PVC9e0", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[0].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC9e1", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[1].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC9e2", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[2].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC9e3", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[3].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC9e4", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[4].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC9e5", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[5].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC9e6", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[6].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC9e7", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[7].disabled=true;
	<%end if%>

<%end if%>	

<% if TCWebAPI_get("WebCustom_Entry","voip_interface", "h") = "10" then%>

	<% if TCWebAPI_get("WanExt_PVC10e0", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[0].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC10e1", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[1].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC10e2", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[2].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC10e3", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[3].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC10e4", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[4].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC10e5", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[5].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC10e6", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[6].disabled=true;
	<%end if%>
	<% if TCWebAPI_get("WanExt_PVC10e7", "Active", "h") <> "Yes" then%>
		form.voipPVC_serv.options[7].disabled=true;
	<%end if%>

<%end if%>	

	if(form.voipBindingEnable[0].checked)
		bindingOn(false);
	else
		bindingOn(true);


}

function do_check(){
	var form=document.VOIP_ADVANCED_form;
	var volumeTx = form.VoIPVoiceVolumeListen.value;
	var volumeRx = form.VoIPVoiceVolumeSpeak.value;	
	var registraExpire = form.VoIPRegistrationExpire.value;
	var reRegistraTime = form.VoIPReRegTime.value;


	if(volumeTx.length <= 0){
		alert("please input Listening Volume");
		return false;
	}		
	if(check_volume(volumeTx) == false){
		return false;
	}
	
	if(volumeRx.length <= 0){
		alert("please input Speaking Volume");
		return false;
	}		
	if(check_volume(volumeRx) == false){
		return false;
	}
	
	if(registraExpire.length <= 0){
		alert("please input Registration Expire");
		return false;
	}

	if((Number(registraExpire) < 60 ) || (Number(registraExpire) > 65536 ) || isNaN(Number(registraExpire))){
		alert('Value of registration expire ' + registraExpire + ' exceeds the range');
		return false;
	}
	
	if((Number(reRegistraTime) < 50 ) || (Number(reRegistraTime) > 65536 ) || isNaN(Number(reRegistraTime))){
		alert('Value of reRegistration time ' + reRegistraTime + ' exceeds the range');
		return false;
	}

	if(Number(reRegistraTime) > ((Number(registraExpire)*9)/10)){
		alert('reRegistration time should not bigger than 0.9*Expires time');
		return false;
	}
	
	return true;
}

function dosave(){
	if(do_check()== false){
		return;	
	}
	document.VOIP_ADVANCED_form.VoIP_advanced_flag.value=1;
	document.VOIP_ADVANCED_form.submit();
}

function doVCChange(){
	document.VOIP_ADVANCED_form.VoIP_advanced_flag.value=2;
	document.VOIP_ADVANCED_form.submit();
}

</script>

</head><body onLoad="onLoadFun()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_advanced.asp" name="VOIP_ADVANCED_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">VoIP Advanced</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>
	<INPUT TYPE="HIDDEN" NAME="VoIP_advanced_flag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="LineFlag" VALUE="0">


	<tr>
		<td class="light-orange" style="height: 22px"></td>
		<td class="light-orange" style="height: 22px"></td>
		<td class="tabdata" align=right style="height: 22px"><font color="#000000">VoIP WAN Binding</font></td>
		<td class="tabdata" align=center style="height: 22px">:</td>
		<td class="tabdata" style="height: 22px">
			<INPUT TYPE="RADIO" NAME="voipBindingEnable" VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "BindingEnable", "h") = "1" then asp_write("checked") end if %> onclick="bindingOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="voipBindingEnable" VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "BindingEnable", "h") <> "1" then asp_write("checked") end if%> onclick="bindingOn(true)"><font color="#000000">Disable</font>
		</td></tr>
		
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<%if TCWebAPI_get("VoIPAdvanced_Common", "SIPBinding", "h") = "Wan" then%>
			<td class="tabdata" align=right><font color="#000000">Binding interface</font></td>
			<td class="tabdata" align=center>:</td>
			<td  class="tabdata">
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>			
			<SELECT NAME="voipPVC" SIZE="1" onchange="doVCChange();">
<%else%>			
			<SELECT NAME="voipPVC" SIZE="1">
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
					<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "0" then  asp_Write("selected") end if %>>PVC0
					<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "1" then  asp_Write("selected") end if %>>PVC1
					<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "2" then  asp_Write("selected") end if %>>PVC2
					<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "3" then  asp_Write("selected") end if %>>PVC3
					<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "4" then  asp_Write("selected") end if %>>PVC4
					<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "5" then  asp_Write("selected") end if %>>PVC5
					<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "6" then  asp_Write("selected") end if %>>PVC6
					<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "7" then  asp_Write("selected") end if %>>PVC7
<%else%>

<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>

	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
					<OPTION VALUE="0" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "0" then  asp_Write("selected") end if %>>PVC0
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "1" then  asp_Write("selected") end if %>>PVC1
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "2" then  asp_Write("selected") end if %>>PVC2
					<OPTION VALUE="3" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "3" then  asp_Write("selected") end if %>>PVC3
					<OPTION VALUE="4" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "4" then  asp_Write("selected") end if %>>PVC4
					<OPTION VALUE="5" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "5" then  asp_Write("selected") end if %>>PVC5
					<OPTION VALUE="6" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "6" then  asp_Write("selected") end if %>>PVC6
					<OPTION VALUE="7" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "7" then  asp_Write("selected") end if %>>PVC7
	<%end if%>
	
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
					<OPTION VALUE="8" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "8" then  asp_Write("selected") end if %>>PTM0
					<OPTION VALUE="9" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "9" then  asp_Write("selected") end if %>>PTM1
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
					<OPTION VALUE="10" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "10" then  asp_Write("selected") end if %>>WAN0
	<%end if%>

<%else%>
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
					<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "0" then  asp_Write("selected") end if %>>PVC0
					<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "1" then  asp_Write("selected") end if %>>PVC1
					<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "2" then  asp_Write("selected") end if %>>PVC2
					<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "3" then  asp_Write("selected") end if %>>PVC3
					<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "4" then  asp_Write("selected") end if %>>PVC4
					<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "5" then  asp_Write("selected") end if %>>PVC5
					<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "6" then  asp_Write("selected") end if %>>PVC6
					<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "7" then  asp_Write("selected") end if %>>PVC7
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
					<OPTION VALUE="8" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "8" then  asp_Write("selected") end if %>>PTM0
					<OPTION VALUE="9" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "9" then  asp_Write("selected") end if %>>PTM1
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
					<OPTION VALUE="10" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "10" then  asp_Write("selected") end if %>>WAN0
	<%end if%>

<%end if%>
<%end if%>
				</SELECT>
			</td>
		<%end if%>
	</tr>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>	
	<%if TCWebAPI_get("VoIPAdvanced_Common", "SIPBinding", "h") = "Wan" then%>
	<%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "8" then %>
	<tr>
        <td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Service</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
		<SELECT NAME="voipPVC_serv" SIZE="1">
		<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "0" then  asp_Write("selected") end if %>>0
		<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "1" then  asp_Write("selected") end if %>>1
		<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "2" then  asp_Write("selected") end if %>>2
		<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "3" then  asp_Write("selected") end if %>>3
		<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "4" then  asp_Write("selected") end if %>>4
		<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "5" then  asp_Write("selected") end if %>>5
		<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "6" then  asp_Write("selected") end if %>>6
		<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "7" then  asp_Write("selected") end if %>>7
		</SELECT>
		</td>
	<%end if%>
	<%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "9" then %>
	<tr>
        <td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Service</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
		<SELECT NAME="voipPVC_serv" SIZE="1">
		<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "0" then  asp_Write("selected") end if %>>0
		<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "1" then  asp_Write("selected") end if %>>1
		<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "2" then  asp_Write("selected") end if %>>2
		<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "3" then  asp_Write("selected") end if %>>3
		<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "4" then  asp_Write("selected") end if %>>4
		<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "5" then  asp_Write("selected") end if %>>5
		<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "6" then  asp_Write("selected") end if %>>6
		<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "7" then  asp_Write("selected") end if %>>7
		</SELECT>
		</td>
	<%end if%>
	<%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "10" then %>
	<tr>
        <td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Service</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
		<SELECT NAME="voipPVC_serv" SIZE="1">
		<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "0" then  asp_Write("selected") end if %>>0
		<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "1" then  asp_Write("selected") end if %>>1
		<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "2" then  asp_Write("selected") end if %>>2
		<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "3" then  asp_Write("selected") end if %>>3
		<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "4" then  asp_Write("selected") end if %>>4
		<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "5" then  asp_Write("selected") end if %>>5
		<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "6" then  asp_Write("selected") end if %>>6
		<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "7" then  asp_Write("selected") end if %>>7
		</SELECT>
		</td>
	<%end if%>
	<%end if%>
		</tr>
<%end if%>			
	<tr>
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr> 
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Region</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Region" SIZE="1">
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "BEL-Belgium" then  asp_Write("selected") end if %>>BEL-Belgium
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "BRA-Brazil" then  asp_Write("selected") end if %>>BRA-Brazil
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "CHL-Chile" then  asp_Write("selected") end if %>>CHL-Chile
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "CHN-China" then  asp_Write("selected") end if %>>CHN-China
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "DNK-Denmark" then  asp_Write("selected") end if %>>DNK-Denmark
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "ESP-Spain" then  asp_Write("selected") end if %>>ESP-Spain
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "ETS-ETSI" then  asp_Write("selected") end if %>>ETS-ETSI
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "FIN-Finland" then  asp_Write("selected") end if %>>FIN-Finland
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "FRA-France" then  asp_Write("selected") end if %>>FRA-France
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "DEU-Germany" then  asp_Write("selected") end if %>>DEU-Germany
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "HUN-Hungary" then  asp_Write("selected") end if %>>HUN-Hungary
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "ITA-Italy" then  asp_Write("selected") end if %>>ITA-Italy
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "JPN-Japan" then  asp_Write("selected") end if %>>JPN-Japan
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "NLD-Netherlands" then  asp_Write("selected") end if %>>NLD-Netherlands
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "USA-United States" then asp_Write("selected") end if %>>USA-United States
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "SWE-Sweden" then asp_Write("selected") end if %>>SWE-Sweden
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "CHE-Switzerland" then asp_Write("selected") end if %>>CHE-Switzerland
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "GBR-Great Britain" then asp_Write("selected") end if %>>GBR-Great Britain
			</SELECT>
		</td></tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right><img src="/exclamation.gif"></div></td><td>&nbsp;</td><td class="tabdata">
<font color="#000000"> You will need to save the configuration of Region to flash and restart system for this change to take effect.</font></td></tr><tr>




	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">DTMF Transport Mode</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="VoIPDTMFTransportMode" SIZE="1">
				<OPTION value="Inband"<%if TCWebAPI_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "Inband" then asp_Write("selected") end if %>>Inband
				<OPTION value="RFC2833"<%if TCWebAPI_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "RFC2833" then asp_Write("selected") end if %>>RFC2833
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>			
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "Via SIP" then asp_Write("selected") end if %>>Via SIP
<% end if %>
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">FAX</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="VoIPFaxCodec" SIZE="1">
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "FaxCodec", "h") = "T38" then asp_Write("selected") end if %>>T38
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "FaxCodec", "h") = "PCMA" then asp_Write("selected") end if %>>PCMA
				<OPTION <%if TCWebAPI_get("VoIPAdvanced_Common", "FaxCodec", "h") = "PCMU" then asp_Write("selected") end if %>>PCMU
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange" style="height: 23px"></td>
		<td class="light-orange" style="height: 23px"></td>
		<td class="tabdata" align=right style="height: 23px"><font color="#000000">Registration Expire</font></td>
		<td class="tabdata" align=center style="height: 23px">:</td>
		<td class="tabdata" style="height: 23px">
			<INPUT TYPE="TEXT" NAME="VoIPRegistrationExpire" SIZE="10" MAXLENGTH="10" VALUE="<%if TCWebAPI_get("VoIPAdvanced_Common", "RegistrationExpire", "h") <> "N/A" then TCWebAPI_get("VoIPAdvanced_Common", "RegistrationExpire", "s") else asp_Write("0") end if %>">
		sec.(60~65536)</td></tr>
	<tr>
		<td class="light-orange" style="height: 23px"></td>
		<td class="light-orange" style="height: 23px"></td>
		<td class="tabdata" align=right style="height: 23px"><font color="#000000">reRegistration Time</font></td>
		<td class="tabdata" align=center style="height: 23px">:</td>
		<td class="tabdata" style="height: 23px">
			<INPUT TYPE="TEXT" NAME="VoIPReRegTime" SIZE="10" MAXLENGTH="10" VALUE="<%if TCWebAPI_get("VoIPSysParam_Common", "SC_SYS_SIP_REREG_TIME", "h") <> "N/A" then TCWebAPI_get("VoIPSysParam_Common", "SC_SYS_SIP_REREG_TIME", "s") else asp_Write("0") end if %>">
		sec.(50~65536)</td></tr>

	<tr>
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">Line</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<select NAME="Line" SIZE="1" onChange="doLineChange()" >
		<OPTION value="0" <%if TCWebApi_get("WebCurSet_Entry","LineNum","h") = "0" then asp_write("selected") end if%> <% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%> disabled=true <%end if%>>1
<% if TCWebAPI_get("VoIPBasic_Common", "VoIPLine2Enable", "h") = "Yes" then%>
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","LineNum","h") = "1" then asp_write("selected") end if%> <% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%> disabled=true <%end if%>>2
<%end if%>


	</select>
		</td>
	</tr>


		<tr>
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td></tr>


	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Listening Volume</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPVoiceVolumeListen" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeListen", "h") <> "N/A" then TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeListen", "s") else asp_Write("0") end if %>">
		<font color="#000000">db (-15~15)</font>
		</td>
		</tr>			
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Speaking Volume</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPVoiceVolumeSpeak" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeSpeak", "h") <> "N/A" then TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeSpeak", "s") else asp_Write("0") end if %>">
			<font color="#000000">db (-15~15)</font>
			</td>
		</tr>	
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="dosave();"></td></tr></table>
</form></body></html>
