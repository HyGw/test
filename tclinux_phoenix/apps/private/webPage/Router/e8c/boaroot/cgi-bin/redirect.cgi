#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type
content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head>"

node_name="System_Entry"
attr_name="wd_W"
W=`/userfs/bin/tcapi get $node_name $attr_name`
attr_name="wd_H"
H=`/userfs/bin/tcapi get $node_name $attr_name`
attr_name="wd_top"
top=`/userfs/bin/tcapi get $node_name $attr_name`
attr_name="wd_left"
left=`/userfs/bin/tcapi get $node_name $attr_name`
attr_name="wd_WDurl"
WDurl=`/userfs/bin/tcapi get $node_name $attr_name`
attr_name="wd_hostname"
hostname=`/userfs/bin/tcapi get $node_name $attr_name`

echo "<script LANGUAGE=\"JavaScript\">"
echo "setTimeout(loadPage,500);"
echo "function loadPage(){"
echo "window.open('$WDurl','popupwindow','width=$W,height=$H,top=$top,left=$left,toolbar=no,menubar=no,scrollbars=no,resizable=no,location=no,status=no');"
echo "window.frames['ifmChild'].location.href = '/cgi-bin/redirect_cancel.cgi';"
echo "window.location.href='http://$hostname';}"
echo "</script>" 

echo "<br>"
echo "<DIV id='divChild' style='DISPLAY: none'><iframe src='' id='ifmChild' name='ifmChild' frameborder='0' width='0' height='0'></iframe></DIV>"
echo "<br>"

echo "</html>"

