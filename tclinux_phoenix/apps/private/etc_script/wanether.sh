#!/bin/sh

################################################################################
#   Global Parameter
################################################################################

OrigCpuPortRateLimitPath="/tmp/.orig_cpu_port_rate_limit"
OrigCpuPortRateLimit=0

CurPortMapPath="/proc/tc3162/eth_portmap"

OrigPortMapPath="/tmp/.orig_eth_portmap"
OrigPortMap=-1_-1_-1_-1_-1_-1_-1

NewPortMap=-1_-1_-1_-1_-1_-1_-1

cur_wan=-1
cur_lan1=-1
cur_lan2=-1
cur_lan3=-1
cur_lan4=-1
cur_lan5=-1
cur_lan6=-1

orig_wan=-1
orig_lan1=-1
orig_lan2=-1
orig_lan3=-1
orig_lan4=-1
orig_lan5=-1
orig_lan6=-1

new_wan=-1
new_lan1=-1
new_lan2=-1
new_lan3=-1
new_lan4=-1
new_lan5=-1
new_lan6=-1


################################################################################
#   Function
################################################################################

restore_cpu_port_rate_limit()
{
    if [ -e $OrigCpuPortRateLimitPath ]; then
        OrigCpuPortRateLimit=`cat $OrigCpuPortRateLimitPath`
    else
        echo "Error: The orignal cpu_port_rate_limit is loss"
        exit 6
    fi

    /userfs/bin/ethphxcmd gsww2 0 1640 $OrigCpuPortRateLimit
}

backup_cpu_port_rate_limit()
{
    OrigCpuPortRateLimit=`/userfs/bin/ethphxcmd gswr2 0 1640 | sed 's/:/\ /g' | cut -d " " -f 3`
    echo $OrigCpuPortRateLimit > $OrigCpuPortRateLimitPath
}

parser_cur_port_map()
{
    tmp=`cat $CurPortMapPath`
    count=0
    for i in $tmp; do
        eval \value$count=$i
        count=`expr $count + 1`
    done

    cur_wan=$value0
    cur_lan1=$value2
    cur_lan2=$value4
    cur_lan3=$value6
    cur_lan4=$value8
    cur_lan5=$value10
    cur_lan6=$value12
}

parser_orig_port_map()
{
    if [ "$cur_wan" = "-1" ]; then
        OrigPortMap=$cur_wan\_$cur_lan1\_$cur_lan2\_$cur_lan3\_$cur_lan4\_$cur_lan5\_$cur_lan6
        echo $OrigPortMap > $OrigPortMapPath
    else
        if [ -e $OrigPortMapPath ]; then
            OrigPortMap=`cat $OrigPortMapPath`
        else
            echo "Error: The default portmap is loss"
            exit 5
        fi
    fi

    tmp=`echo $OrigPortMap | sed 's/_/\ /g'`
    count=0
    for i in $tmp; do
        eval \value$count=$i
        count=`expr $count + 1`
    done

    orig_wan=$value0
    orig_lan1=$value1
    orig_lan2=$value2
    orig_lan3=$value3
    orig_lan4=$value4
    orig_lan5=$value5
    orig_lan6=$value6
}

create_new_portmap()
{
    for i in 1 2 3 4 5 6
    do
        eval tmp=\$orig_lan$i
        if [ "$new_wan" = "$tmp" ]; then
            eval \new_lan$i=-1
        elif [ "$cur_wan" = "$tmp" ]; then
            eval \new_lan$i=$cur_wan
        else
            eval \new_lan$i=\$cur_lan$i
        fi
    done

    NewPortMap=$new_wan\_$new_lan1\_$new_lan2\_$new_lan3\_$new_lan4\_$new_lan5\_$new_lan6
}

close_qos()
{
		/userfs/bin/tcapi set QoS_Common Active No
		/userfs/bin/tcapi commit QoS_Common
}

open_qos()
{
		/userfs/bin/tcapi set QoS_Common Active Yes
		/userfs/bin/tcapi commit QoS_Common
}

################################################################################
#   Main
################################################################################

new_wan=$1

if [ -z "$new_wan" ]; then
    echo "Usage:"
    echo "  wanether.sh [wan_port]"
    echo "  The valid value is -1,0~5"
    exit 1
fi

if [ "`expr $new_wan : [0-5]$`" = "0" ] && [ "$new_wan" != "-1" ]; then
    echo "Error: The input wan_port $new_wan is invalid"
    exit 2
fi

parser_cur_port_map

if [ "$new_wan" = "$cur_wan" ]; then
    if [ "$new_wan" = "-1" ]; then
        echo "EtherWan is already disable"
        exit 3
    else
        echo "EtherWan port$new_wan is already enable"
        exit 4
    fi
fi

parser_orig_port_map
create_new_portmap

#echo OrigPortMap $OrigPortMap
#echo NewPortMap $NewPortMap

if [ "$cur_wan" != "-1" ]; then
    if [ "$new_wan" != "-1" ]; then
        echo "EtherWan port$cur_wan will be repleaced with port$new_wan"
        ifconfig nas10 down
        /userfs/bin/ethphxcmd eth0 vlanpt disable
        ifconfig eth0 down
        echo $NewPortMap > /proc/tc3162/eth_portmap
        result=`echo $?`
        backup_cpu_port_rate_limit
        ifconfig eth0 up
        restore_cpu_port_rate_limit
        dis_cpu_port_rate_limit
        /userfs/bin/ethphxcmd eth0 vlanpt enable
        /userfs/bin/ethphxcmd vlan active 1
        ifconfig nas10 up
        if [ "$result" = "0" ]; then
            echo "Repleace Success"
        else
            echo "Repleace Failed"
        fi
    else
        echo "EtherWan port$cur_wan will be disable"
        ifconfig nas10 down
        /userfs/bin/ethphxcmd eth0 vlanpt disable
        ifconfig eth0 down
        echo $OrigPortMap > /proc/tc3162/eth_portmap
        result=`echo $?`
        backup_cpu_port_rate_limit
        ifconfig eth0 up
        restore_cpu_port_rate_limit
        /userfs/bin/ethphxcmd eth0 vlanpt enable
        /userfs/bin/ethphxcmd vlan active 1
        if [ "$result" = "0" ]; then
            echo "Disable Success"
            /userfs/bin/ponmgr system set startup start
            ifconfig nas0 up
        else
            echo "Disable Failed"
            ifconfig nas10 up
        fi
    fi
else
    echo "EtherWan port$new_wan will be enable"
    ifconfig nas0 down
    /userfs/bin/ponmgr system set startup stop
    /userfs/bin/ethphxcmd eth0 vlanpt disable
    ifconfig eth0 down
    echo $NewPortMap > /proc/tc3162/eth_portmap
    result=`echo $?`
    backup_cpu_port_rate_limit
    ifconfig eth0 up
    restore_cpu_port_rate_limit
    /userfs/bin/ethphxcmd eth0 vlanpt enable
    /userfs/bin/ethphxcmd vlan active 1
    if [ "$result" = "0" ]; then
        echo "Enable Success"
        ifconfig nas10 up
        QoS=`/userfs/bin/tcapi get QoS_Common Active`
    		if [ "$QoS" = "Yes" ]; then
    				close_qos
    				open_qos
    		fi
    else
        echo "Enable Failed"
        /userfs/bin/ponmgr system set startup start
        ifconfig nas0 up
    fi
fi