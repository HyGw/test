<%
If request_Form("VoIP_SIP_flag")="1" then
	tcWebApi_set("VoIPAdvanced_Common", "RegistrationExpire", "regExpTmr")
	tcWebApi_set("VoIPAdvanced_Common", "RegisterRetryInterval", "regRetryInt")
	tcWebApi_set("VoIPAdvanced_Common", "DialPlan", "dialPlan")
	tcWebApi_set("VoIPDigitMap_Entry", "StartDigitTimer", "megacodmstarttime")
	tcWebApi_set("VoIPDigitMap_Entry", "InterDigitTimerShort", "megacodmshorttime")
	tcWebApi_set("VoIPDigitMap_Entry", "InterDigitTimerLong", "megacodmlongtime")
	tcWebApi_set("VoIPDigitMap_Entry", "BusyToneTimer", "busytonetimer")
	tcWebApi_set("VoIPDigitMap_Entry", "HangingReminderToneTimer", "hangingremindertimer")
	tcWebApi_set("VoIPDigitMap_Entry", "TMode", "dmtmode")
	tcWebApi_set("VoIPAdvanced_Common", "DTMFTransportMode", "dtmfRelay")
	if request_Form("dtmfRelay") = "RFC2833" then
	tcWebApi_set("VoIPBasic_Common", "TelephoneEventPayloadType", "rfc2833pt")
	end if
	tcWebApi_set("VoIPAdvanced_Common", "FlashOffTransSetting", "hookFlashRelay")
	tcWebApi_set("VoIPBasic_Common", "HeartbeatSwitch", "natKeepAliveMessage")
	tcWebApi_set("VoIPBasic_Common", "HeartbeatCycle", "natKeepAliveTime")
	tcWebApi_set("VoIPAdvanced_Common", "FaxCtrlMode", "faxctrlmode")
	tcWebApi_set("VoIPAdvanced_Common", "HotLineMode", "DivHotLineType")
	tcWebApi_set("VoIPAdvanced_Common", "RegResponsePortEnable", "DivIsRegisterWithPort")
	tcWebApi_set("VoIPAdvanced_Common", "HotLineActNum", "DivHotLineActiveCode")
	tcWebApi_set("VoIPAdvanced_Common", "CallWaitType", "DivCWType")
	tcWebApi_set("VoIPAdvanced_Common", "NoRegDialToneEnable", "DivDialtone")
	tcWebApi_set("VoIPAdvanced_Common", "PolarityReversalMode", "DivRevPolarityType")
	tcWebApi_set("VoIPAdvanced_Common", "VoiceCodecPriorityCtrl", "DivCodecSelectFlag")
	tcWebApi_set("VoIPAdvanced_Common", "NumberMatchMode", "DivDigitMatchMethod")
	tcWebApi_set("VoIPAdvanced_Common", "SubscribeType", "DivSubscribeType")
	tcWebApi_set("VoIPBasic_Common", "HeartbeatSwitch", "natKeepAliveMessageFlag")
	tcWebApi_set("VoIPAdvanced_Common", "ActLocalSupplementService", "localAppEnableFlag")
	tcWebApi_set("VoIPAdvanced_Common", "SupportSpecCharTransfer", "changeCharEnableFlag")
	tcWebApi_set("VoIPMedia_Common", "FaxCodec", "t38EnableFlag")
	tcWebApi_set("VoIPAdvanced_Entry0", "EchoCancellationEnable", "echoCancelEnableFlag")
	tcWebApi_set("VoIPAdvanced_Entry1", "EchoCancellationEnable", "echoCancelEnableFlag")
	tcWebApi_set("VoIPMedia_Entry0", "VAD", "comfortnoiseEnableFlag")
	tcWebApi_set("VoIPMedia_Entry1", "VAD", "comfortnoiseEnableFlag")
	tcWebApi_set("VoIPMedia_Entry0", "SilenceCompressionEnable", "vadEnableFlag")
	tcWebApi_set("VoIPMedia_Entry1", "SilenceCompressionEnable", "vadEnableFlag")
	tcWebApi_set("VoIPBasic_Common", "PRACKEnable", "PrackEnableFlag")
	tcWebApi_set("VoIPBasic_Common", "LogOffMsgSendEnable", "sendUnregisterFlag")
	tcWebApi_set("VoIPBasic_Common", "SessionUpdateTimerEnable", "sessiontimerEnableFlag")
if request_Form("sessiontimerEnableFlag") = "Yes" then
	tcWebApi_set("VoIPBasic_Common", "SessionUpdateTimer", "sessionTimer")
	tcWebApi_set("VoIPBasic_Common", "SessionUpdateMsgType", "sessionType")
end if
	tcWebApi_set("VoIPBasic_Common", "SIPDSCPMark", "dscpmark")
	tcWebApi_set("VoIPBasic_Common", "RTPDSCPMark", "mymgtos")
	tcWebApi_set("VoIPAdvanced_Entry0", "G722Priority", "codecList0")
	tcWebApi_set("VoIPAdvanced_Entry1", "G722Priority", "codecList1")
	tcWebApi_set("VoIPAdvanced_Entry0", "G711AlawPriority", "codecList2")
	tcWebApi_set("VoIPAdvanced_Entry1", "G711AlawPriority", "codecList3")
	tcWebApi_set("VoIPAdvanced_Entry0", "G711UlawPriority", "codecList4")
	tcWebApi_set("VoIPAdvanced_Entry1", "G711UlawPriority", "codecList5")
	tcWebApi_set("VoIPAdvanced_Entry0", "G729Priority", "codecList6")
	tcWebApi_set("VoIPAdvanced_Entry1", "G729Priority", "codecList7")
	tcWebApi_set("VoIPAdvanced_Entry0", "SIPPacketizationTime", "voicePtime0")
	tcWebApi_set("VoIPAdvanced_Entry1", "SIPPacketizationTime", "voicePtime1")
	tcWebApi_set("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "ingressGain0")
	tcWebApi_set("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "ingressGain1")
	tcWebApi_set("VoIPAdvanced_Entry0", "VoiceVolumeListen", "egressGain0")
	tcWebApi_set("VoIPAdvanced_Entry1", "VoiceVolumeListen", "egressGain1")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HotLineEnable", "warmline0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HotLineEnable", "warmline1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HotLineNumber", "warmlinenum0")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HotLineNumber", "warmlinenum1")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HotLineDelayTime", "warmlinetimeout0")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HotLineDelayTime", "warmlinetimeout1")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIPCallWaitingEnable", "callwait0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIPCallWaitingEnable", "callwait1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIPCallTransfer", "transfer0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIPCallTransfer", "transfer1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "ForwardNumber", "fwdNum0")
	tcWebApi_set("VoIPCallCtrl_Entry1", "ForwardNumber", "fwdNum1")	
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKUCFEnable", "fwdAll0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKUCFEnable", "fwdAll1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKBCFEnable", "fwdBusy0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKBCFEnable", "fwdBusy1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "MTKNCFEnable", "fwdNoAns0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "MTKNCFEnable", "fwdNoAns1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "fwdNoAnsTime0")
	tcWebApi_set("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "fwdNoAnsTime1")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SIP3wayConf", "threepartyservice0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SIP3wayConf", "threepartyservice1Flag")
	tcWebApi_set("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "calleridmode0")
	tcWebApi_set("VoIPCallCtrl_Entry1", "ComingCallDisplayMode", "calleridmode1")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HookMinInterval", "minhookinterval0")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookMinInterval", "minhookinterval1")
	tcWebApi_set("VoIPCallCtrl_Entry0", "HookMaxInterval", "maxhookinterval0")
	tcWebApi_set("VoIPCallCtrl_Entry1", "HookMaxInterval", "maxhookinterval1")
	tcWebApi_set("VoIPCallCtrl_Entry0", "SyncCallerTimeEnable", "synphonedate0Flag")
	tcWebApi_set("VoIPCallCtrl_Entry1", "SyncCallerTimeEnable", "synphonedate1Flag")	
	tcWebApi_commit("VoIPBasic")
	tcWebApi_commit("VoIPDigitMap")
	tcWebApi_commit("VoIPAdvanced")
	tcWebApi_commit("VoIPMedia")
	tcWebApi_commit("VoIPCallCtrl")
	tcWebApi_save()
end if
%>
<HTML><HEAD><TITLE>VOIP设置</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="javascript">

var session	= "0";

function LoadFrame()
{
   with ( document.ConfigForm ) 
   {
      generatePerLineTable();
      CheckPriority();
      if ("Yes" == "<%tcWebApi_get("VoIPBasic_Common", "SessionUpdateTimerEnable", "s") %>")
      {
	   	  document.getElementById("sesstimer").style.display = "";
	   	  document.getElementById("sesstype").style.display = "";      	
      }
	  	else
	   	{
	   	  document.getElementById("sesstimer").style.display = "none";
	   	  document.getElementById("sesstype").style.display = "none";
	   	}  
	  if ( "RFC2833" == "<%tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "s") %>"){
      	document.getElementById("trRFC2833PT").style.display = "";
      }
      else{
      	document.getElementById("trRFC2833PT").style.display = "none";
      } 
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
      tableHtmlOutput += "<td align=center>2</td>";
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

		  var selectOptions1 = Array();
		  selectOptions1[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions1[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions1[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions1[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions1[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions1[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions1[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions1[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  width=180><select name='codecList1'>" + selectOptions1.join("") + "<\/select></td></tr>"; 

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

		  var selectOptions3 = Array();
		  selectOptions3[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions3[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions3[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions3[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions3[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions3[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions3[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions3[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList3'>" + selectOptions3.join("") + "<\/select></td></tr>";  

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

		  var selectOptions5 = Array();
		  selectOptions5[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions5[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions5[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions5[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions5[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions5[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions5[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions5[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList5'>" + selectOptions5.join("") + "<\/select></td></tr>";  
  
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

		  var selectOptions7 = Array();
		  selectOptions7[0] = "<option value=" + 0 + " >" + "0";
		  selectOptions7[1] = "<option value=" + 1 + " >" + "1";
		  selectOptions7[2] = "<option value=" + 2 + " >" + "2";
		  selectOptions7[3] = "<option value=" + 3 + " >" + "3";
		  selectOptions7[4] = "<option value=" + 4 + " >" + "4";
		  selectOptions7[5] = "<option value=" + 5 + " >" + "5";
		  selectOptions7[6] = "<option value=" + 6 + " >" + "6";
		  selectOptions7[7] = "<option value=" + 7 + " >" + "7";
		  tableHtmlOutput += "<td align=center  ><select name='codecList7'>" + selectOptions7.join("") + "<\/select></td></tr>";  
	  
      // Ptime row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >ptime 设置 <\/td>"
      var selectOptions0 = Array();
      selectOptions0[0] = "<option value=10 <%if tcWebApi_get("VoIPAdvanced_Entry0", "SIPPacketizationTime", "h") = "10" then  asp_Write("selected") end if %>>" + 10;
      selectOptions0[1] = "<option value=20 <%if tcWebApi_get("VoIPAdvanced_Entry0", "SIPPacketizationTime", "h") = "20" then  asp_Write("selected") end if %>>" + 20;
      selectOptions0[2] = "<option value=30 <%if tcWebApi_get("VoIPAdvanced_Entry0", "SIPPacketizationTime", "h") = "30" then  asp_Write("selected") end if %>>" + 30;
      tableHtmlOutput += "<td align=center><select name='voicePtime0'>" + selectOptions0.join("") + "<\/select></td>";
      
      var selectOptions1 = Array();
      selectOptions1[0] = "<option value=10 <%if tcWebApi_get("VoIPAdvanced_Entry1", "SIPPacketizationTime", "h") = "10" then asp_Write("selected") end if %>>" + 10;
      selectOptions1[1] = "<option value=20 <%if tcWebApi_get("VoIPAdvanced_Entry1", "SIPPacketizationTime", "h") = "20" then asp_Write("selected") end if %>>" + 20;
      selectOptions1[2] = "<option value=30 <%if tcWebApi_get("VoIPAdvanced_Entry1", "SIPPacketizationTime", "h") = "30" then asp_Write("selected") end if %>>" + 30;
      tableHtmlOutput += "<td align=center><select name='voicePtime1'>" + selectOptions1.join("") + "<\/select></td>";
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
      tableHtmlOutput += "<td align=center>2</td>";
      tableHtmlOutput += "</tr>";

      // Volume row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >发送音量<\/td>"
     	tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'ingressGain0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>\'></td>";    
     	tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'ingressGain1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>\'></td>";    
      tableHtmlOutput += "</tr>";
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >接收音量<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'egressGain0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'egressGain1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPAdvanced_Entry1", "VoiceVolumeListen", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "VoiceVolumeListen", "s") else asp_Write("") end if %>\'></td>";
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
      tableHtmlOutput += "<td align=center>2</td>";
      tableHtmlOutput += "</tr>";
      
      // Warm Line row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用热线<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'warmline0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'warmline1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";

      // Warm Line Number row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >热线号码<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinenum0\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HotLineNumber", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinenum1\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HotLineNumber", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";

      // Warm Line Timeout row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >热线延迟时间(秒)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinetimeout0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HotLineDelayTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HotLineDelayTime", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'warmlinetimeout1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HotLineDelayTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HotLineDelayTime", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";

      // Call waiting row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用呼叫等待<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'callwait0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIPCallWaitingEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'callwait1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIPCallWaitingEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";

      /*zhangcg: 20110612*/
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用呼叫转移<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'transfer0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIPCallTransfer", "h") = "1" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'transfer1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIPCallTransfer", "h") = "1" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";

      // Call forwarding number row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >呼叫前转号码<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNum0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "ForwardNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "ForwardNumber", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNum1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "ForwardNumber", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "ForwardNumber", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";

      // Call forwarding unconditionally row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >无条件呼叫前转<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdAll0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKUCFEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdAll1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKUCFEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";

      // Call forwarding on "busy" row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >遇忙呼叫前转<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdBusy0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKBCFEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdBusy1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKBCFEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";

      // Call forwarding on "no answer" row
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >无应答呼叫前转<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdNoAns0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "MTKNCFEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'fwdNoAns1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "MTKNCFEnable", "h") = "Yes" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";
	  
	  // Call forwarding on "no answer time"
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >无应答呼叫前转等待时间<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNoAnsTime0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "NoAnswerNCFWaitTime", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'fwdNoAnsTime1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "NoAnswerNCFWaitTime", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";

      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用三方会议<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservice0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SIP3wayConf", "h") = "1" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'threepartyservice1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SIP3wayConf", "h") = "1" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "</tr>";

    //caller id mode
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >来电显示模式 <\/td>"
      var selectOptions0 = Array();
      selectOptions0[0] = "<option value=" + 0 + " <%if tcWebApi_get("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "h") = "0" then asp_Write("selected") end if %>>" + "DTMF";
      selectOptions0[1] = "<option value=" + 1 + " <%if tcWebApi_get("VoIPCallCtrl_Entry0", "ComingCallDisplayMode", "h") = "1" then asp_Write("selected") end if %>>" + "FSK";   
      tableHtmlOutput += "<td align=center><select name='calleridmode0'>" + selectOptions0.join("") + "<\/select></td>";
                  
      var selectOptions1 = Array();
      selectOptions1[0] = "<option value=" + 0 + " <%if tcWebApi_get("VoIPCallCtrl_Entry1", "ComingCallDisplayMode", "h") = "0" then asp_Write("selected") end if %>>" + "DTMF";
      selectOptions1[1] = "<option value=" + 1 + " <%if tcWebApi_get("VoIPCallCtrl_Entry1", "ComingCallDisplayMode", "h") = "1" then asp_Write("selected") end if %>>" + "FSK";      
      tableHtmlOutput += "<td align=center><select name='calleridmode1'>" + selectOptions1.join("") + "<\/select></td>";
      tableHtmlOutput += "</tr>";

	//flash hook interval 
      tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >拍叉簧最小时间间隔(毫秒)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookinterval0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookMinInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookMinInterval", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'minhookinterval1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookMinInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookMinInterval", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";

	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >拍叉簧最大时间间隔(毫秒)<\/td>"
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'maxhookinterval0\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry0", "HookMaxInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry0", "HookMaxInterval", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "<td align=center><input type='text' size=20 maxlength=20 name=\'maxhookinterval1\' onblur=\'limitNum(this)\' value=\'<%if tcWebApi_get("VoIPCallCtrl_Entry1", "HookMaxInterval", "h") <> "N/A" then tcWebAPI_get("VoIPCallCtrl_Entry1", "HookMaxInterval", "s") else asp_Write("") end if %>\'></td>";
      tableHtmlOutput += "</tr>";

	//Synchronize phone date time
	    tableHtmlOutput += "<tr>";
      tableHtmlOutput += "<td >启用同步话机时间<\/td>"
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'synphonedate0\' <%if tcWebApi_get("VoIPCallCtrl_Entry0", "SyncCallerTimeEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
      tableHtmlOutput += "<td align=center><input type='checkbox' size=10 name=\'synphonedate1\' <%if tcWebApi_get("VoIPCallCtrl_Entry1", "SyncCallerTimeEnable", "h") = "1" then asp_Write("checked") end if %>></td>";
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
	
function CheckPriority()
{
	  getElementByName("codecList0").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry0", "G722Priority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "G722Priority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList1").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry1", "G722Priority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "G722Priority", "s") else asp_Write("0") end if %>"; 
	  getElementByName("codecList2").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry0", "G711AlawPriority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "G711AlawPriority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList3").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry1", "G711AlawPriority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "G711AlawPriority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList4").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry0", "G711UlawPriority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "G711UlawPriority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList5").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry1", "G711UlawPriority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "G711UlawPriority", "s") else asp_Write("0") end if %>"; 	
	  getElementByName("codecList6").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry0", "G729Priority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "G729Priority", "s") else asp_Write("0") end if %>";
	  getElementByName("codecList7").selectedIndex = 	"<%if tcWebApi_get("VoIPAdvanced_Entry1", "G729Priority", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry1", "G729Priority", "s") else asp_Write("0") end if %>";
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
   	  document.getElementById("sesstype").style.display = "";
   	}
   else
   	{
   	  document.getElementById("sesstimer").style.display = "none";
   	  document.getElementById("sesstype").style.display = "none";
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
    
    for (i = 0; i < 2; i++) 
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
		if (temp < 0 || temp > 20)
		{
			alert('发送音量的范围是0~20db.');
			return;
		}
		
	  var temp = parseInt(ingressGain1.value);	
		if (temp < 0 || temp > 20)
		{
			alert('发送音量的范围是0~20db.');
			return;
		}		
		
		var temp = parseInt(egressGain0.value);	
		if (temp < 0 || temp > 20)
		{
			alert('接收音量的范围是0~20db.');
			return;
		} 
		
		var temp = parseInt(egressGain1.value);	
		if (temp < 0 || temp > 20)
		{
			alert('接收音量的范围是0~20db.');
			return;
		} 		

    	var temp = parseInt(rfc2833pt.value);	
		if (temp < 96 || temp > 127)
		{
			alert('Telephone Event Payload Type的范围是96~127.');
			return;
		}
		
 		var temp = parseInt(mymgtos.value);	
		if (temp < 0 || temp > 63)
		{
			alert('TOS/DSCP 信令优先级的范围是0~63.');
			return;
		}
		
	  if (natKeepAliveMessage.checked)
		{
			natKeepAliveMessageFlag.value = "1";
		}  		
		if (localAppEnable.checked)
		{
			localAppEnableFlag.value = "1";
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
			sessiontimerEnableFlag.value = "Yes";
		}
		if (warmline0.checked)
		{
			warmline0Flag.value = "Yes";
		}
		if (warmline1.checked)
		{
			warmline1Flag.value = "Yes";
		}
		if (callwait0.checked)
		{
			callwait0Flag.value = "Yes";
		}
		if (callwait1.checked)
		{
			callwait1Flag.value = "Yes";
		}
		if (transfer0.checked)
		{
			transfer0Flag.value = "1";
		}
		if (transfer1.checked)
		{
			transfer1Flag.value = "1";
		}
		if (fwdAll0.checked)
		{
			fwdAll0Flag.value = "Yes";
		}
		if (fwdAll1.checked)
		{
			fwdAll1Flag.value = "Yes";
		}
		if (fwdBusy0.checked)
		{
			fwdBusy0Flag.value = "Yes";
		}
		if (fwdBusy1.checked)
		{
			fwdBusy1Flag.value = "Yes";
		}
		if (fwdNoAns0.checked)
		{
			fwdNoAns0Flag.value = "Yes";
		}
		if (fwdNoAns1.checked)
		{
			fwdNoAns1Flag.value = "Yes";
		}
		if (threepartyservice0.checked)
		{
			threepartyservice0Flag.value = "1";
		}
		if (threepartyservice1.checked)
		{
			threepartyservice1Flag.value = "1";			
		}
		if (synphonedate0.checked)
		{
			synphonedate0Flag.value = "1";
		}
		if (synphonedate1.checked)
		{
			synphonedate1Flag.value = "1";	
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
                <TD><A 
                  href="/cgi-bin/help_content.asp#宽带电话设置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0> </A></TD></TR></TBODY></TABLE></TD></TR>
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
								                    <TD align=left>
								                        注册失败时再次尝试注册时间间隔:
								                    </TD>
								                    <TD align=left>
								                    	<input type='text' name='regRetryInt' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "RegisterRetryInterval", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "RegisterRetryInterval", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD align=left>
								                        拨号规则:
								                    </TD>
								                    <TD align=left>
								                    	<input type='text' name='dialPlan' value="<%if tcWebApi_get("VoIPAdvanced_Common", "DialPlan", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "DialPlan", "s") else asp_Write("") end if %>">
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
													   	            <option value="DigitMapShortTimer" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h")	 = "DigitMapShortTimer" then asp_Write("selected") end if %>>拨号短定时器</option>
													   	            <option value="DigitMapLongTimer" <%if tcWebApi_get("VoIPDigitMap_Entry", "TMode", "h") = "DigitMapLongTimer" then asp_Write("selected") end if %>>拨号长定时器</option>
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
									               <tr >
									                  <td>传真协商模式:</td>
									                  <td>
									                  	<select name="faxctrlmode" size="1">
											   	                <option value="other" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "other" then asp_Write("selected") end if %>>自动</option>
											   	                <option value="all" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "all" then asp_Write("selected") end if %>>协商</option>
											   	            </select>
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
											   	                <option value="0" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "0" then asp_Write("selected") end if %>>服务器控制</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "1" then asp_Write("selected") end if %>>本地打开</option>
											   	                <option value="2" <%if tcWebApi_get("VoIPAdvanced_Common", "PolarityReversalMode", "h") = "2" then asp_Write("selected") end if %>>本地关闭</option>
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
									               		<td>激活本地补充业务:</td>
									                  <td valign="middle">
													            <input type='checkbox' name='localAppEnable' ID="Checkbox5" value='YES' <%if tcWebApi_get("VoIPAdvanced_Common", "ActLocalSupplementService", "h") = "1" then asp_Write("checked") end if %>>
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
													            <input type='checkbox' name='echoCancelEnable' ID="Checkbox8" value='YES' <%if tcWebApi_get("VoIPAdvanced_Entry0", "EchoCancellationEnable", "h") = "Yes" then asp_Write("checked") end if %>>
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
													            <input type='checkbox' name='sessiontimerEnable' onClick='sessionClick(this)' ID="Checkbox8" value='NO' <%if tcWebApi_get("VoIPBasic_Common", "SessionUpdateTimerEnable", "h") = "Yes" then asp_Write("checked") end if %>>
													          </td>
									               </tr>
									               <tr id="sesstimer">
									                  <td>设置会话更新周期:</td>
									                  <td>
									                  	<input type='text' name='sessionTimer' onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPBasic_Common", "SessionUpdateTimer", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SessionUpdateTimer", "s") else asp_Write("") end if %>">
									                  	&nbsp;秒
									                  </td>
									               </tr>
									               <tr id="sesstype" >
									                  <td>会话更新消息类型:</td>
									                  <td>
									                  	<select name="sessionType" size="1">
													               <option value="1" <%if tcWebApi_get("VoIPBasic_Common", "SessionUpdateMsgType", "h") = "1" then asp_Write("selected") end if %>>
																         UPDATE
																         <option value="0" <%if tcWebApi_get("VoIPBasic_Common", "SessionUpdateMsgType", "h") = "0" then asp_Write("selected") end if %>>
																         INVITE
														          </select>
									                  </td>
									               </tr>
									                <tr>
  									                <td>DSCPMark:</td>
  									                <td>
  									                  <input type='text' size=20 maxlength=20 name='dscpmark' onblur='limitNum(this)' value="<%if tcWebApi_get("VoIPBasic_Common", "SIPDSCPMark", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SIPDSCPMark", "s") else asp_Write("") end if %>">
  									                </td>
									               </tr>
									               <tr>
										               <td>TOS/DSCP优先级(RTP)</td>
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
					                        	 <input type='hidden' value='0' name="localAppEnableFlag">
					                        	 <input type='hidden' value='0' name="changeCharEnableFlag">
					                        	 <input type='hidden' value='0' name="t38EnableFlag">
					                        	 <input type='hidden' value='No' name="echoCancelEnableFlag">
					                        	 <input type='hidden' value='No' name="comfortnoiseEnableFlag">
					                        	 <input type='hidden' value='No' name="vadEnableFlag">
					                        	 <input type='hidden' value='No' name="PrackEnableFlag">
					                        	 <input type='hidden' value='No' name="sendUnregisterFlag">
					                        	 <input type='hidden' value='No' name="sessiontimerEnableFlag">
					                        	 <input type='hidden' value='No' name="warmline0Flag">
					                        	 <input type='hidden' value='No' name="warmline1Flag">
					                        	 <input type='hidden' value='No' name="callwait0Flag">
					                        	 <input type='hidden' value='No' name="callwait1Flag">
					                        	 <input type='hidden' value='0' name="transfer0Flag">
					                        	 <input type='hidden' value='0' name="transfer1Flag">
					                        	 <input type='hidden' value='No' name="fwdAll0Flag">
					                        	 <input type='hidden' value='No' name="fwdAll1Flag">
					                        	 <input type='hidden' value='No' name="fwdBusy0Flag">
					                        	 <input type='hidden' value='No' name="fwdBusy1Flag">
					                        	 <input type='hidden' value='No' name="fwdNoAns0Flag">
					                        	 <input type='hidden' value='No' name="fwdNoAns1Flag">
					                        	 <input type='hidden' value='0' name="threepartyservice0Flag">
					                        	 <input type='hidden' value='0' name="threepartyservice1Flag">
					                        	 <input type='hidden' value='0' name="synphonedate0Flag">
					                        	 <input type='hidden' value='0' name="synphonedate1Flag">
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
