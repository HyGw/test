#!/bin/sh

set -x

echo $$

PLUGIN_TYPE=$1
PLUGIN_NAME=$2
PLUGIN_VER=$3
PLUGIN_RESET=""
if [ $# = 4 ] ; then
	PLUGIN_RESET=$4
fi
if [ $1 = 0 ]; then
	#bundle
	PLUGIN_TYPE_PATH=plugin-b
	PLUGIN_SUFFIX=jar
elif [ $1 = 5 ]; then
	#plugin c 
	PLUGIN_TYPE_PATH=plugin-c
	PLUGIN_SUFFIX=cpk
else
	echo "other type"
fi

PLUGIN_PATH=/usr/osgi/$PLUGIN_TYPE_PATH/$PLUGIN_NAME
PLUGIN_FILE=/usr/osgi/$PLUGIN_TYPE_PATH/$PLUGIN_NAME/$PLUGIN_NAME.$PLUGIN_SUFFIX

PLUGIN_BACKUP_PATH=/usr/osgi/backup
PLUGIN_VAR_PATH=/var/osgi/$PLUGIN_NAME

if [ ! -d $PLUGIN_PATH ]; then
	mkdir $PLUGIN_PATH
fi

if [ $PLUGIN_TYPE = 0 ]; then #bundle
	echo "plugin b"
	if [ -f $PLUGIN_FILE ]; then #backup jar
		echo "backup jar"
		#mv -f $PLUGIN_FILE $PLUGIN_BACKUP_PATH
	else
		mkdir $PLUGIN_PATH/data
		mkdir $PLUGIN_VAR_PATH	
		ln -fs $PLUGIN_VAR_PATH $PLUGIN_PATH/data/temp
	fi	
	mv -f /tmp/$PLUGIN_NAME.$PLUGIN_SUFFIX $PLUGIN_PATH
	if [ -f $PLUGIN_PATH/Info.plugin ]; then
		mv $PLUGIN_PATH/Info.plugin $PLUGIN_PATH/Info
	fi
	echo "Version:"$PLUGIN_VER > $PLUGIN_PATH/Info.plugin
	# factory install
	if [ "$PLUGIN_RESET" = "Yes" ] ; then
		echo -n "1" > $PLUGIN_PATH/state
	fi
elif [ $PLUGIN_TYPE = 5 ]; then #plugin c
	echo "plugin c"
	if [ -f $PLUGIN_FILE ]; then #backup c
		echo "backup cpk"
		#mv -f $PLUGIN_FILE $PLUGIN_BACKUP_PATH
	else
		mkdir $PLUGIN_PATH/Data
		mkdir $PLUGIN_VAR_PATH	
		ln -fs $PLUGIN_VAR_PATH $PLUGIN_PATH/Temp
	fi	
	#mv -f /tmp/$PLUGIN_NAME.$PLUGIN_SUFFIX $PLUGIN_PATH
	rm -rf $PLUGIN_PATH/Info.plugin
	rm -rf $PLUGIN_PATH/MyPlugin
	#tar -zxv -f $PLUGIN_PATH/$PLUGIN_NAME.$PLUGIN_SUFFIX -C $PLUGIN_PATH
	tar -zxv -f /tmp/$PLUGIN_NAME.$PLUGIN_SUFFIX -C $PLUGIN_PATH
	# factory install
	if [ "$PLUGIN_RESET" = "Yes" ] ; then
		echo "" > $PLUGIN_PATH/pid
	fi
else
	echo "other type"
fi

rm -rf /tmp/$PLUGIN_NAME.*

