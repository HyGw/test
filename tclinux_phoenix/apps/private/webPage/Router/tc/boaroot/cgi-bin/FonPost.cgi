#!/bin/sh
echo -e "Content-Type: text/html \n"
echo -e "<html><body>"
type=`echo $QUERY_STRING | cut -d = -f 1`
if [ "$type" = "fonAccept" ]
then
fonAccept=`echo $QUERY_STRING | cut -d = -f 2`
TELEPHONE=`/userfs/bin/tcapi get Wan_PVC0 USERPHONE`
mac=`/userfs/bin/tcapi get Info_Ether mac`
MAC=`echo $mac | sed -e 's/:/-/g'`
ADDITIONAL1=`/userfs/bin/tcapi get Info_proLineInfo FonMac`
if [ "$fonAccept" = "YES" ];
	then
		ADDITIONAL2="S"
else
		ADDITIONAL2="N"
fi
residential=`/userfs/bin/tcapi get GUI_Entry0 OI_USER_ENTERPRISE`
if [ "$residential" = "0" ];
	then
		ADDITIONAL3="RES"	
	else
		ADDITIONAL3="NRES"	
fi
ADDITIONAL4="ZTE"
VALIDATE="040815162342"
postData="phone=$TELEPHONE&macaddress=$MAC&additional_1=$ADDITIONAL1&additional_2=$ADDITIONAL2&additional_3=$ADDITIONAL3&additional_4=$ADDITIONAL4&validate=$VALIDATE"

/userfs/bin/wget -q -O /tmp/result --connect-timeout=5 --timeout=5 --tries=1 --post-data=$postData https://fon.wifioi.com.br/insert.php --no-check-certificate
contentStr=`cat /tmp/result`;
rm /tmp/result
fi
echo -e "<div>$contentStr</div></body></html>"