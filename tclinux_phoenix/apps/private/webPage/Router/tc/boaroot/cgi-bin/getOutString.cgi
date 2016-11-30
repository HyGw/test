#!/bin/sh
echo -e "Content-Type: text/html \n"
echo -e "<html><body>"
indexVal=`echo $QUERY_STRING | cut -d = -f 2`
case "$indexVal" in
					"1" )
						/userfs/bin/wget -q --connect-timeout=5 --timeout=5 --tries=1 --max-redirect=0 -O /tmp/fon.html http://fon-instalacao.oi.com.br/wmfon.html
						;;
					"2" )
						/userfs/bin/wget -q --connect-timeout=5 --timeout=5 --tries=1 -O /tmp/fon.html http://fon-instalacao.oi.com.br/wmfon2.html
						;;
					"3" )
						/userfs/bin/wget -q --connect-timeout=5 --timeout=5 --tries=1 -O /tmp/fon.html http://fon-instalacao.oi.com.br/wmfon3.html						
						;;
					esac
contentStr=`cat /tmp/fon.html`;
if [ "$contentStr" = "" ]
then
			contentStr="<div></div>"					
fi	
echo -e "$contentStr</body></html>"
