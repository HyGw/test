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
	sn=`echo $sn | cut -d = -f 2`
	
	if [ "$username" = "$username_s" ] && [ "$passwd" = "$passwd_s" ]
	then
		ret=`/userfs/bin/tcapi set EPON_LOIDAuth LOID0 $sn`
		ret=`/userfs/bin/tcapi commit EPON_LOIDAuth`
		ret=`/userfs/bin/tcapi save`
		echo "set OK."
	else
		echo "username/passwd match fail."
	fi
else
	echo "no query string."
fi