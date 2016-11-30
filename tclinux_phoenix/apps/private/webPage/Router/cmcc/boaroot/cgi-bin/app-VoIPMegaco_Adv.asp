<%
If request_Form("VoIP_H248_flag")="1" then
	tcWebApi_set("VoIPCodecs_PVC0_Entry0", "priority", "codec0")
	tcWebApi_set("VoIPCodecs_PVC1_Entry0", "priority", "codec0")
	tcWebApi_set("VoIPCodecs_PVC0_Entry1", "priority", "codec1")
	tcWebApi_set("VoIPCodecs_PVC1_Entry1", "priority", "codec1")
	tcWebApi_set("VoIPCodecs_PVC0_Entry2", "priority", "codec2")
	tcWebApi_set("VoIPCodecs_PVC1_Entry2", "priority", "codec2")
	tcWebApi_set("VoIPCodecs_PVC0_Entry3", "priority", "codec3")
	tcWebApi_set("VoIPCodecs_PVC1_Entry3", "priority", "codec3")
	tcWebApi_set("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "mymgtxgain")
	tcWebApi_set("VoIPAdvanced_Entry0", "VoiceVolumeListen", "mymgrxtain")
	tcWebApi_set("VoIPMedia_Entry0", "VAD", "mymgsliencenble")
	tcWebApi_set("VoIPAdvanced_Entry0", "EchoCancellationEnable", "mymgechocancel")
	tcWebApi_set("VoIPAdvanced_Entry0", "RFC2833Redundance", "mymg2833enable")
	If tcWebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then
	tcWebApi_set("VoIPAdvanced_Entry1", "VoiceVolumeSpeak", "mymgtxgain")
	tcWebApi_set("VoIPAdvanced_Entry1", "VoiceVolumeListen", "mymgrxtain")
	tcWebApi_set("VoIPMedia_Entry1", "VAD", "mymgsliencenble")
	tcWebApi_set("VoIPAdvanced_Entry1", "EchoCancellationEnable", "mymgechocancel")
	tcWebApi_set("VoIPAdvanced_Entry1", "RFC2833Redundance", "mymg2833enable")
	End If
	tcWebApi_set("VoIPMedia_Common", "FaxCodec", "megacofaxmode")
	tcWebApi_set("VoIPAdvanced_Common", "FaxCtrlMode", "megacofaxcontrol")
	tcWebApi_set("VoIPAdvanced_Common", "FaxPassThruCodec", "megacofaxpassthru")
	tcWebApi_set("VoIPAdvanced_Common", "FaxRedundancy", "megacofaxredundancyflag")
	tcWebApi_set("VoIPAdvanced_Common", "FaxRedundancyPT", "megacofaxredundancypt")
	tcWebApi_set("VoIPAdvanced_Common", "DTMFTransportMode", "megacodtmfrelay")
	if request_Form("megacodtmfrelay") = "RFC2833" then
	tcWebAPI_set("VoIPBasic_Common", "TelephoneEventPayloadType", "megacorfc2833pt")
	end if
	tcWebApi_set("VoIPAdvanced_Common", "MaxStartDelay", "megacomaxstartupdelay")
	tcWebApi_set("VoIPDigitMap_Entry", "StartDigitTimer", "megacodigitmapstarttime")
	tcWebApi_set("VoIPDigitMap_Entry", "InterDigitTimerShort", "megacodigitmapshorttime")
	tcWebApi_set("VoIPDigitMap_Entry", "InterDigitTimerLong", "megacodigitmapLongtime")
	tcWebApi_set("VoIPH248_Common", "NoAnswerTimer", "megaconoanswertimer")
	tcWebApi_set("VoIPH248_Common", "HeartbeatMode", "megacoheartbeatenable")
	tcWebApi_set("VoIPH248_Common", "HeartbeatSendMode", "megacohetbtSendMode")
	tcWebApi_set("VoIPH248_Common", "HeartBeatTimer", "megacoHeartBeatTimer")
	tcWebApi_set("VoIPH248_Common", "HeartBeatRetransTimes", "megacoHeartBeatRetransTimes")
	tcWebApi_set("VoIPH248_Common", "SmartSendReg", "cancellineregflag")
	tcWebApi_set("VoIPH248_Common", "NumberMatchMode", "DivDigitMatchMethod")
	tcWebApi_set("VoIPH248_Common", "NumberSmartMatch", "digitmapswitchflag")
	tcWebApi_set("VoIPH248_Common", "RingVoltage", "megacoringingvoltage")
	tcWebApi_set("VoIPAdvanced_Common", "FlashMaxTime", "megacoFlashMaxTime")
	tcWebApi_set("VoIPAdvanced_Common", "FlashMinTime", "megacoFlashMinTime")
	tcWebApi_set("VoIPAdvanced_Common", "ReleaseMinTime", "megacoReleaseMinTime")
	tcWebApi_commit("VoIPCodecs")
	tcWebApi_commit("VoIPAdvanced")
	tcWebApi_commit("VoIPMedia_Common")
	tcWebApi_commit("VoIPDigitMap")
	tcWebApi_commit("VoIPH248_Common")
	tcWebApi_commit("VoIPBasic_Common")
	tcWebApi_save()
end if
%>
<HTML><HEAD><TITLE>VOIP����</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language="javascript" type = "text/javascript">
function LoadFrame()
{
	if ( "RFC2833" == "<%tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "s") %>"){
		document.getElementById("trRFC2833PT").style.display = "";
	}
	else{
		document.getElementById("trRFC2833PT").style.display = "none";
	}
}
function applyClickBasic()
{
   var loc = "app-VoIP.asp";
   var code = "location='" + loc + " '";
   eval(code);
}
function VoiceApplyBasic()
{
	with(document.ConfigForm)
	{
		var temp = parseInt(mymgtxgain.value);	
		if (temp < 0 || temp > 20)
		{
			alert('���������ķ�Χ��0~20db.');
			return;
		}
		
		var temp = parseInt(mymgrxtain.value);	
		if (temp < 0 || temp > 20)
		{
			alert('���������ķ�Χ��0~20db.');
			return;
		}

		var temp = parseInt(megacofaxredundancypt.value);	
		if (temp < 96 || temp > 127)
		{
			alert('��������payload type��Χ��96~127.');
			return;
		}		
		
		var temp = parseInt(megacoReleaseMinTime.value);	
		if (temp < 300 || temp > 1199)
		{
			alert('�һ���С����ķ�Χ��300~1199ms.');
			return;
		}		
		var temp2 = parseInt(megacoFlashMaxTime.value);
		var temp3 = parseInt (megacoFlashMinTime.value);
		
		if (temp2 <= temp3)
		{
			alert('�����ʱ��Ӧ������С���ʱ��.');
			return;			
		}		
		if (temp < temp2)
		{
			alert('�һ���С���Ӧ���������ʱ��.');
			return;
		}
		
		var temp = parseInt(megacorfc2833pt.value);	
		if (temp < 96 || temp > 127)
		{
			alert('Telephone Event Payload Type�ķ�Χ��96~127.');
			return;
		} 
		var codecOld = "";
		var codecNew = "";
		var zz = 0;
    for (k = 0; k < 4; k++) 
    {
      codecOld = codecOld + "," + codecNew;
      codecNew = eval( "codec" + k + ".value" );
      codecList = codecOld.split(",");
      for (var i = 0;i < codecList.length;i++)
      {
	      if (codecNew != 0 && codecNew == codecList[i])
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

		if (megacofaxredundancy.checked)
		{
			megacofaxredundancyflag.value = "1";	
		}
		if (cancellinereg.checked)
		{
			cancellineregflag.value = "1";
		}
		if (digitmapswitch.checked)
		{
			digitmapswitchflag.value = "1";
		}		
		
		VoIP_H248_flag.value = "1";
		submit();	
	}
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
</SCRIPT>
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
                <TD class=welcom vAlign=bottom align=middle width=120>��ӭ���� </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>�˳� 
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
            <P align=right><FONT face=���� color=#ffffff><B><FONT face=���� 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="Ӧ��->VOIP����" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#d8d8d8 height=33>
            <P class=GatewayType align=center>�ͺ�:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#d8d8d8 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#0059A0 colSpan=2 
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
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>VOIP����</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
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
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
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
							                        ����绰�����߼�����
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
								        <TABLE border=1 cellSpacing=1 cellPadding=0 width="98%">
								            <TBODY>
								            		<TR>
								            			 <td class="table1_right" colspan="2" ><strong id='in_h_resource1'>ý������</strong></td>
								            		</TR>
								            		<TR>
								            			 <td class="table1_right" colspan="2" >������������ȼ�</td>
								            		</TR>
								                <TR>
								                    <td class="table1_left" width="35%"align="left">G.722</td>
				                            <td class="table1_right" width="65%">
					                            <select name="codec0" id="g722">
						                            <option value="0" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "0" then asp_Write("selected") end if %>>0</option>
						                            <option value="1" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "1" then asp_Write("selected") end if %>>1</option>
						                            <option value="2" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "2" then asp_Write("selected") end if %>>2</option>
						                            <option value="3" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "3" then asp_Write("selected") end if %>>3</option>
						                            <option value="4" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "4" then asp_Write("selected") end if %>>4</option>
						                            <option value="5" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "5" then asp_Write("selected") end if %>>5</option>
						                            <option value="6" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "6" then asp_Write("selected") end if %>>6</option>
						                            <option value="7" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry0", "priority", "h") = "7" then asp_Write("selected") end if %>>7</option>
					                            </select>
				                            </td>
								                </TR>
								                <TR>
								                    <td class="table1_left">G.711A-law</td>
				                            <td class="table1_right">
					                            <select name="codec1" id="g711a">
						                            <option value="0" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "0" then asp_Write("selected") end if %>>0</option>
						                            <option value="1" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "1" then asp_Write("selected") end if %>>1</option>
						                            <option value="2" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "2" then asp_Write("selected") end if %>>2</option>
						                            <option value="3" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "3" then asp_Write("selected") end if %>>3</option>
						                            <option value="4" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "4" then asp_Write("selected") end if %>>4</option>
						                            <option value="5" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "5" then asp_Write("selected") end if %>>5</option>
						                            <option value="6" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "6" then asp_Write("selected") end if %>>6</option>
						                            <option value="7" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry1", "priority", "h") = "7" then asp_Write("selected") end if %>>7</option>
					                            </select>
				                            </td>
								                </TR>
								                <TR>
								                    <td class="table1_left">G.711U-law</td>
				                            <td class="table1_right">
					                            <select name="codec2" id="g711u">
						                            <option value="0" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "0" then asp_Write("selected") end if %>>0</option>
						                            <option value="1" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "1" then asp_Write("selected") end if %>>1</option>
						                            <option value="2" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "2" then asp_Write("selected") end if %>>2</option>
						                            <option value="3" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "3" then asp_Write("selected") end if %>>3</option>
						                            <option value="4" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "4" then asp_Write("selected") end if %>>4</option>
						                            <option value="5" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "5" then asp_Write("selected") end if %>>5</option>
						                            <option value="6" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "6" then asp_Write("selected") end if %>>6</option>
						                            <option value="7" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry2", "priority", "h") = "7" then asp_Write("selected") end if %>>7</option>
					                            </select>
				                            </td>
										            </TR>
								                <TR>
								                    <td class="table1_left">G.729</td>
				                            <td class="table1_right">
					                            <select name="codec3" id="g729">
						                            <option value="0" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "0" then asp_Write("selected") end if %>>0</option>
						                            <option value="1" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "1" then asp_Write("selected") end if %>>1</option>
						                            <option value="2" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "2" then asp_Write("selected") end if %>>2</option>
						                            <option value="3" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "3" then asp_Write("selected") end if %>>3</option>
						                            <option value="4" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "4" then asp_Write("selected") end if %>>4</option>
						                            <option value="5" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "5" then asp_Write("selected") end if %>>5</option>
						                            <option value="6" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "6" then asp_Write("selected") end if %>>6</option>
						                            <option value="7" <%if tcWebApi_get("VoIPCodecs_PVC0_Entry3", "priority", "h") = "7" then asp_Write("selected") end if %>>7</option>
					                            </select>
				                            </td>
								                </TR>
								                <TR>
								                    <td class="table1_right" colspan="2" >���� </td>
								                </TR>
								                <tr>
				                          <td class="table1_left">����</td>
				                          <td class="table1_right">
				                          	<input name="mymgtxgain" type="text" id="in_send_out_val" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "VoiceVolumeSpeak", "s") else asp_Write("") end if %>">
				                          	(0-20db)
				                          </td>
				                        </tr>
				                        <tr>
				                          <td class="table1_left">����</td>
				                          <td class="table1_right">
				                          	<input name="mymgrxtain" type="text" id="in_incept1_val" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Entry0", "VoiceVolumeListen", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Entry0", "VoiceVolumeListen", "s") else asp_Write("") end if %>">
				                          	(0-20db)
				                          </td>
				                        </tr>             
				                        <tr>
				                          <td class="table1_left">����ѹ�� </td>
				                          <td class="table1_right" valign="middle">
				                          	<table border="0" cellpadding="0" cellspacing="0">
					                            <tr>
					                              <td align="left"><input name="mymgsliencenble" type="radio" id="loudspeaker0" value="0" <%if tcWebApi_get("VoIPMedia_Entry0", "VAD", "h") <> "1" then asp_Write("checked") end if %>></td>
					                              <td align="left">�� &nbsp;&nbsp;&nbsp;</td>
					                              <td align="left"><input name="mymgsliencenble" type="radio" id="loudspeaker1" value="1" <%if tcWebApi_get("VoIPMedia_Entry0", "VAD", "h") = "1" then asp_Write("checked") end if %>></td>
					                              <td align="left">�� </td>
					                            </tr>
				                            </table>
				                          </td>
				                        </tr>
				                        <tr>
				                          <td class="table1_left">��������</td>
				                          <td class="table1_right" valign="middle">
				                          	<table height="25" border="0" cellpadding="0" cellspacing="0">
					                            <tr>
					                              <td align="left"><input name="mymgechocancel" type="radio" id="aftersound0" value="0"  <%if tcWebApi_get("VoIPAdvanced_Entry0", "EchoCancellationEnable", "h") <> "1" then asp_Write("checked") end if %>></td>
					                              <td align="left">�� &nbsp;&nbsp;&nbsp;</td>
					                              <td align="left"><input name="mymgechocancel" type="radio" id="aftersound1" value="1"  <%if tcWebApi_get("VoIPAdvanced_Entry0", "EchoCancellationEnable", "h") = "1" then asp_Write("checked") end if %>></td>
					                              <td align="left">�� </td>
					                            </tr>
				                            </table>
				                          </td>
				                        </tr>
				                        <tr>
				                          <td class="table1_left">RFC2833����</td>
				                          <td class="table1_right">
				                          	<table border="0" cellpadding="0" cellspacing="0">
					                            <tr>
					                              <td align="left"><input name="mymg2833enable" type="radio" id="rfc2833_0" value="0"  <%if tcWebApi_get("VoIPAdvanced_Entry0", "RFC2833Redundance", "h") <> "1" then asp_Write("checked") end if %>></td>
					                              <td align="left" >�� &nbsp;&nbsp;&nbsp;</td>
					                              <td align="left"><input name="mymg2833enable" type="radio" id="rfc2833_1" value="1"  <%if tcWebApi_get("VoIPAdvanced_Entry0", "RFC2833Redundance", "h") = "1" then asp_Write("checked") end if %>></td>
					                              <td align="left" >�� &nbsp;&nbsp;&nbsp;</td>
					                            </tr>
				                            </table>
				                          </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_right" colspan='2' ><strong id='in_h_resource1'>�߼�����</strong></td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">����ģʽ </td>
				                            <td class="table1_right">
				                            	<select name="megacofaxmode" >
				                                <option value="0" <%if tcWebApi_get("VoIPMedia_Common", "FaxCodec", "h") = "0" then asp_Write("selected") end if %>>͸��</option>
				                                <option value="1" <%if tcWebApi_get("VoIPMedia_Common", "FaxCodec", "h") = "1" then asp_Write("selected") end if %>>T38</option>
				                          	  </select>
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">�������ģʽ</td>
				                            <td class="table1_right">
				                            	<select name="megacofaxcontrol">
				                                <option value="all" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "all" then asp_Write("selected") end if %>>T30ȫ��</option>
				                                <option value="other" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxCtrlMode", "h") = "other" then asp_Write("selected") end if %>>��Э��</option>
				                           		</select>
				                           	</td>
				                        </tr>  
				                        <tr>
																	<td class="table1_left">��Э�̴�������</td>
																	<td>
																		<select name="megacofaxpassthru" size="1">
																			<option value="PCMU" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxPassThruCodec", "h") = "PCMU" then asp_Write("selected") end if %>>PCMU</option>
																			<option value="PCMA" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxPassThruCodec", "h") = "PCMA" then asp_Write("selected") end if %>>PCMA</option>
																		</select>
																	</td>
																</tr>
																<tr> 
				                          <td class="table1_left">��������</td>
				                          <td class="table1_right">
				                          	<input name="megacofaxredundancy" type="checkbox" <%if tcWebApi_get("VoIPAdvanced_Common", "FaxRedundancy", "h") = "1" then asp_Write("checked") end if %>>
				                          </td>
				                        </tr>
				                      	<tr>
				                            <td class="table1_left">��������paylod type </td>
				                            <td class="table1_right">
				                            	<input name="megacofaxredundancypt" type="text" id="in_fax_redundancy_pt_val_1" onblur="limitNum(this)"  value="<%if tcWebApi_get("VoIPAdvanced_Common", "FaxRedundancyPT", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "FaxRedundancyPT", "s") else asp_Write("") end if %>">
				                            	&nbsp;(96-127)
				                            </td>
				                        </tr> 
				                        <tr>
				                          <td class="table1_left">DTMF��ʽ</td>
				                          <td class="table1_right">
				                          	<select name="megacodtmfrelay" onchange="dtmfRelayChange(this)">
					                            <option value="InBand" <%if tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "InBand" then asp_Write("selected") end if %>>Inband</option>
					                            <option value="RFC2833" <%if tcWebApi_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "RFC2833" then asp_Write("selected") end if %>>RFC2833</option>
				                         		</select></td>
				                        </tr>
				                         <tr id="trRFC2833PT">
				                            <td class="table1_left">Telephone Event Payload Type:</td>
				                            <td class="table1_right">
				                            	<input type='text' size=20 maxlength=20 name='megacorfc2833pt' onblur='limitNum(this)' value="<%if tcWebApi_get("VoIPBasic_Common", "TelephoneEventPayloadType", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "TelephoneEventPayloadType", "s") else asp_Write("") end if %>">
				                              &nbsp;(96-127)
				                            </td>
				                        </tr>          
				                        <tr>
				                            <td class="table1_left">�������ʱ�� </td>
				                            <td class="table1_right">
				                            	<input name="megacomaxstartupdelay" type="text" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "MaxStartDelay", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "MaxStartDelay", "s") else asp_Write("") end if %>">
				                            	&nbsp;(s)
				                            </td>
				                        </tr>            
				                        <tr>
				                            <td class="table1_left">����������ʱ�� </td>
				                            <td class="table1_right">
				                            	<input name="megacodigitmapstarttime" type="text" onblur="limitNum(this)" id="in_digitMapStartTime_val_1" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "StartDigitTimer", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "StartDigitTimer", "s") else asp_Write("") end if %>">
				                            	&nbsp;(s)
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">���Ŷ̶�ʱ�� </td>
				                            <td class="table1_right">
				                            	<input name="megacodigitmapshorttime" type="text" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerShort", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerShort", "s") else asp_Write("") end if %>">
				                            	&nbsp;(s)
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">���ų���ʱ�� </td>
				                            <td class="table1_right">
				                            	<input name="megacodigitmapLongtime" type="text" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPDigitMap_Entry", "InterDigitTimerLong", "h") <> "N/A" then tcWebAPI_get("VoIPDigitMap_Entry", "InterDigitTimerLong", "s") else asp_Write("") end if %>">
				                            	&nbsp;(s)
				                            </td>
				                        </tr> 
				                        <tr>
				                            <td class="table1_left">�ýв�Ӧʱ�� </td>
				                            <td class="table1_right">
				                            	<input name="megaconoanswertimer" type="text" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPH248_Common", "NoAnswerTimer", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "NoAnswerTimer", "s") else asp_Write("") end if %>">
				                            	&nbsp;(s)
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">������Ϣ </td>
				                            <td class="table1_right">
				                            	<select name="megacoheartbeatenable">
						                            <option value="0"  <%if tcWebApi_get("VoIPH248_Common", "HeartbeatMode", "h") = "0" then asp_Write("selected") end if %>>�ر� </option>
						                            <option value="1"  <%if tcWebApi_get("VoIPH248_Common", "HeartbeatMode", "h") = "1" then asp_Write("selected") end if %>>NOTIFY��ʽ </option>
				                            	</select>
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">������Ϣ����ģʽ </td>
				                            <td class="table1_right">
				                            	<select name="megacohetbtSendMode">
						                            <option value="0" <%if tcWebApi_get("VoIPH248_Common", "HeartbeatSendMode", "h") = "0" then asp_Write("selected") end if %>>����ģʽ </option>
						                            <option value="1" <%if tcWebApi_get("VoIPH248_Common", "HeartbeatSendMode", "h") = "1" then asp_Write("selected") end if %>>�̶����� </option>
				                            	</select>
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">�������� </td>
				                            <td class="table1_right">
				                            	<input name="megacoHeartBeatTimer" type="text" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPH248_Common", "HeartBeatTimer", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "HeartBeatTimer", "s") else asp_Write("") end if %>">
				                            	&nbsp;(s)
				                            </td>
				                        </tr>
				                        <tr>
						                        <td class="table1_left">���������� </td>
						                        <td class="table1_right">
						                        	<input name="megacoHeartBeatRetransTimes" type="text" onblur="limitNum(this)"  value="<%if tcWebApi_get("VoIPH248_Common", "HeartBeatRetransTimes", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "HeartBeatRetransTimes", "s") else asp_Write("") end if %>">
						                        	&nbsp;
						                        </td>
				                        </tr>             
				                        <tr>
				                          <td class="table1_left">���ܷ��Ͷ˵�ע��</td>
				                          <td class="table1_right">
				                          	<input name="cancellinereg" type="checkbox" <%if tcWebApi_get("VoIPH248_Common", "SmartSendReg", "h") = "1" then asp_Write("checked") end if %>>
				                          </td>
				                        </tr>
				                        <tr>
									                  <td class="table1_left">����ƥ�䷽ʽ</td>
									                  <td>
									                  	<select name="DivDigitMatchMethod" size="1">
											   	                <option value="0" <%if tcWebApi_get("VoIPH248_Common", "NumberMatchMode", "h") = "0" then asp_Write("selected") end if %>>��Сƥ��</option>
											   	                <option value="1" <%if tcWebApi_get("VoIPH248_Common", "NumberMatchMode", "h") = "1" then asp_Write("selected") end if %>>���ƥ��</option>
											   	            </select>
									                  </td>
									               </tr>
				                        <tr>
				                          <td class="table1_left">��������ƥ��</td>
				                          <td class="table1_right">
				                          	<input name="digitmapswitch" type="checkbox" <%if tcWebApi_get("VoIPH248_Common", "NumberSmartMatch", "h") = "1" then asp_Write("checked") end if %>>
				                          </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">�����ѹ </td>
				                            <td class="table1_right">
				                            	<select name="megacoringingvoltage">
				                                <option value="10" <%if tcWebApi_get("VoIPH248_Common", "RingVoltage", "h") = "10" then asp_Write("selected") end if %>>10 V</option>
				                                <option value="20" <%if tcWebApi_get("VoIPH248_Common", "RingVoltage", "h") = "20" then asp_Write("selected") end if %>>20 V</option>
				                                <option value="30" <%if tcWebApi_get("VoIPH248_Common", "RingVoltage", "h") = "30" then asp_Write("selected") end if %>>30 V</option>
				                              </select>
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">�����ʱ�� </td>
				                            <td class="table1_right">
				                            	<input name="megacoFlashMaxTime" type="text" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "FlashMaxTime", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "FlashMaxTime", "s") else asp_Write("") end if %>">
				                            	&nbsp;(ms)
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">��С���ʱ�� </td>
				                            <td class="table1_right">
				                            	<input name="megacoFlashMinTime" type="text" onblur="limitNum(this)" value="<%if tcWebApi_get("VoIPAdvanced_Common", "FlashMinTime", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "FlashMinTime", "s") else asp_Write("") end if %>">
				                            	&nbsp;(ms)
				                            </td>
				                        </tr>
				                        <tr>
				                            <td class="table1_left">�һ���С��� </td>
				                            <td class="table1_right">
				                            	<input name="megacoReleaseMinTime" type="text" id="in_on_hook_release_val_1" onblur="limitNum(this)"  value="<%if tcWebApi_get("VoIPAdvanced_Common", "ReleaseMinTime", "h") <> "N/A" then tcWebAPI_get("VoIPAdvanced_Common", "ReleaseMinTime", "s") else asp_Write("") end if %>">
				                            	&nbsp;(300-1199ms)
				                            </td>
				                        </tr>  				                 
										          </TBODY>
										        </TABLE>
										        <TABLE border=0 cellSpacing=1 cellPadding=0 width="98%">
										        	<tr>
										        		<td height="5">
										        		</td>
										        	</tr>
										        	<tr>
										        		<td>
										        			<input type="hidden" name="VoIP_H248_flag" value="0">
										        			<input type="hidden" name="megacofaxredundancyflag" value="0">
										        			<input type="hidden" name="cancellineregflag" value="0">
										        			<input type="hidden" name="digitmapswitchflag" value="0">
										        		</td>
										        	</tr>
										        	<tr>
					                        <td align="center">
					                          <input name="bt_apply" type="button" id="VoiceApplyButton"  value="Ӧ��"  onClick="VoiceApplyBasic()">
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
          <TD width=162 bgColor=#d8d8d8>��</TD>
          <TD width=278 bgColor=#0059A0>��</TD>
          <TD width=196 bgColor=#0059A0><!-- InstanceEndEditable --></TD>
          <TD width=170 
bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
