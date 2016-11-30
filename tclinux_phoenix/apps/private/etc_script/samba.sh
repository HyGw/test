#!/bin/sh

SAMBA_FILE=/etc/samba/smb.conf

if [ ! -n "$3" ]; then
	echo "insufficient arguments!"
	echo "Usage: $0 <netbios_name> <workgroup> <guest>"
	echo "Example: $0 RT2880 Ralink admin"
	exit 0
fi

NETBIOS_NAME="$1"
WORKGROUP="$2"
GUEST="$3"

echo "[global]
netbios name = $NETBIOS_NAME
server string = Samba Server
workgroup = $WORKGROUP
security = user
guest account = $GUEST
log file = /var/log.samba
socket options = SO_RCVBUF=32768 SO_SNDBUF=16384
encrypt passwords = yes
use spne go = no
client use spnego = no
disable spoolss = yes
smb passwd file = /etc/samba/smbpasswd
host msdfs = no
strict allocate = No
os level = 20
log level = 1
max log size = 50
null passwords = yes
mangling method = hash
dos charset = UTF8
unix charset = UTF8
display charset = UTF8" > $SAMBA_FILE
