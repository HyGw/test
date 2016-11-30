/***************************************************************
Copyright Statement:

This software/firmware and related documentation (��EcoNet Software��) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (��EcoNet��) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (��ECONET SOFTWARE��) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ��AS IS�� 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER��S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER��S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/

#include "./tcphy/switch_proc.h"

#ifdef TCSUPPORT_AUTOBENCH
#include "../auto_bench/autobench.h"
#endif

    
#include "./tcphy/tcconsole.h"
#include "eth_proc.h"

#ifdef TCSUPPORT_MT7530_SWITCH_API
#include "./tcphy/mtkswitch_api.h"
#include <modules/eth_global_def.h>
#endif

    
#ifdef LOOPBACK_SUPPORT
#include "fe_verify.h"
#endif

#include "./tcphy/phy_api.h"

#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
#include "devBandWidth.h"
#endif

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#ifdef TCSUPPORT_CPU_MT7520
#define MAX_TX_QUEUE_TRTCM_NUM  32
#define MAX_TRTCM_PARAM_NUM     16
#define MAX_QOS_CHANNEL_NUM     16
#define MAX_LLID_NUM            8
#define MAX_QUEUE_NUM           8
#define MAX_THLD_POS_NUM        3
#define FE_REG_ADDR_STAR	(0xbfb50000)
#define FE_REG_ADDR_END		(0xbfb51944)
#define FE_REG_LEN			(((FE_REG_ADDR_END - FE_REG_ADDR_STAR) >> 2) + 1)
#endif


unsigned char sync_pkt[100] = {
  0x01, 0x00, 0x5e, 0x00, 0x01, 0x81, 0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45, 0x08, 0x00, 0x45, 0x00,
  0x00, 0x48, 0x00, 0x00, 0x40, 0x00, 0x01, 0x11, 0xd6, 0x7a, 0xc0, 0xa8, 0x01, 0x01, 0xe0, 0x00,
  0x01, 0x81, 0x01, 0x3f, 0x01, 0x3f, 0x00, 0x34, 0x5f, 0x00, 0x80, 0x02, 0x00, 0x2c, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa,
  0xbb, 0xff, 0xfe, 0x01, 0x23, 0x45, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x30,
  0x8a, 0xca, 0x1f, 0xdd, 0xa0, 0xe4
};

unsigned char follow_pkt[100] = {
  0x01, 0x00, 0x5e, 0x00, 0x01, 0x81, 0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45, 0x08, 0x00, 0x45, 0x00,
  0x00, 0x48, 0x00, 0x00, 0x40, 0x00, 0x01, 0x11, 0xd6, 0x6f, 0xc0, 0xa8, 0x01, 0x0c, 0xe0, 0x00,
  0x01, 0x81, 0x01, 0x40, 0x01, 0x40, 0x00, 0x34, 0xbb, 0x61, 0x88, 0x02, 0x00, 0x2c, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0x3d, 0xff, 0xfe, 0x81, 0x9f, 0x00, 0x00, 0x01, 0x00, 0x26, 0x02, 0x00, 0x00, 0x00, 0x4f, 0x30,
  0x8a, 0xf5, 0x39, 0x12, 0x23, 0x50
};

unsigned char delay_req_pkt[100] = {
  0x01, 0x00, 0x5e, 0x00, 0x00, 0x6b, 0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45, 0x08, 0x00, 0x45, 0x00,
  0x00, 0x52, 0x00, 0x00, 0x40, 0x00, 0x01, 0x11, 0xd7, 0x86, 0xc0, 0xa8, 0x01, 0x01, 0xe0, 0x00,
  0x00, 0x6b, 0x01, 0x3f, 0x01, 0x3f, 0x00, 0x3e, 0xa4, 0x31, 0x82, 0x02, 0x00, 0x36, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa,
  0xbb, 0xff, 0xfe, 0x01, 0x23, 0x45, 0x00, 0x01, 0x00, 0x16, 0x05, 0x7f, 0x00, 0x00, 0x4f, 0x30,
  0x8a, 0xf8, 0x1f, 0x53, 0x55, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#ifdef TCSUPPORT_FREE_BOOTBASE
#define FLAG_ADDR		0x80001fff;
#else
#define FLAG_ADDR		0x8001ffff;
#endif

/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/
extern int mac_receive_num;
extern int mac_receive_threshold;
extern macAdapter_t *mac_p;
extern uint8 macInitialized;

extern uint8 use_ext_switch;
extern uint16 lan_port_tpid[MT7530_SWIC_PORTNUM];
extern int wan_port_id;
extern uint16 ether_wan_tpid;
extern uint16 cpu_port_tpid;
extern char switch_port_map[ RT63365_SWIC_PORTNUM];    
extern char lan_port_map[ RT63365_SWIC_PORTNUM ];

extern struct net_device *lan_dev;
extern int g_port_reverse;

extern int qos_flag;
extern int g_port_reverse_kernel; /**/

extern uint8 macSTagFlag;
extern uint8 swicVendor;
extern int g_stag_to_vtag;
extern char esd_has_reset;
extern int vport_enable;

extern unsigned char mac_sync_en;

extern int g_force_dstq;

/************************************************************************
*                  E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
#ifdef TCSUPPORT_QOS
    int qos_flag = NULLQOS;
#endif

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
        static int qos_wrr_info[5] = {0};
        static int max_prio = 3;
        static uint8 qos_wrr_user = 0x00;
#endif
static int protect_mode = 0;

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
//#ifdef TCSUPPORT_WAN_ETHER

uint32 calcnt(uint32 basereg){
	int i;
	uint32 ret_val = 0;

#ifdef TCSUPPORT_CPU_MT7505
	for(i=0; i<5;i++){
#else
	for(i=0; i<6;i++){
#endif
		if(i == wan_port_id)
			continue;
		ret_val += switch_reg_read(basereg + i*0x100);
	}

	return ret_val;
}


uint32 ext_cal_cnt(uint32 basereg){
    int i;
    uint32 ret_val = 0;
    for(i=0; i<6;i++){
        if(i == wan_port_id)
            continue;
        ret_val += gswPbusRead(basereg + i*0x100);
    }
    return ret_val;
}

static int doAclTblDisp(void)
{
	u32 value, vawd1, vawd2, rule_vawd1, rule_vawd2, rate_vawd1, rate_vawd2;
	int i;
	struct swconfig *swcfg=NULL;
	u8 portMac[6];

	/*no chip check ,should be added here*/

	while (1){	// wait until not busy
		value = read_reg_word(VTCR);
		if ((value & (1<<31)) == 0){
			break;
		}
	}

	// Display ACL pattern table entry 0~31
	printk("Found following ACL table entries :\n");
	for (i = 0; i < 32; i ++){
		write_reg_word(VTCR, ACL_PATTERN_READ(i));
		while (1){	// wait until not busy
			value = read_reg_word(VTCR);
			if ((value & (1<<31)) == 0){
				break;
			}
		}

		vawd1 = read_reg_word(VAWD1);
		vawd2 = read_reg_word(VAWD2);

		if (vawd2 & (1 << 19)){	// EN==1
			printk("[#%2d] CMP_SEL:%d, SP_map:%02X, OFST_TP:%d, WORD_OFST:%3d, BIT_MASK:%04X, CMP_PAT:%04X\n",
				i, (vawd2 & 1), ((vawd2>>8) & 0xFF), ((vawd2>>16) & 0x07), ((vawd2>>1) & 0x3F), ((vawd1>>16) & 0xFFFF), (vawd1 & 0xFFFF));
		}
	}

	// Display Mask, Rule Ctrl, and Rate Ctrl table entry 0~31
	printk("Found following Mask, Rule ctrl, Rate ctrl table entries : (in vawd1,vawd2)\n");
	for (i = 0; i < 32; i ++){
		write_reg_word(VTCR, ACL_MASK_READ(i));
		while (1){	// wait until not busy
			value = read_reg_word(VTCR);
			if ((value & (1<<31)) == 0){
				break;
			}
		}

		vawd1 = read_reg_word(VAWD1);
		vawd2 = read_reg_word(VAWD2);

		if (vawd1 || vawd2){	// ACL_MASK[63:0] any bit != 0
			write_reg_word(VTCR, ACL_RULE_READ(i));
			while (1){	// wait until not busy
				value = read_reg_word(VTCR);
				if ((value & (1<<31)) == 0){
					break;
				}
			}
			rule_vawd1 = read_reg_word(VAWD1);
			rule_vawd2 = read_reg_word(VAWD2);

			write_reg_word(VTCR, ACL_RATE_READ(i));
			while (1){	// wait until not busy
				value = read_reg_word(VTCR);
				if ((value & (1<<31)) == 0){
					break;
				}
			}
			rate_vawd1 = read_reg_word(VAWD1);
			rate_vawd2 = read_reg_word(VAWD2);

			printk("[#%2d] Mask:%08X,%08X; Rule:%08X,%08X; Rate:%08X,%08X\n",
					i, vawd1, vawd2, rule_vawd1, rule_vawd2, rate_vawd1, rate_vawd2);
		}
	}

	return 0;
}

/********************************proc****************************************/
int acl_rule_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	printk("PATTERN partition: \n\t 0~4 fixed for broadcast&multicast\n") ;
	if( SPECIAL_RULE_NUM )
		printk("\t %d~%d for special rule\n" , 5 , PATTERN_FOR_SPECIAL_NUM-1 );
	if( DMAC_RULE_NUM )
		printk("\t %d~%d for dmac rule\n" , PATTERN_FOR_SPECIAL_NUM , PATTERN_FOR_SPECIAL_NUM+PATTERN_FOR_DMAC_NUM-1 );
	if( SMAC_RULE_NUM )
		printk("\t %d~%d for smac rule\n" , PATTERN_FOR_SPECIAL_NUM+PATTERN_FOR_DMAC_NUM , PATTERN_FOR_SPECIAL_NUM+PATTERN_FOR_DMAC_NUM+PATTERN_FOR_SAMC_NUM-1 );
	if( VLAN_RULE_NUM )
		printk("\t %d~%d for vlan rule\n" , PATTERN_FOR_SPECIAL_NUM+PATTERN_FOR_DMAC_NUM+PATTERN_FOR_SAMC_NUM , PATTERN_ALL_NUM-1 );

	if( PATTERN_ALL_NUM > ACL_MAX_NUM )
	{
		printk("ERROR: the pattern used is over the limit!\n");
	}
	doAclTblDisp();
	return 0;
}

int acl_rule_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int i ;
	int len;
	char get_buf[32];
	int func ;
	int vlan ;
	int rate ;
	int enable ;
	int index;
	int mac_tmp[6];
	unsigned char mac[6];

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	/* use the copy_from_user function to copy buffer data to our get_buf */
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	/* zero terminate get_buf */
	get_buf[len]='\0';

	if(sscanf(get_buf, "%d", &func) != 1)
		return count;

	if( (func > 0) && (func < 4) )
	{
		if(sscanf(get_buf, "%d %d %d", &func , &rate , &enable) != 3)
			return count;
	}
	else if( func < 6)
	{
		if(sscanf(get_buf, "%d %x-%x-%x-%x-%x-%x %d %d %d", &func , &mac_tmp[0], &mac_tmp[1], &mac_tmp[2], &mac_tmp[3], &mac_tmp[4], &mac_tmp[5], 
																			&rate , &enable , &index) != 10)
			return count;
		for( i = 0 ; i < 6 ; i++ )
		{
			mac[i] = mac_tmp[i]&0xff;
		}
		printk("set mac = %02X-%02X-%02X-%02X-%02X-%02X\n",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
	else if( func == 6)
	{
		if(sscanf(get_buf, "%d %d %d %d %d", &func , &vlan ,&rate , &enable, &index) != 5)
			return count;
	}
	else
	{
		printk("error!\n");
		return count;
	}

	if( func < 4 )/*set 1 no-unicast acl rule, 2 broarcast rule, 3 multicast rule*/
		acl_set_upstream_ratelimit_for_special(rate ,enable , (func-1));
	else if( func == 4 )/*4 set dmac*/
		acl_set_upstream_ratelimit_for_mac(mac , rate , enable, index , ACL_SET_MODE_DMAC);
	else if( func == 5 )/*5 set smac*/
		acl_set_upstream_ratelimit_for_mac(mac , rate , enable, index , ACL_SET_MODE_SMAC);
	else if( func == 6 )/*6 set vlan*/
		acl_set_upstream_ratelimit_for_vlan(vlan , rate ,enable, index);

	return count;
}

int special_tpid_read_proc(char *buf, char **start, off_t off, 
    int count, int *eof, void *data)
{
	int i = 0,index = 0;
	off_t pos=0, begin=0 ;
	
	index += sprintf(buf+index, "Switch Port TPID:\nPort   TPID\n");
	for (i = 0; i < MT7530_SWIC_PORTNUM; i++) 
	{
		if(use_ext_switch == 1)
			index += sprintf(buf+index, "%d:    0x%04x\n", i,
			    ((gswPbusRead(0x2010 + (i * 0x100)) & 0xFFFF0000) >> 16));
		else
			index += sprintf(buf+index, "%d:    0x%04x\n", i,
			    ((read_reg_word(GSW_BASE + 0x2010 + 
			        (i * 0x100)) & 0xFFFF0000) >> 16));
		CHK_BUF();
	}
	index += sprintf(buf+index, "Lan Port TPID:\nPort   TPID\n");
	for (i = 0; i < MT7530_SWIC_PORTNUM; i++) 
	{
		index += sprintf(buf+index, "%d:    0x%x\n", i, lan_port_tpid[i]);
		CHK_BUF();
	}
	if (wan_port_id >= 0 && wan_port_id<MT7530_SWIC_PORTNUM-1)
		index += sprintf(buf+index, "EtherWanPort: 0x%04x\n", ether_wan_tpid);

	index += sprintf(buf+index, "Cpu Port: 0x%04x\n", cpu_port_tpid);

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

int special_tpid_write_proc(struct file *file, const char *buffer,
    unsigned long count, void *data)
{
	char val_string[32];
	uint16 addr = 0,tpid = 0;
	uint32 value = 0;
	int i = 0;
	char tmp = 0;
	int port, port_tpid;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if ((2 != sscanf(val_string, "%d:%x", &port, &port_tpid)) ||
		(port < 0) || (port > MT7530_SWIC_PORTNUM-1)){
		printk("Invalid param, must be like: portId[0~%d]:Tpid\n", 
		    MT7530_SWIC_PORTNUM-1);
		return -1;
	}
	tpid = port_tpid;

	/* update lan port tpid from reg & lan_port_map */
	if ((switch_port_map[port] >=0) && (switch_port_map[port]<MT7530_SWIC_PORTNUM))
		lan_port_tpid[switch_port_map[port]] = tpid;

	if (port == wan_port_id)
		ether_wan_tpid = tpid;
	else if (port == 6)
		cpu_port_tpid = tpid;

	addr = port * 0x100 + 0x2010;

	if(use_ext_switch == 1)
	{
		value = gswPbusRead(addr);
		value &= 0x0000FFFF;
		value |= (tpid << 16);
		gswPbusWrite(addr,value);
	}
	else
	{
		value = read_reg_word(GSW_BASE + addr);
		value &= 0x0000FFFF;
		value |= (tpid << 16);
		write_reg_word(GSW_BASE + addr,value);	
	}
	return count;
}

int gpon_bootflag_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index=0 ;
	off_t pos=0, begin=0 ;

	char *boot_flag = FLAG_ADDR;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	index += sprintf(buf+ index, "%d", *boot_flag) ;
	CHK_BUF() ;

	*eof = 1 ;

done:
	*start = buf + (off - begin) ;
	index -= (off - begin) ;
	if(index<0)		index = 0 ;
	if(index>count)		index = count ;
	return index ;
}

static int gpon_commitflag_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index=0 ;
	off_t pos=0, begin=0 ;

	char *commit_flag = COMMIT_FLAG_ADDR;

	if (!macInitialized) {
		*eof = 1;
		return 0;
	}

	index += sprintf(buf+ index, "%d", *commit_flag) ;
	CHK_BUF() ;

	*eof = 1 ;

done:
	*start = buf + (off - begin) ;
	index -= (off - begin) ;
	if(index<0)		index = 0 ;
	if(index>count)		index = count ;
	return index ;
}



unsigned int udp_checksum(const struct sk_buff *skb)
{
	unsigned int checksum = 0;
	unsigned int src_ip1, src_ip2, dest_ip1, dest_ip2;
	unsigned int src_port, dest_port;
	unsigned int udp_length;
	unsigned int udp_protocol;
	unsigned int data;
	unsigned int index;

	udp_protocol = (unsigned int)(*((unsigned char*)(&skb->data[23])));
	src_ip1 = (unsigned int)(*((unsigned short*)(&skb->data[26])));
	src_ip2 = (unsigned int)(*((unsigned short*)(&skb->data[28])));
	dest_ip1 = (unsigned int)(*((unsigned short*)(&skb->data[30])));
	dest_ip2 = (unsigned int)(*((unsigned short*)(&skb->data[32])));
	src_port = (unsigned int)(*((unsigned short*)(&skb->data[34])));
	dest_port = (unsigned int)(*((unsigned short*)(&skb->data[36])));
	udp_length = (unsigned int)(*((unsigned short*)(&skb->data[38])));

//	printk("src ip1: %x, src ip2: %x\n", src_ip1, src_ip2);
//	printk("dest ip1: %x, dest ip2: %x\n", dest_ip1, dest_ip2);
//	printk("src port: %x, dest port: %x\n", src_port, dest_port);
//	printk("udp_length: %x\n", udp_length);
//	printk("udp_protocol: %x\n", udp_protocol);
	data = 0;
	for (index=42; index<skb->len; index+=2){
		data += (unsigned int)(*((unsigned short*)(&skb->data[index])));
	}
//	printk("data: %x\n", data);

	checksum = src_ip1 + src_ip2 + dest_ip1 + dest_ip2 + src_port + 
	    dest_port + (2*udp_length) + data + udp_protocol;
//	printk("checksum1: %x\n", checksum);
	checksum += (checksum >> 16);
//	printk("checksum2: %x\n", checksum);
	checksum &= 0xffff;
//	printk("checksum3: %x\n", checksum);
	checksum = (~checksum);
//	printk("checksum4: %x\n", checksum);
	checksum &= 0xffff;
//	printk("checksum5: %x\n", checksum);

	return checksum;
}

int MT7530_gen_1588_packet(char flag)
{
//	int k;
	struct sk_buff *skb;
    ethTxMsg_t ethTxMsg;
    int ringIdx;

	int tx_len;
	int tx_priority;
	unsigned long checksum;
//	unsigned long sec, nsec;
	uint8 *tx_data;
	static short id=0;

	struct timespec cur_time;

	getnstimeofday(&cur_time);

	skb = dev_alloc_skb(RX_BUF_LEN);
	if (skb == NULL){
		printk("cannot allocate skb\n");
		return 0;
	}

	tx_len = 100;
	tx_data = skb_put(skb, tx_len);

	if (flag == 0)
		memcpy(tx_data, sync_pkt, 100);
	else if (flag == 1)
		memcpy(tx_data, follow_pkt, 100);
	else if (flag == 2)
		memcpy(tx_data, delay_req_pkt, 100);
	else {
		printk("error: 1588 packet type input\n");
		return 0;
	}

	tx_priority = 3;
	skb->priority = tx_priority;

	skb->data[76] = (unsigned char)(0x0);
	skb->data[77] = (unsigned char)(0x0);
	skb->data[78] = (unsigned char)((cur_time.tv_sec>>24) & 0xff);
	skb->data[79] = (unsigned char)((cur_time.tv_sec>>16) & 0xff);
	skb->data[80] = (unsigned char)((cur_time.tv_sec>>8) & 0xff);
	skb->data[81] = (unsigned char)((cur_time.tv_sec>>0) & 0xff);

	skb->data[82] = (unsigned char)((cur_time.tv_nsec>>24) & 0xff);
	skb->data[83] = (unsigned char)((cur_time.tv_nsec>>16) & 0xff);
	skb->data[84] = (unsigned char)((cur_time.tv_nsec>>8) & 0xff);
	skb->data[85] = (unsigned char)((cur_time.tv_nsec>>0) & 0xff);

	skb->data[72] = (unsigned char)((id >> 8) & 0xff);
	skb->data[73] = (unsigned char)(id & 0xff);
	id++;

	checksum = udp_checksum(skb);
	skb->data[40] = (checksum >> 8);
	skb->data[41] = (checksum & 0xff);

	ethTxMsg.msg[0]=0;
    ethTxMsg.msg[1]=0;
    ethTxMsg.raw.channel = 0;
    ethTxMsg.raw.queue = 0;
    ringIdx = 0;
	eth_lan_mac_tx(skb,lan_dev);

	mdelay(100);

	return 0;
}


int gen_1588_pkt_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];
	int flag;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	flag = simple_strtoul(val_string, NULL, 16);
#ifndef MT7505_PQA_SWITCH_TEST
	MT7530_gen_1588_packet((char)(flag));
#endif

	return count;
}


int getETHLinkSt(char *buf)
{
	uint16 index = 0;

	if (!macInitialized) {
		index += sprintf(buf+index, "Down\n");
		return index;
	}

	if ((mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_LINK_UP) == 0) {
		index += sprintf(buf+index, "Down\n");
		return index;
	}

	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_1000MB)
		index += sprintf(buf+index, "1000M/");
	else if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_100MB)
		index += sprintf(buf+index, "100M/");
	else 
		index += sprintf(buf+index, "10M/");
	if (mac_p->macPhyLinkProfile_p->enetMode & LAN_ST_FULL_DUPLEX)
		index += sprintf(buf+index, "Full Duplex\n");
	else 
		index += sprintf(buf+index, "Half Duplex\n");
	return index;
}


int eth_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getETHLinkSt(buf);
	if (len <= off+count) 
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}


#if defined (QOS_REMARKING) || defined (TCSUPPORT_HW_QOS)

static uint8 get_qos_weight(uint8 weight)
{
	if (weight <= 1)
		return MAX_WEIGHT_1023;
	else if ((weight >= 2) && (weight <= 3))
		return MAX_WEIGHT_2047;
	else if ((weight >= 4) && (weight <= 6))
		return MAX_WEIGHT_4095;
	else if (weight >= 7)
		return MAX_WEIGHT_8191;
	return MAX_WEIGHT_8191;
}

static int eth_qoswrr_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	printk("%d %d %d %d %d\n", *qos_wrr_info, *(qos_wrr_info + 1), 
	    *(qos_wrr_info + 2), *(qos_wrr_info + 3), *(qos_wrr_info + 4));
	return 0;
}

static int eth_qoswrr_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int len;
	char get_buf[32];
	uint32 reg;
	int max_wrr_val = 0, i;

	u8 port, queue;
	u32 value;
	u32 switch_reg; 
	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	/* use the copy_from_user function to copy buffer data to our get_buf */
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	/* zero terminate get_buf */
	get_buf[len]='\0';

	if(sscanf(get_buf, "%d %d %d %d %d", qos_wrr_info, (qos_wrr_info+1), 
	    (qos_wrr_info+2), (qos_wrr_info+3), (qos_wrr_info+4)) != 5)
		return count;
	
	/* find max qos wrr weight */
	for (i = 0; i < 4; i++) {
		if (max_wrr_val < qos_wrr_info[i + 1]) {
			max_wrr_val = qos_wrr_info[i + 1];
			max_prio = 3 - i;
		}
	}
	qos_wrr_user = QOS_DMAWRR_USERDEFINE;
	if(*qos_wrr_info == 0) { /*strict priority*/
		//SP mode
		for( queue = 0; queue < 4;queue++){		
		//Port n Queue x min.rate control enable		
			for( port = 0; port < 7;port++){
				switch_reg = 0x1000 + queue*0x8 + port * 0x100;
				value = switch_reg_read(GSW_BASE+switch_reg);
				value|= (1<<15);
				value&=(0xfffff080);
				switch_reg_write((GSW_BASE+switch_reg), value);
				//printf("reg :%X, val: %X\r\n",reg, value);
			}

		//enable MAX_SP_WFQ_Qx_Pn with SP mode		
			for( port = 0; port < 7;port++){
				switch_reg = 0x1000 + queue*0x8 + 0x4+ port * 0x100;
				value=switch_reg_read(GSW_BASE+switch_reg);
				value|= (1<<31);
				switch_reg_write((GSW_BASE+switch_reg), value);
				//printf("reg :%X, val: %X\r\n",reg, value);
			}
		}		
	}	
	else {  /*WRR*/

		//WRR mode
		for (queue=0; queue<4; queue++){
			
			//Port n Queue x min.rate control enable		
			for (port=0; port<7; port++){
				switch_reg = 0x1000 + queue*0x8 + port * 0x100;
				value = switch_reg_read(GSW_BASE+switch_reg);
				value &= (0xfffff080);
				value |= (1<<15);
				switch_reg_write((GSW_BASE+switch_reg), value);
				//printf("reg :%X, val: %X\r\n",reg, value);	
			}

			//enable MAX_SP_WFQ_Qx_Pn with RR mode		
			for (port=0; port<7; port++){
				switch_reg = 0x1000 + queue*0x8 + 0x4+ port * 0x100;///*		
				value = switch_reg_read(GSW_BASE+switch_reg);
				value &= (~(1<<31));
				switch_reg_write((GSW_BASE+switch_reg), value);
				//printf("reg :%X, val: %X\r\n",reg, value);	
			}
		}
		//weight
		for( queue = 0; queue < 4;queue++){
			for( port = 0; port < 7;port++){
				switch_reg = 0x1000 + queue*0x8 + 0x4+ port * 0x100;///*
				value = switch_reg_read(GSW_BASE+switch_reg);
				value &= (~(0xf<<24));
				value|= (((*(qos_wrr_info+4-queue)-1)&0xf)<<24);
				//printf("reg :%X, val: %X\r\n",reg, value);	
				switch_reg_write((GSW_BASE+switch_reg), value);
			}
		}
	}
	return len;
}
#endif



#ifdef TCSUPPORT_QOS
static int eth_tcqos_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	if (qos_flag == QOS_SW_PQ) {
		printk("qos discipline is PQ.\n");
	}
	else if (qos_flag == QOS_SW_WRR) {
		printk("qos discipline is WRR.\n");
	}
	else if (qos_flag == QOS_SW_CAR) {
		printk("qos discipline is CAR.\n");
	}
	else if (qos_flag == QOS_HW_WRR) {
		printk("qos discipline is HW WRR.\n");
	}
	else if (qos_flag == QOS_HW_PQ) {
		printk("qos discipline is HW PQ.\n");
	}
	else {
		printk("qos is disabled.\n");
	}
	return 0;
}

static int eth_tcqos_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){

	char qos_disc[10];
	int len;

	if (count > 10) {
		len = 10;
	}
	else {
		len = count;
	}
	memset(qos_disc, 0, sizeof(qos_disc));
	
	if(copy_from_user(qos_disc, buffer, len))
		return -EFAULT;

	qos_disc[len] = '\0';

	if (!strncmp(qos_disc, "PQ", 2)) {
		qos_flag = QOS_SW_PQ;
	}
	else if (!strncmp(qos_disc, "WRR", 3)) {
		qos_flag = QOS_SW_WRR;
	}
	else if (!strncmp(qos_disc, "CAR", 3)) {
		qos_flag = QOS_SW_CAR;
	}
	else if (!strncmp(qos_disc, "HWWRR", 5)) {
		qos_flag = QOS_HW_WRR;
	}
	else if (!strncmp(qos_disc, "HWPQ", 4)) {
		qos_flag = QOS_HW_PQ;
	}
	else {
		qos_flag = NULLQOS;
	}

	return len;
}
#endif



int eth_rx_receive_num_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
        off_t pos = 0;
        off_t begin = 0;
	
	index += sprintf(buf+index, "mac_receive_num %d mac_receive_threshold %d\n", 
	    mac_receive_num, mac_receive_threshold);
        CHK_BUF();

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

int eth_rx_receive_num_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	
	char tmp[16];
	int len;

	if (count > sizeof(tmp)) {
		len = sizeof(tmp);
	}
	else {
		len = count;
	}

	if(copy_from_user(tmp, buffer, len))
                return -EFAULT;

        /* zero terminate get_buf */
        tmp[len]='\0';

        if ((2 == sscanf(tmp, "%d %d", &mac_receive_num, &mac_receive_threshold)) ){
                printk("mac_receive_num %d mac_receive_threshold %d\n", 
                    mac_receive_num, mac_receive_threshold );
        }else{
                printk("Invalid input %s\n", tmp);

	}
	return count;

}

#if !defined(TCSUPPORT_CT) || defined(TCSUPPORT_CT_PON)
int eth_port_stat_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	uint32 regValue=0;
	int i =0;
	int port_num;
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	uint8 portstatus[4]={0};


    if(macSTagFlag){
	#ifdef TCSUPPORT_2PORTS
		port_num = 2;
	#else
	    port_num = 4;
	#endif
	}else{
	    port_num = 1;
	}
	if (isMT7520S){
		port_num = 1; /*only one giga port*/
	}
	
	if(swicVendor!=0){
		for (i=0; i<port_num; i++){
            if (lan_port_map[i] != -1){
                regValue = switch_reg_read(GSW_PMSR(lan_port_map[i]));
            } else {
                regValue =0;
            }

#if defined(TCSUPPORT_CUC_C5_2P)
			if(i == 3) portstatus[0] = (regValue & MAC_LINK_STS);
			else if(i == 0) portstatus[1] = (regValue & MAC_LINK_STS);
			else if(i == 1) portstatus[2] = (regValue & MAC_LINK_STS);
			else if(i == 2) portstatus[3] = (regValue & MAC_LINK_STS);
#else
    		if (g_port_reverse)
    			portstatus[port_num - i - 1] = (regValue & MAC_LINK_STS);
    		else
    			portstatus[i] = (regValue & MAC_LINK_STS);
#endif
		}	
	}
	
	index += sprintf(buf+index, "%d %d %d %d\n", 
	    portstatus[0], portstatus[1], portstatus[2], portstatus[3]);
	CHK_BUF();

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
#endif


/* support port reverse */
int port_reverse_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

	index += sprintf(buf+index, "%d\n", g_port_reverse);
	CHK_BUF();

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

int port_reverse_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	char get_buf[32];

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	
	get_buf[len]='\0';

	/*printk("get_buf is aa%s\n", get_buf);*/

	g_port_reverse = atoi(get_buf);


#if defined(TCSUPPORT_HWNAT)
 	 port_reverse = g_port_reverse;
#endif

	g_port_reverse_kernel = g_port_reverse;

	/* debug */
	/*printk("g_port_reverse is %d\n", g_port_reverse);*/
	
	return len;
}

/* support stag to vtag */
int stag_to_vtag_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

	index += sprintf(buf+index, "%d\n", g_stag_to_vtag);
	CHK_BUF();

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

int stag_to_vtag_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	char get_buf[32];

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	
	get_buf[len]='\0';

	/*printk("get_buf is aa%s\n", get_buf);*/

	g_stag_to_vtag = atoi(get_buf);

	/* debug */
	/*printk("g_port_reverse is %d\n", g_port_reverse);*/
	
	return len;
}

#if defined(CONFIG_MIPS_RT63365)
#if defined(TCPHY_SUPPORT)

int gsw_check_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

  	index += sprintf(buf+index, "%d\n", esd_has_reset);
	CHK_BUF();

	*eof = 1;
	/*printk("esd_read_value %d\n",esd_has_reset);*/

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}


int gsw_check_write_proc(struct file *file, const char *buffer,
					 	unsigned long count, void *data)
{
	char val_string[32];
	int val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	val = simple_strtoul(val_string, NULL, 10);	
	/*printk("esd_write_value %d\n",val);*/
	if(val == 1)
		esd_has_reset = 0;
	else{
		esd_has_reset = 1;
	}	

	return count;
}


#endif
#endif


#if 1/*def VPORT*/
int vport_enable_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

  	index += sprintf(buf+index, "%d\n", vport_enable);
	CHK_BUF();

	*eof = 1;
	/*printk("vport_enable_value %d\n",esd_has_reset);*/

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}

int vport_enable_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[4];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	vport_enable = simple_strtoul(val_string, NULL, 10);	
	/*printk("vport_enable_value %d\n", vport_enable);*/

	return count;
}
#endif

int ethernet_portmap_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	uint8 port;

	index += sprintf(buf+index, "%d\n", wan_port_id);
	CHK_BUF();

	index += sprintf(buf+index, "lan_port_map\n");
	CHK_BUF();
	for (port=0; port<(RT63365_SWIC_PORTNUM-1); port++){
		index += sprintf(buf+index, "%d %d\n", port, lan_port_map[port]);
		CHK_BUF();
	}

    index += sprintf(buf+index, "switch_port_map\n");
	CHK_BUF();
	for (port=0; port<(RT63365_SWIC_PORTNUM-1); port++){
		index += sprintf(buf+index, "%d %d\n", port, switch_port_map[port]);
		CHK_BUF();
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

static int protect_mode_read_proc(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int index=0 ;
	off_t pos=0, begin=0 ;

	index += sprintf(buf+ index, "protect_mode:%d\n", protect_mode) ;
	CHK_BUF() ;
	*eof = 1 ;

done:
	*start = buf + (off - begin) ;
	index -= (off - begin) ;
	if(index<0)		index = 0 ;
	if(index>count)		index = count ;
	return index ;
}

int eth_sync_mactable_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int index = 0;
	off_t pos = 0;
	off_t begin = 0;

  	index += sprintf(buf+index, "%d\n", mac_sync_en);
	CHK_BUF();

	*eof = 1;
	/*printk("vport_enable_value %d\n",esd_has_reset);*/

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0) 
		index = 0;
	if (index>count) 
		index = count;
	return index;
}

int eth_sync_mactable_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[4];

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	mac_sync_en = simple_strtoul(val_string, NULL, 10);	

	return count;
}

int eth_force_destq_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	printk("%d\n", g_force_dstq);
	return 0;
}

int eth_force_destq_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int tmp_val;
	char val_string[3];	
	if (count > sizeof(val_string) - 1){
		printk("Input Value [0/1/2]\n");
		return -EINVAL;
	}
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;	
	val_string[count] = '\0';	
	sscanf(val_string, "%d\n", &tmp_val);	
	if( (tmp_val<0) || (tmp_val>2)){
		printk("Input Value [0/1/2]\n");
		return -EFAULT;
	}
	else{
		eth_dstq_write(tmp_val);
		printk("g_force_dstq:%d \n", g_force_dstq);
	}
	return count;
}

/******************************************************************************
******************************************************************************/
int eth_proc_init()
{
    struct proc_dir_entry *eth_proc;
    
    eth_proc = create_proc_entry("tc3162/gsw_sp_tpid", 0, NULL);
    eth_proc->read_proc = special_tpid_read_proc;
    eth_proc->write_proc = special_tpid_write_proc;
  
    eth_proc = create_proc_entry("tc3162/gpon_bootflag", 0, NULL);
    eth_proc->read_proc = gpon_bootflag_read_proc;
    eth_proc = create_proc_entry("tc3162/gpon_commitflag", 0, NULL);
    eth_proc->read_proc = gpon_commitflag_read_proc;

    /* ethernet related stats */
    eth_proc = create_proc_entry("tc3162/eth_stats", 0, NULL);
    eth_proc->read_proc = eth_stats_read_proc;
    eth_proc->write_proc = eth_stats_write_proc;


    eth_proc = create_proc_entry("tc3162/gsw_stats", 0, NULL);
    eth_proc->read_proc = gsw_stats_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;

    /* wplin added 20120703 */
    eth_proc = create_proc_entry("tc3162/gsw_mib0", 0, NULL);
    eth_proc->read_proc = gsw_mib0_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;
    eth_proc = create_proc_entry("tc3162/gsw_mib1", 0, NULL);
    eth_proc->read_proc = gsw_mib1_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;
    eth_proc = create_proc_entry("tc3162/gsw_mib2", 0, NULL);
    eth_proc->read_proc = gsw_mib2_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;
    eth_proc = create_proc_entry("tc3162/gsw_mib3", 0, NULL);
    eth_proc->read_proc = gsw_mib3_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;
    eth_proc = create_proc_entry("tc3162/gsw_mib4", 0, NULL);
    eth_proc->read_proc = gsw_mib4_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;
    eth_proc = create_proc_entry("tc3162/gsw_mib5", 0, NULL);
    eth_proc->read_proc = gsw_mib5_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;
    eth_proc = create_proc_entry("tc3162/gsw_mib6", 0, NULL);
    eth_proc->read_proc = gsw_mib6_read_proc;
    eth_proc->write_proc = gsw_stats_write_proc;

    eth_proc = create_proc_entry("tc3162/gen_1588_pkt", 0, NULL);
    eth_proc->write_proc = gen_1588_pkt_write_proc;


    create_proc_read_entry("tc3162/gsw_link_st", 0, NULL, gsw_link_st_proc, NULL);

#ifdef LOOPBACK_SUPPORT
    eth_proc = create_proc_entry("tc3162/eth_loopback", 0, NULL);
    eth_proc->read_proc = eth_loopback_read_proc;
    eth_proc->write_proc = eth_loopback_write_proc;

    eth_proc = create_proc_entry("tc3162/eth_loopback_test", 0, NULL);
    eth_proc->read_proc = eth_loopback_test_read_proc;
    eth_proc->write_proc = eth_loopback_test_write_proc;

    eth_proc = create_proc_entry("tc3162/vlantable_test", 0, NULL);
    eth_proc->write_proc = vlantable_test;
#endif

    create_proc_read_entry("tc3162/eth_link_st", 0, NULL, eth_link_st_proc, NULL);

    eth_proc = create_proc_entry("tc3162/port_reverse", 0, NULL);
    eth_proc->read_proc = port_reverse_read_proc;
    eth_proc->write_proc = port_reverse_write_proc;
    
    eth_proc = create_proc_entry("tc3162/stag_to_vtag", 0, NULL);
    eth_proc->read_proc = stag_to_vtag_read_proc;
    eth_proc->write_proc = stag_to_vtag_write_proc;


    /*vport enable/disable control*/
    eth_proc = create_proc_entry("tc3162/vport_enable", 0, NULL);
    if(eth_proc){
        eth_proc->read_proc = vport_enable_read_proc;
        eth_proc->write_proc = vport_enable_write_proc;
    }
    eth_proc = create_proc_entry("tc3162/eth_rx_packet_number", 0, NULL);
    if(eth_proc){
            eth_proc->read_proc = eth_rx_receive_num_read_proc;
            eth_proc->write_proc = eth_rx_receive_num_write_proc;
    }   
    eth_proc = create_proc_entry("tc3162/eth_portmap", 0, NULL);
    eth_proc->read_proc = ethernet_portmap_read_proc;
    
#ifdef TCPHY_SUPPORT
#if !defined(TCSUPPORT_CT) || defined(TCSUPPORT_CT_PON)
  	create_proc_read_entry("tc3162/eth_port_status", 0, NULL, 
  	    eth_port_stat_read_proc, NULL);
#endif
#endif

#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
        eth_proc = create_proc_entry("tc3162/eth1_qoswrr", 0, NULL);
        eth_proc->read_proc = eth_qoswrr_read_proc;
        eth_proc->write_proc = eth_qoswrr_write_proc;
#endif
    
#ifdef TCSUPPORT_QOS
        eth_proc = create_proc_entry("tc3162/eth1_tcqos_disc", 0, NULL);
        eth_proc->read_proc = eth_tcqos_read_proc;
        eth_proc->write_proc = eth_tcqos_write_proc;
#endif
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
		dev_bandwidth_func_init();
#endif

	eth_proc = create_proc_entry("tc3162/acl_rule", 0, NULL);
	eth_proc->read_proc = acl_rule_read_proc;
	eth_proc->write_proc = acl_rule_write_proc;

	eth_proc = create_proc_entry("tc3162/eth_sync_mactable", 0, NULL);
	eth_proc->read_proc = eth_sync_mactable_read_proc;
	eth_proc->write_proc = eth_sync_mactable_write_proc;

	eth_proc = create_proc_entry("tc3162/eth_force_destq",0, NULL);
	eth_proc->read_proc = eth_force_destq_read_proc;
	eth_proc->write_proc = eth_force_destq_write_proc;
    /*
    tc3162/gsw_mtu
    tc3162/mac_esd_check
    tc3162/protect_mode
    tc3162/cache_line_test
    tc3162/fe_reset
    tc3162/service_change_reset
    tc3162/eth_forward
    tc3162/avalanche_test
    */
    
}

int eth_proc_exit()
{    
    remove_proc_entry("tc3162/gsw_sp_tpid", 0);
    remove_proc_entry("tc3162/gpon_bootflag", 0);
    remove_proc_entry("tc3162/gpon_commitflag", 0);
    remove_proc_entry("tc3162/eth_stats", 0);
    remove_proc_entry("tc3162/gsw_stats", 0);
    remove_proc_entry("tc3162/gsw_link_st", 0);
#ifdef LOOPBACK_SUPPORT
    remove_proc_entry("tc3162/eth_loopback", 0);
    remove_proc_entry("tc3162/eth_loopback_test", 0);
    remove_proc_entry("tc3162/vlantable_test", 0);
#endif
    
    remove_proc_entry("tc3162/gen_1588_pkt", 0);

    remove_proc_entry("tc3162/gsw_mib0", 0);
    remove_proc_entry("tc3162/gsw_mib1", 0);
    remove_proc_entry("tc3162/gsw_mib2", 0);
    remove_proc_entry("tc3162/gsw_mib3", 0);
    remove_proc_entry("tc3162/gsw_mib4", 0);
    remove_proc_entry("tc3162/gsw_mib5", 0);
    remove_proc_entry("tc3162/gsw_mib6", 0);

    
    remove_proc_entry("tc3162/eth_link_st", 0);
    remove_proc_entry("tc3162/port_reverse", 0);
    remove_proc_entry("tc3162/stag_to_vtag", 0);
    remove_proc_entry("tc3162/vport_enable", 0);
    remove_proc_entry("tc3162/eth_rx_packet_number", 0);
    remove_proc_entry("tc3162/eth_portmap", 0);
#ifdef TCPHY_SUPPORT
#if !defined(TCSUPPORT_CT) || defined(TCSUPPORT_CT_PON)
    remove_proc_entry("tc3162/eth_port_status", 0);
#endif
#endif

#if defined(QOS_REMARKING) || defined(TCSUPPORT_HW_QOS)
    remove_proc_entry("tc3162/eth1_qoswrr", 0);
#endif

#ifdef TCSUPPORT_QOS
    remove_proc_entry("tc3162/eth1_tcqos_disc", 0);
#endif
    
    remove_proc_entry("tc3162/protect_mode", 0);
    remove_proc_entry("tc3162/eth_sync_mactable", 0);
    remove_proc_entry("tc3162/eth_force_destq", 0);
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
    dev_bandwidth_func_deinit();
#endif

    return 0;
}
