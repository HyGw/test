<%
If Request_Form("SaveFlag") = "1" then
	TCWebApi_set("Timezone_Entry","TZ","cboTimeZone")
	TCWebApi_set("Timezone_Entry","TYPE","autotimeFlag")
	TCWebApi_set("Timezone_Entry","SERVER","ntpServerOther1Flag")
	TCWebApi_set("Timezone_Entry","SERVER2","ntpServerOther2Flag")
	if Request_Form("isNtpServerTypeSupported") = "1" then
		TCWebApi_set("Timezone_Entry","NTPServerType","AddRouteTimeZone")
	else
		TCWebApi_set("Timezone_Entry","AddRoute","AddRouteTimeZone")
	end if
	TCWebApi_commit("Timezone_Entry")
	TCWebApi_save()
End if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>ʱ�����</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name=GENERATOR>
<% if tcWebApi_get("WebCustom_Entry","isCUCTimeDispSupported", "h") = "Yes" then %>
<script>
function ReloadPage()
{
	window.location.replace("net-time.asp");
}
function refresh()
{
	setTimeout("ReloadPage()", 5000);
}
</script>
<% end if %>
</HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()<% if tcWebApi_get("WebCustom_Entry","isCUCTimeDispSupported", "h") = "Yes" then asp_Write(";refresh()") end if %>" 
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
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=���� color=#ffffff><B><FONT face=���� 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="����->ʱ�����" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
            <P class=GatewayType align=center>�ͺ�:
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
      <SCRIPT language=JavaScript type=text/javascript>
function stSntp(domain,Enable,ntp1,ntp2,ZoneName)
{
	this.domain = domain;
	this.Enable = Enable;
	this.ntp1 = ntp1;
	this.ntp2 = ntp2;
	this.ZoneName = ZoneName;
}
var SntpEntry = new Array(new stSntp("InternetGatewayDevice.Time","0","clock.fmt.he.net","clock.nyc.he.net","Beijing"),null);
var Sntp = SntpEntry[0];
var localtimename = '';
var ntpServers = new Array();
ntpServers[0] = 'clock.fmt.he.net';
ntpServers[1] = 'clock.nyc.he.net';
ntpServers[2] = 'clock.sjc.he.net';
ntpServers[3] = 'clock.via.net';
ntpServers[4] = 'ntp1.tummy.com';
ntpServers[5] = 'time.cachenetworks.com';
ntpServers[6] = 'time.nist.gov';
var timeZones = new Array();
timeZones[0] = '(GMT-12:00) �������ڱ����(��)';
timeZones[1] = '(GMT-11:00) ��;������Ħ��';
timeZones[2] = '(GMT-10:00) ������';
timeZones[3] = '(GMT-09:00) ����˹��';
timeZones[4] = '(GMT-08:00) ����������׼ʱ�䣬�Ừ��';
timeZones[5] = '(GMT-07:00) ����ɣ���ݣ���������';
timeZones[6] = '(GMT-07:00) �����ߣ�����˹';
timeZones[7] = '(GMT-07:00) ɽ��ʱ��(���ô�)';
timeZones[8] = '(GMT-06:00) ������';
timeZones[9] = '(GMT-06:00) �����׼ʱ��(���������ͼ��ô��в�)';
timeZones[10] = '(GMT-06:00) �ϴ���������ī����ǣ�������';
timeZones[11] = '(GMT-06:00) ��˹������ʡ';
timeZones[12] = '(GMT-05:00) �������������';
timeZones[13] = '(GMT-05:00) ����ʱ��';
timeZones[14] = '(GMT-05:00) ӡ�ڰ�����';
timeZones[15] = '(GMT-04:00) ������ʱ��';
timeZones[16] = '(GMT-04:00) ������˹������˹';
timeZones[17] = '(GMT-04:00) ʥ���Ǹ�';
timeZones[18] = '(GMT-03:30) Ŧ����';
timeZones[19] = '(GMT-03:00) ��������';
timeZones[20] = '(GMT-03:00) ����ŵ˹����˹�����ζ�';
timeZones[21] = '(GMT-03:00) ������';
timeZones[22] = '(GMT-02:00) �д��������';
timeZones[23] = '(GMT-01:00) ������Ⱥ��';
timeZones[24] = '(GMT-01:00) ��ý�Ⱥ��';
timeZones[25] = '(GMT) ����������������ά��';
timeZones[26] = '(GMT) �������α�׼ʱ�䣺�����֣�����������˹��,�׶�';
timeZones[27] = '(GMT+01:00) ��ķ˹�ص������֣�������, ����˹�¸��Ħ��άҲ��';
timeZones[28] = '(GMT+01:00) ���������£�������˹������¬�������ǣ�������';
timeZones[29] = '(GMT+01:00) ��³�������籾��������������';
timeZones[30] = '(GMT+01:00) �������ѣ�˹�������ɳ�������ղ�';
timeZones[31] = '(GMT+01:00) ������';
timeZones[32] = '(GMT+02:00) �ŵ䣬��˹̹��������˹��';
timeZones[33] = '(GMT+02:00) ������˹��';
timeZones[34] = '(GMT+02:00) ����';
timeZones[35] = '(GMT+02:00) �����ף�����������';
timeZones[36] = '(GMT+02:00) �ն���������������ӣ������ǣ�����, ά��Ŧ˹';
timeZones[37] = '(GMT+02:00) Ү·����';
timeZones[38] = '(GMT+03:00) �͸��';
timeZones[39] = '(GMT+03:00) �����أ����ŵ�';
timeZones[40] = '(GMT+03:00) Ī˹�ƣ�ʥ�˵ñ��������Ӹ���';
timeZones[41] = '(GMT+03:00) �����';
timeZones[42] = '(GMT+03:30) �º���';
timeZones[43] = '(GMT+04:00) �������ȣ���˹����';
timeZones[44] = '(GMT+04:00) �Ϳ⣬�ڱ���˹��Ү����';
timeZones[45] = '(GMT+04:30) ������';
timeZones[46] = '(GMT+05:00) Ҷ�����ձ�';
timeZones[47] = '(GMT+05:00) ��˹�����������棬��ʲ��';
timeZones[48] = '(GMT+05:30) ���Σ��Ӷ����������µ���';
timeZones[49] = '(GMT+05:45) �ӵ�����';
timeZones[50] = '(GMT+06:00) ����ľͼ������������';
timeZones[51] = '(GMT+06:00) ��˹̹�ɣ��￨';
timeZones[52] = '(GMT+06:00) ˹������';
timeZones[53] = '(GMT+06:30) ����';
timeZones[54] = '(GMT+07:00) ���ȣ����ڣ��żӴ�';
timeZones[55] = '(GMT+07:00) ��˹ŵ�Ƕ�˹��';
timeZones[56] = '(GMT+08:00) ���������죬��ۣ���³ľ��';
timeZones[57] = '(GMT+08:00) ������οˣ���������';
timeZones[58] = '(GMT+08:00) ��¡�£��¼���';
timeZones[59] = '(GMT+08:00) ��˹';
timeZones[60] = '(GMT+08:00) ̨��';
timeZones[61] = '(GMT+09:00) ���棬���ϣ�����';
timeZones[62] = '(GMT+09:00) �׶�';
timeZones[63] = '(GMT+09:00) �ſ�Ŀ�';
timeZones[64] = '(GMT+09:30) ��������';
timeZones[65] = '(GMT+09:30) �����';
timeZones[66] = '(GMT+10:00) ����˹��';
timeZones[67] = '(GMT+10:00) ��������ī������Ϥ��';
timeZones[68] = '(GMT+10:00) �ص���Ī���ȱȸ�';
timeZones[69] = '(GMT+10:00) ������';
timeZones[70] = '(GMT+10:00) ��������˹�п�';
timeZones[71] = '(GMT+11:00) ��ӵ�';
timeZones[72] = '(GMT+11:00) ������Ⱥ�����¿��������';
timeZones[73] = '(GMT+12:00) �¿����������';
timeZones[74] = '(GMT+12:00) 쳼ã�����Ӱ뵺�����ܶ�Ⱥ��';
var timeZonesEng = new Array();
timeZonesEng[0] = '(GMT-12:00) International Date Line West';
timeZonesEng[1] = '(GMT-11:00) Midway Island, Samoa';
timeZonesEng[2] = '(GMT-10:00) Hawaii';
timeZonesEng[3] = '(GMT-09:00) Alaska';
timeZonesEng[4] = '(GMT-08:00) Pacific Time, Tijuana';
timeZonesEng[5] = '(GMT-07:00) Arizona, Mazatlan';
timeZonesEng[6] = '(GMT-07:00-1) Chihuahua, La Paz';
timeZonesEng[7] = '(GMT-07:00-2) Mountain Time';
timeZonesEng[8] = '(GMT-06:00) Central America';
timeZonesEng[9] = '(GMT-06:00-1) Central Time';
timeZonesEng[10] = '(GMT-06:00-2) Guadalajara, Mexico City, Monterrey';
timeZonesEng[11] = '(GMT-06:00-3) Saskatchewan';
timeZonesEng[12] = '(GMT-05:00) Bogota, Lima, Quito';
timeZonesEng[13] = '(GMT-05:00-1) Eastern Time';
timeZonesEng[14] = '(GMT-05:00-2) Indiana';
timeZonesEng[15] = '(GMT-04:00) Atlantic Time';
timeZonesEng[16] = '(GMT-04:00-1) Caracas, La Paz';
timeZonesEng[17] = '(GMT-04:00-2) Santiago';
timeZonesEng[18] = '(GMT-03:30) Newfoundland';
timeZonesEng[19] = '(GMT-03:00) Brasilia';
timeZonesEng[20] = '(GMT-03:00-1) Buenos Aires, Georgetown';
timeZonesEng[21] = '(GMT-03:00-2) Greenland';
timeZonesEng[22] = '(GMT-02:00) Mid-Atlantic';
timeZonesEng[23] = '(GMT-01:00) Azores';
timeZonesEng[24] = '(GMT-01:00-1) Cape Verde Is.';
timeZonesEng[25] = '(GMT) Casablanca, Monrovia';
timeZonesEng[26] = '(GMT-1) Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London';
timeZonesEng[27] = '(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna';
timeZonesEng[28] = '(GMT+01:00-1) Belgrade, Bratislava, Budapest, Ljubljana, Prague';
timeZonesEng[29] = '(GMT+01:00-2) Brussels, Copenhagen, Madrid, Paris';
timeZonesEng[30] = '(GMT+01:00-3) Sarajevo, Skopje, Warsaw, Zagreb';
timeZonesEng[31] = '(GMT+01:00-4) West Central Africa';
timeZonesEng[32] = '(GMT+02:00) Athens, Istanbul, Minsk';
timeZonesEng[33] = '(GMT+02:00-1) Bucharest';
timeZonesEng[34] = '(GMT+02:00-2) Cairo';
timeZonesEng[35] = '(GMT+02:00-3) Harare, Pretoria';
timeZonesEng[36] = '(GMT+02:00-4) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius';
timeZonesEng[37] = '(GMT+02:00-5) Jerusalem';
timeZonesEng[38] = '(GMT+03:00) Baghdad';
timeZonesEng[39] = '(GMT+03:00-1) Kuwait, Riyadh';
timeZonesEng[40] = '(GMT+03:00-2) Moscow, St. Petersburg, Volgograd';
timeZonesEng[41] = '(GMT+03:00-3) Nairobi';
timeZonesEng[42] = '(GMT+03:30) Tehran';
timeZonesEng[43] = '(GMT+04:00) Abu Dhabi, Muscat';
timeZonesEng[44] = '(GMT+04:00-1) Baku, Tbilisi, Yerevan';
timeZonesEng[45] = '(GMT+04:30) Kabul';
timeZonesEng[46] = '(GMT+05:00) Ekaterinburg';
timeZonesEng[47] = '(GMT+05:00-1) Islamabad, Karachi, Tashkent';
timeZonesEng[48] = '(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi';
timeZonesEng[49] = '(GMT+05:45) Kathmandu';
timeZonesEng[50] = '(GMT+06:00) Almaty, Novosibirsk';
timeZonesEng[51] = '(GMT+06:00-1) Astana, Dhaka';
timeZonesEng[52] = '(GMT+06:00-2) Sri Jayawardenepura';
timeZonesEng[53] = '(GMT+06:30) Rangoon';
timeZonesEng[54] = '(GMT+07:00) Bangkok, Hanoi, Jakarta';
timeZonesEng[55] = '(GMT+07:00-1) Krasnoyarsk';
timeZonesEng[56] = '(GMT+08:00) Beijing, Chongquing, Hong Kong, Urumqi';
timeZonesEng[57] = '(GMT+08:00-1) Irkutsk, Ulaan Bataar';
timeZonesEng[58] = '(GMT+08:00-2) Kuala Lumpur, Singapore';
timeZonesEng[59] = '(GMT+08:00-3) Perth';
timeZonesEng[60] = '(GMT+08:00-4) Taipei';
timeZonesEng[61] = '(GMT+09:00) Osaka, Sapporo, Tokyo';
timeZonesEng[62] = '(GMT+09:00-1) Seoul';
timeZonesEng[63] = '(GMT+09:00-2) Yakutsk';
timeZonesEng[64] = '(GMT+09:30) Adelaide';
timeZonesEng[65] = '(GMT+09:30-1) Darwin';
timeZonesEng[66] = '(GMT+10:00) Brisbane';
timeZonesEng[67] = '(GMT+10:00-1) Canberra, Melbourne, Sydney';
timeZonesEng[68] = '(GMT+10:00-2) Guam, Port Moresby';
timeZonesEng[69] = '(GMT+10:00-3) Hobart';
timeZonesEng[70] = '(GMT+10:00-4) Vladivostok';
timeZonesEng[71] = '(GMT+11:00) Magadan';
timeZonesEng[72] = '(GMT+11:00-1) Solomon Is., New Caledonia';
timeZonesEng[73] = '(GMT+12:00) Auckland, Wellington';
timeZonesEng[74] = '(GMT+12:00-1) Fiji, Kamchatka, Marshall Is.';
function getTimeZoneIndex(timeZoneName)
{
	var i = 0, ret = 4;  
	for ( var i = 0; i < timeZonesEng.length; i++ )
	{
		if ( timeZonesEng[i].search(timeZoneName) != -1 )
			break;
		if(timeZonesEng[i] == timeZoneName)
			break;
	}
	if ( i < timeZonesEng.length )
		ret = i;
	return ret;
}
function genlocaltimename()
{
	var obj = getElement('cboTimeZone');
	var index = obj.selectedIndex;
	var val = obj.options[index].value;
	localtimename += "GMT";
	localtimename += val;
	if (val == '')
	{
		localtimename += '00:00:00';
	}
}
function getTimeZoneName(idx) {
	var str = timeZonesEng[idx];
	return str;
}
function getTimeZoneOffset(idx) {
	var str = timeZonesEng[idx];
	var ret = '';
	var nLength = str.indexOf(")");
	ret = str.substr(1, nLength-1);
	return ret;
}
function writeTimeZoneList()
{
	var  vValue;
	var	bExit = 0;
	for( var i = 0; i < timeZones.length; i++ )
	{
		vValue = getTimeZoneOffset(i);
		document.writeln("<option value=" + vValue + ">" + timeZones[i] + "</option>");
		if(document.ConfigForm.CountryName.value == vValue)
		{
			document.ConfigForm.cboTimeZone.selectedIndex= i;
			bExit = 1;
		}
		else
		{
			if((timeZones[i].match("GMT+08:00") != null) && (0 == bExit))
			{
				document.ConfigForm.cboTimeZone.selectedIndex= i;
			}
		}	
	}
}
function ntpChange(optionlist,textbox)
{
if(document.ConfigForm.ntpEnabled.checked)
{
	if( optionlist[optionlist.selectedIndex].value == "Other" )
	{
		textbox.disabled = false;
	}
	else
	{
		textbox.value = "";
		textbox.disabled = true;
	}
}
}

function hideNtpConfig(hide)
{
	var status = 'visible';
	if(hide)
		status = 'hidden';
	if( document.getElementById )
		document.getElementById('ntpConfig').style.visibility = status;
	else
		if(!document.layers)
			document.all.ntpConfig.style.visibility = status;
}
function ntpEnblChange()
{
	if( document.forms[0].ntpEnabled.checked )
	{
		hideNtpConfig(0);
		document.forms[0].autotimeFlag.value = "0";//0 is auto time
	}
	else
	{
		hideNtpConfig(1);
		document.forms[0].autotimeFlag.value = "2";//2 is manual 
	}
}
function writeNtpList(needed)
{
	var  vFlag = "0";
	if(!needed)
	{
		document.writeln("<option value=None>None</option>");
	}
	for( var i = 0; i < ntpServers.length; i++ )
	{
		document.writeln("<option value=" + ntpServers[i] + ">" + ntpServers[i] + "</option>");
		
		if(needed)
		{
			if(document.ConfigForm.ntpServerOther1Flag.value == ntpServers[i])
			{
				document.ConfigForm.ntpServer1.selectedIndex = i;
				vFlag = "1";
			}
		}
		else
		{
			if(document.ConfigForm.ntpServerOther2Flag.value == ntpServers[i])
			{
				document.ConfigForm.ntpServer2.selectedIndex = i+1;
				vFlag = "1";
			}
		}
	}
	document.writeln("<option value=Other>Other</option>");
	if(vFlag == "0")
	{
		if(needed) document.ConfigForm.ntpServer1.selectedIndex =  ntpServers.length;
		else {
			if(document.ConfigForm.ntpServerOther2Flag.value == "None")
				document.ConfigForm.ntpServer2.selectedIndex =  0;
			else
				document.ConfigForm.ntpServer2.selectedIndex =  ntpServers.length+1;
		}
	}
}
function LoadFrame()
{
	var i = 0;
	var ntp1 = Sntp.ntp1 ;
	var ntp2 = Sntp.ntp2;
	var ntp_enabled = Sntp.Enable;
	var tz_name = Sntp.ZoneName;
	with (document.forms[0])
	{
		ntpEnblChange();
		ntpChange(ntpServer1,ntpServerOther1);
		ntpChange(ntpServer2,ntpServerOther2);
	}
}
function isIpFormat(str)
{
	var addrParts = str.split('.');
	if (addrParts.length != 4 )
		return false;
	if (isNaN(addrParts[0] || addrparts[i] == ""))
		return false;
	if (isPlusInteger(addrParts[0]) == false)
		return false;
	return true;
}
function isTValidName(name) {
	var i = 0;
	var unsafeString = "\"<>%\\^[]`\+\$\,='#&:;*/{} \t";
	for ( i = 0; i < name.length; i++ ) {
		for( j = 0; j < unsafeString.length; j++)
			if ( (name.charAt(i)) == unsafeString.charAt(j) )
				return false;
	}
	return true;
}
function checkntpServrState()
{
	var  vForm = document.ConfigForm;
	if(vForm.ntpEnabled.checked) {
		if(vForm.ntpServer1.value == "Other")
			vForm.ntpServerOther1Flag.value = vForm.ntpServerOther1.value;
		else vForm.ntpServerOther1Flag.value = vForm.ntpServer1.value;
	
		if(vForm.ntpServer2.value == "Other")
			vForm.ntpServerOther2Flag.value = vForm.ntpServerOther2.value;
		else vForm.ntpServerOther2Flag.value = vForm.ntpServer2.value;
	}
}
function SubmitForm()
{
	with( document.forms[0] )
	{
		if( ntpEnabled.checked )
		{
			if( ntpServer1.selectedIndex == ntpServers.length )
			{
				if( ntpServerOther1.value.length == 0 )
				{ 
					alert('��SNTP������ѡ��"Other"ʱ,������������Ϊ�ա�');
					document.ConfigForm.SaveFlag.value = 0;
					return;
				}
				if(isIpv6Address(ntpServerOther1.value))
				{
					if(isUnicastIpv6Address(ntpServerOther1.value) == false)
					{
							alert('����������ַ�Ƿ�');
							document.ConfigForm.SaveFlag.value = 0;
							return;
					}
				}
				else if (isIpFormat(ntpServerOther1.value))
				{
					if ( '0.0.0.0' != ntpServerOther1.value && isValidIpAddress(ntpServerOther1.value) == false)
					{
						alert('����������ַ�Ƿ�');
						document.ConfigForm.SaveFlag.value = 0;
						return;
					}
				}
				else
				{
						if(isTValidName(ntpServerOther1.value) == false)
						{
								alert('����������ַ�Ƿ�');
								document.ConfigForm.SaveFlag.value = 0;
								return;
						}
				}
			}
			if( ntpServer2.selectedIndex == ntpServers.length+1 )
			{
				if( ntpServerOther2.value.length == 0 )
				{ 
					alert('��SNTP������ѡ��"Other"ʱ,������������Ϊ�ա�');
					document.ConfigForm.SaveFlag.value = 0;
					return;
				}
				if(isIpv6Address(ntpServerOther2.value))
				{
						if(isUnicastIpv6Address(ntpServerOther2.value) == false)
						{
							alert('�ӷ�������ַ�Ƿ�');
							document.ConfigForm.SaveFlag.value = 0;
							return;
						}
					}
				else if (isIpFormat(ntpServerOther2.value))
				{
					if ('0.0.0.0' != ntpServerOther2.value && isValidIpAddress(ntpServerOther2.value) == false)
					{
						alert('�ӷ�������ַ�Ƿ�');
						document.ConfigForm.SaveFlag.value = 0;
						return;
					}
				}
				else
				{
					if(isTValidName(ntpServerOther2.value) == false)
					{
						alert('�ӷ�������ַ�Ƿ�');
						document.ConfigForm.SaveFlag.value = 0;
						return;
					}
				}
			}
		}
	}
	checkntpServrState();
	document.ConfigForm.SaveFlag.value = "1";
	document.ConfigForm.submit();
}

<% if tcWebApi_get("WebCustom_Entry","isNtpServerTypeSupported","h") = "Yes" then %>
function listCreate(l_flag)
{
		var slNtpServerType = "<% tcWebApi_get("Timezone_Entry","NTPServerType","s") %>";
		var vEntryName = new Array();	
		vEntryName[0]="INTERNET";
		vEntryName[1]="VOICE";
		vEntryName[2]="TR069";
		vEntryName[3]="OTHER";
		for (i = 0; i < 4; i++)
		{
<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") = "Yes" then %>
				if ( 1 == i )
					continue;
<% end if %>
				if(slNtpServerType == i)
				{
					document.write('<option value=' + i + ' selected>'	+ vEntryName[i] + '</option>');
				}
				else
				{
					document.write('<option value=' + i + '>'	+ vEntryName[i] + '</option>');
				}
		}
}
<% else %>
var nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
var vEntryName = vArrayStr.split(',');
vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
var vEntryValue = vArrayStr.split(',');

//sort
vEntryName = SortUtil.UpdateWanIFName( vEntryName, nEntryNum );
vEntryValue = SortUtil.SortMyArray( vEntryValue );		

var slIFNnme = "<% tcWebApi_get("Timezone_Entry","AddRoute","s") %>";			
function listCreate(l_flag)
{
		for (i = 0; i < nEntryNum; i++)
		{
			if(vEntryName[i].indexOf('_R_') > 0){
				if(slIFNnme == vEntryValue[i])
				{
					document.write('<option value=' + vEntryValue[i] + ' selected>'	+ vEntryName[i] + '</option>');
				}
				else
				{
					document.write('<option value=' + vEntryValue[i] + '>'	+ vEntryName[i] + '</option>');
				}
			}
		}
}	
<%end if%>
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 class=Item_L1 height=30>
            <P>ʱ�������</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#ʱ�����" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>�� </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <FORM name="ConfigForm" action="/cgi-bin/net-time.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                        <TD> <P>
                        		<% if tcWebApi_get("WebCustom_Entry","isCUCTimeDispSupported", "h") = "Yes" then %>
                        	  <table cellspacing=0 cellpadding=0 border=0>
                              <tr> 
                                <td width=110>��ǰʱ�䣺</td>
                                <td width=218 colSpan=2><% tcWebApi_get("Timezone_Entry","CURTIME", "s") %></td>
                              </tr>  
                            </table>
                            <%end if%>
                        	  <BR>
                            <INPUT onclick=ntpEnblChange() type=checkbox name=ntpEnabled <%if TCWebApi_get("Timezone_Entry","TYPE", "h") = "0" then asp_Write("checked") end if%>>
                            �Զ�ͬ������ʱ������� 
                            <input type="hidden" name="autotimeFlag" value="0">
                            <BR>
                            <input type="hidden" name="CountryName" value="<% tcWebApi_get("Timezone_Entry","TZ", "s") %>">
                            <input type="hidden" name="ntpServerOther1Flag" value="<% tcWebApi_get("Timezone_Entry","SERVER", "s") %>">
                            <input type="hidden" name="ntpServerOther2Flag" value="<% tcWebApi_get("Timezone_Entry","SERVER2", "s") %>">
                            <INPUT TYPE="HIDDEN" NAME="isNtpServerTypeSupported" value="<% if tcWebApi_get("WebCustom_Entry","isNtpServerTypeSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
                            <DIV id=ntpConfig> <BR>
                          <table cellspacing=0 cellpadding=0 border=0>
                            <tbody>
                              <tr> 
                                <td width=110>ϵͳ����ʱ�䣺</td>
                                <td width=218 colSpan=2>
				                          <SCRIPT language=javascript>
				                          var currTime = '<%tcWebApi_get("DeviceInfo","cur_time2","s")%>';
																  var curTimelist = currTime.split('/');
																  if ( 5 == curTimelist.length )
																  {
																  	document.write(curTimelist[2] + '��' + curTimelist[0] + '��' + curTimelist[1] + '��' + curTimelist[3] + 'ʱ' + curTimelist[4] + '��');
																  }
																  else
																  	document.write('N/A');
																  </SCRIPT>
	                        			</td>
	                              </tr>
                              <tr> 
                                <td width=110>�ӿ�ѡ��</td>
                                <td width=218 colSpan=2><SELECT id="AddRouteTimeZone" 
                        size="1" name="AddRouteTimeZone" > 
                          <SCRIPT language=javascript>
						  listCreate(0);
						  </SCRIPT>
                        </SELECT> </td>
                              </tr>                              
                              <tr> 
                                <td width=110>��SNTP������:</td>
                                <td width=78><select 
                        onChange=ntpChange(this.form.ntpServer1,this.form.ntpServerOther1)  size=1 name=ntpServer1>
                                    <script language=javascript>
						  writeNtpList(true);
						</script>
                                  </select> </td>
                                <td width=140><input maxlength=63 name=ntpServerOther1 value="<%if tcWebApi_get("Timezone_Entry","SERVER","h") <> "N/A" then tcWebApi_get("Timezone_Entry","SERVER","s") end if%>"></td>
                              </tr>
							  
                              <tr>
                                <td>��SNTP������:</td>
                                <td><select 
                        onChange=ntpChange(this.form.ntpServer2,this.form.ntpServerOther2) 
                        size=1 name=ntpServer2>
                                    <script language=javascript>
						writeNtpList(false);
					</script>
                                  </select> </td>
                                <td><input maxlength=63 name=ntpServerOther2 value="<%if tcWebApi_get("Timezone_Entry","SERVER2","h") <> "N/A" then tcWebApi_get("Timezone_Entry","SERVER2","s") end if%>"></td>
                              </tr>
                            </tbody>
                          </table>
                          <P>&nbsp; 
                          <BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>ʱ������:</TD></TR>
                    <TR></TR>
                    <TR>
                      <TD colSpan=2><SELECT size=1 name="cboTimeZone"> 
                     <SCRIPT language=javascript>
					 writeTimeZoneList();
					</SCRIPT>
                        </SELECT> </TD></TR></TBODY></TABLE>
                            <BR>
                            <BR></DIV>
                  <CENTER></CENTER>
                  <P></P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD><input type="hidden" name="SaveFlag" value="0"></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>��</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>��</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2>
            <P align=center><IMG id=btnOK onclick=SubmitForm() height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" width=80 border=0> </P></TD>
          <TD width=170 bgColor=#313031 class=CucBgColorTD_2></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
</BODY></HTML>
