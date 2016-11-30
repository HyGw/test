#!/bin/sh

echo "ddns running"
DDNS_CONF=/etc/ddns.conf
if [ -f $DDNS_CONF ] ; then
	chmod 777 $DDNS_CONF
	. $DDNS_CONF
else
	exit 0
fi
			
if [ $Active = "No" ] ; then
	exit 0
fi

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
  . $PROFILE_CFG
fi

WANIF=none
ACTIVE_INT=none

if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
	pvc_num="$1"
else
	pvc_num="0 1 2 3 4 5 6 7"
fi

EXTEND="0"
EXIT="0"


for j in $pvc_num
do
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
		if [ "$j" = "8" ] || [ "$j" = "9" ] || [ "$j" = "10" ] ; then
			service_num="0 1 2 3 4 5 6 7"		
		else
			service_num="0"
		fi	
	else
		service_num="0"
	fi	

	for k in $service_num
	do
		if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
			if [ "$j" = "8" ] || [ "$j" = "9" ] || [ "$j" = "10" ] ; then
				pppNum="$j""$k"
				nasNum="$j"_"$k"
				ispNum="$j"_"$k"
				EXTEND="1"
			else
				EXTEND="0"
			fi	
		fi

		if [ "$EXTEND" = "1" ] ; then
			CONFFILE=/etc/isp$ispNum.conf
		else
			CONFFILE=/etc/isp$j.conf
		fi

		if [ -f $CONFFILE ] ; then
			chmod 777 $CONFFILE
			. $CONFFILE
			if [ $Active = "Yes" ] ; then
				if [ $DEFAULTROUTE = "Yes" ] ; then
					if [ $ISP = "0" ] || [ $ISP = "1" ] ; then
						if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$EXTEND" = "1" ] ; then
							WANIF=nas$nasNum					
						else
							WANIF=nas$j					
						fi	
					elif [ "$TCSUPPORT_C1_NEW_GUI" != "" ] ; then
						if [ $ISP = "2" ] || [ $ISP = "3" ] ; then
							if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$EXTEND" = "1" ] ; then
					 	 		WANIF=ppp$pppNum
							else	
					 	 		WANIF=ppp$j
							fi
						fi	
					else
						if [ $ISP = "2" ] ; then
							if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$EXTEND" = "1" ] ; then
								WANIF=ppp$pppNum
							else
								WANIF=ppp$j
							fi	
						fi
					fi
					
					if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
						EXIT="1"
					fi
					break
				elif [ $ACTIVE_INT = "none" ] ; then
					if [ $ISP = "0" ] || [ $ISP = "1" ] ; then
						WANIF=nas$j
						ACTIVE_INT=$j	
					elif [ "$TCSUPPORT_C1_NEW_GUI" != "" ] ; then	
						if [ $ISP = "2" ] || [ $ISP = "3" ] ; then
							WANIF=ppp$j
							ACTIVE_INT=$j
						fi
					else	
						if [ $ISP = "2" ] ; then
							WANIF=ppp$j
							ACTIVE_INT=$j
						fi
					fi
				fi
			fi
		fi
	done

	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
		if [ "$EXIT" = "1" ] ; then
			break
		fi	
	fi
done

if [ $WANIF = "none" ] ; then
	echo "no active PVC"
	exit 0
fi		

IPUPDATE_PID=/var/log/ez-ipupdate.pid
IPUPDATE_CONF=/etc/ipupdate.conf

# ifconfig $WANIF | sed -ne 's/ *inet addr:\([0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\) *.*/\1/p' > /tmp/ip.tmp
# IP_USED=`cat /tmp/ip.tmp`

/userfs/bin/ez-ipupdate -c $IPUPDATE_CONF -i $WANIF -d -F $IPUPDATE_PID -P 60 -p 1
# echo $$ >$IPUPDATE_PID

#while true
#do
#	sleep 1
#	ifconfig $WANIF | sed -ne 's/ *inet addr:\([0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\) *.*/\1/p' > /tmp/ip.tmp
# 	IP_NEW=`cat /tmp/ip.tmp`
# 
# 	if [ "$IP_NEW" = "$IP_USED" ] ; then
# 			sleep 10
# 	else
# 			/userfs/bin/ez-ipupdate -c $IPUPDATE_CONF -i $WANIF &
# 			echo $$ >$IPUPDATE_PID
# 			IP_USED="$IP_NEW"
# 	fi
# done


