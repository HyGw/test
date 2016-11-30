<%

IF Request_Form("LineFlag")="1" Then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
End if

if request_Form("VoIP_call_ctrl_flag")="1" then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
	tcWebApi_set("VoIPCallCtrl_Entry","AnonymousCallBlock","Anonymous_call_blocking")
	tcWebApi_set("VoIPCallCtrl_Entry","AnonymousCall","Anonymous_calling")

	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallerIdEnable","VoIPCallerId")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallWaitingReply","VoIPCallWaitingReply")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallWaitingEnable","VoIPCallWaiting")
	
	tcWebApi_set("VoIPCallCtrl_Entry","MTKUCFEnable","MTKUCF")
	tcWebApi_set("VoIPCallCtrl_Entry","MTKBCFEnable","MTKBCF")
	tcWebApi_set("VoIPCallCtrl_Entry","MTKNCFEnable","MTKNCF")
	tcWebApi_set("VoIPCallCtrl_Entry","MTKSIPUCFNumber","MTKVoIPUCFNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","MTKSIPBCFNumber","MTKVoIPBCFNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","MTKSIPNCFNumber","MTKVoIPNCFNumber")
	
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallTransfer","VoIPCallTransfer")

	tcWebApi_set("VoIPCallCtrl_Entry","SIPBlindTransferNumber","VoIPBlindTransferNumber")


	tcWebApi_set("VoIPCallCtrl_Entry","SIPAttendedTransferNumber","VoIPAttendedTransferNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallHold","VoIPCallHold")

	tcWebApi_set("VoIPCallCtrl_Entry","SIP3wayConf","VoIP3wayConf")
	tcWebApi_set("VoIPCallCtrl_Entry","SIP3wayConfNumber","VoIP3wayConfNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPMWIEnable","VoIPMWIEnable")

	tcWebApi_set("VoIPCallCtrl_Entry","SIPDNDEnable","VoIPDNDEnable")



	tcWebApi_commit("VoIPCallCtrl_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript">
function checkDigit(digit) {
	if(((digit >= 48) && (digit <= 57)) || (digit == 42)) {
		return 0;
	} else {
		return -1;
	}
}

function dosave(){
	var form=document.VOIP_CALL_CTRL_form;
	var i;
	var j;
	var tmp;
	var count;
	var numbers = new Array(20);

	count=0;
	if(form.MTKUCF[0].checked) {
		if(form.MTKVoIPUCFNumber.value.length <= 0){
			alert("Please insert Unconditional Forward Number");
			return;
		} else {
			for(i = 0; i < form.MTKVoIPUCFNumber.value.length; i++) {
				tmp = form.MTKVoIPUCFNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Unconditional Forward Number is invalid");
					return;
				}
			}
			numbers[count]=form.MTKVoIPUCFNumber.value;
			count++;
			
		}
	}
	if(form.MTKBCF[0].checked) {
		if(form.MTKVoIPBCFNumber.value.length <= 0){
			alert("Please insert Busy Forward Number");
			return;
		} else {
			for(i = 0; i < form.MTKVoIPBCFNumber.value.length; i++) {
				tmp = form.MTKVoIPBCFNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Busy Forward Number is invalid");
					return;
				}
			}
			numbers[count]=form.MTKVoIPBCFNumber.value;
			count++;
		}
	}
	if(form.MTKNCF[0].checked) {
		if(form.MTKVoIPNCFNumber.value.length <= 0){
			alert("Please insert No Reply Forward Number");
			return;
		} else {
			for(i = 0; i < form.MTKVoIPNCFNumber.value.length; i++) {
				tmp = form.MTKVoIPNCFNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("No Reply Forward Number is invalid");
					return;
				}
			}
			numbers[count]=form.MTKVoIPNCFNumber.value;
			count++;
		}
	}
	if(form.VoIPCallTransfer[0].checked) {

		if(form.VoIPBlindTransferNumber.value.length <= 0){
			alert("Please insert Blind Transfer Number");
			return;
		} else {
			for(i = 0; i < form.VoIPBlindTransferNumber.value.length; i++) {
				tmp = form.VoIPBlindTransferNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Blind Transfer Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPBlindTransferNumber.value;
			count++;
		}

		if(form.VoIPAttendedTransferNumber.value.length <= 0){
			alert("Please insert Attended Transfer Number");
			return;
		} else {
			for(i = 0; i < form.VoIPAttendedTransferNumber.value.length; i++) {
				tmp = form.VoIPAttendedTransferNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Attended Transfer Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPAttendedTransferNumber.value;
			count++;
		}
	}else {
		form.VoIPAttendedTransferNumber.value="";
		form.VoIPBlindTransferNumber.value="";
	}
	if(form.VoIP3wayConf[0].checked) {
		if(form.VoIP3wayConfNumber.value.length <= 0){
			alert("Please insert Conference Number");
			return;
		} else {
			for(i = 0; i < form.VoIP3wayConfNumber.value.length; i++) {
				tmp = form.VoIP3wayConfNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Conference Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIP3wayConfNumber.value;
			count++;
		}
	}else {
		form.VoIP3wayConfNumber.value="";
	}


	for(i = 0; i < count; i++){
		for(j = 0; j < i; j++){
			if(numbers[i]==numbers[j]){
				alert("Duplicate Number is invalid");
				return;
			}

		}

	}


 	form.VoIP_call_ctrl_flag.value=1;
	form.submit();
}

function doLineChange(){
	document.VOIP_CALL_CTRL_form.LineFlag.value = 1;
	document.VOIP_CALL_CTRL_form.submit();
}


function UforwardOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.MTKVoIPUCFNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.MTKVoIPUCFNumber.disabled=true;
<%end if%>	
<%end if%>
}

function BforwardOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.MTKVoIPBCFNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.MTKVoIPBCFNumber.disabled=true;
<%end if%>	
<%end if%>
}

function NforwardOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.MTKVoIPNCFNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.MTKVoIPNCFNumber.disabled=true;
<%end if%>	
<%end if%>
}

<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
function waitingOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIPCallWaitingReply[0].disabled=on;
	form.VoIPCallWaitingReply[1].disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIPCallWaitingReply[0].disabled=true;
	form.VoIPCallWaitingReply[1].disabled=true;
<%end if%>	
<%end if%>
}
<%end if%>

function transferOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIPBlindTransferNumber.disabled=on;

	form.VoIPAttendedTransferNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIPBlindTransferNumber.disabled=true;
	form.VoIPAttendedTransferNumber.disabled=true;
<%end if%>	
<%end if%>
}

function conferenceOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIP3wayConfNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIP3wayConfNumber.disabled=true;
<%end if%>	
<%end if%>
}

function onLoadFun(){
	var form=document.VOIP_CALL_CTRL_form;

<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>
		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>		
	form.Anonymous_call_blocking[0].disabled=true;
	form.Anonymous_call_blocking[1].disabled=true;
	form.Anonymous_calling[0].disabled=true;
	form.Anonymous_calling[1].disabled=true;


	form.VoIPCallerId[0].disabled=true;

	form.MTKUCF[0].disabled=true;
	form.MTKBCF[0].disabled=true;
	form.MTKNCF[0].disabled=true;
	

	form.VoIPCallTransfer[0].disabled=true;
	form.VoIP3wayConf[0].disabled=true;
	form.VoIPDNDEnable[0].disabled=true;
	
	form.VoIPCallerId[1].disabled=true;

	form.MTKUCF[1].disabled=true;
	form.MTKBCF[1].disabled=true;
	form.MTKNCF[1].disabled=true;


	form.VoIPCallTransfer[1].disabled=true;
	form.VoIP3wayConf[1].disabled=true;
	form.VoIPDNDEnable[1].disabled=true;


	form.Line.disabled=true;
	form.MTKVoIPUCFNumber.disabled=true;
	form.MTKVoIPBCFNumber.disabled=true;
	form.MTKVoIPNCFNumber.disabled=true;

	form.VoIPBlindTransferNumber.disabled=true;

	form.VoIPAttendedTransferNumber.disabled=true;
	form.VoIP3wayConfNumber.disabled=true;


<%end if%>
<%end if%>

	if(form.MTKUCF[0].checked)
		UforwardOn(false);
	else
		UforwardOn(true);

	if(form.MTKBCF[0].checked)
		BforwardOn(false);
	else
		BforwardOn(true);
	
	if(form.MTKNCF[0].checked)
		NforwardOn(false);
	else
		NforwardOn(true);
	
	if(form.VoIPCallTransfer[0].checked) {
		transferOn(false);
	} else {
		transferOn(true);
	}

	if(form.VoIP3wayConf[0].checked) {
		conferenceOn(false);
	} else {
		conferenceOn(true);
	}
	
}
</script>
</head><body onLoad="onLoadFun()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_call_ctrl.asp" name="VOIP_CALL_CTRL_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
	<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
	<td width="150" height="30" class="title-main">
	<font color="#FFFFFF">VoIP Call Control</font></td>
	<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>
	<INPUT TYPE="HIDDEN" NAME="VoIP_call_ctrl_flag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="LineFlag" VALUE="0">
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
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Anonymous call blocking</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="Anonymous_call_blocking" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCallBlock", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="Anonymous_call_blocking" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCallBlock", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Anonymous calling</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="Anonymous_calling" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCall", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="Anonymous_calling" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCall", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>



	<tr>
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>







	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Caller Id Display</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
		<INPUT TYPE="RADIO" NAME="VoIPCallerId" VALUE="1" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallerIdEnable", "h") = "1" then asp_write("checked") end if %> "><font color="#000000">Enable</font>
		<INPUT TYPE="RADIO" NAME="VoIPCallerId" VALUE="0" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallerIdEnable", "h") <> "1" then asp_write("checked") end if%> "><font color="#000000">Disable</font>
		<font color="#000000">(On The Local Phone)</font>
		</td></tr>
	<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Waiting</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPCallWaiting" VALUE="1" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingEnable", "h") = "1" then asp_write("checked") end if %> onclick="waitingOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallWaiting" VALUE="0" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingEnable", "h") <> "1" then asp_write("checked") end if%> onclick="waitingOn(true)"><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Waiting SIP Reply</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
		<INPUT TYPE="RADIO" NAME="VoIPCallWaitingReply" VALUE="Ringing" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingReply", "h") = "Ringing" then asp_write("checked") end if %>><font color="#000000">Ringing</font>
		<INPUT TYPE="RADIO" NAME="VoIPCallWaitingReply" VALUE="Queued" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingReply", "h") = "Queued" then asp_write("checked") elseif TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingReply", "h") = "N/A" then asp_write("checked") end if%>><font color="#000000">Queued</font>
		</td></tr>
	<%end if%>	
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Hold</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPCallHold" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallHold", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallHold" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallHold", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">MWI(message waiting indication)</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPMWIEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPMWIEnable", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPMWIEnable" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPMWIEnable", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>
	<tr>
<%end if%>

	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Call Forward</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>
	<tr>
		<td class="light-orange" style="height: 22px"></td>
		<td class="light-orange" style="height: 22px"></td>
		<td class="tabdata" align=right style="height: 22px"><font color="#000000">Unconditional Call Forward</font></td>
		<td class="tabdata" align=center style="height: 22px">:</td>
		<td class="tabdata" style="height: 22px">
			<INPUT TYPE="RADIO" NAME="MTKUCF" VALUE="1" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKUCFEnable", "h") = "1" then asp_write("checked") end if %> onclick="UforwardOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="MTKUCF" VALUE="0" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKUCFEnable", "h") <> "1" then asp_write("checked") end if%> onclick="UforwardOn(true)"><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Unconditional Forward Destination</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="MTKVoIPUCFNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKSIPUCFNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "MTKSIPUCFNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange" style="height: 22px"></td>
		<td class="light-orange" style="height: 22px"></td>
		<td class="tabdata" align=right style="height: 22px"><font color="#000000">Busy Call Forward</font></td>
		<td class="tabdata" align=center style="height: 22px">:</td>
		<td class="tabdata" style="height: 22px">
			<INPUT TYPE="RADIO" NAME="MTKBCF" VALUE="1" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKBCFEnable", "h") = "1" then asp_write("checked") end if %> onclick="BforwardOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="MTKBCF" VALUE="0" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKBCFEnable", "h") <> "1" then asp_write("checked") end if%> onclick="BforwardOn(true)"><font color="#000000">Disable</font>
		</td></tr>	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Busy Forward Destination</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="MTKVoIPBCFNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKSIPBCFNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "MTKSIPBCFNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange" style="height: 22px"></td>
		<td class="light-orange" style="height: 22px"></td>
		<td class="tabdata" align=right style="height: 22px"><font color="#000000">No Reply Call Forward</font></td>
		<td class="tabdata" align=center style="height: 22px">:</td>
		<td class="tabdata" style="height: 22px">
			<INPUT TYPE="RADIO" NAME="MTKNCF" VALUE="1" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKNCFEnable", "h") = "1" then asp_write("checked") end if %> onclick="NforwardOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="MTKNCF" VALUE="0" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKNCFEnable", "h") <> "1" then asp_write("checked") end if%> onclick="NforwardOn(true)"><font color="#000000">Disable</font>
		</td></tr>	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">No Reply Forward Destination</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="MTKVoIPNCFNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "MTKSIPNCFNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "MTKSIPNCFNumber", "s") else asp_Write("") end if %>">
		</td></tr>
		
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Call Transfer</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Transfer</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPCallTransfer" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallTransfer", "h") = "Yes" then asp_write("checked") end if%> onclick="transferOn(false)" ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallTransfer" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallTransfer", "h") <> "Yes" then asp_write("checked") end if%> onclick="transferOn(true)" ><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Blind Transfer Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPBlindTransferNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPBlindTransferNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPBlindTransferNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Attended Transfer Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPAttendedTransferNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPAttendedTransferNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPAttendedTransferNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Three-Way Conference</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Three-Way Conference</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIP3wayConf" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConf", "h") = "Yes" then asp_write("checked") end if%> onclick="conferenceOn(false)" ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIP3wayConf" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConf", "h") <> "Yes" then asp_write("checked") end if%> onclick="conferenceOn(true)" ><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Conference Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIP3wayConfNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConfNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConfNumber", "s") else asp_Write("") end if %>">
		</td></tr>

	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Do Not Disturb</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">DND</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPDNDEnable" VALUE="1" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDEnable", "h") = "1" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPDNDEnable" VALUE="0" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDEnable", "h") <> "1" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>

		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="dosave();"></td></tr></table>
</form></body></html>
