#!/bin/sh
echo -e "Content-Type: text/html \n"

node_name="WebCustom_Entry"
attr_name="querystring"
querystring=`/userfs/bin/tcapi get $node_name $attr_name`

if [ "$querystring" != "" ]
then
	node_name="Account_Entry0"
	attr_name="username"
	username_s=`/userfs/bin/tcapi get $node_name $attr_name`
	attr_name="web_passwd"
	passwd_s=`/userfs/bin/tcapi get $node_name $attr_name`
	
	username=`echo $querystring | cut -d \& -f 1`
	passwd=`echo $querystring | cut -d \& -f 2`
	sn=`echo $querystring | cut -d \& -f 3`
	username=`echo $username | cut -d = -f 2`
	passwd=`echo $passwd | cut -d = -f 2`
	telnet=`echo $sn | cut -d = -f 2`
	
	if [ "$username" = "$username_s" ] && [ "$passwd" = "$passwd_s" ]
	then
		if [ "$telnet" = "Yes" ] 
		then
			ret=`/userfs/bin/tcapi set Account_TelnetEntry Active $telnet`
			ret=`/userfs/bin/tcapi commit Account_TelnetEntry`
			ret=`/userfs/bin/tcapi save`
			echo "telnet open."
		else
			ret=`/userfs/bin/tcapi set Account_TelnetEntry Active $telnet`
			ret=`/userfs/bin/tcapi commit Account_TelnetEntry`
			ret=`/userfs/bin/tcapi save`
			echo "telnet close."
		fi
	else
		echo "username/passwd match fail."
	fi
else
	echo "no query string."
fi