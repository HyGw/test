#!/bin/sh

txpwr=`/userfs/bin/tcapi get Info_PonPhy TxPower`
rxpwr=`/userfs/bin/tcapi get Info_PonPhy RxPower`

echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type
content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html;
charset=gb2312\"><LINK href=\"/JS/stylemain.css\" type=\"text/css\"
rel=\"stylesheet\"></head><script language=\"JavaScript\"> 
var model;
function levelChange(model)
{
	parent.document.ConfigForm.frameloglevel.value=model.value;
	parent.document.ConfigForm.submit();
}
SendPower = (Math.round(Math.log((Number($txpwr))/10000)/(Math.log(10))*100)/10);
RecvPower = (Math.round(Math.log((Number($rxpwr))/10000)/(Math.log(10))*100)/10);

</script>
<body><table width=\"100%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">"

loglevel=`/userfs/bin/tcapi get SysLog_Entry DisplayLevel`
if [ "$loglevel" = "no attribute information" -o "$loglevel" = "no node information" ]
then
	loglevel="7"
fi

Manufacture=`/userfs/bin/tcapi get DeviceInfo_devParaStatic Manufacturer`
if [ "$Manufacture" = "no attribute information" -o "$Manufacture" = "no node information" ]
then
	Manufacture="N/A"
fi

ProductClass=`/userfs/bin/tcapi get DeviceInfo_devParaDynamic ProductClass`
if [ "$ProductClass" = "no attribute information" -o "$ProductClass" = "no node information" ]
then
	ProductClass="N/A"
fi

SerialNumber=`/userfs/bin/tcapi get DeviceInfo_devParaDynamic SerialNum`
if [ "$SerialNumber" = "no attribute information" -o "$SerialNumber" = "no node information" ]
then
	SerialNumber="N/A"
fi

IP=`/userfs/bin/tcapi get Lan_Entry0 IP`
if [ "$IP" = "no attribute information" -o "$IP" = "no node information" ]
then
	IP="N/A"
fi

HWVer=`/userfs/bin/tcapi get DeviceInfo_devParaStatic CustomerHWVersion`
if [ "$HWVer" = "no attribute information" -o "$HWVer" = "no node information" ]
then
	HWVer="N/A"
fi

SWVer=`/userfs/bin/tcapi get DeviceInfo_devParaStatic CustomerSWVersion`
if [ "$SWVer" = "no attribute information" -o "$SWVer" = "no node information" ]
then
	SWVer="N/A"
fi

XponMode=`/userfs/bin/tcapi get XPON_Common xponMode`
if [ "$XponMode" = "no attribute information" -o "$XponMode" = "no node information" ]
then
	XponMode="N/A"
fi

MAC=`/userfs/bin/tcapi get Info_Ether mac`
if [ "$MAC" = "no attribute information" -o "$MAC" = "no node information" ]
then
	MAC="N/A"
fi

OUI=`/userfs/bin/tcapi get DeviceInfo_devParaDynamic ManufacturerOUI`
if [ "$OUI" = "no attribute information" -o "$OUI" = "no node information" ]
then
	OUI="N/A"
fi

webpwd=`/userfs/bin/tcapi get Account_Entry1 web_passwd`
if [ "$webpwd" = "no attribute information" -o "$webpwd" = "no node information" ]
then
	webpwd="N/A"
fi

xponsn=`/userfs/bin/tcapi get GPON_ONU SerialNumber`
if [ "$xponsn" = "no attribute information" -o "$xponsn" = "no node information" ]
then
	xponsn="N/A"
fi

echo -e "<tr rowspan=\"2\"><td>显示级别:<select name=\"levels\" onChange=\"levelChange(this)\">
<option value=\"0\""
	case "$loglevel" in
	"0" )
	echo -e "selected";;
	esac
echo -e ">紧急</option>"
echo -e "<option value=\"1\""
	case "$loglevel" in
	"1" )
	echo -e "selected";;
	esac
echo -e ">警报</option>"
echo -e "<option value=\"2\""
	case "$loglevel" in
	"2" )
	echo -e "selected";;
	esac
echo -e ">重要</option>"
echo -e "<option value=\"3\""
	case "$loglevel" in
	"3" )
	echo -e "selected";;
	esac
echo -e ">错误</option>"
echo -e "<option value=\"4\""
	case "$loglevel" in
	"4" )
	echo -e "selected";;
	esac
echo -e ">警告</option>"
echo -e "<option value=\"5\""
	case "$loglevel" in
	"5" )
	echo -e "selected";;
	esac
echo -e ">注意</option>"
echo -e "<option value=\"6\""
	case "$loglevel" in
	"6" )
	echo -e "selected";;
	esac
echo -e ">通知</option>"
echo -e "<option value=\"7\""
	case "$loglevel" in
	"7" )
	echo -e "selected";;
	esac
echo -e ">调试</option></select></td></tr>"

echo -e "<tr rowspan=\"1\"><td>&nbsp;</td></tr>"

echo -e "<TR><TD>发送光功率:<SCRIPT language=JavaScript type=text/javascript>document.write(SendPower);</SCRIPT>dBm&nbsp;</TD></TR>"
echo -e "<TR><TD>接收光功率:<SCRIPT language=JavaScript type=text/javascript>document.write(RecvPower);</SCRIPT>dBm&nbsp;</TD></TR>"

echo -e "<tr><td><TEXTAREA NAME=\"AlphaLogDisplay\" STYLE=\"WIDTH:458px;HEIGHT:521px;\" WRAP=\"ON\" EDIT=\"OFF\" READONLY>"

echo -e "Manufacture: $Manufacture"
echo -e "ProductClass:$ProductClass"
echo -e "SerialNumber:$SerialNumber"
echo -e "IP:          $IP"
echo -e "HWVer:       $HWVer"
echo -e "SWVer:       $SWVer"
echo -e "XponMode:    $XponMode"
echo -e "MAC:         $MAC"
echo -e "OUI:         $OUI"
echo -e "webpwd:      $webpwd"
echo -e "xponsn:      $xponsn"
echo -e "----------------------------------------------------------"
cat /usr/etc/fwver.conf
cat /proc/version
ret=`/usr/bin/prolinecmd mt7570bob display > /tmp/var/7570.conf`
cat /tmp/var/7570.conf
rm /tmp/var/7570.conf

echo -e " "
	case "$loglevel" in
	"0" )
		LogFileParser 0;;
	esac

	case "$loglevel" in
	"1" )
		LogFileParser 1;;
	esac

	case "$loglevel" in
	"2" )
		LogFileParser 2;;
	esac

	case "$loglevel" in
	"3" )
		LogFileParser 3;;
	esac

	case "$loglevel" in
	"4" )
		LogFileParser 4;;
	esac

	case "$loglevel" in
	"5" )
		LogFileParser 5;;
	esac

	case "$loglevel" in
	"6" )
		LogFileParser 6;;
	esac

	case "$loglevel" in
	"7" )
		LogFileParser 7;;
	esac


echo -e "</TEXTAREA></td></tr>"

echo "</table></body></html>"

