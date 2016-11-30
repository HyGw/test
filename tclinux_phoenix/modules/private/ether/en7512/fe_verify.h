/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7520_20120528/tclinux_phoenix/modules/private/raeth/femac.h#6 $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 Trendchip Technologies, Corp.
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
/*

 */
#ifndef _FE_VERIFY_H
#define _FE_VERIFY_H


#define LOOPBACK_TX			0x01
#define LOOPBACK_RX_DROP	0x02
#define LOOPBACK_RX_CHK		0x03
#define LOOPBACK_TX_QOS		0x04
#define LOOPBACK_MODE_MASK	0x0f
#define LOOPBACK_MODE(x)	((x) & 0x0f)
#define LOOPBACK_REG_TEST 	(1<<4)
#define LOOPBACK_TX_VLAN	(1<<5)
#define LOOPBACK_TX_RANDOM	(1<<6)
#define LOOPBACK_MSG		(1<<7)
#define LOOPBACK_PKT		(1<<11)
#define LOOPBACK_EXT		(1<<12)
#define LOOPBACK_TX_UDPCS	(1<<13)
#define LOOPBACK_TX_TCPCS	(1<<14)
#define LOOPBACK_CSO_TEST	(1<<15)


struct _reg_value{
	u32 reg_offset;
	u32 all1_value;
	u32 all0_value;
};


void macPhyLoopback(macAdapter_t *mac_p, int enable);
void dump_skb(struct sk_buff *skb);
void dump_data(char *p, int len);
int tc3262_gmac_loopback_gen(struct net_device *dev, int npackets, int txlen);
int tc3262_gmac_loopback_chk(struct sk_buff *skb, struct net_device *dev);
int tc3262_gmac_qos_gen(struct net_device *dev, int tx_len);
int tc3262_gmac_isrtest_gen(struct net_device *dev);
int tc3262_gmac_reg_test(void);
int eth_loopback_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data);
int eth_loopback_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data);
int eth_loopback_test_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data);
int eth_loopback_test_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data);
int vlantable_test(struct file *file, const char *buffer,
	unsigned long count, void *data);

int eth_loopback_rx_hook(ethRxMsg_t *ethRxMsg, struct sk_buff *skb);
int eth_loopback_tx_hook(ethTxMsg_t *ethTxMsg, struct sk_buff *skb);


#endif /* _FE_VERIFY_H */


