#ifndef _GSW_H
#define _GSW_H

/********************* MT7530 Switch Register ***********************/
#define MIB_BASE_REG 			0x4000
#define MIB_PORT_OFFSET 		0x0100
#define GSW_BASE     			(0x00)
#define GSW_MAC_CTRL			0x3000

#define MIB_TX_DROP_REG     	(MIB_BASE_REG + 0x0000)
#define MIB_TX_CRC_REG			(MIB_BASE_REG + 0x0004)
#define MIB_TX_UCAST_REG 		(MIB_BASE_REG + 0x0008)
#define MIB_TX_MCAST_REG 		(MIB_BASE_REG + 0x000c)
#define MIB_TX_BCAST_REG 		(MIB_BASE_REG + 0x0010)
#define MIB_TX_COL_REG 			(MIB_BASE_REG + 0x0014)
#define MIB_TX_SCOL_REG 		(MIB_BASE_REG + 0x0018)
#define MIB_TX_MCOL_REG 		(MIB_BASE_REG + 0x001c)
#define MIB_TX_DEFER_REG 		(MIB_BASE_REG + 0x0020)
#define MIB_TX_LCOL_REG 		(MIB_BASE_REG + 0x0024)
#define MIB_TX_XCOL_REG 		(MIB_BASE_REG + 0x0028)
#define MIB_TX_PAUSE_REG 		(MIB_BASE_REG + 0x002c)
#define MIB_TX_PKT_64_CNT		(MIB_BASE_REG + 0x0030)
#define MIB_TX_PKT_65TO127_CNT	(MIB_BASE_REG + 0x0034)
#define MIB_TX_PKT_128TO255_CNT	(MIB_BASE_REG + 0x0038)
#define MIB_TX_PKT_256TO511_CNT (MIB_BASE_REG + 0x003c)
#define MIB_TX_PKT_512TO1023_CNT (MIB_BASE_REG + 0x0040)
#define MIB_TX_PKT_1024TOMAX_CNT (MIB_BASE_REG + 0x0044)
#define MIB_TX_OCT_CNT_L		(MIB_BASE_REG + 0x0048)
#define MIB_TX_OCT_CNT_H		(MIB_BASE_REG + 0x004c)

#define MIB_RX_DROP_REG 		(MIB_BASE_REG + 0x0060)
#define MIB_RX_FILTER_REG 		(MIB_BASE_REG + 0x0064)
#define MIB_RX_UCAST_REG 		(MIB_BASE_REG + 0x0068)
#define MIB_RX_MCAST_REG 		(MIB_BASE_REG + 0x006c)
#define MIB_RX_BCAST_REG 		(MIB_BASE_REG + 0x0070)

#define MIB_RX_ALIGN_ERR_REG 	(MIB_BASE_REG + 0x0074)
#define MIB_RX_FCS_ERR_REG 		(MIB_BASE_REG + 0x0078)
#define MIB_RX_UNDERSIZE_REG 	(MIB_BASE_REG + 0x007c)
#define MIB_RX_FRAG_ERR_REG 	(MIB_BASE_REG + 0x0080)
#define MIB_RX_OVERSIZE_REG 	(MIB_BASE_REG + 0x0084)
#define MIB_RX_JABB_ERR_REG 	(MIB_BASE_REG + 0x0088)
#define MIB_RX_PAUSE_REG 		(MIB_BASE_REG + 0x008c)

#define MIB_RX_PKT_64_CNT 		(MIB_BASE_REG + 0x0090)
#define MIB_RX_PKT_65TO127_CNT 	(MIB_BASE_REG + 0x0094)
#define MIB_RX_PKT_128TO255_CNT (MIB_BASE_REG + 0x0098)
#define MIB_RX_PKT_256TO511_CNT (MIB_BASE_REG + 0x009c)
#define MIB_RX_PKT_512TO1023_CNT (MIB_BASE_REG + 0x00a0)
#define MIB_RX_PKT_1024TOMAX_CNT (MIB_BASE_REG + 0x00a4)

#define MIB_RX_OCT_CNT_L 		(MIB_BASE_REG + 0x00a8)
#define MIB_RX_OCT_CNT_H 		(MIB_BASE_REG + 0x00ac)

#define MIB_RX_CTRL_CNT			(MIB_BASE_REG + 0x00b0)

#define MIB_AE0_CNT				(MIB_BASE_REG + 0xF00)
#define MIB_AE1_CNT 			(MIB_BASE_REG + 0xF04)
#define MIB_AE2_CNT 			(MIB_BASE_REG + 0xF08)
#define MIB_AE3_CNT 			(MIB_BASE_REG + 0xF0c)
#define MIB_AE4_CNT 			(MIB_BASE_REG + 0xF10)


/************************ SLT Program Defined Marco *****************************/
#define 	LINK_SPEED_10M		0
#define 	LINK_SPEED_100M		1
#define 	LINK_SPEED_1000M	2

#define		RELAY_LONG			0
#define		RELAY_LPBK			1
#define		RELAY_OPEN			2
#define		RELAY_P0P4			3

#define PHYOR_DATA 		(0xffff<<0)	
#define PHYOR_ADDR_SHIFT	16
#define PHYOR_REG_SHIFT		21
#define PHYOR_READ		(1<<26)
#define PHYOR_WRITE		(1<<27)
#define PHYOR_MDC		28

#define NEW_PHYOR_DATA		(0xffff<<0)	
#define NEW_PHYOR_REG_SHIFT	16
#define NEW_PHYOR_ADDR_SHIFT	21
#define NEW_PHYOR_OP_SHIFT	26
#define NEW_PHYOR_OP_WR		(1)
#define NEW_PHYOR_OP_RD		(2)
#define NEW_PHYOR_OP_EEE_FA				(0)/*Filed address*/
#define NEW_PHYOR_OP_EEE_WR				(1)
#define NEW_PHYOR_OP_EEE_POST_RD		(2)
#define NEW_PHYOR_OP_EEE_RD				(3)
#define NEW_PHYOR_ST_SHIFT	28
#define NEW_PHYOR_ST_IEEE_NEW	(0)
#define NEW_PHYOR_ST_IEEE_OLD	(1)
#define NEW_PHYOR_BUSY		(1<<31)

#define POST_READ           (1<<1)
#define NORMAL_READ 		(1<<0)

/* pull GPIO to high */
#define GPIO_HIGH(x) do{ (x > 15) ? (VPint(CR_GPIO_CTRL1) |= (1<<(x-16)*2)) : (VPint(CR_GPIO_CTRL) |= (1<<((x)*2))); \
	VPint(CR_GPIO_ODRAIN) |= (1<<(x)); \
	VPint(CR_GPIO_DATA) |= (1<<(x));}while(0)

/* pull GPIO to low */
#define GPIO_LOW(x) do{ (x > 15) ? (VPint(CR_GPIO_CTRL1) |= (1<<(x-16)*2)) : (VPint(CR_GPIO_CTRL) |= (1<<((x)*2))); \
	VPint(CR_GPIO_ODRAIN) |= (1<<(x)); \
	VPint(CR_GPIO_DATA) &= ~(1<<(x));} while(0)

#define MT7530_CHIP	0x7530

#define GPIO_BIN0 0   /*Auto Bench Start of test*/
#define GPIO_BIN1 1
#define GPIO_BIN2 2
#define GPIO_BIN3 3
#define GPIO_BIN4 4
#define GPIO_BIN5 5
#define GPIO_BIN6 6
#define GPIO_BIN7 7
#define GPIO_EOT 0xff /*Auto Bench End of test */

#endif
