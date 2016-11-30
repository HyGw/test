#!/bin/sh

#set -x
#set +x

set -x
PLUGIN_NAME=$1
ENTRYFILE=$2
PLUGIN_PATH=/usr/osgi/plugin-c/$1
PLUGIN_VAR_PATH=/var/osgi/$1
RIGHT=$3
NEEDSTART=$4

if [ $# = 5 ] && [ $5 != 0 ] ; then
	VMCMD="-v $5"	
else
	VMCMD="-a"
fi

if [ ! -d $PLUGIN_VAR_PATH ]; then		
	mkdir $PLUGIN_VAR_PATH
fi
#chmod 700 $PLUGIN_PATH
#chmod 700 $PLUGIN_PATH/Temp
#chmod 700 $PLUGIN_PATH/Data
chmod 500 $PLUGIN_PATH
chmod 777 $PLUGIN_PATH/Temp
chmod 700 $PLUGIN_PATH/Data

if [ $RIGHT = "root" ]
then
	echo "root"
	if [ $NEEDSTART = 1 ] || [ -f $PLUGIN_PATH/pid ]; then	
		killall $ENTRYFILE
		export LD_LIBRARY_PATH=$PLUGIN_PATH/MyPlugin/Lib && cd $PLUGIN_PATH/MyPlugin/ && ./$ENTRYFILE &
		echo 0$!:$ENTRYFILE  > $PLUGIN_PATH/pid
	fi
else
	USERNAME=${PLUGIN_NAME##*"smartgateway."}
	delgroup $USERNAME
	deluser $USERNAME
	echo "user"
	adduser -h /usr/osgi/plugin-c/$1/ $USERNAME -D -H
	chown -R $USERNAME:$USERNAME $PLUGIN_PATH
	if [ $NEEDSTART = 1 ] || [ -f $PLUGIN_PATH/pid ]; then	
		killall $ENTRYFILE
		su -p - $USERNAME -c "export LD_LIBRARY_PATH=$PLUGIN_PATH/MyPlugin/Lib && cd $PLUGIN_PATH/MyPlugin/ && ulimit $VMCMD && ./$ENTRYFILE & "	
		PID=`pidof -s $ENTRYFILE`
		echo 0$PID:$ENTRYFILE  > $PLUGIN_PATH/pid
	fi
fi


set +x
