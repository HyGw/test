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
******************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <linux/mii.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rt_flash.h>
#include <linux/version.h>
#include <ecnt_hook/ecnt_hook_net.h>
#include "eth_en7512.h"
#include "devBandWidth.h"


struct devBandwidthList_s *gDevBandwidthList = NULL;
struct wifi_dev_bandwidth_nodes_s *gWifiMaxBandwidthPtr = NULL;

int (*wifi_bandwidth_hook_tx) (struct sk_buff * skb) = NULL;
int  (*wifi_bandwidth_hook_rx)(unsigned char* srcMac, int length) = NULL;

EXPORT_SYMBOL(wifi_bandwidth_hook_tx);
EXPORT_SYMBOL(wifi_bandwidth_hook_rx);


#define read_reg_word(reg) 			VPint(reg)
#define write_reg_word(reg, wdata) 	VPint(reg)=wdata

#ifdef MT7530_SUPPORT
#define switch_reg_read(reg) 		 gswPbusRead((reg-GSW_BASE))
#define switch_reg_write(reg, wdata) gswPbusWrite((reg-GSW_BASE), wdata)
#else
#define switch_reg_read(reg) 		 read_reg_word(reg)
#define switch_reg_write(reg, wdata) write_reg_word(reg, wdata)
#endif


int isMacMatch(uint8_t * Dst, uint8_t * Src){
	if((Dst[0] == Src[0]) 
		&& (Dst[1] == Src[1]) 
		&& (Dst[2] == Src[2]) 
		&& (Dst[3] == Src[3]) 	
		&& (Dst[4] == Src[4]) 
		&& (Dst[5] == Src[5])){
			return 1;
		}
	
	return 0;
}


int isMacExist(unsigned char* macAddr){
	if(macAddr[0] == 0 		
		&& macAddr[1] == 0			
		&& macAddr[2] == 0		
		&& macAddr[3] == 0		
		&& macAddr[4] == 0		
		&& macAddr[5] == 0){		
		return 0;	
	}else{		
		return 1;	
	}
}

int isDevBandwithIsZero(struct wifi_dev_bandwidth_s *dev_node){
	
	if(dev_node == NULL){
		return 0;
	}

	if(dev_node->dsBandwidth.bandwidth == 0 && dev_node->usBandwidth.bandwidth == 0){
		return 1;
	}

	return 0;
}	

int findWifiBandWidthValidIdx(unsigned char* macAddr){
	int firstUnUsedItem = -1;
	int i = 0;
	struct wifi_dev_bandwidth_s *dev_node;


	for(i = 0 ; i < TRTCM_MATCH_MAC_RULE_NUM; i++){		
		dev_node = &(gWifiMaxBandwidthPtr->wifi_dev_nodes)[i];
		if(isMacExist(dev_node->devMac) && (isMacMatch(macAddr,dev_node->devMac))){
			/*is mac match*/
			return i;
		}

		if(!isDevBandwithIsZero(dev_node)){
			continue;
		}

		if(firstUnUsedItem == -1){			
			firstUnUsedItem = i;
		}		
	}
	
	return firstUnUsedItem;
}

int findWifiBandWidthIdx(unsigned char* macAddr){
	int i = 0;
	struct wifi_dev_bandwidth_s *dev_nodes;

	for(i = 0 ; i < TRTCM_MATCH_MAC_RULE_NUM; i++){		
		dev_nodes = &(gWifiMaxBandwidthPtr->wifi_dev_nodes)[i];
		if(isMacExist(dev_nodes->devMac)){
			if(isMacMatch(macAddr,dev_nodes->devMac)){
				/*is mac match*/
				return i;
			}
		}		
	}
	
	return -1;
}

static int wifi_maxbandwidth_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int i = 0;
	struct wifi_dev_bandwidth_s *dev_nodes;


	if(gWifiMaxBandwidthPtr != NULL){
		index += sprintf(buf+index, "enable = %d\n", gWifiMaxBandwidthPtr->enable);

		for(i = 0; i < TRTCM_MATCH_MAC_RULE_NUM; i++){
			dev_nodes = &(gWifiMaxBandwidthPtr->wifi_dev_nodes)[i];			
			if(dev_nodes != NULL && isMacExist(dev_nodes->devMac)){
				index += sprintf(buf+index, "%d %02x%02x%02x%02x%02x%02x,%d,%d\n", 
					i,
					dev_nodes->devMac[0],
					dev_nodes->devMac[1],
					dev_nodes->devMac[2],
					dev_nodes->devMac[3],
					dev_nodes->devMac[4],
					dev_nodes->devMac[5],
					dev_nodes->usBandwidth.bandwidth,
					dev_nodes->dsBandwidth.bandwidth);
			}
		}
	}else{
		printk("gWifiMaxBandwidthPtr is NULL");
	}



	*eof = 1;

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
	
}

static int wifi_maxbandwidth_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32] = {0};
	int mac[6];
	unsigned char macAddr[6];
	int upRate = 0;
	int downRate = 0;
	int index= -1;	
	struct wifi_dev_bandwidth_s *dev_nodes;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';


	printk("val_string = %s,count=%d\n",val_string,count);

	/* mac,usRate,dsRate=xx:xx:xx:xx:xx:xx,%d,%d*/
	if(count < 18 ){	
		if(gWifiMaxBandwidthPtr != NULL){
			gWifiMaxBandwidthPtr->enable = val_string[0]-'0';
		}
	}else{
		if(gWifiMaxBandwidthPtr == NULL){
			gWifiMaxBandwidthPtr = (uint32 *)kmalloc(sizeof(struct wifi_dev_bandwidth_nodes_s), GFP_KERNEL);
			if(gWifiMaxBandwidthPtr == NULL){
				printk("no memory for wifiBandwidthNodes\n");
				return 0;
			}
			memset(gWifiMaxBandwidthPtr,0,sizeof(struct wifi_dev_bandwidth_nodes_s));
		}

		
		sscanf(val_string, "%02x:%02x:%02x:%02x:%02x:%02x,%d,%d", 
			&mac[0], &mac[1],&mac[2],&mac[3],&mac[4],&mac[5],&upRate,&downRate);

		macAddr[0] = (unsigned char)mac[0];
		macAddr[1] = (unsigned char)mac[1];
		macAddr[2] = (unsigned char)mac[2];
		macAddr[3] = (unsigned char)mac[3];
		macAddr[4] = (unsigned char)mac[4];
		macAddr[5] = (unsigned char)mac[5];
		
		index = findWifiBandWidthValidIdx(macAddr);
		if(index < 0){
			printk("not find valid index for wifiBandwidthNodes\n");				
			return 0;
		}

		dev_nodes = &(gWifiMaxBandwidthPtr->wifi_dev_nodes)[index];
		memset(dev_nodes, 0, sizeof(struct wifi_dev_bandwidth_s));
		dev_nodes->devMac[0] = macAddr[0];			
		dev_nodes->devMac[1] = macAddr[1];			
		dev_nodes->devMac[2] = macAddr[2];			
		dev_nodes->devMac[3] = macAddr[3];			
		dev_nodes->devMac[4] = macAddr[4];			
		dev_nodes->devMac[5] = macAddr[5];			
		dev_nodes->usBandwidth.bandwidth= upRate<<10; 		/*  n kbps <<10 == n*1024 bit/s  */
		dev_nodes->dsBandwidth.bandwidth= downRate<<10; 			/*  n kbps <<10 == n*1024 bit/s  */
	}
		
	return count;
}


void wifiDevBandwidthUpdateTokens(struct wifi_bandwidth_Token_s *wifiBandWidthNode)
{
	uint32 time_span = 0;
	uint32 new_rate = 0;
	uint32 cur_rate = 0;
	
	wifiBandWidthNode->c_time = jiffies;
	
	time_span = wifiBandWidthNode->c_time - wifiBandWidthNode->l_time; 
				
	
	new_rate = min(time_span*wifiBandWidthNode->bandwidth/100, wifiBandWidthNode->bandwidth);		/*bps*/

	wifiBandWidthNode->l_time = wifiBandWidthNode->c_time;
	
	cur_rate = wifiBandWidthNode->c_rate;
	wifiBandWidthNode->c_rate = min(cur_rate+new_rate, wifiBandWidthNode->bandwidth);	

	
	if(gWifiMaxBandwidthPtr->enable == 3){
		printk("[%s:%d]time_span=%d,bandwidth=%d,cur_rate=%d,c_rate=%d\n",__FUNCTION__,__LINE__,time_span,wifiBandWidthNode->bandwidth,cur_rate,wifiBandWidthNode->c_rate);
	}

	return;
}

int wifiMaxBandwidthCheck(struct wifi_bandwidth_Token_s *wifiBandWidthNode, int length){

	if(wifiBandWidthNode == NULL){
		return 1;
	}

	if(wifiBandWidthNode->bandwidth == 0){
		/*if bandwidth is 0, no limit*/
		return 1;
	}
	
	wifiDevBandwidthUpdateTokens(wifiBandWidthNode);
	
	if(wifiBandWidthNode->c_rate < (length << 3)){
		//drop
		if(gWifiMaxBandwidthPtr->enable == 4){		
			printk("[%s:%d]drop packet!!!\n",__FUNCTION__,__LINE__);
			printk("[%s:%d]length=%d!!!\n",__FUNCTION__,__LINE__,length);		
			printk("[%s:%d]c_rate=%d!!!\n",__FUNCTION__,__LINE__,wifiBandWidthNode->c_rate);
		}
		return 0;
	}
	else{
		wifiBandWidthNode->c_rate -= (length)<<3;
	}
	
    return 1;
}

int32_t wifiMaxBandwidthHandler(unsigned char* macAddr, int length, int up_down){
	int index = -1;
	int ret = 1;
	struct wifi_dev_bandwidth_s *dev_nodes;
	
	if(gWifiMaxBandwidthPtr == NULL || (gWifiMaxBandwidthPtr->enable ==0)){
		/*no need to check bandwidth*/
		return 1;
	}

	if(!isMacExist(macAddr)){
		/*mac is empty*/
		return 1;
	}

	index = findWifiBandWidthIdx(macAddr);
	if(index < 0){
		/*mac not find, do nothing*/
		//printk("wifiMaxBandwidthHandler[%d]:mac not find, do nothing",__LINE__);
		return 1;
	}

	dev_nodes = &(gWifiMaxBandwidthPtr->wifi_dev_nodes)[index];

	if(gWifiMaxBandwidthPtr->enable == 3){
		printk("index=%d,up_down=%d, %02x:%02x:%02x:%02x:%02x:%02x\n",
			index,up_down,dev_nodes->devMac[0],dev_nodes->devMac[1],dev_nodes->devMac[2],dev_nodes->devMac[3],dev_nodes->devMac[4],dev_nodes->devMac[5]);
	}
	
	if(up_down == DEV_BANDWIDTH_UP){
		/*upstream*/
		ret = wifiMaxBandwidthCheck(&(dev_nodes->usBandwidth),length); 
	}else{
		ret = wifiMaxBandwidthCheck(&(dev_nodes->dsBandwidth),length); 
	}

	return ret;
}

int32_t wifiBandwidthTxHandler(struct sk_buff * skb){
	uint8 *cp=NULL;
	unsigned char macAddr[6];
	int ret = 1;
	struct wifi_dev_bandwidth_s *dev_nodes;

	if(skb == NULL){
		return 1;
	}
	
	cp = skb->data;
	memcpy(macAddr, cp, 6);		/*dst mac*/

	ret = wifiMaxBandwidthHandler(macAddr,skb->len,DEV_BANDWIDTH_DOWN);

	if(ret != 0){
		/* if ret == 0, packet should be drop*/
		devBandwidthCnt(macAddr,skb->len,DEV_BANDWIDTH_DOWN);
	}

	return ret;
}

int32_t wifiBandwidthRxHandler(unsigned char* srcMac, int length){
	int ret = 1;

	ret =  wifiMaxBandwidthHandler(srcMac,length,DEV_BANDWIDTH_UP);
	
	if(ret != 0){
		/* if ret == 0, packet should be drop*/
		devBandwidthCnt(srcMac,length,DEV_BANDWIDTH_UP);
	}

	return ret;
}

int findMacEntryIndex(unsigned char *mac){
	struct devBandwidth_s *devBandWidthPtr = NULL;
	int firstIndex = -1;
	int i = 0;
	
	if(gDevBandwidthList == NULL)
		return -1;

	for(i = 0; i < MAX_DEV_BANDWIDTH_NUM; i++){
		devBandWidthPtr = &(gDevBandwidthList->bandwidthList[i]);
		if(devBandWidthPtr != NULL && isMacMatch(mac,devBandWidthPtr->mac)){
			return i;
		}

		if(devBandWidthPtr != NULL && devBandWidthPtr->valid){
			continue;
		}
		
		if(firstIndex == -1){
			firstIndex = i;
		}
	}

	return firstIndex;	
}

__IMEM int findMacMatchItem(unsigned char *mac){
	struct devBandwidth_s *devBandWidthPtr = NULL;
	int i = 0;
	
	if(gDevBandwidthList == NULL)
		return -1;

	for(i = 0; i < MAX_DEV_BANDWIDTH_NUM; i++){
		devBandWidthPtr = &(gDevBandwidthList->bandwidthList[i]);
		if(devBandWidthPtr != NULL && isMacMatch(mac,devBandWidthPtr->mac)){
			return i;
		}		
	}

	return -1;	
}


int delDevBandWidthMac(unsigned char *mac){
	int index = -1;
	struct devBandwidth_s *devBandWidthPtr = NULL;

	if(gDevBandwidthList == NULL)
		return 0;

	if(mac == NULL){
		return -1;
	}


	index = findMacMatchItem(mac);

	if(index < 0){
		return -1;
	}

	devBandWidthPtr = &(gDevBandwidthList->bandwidthList[index]);
	if(devBandWidthPtr == NULL){
		printk("updateDevBandWidthMac:devBandWidthPtr == NULL,index =%d\n",index);
		return -2;
	}
	
	devBandWidthPtr->valid = 0;
	return 0;
}


int updateDevBandWidthMac(unsigned char *mac){
	int index = -1;
	struct devBandwidth_s *devBandWidthPtr = NULL;

	
	
	if(gDevBandwidthList == NULL){
		gDevBandwidthList = (uint32 *)kmalloc(sizeof(struct devBandwidthList_s), GFP_KERNEL);
		if(gDevBandwidthList == NULL){
			printk("no memory for wifiBandwidthNodes\n");
			return -1;
		}
		memset(gDevBandwidthList,0,sizeof(struct devBandwidthList_s));		
	}	

	index = findMacEntryIndex(mac);
	if(index < 0){
		return -2;
	}

	devBandWidthPtr = &(gDevBandwidthList->bandwidthList[index]);
	if(devBandWidthPtr == NULL){
		printk("updateDevBandWidthMac:devBandWidthPtr == NULL,index =%d\n",index);
		return -3;
	}
	
	memcpy(&(devBandWidthPtr->mac),mac, ETH_ALEN);
	devBandWidthPtr->valid = 1;
	
	printk( "updateDevBandWidthMac:index=%d,%02x:%02x:%02x:%02x:%02x:%02x\n", 
		index,devBandWidthPtr->mac[0], devBandWidthPtr->mac[1],devBandWidthPtr->mac[2],devBandWidthPtr->mac[3],devBandWidthPtr->mac[4],devBandWidthPtr->mac[5]);
	return 0;
}


__IMEM void devBandwidthCnt(unsigned char* mac, int length, int up_down){		
	struct devBandwidth_s *devBandWidthPtr = NULL;
	int index = -1; 
	
	if(gDevBandwidthList == NULL || gDevBandwidthList->enable == 0){
		return;
	}

	index = findMacMatchItem(mac);			
	if(index < 0){
		return;
	}
	
	devBandWidthPtr = &(gDevBandwidthList->bandwidthList[index]);
	switch(up_down){
		case DEV_BANDWIDTH_UP:
			devBandWidthPtr->upBytes += length;
			break;
		case DEV_BANDWIDTH_DOWN:
			devBandWidthPtr->downBytes += length;
			break;
		
	}

	return;
}


__IMEM void checkMacBandWitdh(struct sk_buff *skb, int type, uint32 frameSize){
	uint8 *cp=NULL;
	unsigned char macAddr[6];

	if(skb == NULL){
		return;
	}

	if(gDevBandwidthList == NULL || gDevBandwidthList->enable == 0){
		return;
	}


	cp = skb->data;
	if(type == DEV_BANDWIDTH_UP){
		/*src mac*/
		memcpy(macAddr, cp + 6, 6);
	}else{
		/*dst mac*/
		memcpy(macAddr, cp, 6);	
	}
	
	devBandwidthCnt(macAddr, frameSize, type);
	return;
}

static int dev_Bandwidth_read_proc(char *buf, char **start, off_t off,
		int count, int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int i;
	struct devBandwidth_s *devBandWidthPtr = NULL;
	unsigned long uniUpxCnt;
	unsigned long uniDownCnt;	
	unsigned long uniUpBytes;
	unsigned long uniDownBytes;
	int upRuleIndex = 0;
	int downRuleIndex = 0;

	
	if(gDevBandwidthList != NULL){
		index += sprintf(buf+index, "enable = %d\n", gDevBandwidthList->enable);

		
		for(i = 0; i < MAX_DEV_BANDWIDTH_NUM; i++){		
			devBandWidthPtr = &(gDevBandwidthList->bandwidthList[i]);
			if(devBandWidthPtr->valid == 0){
				continue;
			}

			uniUpBytes = devBandWidthPtr->upBytes;
			uniDownBytes = devBandWidthPtr->downBytes;
		
			index += sprintf(buf+index, "%d,%d,%d,%02x%02x%02x%02x%02x%02x\n", 
				i, uniUpBytes,uniDownBytes,devBandWidthPtr->mac[0], devBandWidthPtr->mac[1],devBandWidthPtr->mac[2],devBandWidthPtr->mac[3],devBandWidthPtr->mac[4],devBandWidthPtr->mac[5]);
		}
	}else{
		index += sprintf(buf+index, "enable = %d\n", 0);
		printk("hwnat_mac_bandwidth_read_proc: gDevBandwidthList == NULL\n");
	}

	if (index <= off + count)
		*eof = 1;
	
	*start = buf + off;
	index -= off;
	if (index > count)
		index = count;
	if (index < 0)
		index = 0;
	return index;
}

int dev_Bandwidth_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	char get_buf[32];	
	int mac[6];
	unsigned char strMac[6];
	int actionType = 0;
	int enable = 0;

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	
	get_buf[len]='\0';
	if(len < 12 ){
		enable = get_buf[0] - '0';
		if(enable == 0){		
			if(gDevBandwidthList != NULL)
				gDevBandwidthList->enable = enable;
		}else{
			if(gDevBandwidthList == NULL){
				gDevBandwidthList = (uint32 *)kmalloc(sizeof(struct devBandwidthList_s), GFP_KERNEL);
				if(gDevBandwidthList == NULL){
					printk("no memory for wifiBandwidthNodes\n");
					return -1;
				}
				memset(gDevBandwidthList,0,sizeof(struct devBandwidthList_s));		
			}
			
			gDevBandwidthList->enable = enable;
		}
	}else{
		sscanf(get_buf, "%d %02x:%02x:%02x:%02x:%02x:%02x", 
			&actionType, &mac[0], &mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);

		strMac[0] = (unsigned char)mac[0];
		strMac[1] = (unsigned char)mac[1];
		strMac[2] = (unsigned char)mac[2];
		strMac[3] = (unsigned char)mac[3];
		strMac[4] = (unsigned char)mac[4];
		strMac[5] = (unsigned char)mac[5];

		if(actionType == 1){
			/*add and update mac*/
			updateDevBandWidthMac(strMac);
		}else if(actionType == 2){
			delDevBandWidthMac(strMac);
		}
	}

	if(gDevBandwidthList)
		printk("enable=%d,%02x%02x%02x%02x%02x%02x,actionType=%d\n", gDevBandwidthList->enable, strMac[0], strMac[1],strMac[2],strMac[3],strMac[4],strMac[5],actionType);
	else
		printk("gDevBandwidthList is NULL\n");
	
	return len;
}

static int total_Bandwidth_read_proc(char *buf, char **start, off_t off,
		int count, int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
		
	if (isMT7520G || isMT7525G){
		index += sprintf(buf+index, "%d,%d\n", 
			(gswPbusRead(EXT_GSW_TX_OCL(6))+(gswPbusRead(EXT_GSW_TX_OCH(6))>>32)),
			(gswPbusRead(EXT_GSW_RX_OCL(6))+(gswPbusRead(EXT_GSW_RX_OCH(6))>>32)));
	}else{
		index += sprintf(buf+index, "%d,%d\n", 
			(switch_reg_read(GSW_TX_OCL(6))+(switch_reg_read(GSW_TX_OCH(6))>>32)),
			(switch_reg_read(GSW_RX_OCL(6))+(switch_reg_read(GSW_RX_OCH(6))>>32)));
	}
	
	if (index <= off + count)
		*eof = 1;
	
	*start = buf + off;
	index -= off;
	if (index > count)
		index = count;
	if (index < 0)
		index = 0;
	return index;
}

ecnt_ret_val ecnt_updateDevBandWidthMac(struct ecnt_data *in_data){
	struct net_data_s *net_data = (struct net_data_s *)in_data;
	unsigned char *addr = net_data->br_fdb_data.addr;
	struct net_bridge_fdb_entry *fdb = net_data->br_fdb_data.fdb;


	if ( NULL != strstr(fdb->dst->dev->name, "eth0.") || NULL != strstr(fdb->dst->dev->name, "ra") )
	{
		updateDevBandWidthMac(addr);
	}			

	return ECNT_CONTINUE;
}

ecnt_ret_val ecnt_delDevBandWidthMac(struct ecnt_data *in_data){
	struct net_data_s *net_data = (struct net_data_s *)in_data;
	unsigned char *addr = net_data->br_fdb_data.addr;
	struct net_bridge_fdb_entry *fdb = net_data->br_fdb_data.fdb;

	if ( NULL != strstr(fdb->dst->dev->name, "eth0.") || NULL != strstr(fdb->dst->dev->name, "ra")){
		delDevBandWidthMac(addr);
	}

	return ECNT_CONTINUE;
}

struct ecnt_hook_ops ecnt_updateDevBandWidthMac_op = {
	.name = "ecnt_updateDevBandWidthMac",
	.is_execute = 1,
	.hookfn = ecnt_updateDevBandWidthMac,
	.maintype = ECNT_NET_BR_FDB,
	.subtype = ECNT_BR_FDB_CREATE,
	.priority = 1
};


struct ecnt_hook_ops ecnt_delDevBandWidthMac_op = {
	.name = "ecnt_delDevBandWidthMac",
	.is_execute = 1,
	.hookfn = ecnt_delDevBandWidthMac,
	.maintype = ECNT_NET_BR_FDB,
	.subtype = ECNT_BR_FDB_DELETE,
	.priority = 1
};


int dev_bandwidth_func_init(){
    struct proc_dir_entry *proc;

	proc = create_proc_entry("tc3162/totalBandwidth", 0, NULL);
	proc->read_proc = total_Bandwidth_read_proc;


	proc = create_proc_entry("tc3162/devBandwidth", 0, NULL);
	proc->read_proc = dev_Bandwidth_read_proc;
	proc->write_proc = dev_Bandwidth_write_proc;

	proc = create_proc_entry("tc3162/wifi_dev_maxbandwidth", 0, NULL);
	proc->read_proc = wifi_maxbandwidth_read_proc;
	proc->write_proc = wifi_maxbandwidth_write_proc;
	
	rcu_assign_pointer(wifi_bandwidth_hook_tx,
						wifiBandwidthTxHandler);
	rcu_assign_pointer(wifi_bandwidth_hook_rx,
						wifiBandwidthRxHandler);

	ecnt_register_hook(&ecnt_updateDevBandWidthMac_op);
	ecnt_register_hook(&ecnt_delDevBandWidthMac_op);
}

int dev_bandwidth_func_deinit(){

	remove_proc_entry("tc3162/totalBandwidth", 0);	
	remove_proc_entry("tc3162/devBandwidth", 0);
	remove_proc_entry("tc3162/wifi_dev_maxbandwidth", 0);

	rcu_assign_pointer(wifi_bandwidth_hook_tx, NULL);
	rcu_assign_pointer(wifi_bandwidth_hook_rx, NULL);

	ecnt_unregister_hook(&ecnt_updateDevBandWidthMac_op);
	ecnt_unregister_hook(&ecnt_delDevBandWidthMac_op);
}

