<%
If Request_Form("SaveFlag") = "1" then
	TCWebApi_set("Timezone_Entry","TZ","cboTimeZone")
	TCWebApi_set("Timezone_Entry","TYPE","autotimeFlag")
	TCWebApi_set("Timezone_Entry","SERVER","ntpServerOther1Flag")
	TCWebApi_set("Timezone_Entry","SERVER2","ntpServerOther2Flag")
	TCWebApi_commit("Timezone_Entry")
	TCWebApi_save()
End if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>时间管理</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

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
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->时间管理" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33>
            <P class=GatewayType align=center>型号:
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
timeZones[0] = '(GMT-12:00) 国际日期变更线(西)';
timeZones[1] = '(GMT-11:00) 中途岛，萨摩亚';
timeZones[2] = '(GMT-10:00) 夏威夷';
timeZones[3] = '(GMT-09:00) 阿拉斯加';
timeZones[4] = '(GMT-08:00) 美国西部标准时间，提华纳';
timeZones[5] = '(GMT-07:00) 亚利桑那州，马萨特兰';
timeZones[6] = '(GMT-07:00) 奇瓦瓦，拉巴斯';
timeZones[7] = '(GMT-07:00) 山地时区(加拿大)';
timeZones[8] = '(GMT-06:00) 中美洲';
timeZones[9] = '(GMT-06:00) 中央标准时间(用于美国和加拿大中部)';
timeZones[10] = '(GMT-06:00) 瓜达拉哈拉，墨西哥城，蒙特雷';
timeZones[11] = '(GMT-06:00) 萨斯喀彻温省';
timeZones[12] = '(GMT-05:00) 波哥大，利马，基多';
timeZones[13] = '(GMT-05:00) 东部时间';
timeZones[14] = '(GMT-05:00) 印第安那州';
timeZones[15] = '(GMT-04:00) 大西洋时间';
timeZones[16] = '(GMT-04:00) 加拉加斯，拉巴斯';
timeZones[17] = '(GMT-04:00) 圣地亚哥';
timeZones[18] = '(GMT-03:30) 纽芬兰';
timeZones[19] = '(GMT-03:00) 巴西利亚';
timeZones[20] = '(GMT-03:00) 布宜诺斯艾利斯，乔治敦';
timeZones[21] = '(GMT-03:00) 格陵兰';
timeZones[22] = '(GMT-02:00) 中大西洋地区';
timeZones[23] = '(GMT-01:00) 亚述尔群岛';
timeZones[24] = '(GMT-01:00) 佛得角群岛';
timeZones[25] = '(GMT) 卡萨布兰卡，蒙罗维亚';
timeZones[26] = '(GMT) 格林尼治标准时间：都柏林，爱丁堡，里斯本,伦敦';
timeZones[27] = '(GMT+01:00) 阿姆斯特丹，柏林，伯尔尼, 罗马，斯德哥尔摩，维也纳';
timeZones[28] = '(GMT+01:00) 贝尔格莱德，布拉迪斯拉发，卢布尔雅那，布拉格';
timeZones[29] = '(GMT+01:00) 布鲁塞尔，哥本哈根，马德里，巴黎';
timeZones[30] = '(GMT+01:00) 萨拉热窝，斯科普里，华沙，萨格勒布';
timeZones[31] = '(GMT+01:00) 中西非';
timeZones[32] = '(GMT+02:00) 雅典，伊斯坦布尔，明斯克';
timeZones[33] = '(GMT+02:00) 布加勒斯特';
timeZones[34] = '(GMT+02:00) 开罗';
timeZones[35] = '(GMT+02:00) 哈拉雷，比勒陀利亚';
timeZones[36] = '(GMT+02:00) 赫尔辛基，基辅，里加，索非亚，塔林, 维尔纽斯';
timeZones[37] = '(GMT+02:00) 耶路撒冷';
timeZones[38] = '(GMT+03:00) 巴格达';
timeZones[39] = '(GMT+03:00) 科威特，利雅得';
timeZones[40] = '(GMT+03:00) 莫斯科，圣彼得堡，伏尔加格勒';
timeZones[41] = '(GMT+03:00) 奈洛比';
timeZones[42] = '(GMT+03:30) 德黑兰';
timeZones[43] = '(GMT+04:00) 阿布扎比，马斯喀特';
timeZones[44] = '(GMT+04:00) 巴库，第比利斯，耶烈万';
timeZones[45] = '(GMT+04:30) 喀布尔';
timeZones[46] = '(GMT+05:00) 叶卡特琳堡';
timeZones[47] = '(GMT+05:00) 伊斯兰堡，卡拉奇，塔什干';
timeZones[48] = '(GMT+05:30) 清奈，加尔各答，孟买，新德里';
timeZones[49] = '(GMT+05:45) 加德满都';
timeZones[50] = '(GMT+06:00) 阿拉木图，新西伯利亚';
timeZones[51] = '(GMT+06:00) 阿斯坦纳，达卡';
timeZones[52] = '(GMT+06:00) 斯里兰卡';
timeZones[53] = '(GMT+06:30) 仰光';
timeZones[54] = '(GMT+07:00) 曼谷，河内，雅加达';
timeZones[55] = '(GMT+07:00) 拉斯诺亚尔斯克';
timeZones[56] = '(GMT+08:00) 北京，重庆，香港，乌鲁木齐';
timeZones[57] = '(GMT+08:00) 伊尔库次克，乌兰巴托';
timeZones[58] = '(GMT+08:00) 吉隆坡，新加坡';
timeZones[59] = '(GMT+08:00) 珀斯';
timeZones[60] = '(GMT+08:00) 台北';
timeZones[61] = '(GMT+09:00) 大阪，札幌，东京';
timeZones[62] = '(GMT+09:00) 首尔';
timeZones[63] = '(GMT+09:00) 雅库茨克';
timeZones[64] = '(GMT+09:30) 阿德莱德';
timeZones[65] = '(GMT+09:30) 达尔文';
timeZones[66] = '(GMT+10:00) 布里斯班';
timeZones[67] = '(GMT+10:00) 堪培拉，墨尔本，悉尼';
timeZones[68] = '(GMT+10:00) 关岛，莫尔兹比港';
timeZones[69] = '(GMT+10:00) 霍巴特';
timeZones[70] = '(GMT+10:00) 符拉迪沃斯托克';
timeZones[71] = '(GMT+11:00) 马加丹';
timeZones[72] = '(GMT+11:00) 所罗门群岛，新喀里多尼亚';
timeZones[73] = '(GMT+12:00) 奥克兰，惠灵顿';
timeZones[74] = '(GMT+12:00) 斐济，堪察加半岛，马绍尔群岛';
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
		else document.ConfigForm.ntpServer2.selectedIndex =  0;
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
/*setCheck('ntpEnabled',ntp_enabled);
if( ntp1.length )
{
for( i = 0; i < ntpServers.length; i++ )
{
if( ntp1 == ntpServers[i] )
{
ntpServer1.selectedIndex = i;
break;
}
}
if( i == ntpServers.length )
{
ntpServer1.selectedIndex = i; 
ntpServerOther1.value = ntp1;
}
}
ntpChange(ntpServer1,ntpServerOther1);
if( ntp2.length )
{
for( i = 0; i < ntpServers.length; i++ )
{
if( ntp2 == ntpServers[i] )
{
ntpServer2.selectedIndex = i+1;
break;
}
}
if( i == ntpServers.length )
{
ntpServer2.selectedIndex = i+1; 
ntpServerOther2.value = ntp2;
}
}
ntpChange(ntpServer2,ntpServerOther2);
cboTimeZone.selectedIndex = getTimeZoneIndex(tz_name);*/
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
//var Form = new webSubmitForm();
//Form.usingPrefix('x');

with( document.forms[0] )
{
	if( ntpEnabled.checked )
	{
//Form.addParameter('X_ATP_SNTPEnable',1);
	if( ntpServer1.selectedIndex == ntpServers.length )
	{
		if( ntpServerOther1.value.length == 0 )
		{ 
			alert('主SNTP服务器选择"Other"时,后面的输入框不能为空。');
			document.ConfigForm.SaveFlag.value = 0;
			return;
		}
		if(isIpv6Address(ntpServerOther1.value))
		{
			if(isUnicastIpv6Address(ntpServerOther1.value) == false)
			{
					alert('主服务器地址非法');
					document.ConfigForm.SaveFlag.value = 0;
					return;
			}
		}
		else if (isIpFormat(ntpServerOther1.value))
		{
			if (isValidIpAddress(ntpServerOther1.value) == false)
			{
				alert('主服务器地址非法');
				document.ConfigForm.SaveFlag.value = 0;
				return;
			}
		}
		else
		{
				if(isTValidName(ntpServerOther1.value) == false)
				{
						alert('主服务器地址非法');
						document.ConfigForm.SaveFlag.value = 0;
						return;
				}
		}
//Form.addParameter('NTPServer1',ntpServerOther1.value)
	}
	else
	{
//Form.addParameter('NTPServer1',ntpServer1[ntpServer1.selectedIndex].value);
	}
	if( ntpServer2.selectedIndex == ntpServers.length+1 )
	{
		if( ntpServerOther2.value.length == 0 )
		{ 
			alert('从SNTP服务器选择"Other"时,后面的输入框不能为空。');
			document.ConfigForm.SaveFlag.value = 0;
			return;
		}
		if(isIpv6Address(ntpServerOther2.value))
		{
				if(isUnicastIpv6Address(ntpServerOther2.value) == false)
				{
					alert('从服务器地址非法');
					document.ConfigForm.SaveFlag.value = 0;
					return;
				}
			}
		else if (isIpFormat(ntpServerOther2.value))
		{
			if (isValidIpAddress(ntpServerOther2.value) == false)
			{
				alert('从服务器地址非法');
				document.ConfigForm.SaveFlag.value = 0;
				return;
			}
		}
		else
		{
			if(isTValidName(ntpServerOther2.value) == false)
			{
				alert('从服务器地址非法');
				document.ConfigForm.SaveFlag.value = 0;
				return;
			}
		}
//Form.addParameter('NTPServer2',ntpServerOther2.value);
	}
else
{
if( ntpServer2.selectedIndex > 0)
{
//Form.addParameter('NTPServer2',ntpServer2[ntpServer2.selectedIndex].value);
}
else
{
//Form.addParameter('NTPServer2','');
}
}
//Form.addParameter('LocalTimeZoneName',getTimeZoneName(cboTimeZone.selectedIndex));
}
else
{
//Form.addParameter('X_ATP_SNTPEnable',0);
}
}
//Form.setAction('setcfg.cgi?x=InternetGatewayDevice.Time&RequestFile=html/network/sntp.asp');
//Form.submit();
checkntpServrState();
document.ConfigForm.SaveFlag.value = "1";
document.ConfigForm.submit();

}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>时间服务器</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#时间管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name="ConfigForm" action="/cgi-bin/net-time.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                        <TD> <P><BR>
                            <INPUT onclick=ntpEnblChange() type=checkbox name=ntpEnabled <%if TCWebApi_get("Timezone_Entry","TYPE", "h") = "0" then asp_Write("checked") end if%>>
                            自动同步网络时间服务器 
                            <input type="hidden" name="autotimeFlag" value="0">
                            <BR>
                            <input type="hidden" name="CountryName" value="<% tcWebApi_get("Timezone_Entry","TZ", "s") %>">
                            <input type="hidden" name="ntpServerOther1Flag" value="<% tcWebApi_get("Timezone_Entry","SERVER", "s") %>">
                            <input type="hidden" name="ntpServerOther2Flag" value="<% tcWebApi_get("Timezone_Entry","SERVER2", "s") %>">
                            <BR>
                          <table cellspacing=0 cellpadding=0 border=0>
                            <tbody>
                              <tr> 
                                <td width=110>主SNTP服务器:</td>
                                <td width=78><select 
                        onChange=ntpChange(this.form.ntpServer1,this.form.ntpServerOther1)  size=1 name=ntpServer1>
                                    <script language=javascript>
						  writeNtpList(true);
						</script>
                                  </select> </td>
                                <td width=140><input maxlength=63 name=ntpServerOther1 value="<%if tcWebApi_get("Timezone_Entry","SERVER","h") <> "N/A" then tcWebApi_get("Timezone_Entry","SERVER","s") end if%>"></td>
                              </tr>
							  
                              <tr style="display:none">
                                <td>从SNTP服务器:</td>
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
                          <DIV id=ntpConfig> <BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>时间区域:</TD></TR>
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
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center><IMG id=btnOK onclick=SubmitForm() height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" width=80 border=0> </P></TD>
          <TD width=170 bgColor=#313031></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
</BODY></HTML>
