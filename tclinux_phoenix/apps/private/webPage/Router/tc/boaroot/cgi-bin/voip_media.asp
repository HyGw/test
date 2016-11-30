<%
IF Request_Form("LineFlag")="1" Then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
End if



If request_Form("VoIP_media_flag")="1" then


	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
	tcWebApi_set("VoIPMedia_Entry","VAD","VoIPVAD")
	tcWebApi_set("VoIPMedia_Entry","EchoCancellationEnable","VoIPEchoCancellation")
If TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then
	tcWebApi_set("VoIPMedia_Entry","SilenceCompressionEnable","VoIPSilenceCompressionEnable")
end if
	tcWebApi_set("VoIPMedia_Entry","CodecIndex","editnum")

	tcWebApi_set("VoIPMedia_Common","711APTime","G711APTime")
	tcWebApi_set("VoIPMedia_Common","711UPTime","G711UPTime")
	tcWebApi_set("VoIPMedia_Common","722PTime","G722PTime")
	tcWebApi_set("VoIPMedia_Common","726-32PTime","G726_32PTime")
	tcWebApi_set("VoIPMedia_Common","729PTime","G729PTime")
    tcWebApi_set("VoIPMedia_Common","AMRPTime","AMRPTime")
    tcWebApi_set("VoIPMedia_Common","723PTime","G723PTime")


	tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs0","Codec1")
	tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs1","Codec2")
	tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs2","Codec3")
	tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs3","Codec4")
	tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs4","Codec5")
	tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs5","Codec6")
	tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs6","Codec7")


	tcWebApi_commit("VoIPMedia_Entry")

	

end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" type='text/javascript' src="/chksel.js"></script>
<script language="JavaScript">


function chkRepeatCodec(){
	var HaveSettingCodecs = new Array(7);
	var f=top.main.document.VOIP_MEDIA_form;
	var i;
	var j;
	var value1;
	var value2;
	
	HaveSettingCodecs[0] = f.Codec1;
	HaveSettingCodecs[1] = f.Codec2;
	HaveSettingCodecs[2] = f.Codec3;
	HaveSettingCodecs[3] = f.Codec4;
	HaveSettingCodecs[4] = f.Codec5;
	HaveSettingCodecs[5] = f.Codec6;
	HaveSettingCodecs[6] = f.Codec7;

	for (i = 0 ; i < 7 ; i++) {
		for (j = i + 1 ; j < 7 ; j++) {
			if (HaveSettingCodecs[i].selectedIndex == HaveSettingCodecs[j].selectedIndex) {
				value1 = HaveSettingCodecs[i].options[HaveSettingCodecs[i].selectedIndex].value;
				value2 = HaveSettingCodecs[j].options[HaveSettingCodecs[j].selectedIndex].value;

				if ((value1 != 'N/A') && (value2 != 'N/A')) {
					alert("Can't set a repeat codec!");
					return 1;
				}
			}
		}
	}
	
	return 0;
}
	
function dosave(){
	var form=document.VOIP_MEDIA_form;
	if(!chkRepeatCodec()){
		form.VoIP_media_flag.value=1;
		form.submit();
	}
}

function doLineChange(){
	document.VOIP_MEDIA_form.LineFlag.value = 1;
	document.VOIP_MEDIA_form.submit();
}
function add_packtime_option(i){
	var f=top.main.document.VOIP_MEDIA_form.VoIPPacketizationTime.options
	var additem;
	if(i=="G.723"){
		additem=new Option("30","30");
		f.add(additem); 
		additem=new Option("60","60");
		f.add(additem);
		additem=new Option("90","90");
		f.add(additem);
	}else{
		additem=new Option("10","10");
		f.add(additem); 
		additem=new Option("20","20");
		f.add(additem);
		additem=new Option("30","30");
		f.add(additem); 
		additem=new Option("40","40");
		f.add(additem);	
	}
}
function checksel(){
	var f=top.main.document.VOIP_MEDIA_form;
	var form=document.VOIP_MEDIA_form;

<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>
		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>			
		
	form.Line.disabled=true;
	form.G711APTime.disabled=true;
	form.G711UPTime.disabled=true;
	form.G722PTime.disabled=true;
	form.G726_32PTime.disabled=true;
	form.G729PTime.disabled=true;
	form.AMRPTime.disabled=true;
	form.G723PTime.disabled=true;

	form.VoIPVAD[0].disabled=true;	
	form.VoIPVAD[1].disabled=true;

<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	form.VoIPPacketizationTime.disabled=true;
	form.VoIPSupportedCodecs.disabled=true;
	form.VoIPSilenceCompressionEnable[0].disabled=true;
	form.VoIPSilenceCompressionEnable[1].disabled=true;
<%end if%>

<%end if%>
<%end if%>


}

</script>
</head><body onload="checksel()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_media.asp" name="VOIP_MEDIA_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">



	<tr>
	<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
	<td width="150" height="30" class="title-main">
	<font color="#FFFFFF">Common settings</font></td>
	<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>
	<INPUT TYPE="HIDDEN" NAME="LineFlag" VALUE="0">

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">G711-A PTime</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="G711APTime" SIZE="1">
				<OPTION value="10"<%if TCWebAPI_get("VoIPMedia_Common", "711APTime", "h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="20"<%if TCWebAPI_get("VoIPMedia_Common", "711APTime", "h") = "20" then asp_Write("selected") end if %>>20
				<OPTION value="30"<%if TCWebAPI_get("VoIPMedia_Common", "711APTime", "h") = "30" then asp_Write("selected") end if %>>30
				<OPTION value="40"<%if TCWebAPI_get("VoIPMedia_Common", "711APTime", "h") = "40" then asp_Write("selected") end if %>>40
			</SELECT>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">G711-U PTime</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="G711UPTime" SIZE="1">
				<OPTION value="10"<%if TCWebAPI_get("VoIPMedia_Common", "711UPTime", "h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="20"<%if TCWebAPI_get("VoIPMedia_Common", "711UPTime", "h") = "20" then asp_Write("selected") end if %>>20
				<OPTION value="30"<%if TCWebAPI_get("VoIPMedia_Common", "711UPTime", "h") = "30" then asp_Write("selected") end if %>>30
				<OPTION value="40"<%if TCWebAPI_get("VoIPMedia_Common", "711UPTime", "h") = "40" then asp_Write("selected") end if %>>40
			</SELECT>
		</td></tr>	
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">G722 PTime</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="G722PTime" SIZE="1">
				<OPTION value="10"<%if TCWebAPI_get("VoIPMedia_Common", "722PTime", "h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="20"<%if TCWebAPI_get("VoIPMedia_Common", "722PTime", "h") = "20" then asp_Write("selected") end if %>>20
				<OPTION value="30"<%if TCWebAPI_get("VoIPMedia_Common", "722PTime", "h") = "30" then asp_Write("selected") end if %>>30
				<OPTION value="40"<%if TCWebAPI_get("VoIPMedia_Common", "722PTime", "h") = "40" then asp_Write("selected") end if %>>40
			</SELECT>
		</td></tr>	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">G726-32 PTime</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="G726_32PTime" SIZE="1">
				<OPTION value="10"<%if TCWebAPI_get("VoIPMedia_Common", "726-32PTime", "h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="20"<%if TCWebAPI_get("VoIPMedia_Common", "726-32PTime", "h") = "20" then asp_Write("selected") end if %>>20
				<OPTION value="30"<%if TCWebAPI_get("VoIPMedia_Common", "726-32PTime", "h") = "30" then asp_Write("selected") end if %>>30
				<OPTION value="40"<%if TCWebAPI_get("VoIPMedia_Common", "726-32PTime", "h") = "40" then asp_Write("selected") end if %>>40
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">G729 PTime</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="G729PTime" SIZE="1">
				<OPTION value="10"<%if TCWebAPI_get("VoIPMedia_Common", "729PTime", "h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="20"<%if TCWebAPI_get("VoIPMedia_Common", "729PTime", "h") = "20" then asp_Write("selected") end if %>>20
				<OPTION value="30"<%if TCWebAPI_get("VoIPMedia_Common", "729PTime", "h") = "30" then asp_Write("selected") end if %>>30
				<OPTION value="40"<%if TCWebAPI_get("VoIPMedia_Common", "729PTime", "h") = "40" then asp_Write("selected") end if %>>40
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">AMR PTime</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="AMRPTime" SIZE="1">
				<OPTION value="20"<%if TCWebAPI_get("VoIPMedia_Common", "AMRPTime", "h") = "20" then asp_Write("selected") end if %>>20
				<OPTION value="40"<%if TCWebAPI_get("VoIPMedia_Common", "AMRPTime", "h") = "40" then asp_Write("selected") end if %>>40
			</SELECT>
		</td></tr>

	<tr>
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

<tr>
	<tr>
	<td width="150" height="30" class="title-main">
	<font color="#FFFFFF">Line settings</font></td>
	<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>

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
<%end if%></select></td>
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
		<td class="tabdata" align=right><font color="#000000">VAD & Silence compression</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPVAD" VALUE="Yes" <%if TCWebAPI_get("VoIPMedia_Entry", "VAD", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPVAD" VALUE="No" <%if TCWebAPI_get("VoIPMedia_Entry", "VAD", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Silence compression</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPSilenceCompressionEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPMedia_Entry", "SilenceCompressionEnable", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPSilenceCompressionEnable" VALUE="No" <%if TCWebAPI_get("VoIPMedia_Entry", "SilenceCompressionEnable", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>		
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Echo cancellation</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">	
			<INPUT TYPE="RADIO" NAME="VoIPEchoCancellation" VALUE="Yes" <%if TCWebAPI_get("VoIPMedia_Entry", "EchoCancellationEnable", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPEchoCancellation" VALUE="No" <%if TCWebAPI_get("VoIPMedia_Entry", "EchoCancellationEnable", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>	
<% end if %>

	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">CODEC Settings</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade class="light-orange-line"></td><td width="10"><hr noshade class="light-orange-line"></td><td width="440"><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">1st Codec</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Codec1" SIZE="1">
				<OPTION value="G.711 A-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "G.711 A-law" then asp_Write("selected") end if %>>G.711 A-law
				<OPTION value="G.711 U-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "G.711 U-law" then asp_Write("selected") end if %>>G.711 U-law
				<OPTION value="G.722"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "G.722" then asp_Write("selected") end if %>>G.722
				<OPTION value="G.726 - 32"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "G.726 - 32" then asp_Write("selected") end if %>>G.726 - 32
				<OPTION value="G.729"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "G.729" then asp_Write("selected") end if %>>G.729
				<OPTION value="AMR"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "AMR" then asp_Write("selected") end if %>>AMR
				<OPTION value="G.723"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "G.723" then asp_Write("selected") end if %>>G.723
				<OPTION value="N/A"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "h") = "N/A" then asp_Write("selected") end if %>>N/A
			</SELECT>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">2nd Codec</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Codec2" SIZE="1">
				<OPTION value="G.711 A-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "G.711 A-law" then asp_Write("selected") end if %>>G.711 A-law
				<OPTION value="G.711 U-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "G.711 U-law" then asp_Write("selected") end if %>>G.711 U-law
				<OPTION value="G.722"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "G.722" then asp_Write("selected") end if %>>G.722
				<OPTION value="G.726 - 32"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "G.726 - 32" then asp_Write("selected") end if %>>G.726 - 32
				<OPTION value="G.729"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "G.729" then asp_Write("selected") end if %>>G.729
				<OPTION value="AMR"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "AMR" then asp_Write("selected") end if %>>AMR
				<OPTION value="G.723"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "G.723" then asp_Write("selected") end if %>>G.723
				<OPTION value="N/A"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "h") = "N/A" then asp_Write("selected") end if %>>N/A
			</SELECT>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">3rd Codec</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Codec3" SIZE="1">
				<OPTION value="G.711 A-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "G.711 A-law" then asp_Write("selected") end if %>>G.711 A-law
				<OPTION value="G.711 U-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "G.711 U-law" then asp_Write("selected") end if %>>G.711 U-law
				<OPTION value="G.722"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "G.722" then asp_Write("selected") end if %>>G.722
				<OPTION value="G.726 - 32"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "G.726 - 32" then asp_Write("selected") end if %>>G.726 - 32
				<OPTION value="G.729"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "G.729" then asp_Write("selected") end if %>>G.729
				<OPTION value="AMR"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "AMR" then asp_Write("selected") end if %>>AMR
				<OPTION value="G.723"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "G.723" then asp_Write("selected") end if %>>G.723
				<OPTION value="N/A"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "h") = "N/A" then asp_Write("selected") end if %>>N/A
			</SELECT>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">4th Codec</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Codec4" SIZE="1">
				<OPTION value="G.711 A-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "G.711 A-law" then asp_Write("selected") end if %>>G.711 A-law
				<OPTION value="G.711 U-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "G.711 U-law" then asp_Write("selected") end if %>>G.711 U-law
				<OPTION value="G.722"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "G.722" then asp_Write("selected") end if %>>G.722
				<OPTION value="G.726 - 32"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "G.726 - 32" then asp_Write("selected") end if %>>G.726 - 32
				<OPTION value="G.729"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "G.729" then asp_Write("selected") end if %>>G.729
				<OPTION value="AMR"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "AMR" then asp_Write("selected") end if %>>AMR
				<OPTION value="G.723"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "G.723" then asp_Write("selected") end if %>>G.723
				<OPTION value="N/A"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "h") = "N/A" then asp_Write("selected") end if %>>N/A
			</SELECT>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">5th Codec</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Codec5" SIZE="1">
				<OPTION value="G.711 A-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "G.711 A-law" then asp_Write("selected") end if %>>G.711 A-law
				<OPTION value="G.711 U-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "G.711 U-law" then asp_Write("selected") end if %>>G.711 U-law
				<OPTION value="G.722"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "G.722" then asp_Write("selected") end if %>>G.722
				<OPTION value="G.726 - 32"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "G.726 - 32" then asp_Write("selected") end if %>>G.726 - 32
				<OPTION value="G.729"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "G.729" then asp_Write("selected") end if %>>G.729
				<OPTION value="AMR"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "AMR" then asp_Write("selected") end if %>>AMR
				<OPTION value="G.723"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "G.723" then asp_Write("selected") end if %>>G.723
				<OPTION value="N/A"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "h") = "N/A" then asp_Write("selected") end if %>>N/A
			</SELECT>
		</td></tr>
    
    <tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">6th Codec</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Codec6" SIZE="1">
				<OPTION value="G.711 A-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "G.711 A-law" then asp_Write("selected") end if %>>G.711 A-law
				<OPTION value="G.711 U-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "G.711 U-law" then asp_Write("selected") end if %>>G.711 U-law
				<OPTION value="G.722"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "G.722" then asp_Write("selected") end if %>>G.722
				<OPTION value="G.726 - 32"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "G.726 - 32" then asp_Write("selected") end if %>>G.726 - 32
				<OPTION value="G.729"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "G.729" then asp_Write("selected") end if %>>G.729
				<OPTION value="AMR"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "AMR" then asp_Write("selected") end if %>>AMR
				<OPTION value="G.723"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "G.723" then asp_Write("selected") end if %>>G.723
				<OPTION value="N/A"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs5", "h") = "N/A" then asp_Write("selected") end if %>>N/A
			</SELECT>
		</td></tr>

	<tr>	
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">7th Codec</font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Codec7" SIZE="1">
				<OPTION value="G.711 A-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "G.711 A-law" then asp_Write("selected") end if %>>G.711 A-law
				<OPTION value="G.711 U-law"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "G.711 U-law" then asp_Write("selected") end if %>>G.711 U-law
				<OPTION value="G.722"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "G.722" then asp_Write("selected") end if %>>G.722
				<OPTION value="G.726 - 32"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "G.726 - 32" then asp_Write("selected") end if %>>G.726 - 32
				<OPTION value="G.729"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "G.729" then asp_Write("selected") end if %>>G.729
				<OPTION value="AMR"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "AMR" then asp_Write("selected") end if %>>AMR
				<OPTION value="G.723"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "G.723" then asp_Write("selected") end if %>>G.723
				<OPTION value="N/A"<%if TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs6", "h") = "N/A" then asp_Write("selected") end if %>>N/A
			</SELECT>
		</td></tr>	

	<tr>	
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<INPUT TYPE="HIDDEN" NAME="VoIPSupportedCodecsTmp" VALUE="N/A">
		<td class="tabdata" align=right><font color="#000000">Priority index</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata" id="PriorityIndex"><%tcWebApi_get("VoIPMedia_Entry","CodecIndex", "s")%>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Supported codec</font></td>

		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<SELECT NAME="VoIPSupportedCodecs" SIZE="1" 
				<OPTION value="G.711 A-law" >G.711 A-law
				<OPTION value="G.711 U-law" >G.711 U-law
				<OPTION value="G.722" >G.722
				<OPTION value="G.726 - 32" >G.726 - 32
				<OPTION value="G.729" >G.729
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
				<OPTION value="G.726 - 16" >G.726 - 16
				<OPTION value="G.726 - 24" >G.726 - 24
				<OPTION value="G.726 - 40" >G.726 - 40
				<OPTION value="G.723" >G.723

				
<% end if %>
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Packetization time</font></td>
		<td class="tabdata" align=center>:</td>
		<INPUT TYPE="HIDDEN" NAME="VoIPPacketizationTimeTmp" VALUE="N/A">
		<td class="tabdata">
			<SELECT NAME="VoIPPacketizationTime" SIZE="1">
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
	<TR>
		<TD class=light-orange width=150>&nbsp;</TD>
		<TD class=light-orange width=10></TD>

		<%If request_Form("Line")="1" then %>
			<TD class=tabdata><iframe src="/cgi-bin/voip_media2.cgi" id="cgi_frame" frameborder="0" width="550" height="250"></iframe></TD>	
		
		<%else %>
			<TD class=tabdata><iframe src="/cgi-bin/voip_media.cgi" id="cgi_frame" frameborder="0" width="550" height="250"></iframe></TD>		
		<% end if %>

		</TR></TBODY></TABLE>
<% end if %>		
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="VoIP_media_flag" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="editnum" VALUE="<%tcWebApi_get("WebCurSet_Entry","SIPIndexMedia","s")%>">
		<INPUT TYPE="HIDDEN" NAME="delFlag" VALUE="0">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="dosave();"></td></tr></table>
</form></body></html>
