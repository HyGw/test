#!/bin/sh

if [ $# != 1 ] ; then
	echo "usage: $0 [mtdblockn]"
	exit 0
fi

mount -t jffs2 -o rw /dev/mtdblock$1 /usr/local/ct
if [ "$?" != "0" ] ;then
echo "erase jffs2 and mount again!"
/userfs/bin/mtd erase jffs2
mount -t jffs2 -o rw /dev/mtdblock$1 /usr/local/ct
else
echo "mount jffs2 success!"
fi
