#!/bin/sh

#if [ $# = 4 ] ; then
#	echo "usage: $0 [filePath] [fileName] [serverIp] [port]"
#	exit 0
#fi


filePath=$1
fileName=$2
serverIp=$3

if [ $# = 4 ]; then
port=$4
fi

cd $filePath && /usr/bin/tftp -p -r $fileName $serverIp $port

echo "cd $filePath"
echo "/usr/bin/tftp -p -r $fileName $serverIp $port"
echo "tftp.sh end!!!"
