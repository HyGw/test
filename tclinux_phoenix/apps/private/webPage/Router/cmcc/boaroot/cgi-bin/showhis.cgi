#!/bin/sh

echo "Content-Type: text/html"
echo ""

count=`/userfs/bin/tcapi get appFTP_Downloading historycount`
i=0

echo "var DownEntry=new Array();"
	
if [ "$count" -gt 0 ]
	then
		while [ "$count" -gt "$i" ]
		do
			node_name="appFTP_Entry$i"
			username=`/userfs/bin/tcapi get $node_name username`
			password=`/userfs/bin/tcapi get $node_name password`
			url=`/userfs/bin/tcapi get $node_name url`
			port=`/userfs/bin/tcapi get $node_name port`
			deviceid=`/userfs/bin/tcapi get $node_name deviceid`
			savepath=`/userfs/bin/tcapi get $node_name savepath`
			result=`/userfs/bin/tcapi get $node_name result`

			if [ "$username" = "" -o "$username" = "N/A" ]
			then
				username="&nbsp;"
			fi
		
			if [ "$password" = "" -o "$password" = "N/A" ]
			then
				password="&nbsp;"
			fi
			
			case "$result" in
				0) showresult="Success";;
				1) showresult="Failed:UserName or Password Error";;
				2) showresult="Failed:Create Fold Error";;
				10) showresult="Downloading";;
				*) showresult="Failed:Download Error";;
			esac
			
			if [ "$savepath" != "" ]
			then
				echo "DownEntry[$i]=new Array(\"\", \"\", \"\", \"$username\", \"$password\", \"$port\", \"$url\",\"$deviceid/$savepath/${url##*/}\",\"$showresult\");"
			else
				echo "DownEntry[$i]=new Array(\"\", \"\", \"\", \"$username\", \"$password\", \"$port\", \"$url\",\"$deviceid/${url##*/}\",\"$showresult\");"
			fi
				
			i=`expr $i + 1`
		done		
fi

