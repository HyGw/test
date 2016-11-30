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
	tcWebApi_set("VoIPDigitMap_Entry", "NoAnswerTimer", "noanswertimer")
	tcWebApi_set("VoIPDigitMap_Entry", "TMode", "dmtmode")
	tcWebApi_set("VoIPAdvanced_Common", "DTMFTransportMode", "dtmfRelay")
if request_Form("dtmfRelay") = "RFC2833" then
	tcWebApi_set("VoIPBasic_Common", "TelephoneEventPayloadType", "rfc2833pt")
end if
	tcWebApi_set("VoIPAdvanced_Common", "FlashOffTransSetting", "hookFlashRelay")
	tcWebApi_set("VoIPBasic_Common", "HeartbeatSwitch", "natKeepAliveMessageFlag")
	tcWebApi_set("VoIPBasic_Common", "HeartbeatCycle", "natKeepAliveTime")
	tcWebApi_set("VoIPAdvanced_Common", "OptionsSericeMode", "optionssericemode")
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
	tcWebApi_set("VoIPAdvanced_Common", "ActLocalSupplementService", "DivSupplementServiceType")
	tcWebApi_set("VoIPAdvanced_Common", "EndCharHandleMode", "EndCharacterHandleMode")
	tcWebApi_set("VoIPAdvanced_Common", "SupportSpecCharTransfer", "changeCharEnableFlag")
	tcWebApi_set("VoIPAdvanced_Common", "SupReferSpecCharTrans", "changeReferCharEnableFlag")
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
	tcWebApi_set("VoIPAdvanced_Common", "CallSynEnAutoSwitch", "autoSynPhoneDateSwitchFlag")
	tcWebApi_set("VoIPAdvanced_Common", "SyncSysTimeEnable", "syncSystemDateFlag")
	tcWebApi_set("VoIPAdvanced_Common", "SC_SYS_VOICE_JB_TYPE", "AJBEnableFlag")
	tcWebApi_set("VoIPAdvanced_Common", "SC_SYS_VOICE_JB_LEN", "JBMaxLen")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_FLAG", "sessiontimerEnableFlag")
if request_Form("sessiontimerEnableFlag") = "1" then
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", "sessionTimer")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_MIN_EXP", "sessionMinSE")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_REFRESHER", "sessionRefresher")
	tcWebApi_set("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_METHOD", "sessionMethod")
end if
	tcWebApi_set("VoIPBasic_Common", "VoIP8021PMarkEnable", "VoIP8021pEnableFlag")
if request_Form("VoIP8021pEnableFlag") = "1" then
	tcWebApi_set("VoIPBasic_Common", "SIP8021PMark", "VoIP8021PSIP")
	tcWebApi_set("VoIPBasic_Common", "RTP8021PMark", "VoIP8021PRTP")
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
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIP3wayConfServerMix", "threepartyservermix0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIP3wayConfServAddr", "threepartyserviceadd0")	
	tcWebApi_set("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "calleridmode0")
	tcWebApi_set("VoIPAdvanced_Entry0", "SubscribeType", "DivSubscribeType0")
	tcWebApi_set("VoIPAdvanced_Entry0", "SubscribeExpire", "DivSubscribeExpire0")	
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
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIP3wayConfServerMix", "threepartyservermix1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIP3wayConfServAddr", "threepartyserviceadd1")	
	tcWebApi_set("VoIPCallCtrl_Entry1", "ComingCallDisplayMode", "calleridmode1")
	tcWebApi_set("VoIPAdvanced_Entry1", "SubscribeType", "DivSubscribeType1")
	tcWebApi_set("VoIPAdvanced_Entry1", "SubscribeExpire", "DivSubscribeExpire1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookMinInterval", "minhookinterval1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookMaxInterval", "maxhookinterval1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookReleaseMin", "minhookrelease1")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SyncCallerTimeEnable", "synphonedate1Flag")	
	End If
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMapEnable", "DigitMapEnableFlag")
		
	tcWebApi_set("VoIPDigitMap_Entry", "DigitMapSpecialEnable", "DigitMapSpecialEnableFlag")
	if request_Form("DigitMapSpecialEnableFlag") = "1" then	
		tcWebApi_set("VoIPDigitMap_Entry", "DigitMapSpecial", "ipt_DigitMapSpecial")
	end if
	tcWebApi_set("VoIPDigitMap_Entry", "PBXPrefixEnable", "PBXPrefixEnableFlag")
	if request_Form("PBXPrefixEnableFlag") = "1" then	
		tcWebApi_set("VoIPDigitMap_Entry", "PBXPrefix", "ipt_PBXPrefix")
	end if	
	tcWebApi_set("VoIPDigitMap_Entry", "PBXDialSecondaryEnable", "PBXDialSecondaryEnableFlag")	

	tcWebApi_commit("VoIPBasic_Common")
	tcWebApi_save()
end if
end if
%>
<HTML><HEAD><TITLE>VOIP����</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="javascript">
<%
If request_Form("VoIP_SIP_flag")="1" then
	if tcWebApi_get("WebCurSet_Entry", "VoIPIdle", "h") <> "1"  then
		asp_write("alert('����绰ͨ���У����ܱ��棬��ͨ���������ٱ��档');")
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
			alert("���Ź������󳤶Ȳ��ܳ���4096������");
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
   	   if("2" != "<%tcWebApi_get("VoIPAdvanced_Common", "SC_SYS_VOICE_JB_TYPE", "s") %>")
   	{
   	  document.getElementById("JB_LEN").style.display = "";
   	}
   else
   	{
   	  document.getElementById("JB_LEN").style.display = "none";
   	}
	   	 
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
		
      if ("1" == "<%tcWebApi_get("VoIPBasic_Common", "VoIP8021PMarkEnable", "s") %>")
      {
	   	  document.getElementById("8021psip").style.display = "";
	   	  document.getElementById("8021prtp").style.display = "";      	
      }
	  else
	  {
	   	  document.getElementById("8021psip").style.display = "none";
	   	  document.getElementById("8021prtp").style.display = "none";      	
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

	getElById('txt_DigitMapSpecial').value=getElById('ipt_DigitMapSpecial').value;	
	CheckDigitMapSpecialEnable(getElById('cb_DigitMapSpecialEnable'));
	CheckPBXPrefixEnable(getElById('cb_PBXPrefixEnable'));	
<% if tcWebApi_get("WebCustom_Entry", "isCTSCSupported", "h") = "Yes"  then %>	
<% if tcwebApi_get("WanInfo_Common","NoGUIAccessLimit","h" ) <> "1" then %>
	getElById('txt_DigitMap').disabled = true;
<% end if %>
<%end if%>
}	

function doCheckSelect(obj,gid){
	if (obj.checked)
	{
	    var sid="callwait" + gid;
	    setCheck(sid,0);
	    sid="fwdBusy" + gid;
	    setCheck(sid,0);
	    sid="fwdNoAns" + gid;
	    setCheck(sid,0);
	    sid="fwdAll" + gid;
	    setCheck(sid,0);
	    obj.checked = true;
	}
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
      tableHtmlOutput += "<td >ptime ���� <\/td>"
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
      tableHtmlOutput += "<b>������С����(db)</b><br>";
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
      tableHtmlOutput += "<td >��������<\/td>"
     	tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'ingressGain0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>\'></td>";    
     	<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
     	tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'ingressGain1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>\'></td>";    
      <% end if %>
      tableHtmlOutput += "</tr>";
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >��������<\/td>"
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
      tableHtmlOutput += "<b>����ҵ������</b><br>";
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
      tableHtmlOutput += "<td >��������<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'warmline0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'warmline1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Warm Line Number row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >���ߺ���<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinenum0\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HotLineNumber", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinenum1\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HotLineNumber", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Warm Line Timeout row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >�����ӳ�ʱ��(��)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinetimeout0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineDelayTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HotLineDelayTime", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinetimeout1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineDelayTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HotLineDelayTime", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call waiting row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >���ú��еȴ�<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'callwait0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIPCallWaitingEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,0)'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'callwait1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIPCallWaitingEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,1)'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      /*zhangcg: 20110612*/
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >���ú���ת��<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'transfer0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIPCallTransfer", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'transfer1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIPCallTransfer", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding number row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >����ǰת����<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNum0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "ForwardNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "ForwardNumber", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNum1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "ForwardNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "ForwardNumber", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding unconditionally row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >����������ǰת<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdAll0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKUCFEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,0)'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdAll1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKUCFEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,1)'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding on "busy" row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >��æ����ǰת<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdBusy0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKBCFEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,0)'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdBusy1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKBCFEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,1)'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      // Call forwarding on "no answer" row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >��Ӧ�����ǰת<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdNoAns0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKNCFEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,0)'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdNoAns1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKNCFEnable", "h") = "1" then asp_Write("checked") end if %> onclick='doCheckSelect(this,1)'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";
	  
	  // Call forwarding on "no answer time"
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >��Ӧ�����ǰת�ȴ�ʱ��<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNoAnsTime0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNoAnsTime1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >������������<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservice0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIP3wayConf", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservice1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIP3wayConf", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >���÷���������<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservermix0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIP3wayConfServerMix", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservermix1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIP3wayConfServerMix", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >����������Դ������<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'threepartyserviceadd0\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIP3wayConfServAddr", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "SIP3wayConfServAddr", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'threepartyserviceadd1\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIP3wayConfServAddr", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "SIP3wayConfServAddr", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";	
	  
    //caller id mode
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >������ʾģʽ <\/td>"
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

      //subscribe type
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >�������� <\/td>"
      var selectOptions0 = Array();
      selectOptions0[0] = "<option value=" + 0 + " <%if tcWebApi_get("VoIPAdvanced_Entry0", "SubscribeType", "h") = "0" then asp_Write("selected") end if %>>" + "������";
      selectOptions0[1] = "<option value=" + 1 + " <%if tcWebApi_get("VoIPAdvanced_Entry0", "SubscribeType", "h") = "1" then asp_Write("selected") end if %>>" + "����ҵ��";   
      selectOptions0[2] = "<option value=" + 2 + " <%if tcWebApi_get("VoIPAdvanced_Entry0", "SubscribeType", "h") = "2" then asp_Write("selected") end if %>>" + "����ע��";
      selectOptions0[3] = "<option value=" + 3 + " <%if tcWebApi_get("VoIPAdvanced_Entry0", "SubscribeType", "h") = "3" then asp_Write("selected") end if %>>" + "����ȫ��";   
      tableHtmlOutput += "<td align=center><select name='DivSubscribeType0'>" + selectOptions0.join("") + "<\/select></td>";
                  
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>       
      var selectOptions1 = Array();
      selectOptions1[0] = "<option value=" + 0 + " <%if tcWebApi_get("VoIPAdvanced_Entry1", "SubscribeType", "h") = "0" then asp_Write("selected") end if %>>" + "������";
      selectOptions1[1] = "<option value=" + 1 + " <%if tcWebApi_get("VoIPAdvanced_Entry1", "SubscribeType", "h") = "1" then asp_Write("selected") end if %>>" + "����ҵ��";   
      selectOptions1[2] = "<option value=" + 2 + " <%if tcWebApi_get("VoIPAdvanced_Entry1", "SubscribeType", "h") = "2" then asp_Write("selected") end if %>>" + "����ע��";
      selectOptions1[3] = "<option value=" + 3 + " <%if tcWebApi_get("VoIPAdvanced_Entry1", "SubscribeType", "h") = "3" then asp_Write("selected") end if %>>" + "����ȫ��";   
      tableHtmlOutput += "<td align=center><select name='DivSubscribeType1'>" + selectOptions1.join("") + "<\/select></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >��������(��)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'DivSubscribeExpire0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry0", "SubscribeExpire", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "SubscribeExpire", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'DivSubscribeExpire1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry1", "SubscribeExpire", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "SubscribeExpire", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";


	//flash hook interval 
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >�Ĳ����Сʱ����(����)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookinterval0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookMinInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookMinInterval", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookinterval1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookMinInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookMinInterval", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      tableHtmlOutput += "</tr>";

	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >�Ĳ�����ʱ����(����)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'maxhookinterval0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookMaxInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookMaxInterval", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'maxhookinterval1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookMaxInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookMaxInterval", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>      
      tableHtmlOutput += "</tr>";
	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >�һ���Сʱ����(����)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookrelease0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookReleaseMin", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookReleaseMin", "s") else asp_Write("") end if %>\'></td>";
      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookrelease1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookReleaseMin", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookReleaseMin", "s") else asp_Write("") end if %>\'></td>";
      <% end if %>
      
      tableHtmlOutput += "</tr>";
	//Synchronize phone date time
	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >����ͬ������ʱ��<\/td>"
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
      tableHtmlOutput += "<b>��ͼ����</b><br>";
      tableHtmlOutput += "<table id='Table_DigitMap' border='1'><tbody id='Tbody_DigitMap'>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >������ͨ��ͼ<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 id=\'cb_DigitMapEnable\' name=\'cb_DigitMapEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "DigitMapEnable", "h") = "1" then asp_Write("checked") end if %> onclick='CheckDigitMapEnable(this)'></td>";
      tableHtmlOutput += "</tr>";
      
      tableHtmlOutput += "<tr id='sec_DigitMap'>";
      tableHtmlOutput += "<td align=center colSpan=2><textarea id=\'txt_DigitMap\' name=\'txt_DigitMap\' rows=4 maxlength=4096 wrap='OFF' style='WIDTH:420px;'></textarea></td>";
      tableHtmlOutput += "</tr>"; 
         
      tableHtmlOutput += "<tr>";      
      tableHtmlOutput += "<td >�����ض���ͼ<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 id=\'cb_DigitMapSpecialEnable\' name=\'cb_DigitMapSpecialEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "DigitMapSpecialEnable", "h") = "1" then asp_Write("checked") end if %> onclick='CheckDigitMapSpecialEnable(this)'></td>";
      tableHtmlOutput += "</tr>";
      
      tableHtmlOutput += "<tr id='sec_DigitMapSpecial'>";
      tableHtmlOutput += "<td align=center colSpan=2><textarea id=\'txt_DigitMapSpecial\' name=\'txt_DigitMapSpecial\' rows=1 maxlength=512 wrap='OFF' style='WIDTH:420px;'></textarea></td>";
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr style='display:none'>";
      tableHtmlOutput += "<td align=center colSpan=2><input type='text' value='<%tcWebAPI_get("VoIPDigitMap_Entry", "DigitMapSpecial", "s")%>' name='ipt_DigitMapSpecial' id='ipt_DigitMapSpecial'></td>";
      tableHtmlOutput += "</tr>";
            
      tableHtmlOutput += "<tr>";      
      tableHtmlOutput += "<td >����PBXǰ׺<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 id=\'cb_PBXPrefixEnable\' name=\'cb_PBXPrefixEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "PBXPrefixEnable", "h") = "1" then asp_Write("checked") end if %> onclick='CheckPBXPrefixEnable(this)'></td>";
      tableHtmlOutput += "</tr>";
      
      tableHtmlOutput += "<tr id='sec_PBXPrefix'>";
      tableHtmlOutput += "<td >PBXǰ׺<\/td>"    
      tableHtmlOutput += "<td align=center><input type='text' size=12 maxlength=8 name=\'ipt_PBXPrefix\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPDigitMap_Entry", "PBXPrefix", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "PBXPrefix", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";
                  
      tableHtmlOutput += "<tr>";      
      tableHtmlOutput += "<td >PBX���ö��β�������ʾ<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'cb_PBXDialSecondaryEnable\' <%if tcWebApi_get("VoIPDigitMap_Entry", "PBXDialSecondaryEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";

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
			alert('"' + o.value + '"' + 'ֻ����������.');	
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

function AJBClick(cb){
   if(cb.checked == true )
   	{
   	  document.getElementById("JB_LEN").style.display = "";
   	}
   else
   	{
   	  document.getElementById("JB_LEN").style.display = "none";
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

function Priority8021PClick(cb){
   if(cb.checked == true )
   	{
	  document.getElementById("8021psip").style.display = "";
	  document.getElementById("8021prtp").style.display = "";        	
   	}
   else
   	{
   	  document.getElementById("8021psip").style.display = "none";
   	  document.getElementById("8021prtp").style.display = "none";
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
    var regDigitMap = /[^0-9\[\]\-\|\.\*#xXABCDTLStlsEF]/g;
    
		if (cb_DigitMapSpecialEnable.checked)
		{
			if(regDigitMap.test(getElById('txt_DigitMapSpecial').value)){
				alert("�ض���ͼ���в��Ϸ��ַ�!");
				return;
			}
		}
		if (cb_DigitMapEnable.checked){		
			if(regDigitMap.test(getElById('txt_DigitMap').value)){
				alert("��ͨ��ͼ���в��Ϸ��ַ�!");
				return;
			}
			getElById('dialPlan').value=getElById('txt_DigitMap').value;
		}
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
	          alert("�����������ȼ������ظ�������������!");
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
		    alert("����ȫ��������������,������ѡ��һ��!");
				return;
			}
	  }
	   
	  var temp = parseInt(ingressGain0.value);	
		if (temp < -12 || temp > 12)
		{
			alert('���������ķ�Χ��-12~12db.');
			return;
		}
		
	  <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>   
	  var temp = parseInt(ingressGain1.value);	
		if (temp < -12 || temp > 12)
		{
			alert('���������ķ�Χ��-12~12db.');
			return;
		}		
	    <%end if%>
			
		var temp = parseInt(egressGain0.value);	
		if (temp < -12 || temp > 12)
		{
			alert('���������ķ�Χ��-12~12db.');
			return;
		} 

		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>   
		var temp = parseInt(egressGain1.value);	
		if (temp < -12 || temp > 12)
		{
			alert('���������ķ�Χ��-12~12db.');
			return;
		} 		
	    <%end if%>
			
		var temp = parseInt(rfc2833pt.value);	
		if (temp < 96 || temp > 127)
		{
			alert('Telephone Event Payload Type�ķ�Χ��96~127.');
			return;
		}
		var temp = parseInt(JBMaxLen.value);	
		if (temp < 1 || temp > 1000)
		{
			alert('jitter buffer ���ֵ�ķ�Χ��1~1000.');
			return;
		}
		
		var temp = parseInt(siprtpredundancypt.value);	
		if (temp < 96 || temp > 127)
		{
			alert('RTP����payload type��Χ��96~127.');
			return;
		}		

		var temp = parseInt(VoIP8021PSIP.value);	
		if (temp < 0 || temp > 7)
		{
			alert('SIP 802.1p��Χ��0~7.');
			return;
		}	

		var temp = parseInt(VoIP8021PRTP.value);	
		if (temp < 0 || temp > 7)
		{
			alert('RTP 802.1p��Χ��0~7.');
			return;
		}	
		
 		var temp1 = parseInt(sessionTimer.value);	
		if (temp1 < 90)
		{
			alert('�Ựˢ������Ӧ������90s.');
			return;
		}
		
 		var temp2 = parseInt(sessionMinSE.value);	
		if (temp2 < 90)
		{
			alert('��С�Ựˢ������Ӧ������90s.');
			return;
		}	
		
		if (temp1 < temp2)
		{
			alert ('�Ựˢ������Ӧ��������С�Ựˢ������.');	
			return;
		}

		var temp = parseInt (dscpmark.value);
		if (temp < 0 || temp > 63)
		{
			alert ('SIP DSCP���ȼ��ķ�Χ��0~63.');
			return;
		}
	
		var temp = parseInt (mymgtos.value);
		if (temp < 0 || temp > 63)
		{
			alert ('RTP DSCP���ȼ��ķ�Χ��0~63.');
			return;
		}

		var temp1 = parseInt (minhookinterval0.value);
		var temp2 = parseInt (maxhookinterval0.value);
		var temp3 = parseInt (minhookrelease0.value);		
		if (temp3 < 300 || temp3 > 1199)
		{
			alert('�һ���С����ķ�Χ��300~1199ms.');
			return;
		}	
		if (temp1 >= temp2)
		{
			alert('�����ʱ��Ӧ������С���ʱ��.');
			return;
		}		
		if (temp3 < temp2)
		{
			alert('�һ���С���Ӧ���������ʱ��.');
			return;
		}
		
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>   
		var temp1 = parseInt (minhookinterval1.value);
		var temp2 = parseInt (maxhookinterval1.value);
		var temp3 = parseInt (minhookrelease1.value);		
		if (temp3 < 300 || temp3 > 1199)
		{
			alert('�һ���С����ķ�Χ��300~1199ms.');
			return;
		}	
		if (temp1 >= temp2)
		{
			alert('�����ʱ��Ӧ������С���ʱ��.');
			return;
		}		
		if (temp3 < temp2)
		{
			alert('�һ���С���Ӧ���������ʱ��.');
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
		if (changeReferCharEnable.checked)
		{
			changeReferCharEnableFlag.value = "1";
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
		if (autoSynPhoneDateSwitch.checked)
		{
			autoSynPhoneDateSwitchFlag.value = "1";
		}
		if (syncSystemDate.checked)
		{
			syncSystemDateFlag.value = "Yes";
		}
		if (AJBEnable.checked)
		{
			AJBEnableFlag.value = "1";
		}	
		else
		{
			AJBEnableFlag.value = "2";
			JBMaxLen.value="400";
			
		}	
		if (VoIP8021pEnable.checked)
		{
			VoIP8021pEnableFlag.value = "1";
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
		if (threepartyservermix0.checked)
		{
			threepartyservermix0Flag.value = "Yes";
		}
		<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %> 		
		if (threepartyservermix1.checked)
		{
			threepartyservermix1Flag.value = "Yes";			
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
            value="Ӧ��->VOIP����" name=Selected_Menu> </FONT></B><SPAN 
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
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>VOIP����</P></TD>
          <TD width=7 bgColor=#ef8218>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=13><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#����绰����" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0> </A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isWebTYLOGOSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>  
              </TBODY></TABLE></TD></TR>
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
                  <FORM id=ConfigForm name="ConfigForm" action="/cgi-bin/app-VoIP_Adv.asp" method="post">
                 		<DIV id=pptp>
                  		<TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
										     <TBODY>
							                <TR>
							                    <TD>
							                        <input type='button' style="font-size:100%;width:170;height:30" onClick="applyClickBasic()" value='����绰��������'>
							                    </TD>
							                </TR>
							                <TR>
							                    <TD height="10">
							                        
							                    </TD>
							                </TR>
							                <TR>
							                    <TD class=table1_head>
							                    	<B>
							                        ����绰�߼�����
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
								                        ע������(��):
								                    </TD>
								                    <TD width="65%" colSpan=2 align=left>
										                   <input type='text' name='regExpTmr' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "RegistrationExpire", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "RegistrationExpire", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD width="35%" align=left>
								                        �������ʱ��(��):
								                    </TD>
								                    <TD width="65%" colSpan=2 align=left>
										                   <input type='text' name='maxStartDelay' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "MaxStartDelay", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "MaxStartDelay", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD align=left>
								                        ע��ʧ��ʱ�ٴγ���ע��ʱ����:
								                    </TD>
								                    <TD align=left>
								                    	<input type='text' name='regRetryInt' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "RegisterRetryInterval", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "RegisterRetryInterval", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR style="display:none">
								                    <TD align=left>
								                        ���Ź���:
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
									                  <td >����������ʱ��ʱ��(��):</td>
									                  <td >
									                  	<input type='text' name='megacodmstarttime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "StartDigitTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "StartDigitTimer", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr>
									                  <td>���Ŷ̶�ʱ��ʱ��(��):</td>
									                  <td>
									                  	<input type='text' name='megacodmshorttime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerShort", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerShort", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr>
									                  <td>���ų���ʱ��ʱ��(��):</td>
									                  <td>
									                  	<input type='text' name='megacodmlongtime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerLong", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerLong", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr>
									                  <td>����T��ʱ��ʱ��(��):</td>
									                  <td>
									                  	<input type='text' name='megacodmttime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerT", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerT", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>æ��ʱ��(��):</td>
									                  <td>
									                  	<input type='text' name='busytonetimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "BusyToneTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "BusyToneTimer", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>������ʱ��(��):</td>
									                  <td>
									                  	<input type='text' name='hangingremindertimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "HangingReminderToneTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "HangingReminderToneTimer", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>�ýв�Ӧʱ��(��):</td>
									                  <td>
									                  	<input type='text' name='noanswertimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "NoAnswerTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "NoAnswerTimer", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>�����Tģʽ:</td>
									                  <td>
									                  	<select name="dmtmode" size="1" >
													   	            <option value="0" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h")	 = "0" then asp_Write("selected") end if %>>���Ŷ̶�ʱ��</option>
													   	            <option value="1" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h") = "1" then asp_Write("selected") end if %>>���ų���ʱ��</option>
													   	            <option value="2" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h") = "2" then asp_Write("selected") end if %>>����T��ʱ��</option>
													   	        </select>
													   	      </td>
									               </tr>
									               <tr >
									                  <td>Dtmf ���䷽ʽ:</td>
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
									                  <td>����ת������:</td>
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
									                  <td>SIP ����Э��:</td>
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
									                  <td>������Ϣ����:</td>
									                  <td>
									                		<input type='checkbox' name='natKeepAliveMessage' <%if tcWebApi_get("VoIPBasic_Common", "HeartbeatSwitch", "h") = "1" then asp_Write("checked") end if %>>
									                  </td>
									               </tr>
									               <tr >
									                  <td>������Ϣʱ��(��):</td>
									                  <td>
									                  	<input type='text' name='natKeepAliveTime' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "HeartbeatCycle", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "HeartbeatCycle", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <% if tcWebApi_get("WebCustom_Entry", "isCTPONC9Supported", "h") = "Yes"  then %>
									               <tr >
									                  <td>����From Toͷ��URI������:</td>
									                  <td>
									                  	<select name="fromtoheadurl" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "FromToHeadURI", "h") = "0" then asp_Write("selected") end if %>>ע�������</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "FromToHeadURI", "h") = "1" then asp_Write("selected") end if %>>���������</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "FromToHeadURI", "h") = "2" then asp_Write("selected") end if %>>���ַ�����</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <% end if %>
                                                                                       <tr >
									                  <td>����OPTIONSģʽ</td>
									                  <td>
									                  	<select name="optionssericemode" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "OptionsSericeMode", "h") = "0" then asp_Write("selected") end if %>>ע�������ģʽ</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "OptionsSericeMode", "h") = "1" then asp_Write("selected") end if %>>���������ģʽ</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "OptionsSericeMode", "h") = "2" then asp_Write("selected") end if %>>Outboundģʽ</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>����Э��ģʽ:</td>
									                  <td>
									                  	<select name="faxctrlmode" size="1">
											   	                <option value="other" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "other" then asp_Write("selected") end if %>>��Э��</option>
											   	                <option value="all" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "all" then asp_Write("selected") end if %>>T30ȫ��</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <tr >
									                  <td>����͸�������:</td>
									                  <td>
									                  	<select name="faxpassthru" size="1">
											   	                <option value="PCMU" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxPassThruCodec", "h") = "PCMU" then asp_Write("selected") end if %>>PCMU</option>
											   	                <option value="PCMA" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxPassThruCodec", "h") = "PCMA" then asp_Write("selected") end if %>>PCMA</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <tr >
									                  <td>RTP����:</td>
									                  <td>
									                      <input name="siprtpredundancy" type="checkbox" <%if tcWebApi_get("VoIPAdvanced_Common", "rtpRedundantEnable", "h") = "1" then asp_Write("checked") end if %>>
									                  </td>
									                  </tr>
									               <tr>
									                      <td>RTP����paylod type:</td>
									                      <td>
									                      <input name="siprtpredundancypt" type="text" id="sip_rtp_redundancy_pt_val_1" onblur="limitNum(this)"  value="<%if tcWebApi_get("VoIPAdvanced_Common", "rtpRedundantPT", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "rtpRedundantPT", "s") else asp_Write("") end if %>">
									                           &nbsp;(96-127)
									                      </td>
									               </tr> 
									               <tr >
									                  <td>����ʵ�ַ�ʽ:</td>
									                  <td>
									                  	<select name="DivHotLineType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "HotLineMode", "h") = "0" then asp_Write("selected") end if %>>�ϱ��û�����</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "HotLineMode", "h") = "1" then asp_Write("selected") end if %>>�ϱ�NoDial</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "HotLineMode", "h") = "2" then asp_Write("selected") end if %>>�ϱ�������</option>
											   	            </select>
											   	          </td>
									               </tr>
									               <tr >
									                  <td>ע�������Ƿ���˿ںţ�</td>
									                  <td>
									                  	<select name="DivIsRegisterWithPort" size="1">
											   	                <option value="Yes" <%if tcWebApi_get("VoIPAdvanced_Common", "RegResponsePortEnable", "h") = "Yes" then asp_Write("selected") end if %>>�����˿ں�</option>
											   	                <option value="No" <%if tcWebApi_get("VoIPAdvanced_Common", "RegResponsePortEnable", "h") = "No" then asp_Write("selected") end if %>>���˿ں�</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>�����Ƿ��"user=phone"��</td>
									                  <td>
									                  	<select name="DivIsMsgWithUser" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "MsgWithUserEnable", "h") = "0" then asp_Write("selected") end if %>>����user=phone</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "MsgWithUserEnable", "h") = "1" then asp_Write("selected") end if %>>��user=phone</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>���߼�����:</td>
									                  <td>
									                  	<input type='text' name='DivHotLineActiveCode' value="<%if tcWebApi_get("VoIPAdvanced_Common", "HotLineActNum", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "HotLineActNum", "s") else asp_Write("") end if %>">
									                  </td>
									               </tr>
									               <tr >
									                  <td>���еȴ����ͣ�</td>
									                  <td>
									                  	<select name="DivCWType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "CallWaitType", "h") = "0" then asp_Write("selected") end if %>>ֱ���л�</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "CallWaitType", "h") = "1" then asp_Write("selected") end if %>>�Ӱ����ּ��л�</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>δע�Ღ������/��:</td>
									                  <td>
									                  	<select name="DivDialtone" size="1">
											   	                <option value="Yes" <%if tcWebApi_get("VoIPAdvanced_Common", "NoRegDialToneEnable", "h") = "Yes" then asp_Write("selected") end if %>>�رղ�����</option>
											   	                <option value="No" <%if tcWebApi_get("VoIPAdvanced_Common", "NoRegDialToneEnable", "h") = "No" then asp_Write("selected") end if %>>���ò�����</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>�����������ȼ����ƣ�</td>
									                  <td>
									                  	<select name="DivCodecSelectFlag" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "VoiceCodecPriorityCtrl", "h") = "0" then asp_Write("selected") end if %>>��������</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "VoiceCodecPriorityCtrl", "h") = "1" then asp_Write("selected") end if %>>��������</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>����ƥ�䷽ʽ��</td>
									                  <td>
									                  	<select name="DivDigitMatchMethod" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "NumberMatchMode", "h") = "0" then asp_Write("selected") end if %>>��Сƥ��</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "NumberMatchMode", "h") = "1" then asp_Write("selected") end if %>>ģ�����ƥ��</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "NumberMatchMode", "h") = "2" then asp_Write("selected") end if %>>���ƥ��</option>
											   	            </select>
									                  </td>
									               </tr>

									               <tr >
									                  <td>����ҵ�����ͣ�</td>
									                  <td>
									                  	<select name="DivSupplementServiceType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "0" then asp_Write("selected") end if %>>None</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "1" then asp_Write("selected") end if %>>Local</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "2" then asp_Write("selected") end if %>>INFO</option>
											   	                <option value="3" <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "3" then asp_Write("selected") end if %>>CTC_IMS</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									                  <td>��ֹ������ģʽ��</td>
									                  <td>
									                  	<select name="EndCharacterHandleMode" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "EndCharHandleMode", "h") = "0" then asp_Write("selected") end if %>>��ֹģʽ</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "EndCharHandleMode", "h") = "1" then asp_Write("selected") end if %>>����ģʽ</option>											   	                
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									               		<td>֧��#ת��%23:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='changeCharEnable' ID="Checkbox6" value='YES' <%if tcWebApi_get("VoIPAdvanced_Common", "SupportSpecCharTransfer", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>֧��@ת��%40:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='changeReferCharEnable' ID="Checkbox6" value='YES' <%if tcWebApi_get("VoIPAdvanced_Common", "SupReferSpecCharTrans", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>֧��T38:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='t38Enable' ID="Checkbox7" value='1'  <%if tcWebApi_get("VoIPMedia_Common", "FaxCodec", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>������������:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='comfortnoiseEnable' ID="Checkbox81" value='YES' <%if tcWebApi_get("VoIPMedia_Entry0", "VAD", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>����ѹ������:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='vadEnable' ID="Checkbox82" value='YES' <%if tcWebApi_get("VoIPMedia_Entry0", "SilenceCompressionEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>֧��Prack:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='PrackEnable' ID="Checkbox9" value='YES' <%if tcWebApi_get("VoIPBasic_Common", "PRACKEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>�Ƿ��ȷ���ע����Ϣ:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='sendUnregister' ID="Checkbox10" value='YES' <%if tcWebApi_get("VoIPBasic_Common", "LogOffMsgSendEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>�Զ�����ͬ������ʱ�俪��:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='autoSynPhoneDateSwitch' ID="Checkbox12" value='YES' <%if tcWebApi_get("VoIPAdvanced_Common", "CallSynEnAutoSwitch", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>�Ƿ�У׼�ն�ʱ��:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='syncSystemDate' ID="Checkbox13" value='YES' <%if tcWebApi_get("VoIPAdvanced_Common", "SyncSysTimeEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>����802.1p����:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='VoIP8021pEnable' onClick='Priority8021PClick(this)' ID="Checkbox11" value='0' <%if tcWebApi_get("VoIPBasic_Common", "VoIP8021PMarkEnable", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr id="8021psip">
									                  <td>802.1p(SIP)[0-7]:</td>
									                  <td>
									                  	<input type='text' name='VoIP8021PSIP' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "SIP8021PMark", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SIP8021PMark", "s") else asp_Write("") end if %>">
									                  	&nbsp;
									                  </td>
									               </tr>
									               <tr id="8021prtp">
									                  <td>802.1p(RTP)[0-7]:</td>
									                  <td>
									                  	<input type='text' name='VoIP8021PRTP' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "RTP8021PMark", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "RTP8021PMark", "s") else asp_Write("") end if %>">
									                  	&nbsp;
									                  </td>
									               </tr>
									               <tr >
									               		<td>���ûỰ��������:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='sessiontimerEnable' onClick='sessionClick(this)' ID="Checkbox11" value='0' <%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_FLAG", "h") = "1" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr id="sesstimer">
									                  <td>�Ự��������:</td>
									                  <td>
									                  	<input type='text' name='sessionTimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_TIMER", "s") else asp_Write("") end if %>">
									                  	&nbsp;��
									                  </td>
									               </tr>
									               <tr id="sessminse">
									                  <td>��С�Ự��������:</td>
									                  <td>
									                  	<input type='text' name='sessionMinSE' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_MIN_EXP", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SC_ACCT_SIP_SESSION_MIN_EXP", "s") else asp_Write("") end if %>">
									                  	&nbsp;��
									                  </td>
									               </tr>
									               <tr id="sessrefresher" >
									                  <td>�Ự���·�:</td>
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
									                  <td>�Ự������Ϣ����:</td>
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
  									                <td>TOS/DSCP���ȼ�(SIP):</td>
  									                <td>
  									                  <input type='text' size=20 maxlength=20 name='dscpmark' onblur='limitNum(this)' value="<%if tcWebApi_get("VoIPBasic_Common", "SIPDSCPMark", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SIPDSCPMark", "s") else asp_Write("") end if %>">
  									                </td>
									               </tr>
									               <tr>
										               <td>TOS/DSCP���ȼ�(RTP):</td>
										               <td><input name="mymgtos" type="text" id="in_h_tos_val" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "RTPDSCPMark", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "RTPDSCPMark", "s") else asp_Write("") end if %>"></td>
									               </tr>
									               <tr >
									                  <td>���Է�ת��ʽ��</td>
									                  <td>
									                  	<select name="DivRevPolarityType" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "0" then asp_Write("selected") end if %>>�ر�</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "1" then asp_Write("selected") end if %>>���ش�</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "2" then asp_Write("selected") end if %>>����������</option>
											   	            </select>
									                  </td>
									               </tr>
									               <tr >
									               		<td>������������:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='echoCancelEnable' ID="Checkbox8" value='YES' <%if tcWebApi_get("VoIPMedia_Common", "EchoCancellationEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr >
									               		<td>����Ӧjitter buffer����:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='AJBEnable' onClick='AJBClick(this)' ID="Checkbox15" value='1' <%if tcWebApi_get("VoIPAdvanced_Common", "SC_SYS_VOICE_JB_TYPE", "h") <> "2" then asp_Write("checked") end if%>>
													          </td>
									               </tr>
									               <tr id="JB_LEN">
									                  <td>jitter buffer���ֵ:</td>
									                  <td>
									                  	<input type='text' name='JBMaxLen' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "SC_SYS_VOICE_JB_TYPE", "h") <> "2" then if tcWebApi_get("VoIPAdvanced_Common", "SC_SYS_VOICE_JB_LEN", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "SC_SYS_VOICE_JB_LEN", "s") else asp_Write("200") end if else asp_Write("200") end if %>">
									                  </td>
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
					                        	 <input type='hidden' value='0' name="changeReferCharEnableFlag">
					                        	 <input type='hidden' value='0' name="t38EnableFlag">
					                        	 <input type='hidden' value='No' name="echoCancelEnableFlag">
					                        	 <input type='hidden' value='No' name="comfortnoiseEnableFlag">
					                        	 <input type='hidden' value='No' name="vadEnableFlag">
					                        	 <input type='hidden' value='No' name="PrackEnableFlag">
					                        	 <input type='hidden' value='No' name="sendUnregisterFlag">
					                        	 <input type='hidden' value='0' name="autoSynPhoneDateSwitchFlag">
					                        	 <input type='hidden' value='No' name="syncSystemDateFlag">
					                        	 <input type='hidden' value='2' name="AJBEnableFlag">
					                        	 <input type='hidden' value='0' name="sessiontimerEnableFlag">
					                        	 <input type='hidden' value='0' name="VoIP8021pEnableFlag">
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
					                        	 <input type='hidden' value='No' name="threepartyservermix0Flag">
					                        	 <input type='hidden' value='No' name="threepartyservermix1Flag">
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

					                        	 <input type='hidden' value='0' name="DigitMapSpecialEnableFlag">
					                        	 <input type='hidden' value='0' name="PBXPrefixEnableFlag">
					                        	 <input type='hidden' value='0' name="PBXDialSecondaryEnableFlag">
					                        	 <input type='hidden' value='0' name="siprtpredundancyflag">
					                        </td>
					                    </tr>
					                    <tr>
					                        <td align="center">
					                          <input type='button' onClick="btnApplySip()" value='Ӧ��' ID="ApplyButton">
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
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>&nbsp;</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
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
          <TD width=162 bgColor=#ef8218>��</TD>
          <TD width=278 bgColor=#427594>��</TD>
          <TD width=196 bgColor=#427594><!-- InstanceEndEditable --></TD>
          <TD width=170 
bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
