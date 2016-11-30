/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
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
#include <linux/module.h>
#include <asm/tc3162/tc3162.h>
#include <linux/netdevice.h>
#include <ecnt_hook/ecnt_hook_qdma.h>
#include <ecnt_hook/ecnt_hook_ppe.h>
#include "fe_reg_en7512.h"
#include "fe.h"
#include "fe_api.h"

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
unsigned int flowByteCnt_HW[8] = {0};

/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define FE_VIP_TYPE_ETHER		0
#define FE_VIP_TYPE_PPPOE 		1
#define FE_VIP_TYPE_IP 			2
#define FE_VIP_TYPE_TCP 		3
#define FE_VIP_TYPE_UDP 		4

#define FE_VIP_PACKET_NUM  		32
#define FE_L2LU_KEY_NUM 		15

#define	UP_QUEUE_CNT_START		10
#define FLOW_NUMBER			8
/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

typedef struct fe_vip_packet_s{
	unsigned short en: 1;
	unsigned short type: 3;
	unsigned short sp_en: 1;
	unsigned short dp_en: 1;
	unsigned short sw_en: 1;
	unsigned short res: 9;
	union{	
		unsigned short sport;
		unsigned short etype;
		unsigned short ppp;
		unsigned short iptype;
	};
	unsigned short dport;	
} fe_vip_packet;
	
typedef struct fe_l2lu_key_s{
	unsigned int dscp_mask: 8;
	unsigned int cvid_en: 1;
	unsigned int cpcp_en: 1;
	unsigned int res0: 2;
	unsigned int svid_en: 1;
	unsigned int spcp_en: 1;
	unsigned int res1: 2;	 
	unsigned int dscp: 16;
	unsigned int cvid: 12;
	unsigned int res2: 1;	
	unsigned int cpcp: 3;
	unsigned int svid: 12;
	unsigned int res3: 1;	 
	unsigned int spcp: 3;	
} fe_l2lu_key;

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
int fe_api_set_pkt_length(struct ecnt_fe_data *fe_data);
int fe_api_set_channel_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_mac_addr(struct ecnt_fe_data *fe_data);
int fe_get_hwfwd_channel(struct ecnt_fe_data *fe_data);
int fe_set_hwfwd_channel(struct ecnt_fe_data *fe_data);
int fe_api_set_channel_retire(struct ecnt_fe_data *fe_data);
int fe_api_set_crc_strip(struct ecnt_fe_data *fe_data);
int fe_api_set_padding(struct ecnt_fe_data *fe_data);
int fe_api_get_ext_tpid(struct ecnt_fe_data *fe_data);
int fe_api_set_ext_tpid(struct ecnt_fe_data *fe_data);
int fe_api_get_fw_cfg(struct ecnt_fe_data *fe_data);
int fe_api_set_fw_cfg(struct ecnt_fe_data *fe_data);
int fe_api_set_drop_udp_chksum_err_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_drop_tcp_chksum_err_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_drop_ip_chksum_err_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_drop_runt_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_drop_long_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_drop_crc_err_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_vlan_check(struct ecnt_fe_data *fe_data);
int fe_api_get_ok_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_rx_err_crc_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_rx_drop_fifo_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_rx_drop_err_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_ok_byte_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_tx_get_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_tx_drop_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_time_stamp(struct ecnt_fe_data *fe_data);
int fe_api_set_time_stamp(struct ecnt_fe_data *fe_data);
int fe_api_set_ins_vlan_tpid(struct ecnt_fe_data *fe_data);
int fe_api_set_vlan_enable(struct ecnt_fe_data *fe_data);
int fe_api_set_black_list(struct ecnt_fe_data *fe_data);
int fe_api_set_ether_type(struct ecnt_fe_data *fe_data);
int fe_api_set_L2U_key(struct ecnt_fe_data *fe_data);
int fe_api_get_ac_group_ptk_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_ac_group_byte_cnt(struct ecnt_fe_data *fe_data);
int fe_api_set_meter_group(struct ecnt_fe_data *fe_data);
int fe_api_get_meter_group(struct ecnt_fe_data *fe_data);
int fe_api_set_gdm_pcp_coding(struct ecnt_fe_data *fe_data);
int fe_api_set_cdm_pcp_coding(struct ecnt_fe_data *fe_data);
int fe_api_set_vip_enable(struct ecnt_fe_data *fe_data);
int fe_api_get_eth_rx_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_eth_tx_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_eth_frame_cnt(struct ecnt_fe_data *fe_data);
int fe_api_get_eth_err_cnt(struct ecnt_fe_data *fe_data);
int fe_api_set_clear_mib(struct ecnt_fe_data *fe_data);
int fe_api_set_cdm_rx_red_drop_mode(struct ecnt_fe_data *fe_data);
int fe_api_get_cdm_rx_red_drop_mode(struct ecnt_fe_data *fe_data);
int fe_api_set_channel_retire_all(struct ecnt_fe_data *fe_data);
int fe_api_set_channel_retire_one(struct ecnt_fe_data *fe_data);
int fe_api_set_tx_rate(struct ecnt_fe_data *fe_data);
int fe_api_set_rxuc_rate(struct ecnt_fe_data *fe_data);
int fe_api_set_rxbc_rate(struct ecnt_fe_data *fe_data);
int fe_api_set_rxmc_rate(struct ecnt_fe_data *fe_data);
int fe_api_set_rxoc_rate(struct ecnt_fe_data *fe_data);
int fe_api_add_vip_ether(struct ecnt_fe_data *fe_data);
int fe_api_add_vip_ppp(struct ecnt_fe_data *fe_data);
int fe_api_add_vip_ip(struct ecnt_fe_data *fe_data);
int fe_api_add_vip_tcp(struct ecnt_fe_data *fe_data);
int fe_api_add_vip_udp(struct ecnt_fe_data *fe_data);
int fe_api_del_vip_ether(struct ecnt_fe_data *fe_data);
int fe_api_del_vip_ppp(struct ecnt_fe_data *fe_data);
int fe_api_del_vip_ip(struct ecnt_fe_data *fe_data);
int fe_api_del_vip_tcp(struct ecnt_fe_data *fe_data);
int fe_api_del_vip_udp(struct ecnt_fe_data *fe_data);
int fe_api_add_l2lu_vlan_dscp(struct ecnt_fe_data *fe_data);
int fe_api_add_l2lu_vlan_trfc(struct ecnt_fe_data *fe_data);
int fe_api_del_l2lu_vlan_dscp(struct ecnt_fe_data *fe_data);
int fe_api_del_l2lu_vlan_trfc(struct ecnt_fe_data *fe_data);
int fe_api_set_tx_favor_oam_enable(struct ecnt_fe_data *fe_data);

/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
/*
  *spinlock_t hookFuncLock[FE_FUNCTION_MAX_NUM] ;
 */

typedef int (*fe_api_op_t)(struct ecnt_fe_data *fe_data);

/* Warning: same sequence with enum 'FE_HookFunctionID_t' in ecnt_hook_fe.h */
fe_api_op_t
fe_operation[]=
{
	fe_api_set_pkt_length,
	fe_api_set_channel_enable,
	fe_api_set_mac_addr,
	fe_set_hwfwd_channel,
	fe_api_set_channel_retire,
	fe_api_set_crc_strip,
	fe_api_set_padding,
	fe_api_get_ext_tpid,
	fe_api_set_ext_tpid,
	fe_api_get_fw_cfg,
	fe_api_set_fw_cfg,
	fe_api_set_drop_udp_chksum_err_enable,
	fe_api_set_drop_tcp_chksum_err_enable,
	fe_api_set_drop_ip_chksum_err_enable,
	fe_api_set_drop_crc_err_enable,
	fe_api_set_drop_runt_enable,
	fe_api_set_drop_long_enable,
	fe_api_set_vlan_check,
	fe_api_get_ok_cnt,
	fe_api_get_rx_err_crc_cnt,
	fe_api_get_rx_drop_fifo_cnt,
	fe_api_get_rx_drop_err_cnt,
	fe_api_get_ok_byte_cnt,
	fe_api_get_tx_get_cnt,
	fe_api_get_tx_drop_cnt,
	fe_api_get_time_stamp,
	fe_api_set_time_stamp,
	fe_api_set_ins_vlan_tpid,
	fe_api_set_vlan_enable,
	fe_api_set_black_list,
	fe_api_set_ether_type,
	fe_api_set_L2U_key,
	fe_api_get_ac_group_ptk_cnt,
	fe_api_get_ac_group_byte_cnt,
	fe_api_set_meter_group,
	fe_api_get_meter_group,
	fe_api_set_gdm_pcp_coding,
	fe_api_set_cdm_pcp_coding,
	fe_api_set_vip_enable,
	fe_api_get_eth_rx_cnt,
	fe_api_get_eth_tx_cnt,
	fe_api_get_eth_frame_cnt,
	fe_api_get_eth_err_cnt,
	fe_api_set_clear_mib,
	fe_api_set_cdm_rx_red_drop_mode,
	fe_api_get_cdm_rx_red_drop_mode,
	fe_api_set_channel_retire_all,
	fe_api_set_channel_retire_one,
	fe_api_set_tx_rate,
	fe_api_set_rxuc_rate,
	fe_api_set_rxbc_rate,
	fe_api_set_rxmc_rate,
	fe_api_set_rxoc_rate,
	fe_api_add_vip_ether,
	fe_api_add_vip_ppp,
	fe_api_add_vip_ip,
	fe_api_add_vip_tcp,
	fe_api_add_vip_udp,
	fe_api_del_vip_ether,
	fe_api_del_vip_ppp,
	fe_api_del_vip_ip,
	fe_api_del_vip_tcp,
	fe_api_del_vip_udp,	
	fe_api_add_l2lu_vlan_dscp,
	fe_api_add_l2lu_vlan_trfc,
	fe_api_del_l2lu_vlan_dscp,
	fe_api_del_l2lu_vlan_trfc,
	fe_api_set_tx_favor_oam_enable
};

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
unsigned int fe_reg_read(unsigned int reg_offset){
	return read_reg_word(FE_BASE + reg_offset);

}

void fe_reg_write(unsigned int reg_offset, unsigned int value){
	write_reg_word((FE_BASE + reg_offset), value);
	return;
}

void fe_reg_modify_bits(unsigned int reg_offset, unsigned int Data, unsigned int Offset, unsigned int Len)
{
	unsigned int Mask = 0;
	unsigned int Value;
	unsigned int i;

	for (i = 0; i < Len; i++) {
		Mask |= 1 << (Offset + i);
	}

	Value = fe_reg_read(reg_offset);
	Value &= ~Mask;
	Value |= (Data << Offset) & Mask;;

	fe_reg_write(reg_offset, Value);
}

ecnt_ret_val ecnt_fe_api_hook(struct ecnt_data *in_data)
{
	struct ecnt_fe_data *fe_data = (struct ecnt_fe_data *)in_data ;
	/* ulong flags = 0 ; */
	
	if(fe_data->function_id >= FE_FUNCTION_MAX_NUM) {
		printk("fe_data->function_id is %d, exceed max number: %d", fe_data->function_id, FE_FUNCTION_MAX_NUM);
		return ECNT_HOOK_ERROR;
	}

	/* spin_lock_irqsave(&hookFuncLock[fe_data->function_id], flags) ; */
	fe_data->retValue = fe_operation[fe_data->function_id](fe_data) ;
	/* spin_unlock_irqrestore(&hookFuncLock[fe_data->function_id], flags) ; */
	
	return ECNT_CONTINUE;
}

int fe_api_set_pkt_length(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	uint length_long = fe_data->api_data.pkt_len.length_long;
	uint length_short = fe_data->api_data.pkt_len.length_short;

	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_LEN_CFG;
	}else{
		base_addr = GDMA2_LEN_CFG;
	}
	
	val = read_reg_word(base_addr);
	if(length_long != 0)
		val &= ~(0xffff<<16);
	if(length_short != 0)
		val &= ~(0xffff);
	
	val |= ((length_long << 16) | length_short);
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_channel_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_TXRX_Sel_t txrx_sel = fe_data->txrx_sel;
	FE_Enable_t enable = fe_data->api_data.enable;
	uint channel = fe_data->channel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		if(txrx_sel == FE_GDM_SEL_TX)
			base_addr = GDMA1_TXCHN_EN;
		else
			base_addr = GDMA1_RXCHN_EN;
	}else{
		if(txrx_sel == FE_GDM_SEL_TX)
			base_addr = GDMA2_TXCHN_EN;
		else
			base_addr = GDMA2_RXCHN_EN;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<channel);
	}else{
		val |= (1<<channel);
	}
	write_reg_word(base_addr, val);
	
	return 0;
}

int fe_api_set_mac_addr(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr_lsb, base_addr_msb, val;
	unchar *mac = fe_data->api_data.mac_addr.mac;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	int mask = fe_data->api_data.mac_addr.mask;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr_lsb = GDMA1_MAC_ADRL;
		base_addr_msb = GDMA1_MAC_ADRH;
	}else{
		base_addr_lsb = GDMA2_MAC_ADRL;
		base_addr_msb = GDMA2_MAC_ADRH;
	}

	val = (mac[2]<<24) | (mac[3]<<16) | (mac[4]<<8) | (mac[5]<<0);
	write_reg_word(base_addr_lsb, val);
	val = (mask<<16) | (mac[0]<<8) | (mac[1]<<0);
	write_reg_word(base_addr_msb, val);
	return 0;
}

int fe_get_hwfwd_channel(struct ecnt_fe_data *fe_data)
{
	unsigned int	reg, value ;
	FE_Cdma_Sel_t	cdm_sel = fe_data->cdm_sel;
	uint			channel = fe_data->channel;

	if (cdm_sel == FE_CDM_SEL_CDMA1){
		reg = CDMA1_HWF_CHN_EN;
	}else if(cdm_sel == FE_CDM_SEL_CDMA2){
		reg = CDMA2_HWF_CHN_EN;
	}else{
		return -1;
	}

	value = read_reg_word(reg);

	value &= (1 << channel);

	fe_data->api_data.enable = (value >> channel);

	return 0;
}

int fe_set_hwfwd_channel(struct ecnt_fe_data *fe_data)
{
	FE_Cdma_Sel_t	cdm_sel = fe_data->cdm_sel;
	uint			channel = fe_data->channel;
	FE_Enable_t		enable = fe_data->api_data.enable;
	unsigned int	reg, value ;

	if (cdm_sel == FE_CDM_SEL_CDMA1){
		reg = CDMA1_HWF_CHN_EN;
	}else if(cdm_sel == FE_CDM_SEL_CDMA2){
		reg = CDMA2_HWF_CHN_EN;
	}else{
		return -1;
	}

	value = read_reg_word(reg);

	value &= ~(1 << channel);

	value |= (enable << channel);
	
	write_reg_word(reg, value);

	return 0;
}

static int fe_channel_retire_one(FE_Gdma_Sel_t idx,uint chn)
{
	unsigned int reg,reg2,value,retry = 0, probe;
	int ret = 0;

	if (idx == FE_GDM_SEL_GDMA1){
		reg = GDMA1_CHN_RLS;
		reg2 = GDMA1_TX_CHN_VLD;
		probe = 0x160001;
	}else if(idx == FE_GDM_SEL_GDMA2){
		reg = GDMA2_CHN_RLS;
		reg2= GDMA2_TX_CHN_VLD;
		probe = 0xb60001;
	}else{
		return -1;
	}	

	chn &= 0x1f;
	
	value = (chn << GDMA_CHN_RLS_CHN_OFFSET) | (1 << GDMA_CHN_RLS_EN_OFFSET);
	
	write_reg_word(reg,value);
	
retry:
	
	mdelay(1);
	
	retry++;

	if (retry < GDMA_CHN_RLS_TIMEOUT)
	{
		for(value = 0; value < 10; value++)
		{
			write_reg_word(SCU_PROBE_MODE_CFG,probe);
			if ( (read_reg_word(reg) & (1 << GDMA_CHN_RLS_STAT_OFFSET)) == 0 
				|| (read_reg_word(reg2) & (1 << chn)) != 0 
				|| (read_reg_word(FE_PROBE_H) & (1 << chn)) == 0 ) {
				goto retry;
			}
		}
	}else{
		ret = -1;			
	}

	write_reg_word(reg,0);

	return ret;
}


/*
    used when wan link is down,and called by Wan MAC Driver.  this function will retire each channel twice by two loops. First loop retire the 
    channels queued in PSE buffer, these channels will be retired done at first loop. Second loop will given the chance to those channels not 
    queued in PSE buffer but queued in QDMA TX queue. All Channels will be retired done at second loop. 
*/
int fe_api_set_channel_retire_all(struct ecnt_fe_data *fe_data)
{
	unsigned int txreg,rxreg,txchn,rxchn;
	unsigned int hwreg,qdmareg,hw,qdma,i,j;
	QDMA_TxBufCtrl_T oldtxbuff,newTxbuff;
	unsigned int reg, reg2;
	int ret;
	
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	uint channel = fe_data->channel;

	if (gdm_sel == FE_GDM_SEL_GDMA1){
		txreg = GDMA1_TXCHN_EN;
		rxreg = GDMA1_RXCHN_EN;
		hwreg = CDMA1_HWF_CHN_EN;
		reg = GDMA1_CHN_RLS;
		reg2 = GDMA1_TX_CHN_VLD;
	}else if(gdm_sel == FE_GDM_SEL_GDMA2){
		txreg = GDMA2_TXCHN_EN;
		rxreg = GDMA2_RXCHN_EN;	
		hwreg = CDMA2_HWF_CHN_EN;	
		reg = GDMA2_CHN_RLS;
		reg2= GDMA2_TX_CHN_VLD;
	}else{
		return -1;
	}	

	txchn = read_reg_word(txreg);
	
	rxchn = read_reg_word(rxreg);
	
	hw = read_reg_word(hwreg);
	
	QDMA_API_GET_TXBUF_THRESHOLD(ECNT_QDMA_WAN,&oldtxbuff);

	write_reg_word(hwreg,0);

	newTxbuff.mode = QDMA_ENABLE;
	newTxbuff.chnThreshold = 1;
	newTxbuff.totalThreshold = 0x40;
	QDMA_API_SET_TXBUF_THRESHOLD(ECNT_QDMA_WAN,&newTxbuff);

	write_reg_word(rxreg,0);
	
	write_reg_word(txreg,0);
	
	mdelay(1);

	j = 0;
	while(j < 2)
	{
		for(i = 0; i < channel; i++)
		{
			ret = fe_channel_retire_one(gdm_sel,i);
			if(ret == -1 && j == 1) {
				printk("fe_channel_retire_one(%d/%d): timeout (%08x) ,(%08x) ,(%08x) \n",i,gdm_sel,read_reg_word(reg),read_reg_word(reg2),read_reg_word(FE_PROBE_H));
			}
		}
		j++;
	}
	
	write_reg_word(txreg,txchn);
	
	write_reg_word(rxreg,rxchn);
	
	QDMA_API_SET_TXBUF_THRESHOLD(ECNT_QDMA_WAN,&oldtxbuff);

	write_reg_word(hwreg,hw);

	return 0;
}

int fe_api_set_channel_retire_one(struct ecnt_fe_data *fe_data)
{
	unsigned int hwreg,qdmareg,hw,qdma;
	QDMA_TxBufCtrl_T oldtxbuff,newTxbuff;
	
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	uint channel = fe_data->channel;
	 
	if (gdm_sel == FE_GDM_SEL_GDMA1){
		hwreg = CDMA1_HWF_CHN_EN;
	}else if(gdm_sel == FE_GDM_SEL_GDMA2){
		hwreg = CDMA2_HWF_CHN_EN;		
	}else{
		return -1;
	}
	
	hw = read_reg_word(hwreg);
	
	QDMA_API_GET_TXBUF_THRESHOLD(ECNT_QDMA_WAN,&oldtxbuff);	
	write_reg_word(hwreg,hw & (~(1<<channel)) );
	
	newTxbuff.mode = QDMA_ENABLE;
	newTxbuff.chnThreshold = 1;
	newTxbuff.totalThreshold = 0x40;
	QDMA_API_SET_TXBUF_THRESHOLD(ECNT_QDMA_WAN,&newTxbuff);

	mdelay(1);	

	fe_channel_retire_one(gdm_sel,channel);
	
	QDMA_API_SET_TXBUF_THRESHOLD(ECNT_QDMA_WAN,&oldtxbuff);

	write_reg_word(hwreg,hw);

	return 0;

}
int fe_api_set_channel_retire(struct ecnt_fe_data *fe_data)
{
	unsigned int reg,reg2,value,mask,ret = 0;
	unsigned int txreg,rxreg,txchn,rxchn;
	FE_Enable_t rxHwfwd;
	struct ecnt_fe_data cdm_fe_data;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	uint channel = fe_data->channel;
	uint mode = fe_data->reg_val;
	 
	if (gdm_sel == FE_GDM_SEL_GDMA1){
		reg = GDMA1_CHN_RLS;
		reg2 = GDMA1_TX_CHN_VLD;
		txreg = GDMA1_TXCHN_EN;
		rxreg = GDMA1_RXCHN_EN;
	}else if(gdm_sel == FE_GDM_SEL_GDMA2){
		reg = GDMA2_CHN_RLS;
		reg2= GDMA2_TX_CHN_VLD;
		txreg = GDMA2_TXCHN_EN;
		rxreg = GDMA2_RXCHN_EN;		
	}else{
		return -1;
	}	
	
	mask = read_reg_word(reg2);

	if ( ((1<< channel)& mask) == 0)
		return 0;
	
	if (gdm_sel == FE_GDM_SEL_GDMA1) {
		cdm_fe_data.cdm_sel = FE_CDM_SEL_CDMA1;
		cdm_fe_data.channel = channel;
		
		fe_get_hwfwd_channel(&cdm_fe_data);
		rxHwfwd = cdm_fe_data.api_data.enable;

		cdm_fe_data.api_data.enable = FE_DISABLE;
		fe_set_hwfwd_channel(&cdm_fe_data);
	}
	else {
		cdm_fe_data.cdm_sel = FE_CDM_SEL_CDMA2;
		cdm_fe_data.channel = channel;
		
		fe_get_hwfwd_channel(&cdm_fe_data);
		rxHwfwd = cdm_fe_data.api_data.enable;
		
		cdm_fe_data.api_data.enable = FE_DISABLE;
		fe_set_hwfwd_channel(&cdm_fe_data);
	}

	if (mode == FE_LINKDOWN){
		txchn = read_reg_word(txreg);
		rxchn = read_reg_word(rxreg);
		write_reg_word(txreg,0);
		write_reg_word(rxreg,0);
	}

	value = (channel << GDMA_CHN_RLS_CHN_OFFSET) | (1 << GDMA_CHN_RLS_EN_OFFSET);
	
	write_reg_word(reg,value);

	mdelay(1);
	
	value = 0;
		
	while( ( (read_reg_word(reg) & (1 << GDMA_CHN_RLS_STAT_OFFSET)) == 0 
				||  (read_reg_word(reg2) & (1 << channel)) != 0 )
				&& (value++ < GDMA_CHN_RLS_TIMEOUT)){
		mdelay(1);
	}
	
	if (value >= GDMA_CHN_RLS_TIMEOUT){
		printk("fe_api_set_channel_retire: timeout \n");
		ret = -1;
	}

	write_reg_word(reg,0);
	
	if (mode == FE_LINKDOWN){
		write_reg_word(txreg,txchn);
		write_reg_word(rxreg,rxchn);
	}
	
	if (gdm_sel == FE_GDM_SEL_GDMA1) {
		cdm_fe_data.cdm_sel = FE_CDM_SEL_CDMA1;
		cdm_fe_data.channel = channel;
		cdm_fe_data.api_data.enable = rxHwfwd;
		fe_set_hwfwd_channel(&cdm_fe_data);
	} else {
		cdm_fe_data.cdm_sel = FE_CDM_SEL_CDMA2;
		cdm_fe_data.channel = channel;
		cdm_fe_data.api_data.enable = rxHwfwd;
		fe_set_hwfwd_channel(&cdm_fe_data);
	}
	
	return ret;

}

int fe_api_set_crc_strip(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_Enable_t enable = fe_data->api_data.enable;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<16);
	}else{
		val |= (1<<16);
	}
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_padding(struct ecnt_fe_data *fe_data)
{
    unsigned long int addr, val;
    FE_Enable_t enable = fe_data->api_data.enable;
    
    addr = FE_CPORT_CFG;
    val = read_reg_word(addr);
    if(enable == FE_DISABLE){
        val &= ~(1<<26);
    }else{
        val |= (1<<26);
    }
    write_reg_word(addr, val);
    return 0;
}

int fe_api_get_ext_tpid(struct ecnt_fe_data *fe_data)
{
    fe_data->reg_val = (read_reg_word(FE_DMA_GLO_CFG)>>16) & 0xffff;
    return 0;
}

int fe_api_set_ext_tpid(struct ecnt_fe_data *fe_data)
{
	unsigned long int addr, val;
	uint tpid = fe_data->reg_val;
	
    addr = FE_DMA_GLO_CFG;
    val = read_reg_word(addr);
    
    val &= ~(0xffff<<16);
    val |= (tpid<<16);
    write_reg_word(addr, val);
    return 0;
}

int fe_api_get_fw_cfg(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;

	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	fe_data->reg_val = read_reg_word(base_addr);
	return 0;
}

int fe_api_set_fw_cfg(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, offset, mask, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_Frame_type_t dp_sel = fe_data->api_data.fw_cfg.dp_sel;
	FE_Frame_dp_t dp_type = fe_data->api_data.fw_cfg.dp_val;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}
	if(dp_sel == FE_TYPE_OC){
		offset = GDM_OFRC_P_SHIFT;
		mask = GDM_OFRC_P;
	}else if(dp_sel == FE_TYPE_MC){
		offset = GDM_MFRC_P_SHIFT;
		mask = GDM_MFRC_P;
	}else if(dp_sel == FE_TYPE_BC){
		offset = GDM_BFRC_P_SHIFT;
		mask = GDM_BFRC_P;
	}else if(dp_sel == FE_TYPE_UC){
		offset = GDM_UFRC_P_SHIFT;
		mask = GDM_UFRC_P;
	}
	val = read_reg_word(base_addr);
	val &= ~(mask);
	val |= (dp_type << offset);

	write_reg_word(base_addr, val);
	
	return 0;
}

int fe_api_set_drop_udp_chksum_err_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;	
	FE_Enable_t enable = fe_data->api_data.enable;

	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<20);
	}else{
		val |= (1<<20);
	}
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_drop_tcp_chksum_err_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;	
	FE_Enable_t enable = fe_data->api_data.enable;

	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<21);
	}else{
		val |= (1<<21);
	}
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_drop_ip_chksum_err_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;	
	FE_Enable_t enable = fe_data->api_data.enable;

	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<22);
	}else{
		val |= (1<<22);
	}
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_drop_crc_err_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_Enable_t enable = fe_data->api_data.enable;

	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<23);
	}else{
		val |= (1<<23);
	}
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_drop_runt_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_Enable_t enable = fe_data->api_data.enable;

	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<24);
	}else{
		val |= (1<<24);
	}
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_drop_long_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_Enable_t enable = fe_data->api_data.enable;

	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_FWD_CFG;
	}else{
		base_addr = GDMA2_FWD_CFG;
	}

	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
		val &= ~(1<<25);
	}else{
		val |= (1<<25);
	}
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_vlan_check(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_Enable_t enable = fe_data->api_data.enable;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_VLAN_CHECK;
	}else{
		base_addr = GDMA2_VLAN_CHECK;
	}
	
	write_reg_word(base_addr, enable);

	return 0;
}

int fe_api_get_ok_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_TXRX_Sel_t txrx_sel = fe_data->txrx_sel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
        if (txrx_sel == FE_GDM_SEL_TX)
		    base_addr = GDMA1_TX_OK_CNT;
        else
            base_addr = GDMA1_RX_OK_CNT;
	}else{
        if (txrx_sel == FE_GDM_SEL_TX)
		    base_addr = GDMA2_TX_OKCNT;
        else
            base_addr = GDMA2_RX_OKCNT;
	}

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_get_rx_err_crc_cnt(struct ecnt_fe_data *fe_data)
{
	read_reg_word(GDMA2_RX_ETHCRCCNT);
	return 0;
}

int fe_api_get_rx_drop_fifo_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_RX_OVER_DROP_CNT;
	}else{
		base_addr = GDMA2_RX_OVDROPCNT;
	}

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_get_rx_drop_err_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_RX_ERROR_DROP_CNT;
	}else{
		base_addr = GDMA2_RX_ERRDROPCNT;
	}

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_get_ok_byte_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_TXRX_Sel_t txrx_sel = fe_data->txrx_sel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
        if (txrx_sel == FE_GDM_SEL_TX)
		    base_addr = GDMA1_TX_OK_BYTE_CNT;
        else
            base_addr = GDMA1_RX_BYTECNT;
	}else{
        if (txrx_sel == FE_GDM_SEL_TX)
		    base_addr = GDMA2_TX_OKBYTE_CNT;
        else
            base_addr = GDMA2_RX_OKBYTECNT;
	}

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_get_tx_get_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_TX_GET_CNT;
	}else{
		base_addr = GDMA2_TX_GETCNT;
	}

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_get_tx_drop_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_TX_DROP_CNT;
	}else{
		base_addr = GDMA2_TX_DROPCNT;
	}

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_get_time_stamp(struct ecnt_fe_data *fe_data)
{
	fe_data->cnt = read_reg_word(FE_FOE_TS) & 0xFFFF;
	return 0;
}

int fe_api_set_time_stamp(struct ecnt_fe_data *fe_data)
{
	write_reg_word(FE_FOE_TS, fe_data->timeStamp & 0xFFFF);
	return 0;
}
int fe_api_set_ins_vlan_tpid(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Cdma_Sel_t cdm_sel = fe_data->cdm_sel;
	uint tpid = fe_data->reg_val;
	
	if(cdm_sel == FE_CDM_SEL_CDMA1){
		base_addr = CDMA1_VLAN_CTRL;
	}else{
		base_addr = CDMA2_VLAN_CTRL;
	}

	val = read_reg_word(base_addr);
    val &= ~(INS_VLAN);
    val |= (tpid<<INS_VLAN_SHIFT);
    write_reg_word(base_addr, val);
   
	return 0;
}

int fe_api_set_vlan_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val;
	FE_Cdma_Sel_t cdm_sel = fe_data->cdm_sel;
	FE_Enable_t enable = fe_data->api_data.enable;
	
	if(cdm_sel == FE_CDM_SEL_CDMA1){
		base_addr = CDMA1_VLAN_CTRL;
	}else{
		base_addr = CDMA2_VLAN_CTRL;
	}
	
	val = read_reg_word(base_addr);
	if(enable == FE_DISABLE){
        val &= ~(1<<1);
    }else{
        val |= (1<<1);
    }
	write_reg_word(base_addr, val);

	return 0;
}

int fe_api_set_black_list(struct ecnt_fe_data *fe_data)
{
	FE_Enable_t enable = fe_data->api_data.enable;
	write_reg_word(L2BR_CFG, enable);
	return 0;
}

int fe_api_set_ether_type(struct ecnt_fe_data *fe_data)
{
	unsigned long int val, type;
	uint index = fe_data->index;
	FE_Enable_t enable = fe_data->api_data.eth_cfg.enable;
	FE_PPPOE_t is_pppoe = fe_data->api_data.eth_cfg.is_pppoe;
	uint value = fe_data->api_data.eth_cfg.value;
	
	if(index < 0 || index > 15){
		printk("Error index %d, should be 0~15!\n", index);
		return 1;
	}
	val = read_reg_word(L2BR_ETYPE_EN);
	if(enable == FE_DISABLE){
		if(is_pppoe)	
			val &= ~(1<<(index+16));
		else
			val &= ~(1<<index);
	}else{
		if(is_pppoe)
			val |= (1<<(index+16));
		else
			val |= (1<<index);
	}
	write_reg_word(L2BR_ETYPE_EN, val);
	
	type = read_reg_word(L2BR_ETYPE_N(index));
	type &= ~(0xffff << (16*(index%2)));
	type |= value << (16*(index%2));
	write_reg_word(L2BR_ETYPE_N(index), type);
	return 0;
}

int fe_api_set_L2U_key(struct ecnt_fe_data *fe_data)
{
	/* TO DO */
	return 0;
}

int fe_api_get_ac_group_ptk_cnt(struct ecnt_fe_data *fe_data)
{
	uint index = fe_data->index;
	fe_data->cnt = read_reg_word(PPE_AC_PCNT(index));
	return 0;
}

int fe_api_get_ac_group_byte_cnt(struct ecnt_fe_data *fe_data)
{
	uint index = fe_data->index;
	uint flowid = index - UP_QUEUE_CNT_START;
	uint softCnt = 0;
	
	fe_data->cnt = read_reg_word(PPE_AC_BCNT_L(index));
	fe_data->cnt_hi = read_reg_word(PPE_AC_BCNT_H(index));

	if(flowid < FLOW_NUMBER){
		flowByteCnt_HW[flowid] += fe_data->cnt;
		PPE_API_GET_SOFT_CNT(flowid, &softCnt);
		fe_data->api_data.byteCnt.byteCnt_L = flowByteCnt_HW[flowid] + softCnt;
	}

	return 0;
}

int fe_api_set_meter_group(struct ecnt_fe_data *fe_data)
{
	return 0;
}

int fe_api_get_meter_group(struct ecnt_fe_data *fe_data)
{
	return 0;
}

int fe_api_set_gdm_pcp_coding(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, offset, mask, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_TXRX_Sel_t txrx_sel = fe_data->txrx_sel;
	FE_PcpMode_t mode = fe_data->api_data.coding_mode;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1)
		base_addr = GDMA1_LAN_PCP;
	else
		base_addr = GDMA2_WAN_PCP;
		
	if(txrx_sel == FE_GDM_SEL_TX){
		offset = GDM_TX_ENCODING_SHIFT;
		mask = GDM_TX_ENCODING_MASK;
	}else{
		offset = GDM_RX_DECODING_SHIFT;
		mask = GDM_RX_DECODING_MASK;
	}
	
	val = read_reg_word(base_addr);
	val &= ~(mask);
	val |= (mode << offset);
	
	write_reg_word(base_addr, val);
	return 0;
}

int fe_api_set_cdm_pcp_coding(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, offset, mask, val;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_TXRX_Sel_t txrx_sel = fe_data->txrx_sel;
	FE_PcpMode_t mode = fe_data->api_data.coding_mode;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1)
		base_addr = GDMA1_LAN_PCP;
	else
		base_addr = GDMA2_WAN_PCP;
		
	if(txrx_sel == FE_GDM_SEL_TX){
		offset = CDM_TX_ENCODING_SHIFT;
		mask = CDM_TX_ENCODING_MASK;
	}else{
		offset = CDM_RX_DECODING_SHIFT;
		mask = CDM_RX_DECODING_MASK;
	}
	val = read_reg_word(base_addr);
	val &= ~(mask);
	val |= (mode << offset);
	
	write_reg_word(base_addr, val);
	return 0;
}


int fe_api_set_vip_enable(struct ecnt_fe_data *fe_data)
{
	unsigned long int val, type;
	uint index = fe_data->index;
	FE_Enable_t enable = fe_data->api_data.vip_cfg.enable;
	FE_Patn_type patten_type = fe_data->api_data.vip_cfg.patten_type;
	uint patten = fe_data->api_data.vip_cfg.patten;
	
	if(index < 0 || index > 31){
		printk("Error index %d, should be 0~31!\n", index);
		return 1;
	}
	
	type = 0;
	val = 0;
	
	if(enable == FE_ENABLE){
		type |= ((0x01<<VIP_EN_CPU_OFFSET) | (0x01<<VIP_EN_ENABLE_OFFSET) | patten_type<<VIP_EN_TYPE_OFFSET);
		val = patten;
	}
	
	write_reg_word(FE_VIP_EN(index), type);
	write_reg_word(FE_VIP_PATN(index), val);

	return 0;
}

int fe_api_get_eth_rx_cnt(struct ecnt_fe_data *fe_data)
{
	fe_data->api_data.FE_RxCnt.rxFrameCnt = read_reg_word(GDMA2_RX_ETHERPCNT);
	fe_data->api_data.FE_RxCnt.rxFrameLen = read_reg_word(GDMA2_RX_ETHERPLEN);
	fe_data->api_data.FE_RxCnt.rxDropCnt = read_reg_word(GDMA2_RX_ETHDROPCNT);
	fe_data->api_data.FE_RxCnt.rxBroadcastCnt = read_reg_word(GDMA2_RX_ETHBCCNT);
	fe_data->api_data.FE_RxCnt.rxMulticastCnt = read_reg_word(GDMA2_RX_ETHMCCNT);
	fe_data->api_data.FE_RxCnt.rxCrcCnt = read_reg_word(GDMA2_RX_ETHCRCCNT);
	fe_data->api_data.FE_RxCnt.rxFragFameCnt = read_reg_word(GDMA2_RX_ETHFRACCNT);
	fe_data->api_data.FE_RxCnt.rxJabberFameCnt = read_reg_word(GDMA2_RX_ETHJABCNT);
	fe_data->api_data.FE_RxCnt.rxLess64Cnt = read_reg_word(GDMA2_RX_ETHRUNTCNT);
	fe_data->api_data.FE_RxCnt.rxMore1518Cnt = read_reg_word(GDMA2_RX_ETHLONGCNT);
	fe_data->api_data.FE_RxCnt.rxEq64Cnt = read_reg_word(GDMA2_RX_ETH_64_CNT);
	fe_data->api_data.FE_RxCnt.rxFrom65To127Cnt = read_reg_word(GDMA2_RX_ETH_65_TO_127_CNT);
	fe_data->api_data.FE_RxCnt.rxFrom128To255Cnt = read_reg_word(GDMA2_RX_ETH_128_TO_255_CNT);
	fe_data->api_data.FE_RxCnt.rxFrom256To511Cnt = read_reg_word(GDMA2_RX_ETH_256_TO_511_CNT);
	fe_data->api_data.FE_RxCnt.rxFrom512To1023Cnt = read_reg_word(GDMA2_RX_ETH_512_TO_1023_CNT);
	fe_data->api_data.FE_RxCnt.rxFrom1024To1518Cnt = read_reg_word(GDMA2_RX_ETH_1024_TO_1518_CNT);
	return 0;
}

int fe_api_get_eth_tx_cnt(struct ecnt_fe_data *fe_data)
{
	fe_data->api_data.FE_TxCnt.txFrameCnt = read_reg_word(GDMA2_TX_ETHCNT);
	fe_data->api_data.FE_TxCnt.txFrameLen = read_reg_word(GDMA2_TX_ETHLENCNT);
	fe_data->api_data.FE_TxCnt.txDropCnt = read_reg_word(GDMA2_TX_ETHDROPCNT);
	fe_data->api_data.FE_TxCnt.txBroadcastCnt = read_reg_word(GDMA2_TX_ETHBCDCNT);
	fe_data->api_data.FE_TxCnt.txMulticastCnt = read_reg_word(GDMA2_TX_ETHMULTICASTCNT);
	fe_data->api_data.FE_TxCnt.txLess64Cnt = read_reg_word(GDMA2_TX_ETH_LESS64_CNT);
	fe_data->api_data.FE_TxCnt.txMore1518Cnt = read_reg_word(GDMA2_TX_ETH_MORE1518_CNT);
	fe_data->api_data.FE_TxCnt.txEq64Cnt = read_reg_word(GDMA2_TX_ETH_64_CNT);
	fe_data->api_data.FE_TxCnt.txFrom65To127Cnt = read_reg_word(GDMA2_TX_ETH_65_TO_127_CNT);
	fe_data->api_data.FE_TxCnt.txFrom128To255Cnt = read_reg_word(GDMA2_TX_ETH_128_TO_255_CNT);
	fe_data->api_data.FE_TxCnt.txFrom256To511Cnt = read_reg_word(GDMA2_TX_ETH_256_TO_511_CNT);
	fe_data->api_data.FE_TxCnt.txFrom512To1023Cnt = read_reg_word(GDMA2_TX_ETH_512_TO_1023_CNT);
	fe_data->api_data.FE_TxCnt.txFrom1024To1518Cnt = read_reg_word(GDMA2_TX_ETH_1024_TO_1518_CNT);
	return 0;
}

int fe_api_get_eth_frame_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr;
	FE_TXRX_Sel_t txrx_sel = fe_data->txrx_sel;
	if(txrx_sel == FE_GDM_SEL_TX)
		base_addr = GDMA2_TX_ETHCNT;
	else
		base_addr = GDMA2_RX_ETHERPCNT;

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_get_eth_err_cnt(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr;
	FE_Err_type_t err_type = fe_data->err_type;
	if(err_type == FE_ERR_CRC)
		base_addr = GDMA2_RX_ETHCRCCNT;
	else if(err_type == FE_ERR_LONG)
		base_addr = GDMA2_RX_ETHLONGCNT;
	else if(err_type == FE_ERR_RUNT)
		base_addr = GDMA2_RX_ETHRUNTCNT;

	fe_data->cnt = read_reg_word(base_addr);
	return 0;
}

int fe_api_set_clear_mib(struct ecnt_fe_data *fe_data)
{
	unsigned long int base_addr, val, offset;
	FE_Gdma_Sel_t gdm_sel = fe_data->gdm_sel;
	FE_TXRX_Sel_t txrx_sel = fe_data->txrx_sel;
	
	if(gdm_sel == FE_GDM_SEL_GDMA1){
		base_addr = GDMA1_MIB_CLER;
	}else{
		base_addr = GDMA2_MIB_CLER;
	}
	offset = txrx_sel;
	
	val = read_reg_word(base_addr);
	val |= (1<<offset);
	
	write_reg_word(base_addr, val);
	
	return 0;
}

int  fe_get_ok_cnt(int  gdm_sel, int txrx_sel)
{
	/* NULL fun for hw_nat */
	return 0;
}

int qdma_set_channel_retire(uint channel)
{
	/* NULL fun for hw_nat */
	return 0;
}

void qdma_dma_mode(uint  txMode, uint rxMode, uint hwFwdMode)
{
	/* NULL fun for hw_nat */
	return;
}

int fe_api_set_cdm_rx_red_drop_mode(struct ecnt_fe_data *fe_data)
{
	FE_Cdma_Sel_t cdm_sel = fe_data->cdm_sel;
    FE_RedDropQ_Sel_t dropQ_sel = fe_data->dropQ_sel;
    FE_RedDropMode_Sel_t dropMode_sel = fe_data->dropMode_sel;
    unsigned long int reg, val, offset, mask;
	
	if(cdm_sel == FE_CDM_SEL_CDMA1)
		reg = CDMA1_FWD_CFG;
	else
		reg = CDMA2_FWD_CFG;

    mask = CDM_FWD_CFG_Q_MASK;
		
	if(dropQ_sel == FE_RED_DROP_Q0)
		offset = CDM_FWD_CFG_Q0_OFFSET;
    else if(dropQ_sel == FE_RED_DROP_Q1L)
        offset = CDM_FWD_CFG_Q1L_OFFSET;
	else
		offset = CDM_FWD_CFG_Q1H_OFFSET;
	
	val = read_reg_word(reg);
	val &= ~(mask << offset);
	val |= (dropMode_sel << offset);
	
	write_reg_word(reg, val);
	return 0;
}

int fe_api_get_cdm_rx_red_drop_mode(struct ecnt_fe_data *fe_data)
{
	FE_Cdma_Sel_t cdm_sel = fe_data->cdm_sel;
    FE_RedDropQ_Sel_t dropQ_sel = fe_data->dropQ_sel;
    unsigned long int reg, val, offset, mask;
	
	if(cdm_sel == FE_CDM_SEL_CDMA1)
		reg = CDMA1_FWD_CFG;
	else
		reg = CDMA2_FWD_CFG;

    mask = CDM_FWD_CFG_Q_MASK;
		
	if(dropQ_sel == FE_RED_DROP_Q0)
		offset = CDM_FWD_CFG_Q0_OFFSET;
    else if(dropQ_sel == FE_RED_DROP_Q1L)
        offset = CDM_FWD_CFG_Q1L_OFFSET;
	else
		offset = CDM_FWD_CFG_Q1H_OFFSET;
	
	val = read_reg_word(reg);
    fe_data->dropMode_sel = (val >> offset) & mask;
        
	return 0;
}

static int fe_set_tx_rate(unsigned int rate,unsigned int mode,unsigned maxBkSize,unsigned tick)
{

	unsigned int value = 0;

	if (mode > 0)
		mode = 1;

	
	if(rate == 0){
		write_reg_word(GDMA2_SHRP_CFG,0);
	}else{
		value = (1 << GDMA_SHRP_CFG_EN_OFFSET) | (tick << GDMA_SHRP_CFG_TICK_OFFSET)
					|(rate << GDMA_SHRP_CFG_RATE_OFFSET) | maxBkSize << GDMA_SHRP_CFG_BUCK_OFFSET | mode;  

		write_reg_word(GDMA2_SHRP_CFG,value);
	}
	return 0;
}

static int fe_set_rx_rate(unsigned int type,unsigned int rate,unsigned int mode)
{
	unsigned int reg = 0,value = 0,tick = 0;

	if (type == FE_WAN_RX_UC_RATE)
		reg = GDMA2_RXUC_SHPR_CFG;
	else if (type == FE_WAN_RX_BC_RATE)
		reg = GDMA2_RXBC_SHPR_CFG;
	else if (type == FE_WAN_RX_MC_RATE)
		reg = GDMA2_RXMC_SHPR_CFG;
	else if  (type == FE_WAN_RX_OC_RATE)
		reg = GDMA2_RXOC_SHPR_CFG;
	else
		return -1;


	if (mode > 0)
		mode = 1;
	
	rate &= 0x1fffff;
	
	if(rate == 0){
		write_reg_word(reg,0);
	}else{
		value = (1 << GDMA_SHRP_CFG_EN_OFFSET) | (tick << GDMA_SHRP_CFG_TICK_OFFSET)
					|(rate << GDMA_SHRP_CFG_RATE_OFFSET) | 0xfe | mode;  

		write_reg_word(reg,value);
	}
	return 0;
}

static int fe_set_rxuc_rate(unsigned int rate,unsigned int mode)
{
	return fe_set_rx_rate(FE_WAN_RX_UC_RATE,rate,mode);
}

static int fe_set_rxbc_rate(unsigned int rate,unsigned int mode)
{
	return fe_set_rx_rate(FE_WAN_RX_BC_RATE,rate,mode);
}

static  int fe_set_rxmc_rate(unsigned int rate,unsigned int mode)
{
	return fe_set_rx_rate(FE_WAN_RX_MC_RATE,rate,mode);
}

static int fe_set_rxoc_rate(unsigned int rate,unsigned int mode)
{
	return fe_set_rx_rate(FE_WAN_RX_OC_RATE,rate,mode);
}

static int  fe_set_vip_type(int idx,fe_vip_packet* vip){

	unsigned int value;

	if (idx >= FE_VIP_PACKET_NUM)
		return -1;

	write_reg_word(FE_VIP_EN(idx),0);
	write_reg_word(FE_VIP_PATN(idx),0);

	value = vip->sport;
	if (vip->type == FE_VIP_TYPE_TCP || vip->type == FE_VIP_TYPE_UDP)
		value |= (vip->dport << VIP_PATN_DP_OFFSET);
	
	write_reg_word(FE_VIP_PATN(idx),value);

	value = (1 << VIP_EN_CPU_OFFSET) | (vip->sw_en << VIP_EN_SWEN_OFFSET) 
			  | (vip->dp_en << VIP_EN_DPEN_OFFSET) | (vip->sp_en << VIP_EN_SPEN_OFFSET) 
			  | (vip->type << VIP_EN_TYPE_OFFSET) | (1 << VIP_EN_ENABLE_OFFSET);
	
	write_reg_word(FE_VIP_EN(idx),value);
	
	return idx;
}


static int fe_find_vip_type(fe_vip_packet* vip){

	unsigned int en_val;
	unsigned int patn_val;
	int i;

	for(i = 0; i< FE_VIP_PACKET_NUM;i++){
		en_val = read_reg_word(FE_VIP_EN(i));
		if ((en_val & 1) == 0)
			continue;
		
		if (((en_val >> VIP_EN_TYPE_OFFSET) & 0x07) !=  vip->type)
			continue;	

		patn_val = read_reg_word(FE_VIP_PATN(i));

		if( vip->type < 3 )/*0: ETH_TYPE ; 1:PPPOE protocol ; 2:IP protocol*/
		{
			if( (patn_val & 0xffff) != vip->etype )
				continue ;
		}
		else/*3:TCP ports; 4:UDP ports*/
		{
			if( (((en_val >> VIP_EN_DPEN_OFFSET ) & 0x01) ==  vip->dp_en) && (((en_val >> VIP_EN_SPEN_OFFSET ) & 0x01) ==  vip->sp_en) 
				&& (((en_val >> VIP_EN_SWEN_OFFSET ) & 0x01) ==  vip->sw_en) )/*compare port-type*/
			{
				if( (vip->sp_en == 1) && ((patn_val & 0xffff) != vip->sport) )
					continue;	
				if( ( vip->dp_en ==1 ) && (((patn_val >> 16) & 0xffff) != vip->dport) )
					continue ;
			}
			else
			{
			continue;
			}
		}

		return i;
	}

	return -1;
}

static int fe_get_vip_empty(void){

	unsigned int value;
	int i;

	for(i = 0; i< FE_VIP_PACKET_NUM;i++){
		value = read_reg_word(FE_VIP_EN(i));
		if ((value & 1) == 0)
			break;
	}

	return (i < FE_VIP_PACKET_NUM) ? i : (-1);
}

static int fe_del_vip_type(int idx){
	
	if (idx >= FE_VIP_PACKET_NUM)
		return -1;

	write_reg_word(FE_VIP_EN(idx),0);
	write_reg_word(FE_VIP_PATN(idx),0);
	return 0;
}

static int fe_add_vip_common(fe_vip_packet* vip)
{
	int idx;

	if (fe_find_vip_type(vip) >= 0)
		return -1;

	if ((idx = fe_get_vip_empty()) < 0)
		return -1;

	return fe_set_vip_type(idx,vip);
}

static int fe_del_vip_common(fe_vip_packet* vip)
{
	int idx;
	
	if ((idx = fe_find_vip_type(vip)) < 0)
		return -1;	

	return fe_del_vip_type(idx);
}


static int fe_add_vip_ether(uint16 type)
{
	fe_vip_packet	vip;

	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 0;
	vip.etype = type;

	return fe_add_vip_common(&vip);
}

static int fe_del_vip_ether(uint16 type)
{
	fe_vip_packet	vip;
	
	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 0;
	vip.etype = type;

	return fe_del_vip_common(&vip);
}

static int fe_add_vip_ppp(uint16 type)
{
	fe_vip_packet	vip;
	
	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 1;
	vip.ppp = type;

	return fe_add_vip_common(&vip);
}

static int fe_del_vip_ppp(uint16 type)
{
	fe_vip_packet	vip;
	
	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 1;
	vip.ppp = type;

	return fe_del_vip_common(&vip);
}

static int fe_add_vip_ip(uint16 type)
{
	fe_vip_packet	vip;

	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 2;
	vip.iptype= type;

	return fe_add_vip_common(&vip);
}

static int fe_del_vip_ip(uint16 type)
{
	fe_vip_packet	vip;
	
	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 2;
	vip.iptype= type;

	return fe_del_vip_common(&vip);
}

static int fe_add_vip_tcp(uint16 src, uint16 dst,unchar mode)
{
	fe_vip_packet	vip;

	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 3;
	vip.sport= src;
	vip.dport = dst;
	vip.sp_en = mode & 0x01 ;
	vip.dp_en = (mode & 0x02)>>1 ;
	vip.sw_en = (mode & 0x04)>>2 ;
	
	return fe_add_vip_common(&vip);
}

static int fe_del_vip_tcp(uint16 src, uint16 dst,unchar mode)
{
	fe_vip_packet	vip;

	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type =3;
	vip.sport= src;
	vip.dport = dst;
	vip.sp_en = mode & 0x01 ;
	vip.dp_en = (mode & 0x02)>>1 ;
	vip.sw_en = (mode & 0x04)>>2 ;

	return fe_del_vip_common(&vip);
}

static int fe_add_vip_udp(uint16 src, uint16 dst,unchar mode)
{
	fe_vip_packet	vip;

	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 4;
	vip.sport= src;
	vip.dport = dst;
	vip.sp_en = mode & 0x01 ;
	vip.dp_en = (mode & 0x02)>>1 ;
	vip.sw_en = (mode & 0x04)>>2 ;
	
	return fe_add_vip_common(&vip);
}

static int fe_del_vip_udp(uint16 src, uint16 dst,unchar mode)
{
	fe_vip_packet	vip;
	
	memset(&vip,0,sizeof(vip));
	
	vip.en = 1;
	vip.type = 4;
	vip.sport= src;
	vip.dport = dst;
	vip.sp_en = mode & 0x01 ;
	vip.dp_en = (mode & 0x02)>>1 ;
	vip.sw_en = (mode & 0x04)>>2 ;

	return fe_del_vip_common(&vip);
}

static int fe_set_l2lu_key(int idx ,fe_l2lu_key* key){

	unsigned int value;

	if (idx >= FE_L2LU_KEY_NUM) 
		return -1;
	
	write_reg_word(PSE_L2LU_KEY0(idx),0);
	write_reg_word(PSE_L2LU_KEY1(idx),0);

	value = (key->spcp << L2LU_KEY0_SPCP_OFFSET) | (key->svid << L2LU_KEY0_SVID_OFFSET) 
			| (key->cpcp << L2LU_KEY0_CPCP_OFFSET) | key->cvid;

	write_reg_word(PSE_L2LU_KEY1(idx),value);

	value = (key->dscp << L2LU_KEY1_DSCP_OFFSET) | (key->spcp_en << L2LU_KEY1_SPCP_EN_OFFSET) 
			| (key->svid_en << L2LU_KEY1_SVID_EN_OFFSET) | (key->cpcp_en << L2LU_KEY1_CPCP_EN_OFFSET)
			|  (key->cvid_en << L2LU_KEY1_CVID_EN_OFFSET) | key->dscp_mask;

	write_reg_word(PSE_L2LU_KEY0(idx),value);

	return idx;
}

static int fe_find_l2lu_key(fe_l2lu_key* key)
{
	unsigned int val;
	int i;


	for(i = 0; i< FE_VIP_PACKET_NUM;i++){
		val = read_reg_word(PSE_L2LU_KEY0(i));
		if ((val & 0xff) != key->dscp_mask)
			continue;
		if (((val >> 8) & 0x1) != key->cvid_en)
			continue;
		if (((val >> 9) & 0x1) != key->cpcp_en)
			continue;
		if (((val >> 12) & 0x1) != key->svid_en)
			continue;
		if (((val >> 13) & 0x1) != key->spcp_en)
			continue;
		if (((val >> 16) & 0xffff) != key->dscp)
			continue;
		
		val = read_reg_word(PSE_L2LU_KEY1(i));
		if ((val & 0xfff) != key->cvid)
			continue;
		if (((val >> 13) & 0x7) != key->cpcp)
			continue;		
		if (((val >> 16) & 0xfff) != key->svid)
			continue;	
		if (((val >> 29) & 0x7) != key->spcp)
			continue;
		
		return i;
	}

	return -1;

}

static int fe_get_l2lu_empty(void)
{
	unsigned int val,i;

	for(i = 0; i< FE_L2LU_KEY_NUM;i++){
		val = read_reg_word(PSE_L2LU_KEY0(i));
		if ((val & 0xffff) == 0)
			break;
	}
	
	return  (i < FE_L2LU_KEY_NUM) ? i: (-1);
}

static int fe_del_l2lu_key(int idx)
{
	if (idx >= FE_L2LU_KEY_NUM)
		return -1;
	
	write_reg_word(PSE_L2LU_KEY0(idx),0);
	write_reg_word(PSE_L2LU_KEY1(idx),0);
	return 0;
}

static int fe_add_l2lu_common(fe_l2lu_key* key)
{
	int idx ;

//	if (fe_find_l2lu_key(key) >= 0)
//		return -1;

	if ((idx = fe_get_l2lu_empty()) < 0)
		return -1;

	return fe_set_l2lu_key(idx,key);
}

static int fe_del_l2lu_common(fe_l2lu_key* key)
{
	int idx;

	if ((idx = fe_find_l2lu_key(key)) < 0)
		return -1;

	return fe_del_l2lu_key(idx);
}


static int fe_add_l2lu_vlan_dscp(ushort svlan,ushort cvlan,unchar dscp, unchar mask)
{
	fe_l2lu_key key;

	memset(&key,0,sizeof(key));
	
	key.svid_en = mask & 1;
	key.spcp_en = (mask >> 1) & 1;
	key.cvid_en = (mask >> 2) & 1;
	key.cpcp_en = (mask >> 3) & 1;
	key.dscp_mask= ((mask >> 4) & 1) ? 0x1f : 0;

	if (key.svid_en)
		key.svid = svlan & 0xfff;
	
	if (key.spcp_en)
		key.spcp = (svlan >> 13) & 0x7;

	if (key.cvid_en)
		key.cvid = cvlan & 0xfff;

	if (key.cpcp_en)
		key.cpcp = (cvlan >> 13) & 0x7;

	if (key.dscp_mask)
		key.dscp = dscp;
	
	return fe_add_l2lu_common(&key);
}

static int fe_del_l2lu_vlan_dscp(ushort svlan,ushort cvlan,unchar dscp, unchar mask)
{
	fe_l2lu_key key;

	memset(&key,0,sizeof(key));
	
	key.svid_en = mask & 1;
	key.spcp_en = (mask >> 1) & 1;
	key.cvid_en = (mask >> 2) & 1;
	key.cpcp_en = (mask >> 3) & 1;
	key.dscp_mask= ((mask >> 4) & 1) ? 0x1f : 0;

	if (key.svid_en)
		key.svid = svlan & 0xfff;
	
	if (key.spcp_en)
		key.spcp = (svlan >> 13) & 0x7;

	if (key.cvid_en)
		key.cvid = cvlan & 0xfff;

	if (key.cpcp_en)
		key.cpcp = (cvlan >> 13) & 0x7;

	if (key.dscp_mask)
		key.dscp = dscp;

	return fe_del_l2lu_common(&key);
}

static int fe_add_l2lu_vlan_trfc(ushort svlan,ushort cvlan,unchar trfc, unchar mask)
{
	fe_l2lu_key key;

	memset(&key,0,sizeof(key));
	
	key.svid_en = mask & 1;
	key.spcp_en = (mask >> 1) & 1;
	key.cvid_en = (mask >> 2) & 1;
	key.cpcp_en = (mask >> 3) & 1;
	key.dscp_mask= ((mask >> 4) & 1) ? 0xff : 0;

	if (key.svid_en)
		key.svid = svlan & 0xfff;
	
	if (key.spcp_en)
		key.spcp = (svlan >> 13) & 0x7;

	if (key.cvid_en)
		key.cvid = cvlan & 0xfff;

	if (key.cpcp_en)
		key.cpcp = (cvlan >> 13) & 0x7;

	if (key.dscp_mask)
		key.dscp = trfc;

	return fe_add_l2lu_common(&key);
}

static int fe_del_l2lu_vlan_trfc(ushort svlan,ushort cvlan,unchar trfc, unchar mask)
{
	fe_l2lu_key key;

	memset(&key,0,sizeof(key));
	
	key.svid_en = mask & 1;
	key.spcp_en = (mask >> 1) & 1;
	key.cvid_en = (mask >> 2) & 1;
	key.cpcp_en = (mask >> 3) & 1;
	key.dscp_mask= ((mask >> 4) & 1) ? 0xff : 0;

	if (key.svid_en)
		key.svid = svlan & 0xfff;
	
	if (key.spcp_en)
		key.spcp = (svlan >> 13) & 0x7;

	if (key.cvid_en)
		key.cvid = cvlan & 0xfff;

	if (key.cpcp_en)
		key.cpcp = (cvlan >> 13) & 0x7;

	if (key.dscp_mask)
		key.dscp = trfc;

	return fe_del_l2lu_common(&key);
}

int fe_set_tx_favor_oam_enable(unchar enable)
{
	unsigned int val;
	
	if((0 != enable)&&(1 != enable))
		return -1;
		
	if(!isEN7526c)
		return -1;

	val = read_reg_word(GDMA2_FWD_CFG);

	val &= ~(1 << GDMA2_TX_FAVOR_OAM_OFFSET);
	val |= enable<<GDMA2_TX_FAVOR_OAM_OFFSET;

	write_reg_word(GDMA2_FWD_CFG,val);

	return 0;
}

int fe_api_set_tx_rate(struct ecnt_fe_data *fe_data)
{
	 uint rate = fe_data->api_data.rate_cfg.rate;
         uint mode = fe_data->api_data.rate_cfg.mode;
	 uint maxBkSize = fe_data->api_data.rate_cfg.maxBkSize;
         uint tick = fe_data->api_data.rate_cfg.tick;
	 fe_data->retValue = fe_set_tx_rate(rate,mode,maxBkSize,tick);
	return 0;
}

int fe_api_set_rxuc_rate(struct ecnt_fe_data *fe_data)
{
	uint rate = fe_data->api_data.rate_cfg.rate;
	uint mode = fe_data->api_data.rate_cfg.mode;
	fe_data->retValue = fe_set_rxuc_rate(rate,mode);
	return 0;
}

int fe_api_set_rxbc_rate(struct ecnt_fe_data *fe_data)
{
	uint rate = fe_data->api_data.rate_cfg.rate;
	uint mode = fe_data->api_data.rate_cfg.mode;
	fe_data->retValue = fe_set_rxbc_rate(rate,mode);
	return 0;
}

int fe_api_set_rxmc_rate(struct ecnt_fe_data *fe_data)
{
	uint rate = fe_data->api_data.rate_cfg.rate;
	uint mode = fe_data->api_data.rate_cfg.mode;
	fe_data->retValue = fe_set_rxmc_rate(rate,mode);
	return 0;
}

int fe_api_set_rxoc_rate(struct ecnt_fe_data *fe_data)
{
	uint rate = fe_data->api_data.rate_cfg.rate;
	uint mode = fe_data->api_data.rate_cfg.mode;
	fe_data->retValue = fe_set_rxoc_rate(rate,mode);
	return 0;
}

int fe_api_add_vip_ether(struct ecnt_fe_data *fe_data)
{
	ushort type = fe_data->api_data.vip.type;
	fe_data->retValue = fe_add_vip_ether(type);
	return 0;
}

int fe_api_add_vip_ppp(struct ecnt_fe_data *fe_data)
{
	ushort type = fe_data->api_data.vip.type;
	fe_data->retValue = fe_add_vip_ppp(type);
	return 0;
}

int fe_api_add_vip_ip(struct ecnt_fe_data *fe_data)
{
	ushort type = fe_data->api_data.vip.type;
	fe_data->retValue = fe_add_vip_ip(type);
	return 0;
}

int fe_api_add_vip_tcp(struct ecnt_fe_data *fe_data)
{
	ushort src = fe_data->api_data.vip.src;
	ushort dst = fe_data->api_data.vip.dst;
	unchar mode = fe_data->api_data.vip.mode;
	fe_data->retValue = fe_add_vip_tcp(src,dst,mode);
#ifdef TCSUPPORT_FE_VIP_ENHANCEMENT
	QDMA_API_READ_VIP_INFO(ECNT_QDMA_WAN);
#endif
	return 0;
}

int fe_api_add_vip_udp(struct ecnt_fe_data *fe_data)
{
	ushort src = fe_data->api_data.vip.src;
	ushort dst = fe_data->api_data.vip.dst;
	unchar mode = fe_data->api_data.vip.mode;
	fe_data->retValue = fe_add_vip_udp(src,dst,mode);
#ifdef TCSUPPORT_FE_VIP_ENHANCEMENT
	QDMA_API_READ_VIP_INFO(ECNT_QDMA_WAN);
#endif
	return 0;
}

int fe_api_del_vip_ether(struct ecnt_fe_data *fe_data)
{
	ushort type = fe_data->api_data.vip.type;
	fe_data->retValue = fe_del_vip_ether(type);
	return 0;
}

int fe_api_del_vip_ppp(struct ecnt_fe_data *fe_data)
{
	ushort type = fe_data->api_data.vip.type;
	fe_data->retValue = fe_del_vip_ppp(type);
	return 0;
}

int fe_api_del_vip_ip(struct ecnt_fe_data *fe_data)
{
	ushort type = fe_data->api_data.vip.type;
	fe_data->retValue = fe_del_vip_ip(type);
	return 0;

}

int fe_api_del_vip_tcp(struct ecnt_fe_data *fe_data)
{
	ushort src = fe_data->api_data.vip.src;
	ushort dst = fe_data->api_data.vip.dst;
	unchar mode = fe_data->api_data.vip.mode;
	fe_data->retValue = fe_del_vip_tcp(src,dst,mode);
#ifdef TCSUPPORT_FE_VIP_ENHANCEMENT
	QDMA_API_READ_VIP_INFO(ECNT_QDMA_WAN);
#endif
	return 0;
}

int fe_api_del_vip_udp(struct ecnt_fe_data *fe_data)
{
	ushort src = fe_data->api_data.vip.src;
	ushort dst = fe_data->api_data.vip.dst;
	unchar mode = fe_data->api_data.vip.mode;
	fe_data->retValue = fe_del_vip_udp(src,dst,mode);
#ifdef TCSUPPORT_FE_VIP_ENHANCEMENT
	QDMA_API_READ_VIP_INFO(ECNT_QDMA_WAN);
#endif
	return 0;
}

int fe_api_add_l2lu_vlan_dscp(struct ecnt_fe_data *fe_data)
{
	unchar mask = fe_data->api_data.l2lu.mask;
	unchar dscp = fe_data->api_data.l2lu.dscp;
	ushort svlan = fe_data->api_data.l2lu.svlan;
	ushort cvlan = fe_data->api_data.l2lu.cvlan;
	fe_data->retValue = fe_add_l2lu_vlan_dscp(svlan,cvlan,dscp,mask);	
	return 0;
}

int fe_api_add_l2lu_vlan_trfc(struct ecnt_fe_data *fe_data)
{
	unchar mask = fe_data->api_data.l2lu.mask;
	unchar trfc = fe_data->api_data.l2lu.dscp;
	ushort svlan = fe_data->api_data.l2lu.svlan;
	ushort cvlan = fe_data->api_data.l2lu.cvlan;
	fe_data->retValue = fe_add_l2lu_vlan_trfc(svlan,cvlan,trfc,mask);		
	return 0;	
}

 int fe_api_del_l2lu_vlan_dscp(struct ecnt_fe_data *fe_data)
{
	unchar mask = fe_data->api_data.l2lu.mask;
	unchar dscp = fe_data->api_data.l2lu.dscp;
	ushort svlan = fe_data->api_data.l2lu.svlan;
	ushort cvlan = fe_data->api_data.l2lu.cvlan;
	fe_data->retValue = fe_del_l2lu_vlan_dscp(svlan,cvlan,dscp,mask);
	return 0;
}

 int fe_api_del_l2lu_vlan_trfc(struct ecnt_fe_data *fe_data)
{
	unchar mask = fe_data->api_data.l2lu.mask;
	unchar trfc = fe_data->api_data.l2lu.dscp;
	ushort svlan = fe_data->api_data.l2lu.svlan;
	ushort cvlan = fe_data->api_data.l2lu.cvlan;
	fe_data->retValue = fe_del_l2lu_vlan_trfc(svlan,cvlan,trfc,mask);	
	return 0;
}

 int fe_api_set_tx_favor_oam_enable(struct ecnt_fe_data *fe_data)
{
	unchar enable = fe_data->api_data.enable;
	fe_data->retValue = fe_set_tx_favor_oam_enable(enable);	
	return 0;
}

long fe_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) 
{
	int ret = 0 ;
	struct ecnt_fe_data data;
	struct ecnt_pcie_data* puser = (struct ecnt_fe_data*)arg;

	if (cmd >= FE_FUNCTION_MAX_NUM)
		return -1;
	
	memset(&data,0,sizeof(struct ecnt_fe_data));
	copy_from_user(&data, puser ,sizeof(struct ecnt_fe_data));
	ret = fe_operation[data.function_id](&data);
	copy_to_user(puser,&data,sizeof(struct ecnt_fe_data));
	
	return ret;
}

EXPORT_SYMBOL(fe_get_ok_cnt);
EXPORT_SYMBOL(fe_reg_read);
EXPORT_SYMBOL(fe_reg_write);
EXPORT_SYMBOL(fe_reg_modify_bits);
EXPORT_SYMBOL(qdma_set_channel_retire);
EXPORT_SYMBOL(qdma_dma_mode);

