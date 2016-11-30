#!/bin/sh

if [ $# != 2 ] ; then
	echo "usage: $0 type jvmx"
	exit 0
fi

UPG_TYPE=$1
UPG_CONTENT=$2
# kill jvm
/usr/bin/killall -9 jamvm
if [ "$UPG_TYPE" = "jvm" ];  then
	# kill jvm
	/usr/bin/killall -9 jamvm
	# remove jvm bin lib share
	/bin/rm -rf /usr/osgi/$UPG_CONTENT
	/bin/rm -rf /usr/osgi/bin
	/bin/rm -rf /usr/osgi/lib
	/bin/rm -rf /usr/osgi/share
	# create jvm
	/bin/mkdir -p /usr/osgi/$UPG_CONTENT
	# extract pack
	/bin/tar -zxvf /tmp/phone-temp -C /usr/osgi/$UPG_CONTENT 2>/tmp/tarerr
	# do ln jvm
	cd /usr/osgi && ln -fs ./$UPG_CONTENT/bin/ ./bin && ln -fs ./$UPG_CONTENT/lib/ ./lib && ln -fs ./$UPG_CONTENT/share/ ./share
	# start osgi
	cd /usr/osgi/felix-framework && /usr/osgi/bin/jamvm -jar ./bin/felix.jar &
elif [ "$UPG_TYPE" = "restorejvm" ] ; then
	# kill jvm
	/usr/bin/killall -9 jamvm
	/bin/rm -rf /usr/osgi/bin
	/bin/rm -rf /usr/osgi/lib
	/bin/rm -rf /usr/osgi/share
	cd /usr/osgi && ln -fs ./$UPG_CONTENT/bin/ ./bin && ln -fs ./$UPG_CONTENT/lib/ ./lib && ln -fs ./$UPG_CONTENT/share/ ./share
elif [ "$UPG_TYPE" = "framework" ] ; then
	# kill jvm
	/usr/bin/killall -9 jamvm
	# remove framework ln
	/bin/rm -rf /usr/osgi/$UPG_CONTENT
	/bin/rm -rf /usr/osgi/felix-framework
	# create framework
	/bin/mkdir -p /usr/osgi/$UPG_CONTENT
	# extract pack
	/bin/tar -zxvf /tmp/phone-temp -C /usr/osgi/$UPG_CONTENT 2>/tmp/tarerr
	# do ln jvm
	cd /usr/osgi && ln -fs ./$UPG_CONTENT/ ./felix-framework
	# start osgi
	cd /usr/osgi/felix-framework && /usr/osgi/bin/jamvm -jar ./bin/felix.jar &
elif [ "$UPG_TYPE" = "restoreframework" ] ; then
	# kill jvm
	/usr/bin/killall -9 jamvm
	/bin/rm -rf /usr/osgi/felix-framework
	cd /usr/osgi && ln -fs ./$UPG_CONTENT/ ./felix-framework
elif [ "$UPG_TYPE" = "ctgw" ] ; then
	# remove ctgw lib ln
	/bin/rm -rf /usr/osgi/$UPG_CONTENT
	/bin/rm -rf /usr/osgi/ctgwlib
	# create ctgwlibs
	/bin/mkdir -p /usr/osgi/$UPG_CONTENT
	# extract pack
	/bin/tar -zxvf /tmp/phone-temp -C /usr/osgi/$UPG_CONTENT 2>/tmp/tarerr
	# do ln ctgw lib
	cd /usr/osgi && ln -fs ./$UPG_CONTENT/ ./ctgwlib
	# start ctgw test
	/userfs/bin/ctsgw_test > /tmp/ctsgw_test
elif [ "$UPG_TYPE" = "restorectgw" ] ; then
	/bin/rm -rf /usr/osgi/ctgwlib
	cd /usr/osgi && ln -fs ./$UPG_CONTENT/ ./ctgwlib
elif [ "$UPG_TYPE" = "mobilechk" ] ; then
	# remove mobile
	/bin/rm -rf /usr/osgi/$UPG_CONTENT
	# create mobiles
	/bin/mkdir -p /usr/osgi/$UPG_CONTENT
	# extract pack
	/bin/tar -zxvf /tmp/phone-temp -C /usr/osgi/$UPG_CONTENT 2>/tmp/tarerr
	# start mobile test
	cd /usr/osgi/$UPG_CONTENT && /usr/osgi/$UPG_CONTENT/mobile-manager -upgtest
elif [ "$UPG_TYPE" = "mobileln" ] ; then
	# kill mobile-manager
	/usr/bin/killall -9 mobile-manager
	# remove mobile ln
	/bin/rm -rf /usr/osgi/mobile
	# do ln mobile
	cd /usr/osgi && ln -fs ./$UPG_CONTENT/ ./mobile
elif [ "$UPG_TYPE" = "remove" ] ; then
	# remove useless files
	/bin/rm -rf /usr/osgi/$UPG_CONTENT
fi


