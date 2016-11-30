<%
If request_Form("VoIP_SIP_flag")="1" then
	tcWebApi_constSet("WebCurSet_Entry", "VoIPIdle", "1")
	if tcWebApi_get("VoIPBasic_Entry0", "Enable", "h") = "Yes"  then
		if tcWebApi_get("VoIPSysParam_Entry0", "SC_LINE_INFO_STATUS", "h") <> "Idle"  then
			tcWebApi_constSet("WebCurSet_Entry", "VoIPIdle", "0")
		end if
	end if
	if tcWebApi_get("WebCurSet_Entry", "VoIPIdle", "h") = "1"  then
		if tcWebApi_get("VoIPBasic_Entry1", "Enable", "h") = "Yes"  then
			if tcWebApi_get("VoIPSysParam_Entry1", "SC_LINE_INFO_STATUS", "h") <> "Idle"  then
				tcWebApi_constSet("WebCurSet_Entry", "VoIPIdle", "0")
			end if
		end if
	end if
	if tcWebApi_get("WebCurSet_Entry", "VoIPIdle", "h") = "1"  then
		if tcWebApi_get("InfoVoIPH248_Entry0", "lineInfoStatus", "h") <> "N/A" then
			if tcWebApi_get("InfoVoIPH248_Entry0", "lineInfoStatus", "h") <> "Idle"  then
				tcWebApi_constSet("WebCurSet_Entry", "VoIPIdle", "0")
			end if
		end if
	end if
	if tcWebApi_get("WebCurSet_Entry", "VoIPIdle", "h") = "1"  then
		if tcWebApi_get("VoIPBasic_Common", "VoIPLine2Enable", "h") = "Yes"  then
			if tcWebApi_get("InfoVoIPH248_Entry1", "lineInfoStatus", "h") <> "N/A" then
				if tcWebApi_get("InfoVoIPH248_Entry1", "lineInfoStatus", "h") <> "Idle"  then
					tcWebApi_constSet("WebCurSet_Entry", "VoIPIdle", "0")
				end if
			end if
		end if
	end if
if tcWebApi_get("WebCurSet_Entry", "VoIPIdle", "h") = "1"  then
	tcWebApi_set("VoIPAdvanced_Common", "RegistrationExpire", "regExpTmr")
	tcWebApi_set("VoIPAdvanced_Common", "MaxStartDelay", "maxStartDelay")
	tcWebApi_set("VoIPAdvanced_Common", "RegisterRetryInterval", "regRetryInt")
if Request_Form("isMultiDigitMap") = "1" then
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap1", "DigitMap1")	
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap2", "DigitMap2")	
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap3", "DigitMap3")	
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap4", "DigitMap4")	
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap5", "DigitMap5")	
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap6", "DigitMap6")	
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap7", "DigitMap7")	
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMap8", "DigitMap8")	
else
	tcWebApi_set("VoIPAdvanced_Common", "DialPlan", "dialPlan")
end if
	tcWebApi_set("VoIPDigitMap_Entry", "StartDigitTimer", "megacodmstarttime")
	tcWebApi_set("VoIPDigitMap_Entry", "InterDigitTimerShort", "megacodmshorttime")
	tcWebApi_set("VoIPDigitMap_Entry", "InterDigitTimerLong", "megacodmlongtime")
	tcWebApi_set("VoIPDigitMap_Entry", "InterDigitTimerT", "megacodmttime")	
	tcWebApi_set("VoIPDigitMap_Entry", "BusyToneTimer", "busytonetimer")
	tcWebApi_set("VoIPDigitMap_Entry", "HangingReminderToneTimer", "hangingremindertimer")
	tcWebApi_set("VoIPDigitMap_Entry", "TMode", "dmtmode")
	tcWebApi_set("VoIPAdvanced_Common", "DTMFTransportMode", "dtmfRelay")
if request_Form("dtmfRelay") = "RFC2833" then
	tcWebApi_set("VoIPBasic_Common", "TelephoneEventPayloadType", "rfc2833pt")
end if
	tcWebApi_set("VoIPAdvanced_Common", "FlashOffTransSetting", "hookFlashRelay")
	tcWebApi_set("VoIPBasic_Common", "HeartbeatSwitch", "natKeepAliveMessageFlag")
	tcWebApi_set("VoIPBasic_Common", "HeartbeatCycle", "natKeepAliveTime")
	tcWebApi_set("VoIPAdvanced_Common", "FaxCtrlMode", "faxctrlmode")
	tcWebApi_set("VoIPAdvanced_Common", "rtpRedundantEnable", "siprtpredundancyflag")
	tcWebApi_set("VoIPAdvanced_Common", "rtpRedundantPT", "siprtpredundancypt")
	If tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes" then
	tcWebApi_set("VoIPAdvanced_Common", "FromToHeadURI", "fromtoheadurl")
	End If
	tcWebApi_set("VoIPAdvanced_Common", "FaxPassThruCodec", "faxpassthru")
	tcWebApi_set("VoIPAdvanced_Common", "HotLineMode", "DivHotLineType")
	tcWebApi_set("VoIPAdvanced_Common", "RegResponsePortEnable", "DivIsRegisterWithPort")
	tcWebApi_set("VoIPAdvanced_Common", "MsgWithUserEnable", "DivIsMsgWithUser")
	tcWebApi_set("VoIPAdvanced_Common", "HotLineActNum", "DivHotLineActiveCode")
	tcWebApi_set("VoIPAdvanced_Common", "CallWaitType", "DivCWType")
	tcWebApi_set("VoIPAdvanced_Common", "NoRegDialToneEnable", "DivDialtone")
	tcWebApi_set("VoIPAdvanced_Common", "PolarityReversalMode", "DivRevPolarityType")
	tcWebApi_set("VoIPAdvanced_Common", "VoiceCodecPriorityCtrl", "DivCodecSelectFlag")
	tcWebApi_set("VoIPAdvanced_Common", "NumberMatchMode", "DivDigitMatchMethod")
	tcWebApi_set("VoIPAdvanced_Common", "SubscribeType", "DivSubscribeType")
	tcWebApi_set("VoIPAdvanced_Common", "SubscribeExpire", "DivSubscribeExpire")
	tcWebApi_set("VoIPAdvanced_Common", "ActLocalSupplementService", "DivSupplementServiceType")
	tcWebApi_set("VoIPAdvanced_Common", "SupportSpecCharTransfer", "changeCharEnableFlag")
	tcWebApi_set("VoIPMedia_Common", "FaxCodec", "t38EnableFlag")
	tcWebApi_set("VoIPMedia_Common", "EchoCancellationEnable", "echoCancelEnableFlag")
	tcWebApi_set("VoIPMedia_Entry0", "VAD", "comfortnoiseEnableFlag")
	tcWebApi_set("VoIPMedia_Entry0", "SilenceCompressionEnable", "vadEnableFlag")
	If tcWebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then
	tcWebApi_set("VoIPMedia_Common", "EchoCancellationEnable", "echoCancelEnableFlag")
	tcWebApi_set("VoIPMedia_Entry1", "VAD", "comfortnoiseEnableFlag")
	tcWebApi_set("VoIPMedia_Entry1", "SilenceCompressionEnable", "vadEnableFlag")
	End If
	tcWebApi_set("VoIPBasic_Common", "PRACKEnable", "PrackEnableFlag")
	tcWebApi_set("VoIPBasic_Common", "LogOffMsgSendEnable", "sendUnregisterFlag")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_FLAG", "sessiontimerEnableFlag")
if request_Form("sessiontimerEnableFlag") = "1" then
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", "sessionTimer")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_MIN_EXP", "sessionMinSE")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_REFRESHER", "sessionRefresher")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_METHOD", "sessionMethod")
end if
	tcWebApi_set("VoIPBasic_Common", "SIPDSCPMark", "dscpmark")
	tcWebApi_set("VoIPBasic_Common", "RTPDSCPMark", "mymgtos")
	tcWebApi_set("VoIPCodecs_PVC0_Entry0", "priority", "codecList0")
	tcWebApi_set("VoIPCodecs_PVC1_Entry0", "priority", "codecList1")
	tcWebApi_set("VoIPCodecs_PVC0_Entry1", "priority", "codecList2")
	tcWebApi_set("VoIPCodecs_PVC1_Entry1", "priority", "codecList3")
	tcWebApi_set("VoIPCodecs_PVC0_Entry2", "priority", "codecList4")
	tcWebApi_set("VoIPCodecs_PVC1_Entry2", "priority", "codecList5")
	tcWebApi_set("VoIPCodecs_PVC0_Entry3", "priority", "codecList6")
	tcWebApi_set("VoIPCodecs_PVC1_Entry3", "priority", "codecList7")
	tcWebApi_set("VoIPCodecs_PVC0", "SIPPacketizationTime", "voicePtime0")
	tcWebApi_set("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "ingressGain0")
	tcWebApi_set("VoIPAdvanced_Entry0", "VoiceVolumeListen", "egressGain0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HotLineEnable", "warmline0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HotLineNumber", "warmlinenum0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HotLineDelayTime", "warmlinetimeout0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIPCallWaitingEnable", "callwait0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIPCallTransfer", "transfer0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "ForwardNumber", "fwdNum0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKSIPUCFNumber", "fwdNum0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKSIPBCFNumber", "fwdNum0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKSIPNCFNumber", "fwdNum0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKUCFEnable", "fwdAll0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKBCFEnable", "fwdBusy0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKNCFEnable", "fwdNoAns0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "fwdNoAnsTime0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIP3wayConf", "threepartyservice0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "calleridmode0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HookMinInterval", "minhookinterval0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HookMaxInterval", "maxhookinterval0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HookReleaseMin", "minhookrelease0")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SyncCallerTimeEnable", "synphonedate0Flag")
	If tcWebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then
	tcWebApi_set("VoIPCodecs_PVC1", "SIPPacketizationTime", "voicePtime1")
	tcWebApi_set("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "ingressGain1")
	tcWebApi_set("VoIPAdvanced_Entry1", "VoiceVolumeListen", "egressGain1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HotLineEnable", "warmline1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HotLineNumber", "warmlinenum1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HotLineDelayTime", "warmlinetimeout1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIPCallWaitingEnable", "callwait1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIPCallTransfer", "transfer1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "ForwardNumber", "fwdNum1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKSIPUCFNumber", "fwdNum1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKSIPBCFNumber", "fwdNum1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKSIPNCFNumber", "fwdNum1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKUCFEnable", "fwdAll1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKBCFEnable", "fwdBusy1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKNCFEnable", "fwdNoAns1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "fwdNoAnsTime1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIP3wayConf", "threepartyservice1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "ComingCallDisplayMode", "calleridmode1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookMinInterval", "minhookinterval1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookMaxInterval", "maxhookinterval1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookReleaseMin", "minhookrelease1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SyncCallerTimeEnable", "synphonedate1Flag")	
	End If
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMapEnable", "DigitMapEnableFlag")
	if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then			
		tcWebApi_set("VoIPDigitMap_Entry", "DigitMapSpecialEnable", "DigitMapSpecialEnableFlag")
		if request_Form("DigitMapSpecialEnableFlag") = "1" then	
			tcWebApi_set("VoIPDigitMap_Entry", "DigitMapSpecial", "ipt_DigitMapSpecial")
		end if
		tcWebApi_set("VoIPDigitMap_Entry", "PBXPrefixEnable", "PBXPrefixEnableFlag")
		if request_Form("PBXPrefixEnableFlag") = "1" then	
			tcWebApi_set("VoIPDigitMap_Entry", "PBXPrefix", "ipt_PBXPrefix")
		end if	
		tcWebApi_set("VoIPDigitMap_Entry", "PBXDialSecondaryEnable", "PBXDialSecondaryEnableFlag")	
	end if
	tcWebApi_commit("VoIPBasic_Common")
	tcWebApi_save()
end if
end if
%>
<HTML><HEAD><TITLE>VOIP设置</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="javascript">
<%
if request_Form("VoIP_SIP_flag")="1" then
	if tcWebApi_get("WebCurSet_Entry", "VoIPIdle", "h") <> "1"  then
		asp_write("alert('Voip is calling ,can not save,please do not save until voip ends.');")
	end if
end if
%>
var session	= "0";
<% if tcWebApi_get("WebCustom_Entry","isMultiDigitMap","h") = "Yes" then %>
function getDigitMapValue()
{
    if(document.getElementById("DigitMap1").value.length < 512){
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value;
   	}
   	else if(document.getElementById("DigitMap2").value.length < 512){
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value + document.getElementById("DigitMap2").value;
   	}else if(document.getElementById("DigitMap3").value.length < 512){
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value + 
   																								document.getElementById("DigitMap2").value +
   																								document.getElementById("DigitMap3").value;
   	}else if(document.getElementById("DigitMap4").value.length < 512){
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value + 
   																								document.getElementById("DigitMap2").value +
   																								document.getElementById("DigitMap3").value +
   																								document.getElementById("DigitMap4").value;
   	}else if(document.getElementById("DigitMap5").value.length < 512){
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value + 
   																								document.getElementById("DigitMap2").value +
   																								document.getElementById("DigitMap3").value +
   																								document.getElementById("DigitMap4").value +
   																								document.getElementById("DigitMap5").value;
   	}else if(document.getElementById("DigitMap6").value.length < 512){
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value + 
   																								document.getElementById("DigitMap2").value +
   																								document.getElementById("DigitMap3").value +
   																								document.getElementById("DigitMap4").value + 
   																								document.getElementById("DigitMap5").value +
   																								document.getElementById("DigitMap6").value;
   	}else if(document.getElementById("DigitMap7").value.length < 512){
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value + 
   																								document.getElementById("DigitMap2").value +
   																								document.getElementById("DigitMap3").value +
   																								document.getElementById("DigitMap4").value +
   																								document.getElementById("DigitMap5").value + 
   																								document.getElementById("DigitMap6").value +
   																								document.getElementById("DigitMap7").value;
   	}else{
   		document.getElementById("dialPlan").value = document.getElementById("DigitMap1").value + 
   																								document.getElementById("DigitMap2").value +
   																								document.getElementById("DigitMap3").value +
   																								document.getElementById("DigitMap4").value +
   																								document.getElementById("DigitMap5").value + 
   																								document.getElementById("DigitMap6").value + 
   																								document.getElementById("DigitMap7").value +
   																								document.getElementById("DigitMap8").value;
   	}

}
function setDigitMapValue()
{
		var len = document.getElementById("dialPlan").value.length;
		var num = len/512;

		if(len > 4096){
			alert("拨号规则的最大长度不能超过4096！！！");
			return -1;
		}
		
		if(num < 1){
			document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value;
			document.getElementById("DigitMap2").value = "";
		}else if(num < 2){
   		document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value.substring(0,512);
   		document.getElementById("DigitMap2").value = document.getElementById("dialPlan").value.substring(512);		
   		document.getElementById("DigitMap3").value = "";
		}else if(num < 3){
   		document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value.substring(0,512);
   		document.getElementById("DigitMap2").value = document.getElementById("dialPlan").value.substring(512,512*2);
   		document.getElementById("DigitMap3").value = document.getElementById("dialPlan").value.substring(512*2);
   		document.getElementById("DigitMap4").value = "";
		}else if(num < 4){
   		document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value.substring(0,512);
   		document.getElementById("DigitMap2").value = document.getElementById("dialPlan").value.substring(512,512*2);
   		document.getElementById("DigitMap3").value = document.getElementById("dialPlan").value.substring(512*2,512*3);
   		document.getElementById("DigitMap4").value = document.getElementById("dialPlan").value.substring(512*3);
   		document.getElementById("DigitMap5").value = "";
		}else if(num < 5){
   		document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value.substring(0,512);
   		document.getElementById("DigitMap2").value = document.getElementById("dialPlan").value.substring(512,512*2);
   		document.getElementById("DigitMap3").value = document.getElementById("dialPlan").value.substring(512*2,512*3);
   		document.getElementById("DigitMap4").value = document.getElementById("dialPlan").value.substring(512*3,512*4);
   		document.getElementById("DigitMap5").value = document.getElementById("dialPlan").value.substring(512*4);
   		document.getElementById("DigitMap6").value = "";
		}else if(num < 6){
   		document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value.substring(0,512);
   		document.getElementById("DigitMap2").value = document.getElementById("dialPlan").value.substring(512,512*2);
   		document.getElementById("DigitMap3").value = document.getElementById("dialPlan").value.substring(512*2,512*3);
   		document.getElementById("DigitMap4").value = document.getElementById("dialPlan").value.substring(512*3,512*4);
   		document.getElementById("DigitMap5").value = document.getElementById("dialPlan").value.substring(512*4,512*5);
   		document.getElementById("DigitMap6").value = document.getElementById("dialPlan").value.substring(512*5);
   		document.getElementById("DigitMap7").value = "";
		}else if(num < 7){
   		document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value.substring(0,512);
   		document.getElementById("DigitMap2").value = document.getElementById("dialPlan").value.substring(512,512*2);
   		document.getElementById("DigitMap3").value = document.getElementById("dialPlan").value.substring(512*2,512*3);
   		document.getElementById("DigitMap4").value = document.getElementById("dialPlan").value.substring(512*3,512*4);
   		document.getElementById("DigitMap5").value = document.getElementById("dialPlan").value.substring(512*4,512*5);
   		document.getElementById("DigitMap6").value = document.getElementById("dialPlan").value.substring(512*5,512*6);
   		document.getElementById("DigitMap7").value = document.getElementById("dialPlan").value.substring(512*6);
   		document.getElementById("DigitMap8").value = "";
		}else if(num <= 8){
   		document.getElementById("DigitMap1").value = document.getElementById("dialPlan").value.substring(0,512);
   		document.getElementById("DigitMap2").value = document.getElementById("dialPlan").value.substring(512,512*2);
   		document.getElementById("DigitMap3").value = document.getElementById("dialPlan").value.substring(512*2,512*3);
   		document.getElementById("DigitMap4").value = document.getElementById("dialPlan").value.substring(512*3,512*4);
   		document.getElementById("DigitMap5").value = document.getElementById("dialPlan").value.substring(512*4,512*5);
   		document.getElementById("DigitMap6").value = document.getElementById("dialPlan").value.substring(512*5,512*6);
   		document.getElementById("DigitMap7").value = document.getElementById("dialPlan").value.substring(512*6,512*7);
   		document.getElementById("DigitMap8").value = document.getElementById("dialPlan").value.substring(512*7);
		}

   return 0;
}
<%end if%>
function LoadFrame()
{
   with ( document.ConfigForm ) 
   {
      generatePerLineTable();
      CheckPriority();
      if ("1" == "<%tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_FLAG", "s") %>")
      {
	   	  document.getElementById("sesstimer").style.display = "";
	   	  document.getElementById("sessminse").style.display = "";      	
	   	  document.getElementById("sessrefresher").style.display = "";
	   	  document.getElementById("sessmethod").style.display = "";      
      }
	  	else
	   	{
	   	  document.getElementById("sesstimer").style.display = "none";
	   	  document.getElementById("sessminse").style.display = "none";      	
	   	  document.getElementById("sessrefresher").style.display = "none";
	   	  document.getElementById("sessmethod").style.display = "none";  
	   	}  
	  if ( "RFC2833" == "<%tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "s") %>"){
      	document.getElementById("trRFC2833PT").style.display = "";
      }
      else{
      	document.getElementById("trRFC2833PT").style.display = "none";
      } 
   }	
<% if tcWebApi_get("WebCustom_Entry","isMultiDigitMap","h") = "Yes" then %>
   getDigitMapValue();
<%end if%>
	CheckDigitMapEnable(getElById('cb_DigitMapEnable'));
	getElById('txt_DigitMap').value=getElById('dialPlan').value;
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>	
	getElById('txt_DigitMapSpecial').value=getElById('ipt_DigitMapSpecial').value;	
	CheckDigitMapSpecialEnable(getElById('cb_DigitMapSpecialEnable'));
	CheckPBXPrefixEnable(getElById('cb_PBXPrefixEnable'));	
<% if tcwebApi_get("WanInfo_Common","NoGUIAccessLimit","h" ) <> "1" then %>
	getElById('txt_DigitMap').disabled = true;
<% end if %>
<%end if%>
}	

function generatePerLineTable() {

   // Generate the table
   if (document.body.innerHTML) {
      var tableHtmlOutput = "<table id='perLineTable1' border='1'><tbody id='perLineTbody1'>";
			
      // Line row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >Line    <\/td>"
      tableHtmlOutput += "<td align=center>1</td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center>2</td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Codec rows
		  tableHtmlOutput += "<tr><td >G.722<\/td>"
		  var selectOptions0 = Array();
		  selectOptions0[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions0[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions0[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions0[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions0[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions0[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions0[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions0[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  width=180><select name='codecList0'>" + selectOptions0.join("") + "<\/select></td>";

			<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
		  var selectOptions1 = Array();
		  selectOptions1[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions1[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions1[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions1[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions1[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions1[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions1[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions1[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  width=180><select name='codecList1'>" + selectOptions1.join("") + "<\/select></td>"; 
		  <% end if %>
		  tableHtmlOutput += "</tr>";

		  tableHtmlOutput += "<tr><td >G.711ALaw<\/td>"
		  var selectOptions2 = Array();
		  selectOptions2[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions2[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions2[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions2[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions2[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions2[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions2[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions2[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList2'>" + selectOptions2.join("") + "<\/select></td>";

			<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
		  var selectOptions3 = Array();
		  selectOptions3[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions3[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions3[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions3[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions3[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions3[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions3[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions3[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList3'>" + selectOptions3.join("") + "<\/select></td>";  
		  <% end if %>
		  tableHtmlOutput += "</tr>";

		  tableHtmlOutput += "<tr><td >G.711ULaw<\/td>"
		  var selectOptions4 = Array();
		  selectOptions4[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions4[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions4[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions4[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions4[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions4[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions4[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions4[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList4'>" + selectOptions4.join("") + "<\/select></td>";

			<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
		  var selectOptions5 = Array();
		  selectOptions5[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions5[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions5[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions5[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions5[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions5[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions5[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions5[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList5'>" + selectOptions5.join("") + "<\/select></td>";
		  <% end if %> 
		  tableHtmlOutput += "</tr>"; 
  
		  tableHtmlOutput += "<tr><td >G.729<\/td>"
		  var selectOptions6 = Array();
		  selectOptions6[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions6[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions6[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions6[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions6[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions6[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions6[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions6[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList6'>" + selectOptions6.join("") + "<\/select></td>";

			<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
		  var selectOptions7 = Array();
		  selectOptions7[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions7[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions7[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions7[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions7[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions7[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions7[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions7[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList7'>" + selectOptions7.join("") + "<\/select></td>";  
		  <% end if %>
		  tableHtmlOutput += "</tr>"; 
	  
      // Ptime row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >ptime 设置 <\/td>"
      var selectOptions0 = Array();
      selectOptions0[0] = "<option value=10 <%if tcWebApi_get("VoIPCodecs_PVC0", "SIPPacketizationTime", "h") = "10" then  asp_Write("selected") end if %>>" + 10;
      selectOptions0[1] = "<option value=20 <%if tcWebApi_get("VoIPCodecs_PVC0", "SIPPacketizationTime", "h") = "20" then  asp_Write("selected") end if %>>" + 20;
      selectOptions0[2] = "<option value=30 <%if tcWebApi_get("VoIPCodecs_PVC0", "SIPPacketizationTime", "h") = "30" then  asp_Write("selected") end if %>>" + 30;
      tableHtmlOutput += "<td align=center><select name='voicePtime0'>" + selectOptions0.join("") + "<\/select></td>";
      
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      var selectOptions1 = Array();
      selectOptions1[0] = "<option value=10 <%if tcWebApi_get("VoIPCodecs_PVC1", "SIPPacketizationTime", "h") = "10" then asp_Write("selected") end if %>>" + 10;
      selectOptions1[1] = "<option value=20 <%if tcWebApi_get("VoIPCodecs_PVC1", "SIPPacketizationTime", "h") = "20" then asp_Write("selected") end if %>>" + 20;
      selectOptions1[2] = "<option value=30 <%if tcWebApi_get("VoIPCodecs_PVC1", "SIPPacketizationTime", "h") = "30" then asp_Write("selected") end if %>>" + 30;
      tableHtmlOutput += "<td align=center><select name='voicePtime1'>" + selectOptions1.join("") + "<\/select></td>";
      <% end if %>
      
      tableHtmlOutput += "</tr>";

      //End of table
      tableHtmlOutput += "</tbody></table>";

      //////////////////////////////////////////
      //       new table for volume
      //////////////////////////////////////////
      tableHtmlOutput += "<br>";
      tableHtmlOutput += "<b>音量大小设置(db)</b><br>";
      tableHtmlOutput += "<table id='perLineTable2' border='1'><tbody id='perLineTbody2'>";

      // Line row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >Line            <\/td>"
      tableHtmlOutput += "<td align=center>1</td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center>2</td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Volume row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >发送音量<\/td>"
     	tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'ingressGain0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>\'></td>";    
     	<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
     	tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'ingressGain1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>\'></td>";    
      <% end if %>
      tableHtmlOutput += "</tr>";
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >接收音量<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'egressGain0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry0", "VoiceVolumeListen", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "VoiceVolumeListen", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'egressGain1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry1", "VoiceVolumeListen", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "VoiceVolumeListen", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      //End of table
      tableHtmlOutput += "</tbody></table>";

      //////////////////////////////////////////
      //   new table for complement service
      //////////////////////////////////////////
      tableHtmlOutput += "<br>";
      tableHtmlOutput += "<b>补充业务设置</b><br>";
      tableHtmlOutput += "<table id='perLineTable3' border='1'><tbody id='perLineTbody3'>";

      // Line row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >Line            <\/td>"
      tableHtmlOutput += "<td align=center>1</td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center>2</td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Warm Line row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用热线<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'warmline0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'warmline1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Warm Line Number row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >热线号码<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinenum0\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HotLineNumber", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinenum1\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HotLineNumber", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Warm Line Timeout row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >热线延迟时间(秒)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinetimeout0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineDelayTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HotLineDelayTime", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinetimeout1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineDelayTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HotLineDelayTime", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call waiting row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用呼叫等待<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'callwait0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIPCallWaitingEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'callwait1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIPCallWaitingEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      /*zhangcg: 20110612*/
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用呼叫转移<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'transfer0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIPCallTransfer", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'transfer1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIPCallTransfer", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding number row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >呼叫前转号码<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNum0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "ForwardNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "ForwardNumber", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNum1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "ForwardNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "ForwardNumber", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding unconditionally row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >无条件呼叫前转<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdAll0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKUCFEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdAll1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKUCFEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding on "busy" row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >遇忙呼叫前转<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdBusy0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKBCFEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdBusy1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKBCFEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding on "no answer" row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >无应答呼叫前转<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdNoAns0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKNCFEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdNoAns1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKNCFEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";
	  
	  // Call forwarding on "no answer time"
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >无应答呼叫前转等待时间<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNoAnsTime0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNoAnsTime1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用三方会议<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservice0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIP3wayConf", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservice1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIP3wayConf", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

    //caller id mode
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >来电显示模式 <\/td>"
      var selectOptions0 = Array();
      selectOptions0[0] = "<option value=" + 0 + " <%if tcWebApi_get("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "h") = "0" then asp_Write("selected") end if %>>" + "DTMF";
      selectOptions0[1] = "<option value=" + 1 + " <%if tcWebApi_get("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "h") = "1" then asp_Write("selected") end if %>>" + "FSK";   
      tableHtmlOutput += "<td align=center><select name='calleridmode0'>" + selectOptions0.join("") + "<\/select></td>";
                  
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>       
      var selectOptions1 = Array();
      selectOptions1[0] = "<option value=" + 0 + " <%if tcWebApi_get("VoIPCallCtrl_Entry1", "ComingCallDisplayMode", "h") = "0" then asp_Write("selected") end if %>>" + "DTMF";
      selectOptions1[1] = "<option value=" + 1 + " <%if tcWebApi_get("VoIPCallCtrl_Entry1", "ComingCallDisplayMode", "h") = "1" then asp_Write("selected") end if %>>" + "FSK";      
      tableHtmlOutput += "<td align=center><select name='calleridmode1'>" + selectOptions1.join("") + "<\/select></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

	//flash hook interval 
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >拍叉簧最小时间间隔(毫秒)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookinterval0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookMinInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookMinInterval", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookinterval1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookMinInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookMinInterval", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >拍叉簧最大时间间隔(毫秒)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'maxhookinterval0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookMaxInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookMaxInterval", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'maxhookinterval1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookMaxInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookMaxInterval", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>      
      tableHtmlOutput += "</tr>";
	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >挂机最小时间间隔(毫秒)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookrelease0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookReleaseMin", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookReleaseMin", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookrelease1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookReleaseMin", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookReleaseMin", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      
      tableHtmlOutput += "</tr>";
	//Synchronize phone date time
	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用同步话机时间<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'synphonedate0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SyncCallerTimeEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'synphonedate1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SyncCallerTimeEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      //End of table
      tableHtmlOutput += "</tbody></table>";

      //////////////////////////////////////////
      //       new table for digitmap
      //////////////////////////////////////////
      tableHtmlOutput += "<br>";
      tableHtmlOutput += "<b>数图设置</b><br>";
      tableHtmlOutput += "<table id='Table_DigitMap' border='1'><tbody id='Tbody_DigitMap'>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用普通数图<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 id=\'cb_DigitMapEnable\' name=\'cb_DigitMapEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "DigitMapEnable", "h") = "1" then asp_Write("checked") end if %> onclick='CheckDigitMapEnable(this)'></td>";
      tableHtmlOutput += "</tr>";
      
      tableHtmlOutput += "<tr id='sec_DigitMap'>";
      tableHtmlOutput += "<td align=center colSpan=2><textarea id=\'txt_DigitMap\' name=\'txt_DigitMap\' rows=4 maxlength=4096 wrap='OFF' style='WIDTH:420px;'></textarea></td>";
      tableHtmlOutput += "</tr>"; 
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>             
      tableHtmlOutput += "<tr>";      
      tableHtmlOutput += "<td >启用特定数图<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 id=\'cb_DigitMapSpecialEnable\' name=\'cb_DigitMapSpecialEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "DigitMapSpecialEnable", "h") = "1" then asp_Write("checked") end if %> onclick='CheckDigitMapSpecialEnable(this)'></td>";
      tableHtmlOutput += "</tr>";
      
      tableHtmlOutput += "<tr id='sec_DigitMapSpecial'>";
      tableHtmlOutput += "<td align=center colSpan=2><textarea id=\'txt_DigitMapSpecial\' name=\'txt_DigitMapSpecial\' rows=1 maxlength=512 wrap='OFF' style='WIDTH:420px;'></textarea></td>";
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr style='display:none'>";
      tableHtmlOutput += "<td align=center colSpan=2><input type='text' value='<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMapSpecial", "s")%>' name='ipt_DigitMapSpecial' id='ipt_DigitMapSpecial'></td>";
      tableHtmlOutput += "</tr>";
            
      tableHtmlOutput += "<tr>";      
      tableHtmlOutput += "<td >启用PBX前缀<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 id=\'cb_PBXPrefixEnable\' name=\'cb_PBXPrefixEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "PBXPrefixEnable", "h") = "1" then asp_Write("checked") end if %> onclick='CheckPBXPrefixEnable(this)'></td>";
      tableHtmlOutput += "</tr>";
      
      tableHtmlOutput += "<tr id='sec_PBXPrefix'>";
      tableHtmlOutput += "<td >PBX前缀<\/td>"    
      tableHtmlOutput += "<td align=center><input type='text' size=12 maxlength=8 name=\'ipt_PBXPrefix\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPDigitMap_Entry", "PBXPrefix", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "PBXPrefix", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";
                  
      tableHtmlOutput += "<tr>";      
      tableHtmlOutput += "<td >PBX启用二次拨号音提示<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'cb_PBXDialSecondaryEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "PBXDialSecondaryEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";
<% end if %>
      //End of table
      tableHtmlOutput += "</tbody></table>";

      document.getElementById("perLineTablePlaceholder1").innerHTML = tableHtmlOutput;
   } 
   else 
   {
      alert("Cannot create per line table in this browser.");
   }
}	
	
function CheckDigitMapEnable(cbCtl)
{   
	if (cbCtl.checked == true)
	{
		setDisplay('sec_DigitMap',1);
	}
	else
	{
		setDisplay('sec_DigitMap',0);
	}
}

<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>	
function CheckDigitMapSpecialEnable(cbCtl)
{   
	if (cbCtl.checked == true)
	{
		setDisplay('sec_DigitMapSpecial',1);
	}
	else
	{
		setDisplay('sec_DigitMapSpecial',0);
	}
}

function CheckPBXPrefixEnable(cbCtl)
{   
	if (cbCtl.checked == true)
	{
		setDisplay('sec_PBXPrefix',1);
	}
	else
	{
		setDisplay('sec_PBXPrefix',0);
	}
}
<% end if %>

function CheckPriority()
{
	  getElementByName("codecList0").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC0_Entry0", "priority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList2").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC0_Entry1", "priority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList4").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC0_Entry2", "priority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList6").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC0_Entry3", "priority", "s") else asp_Write("0") end if %>";

	  <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
	  getElementByName("codecList1").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC1_Entry0", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC1_Entry0", "priority", "s") else asp_Write("0") end if %>"; 
	  getElementByName("codecList3").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC1_Entry1", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC1_Entry1", "priority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList5").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC1_Entry2", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC1_Entry2", "priority", "s") else asp_Write("0") end if %>"; 	
	  getElementByName("codecList7").selectedIndex = 	"<%if tcWebApi_get("VoIPCodecs_PVC1_Entry3", "priority", "h") <> "N/A" then tcWebAPI_get("VoIPCodecs_PVC1_Entry3", "priority", "s") else asp_Write("0") end if %>";
      <% end if %>
}		
	
function applyClickBasic()
{
   var loc = "app-VoIP.asp";
   var code = "location='" + loc + " '";
   eval(code);
}	

function limitNum(o)
{
	if (o.value != "")
	{
		if (!isInteger(o.value))
		{
			alert('"' + o.value + '"' + '只能输入数字.');	
			window.setTimeout( function(){ o.focus(); }, 0);
		}
	}
}

function dtmfRelayChange(o){
  var idx = o.selectedIndex;
  var val = o.options[idx].value;
  if ( val == "RFC2833" ){
  	document.getElementById("trRFC2833PT").style.display = "";
  }
  else{
  	document.getElementById("trRFC2833PT").style.display = "none";
  }
}

function sessionClick(cb){
   if(cb.checked == true )
   	{
   	  document.getElementById("sesstimer").style.display = "";
   	  document.getElementById("sessminse").style.display = "";
   	  document.getElementById("sessrefresher").style.display = "";
   	  document.getElementById("sessmethod").style.display = "";
   	}
   else
   	{
   	  document.getElementById("sesstimer").style.display = "none";
   	  document.getElementById("sessminse").style.display = "none";
   	  document.getElementById("sessrefresher").style.display = "none";
   	  document.getElementById("sessmethod").style.display = "none";
   	}
}
function btnApplySip()
{
	with (document.ConfigForm)
	{
    var iCw = 0;
    var iCfnum = 0;
    var iCf = 0;
    var i = 0;
    var checkMarkTest;
    
	if (cb_DigitMapEnable.checked)
		getElById('dialPlan').value=getElById('txt_DigitMap').value;
<% if tcWebApi_get("WebCustom_Entry","isMultiDigitMap","h") = "Yes" then %>
    var ret = -1;
    
    ret = setDigitMapValue();
    if(ret < 0)
    	return;
<%end if%>
	<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>   
    for (i = 0; i < 2; i++) 
	<%end if%>
    {
      iCw = 0;
      iCfnum = 0;
      iCf = 0;

      checkMarkTest = eval("callwait" + i + ".checked == true");
      if ( checkMarkTest )
      {
          iCw = 1; 
      }
      checkMarkTest = eval("fwdBusy" + i + ".checked == true");
      if ( checkMarkTest )
      {
          iCf++; 
      }
      checkMarkTest = eval("fwdNoAns" + i + ".checked == true");
      if ( checkMarkTest )
      {
          iCf++; 
      }
      checkMarkTest = eval("fwdAll" + i + ".checked == true");
      if ( checkMarkTest )
      {
          iCf++; 
      }
      checkMarkTest = eval("fwdNum" + i + ".value != ''");
      if ( checkMarkTest )
      {
          iCfnum = 1;
      }
      if ((iCw + iCf) > 1) {
          alert("Call feature can only choose one!");
          return;
      }
      
      if ((iCf == 1) && (iCfnum == 0)) {
          alert("Call forward number is invalid!");
          return;
      }

			var codecOld = "";
			var codecNew = "";
      var k = 0;
      var m = 0;
      var mm = 0;
      var zz = 0;
      for (k = i; k < 8; k+=2) 
      {
        codecOld = codecOld + "," + codecNew;
        codecNew = eval( "codecList" + k + ".value" );
        codecList = codecOld.split(",");
        for (var j = 0;j < codecList.length;j++)
	      {
		      if (codecNew != 0 && codecNew == codecList[j])
		      {
	          //alert("Codec priority is same, please config again!");
	          alert("语音编码优先级设置重复，请重新设置!");
	          return;
		      }
		    }
				if(codecNew == 0)
				{
				    zz++;
				}
      }
			if(zz == 4)
		  {
		    alert("不能全部禁用语音编码,请至少选择一种!");
				return;
			}
	  }
	   
	  var temp = parseInt(ingressGain0.value);	
		if (temp < -12 || temp > 12)
		{
			alert('发送音量的范围是-12~12db.');
			return;
		}
		
	  <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>   
	  var temp = parseInt(ingressGain1.value);	
		if (temp < -12 || temp > 12)
		{
			alert('发送音量的范围是-12~12db.');
			return;
		}		
	    <%end if%>
			
		var temp = parseInt(egressGain0.value);	
		if (temp < -12 || temp > 12)
		{
			alert('接收音量的范围是-12~12db.');
			return;
		} 

		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>   
		var temp = parseInt(egressGain1.value);	
		if (temp < -12 || temp > 12)
		{
			alert('接收音量的范围是-12~12db.');
			return;
		} 		
	    <%end if%>
			
		var temp = parseInt(rfc2833pt.value);	
		if (temp < 96 || temp > 127)
		{
			alert('Telephone Event Payload Type的范围是96~127.');
			return;
		}
		
		var temp = parseInt(siprtpredundancypt.value);	
		if (temp < 96 || temp > 127)
		{
			alert('RTP冗余payload type范围是96~127.');
			return;
		}		
		
 		var temp1 = parseInt(sessionTimer.value);	
		if (temp1 < 90)
		{
			alert('会话刷新周期应不短于90s.');
			return;
		}
		
 		var temp2 = parseInt(sessionMinSE.value);	
		if (temp2 < 90)
		{
			alert('最小会话刷新周期应不短于90s.');
			return;
		}	
		
		if (temp1 < temp2)
		{
			alert ('会话刷新周期应不短于最小会话刷新周期.');	
			return;
		}

		var temp = parseInt (dscpmark.value);
		if (temp < 0 || temp > 63)
		{
			alert ('SIP DSCP优先级的范围是0~63.');
			return;
		}
	
		var temp = parseInt (mymgtos.value);
		if (temp < 0 || temp > 63)
		{
			alert ('RTP DSCP优先级的范围是0~63.');
			return;
		}

		var temp1 = parseInt (minhookinterval0.value);
		var temp2 = parseInt (maxhookinterval0.value);
		var temp3 = parseInt (minhookrelease0.value);		
		if (temp3 < 300 || temp3 > 1199)
		{
			alert('挂机最小间隔的范围是300~1199ms.');
			return;
		}	
		if (temp1 >= temp2)
		{
			alert('最大插簧时长应大于最小插簧时长.');
			return;
		}		
		if (temp3 < temp2)
		{
			alert('挂机最小间隔应大于最大插簧时长.');
			return;
		}
		
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>   
		var temp1 = parseInt (minhookinterval1.value);
		var temp2 = parseInt (maxhookinterval1.value);
		var temp3 = parseInt (minhookrelease1.value);		
		if (temp3 < 300 || temp3 > 1199)
		{
			alert('挂机最小间隔的范围是300~1199ms.');
			return;
		}	
		if (temp1 >= temp2)
		{
			alert('最大插簧时长应大于最小插簧时长.');
			return;
		}		
		if (temp3 < temp2)
		{
			alert('挂机最小间隔应大于最大插簧时长.');
			return;
		}		
		<%end if%>
		
		if (natKeepAliveMessage.checked)
		{
			natKeepAliveMessageFlag.value = "1";
		}  		
		if (changeCharEnable.checked)
		{
			changeCharEnableFlag.value = "1";
		}
		if (t38Enable.checked)
		{
			t38EnableFlag.value = "1";
		}
		if (echoCancelEnable.checked)
		{
			echoCancelEnableFlag.value = "Yes";
		}
		if (comfortnoiseEnable.checked)
		{
			comfortnoiseEnableFlag.value = "1";
		}
		if (vadEnable.checked)
		{
			vadEnableFlag.value = "Yes";
		}
		if (PrackEnable.checked)
		{
			PrackEnableFlag.value = "Yes";
		}
		if (sendUnregister.checked)
		{
			sendUnregisterFlag.value = "Yes";
		}
		if (sessiontimerEnable.checked)
		{
			sessiontimerEnableFlag.value = "1";
		}
		if (warmline0.checked)
		{
			warmline0Flag.value = "1";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 		
		if (warmline1.checked)
		{
			warmline1Flag.value = "1";
		}
	    <%end if%>		
		if (callwait0.checked)
		{
			callwait0Flag.value = "1";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 		
		if (callwait1.checked)
		{
			callwait1Flag.value = "1";
		}
	    <%end if%>
		if (transfer0.checked)
		{
			transfer0Flag.value = "Yes";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 
		if (transfer1.checked)
		{
			transfer1Flag.value = "Yes";
		}
	    <%end if%>
		if (fwdAll0.checked)
		{
			fwdAll0Flag.value = "1";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 
		if (fwdAll1.checked)
		{
			fwdAll1Flag.value = "1";
		}
	    <%end if%>
		if (fwdBusy0.checked)
		{
			fwdBusy0Flag.value = "1";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 		
		if (fwdBusy1.checked)
		{
			fwdBusy1Flag.value = "1";
		}
	    <%end if%>
		if (fwdNoAns0.checked)
		{
			fwdNoAns0Flag.value = "1";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 		
		if (fwdNoAns1.checked)
		{
			fwdNoAns1Flag.value = "1";
		}
	    <%end if%>
		if (threepartyservice0.checked)
		{
			threepartyservice0Flag.value = "Yes";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 		
		if (threepartyservice1.checked)
		{
			threepartyservice1Flag.value = "Yes";			
		}
	    <%end if%>
		if (synphonedate0.checked)
		{
			synphonedate0Flag.value = "1";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 		
		if (synphonedate1.checked)
		{
			synphonedate1Flag.value = "1";	
		}
	    <%end if%>
			
		if (cb_DigitMapEnable.checked)
		{
			DigitMapEnableFlag.value = "1";
		}
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
		if (cb_DigitMapSpecialEnable.checked)
		{
			DigitMapSpecialEnableFlag.value = "1";
			ipt_DigitMapSpecial.value = txt_DigitMapSpecial.value;
		}
		if (cb_PBXPrefixEnable.checked)
		{
			PBXPrefixEnableFlag.value = "1";
		}
		if (cb_PBXDialSecondaryEnable.checked)
		{
			PBXDialSecondaryEnableFlag.value = "1";
		}
<% end if%>
		if (siprtpredundancy.checked)
		{
			siprtpredundancyflag.value = "1";	
		}

			
		VoIP_SIP_flag.value = "1";
		submit();
	}
}

</script>
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
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="应用->VOIP设置" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
            <P class=GatewayType align=center>型号:
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
          <TD height=15><IMG height=15 src="/img/panel1.gif" width=164 
            border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 border=0> 
          </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
            <P>VOIP设置</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=13><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#宽带电话设置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0> </A></TD></TR>
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
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id=ConfigForm name="ConfigForm" action="/cgi-bin/app-VoIP_Adv.asp" method="post">
                 		<DIV id=pptp>
                  		<TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
										     <TBODY>
							                <TR>
							                    <TD>
							                        <input type='button' style="font-size:100%;width:170;height:30" onClick="applyClickBasic()" value='宽带电话基本设置'>
							                    </TD>
							                </TR>
							                <TR>
							                    <TD height="10">
							                        
							                    </TD>
							                </TR>
							                <TR>
							                    <TD class=table1_head>
							                    	<B>
							                        宽带电话高级设置
							                      </B>
							                    </TD>
							                </TR>
							                <TR>
							                    <TD height="10">
							                        
							                    </TD>
							                </TR>
							            </TBODY>
								        </TABLE>
								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
								            <TBODY>
								                <TR>
								                    <TD class=prompt>
								                        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
								                            <TBODY>
								                                <TR>
								                                    <TD style="PADDING-LEFT: 10px" width="100%"></TD>
								                                </TR>
								                            </TBODY>
								                        </TABLE>
								                    </TD>
								                </TR>
								            </TBODY>
								        </TABLE>
								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
								            <TBODY>
								                <TR>
								                    <TD height=5></TD>
								                </TR>
								            </TBODY>
							       		</TABLE>
								        <TABLE border=0 cellSpacing=1 cellPadding=0 width="98%">
								            <TBODY>
								                <TR>
								                    <TD width="35%" align=left>
								                        注册周期(秒):
								                    </TD>
								                    <TD width="65%" colSpan=2 align=left>
										                   <input type='text' name='regExpTmr' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "RegistrationExpire", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "RegistrationExpire", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD width="35%" align=left>
								                        最大启动时延(秒):
								                    </TD>
								                    <TD width="65%" colSpan=2 align=left>
										                   <input type='text' name='maxStartDelay' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "MaxStartDelay", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "MaxStartDelay", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>								          
								                <TR>
								                    <TD align=left>
								                        注册失败时再次尝试注册时间间隔:
								                    </TD>
								                    <TD align=left>
								                    	<input type='text' name='regRetryInt' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "RegisterRetryInterval", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "RegisterRetryInterval", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR style="display:none">
								                    <TD align=left>
								                        拨号规则:
								                    </TD>
								                    <TD align=left>
								                    	
<% if tcWebApi_get("WebCustom_Entry","isMultiDigitMap","h") = "Yes" then %>
								                    	<input type='text' name='dialPlan' id='dialPlan' value="">
<% else %>
								                    	<input type='text' name='dialPlan' value="<%if tcWebApi_get("VoIPAdvanced_Common", "DialPlan", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "DialPlan", "s") else asp_Write("") end if %>">
<%end if%>
								                    </TD>
								                </TR>
									               <tr >
									                  <td >拨号启动定时器时长(秒):</td>
									                  <td >
									                  	<input type='text' name='megacodmstarttime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "StartDigitTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "StartDigitTimer", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr>
									                  <td>拨号短定时器时长(秒):</td>
									                  <td>
									                  	<input type='text' name='megacodmshorttime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerShort", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerShort", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr>
									                  <td>拨号长定时器时长(秒):</td>
									                  <td>
									                  	<input type='text' name='megacodmlongtime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerLong", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerLong", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr>
									                  <td>拨号T定时器时长(秒):</td>
									                  <td>
									                  	<input type='text' name='megacodmttime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerT", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerT", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>忙音时长(秒):</td>
									                  <td>
									                  	<input type='text' name='busytonetimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "BusyToneTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "BusyToneTimer", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>嗥鸣音时长(秒):</td>
									                  <td>
									                  	<input type='text' name='hangingremindertimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "HangingReminderToneTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "HangingReminderToneTimer", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>号码表T模式:</td>
									                  <td>
									                  	<select name="dmtmode" size="1" >
													   	            <option value="0" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h") = "0" then asp_Write("selected") end if %>>拨号短定时器</option>
													   	            <option value="1" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h") = "1" then asp_Write("selected") end if %>>拨号长定时器</option>
													   	            <option value="2" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h") = "2" then asp_Write("selected") end if %>>拨号T定时器</option>
													   	        </select>
													   	      </td>
									               </tr>
									               <tr >
									                  <td>Dtmf 传输方式:</td>
									                  <td>
									                  	<select name="dtmfRelay" size="1"  onchange="dtmfRelayChange(this)">
													   	            <option value="SIPInfo" <%if tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "SIPInfo" then asp_Write("selected") end if %>>
													   	            SIPInfo
													   	            <option value="RFC2833" <%if tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "RFC2833" then asp_Write("selected") end if %>>
													   	            RFC2833
													   	            <option value="InBand" <%if tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "InBand" then asp_Write("selected") end if %>>
													   	            InBand
													   	        </select>
									                  </td>
									               </tr>
									                <tr id="trRFC2833PT">
									                  <td>Telephone Event Payload Type:</td>
									                  <td>
									                    <input type='text' size=20 maxlength=20 name='rfc2833pt' onblur='limitNum(this)' value="<%if tcWebApi_get("VoIPBasic_Common", "TelephoneEventPayloadType", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "TelephoneEventPayloadType", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>闪断转发设置:</td>
									                  <td>
									                  	<select name="hookFlashRelay" size="1">
																         <option value="SIPInfo" <%if tcWebApi_get("VoIPAdvanced_Common", "FlashOffTransSetting", "h") = "SIPInfo" then asp_Write("selected") end if %>>
																         SIPInfo
																         <option value="None" <%if tcWebApi_get("VoIPAdvanced_Common", "FlashOffTransSetting", "h") = "None" then asp_Write("selected") end if %>>
																         None
																      </select>
																    </td>
									               </tr>
									               <tr style="display:none">
									                  <td>SIP 传输协议:</td>
									                  <td>
									                  	<select name="transport" size="1">
												    	            <option value="UDP" <%if tcWebApi_get("VoIPBasic_Common", "SIPTransportProtocol", "h") = "UDP" then asp_Write("selected") end if %>>
																         	UDP
																         	<option value="TCP" <%if tcWebApi_get("VoIPBasic_Common", "SIPTransportProtocol", "h") = "TCP" then asp_Write("selected") end if %>>
																         	TCP
												    	        </select>
									                  </td>
									               </tr>
												  			 <tr>
									                  <td>心跳消息启用:</td>
									                  <td>
									                		<input type='checkbox' name='natKeepAliveMessage' <%if tcWebApi_get("VoIPBasic_Common", "HeartbeatSwitch", "h") = "1" then asp_Write("checked") end if %>>
									                  </td>
									               </tr>
									               <tr >
									                  <td>心跳消息时长(秒):</td>
									                  <td>
									                  	<input type='text' name='natKeepAliveTime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "HeartbeatCycle", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "HeartbeatCycle", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <% if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes"  then %>
									               <tr >
									                  <td>构造From To头域URI服务器:</td>
									                  <td>
									                  	<select name="fromtoheadurl" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "FromToHeadURI", "h") = "0" then asp_Write("selected") end if %>>注册服务器</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "FromToHeadURI", "h") = "1" then asp_Write("selected") end if %>>代理服务器</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "FromToHeadURI", "h") = "2" then asp_Write("selected") end if %>>出局服务器</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <% end if %>
									               <tr >
									                  <td>传真协商模式:</td>
									                  <td>
									                  	<select name="faxctrlmode" size="1">
											   	                <option value="other" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "other" then asp_Write("selected") end if %>>自协商</option>
											   	                <option value="all" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "all" then asp_Write("selected") end if %>>T30全控</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <tr >
									                  <td>传真透传编解码:</td>
									                  <td>
									                  	<select name="faxpassthru" size="1">
											   	                <option value="PCMU" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxPassThruCodec", "h") = "PCMU" then asp_Write("selected") end if %>>PCMU</option>
											   	                <option value="PCMA" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxPassThruCodec", "h") = "PCMA" then asp_Write("selected") end if %>>PCMA</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <tr >
									               <td>RTP冗余:</td>
									               <td>
									                      <input name="siprtpredundancy" type="checkbox" <%if tcWebApi_get("VoIPAdvanced_Common", "rtpRedundantEnable", "h") = "1" then asp_Write("checked") end if %>>
									               </td>
									               </tr>
									               <tr>
									               <td>RTP冗余paylod type:</td>
									               <td>
									                      <input name="siprtpredundancypt" type="text" id="sip_rtp_redundancy_pt_val_1" onblur="limitNum(this)"  value="<%if tcWebApi_get("VoIPAdvanced_Common", "rtpRedundantPT", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "rtpRedundantPT", "s") else asp_Write("") end if %>">
									               &nbsp;(96-127)
									               </td>
									               </tr> 
									               <tr >
									                  <td>热线实现方式:</td>
									                  <td>
									                  	<select name="DivHotLineType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "HotLineMode", "h") = "0" then asp_Write("selected") end if %>>上报用户号码</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "HotLineMode", "h") = "1" then asp_Write("selected") end if %>>上报NoDial</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "HotLineMode", "h") = "2" then asp_Write("selected") end if %>>上报激活码</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <tr >
									                  <td>注册请求是否带端口号：</td>
									                  <td>
									                  	<select name="DivIsRegisterWithPort" size="1">
											   	                <option value="Yes" <%if tcWebApi_get("VoIPAdvanced_Common", "RegResponsePortEnable", "h") = "Yes" then asp_Write("selected") end if %>>不带端口号</option>
											   	                <option value="No" <%if tcWebApi_get("VoIPAdvanced_Common", "RegResponsePortEnable", "h") = "No" then asp_Write("selected") end if %>>带端口号</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>报文是否带"user=phone"：</td>
									                  <td>
									                  	<select name="DivIsMsgWithUser" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "MsgWithUserEnable", "h") = "0" then asp_Write("selected") end if %>>不带user=phone</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "MsgWithUserEnable", "h") = "1" then asp_Write("selected") end if %>>带user=phone</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>热线激活码:</td>
									                  <td>
									                  	<input type='text' name='DivHotLineActiveCode' value="<%if tcWebApi_get("VoIPAdvanced_Common", "HotLineActNum", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "HotLineActNum", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>呼叫等待类型：</td>
									                  <td>
									                  	<select name="DivCWType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "CallWaitType", "h") = "0" then asp_Write("selected") end if %>>直接切换</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "CallWaitType", "h") = "1" then asp_Write("selected") end if %>>加按数字键切换</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>未注册拨号音开/关:</td>
									                  <td>
									                  	<select name="DivDialtone" size="1">
											   	                <option value="Yes" <%if tcWebApi_get("VoIPAdvanced_Common", "NoRegDialToneEnable", "h") = "Yes" then asp_Write("selected") end if %>>关闭拨号音</option>
											   	                <option value="No" <%if tcWebApi_get("VoIPAdvanced_Common", "NoRegDialToneEnable", "h") = "No" then asp_Write("selected") end if %>>启用拨号音</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>极性反转方式：</td>
									                  <td>
									                  	<select name="DivRevPolarityType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "0" then asp_Write("selected") end if %>>关闭</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "1" then asp_Write("selected") end if %>>本地打开</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "2" then asp_Write("selected") end if %>>服务器控制</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>语音编码优先级控制：</td>
									                  <td>
									                  	<select name="DivCodecSelectFlag" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "VoiceCodecPriorityCtrl", "h") = "0" then asp_Write("selected") end if %>>主叫优先</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "VoiceCodecPriorityCtrl", "h") = "1" then asp_Write("selected") end if %>>本地优先</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>号码匹配方式：</td>
									                  <td>
									                  	<select name="DivDigitMatchMethod" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "NumberMatchMode", "h") = "0" then asp_Write("selected") end if %>>最小匹配</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "NumberMatchMode", "h") = "1" then asp_Write("selected") end if %>>模糊最大匹配</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "NumberMatchMode", "h") = "2" then asp_Write("selected") end if %>>最大匹配</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>订阅类型：</td>
									                  <td>
									                  	<select name="DivSubscribeType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "SubscribeType", "h") = "0" then asp_Write("selected") end if %>>不订阅</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "SubscribeType", "h") = "1" then asp_Write("selected") end if %>>订阅业务</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "SubscribeType", "h") = "2" then asp_Write("selected") end if %>>订阅注册</option>
											   	                <option value="3" <%if tcWebApi_get("VoIPAdvanced_Common", "SubscribeType", "h") = "3" then asp_Write("selected") end if %>>订阅全部</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>订阅周期(秒):</td>
									                  <td>
									                  	<input type='text' name='DivSubscribeExpire' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "SubscribeExpire", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "SubscribeExpire", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>									               
									               <tr >
									                  <td>补充业务类型：</td>
									                  <td>
									                  	<select name="DivSupplementServiceType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "0" then asp_Write("selected") end if %>>None</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "1" then asp_Write("selected") end if %>>Local</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "2" then asp_Write("selected") end if %>>INFO</option>
											   	                <option value="3" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "3" then asp_Write("selected") end if %>>IMS</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									               		<td>支持#转换%23:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='changeCharEnable' ID="Checkbox6" value='YES' <%if tcWebApi_get("VoIPAdvanced_Common", "SupportSpecCharTransfer", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>支持T38:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='t38Enable' ID="Checkbox7" value='1'  <%if tcWebApi_get("VoIPMedia_Common", "FaxCodec", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>回音消除启用:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='echoCancelEnable' ID="Checkbox8" value='YES' <%if tcWebApi_get("VoIPMedia_Common", "EchoCancellationEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>舒适噪音启用:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='comfortnoiseEnable' ID="Checkbox81" value='YES' <%if tcWebApi_get("VoIPMedia_Entry0", "VAD", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>静音压缩启用:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='vadEnable' ID="Checkbox82" value='YES' <%if tcWebApi_get("VoIPMedia_Entry0", "SilenceCompressionEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>支持Prack:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='PrackEnable' ID="Checkbox9" value='YES' <%if tcWebApi_get("VoIPBasic_Common", "PRACKEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>是否先发送注销消息:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='sendUnregister' ID="Checkbox10" value='YES' <%if tcWebApi_get("VoIPBasic_Common", "LogOffMsgSendEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>启用会话更新周期:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='sessiontimerEnable' onClick='sessionClick(this)' ID="Checkbox11" value='0' <%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_FLAG", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr id="sesstimer">
									                  <td>会话更新周期:</td>
									                  <td>
									                  	<input type='text' name='sessionTimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", "s") else asp_Write("") end if %>">
									                  	&nbsp;秒
									                  </td>
									               </tr>
									               <tr id="sessminse">
									                  <td>最小会话更新周期:</td>
									                  <td>
									                  	<input type='text' name='sessionMinSE' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_MIN_EXP", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_MIN_EXP", "s") else asp_Write("") end if %>">
									                  	&nbsp;秒
									                  </td>
									               </tr>
									               <tr id="sessrefresher" >
									                  <td>会话更新方:</td>
									                  <td>
									                  	<select name="sessionRefresher" size="1">
													               <option value="0" <%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_REFRESHER", "h") = "0" then asp_Write("selected") end if %>>
																         none
																         <option value="1" <%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_REFRESHER", "h") = "1" then asp_Write("selected") end if %>>
																         uac
																         <option value="2" <%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_REFRESHER", "h") = "2" then asp_Write("selected") end if %>>
																         uas
														          </select>
									                  </td>
									               </tr>									               
									               <tr id="sessmethod" >
									                  <td>会话更新消息类型:</td>
									                  <td>
									                  	<select name="sessionMethod" size="1">
													               <option value="0" <%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_METHOD", "h") = "0" then asp_Write("selected") end if %>>
																         UPDATE
																         <option value="1" <%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_METHOD", "h") = "1" then asp_Write("selected") end if %>>
																         INVITE
														          </select>
									                  </td>
									               </tr>
									                <tr>
  									                <td>TOS/DSCP优先级(SIP):</td>
  									                <td>
  									                  <input type='text' size=20 maxlength=20 name='dscpmark' onblur='limitNum(this)' value="<%if tcWebApi_get("VoIPBasic_Common", "SIPDSCPMark", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SIPDSCPMark", "s") else asp_Write("") end if %>">
  									                </td>
									               </tr>
									               <tr>
										               <td>TOS/DSCP优先级(RTP):</td>
										               <td><input name="mymgtos" type="text" id="in_h_tos_val" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "RTPDSCPMark", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "RTPDSCPMark", "s") else asp_Write("") end if %>"></td>
									               </tr>
									             </TBODY>
										        </TABLE>
										        <div id="perLineTablePlaceholder1">
										        </div>
										        <TABLE border=0 cellSpacing=1 cellPadding=0 width="98%">
										        	<tr>
										        		<td height="10">
										        		</td>
										        	</tr>
										        	<tr>
					                        <td align="center">
					                        	 <input type='hidden' value='0' name="VoIP_SIP_flag">
					                        	 <input type='hidden' value='0' name="natKeepAliveMessageFlag">
					                        	 <input type='hidden' value='0' name="changeCharEnableFlag">
					                        	 <input type='hidden' value='0' name="t38EnableFlag">
					                        	 <input type='hidden' value='No' name="echoCancelEnableFlag">
					                        	 <input type='hidden' value='No' name="comfortnoiseEnableFlag">
					                        	 <input type='hidden' value='No' name="vadEnableFlag">
					                        	 <input type='hidden' value='No' name="PrackEnableFlag">
					                        	 <input type='hidden' value='No' name="sendUnregisterFlag">
					                        	 <input type='hidden' value='0' name="sessiontimerEnableFlag">
					                        	 <input type='hidden' value='0' name="warmline0Flag">
					                        	 <input type='hidden' value='0' name="warmline1Flag">
					                        	 <input type='hidden' value='0' name="callwait0Flag">
					                        	 <input type='hidden' value='0' name="callwait1Flag">
					                        	 <input type='hidden' value='No' name="transfer0Flag">
					                        	 <input type='hidden' value='No' name="transfer1Flag">
					                        	 <input type='hidden' value='0' name="fwdAll0Flag">
					                        	 <input type='hidden' value='0' name="fwdAll1Flag">
					                        	 <input type='hidden' value='0' name="fwdBusy0Flag">
					                        	 <input type='hidden' value='0' name="fwdBusy1Flag">
					                        	 <input type='hidden' value='0' name="fwdNoAns0Flag">
					                        	 <input type='hidden' value='0' name="fwdNoAns1Flag">
					                        	 <input type='hidden' value='No' name="threepartyservice0Flag">
					                        	 <input type='hidden' value='No' name="threepartyservice1Flag">
					                        	 <input type='hidden' value='0' name="synphonedate0Flag">
					                        	 <input type='hidden' value='0' name="synphonedate1Flag">
<input type="hidden" name="isMultiDigitMap" value="<% if tcWebApi_get("WebCustom_Entry","isMultiDigitMap","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<% if tcWebApi_get("WebCustom_Entry","isMultiDigitMap","h") = "Yes" then %>
<input type='hidden' id='DigitMap1' name="DigitMap1" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap1", "s")%>">
<input type='hidden' id='DigitMap2' name="DigitMap2" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap2", "s")%>">
<input type='hidden' id='DigitMap3' name="DigitMap3" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap3", "s")%>">
<input type='hidden' id='DigitMap4' name="DigitMap4" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap4", "s")%>">
<input type='hidden' id='DigitMap5' name="DigitMap5" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap5", "s")%>">
<input type='hidden' id='DigitMap6' name="DigitMap6" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap6", "s")%>">
<input type='hidden' id='DigitMap7' name="DigitMap7" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap7", "s")%>">
<input type='hidden' id='DigitMap8' name="DigitMap8" value="<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMap8", "s")%>">
<% end if%>
					                        	 <input type='hidden' value='0' name="DigitMapEnableFlag">
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>
					                        	 <input type='hidden' value='0' name="DigitMapSpecialEnableFlag">
					                        	 <input type='hidden' value='0' name="PBXPrefixEnableFlag">
					                        	 <input type='hidden' value='0' name="PBXDialSecondaryEnableFlag">
<% end if%>
					                        	 <input type='hidden' value='0' name="siprtpredundancyflag">
					                        </td>
					                    </tr>
					                    <tr>
					                        <td align="center">
					                          <input type='button' onClick="btnApplySip()" value='应用' ID="ApplyButton">
					                        </td>
					                    </tr>
										        </TABLE>
										      </DIV>
                    </FORM>
                  </TD>
                </TR>
              </TBODY>
            </TABLE>
          </TD>
        </TR>      
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>&nbsp;</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE>
      </TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>　</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2><!-- InstanceEndEditable --></TD>
          <TD width=170 
bgColor=#313031  class=CucBgColorTD_2>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
