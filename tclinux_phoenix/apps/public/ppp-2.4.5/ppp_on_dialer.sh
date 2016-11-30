#!/bin/sh
#
#This is part 2 of ppp dial script. it will perform the connection 
#protocol for the desired connection.
#detail configuration please reference to ISP you will access.
#only script for need to auth
#auther: gcyin
echo "APN:$APN TEL:$TELEPHONE USRNAME:$USERNAME PWD:$PASSWORD PIN:$PIN" > /var/tmp/3g_dial_info
cd && ./userfs/bin/chat -v 								\
	TIMEOUT			3								\
	ABORT			'\nBUSY\r'							\
	ABORT			'\nNO CARRIER\r'						\
	ABORT			'\nNO ANSWER\r'							\
	ABORT			'\nRINGING\r\n\r\nRINGING\r'					\
	ABORT			'\nNO DIALTONE\r'						\
	''			\rAT								\
	'OK-+++\c-OK'		ATH0								\
	TIMEOUT			30								\
	''              	AT+CPIN=\"$PIN\"				\
	OK              	AT+CGDCONT=1,\"IP\",\"$APN\"				\
	OK			AT								\
	OK			AT								\
	OK			ATDT$TELEPHONE							\
	CONNECT			''								\
