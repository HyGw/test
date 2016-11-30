#include <linux/module.h>
#include <asm/tc3162/tc3162.h>
#include <linux/netdevice.h>

#include "fe_reg_en7512.h"
#include "fe.h"
#include "fe_api.h"
#include "fe_test.h"


void fe_reg_modify(unsigned int reg_offset, unsigned int Data, unsigned int Offset, unsigned int Len)
{
	unsigned int Mask = 0;
	unsigned int Value;
	unsigned int i;

	for (i = 0; i < Len; i++) {
		Mask |= 1 << (Offset + i);
	}

	Value = read_reg_word(reg_offset);
	Value &= ~Mask;
	Value |= (Data << Offset) & Mask;
	write_reg_word(reg_offset, Value);
}

unsigned int fe_reg_get_bits(unsigned int reg, unsigned int offset, unsigned int len)
{
	unsigned int mask = 0;
	unsigned int value;
	unsigned int i;

	for (i = 0; i < len; i++) {
		mask |= (1 << i);
	}
	
	value = read_reg_word(reg) >> offset;
	
	return value & mask;	
}

//////////////////Acclocate PSE buffer for each port////////////////////////
int fe_allocate_port_buff(fe_buff* buff){

	unsigned int value = 0;

	value += buff->p0;
	value += buff->p1;	
	value += buff->p2;	
	value += buff->p4;	
	value += buff->p5;	
	value += buff->vp1;	
	value += buff->vp2;	

	if(value > fe_reg_get_bits(PSE_FQFC_CFG,PSE_FQFC_CFG_FQMAX_OFFSET,PSE_FQFC_CFG_FQMAX_LENGTH))
		return -1;

	value = (buff->p2 << PSE_IQ_RES1_P2_OFFSET) | (buff->p1 << PSE_IQ_RES1_P1_OFFSET) |  buff->p0;

	write_reg_word(PSE_IQ_REV1,value);

	value = (buff->p5 << PSE_IQ_RES2_P5_OFFSET) |  buff->p4;

	write_reg_word(PSE_IQ_REV2,value);

	value = (buff->vp2 << PSE_IQ_VIP_P2_OFFSET) |  buff->vp1;

	write_reg_word(PSE_IQ_REV_VIP,value);

	return 0;
	
}


int fe_parse_port_buff(char* buff){

	unsigned int i,len = 0,input ,output,max,res;

	len += sprintf(buff+len,"PSE Buffer Usage: \n");

	len += sprintf(buff+len,"FQ: Curr(%d) MinLeft(%d)\n",
			fe_reg_get_bits(PSE_FQ_MIN,PSE_FQ_MIN_CUR_OFFSET,PSE_FQ_MIN_CUR_LENGTH),
			fe_reg_get_bits(PSE_FQ_MIN,PSE_FQ_MIN_MIN_OFFSET,PSE_FQ_MIN_MIN_LENGTH));

	for( i= 0;i<7;i++){
		if (i < 4)
		{
			res = (read_reg_word(PSE_IQ_REV1) >> (i*8)) & 0xff;
			input = (read_reg_word(PSE_IQ_STA1) >> (i*8)) & 0xff;
			output = (read_reg_word(PSE_OQ_STA1) >> (i*8)) & 0xff;
			max = (read_reg_word(PSE_IQ_MAX1) >> (i*8)) & 0xff;
		}
		else
		{
			res = (read_reg_word(PSE_IQ_REV2) >> ((i-4)*8)) & 0xff;
			input = (read_reg_word(PSE_IQ_STA2) >> ((i-4)*8)) & 0xff;
			output = (read_reg_word(PSE_OQ_STA2) >> ((i-4)*8)) & 0xff;
			max = (read_reg_word(PSE_IQ_MAX2) >> ((i-4)*8)) & 0xff;		
		}
		len += sprintf(buff+len,"P%d: Res(%d) Input(%d) Output(%d) Max(%d)\n",i,res,input,output,max);
	}
	return 0;
}

///////////////CDM FWD CFG Setting////////////////////////
int fe_parse_fwd_cfg(int pse_idx,char* buff){



	return 0;
}

/*
int fe_set_packet_length(int gdm_sel,unsigned int length_long, unsigned  int length_short)
{
	unsigned long int base_addr, val;


	if(gdm_sel == MT7512_FE_GDM1){
		base_addr = GDMA1_LEN_CFG;
	}else{
		base_addr = GDMA2_LEN_CFG;
	}

	write_reg_word(base_addr, ((length_long << 16) | length_short));

	return 0;
}

int fe_set_tx_channel_enable(int gdm_sel,int channel, int enable)
{
	unsigned long int base_addr, val;


	if(gdm_sel == MT7512_FE_GDM1){
		base_addr = GDMA1_TXCHN_EN;
	}else{
		base_addr = GDMA2_TXCHN_EN;
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


int fe_set_rx_channel_enable(int gdm_sel,int channel, int enable)
{
	unsigned long int base_addr, val;


	if(gdm_sel == MT7512_FE_GDM1){
		base_addr = GDMA1_RXCHN_EN;
	}else{
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

int  fe_set_crc_strip(int  gdm_sel, int enable)
{
	unsigned long int base_addr, val;

	if(gdm_sel == MT7512_FE_GDM1){
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
*/
	int fe_set_hwfwd_channel(int pse_idx,int chn,int val)
	{
		
		unsigned int  reg,value ;
	
		if (pse_idx == MT7512_FE_CDM1){
			reg = CDMA1_HWF_CHN_EN;
		}else if(pse_idx == MT7512_FE_CDM2){
			reg = CDMA2_HWF_CHN_EN;
		}else{
			return -1;
		}
	
		value = read_reg_word(reg);
	
		value &= ~(1 << chn);
	
		value |= (val << chn);
		
		write_reg_word(reg,value);
	
		return 0;
	}
	
	int fe_set_channel_retire(int  pse_idx, int chn, int mode)
	{
		unsigned int  reg,reg2,value,mask,ret = 0;
		unsigned int txreg,rxreg,txchn,rxchn;
		
		if (pse_idx == MT7512_FE_GDM1){
			reg = GDMA1_CHN_RLS;
			reg2 = GDMA1_TX_CHN_VLD;
			txreg = GDMA1_TXCHN_EN;
			rxreg = GDMA1_RXCHN_EN;
		}else if(pse_idx == MT7512_FE_GDM2){
			reg = GDMA2_CHN_RLS;
			reg2 = GDMA2_TX_CHN_VLD;
			txreg = GDMA2_TXCHN_EN;
			rxreg = GDMA2_RXCHN_EN; 	
		}else{
			return -1;
		}	
		
		mask = read_reg_word(reg2);
	
		if ( ((1<< chn)& mask) == 0)
			return 0;
	
	
		if (pse_idx == MT7512_FE_GDM1)
			fe_set_hwfwd_channel(MT7512_FE_CDM1,chn,0);
		else 
			fe_set_hwfwd_channel(MT7512_FE_CDM2,chn,0);
	
		if (mode == 1){
			txchn = read_reg_word(txreg);
			rxchn = read_reg_word(rxreg);
			write_reg_word(txreg,0);
			write_reg_word(rxreg,0);
		}
	
		value = (chn << GDMA_CHN_RLS_CHN_OFFSET) | (1 << GDMA_CHN_RLS_EN_OFFSET);
		
		write_reg_word(reg,value);
	
		mdelay(1);
		
		value = 0;
			
		while( ( (read_reg_word(reg) & (1 << GDMA_CHN_RLS_STAT_OFFSET)) == 0 
					||	(read_reg_word(reg2) & (1 << chn)) != 0 )
					&& (value++ < GDMA_CHN_RLS_TIMEOUT)){
			mdelay(1);
		}
		
		if (value >= GDMA_CHN_RLS_TIMEOUT){
			printk("fe_set_channel_retire: timeout \n");
			ret = -1;
		}
	
		write_reg_word(reg,0);
		
		if (mode == 1){
			write_reg_word(txreg,txchn);
			write_reg_word(rxreg,rxchn);
		}
		
		if (pse_idx == MT7512_FE_GDM1)
			fe_set_hwfwd_channel(MT7512_FE_CDM1,chn,1);
		else 
			fe_set_hwfwd_channel(MT7512_FE_CDM2,chn,1);
		
		return ret;
	
	}


void fe_set_mac_addr( int gdm_sel,unsigned char *mac, int mask)
{
	unsigned long int base_addr_lsb, base_addr_msb, val;

	if(gdm_sel == MT7512_FE_GDM1){
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
}

int fe_get_mac_addr(int pse_idx,unsigned char* addr){

	unsigned int value, reg;;

	if (pse_idx == MT7512_FE_GDM1){
		reg = GDMA1_MAC_ADRL;
	}else if(pse_idx == MT7512_FE_GDM2){
		reg = GDMA2_MAC_ADRL;
	}else{
		return -1;
	}

	value = read_reg_word(reg);
	
	addr[5] = value;
	addr[4] = (value >> 8);
	addr[3] = (value >> 16);
	addr[2] = (value >> 24);	
	
	value = read_reg_word(reg+4);
	
	addr[1] = value;
	addr[0] = (value >> 8);
	
	return 0;
}

int fe_parse_mac_addr(int pse_idx, char* buf){

	unsigned char mac[6];

	fe_get_mac_addr(pse_idx,mac);
	
	if (pse_idx == MT7512_FE_GDM1){
		sprintf(buf,"GDMA1 Mac Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
						mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	}else if(pse_idx == MT7512_FE_GDM2){
		sprintf(buf,"GDMA2 Mac Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
						mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);		
	}else{
		sprintf(buf,"GDMA Mac Addr: PSE IDX Error! \n");
	}
	
	return 0;
}



int  fe_set_vip_packet(int idx,fe_vip_packet* vip){

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

int fe_add_vip_packet(fe_vip_packet* vip){

	unsigned int value;
	int i;

	for(i = 0; i< FE_VIP_PACKET_NUM;i++){
		value = read_reg_word(FE_VIP_EN(i));
		if ((value & 1) == 0)
			break;
	}

	return fe_set_vip_packet(i,vip);
}


int fe_get_vip_packet(int idx,fe_vip_packet* vip){

	unsigned int value;
	
	if (idx <0 || idx >= FE_VIP_PACKET_NUM)
		return -1;

//	value = fe_reg_read(FE_VIP_EN(idx));
	vip->en = fe_reg_get_bits(FE_VIP_EN(idx),VIP_EN_ENABLE_OFFSET,1); 
	vip->type = fe_reg_get_bits(FE_VIP_EN(idx),VIP_EN_TYPE_OFFSET,VIP_EN_TYPE_LENGTH);
	vip->sp_en = fe_reg_get_bits(FE_VIP_EN(idx),VIP_EN_SPEN_OFFSET,1); 
	vip->dp_en = fe_reg_get_bits(FE_VIP_EN(idx),VIP_EN_DPEN_OFFSET,1);  	
	vip->sw_en = fe_reg_get_bits(FE_VIP_EN(idx),VIP_EN_SWEN_OFFSET,1); 
//	value = fe_reg_read(FE_VIP_PATN(idx));
	vip->sport =  fe_reg_get_bits(FE_VIP_PATN(idx),VIP_PATN_SP_OFFSET,VIP_PATN_PORT_LENGTH); 	
	vip->dport =  fe_reg_get_bits(FE_VIP_PATN(idx),VIP_PATN_DP_OFFSET,VIP_PATN_PORT_LENGTH); 	
	
	return 0;
}


int fe_del_vip_packet(int idx){
	
	if (idx <0 || idx >= FE_VIP_PACKET_NUM)
		return -1;

	write_reg_word(FE_VIP_EN(idx),0);
	write_reg_word(FE_VIP_PATN(idx),0);
	return 0;
}

int fe_parse_vip_packet(char* buf){
	
	unsigned int i,len = 0;
	fe_vip_packet vip;
		

	for(i = 0; i < FE_VIP_PACKET_NUM;i++){

		fe_get_vip_packet(i,&vip);
		len += sprintf(buf+len,"FE VIP(%d): ",i);
		
		if (vip.en == 0){
			len += sprintf(buf+len,"Dis\n");
			continue;
		}

		if(vip.type == FE_VIP_TYPE_ETHER){
			len += sprintf(buf+len,"Ether Type(%x) \n",vip.etype);
			continue;
		}else if (vip.type == FE_VIP_TYPE_PPPOE){
			len += sprintf(buf+len,"PPPoE Type(%x) \n",vip.ppp);
			continue;
		}else if (vip.type == FE_VIP_TYPE_IP){
			len += sprintf(buf+len,"IP Type(%x) \n",vip.iptype);
			continue;
		}else if (vip.type == FE_VIP_TYPE_TCP){
			len += sprintf(buf+len,"TCP Type ");
		}else if(vip.type == FE_VIP_TYPE_UDP){
			len += sprintf(buf+len,"UDP Type ");
		}else{
			len += sprintf(buf+len,"Error Type \n");
			continue;
		}

		if(vip.sp_en){
			len += sprintf(buf+len,"SrcPort(%d) ",vip.sport);
		}
		
		if(vip.dp_en){
			len += sprintf(buf+len,"DstPort(%d) ",vip.dport);
		}
		
		if(vip.sw_en){
			len += sprintf(buf+len,"Swap Port");
		}	
		len += sprintf(buf+len,"\n");	
	}
	
	return 0;
}
/////////////////////////GDM L2 Offset Setting///////////////////////////////

int fe_set_l2lu_key(int idx ,fe_l2lu_key* key){

	unsigned int value;

//	if(key->dscp)
//		key->dscp_mask = 0xff;

	if(key->spcp)
		key->spcp_en= 1;	

	if(key->svid)
		key->svid_en= 1;		

	if(key->cpcp)
		key->cpcp_en= 1;	

	if(key->cvid)
		key->cvid_en= 1;	

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


int fe_add_l2lu_key(fe_l2lu_key* key){

	unsigned int value;
	int i;

	if(key == NULL)
		return -1;
	
	for(i = 0; i< FE_L2LU_KEY_NUM;i++){
		value = read_reg_word(PSE_L2LU_KEY0(i));
		if ((value & 0xffff) == 0)
			break;
	}
	
	if (i == FE_L2LU_KEY_NUM)
		return -1;

	return fe_set_l2lu_key(i,key);
}

int fe_get_l2lu_key(int idx,fe_l2lu_key* key){

	unsigned int value;
	
	if (idx <0 || idx >= FE_L2LU_KEY_NUM)
		return -1;

	value = read_reg_word(PSE_L2LU_KEY0(idx));
	*((unsigned int*)key) = value;
	value = read_reg_word(PSE_L2LU_KEY1(idx));
	*(((unsigned int*)key) + 1) = value;	
	return 0;
}

int fe_parse_l2lu_key(char* buf){

	unsigned int i,len = 0;
	fe_l2lu_key key;

	for(i = 0; i < FE_L2LU_KEY_NUM; i++){
		
		fe_get_l2lu_key(i,&key);
		
		len += sprintf(buf+len,"FE L2 Offset Key(%d): ",i);
		
		if (!(key.dscp_mask || key.spcp_en || key.svid_en || key.cpcp_en || key.cvid_en) ){
			len += sprintf(buf+len,"Disable \n");
			continue;
		}


		if(key.dscp_mask)
			len += sprintf(buf+len,"dscp = %d",key.dscp);

		if(key.cvid_en)
			len += sprintf(buf+len,"cvid = %d",key.cvid);
		
		if(key.cpcp_en)
			len += sprintf(buf+len,"cpcp = %d",key.cpcp);
	
		if(key.svid_en)
			len += sprintf(buf+len,"svid = %d",key.svid);	
		
		if(key.spcp_en)
			len += sprintf(buf+len,"svid = %d",key.spcp);			
	}
	return 0;
}


int fe_del_l2lu_key(int idx)
{
	if (idx <0 || idx >= FE_L2LU_KEY_NUM)
		return -1;
	
	write_reg_word(PSE_L2LU_KEY0(idx),0);
	write_reg_word(PSE_L2LU_KEY1(idx),0);
	return 0;
}

//////////////////////GDM2 Tx Rate Limit Setting //////////////////// 

int fe_set_wan_tx_rate(unsigned int rate,unsigned int mode){

	unsigned int value = 0,tick = 0;

	if (mode > 0)
		mode = 1;

	
	if(rate == 0){
		write_reg_word(GDMA2_SHRP_CFG,0);
	}else{
		value = (1 << GDMA_SHRP_CFG_EN_OFFSET) | (tick << GDMA_SHRP_CFG_TICK_OFFSET)
					|(rate << GDMA_SHRP_CFG_RATE_OFFSET) | 0xfe | mode;  

		write_reg_word(GDMA2_SHRP_CFG,value);
	}
	return 0;
}

int fe_parse_wan_tx_rate(char* buf){

	unsigned int reg = 0,len = 0,factor = 1;	
	
	len += sprintf(buf+len,"GDMA2 Tx Rate: ");

	reg = GDMA2_SHRP_CFG;
	


	if (fe_reg_get_bits(reg,GDMA_SHRP_CFG_EN_OFFSET,1) == 0) {
		len += sprintf(buf+len,"Unlimited!\n");
		return 0;
	}
	
	factor = fe_reg_get_bits(reg,GDMA_SHRP_CFG_TICK_OFFSET,2);
	if (factor == 1)
		factor = 10;
	else if (factor == 2)
		factor = 100;
	else if (factor == 3)
		factor = 1000;
	else
		factor = 1;
	
	len += sprintf(buf+len,"%d %s \n",
		fe_reg_get_bits(reg,GDMA_SHRP_CFG_RATE_OFFSET,GDMA_SHRP_CFG_RATE_LENGTH) / factor,
		fe_reg_get_bits(reg,GDMA_SHRP_CFG_RATE_OFFSET,1) ? "KBps" : "KPps");

	return 0;
}


////////////////////GDM2 Rx Rate Limit Setting//////////////////////

int fe_set_wan_rx_rate(unsigned int type,unsigned int rate,unsigned int mode){

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
	
	if(rate == 0){
		write_reg_word(reg,0);
	}else{
		value = (1 << GDMA_SHRP_CFG_EN_OFFSET) | (tick << GDMA_SHRP_CFG_TICK_OFFSET)
					|(rate << GDMA_SHRP_CFG_RATE_OFFSET) | 0xfe | mode;  

		write_reg_word(reg,value);
	}
	return 0;
}

int fe_parse_wan_rx_rate(int type,char* buf){

	unsigned int reg = 0,len = 0,factor = 1;	
	
	if (type == FE_WAN_RX_UC_RATE){
		reg = GDMA2_RXUC_SHPR_CFG;
		len += sprintf(buf+len,"GDMA2 RxUnicast Rate: ");
	}else if (type == FE_WAN_RX_BC_RATE){
		reg = GDMA2_RXBC_SHPR_CFG;
		len += sprintf(buf+len,"GDMA2 RxBroadCast Rate: ");
	}else if (type == FE_WAN_RX_MC_RATE){
		reg = GDMA2_RXMC_SHPR_CFG;
		len += sprintf(buf+len,"GDMA2 RxMulticast Rate: ");
	}else if  (type == FE_WAN_RX_OC_RATE){
		reg = GDMA2_RXOC_SHPR_CFG;
		len += sprintf(buf+len,"GDMA2 RxOtherUnicast Rate: ");
	}else{
		len += sprintf(buf+len,"GDMA2 Rx Rate Error! \n ");
		return -1;
	}

//	value = fe_reg_read(reg);
	if (fe_reg_get_bits(reg,GDMA_SHRP_CFG_EN_OFFSET,1) == 0) {
		len += sprintf(buf+len,"Unlimited!\n");
		return 0;
	}

	factor = fe_reg_get_bits(reg,GDMA_SHRP_CFG_TICK_OFFSET,2);
	if (factor == 1)
		factor = 10;
	else if (factor == 2)
		factor = 100;
	else if (factor == 3)
		factor = 1000;
	else
		factor =1;
	
	len += sprintf(buf+len," %d kBps! \n",
		fe_reg_get_bits(reg,GDMA_SHRP_CFG_RATE_OFFSET,GDMA_SHRP_CFG_RATE_LENGTH)/ factor);

	return 0;
}


int fe_get_packet_length(int pse_idx,unsigned int* shorter,unsigned int* longer){

	unsigned int reg;

	if (pse_idx == MT7512_FE_GDM1){
		reg = GDMA1_LEN_CFG;
	}else if(pse_idx == MT7512_FE_GDM2){
		reg = GDMA2_LEN_CFG;
	}else{
		return -1;
	}

	*shorter = fe_reg_get_bits(reg,GDMA_LEN_CFG_RUNT_OFFSET,GDMA_LEN_CFG_LENGTH);
	*longer = fe_reg_get_bits(reg,GDMA_LEN_CFG_LONG_OFFSET,GDMA_LEN_CFG_LENGTH);

	return 0;
}

int fe_parse_packet_length(int pse_idx,char* buf){

	unsigned int longer = 0,shorter = 0;
	
	fe_get_packet_length(pse_idx,&shorter,&longer);
	
	if (pse_idx == MT7512_FE_GDM1)
		sprintf(buf,"GDMA1 Length Cfg: Long(%u) Short(%u)\n",longer,shorter);
	else if(pse_idx == MT7512_FE_GDM2)
		sprintf(buf,"GDMA2 Length Cfg: Long(%u) Short(%u)\n",longer,shorter);
	else
		sprintf(buf,"GDMA Length Cfg: PSE Idx Error!\n");
	
	return 0;
}

/////////////////////////////////////
int fe_set_cpu_rx_rate(int pse_idx,int q1h,int q1,int q0){

	unsigned int reg;

	if (pse_idx == MT7512_FE_CDM1){
		reg = CDMA1_FWD_CFG;
	}else if(pse_idx == MT7512_FE_CDM2){
		reg = CDMA2_FWD_CFG;
	}else{
		return -1;
	}

	fe_reg_modify(reg,q1h,CDM_FWD_CFG_Q1H_OFFSET,CDM_FWD_CFG_Q_LENGTH);
	fe_reg_modify(reg,q1,CDM_FWD_CFG_Q1L_OFFSET,CDM_FWD_CFG_Q_LENGTH);
	fe_reg_modify(reg,q0,CDM_FWD_CFG_Q0_OFFSET,CDM_FWD_CFG_Q_LENGTH);

	return 0;
}

int fe_get_cpu_rx_rate(int pse_idx,int* q1h,int* q1,int* q0){

	unsigned int reg;

	if (pse_idx == MT7512_FE_CDM1){
		reg = CDMA1_FWD_CFG;
	}else if(pse_idx == MT7512_FE_CDM2){
		reg = CDMA2_FWD_CFG;
	}else{
		return -1;
	}

	*q1h = fe_reg_get_bits(reg,CDM_FWD_CFG_Q1H_OFFSET,CDM_FWD_CFG_Q_LENGTH);
	*q1 = fe_reg_get_bits(reg,CDM_FWD_CFG_Q1L_OFFSET,CDM_FWD_CFG_Q_LENGTH);
	*q0 = fe_reg_get_bits(reg,CDM_FWD_CFG_Q0_OFFSET,CDM_FWD_CFG_Q_LENGTH);


	return 0;
}

/////////////////////////////////////////////////
int fe_set_vlan_ctrl(int pse_idx,fe_vlan_ctrl* vlan){

	unsigned int reg,value = 0;

	if (pse_idx == MT7512_FE_CDM1){
		reg = CDMA1_VLAN_CTRL;
	}else if(pse_idx == MT7512_FE_CDM2){
		reg = CDMA2_VLAN_CTRL;
	}else if (pse_idx == MT7512_FE_GDM1){
		reg = GDMA1_VLAN_CHECK;
	}else if(pse_idx == MT7512_FE_GDM2){
		reg = GDMA2_VLAN_CHECK;
	}else{
		return -1;
	}

	value = vlan->sp_en << CDMA_VLAN_CTRL_SP_OFFSET;

	if(pse_idx == MT7512_FE_CDM1 || pse_idx == MT7512_FE_CDM2)
		value |= (vlan->tpid << CDMA_VLAN_CTRL_TPID_OFFSET) 
				|(vlan->rx_untag << CDMA_VLAN_CTRL_UNTAG_OFFSET);

	write_reg_word(reg,value);
	
	return 0;
}

int fe_parse_vlan_ctrl(int pse_idx,char* buff){

	unsigned int len = 0,reg =0;
	
	if (pse_idx == MT7512_FE_CDM1){
		reg = CDMA1_VLAN_CTRL;
		len += sprintf(buff + len,"CDMA1 Vlan Ctrl: SpecialTag(%s) RxUntag(%s) TPID(%04x)\n",
					fe_reg_get_bits(reg,CDMA_VLAN_CTRL_SP_OFFSET,1) ? "Enable":"Disable",
					fe_reg_get_bits(reg,CDMA_VLAN_CTRL_UNTAG_OFFSET,1) ? "Yes":"No",
					fe_reg_get_bits(reg,CDMA_VLAN_CTRL_TPID_OFFSET,16));
					
	}else if(pse_idx == MT7512_FE_CDM2){
		reg = CDMA2_VLAN_CTRL;
		len += sprintf(buff + len,"CDMA2 Vlan Ctrl: SpecialTag(%s) RxUntag(%s) TPID(%04x)\n",
				fe_reg_get_bits(reg,CDMA_VLAN_CTRL_SP_OFFSET,1) ? "Enable":"Disable",
				fe_reg_get_bits(reg,CDMA_VLAN_CTRL_UNTAG_OFFSET,1) ? "Yes":"No",
				fe_reg_get_bits(reg,CDMA_VLAN_CTRL_TPID_OFFSET,16));

	}else if (pse_idx == MT7512_FE_GDM1){
		reg = GDMA1_VLAN_CHECK;
		len += sprintf(buff + len,"GDMA1 Vlan Ctrl: SpecialTag(%s)\n",
					fe_reg_get_bits(reg,CDMA_VLAN_CTRL_SP_OFFSET,1) ? "Enable":"Disable");

	}else if(pse_idx == MT7512_FE_GDM2){
		reg = GDMA2_VLAN_CHECK;
		len += sprintf(buff + len,"GDMA2 Vlan Ctrl: SpecialTag(%s)\n",
					fe_reg_get_bits(reg,CDMA_VLAN_CTRL_SP_OFFSET,1) ? "Enable":"Disable");	
	}else{
		len += sprintf(buff + len,"CDMA&GDMA Vlan Ctrl: PSE Idx Error!\n");		
	}

	return 0;
}


int fe_clear_mib_counter(int pse_idx)
{
	unsigned int reg;
	 if (pse_idx == MT7512_FE_GDM1){
		reg = GDMA1_MIB_CLER;
	}else if(pse_idx == MT7512_FE_GDM2){
		reg = GDMA2_MIB_CLER;
	}else{
		return -1;
	}

	write_reg_word(reg,3);
	
	return 0;
}
//////////////////////////////////////////////////////////////////////
int fe_parse_pse_reg(void){

	char buff[1024];
	
	buff[0] = 0;
	fe_parse_port_buff(buff);
	printk("%s",buff);
	return 0;
}

int fe_parse_l2lu_reg(void){

	char buff[1024];
	
	buff[0] = 0;
	fe_parse_l2lu_key(buff);
	printk("%s",buff);
	return 0;
}

int fe_parse_vip_reg(void){

	char buff[2048];
	
	buff[0] = 0;
	fe_parse_vip_packet(buff);
	printk("%s",buff);
	return 0;
}



int fe_parse_cdm_reg(int pse_idx){
	
	char buff[1024];

	buff[0] = 0;
	
//	fe_parse_packet_length(pse_idx,buff);
	printk("%s",buff);
	buff[0] = 0;
	

	return 0;
}

int fe_parse_gdm_reg(int pse_idx){

	char buff[1024];
	
	buff[0] = 0;

	fe_parse_fwd_cfg(pse_idx,buff);
	printk("%s",buff);
	buff[0] = 0;

	if(pse_idx == MT7512_FE_GDM2){
		fe_parse_wan_tx_rate(buff);
		printk("%s",buff);
		buff[0] = 0;
	}
	
	fe_parse_mac_addr(pse_idx,buff);
	printk("%s",buff);
	buff[0] = 0;

	fe_parse_vlan_ctrl(pse_idx,buff);
	printk("%s",buff);
	buff[0] = 0;

	fe_parse_packet_length(pse_idx,buff);
	printk("%s",buff);
	buff[0] = 0;

	if(pse_idx == MT7512_FE_GDM1){
		



	}

	if(pse_idx == MT7512_FE_GDM2){
		fe_parse_wan_rx_rate(FE_WAN_RX_UC_RATE,buff);
		printk("%s",buff);
		buff[0] = 0;

		fe_parse_wan_rx_rate(FE_WAN_RX_BC_RATE,buff);
		printk("%s",buff);
		buff[0] = 0;		
		
		fe_parse_wan_rx_rate(FE_WAN_RX_MC_RATE,buff);
		printk("%s",buff);
		buff[0] = 0;

		fe_parse_wan_rx_rate(FE_WAN_RX_OC_RATE,buff);
		printk("%s",buff);
		buff[0] = 0;		
	}

	return 0;
}

//int pse_vlan_ctl = 0;
//EXPORT_SYMBOL(pse_vlan_ctl);

int fe_test_cmd_proc(char* cmd,char* subcmd,unsigned int v0,unsigned int v1,
							unsigned int v2,unsigned int v3){

	if(memcmp(cmd,"parse",5) == 0){
		if(memcmp(subcmd,"pse",3)==0){
			fe_parse_pse_reg();
		}else if(memcmp(subcmd,"l2lu",4)==0){
			fe_parse_l2lu_reg();
		}else if(memcmp(subcmd,"vip",3)==0){
			fe_parse_vip_reg();
		}else if(memcmp(subcmd,"cdm1",4)==0){
			fe_parse_cdm_reg(MT7512_FE_CDM1);
		}else if(memcmp(subcmd,"cdm2",4)==0){
			fe_parse_cdm_reg(MT7512_FE_CDM2);
		}else if(memcmp(subcmd,"gdm1",4)==0){
			fe_parse_cdm_reg(MT7512_FE_GDM1);
		}else if(memcmp(subcmd,"gdm2",4)==0){
			fe_parse_cdm_reg(MT7512_FE_GDM2);
		}else if(memcmp(subcmd,"all",3)==0){
			fe_parse_pse_reg();
			fe_parse_l2lu_reg();
			fe_parse_vip_reg();
			fe_parse_cdm_reg(MT7512_FE_CDM1);
			fe_parse_gdm_reg(MT7512_FE_GDM1);	
			fe_parse_cdm_reg(MT7512_FE_CDM2);
			fe_parse_gdm_reg(MT7512_FE_GDM2);
		}
	}else if(memcmp(cmd,"test ",4) == 0){
		if(memcmp(subcmd,"init",4)==0){
			fe_test_init(v0);
		}else if(memcmp(subcmd,"vip",3)==0){
			fe_test_vip(v0);
		}else if(memcmp(subcmd,"offload",7)==0){
			fe_test_offload();
		}else if(memcmp(subcmd,"error",5)==0){
			fe_test_error();
		}else if(memcmp(subcmd,"intr",4)==0){
			fe_test_interrupt(v0);
		}else if(memcmp(subcmd,"buff",4)==0){
			fe_test_buffer(v0,v1);
		}else if(memcmp(subcmd,"mib",3)==0){
			fe_test_mib();
		}else if(memcmp(subcmd,"dump",4)==0){
			fe_test_dump(v0);
		}else if(memcmp(subcmd,"send",4)==0){
			fe_test_send(v0,v1);
		}else if(memcmp(subcmd,"retire",6)==0){
			fe_test_channel_retire(v0,v1);
		}else if(memcmp(subcmd,"rxrate",6)==0){
			fe_set_wan_rx_rate(v0,v1,v2);
		}else if(memcmp(subcmd,"txrate",6)==0){
			fe_set_wan_tx_rate(v0,v1);
		}else if(memcmp(subcmd,"pcp",3)==0){
			fe_test_pcp(v0,v1);
		}else if(memcmp(subcmd,"l2lu",4)==0){
			fe_test_l2lu(v0,v1);
		}else if(memcmp(subcmd,"misc",4)==0){
			fe_test_misc(v0,v1);
		}else if(memcmp(subcmd,"cpu",3)==0){
			fe_test_cpu_protect(v0,v1);
		}else if(memcmp(subcmd,"isr",3)==0){
			if (v0){
				fe_test_register_isr();
			}else{
				fe_test_unregister_isr();
			}
		}
		
	}
	

	return 0;
}

static int fe_test_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	printk(" \n test init <0|1> ");
	printk(" \n test vip <idx>");
	printk(" \n test offload ");
	printk(" \n test error ");
	printk(" \n test intr <0|1> ");
	printk(" \n test buff <0|1> ");
	printk(" \n test mib ");
	printk(" \n test retire <0|1> ");
	printk(" \n test rxrate <0|1> ");
	printk(" \n test txrate <0|1> ");
	printk(" \n test pcp <0|1|2|3> ");
	printk(" \n test l2lu <idx> <length> ");
	printk(" \n test misc <0|1> ");
	printk(" \n test cpu <time> ");
	printk(" \n test isr <0|1> 0:Unreg 1:Reg");
	printk(" \n test send <mask> <length> ");
	printk(" \n test dump <flag> flag:Bit0 = Rx Bit1 = Tx \n");

	
	return 0;
}

static int fe_test_write_proc(struct file *file, const char *buffer,unsigned long count, void *data)
{
	char val_string[64],cmd[8],subcmd[8];
	unsigned int v0 =0 ,v1 = 0;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';
	sscanf(val_string,"%s %s %x %x",cmd, subcmd,&v0,&v1);

	fe_test_cmd_proc(cmd,subcmd,v0,v1,0,0);
	
	return count;
}



static int __init fe_init_test(void)
{
	struct proc_dir_entry *fe_proc;


	printk("FE Test Init\n");


	fe_proc = create_proc_entry("tc3162/fe_test", 0, NULL);
	fe_proc->read_proc = fe_test_read_proc;
	fe_proc->write_proc = fe_test_write_proc;

	return 0;
}

static void __exit fe_exit_test(void) 
{
	remove_proc_entry("tc3162/fe_test", 0);
	return;
}


/* Register startup/shutdown routines */
module_init(fe_init_test);
module_exit(fe_exit_test);


