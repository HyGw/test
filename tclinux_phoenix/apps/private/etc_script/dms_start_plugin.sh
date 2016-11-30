#!/bin/sh

DEVICECFG_PATH=$1
DMSCFG_PATH=$2

mount -t vfat /dev/sda1 /usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.dlna/userfs/dlna/media/

export LD_LIBRARY_PATH=/usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.dlna/userfs/dlna/Thirdp/lib:/usr/osgi/plugin-b/com.chinatelecom.econet.smartgateway.dlna/userfs/dlna/lib:$LD_LIBRARY_PATH
cd /usr/osgi/plugin-b/com.chinatelecom.youhua.smartgateway.dlna/userfs/dlna/bin/
./dlna_dmsTcApp $DEVICECFG_PATH $DMSCFG_PATH &
echo $! > /var/run/dms.pid

exit 0
