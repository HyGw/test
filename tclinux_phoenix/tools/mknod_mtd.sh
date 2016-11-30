#!/bin/sh

if [ $# != 2 ] ; then
	echo "usage: $0 [num] [filesystem_dir]"
	exit 0
fi
i=0
j=0 
while [ $i -lt $1 ]
do
	j=`expr $i \* 2`
	mknod $2/dev/mtd$i c 90 $j
	mknod $2/dev/mtdblock$i b 31 $i
 	i=`expr $i + 1`
done
exit 0