/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/

#include <stdint.h>
#include <sys/ioctl.h>
#include "cfg_manager.h"
#include "bandwidth.h"
#ifdef TCSUPPORT_MT7530_SWITCH_API
#include "../ethcmd/libmii.h"
#include "../lib/mtkswitch_api.h"
#endif

int getACLRate(int rate){
	int aclRate = 0x3FFF;
	int r1 = 0;
	int r2 = 0;

	r1 = rate/64;
	r2 = rate%32; 
	if(rate != 0){							
		if(r1 == 0){
			aclRate = 1;			/*ACL rate is n*64kbps */
		}else{
			if(r2 < 32){
				aclRate = r1;			/*ACL rate is n*64kbps */
			}else{
				aclRate = r1+1;			/*ACL rate is n*64kbps */
			}
		}
			
	}

	return aclRate;
}

int devMaxBandwidth_configure(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	char macAddr[20] = {0};
	int mac[6];
	char upRate[32] = {0};
	int rate = 0;
	char downRate[32] = {0};
	int upRuleExist = 0;	int downRuleExist = 0;
	struct mt7530_switch_dev_speed_acl_s devMacAcl;
	int i = 0;
	char wifiBandwidthBuf[128] = {0};


	bzero(&devMacAcl, sizeof(struct mt7530_switch_dev_speed_acl_s));
	bzero(nodeName, sizeof(nodeName));
	strcpy(nodeName[0], "MaxBandWidth");
	for(i = 0; i < TRTCM_MATCH_MAC_RULE_NUM; i++){
		sprintf(nodeName[1],"Entry%d",i);
		
		bzero(macAddr, sizeof(macAddr));
		if(getAttrValue(top, nodeName, "mac", macAddr) == TCAPI_PROCESS_OK && strlen(macAddr) != 0){
			sscanf(macAddr, "%02x%02x%02x%02x%02x%02x", 
				&mac[0], &mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
			devMacAcl.trtcm_acl_mac_table[i].dev_mac[0] = mac[0];
			devMacAcl.trtcm_acl_mac_table[i].dev_mac[1] = mac[1];
			devMacAcl.trtcm_acl_mac_table[i].dev_mac[2] = mac[2];
			devMacAcl.trtcm_acl_mac_table[i].dev_mac[3] = mac[3];
			devMacAcl.trtcm_acl_mac_table[i].dev_mac[4] = mac[4];
			devMacAcl.trtcm_acl_mac_table[i].dev_mac[5] = mac[5];
			

			bzero(upRate, sizeof(upRate));
			bzero(downRate, sizeof(downRate));			
			if(getAttrValue(top, nodeName, "upRate", upRate) == TCAPI_PROCESS_OK && strlen(upRate) != 0){
				rate = atoi(upRate);		/*kbps*/		
				devMacAcl.trtcm_acl_mac_table[i].us_queue_speed = getACLRate(rate);
				if((upRuleExist == 0) && (rate != 0)){
					upRuleExist = 1;
				}
			}else{
				/*upRate is not exist*/	
				continue;
			}
			
			if(getAttrValue(top, nodeName, "downRate", downRate) == TCAPI_PROCESS_OK && strlen(downRate) != 0){
				rate = atoi(downRate);
				devMacAcl.trtcm_acl_mac_table[i].ds_queue_speed = getACLRate(rate);
				if((downRuleExist == 0) && (atoi(downRate) != 0)){
					downRuleExist = 1;
				}
			}else{
				/*downRate is not exist*/	
				continue;
			}

			memset(wifiBandwidthBuf,0,sizeof(wifiBandwidthBuf));
			sprintf(wifiBandwidthBuf,"echo %02x:%02x:%02x:%02x:%02x:%02x,%s,%s > /proc/tc3162/wifi_dev_maxbandwidth",
				mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],upRate,downRate);
			system(wifiBandwidthBuf);
		}else{
			/*mac is not exist*/	
			continue;
		}	
	}
	
	if(downRuleExist || upRuleExist){
		devMacAcl.enable = 1;		
		system("echo 1 > /proc/tc3162/wifi_dev_maxbandwidth");
	}


	macMT7530SetMacMatchACL(&devMacAcl);
	return SUCCESS;

}

/*______________________________________________________________________________
**	devMaxBandwidth_init
**
**	descriptions:
**		the init function of DataSpeedLimi.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
devMaxBandwidth_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name="MaxBandWidth",
			.type=ENTRY_TYPE|TRTCM_MATCH_MAC_RULE_NUM,
			.adv_table=NULL,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=devMaxBandWidth_execute,
			.cfg_boot=devMaxBandWidth_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}


/*
devMaxBandWidth execute function
*/
int devMaxBandWidth_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	devMaxBandwidth_configure(top);
	return 0;
}
/*
devMaxBandWidth  boot function
*/
int devMaxBandWidth_boot(mxml_node_t *top)
{
	devMaxBandwidth_configure(top);
	return 0;
}


char *
bandwidth_adv_table[MAX_NODE_NAME]=
{
	"Common","DeviceSpeed","",
};

/*______________________________________________________________________________
**	dataspeedlimit_init
**
**	descriptions:
**		the init function of DataSpeedLimi.
**	parameters:
**
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
int
devBandwidth_init(void)
{
	int ret=0;
	cfg_node_t node={
			.name="BandWidth",
			.type=ENTRY_TYPE|ADV_FLAG|DEV_MAC_RULE_NUM,
			.adv_table=bandwidth_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=devBandWidth_execute,
			.cfg_boot=devBandWidth_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}


int devBandwidth_configure(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char enable[8]={0};
	char cmd[64] = {0};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"BandWidth");
	strcpy(nodeName[1],"Common");
	if((getAttrValue(top, nodeName, "Enable", enable) != TCAPI_PROCESS_OK) 
		|| strlen(enable) == 0 
		|| strcmp(enable,"0") == 0){
		
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"echo 0 > %s",FEMAC_BANDWIDTH_PATH);
		system(cmd);

		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"echo 0 > %s",HWNAT_BANDWIDTH_PATH);
		system(cmd);		
	}else{
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"echo %s > %s",enable,FEMAC_BANDWIDTH_PATH);
		system(cmd);
		
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"echo %s > %s",enable,HWNAT_BANDWIDTH_PATH);		
		system(cmd);
	}
	
	return 0;
}

/*
devBandWidth execute function
*/
int devBandWidth_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	devBandwidth_configure(top);
	return 0;
}

/*
devBandWidth boot function
*/
int devBandWidth_boot(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	devBandwidth_configure(top);
	return 0;
}


