#!/bin/sh
echo -e "Content-Type: text/html \n"

type=`echo $QUERY_STRING | cut -d \& -f 1`
opt=`echo $QUERY_STRING | cut -d \& -f 2`
optval=`echo $opt | cut -d = -f 2`
optval=`echo $optval | sed -e 's/%20/ /g'`

`/userfs/bin/$optval`
echo "cmd:$optval"