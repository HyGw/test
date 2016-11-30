#!/bin/sh
echo -e "Content-Type: text/html \n"

name=`echo $QUERY_STRING | cut -d \& -f 1`
password=`echo $QUERY_STRING | cut -d \& -f 2`
nameval=`echo $name | cut -d = -f 2` 
passwordval=`echo $password | cut -d = -f 2`

`/userfs/bin/tcapi set WebCurSet_Entry wan_pvc 0`
`/userfs/bin/tcapi set Wan_PVC USERNAME $nameval`
`/userfs/bin/tcapi set Wan_PVC PASSWORD $passwordval`
`/userfs/bin/tcapi commit Wan_PVC`
