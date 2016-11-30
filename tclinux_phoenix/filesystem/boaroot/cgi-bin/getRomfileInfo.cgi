#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css>"
echo "<meta http-equiv=Content-Type content=\"text/html; charset=gb2312\"></head>"
echo "<body leftmargin=0 topmargin=0><table cellspacing=0 cellPadding=0 border=0><tr><td>"
echo "<SELECT id='sel_romfile' size='1' name='sel_romfile' onChange='doVersionTypeChange();'>"

count=`/userfs/bin/tcapi get System_Entry romfileCount`
romfileSelect=`/userfs/bin/tcapi get System_Entry romfileSelect`
node_name="System_Entry"
while [ $count -gt 0 ]
do
	attr_name="romfile$count"
	attr_value=`/userfs/bin/tcapi get $node_name $attr_name`
	show_value="${attr_value#*_}"
	show_value="${show_value%.*}"
	if [ "$attr_value" = "$romfileSelect" ] ;then
		echo "<option value='$attr_value' selected>$show_value Version</option>"
	else
		echo "<option value='$attr_value'>$show_value Version</option>"
	fi
	count=`expr $count - 1`
done
echo "</SELECT>"
echo "<SCRIPT language=JavaScript type=text/javascript>
function doVersionTypeChange(){
		if (confirm('变更Version Type，设备将会重启。您是否确认？') == false)
			return;
		var f = parent.document.ConfigForm;	
		if ( null != f )
		{
			f.romfileselect.value = document.getElementById('sel_romfile').value;
			f.Register_CHANGE.value = '2';
			f.submit();
		}
}		
</SCRIPT>"
echo "</td></tr></table></body></html>"
 
