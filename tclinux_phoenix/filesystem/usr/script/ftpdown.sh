#!/bin/sh

#usage:
#	ftpdown.sh $1 $2 $3 $4 $5 $6 $7 $8
#		$1: history id, which the result will be writed to. example: 0
#		$2: ftp host. example: www.aaa.com
#		$3: ftp port. example: 21
#		$4: username
#		$5: password
#		$6: remote file path. example: somefold/somefile.avi
#		$7: usb mount fold. example: usb1_1
#		$8: user fold. example: a/b/c (not allow: /a/b/c)
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#	support $9
#		$9: ftp client id, which the result will be also writed to.
#endif/*TCSUPPORT_COMPILE*/
rootpath="/tmp/mnt"
historymaxid=9
ret=0

#usage:
#	find_entry_id $1
#		$1: history id which will be found
#		return: 0 if found;
#				1 if not found
#		echo: Entry No. if found;
#			  "" if not found
find_entry_id()
{
	local i=0
	local temp=0
	while [ $i -le ${historymaxid} ]
	do
		temp=`/userfs/bin/tcapi get appFTP_Entry$i id`
		if [ ${temp} -eq $1 ]
		then
			echo $i
			return 0
		fi
		i=`expr $i + 1`
	done
	return 1
}

#usage:
#	check_download $1
#		$1: the number which will be checked
#		echo: 0 if $1 equal 0
#			  1 if $1 not equal 0
check_download()
{
	local err="${1%:*}"
	case "$err" in
	"")
		echo 0
		;;
	"ftpget: PASS error"|"ftpget: USER error")
		echo 1
		;;
	*)
		echo 99
		;;
	esac
}

if [ -d "${rootpath}/${7}" ]
then
	if [ "${8}" != "" ]
	then	#user save path is not "", create the fold then download
		/bin/mkdir -p "${rootpath}/${7}/${8}"
		if [ $? -eq 0 ]
		then
			local result="`/usr/bin/ftpget -u \"${4}\" -p \"${5}\" -P \"${3}\" \"${2}\" \"${rootpath}/${7}/${8}/${6##*/}\" \"${6}\" 2>&1`"
			ret=`check_download "$result"`
		else
			ret=2   #create fold errorid
		fi
	else	##user save path is "", direct download
		local result="`/usr/bin/ftpget -u \"${4}\" -p \"${5}\" -P \"${3}\" \"${2}\" \"${rootpath}/${7}/${6##*/}\" \"${6}\" 2>&1`"
		ret=`check_download "$result"`
	fi
else
   ret=2
fi

#write the result to the correct Entry
index=`find_entry_id ${1}`
if [ $? -eq 0 ]
then
	/userfs/bin/tcapi set appFTP_Entry${index} result $ret
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CUC_CWMP_PARAMETER)
if [ $# -eq 9 ]
		then
			/userfs/bin/tcapi set appFTP_Entry${9} result $ret
		fi
#endif/*TCSUPPORT_COMPILE*/
fi
return $ret