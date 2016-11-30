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

#ifndef _DEVBANDWIDTH_H
#define _DEVBANDWIDTH_H

#include <linux/types.h>


#define DEV_BANDWIDTH_UP 		1
#define DEV_BANDWIDTH_DOWN 		2
#define TRTCM_MATCH_MAC_RULE_NUM  		10		/*for max bandwidth*/
#define MAX_DEV_BANDWIDTH_NUM 30				/*bandwidth monitor of dev*/

typedef struct devBandwidth_s{
	unsigned char mac[6];
	long upBytes;
	long downBytes;
	int valid;
}devBandwidth_t;

typedef struct devBandwidthList_s{	
	int enable;
	struct devBandwidth_s bandwidthList[MAX_DEV_BANDWIDTH_NUM];
}devBandwidthList_t;


/*wifi*/
typedef struct wifi_bandwidth_Token_s {
	uint32 bandwidth; 
	uint32 c_rate; /*current rate*/
	uint32 c_time; /*current time*/
	uint32 l_time; /*last check time*/
} wifi_bandwidth_Token_t;

typedef struct wifi_dev_bandwidth_s {
	unsigned char devMac[6];
	struct wifi_bandwidth_Token_s dsBandwidth;
	struct wifi_bandwidth_Token_s usBandwidth;
} wifi_dev_bandwidth_t;

typedef struct wifi_dev_bandwidth_nodes_s {
	int enable;	
	struct wifi_dev_bandwidth_s wifi_dev_nodes[TRTCM_MATCH_MAC_RULE_NUM];
}wifi_dev_bandwidth_nodes_t;


void devBandwidthCnt(unsigned char* mac, int length, int up_down);
int isMacMatch(uint8_t * Dst, uint8_t * Src);
void checkMacBandWitdh(struct sk_buff *skb, int type, uint32 frameSize);

int dev_bandwidth_func_init(void);
int dev_bandwidth_func_deinit(void);

#endif /* _DEVBANDWIDTH_H */

