#!/bin/sh

#set -x
#set +x

set -x
echo delete
PLUGIN_NAME=$1
PLUGIN_TYPE=$2
DEL_TYPE=$3

PLUGIN_VAR_PATH=/var/osgi/$PLUGIN_NAME

if [ $2 = 0 ]; then #bundle
	if [ $DEL_TYPE = 0 ]; then #factory only data and temp
		rm -rf /usr/osgi/plugin-b/$PLUGIN_NAME/data/*	
		rm -rf $PLUGIN_VAR_PATH/*
		ln -fs $PLUGIN_VAR_PATH /usr/osgi/plugin-b/$PLUGIN_NAME/data/temp
	elif [ $DEL_TYPE = 1 ]; then #factory
		rm -rf /usr/osgi/plugin-b/$PLUGIN_NAME/*	
		rm -rf $PLUGIN_VAR_PATH/*
		mkdir /usr/osgi/plugin-b/$PLUGIN_NAME/data
		mkdir $PLUGIN_VAR_PATH	
		ln -fs $PLUGIN_VAR_PATH /usr/osgi/plugin-b/$PLUGIN_NAME/data/temp
		cp -f /usr/osgi/factory/$PLUGIN_NAME.jar /usr/osgi/plugin-b/$PLUGIN_NAME/
		cp -f /usr/osgi/factory/$PLUGIN_NAME.info /usr/osgi/plugin-b/$PLUGIN_NAME/Info.plugin		
	else #uninstall
		rm -rf /usr/osgi/plugin-b/$PLUGIN_NAME	
		rm -rf $PLUGIN_VAR_PATH	
	fi
elif [ $2 = 5 ]; then
	#plugin c
	if [ $DEL_TYPE = 0 ]; then #factory only data and temp
		rm -rf /usr/osgi/plugin-c/$PLUGIN_NAME/Data/*
		rm -rf $PLUGIN_VAR_PATH/*
		ln -fs $PLUGIN_VAR_PATH /usr/osgi/plugin-c/$PLUGIN_NAME/Temp
	elif [ $DEL_TYPE = 1 ]; then #factory
		rm -rf $PLUGIN_VAR_PATH/*
		rm -rf /usr/osgi/plugin-c/$PLUGIN_NAME/*
		mkdir /usr/osgi/plugin-c/$PLUGIN_NAME/Data
		ln -fs $PLUGIN_VAR_PATH /usr/osgi/plugin-c/$PLUGIN_NAME/Temp		
		tar -zxv -f /usr/osgi/factory/$PLUGIN_NAME.cpk -C /usr/osgi/plugin-c/$PLUGIN_NAME
	else #unstall
		while read line 
		do
			RIGHT=`expr "$line" : "Right:\(.*\)"`
			if [ $RIGHT != "root" ]
			then
				USERNAME=${PLUGIN_NAME##*"smartgateway."}
				delgroup $USERNAME
				deluser $USERNAME
			fi
		done < $path/Info.plugin
		rm -rf /usr/osgi/plugin-c/$PLUGIN_NAME
		rm -rf $PLUGIN_VAR_PATH
	fi
else
	echo "other type"
fi

set +x
