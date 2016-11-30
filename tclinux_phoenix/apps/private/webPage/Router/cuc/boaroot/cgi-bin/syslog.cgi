#!/bin/sh

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
</script>
<body><table width=\"580\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">"

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

echo -e "<tr><td><TEXTAREA NAME=\"AlphaLogDisplay\" ROWS=\"33\" COLS=\"63\" WRAP=\"OFF\" EDIT=\"OFF\" READONLY>"

echo -e "Manufacture: $Manufacture"
echo -e "ProductClass:$ProductClass"
echo -e "SerialNumber:$SerialNumber"
echo -e "IP:          $IP"
echo -e "HWVer:       $HWVer"
echo -e "SWVer:       $SWVer"
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

