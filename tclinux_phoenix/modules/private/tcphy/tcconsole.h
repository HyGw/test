/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#ifndef _TCCONSOLE_H_
#define _TCCONSOLE_H_

#define TC_CONSOLE_ENABLE       1   /*Ryan_20091019*/

#define MAX_VLAN_LAYER	2

#define MAC_RTS_START 		0x01
#define MAC_RTS_RESPONSE 	0x02
#define MAC_RTS_STOP 		0x03
#define MAC_RTS_ON		 	0x04
#define MAC_RTS_CONSOLE_ON	0x05
#define MAC_RTS_CONSOLE_CMD	0x06
#define MAC_RTS_CONSOLE_DATA	0x07

int isTCConsolePkt(struct sk_buff *skb);
int tcconsole_pro_idx(struct sk_buff *skb, int* vlan_level);
void tcconsole_chk(void);
void uart_msg_to_tcconsole(char* msg, int len);
int tcconsole_proc(struct sk_buff *skb);
void create_tcconsole_proc(void);
void delete_tcconsole_prco(void);
void macSend(uint32 chanId, struct sk_buff *skb);
void delete_tcconsole_proc(void);

extern void tcconsole_cmd(char* cmd, int len);
extern void (*send_uart_msg)(char* msg, int len);

typedef struct tcconsole_uart_timeout{
	uint8 sendUart;
	uint8 timeout;
	uint16 rcvpkt;
}tcconsole_uart_timeout_t;


typedef struct dmtConsoleData_s {
	uint8 *data;
	uint32 len;
} dmtConsoleData_t;

void getTcconsoleFile(void);

#endif
