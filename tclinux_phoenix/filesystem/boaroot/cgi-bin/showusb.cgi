#!/bin/sh

echo "Content-Type: text/html"
echo ""

mypath="/tmp/mnt"
count=0

showusb()
{
	for i in `ls`
    do
        if [ -d $i -a "$i" != "dev" ]
        then
			echo "usbvalue["$count"]=\""$i"\";"
			count=`expr $count + 1`
        fi
    done
	
}

echo "var usbvalue = new Array();"

if [ -d "$mypath" ]
then
	cd $mypath
	showusb ""
fi
