#!/bin/sh

rootpath="/tmp/mnt"
sleeptime=10

restore_allow=`/userfs/bin/tcapi get usbRestore_Entry autorestore`
if [ ${restore_allow} != "1" ]
then
	rm -f /tmp/var/romfile.cfg~~
	exit 0
fi

sleep $sleeptime

target="/tmp/var/romfile.cfg~~"
#mountname=`/userfs/bin/tcapi get usbRestore_Entry dev`
sourcefilename=`/userfs/bin/tcapi get usbRestore_Entry target`

	for file in `ls ${rootpath}`
	do
		if [ -d ${rootpath}"/"${file} ] 
		then
			
			local source="${rootpath}/${file}/${sourcefilename}"
			if [ -f ${source} ]
			then
				/usr/bin/backuprestorecmd -r -${source} -${target}
				if [ -f "/tmp/restore_success"]
				then
					rm -f /tmp/restore_success
				exit 0
				fi
			fi
		fi
	done

rm -f /tmp/var/romfile.cfg~~
