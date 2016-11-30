/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attempt
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    
    hook.c

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Steven Liu  2006-10-06      Initial version
*/

#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/foe_hook.h>
#include <net/sock.h>

#ifdef TCSUPPORT_HWNAT_L2VID
int (*ra_sw_nat_hook_rx_set_l2lu) (struct sk_buff * skb) = NULL;
EXPORT_SYMBOL(ra_sw_nat_hook_rx_set_l2lu);
#endif
int (*ra_sw_nat_hook_rx) (struct sk_buff * skb) = NULL;
int (*ra_sw_nat_ds_offload) (struct sk_buff * skb, int *dp) = NULL;
int (*ra_sw_nat_hook_update_dp)(int index, int dp) = NULL;
int (*ra_sw_nat_hook_update_vlan)(int index,int outer_vlan,int inner_vlan) = NULL;

int (*ra_sw_nat_hook_save_rxinfo)(struct sk_buff *skb) = NULL;
int (*ra_sw_nat_hook_restore_rxinfo)(struct sk_buff *skb) = NULL;
int (*ra_sw_nat_hook_save_txinfo)(struct sk_buff *skb) = NULL;
int (*ra_sw_nat_hook_restore_txinfo)(struct sk_buff *skb) = NULL;
int (*ra_sw_nat_hook_is_hwnat_pkt)(struct sk_buff *skb) = NULL;
int (*ra_sw_nat_hook_sendto_ppe)(struct sk_buff *skb) = NULL;
int (*ra_sw_nat_hook_set_l2tp_dev)(struct net_device *dev) = NULL;
struct net_device* (*ra_sw_nat_hook_read_l2tp_dev)(void) = NULL;
int (*ra_sw_nat_rtsp_offload_restore) (struct sk_buff * skb) = NULL;
int (*ra_sw_nat_rtsp_data_handle) (struct sk_buff * skb, char *rb_ptr, unsigned int datalen) = NULL;

#ifdef TCSUPPORT_MT7510_FE
int (*ra_sw_nat_hook_tx) (struct sk_buff * skb, struct port_info * pinfo, int magic);
#else
int (*ra_sw_nat_hook_tx) (struct sk_buff * skb, int gmac_no) = NULL;
#endif
int (*ra_sw_nat_hook_free) (struct sk_buff * skb) = NULL;
int (*ra_sw_nat_hook_rxinfo) (struct sk_buff * skb, int magic, char *data, int data_length) = NULL;
int (*ra_sw_nat_hook_txq) (struct sk_buff * skb, int txq) = NULL;
int (*ra_sw_nat_hook_magic) (struct sk_buff * skb, int magic) = NULL;
int (*ra_sw_nat_hook_set_magic) (struct sk_buff * skb, int magic) = NULL;
int (*ra_sw_nat_hook_xfer) (struct sk_buff *skb, const struct sk_buff *prev_p) = NULL;
int (*ra_sw_nat_hook_is_alive_pkt)(unsigned int crsn) = NULL;
#ifdef TCSUPPORT_RA_HWNAT_ENHANCE_HOOK
int (*ra_sw_nat_hook_drop_packet) (struct sk_buff * skb) = NULL;
int (*ra_sw_nat_hook_clean_table) (void) = NULL;
#endif
int (*ra_sw_nat_hook_cpu_meter)(struct sk_buff* skb,FETxMsg_T* txMsg,struct port_info* pinfo,unsigned char dir,unsigned short mtrIndex,unsigned int rateScale) = NULL;

int is_hwnat_dont_clean = 0;
EXPORT_SYMBOL(is_hwnat_dont_clean);

int (*ra_sw_nat_hook_foeentry) (void * inputvalue,int operation) = NULL;
#ifdef TCSUPPORT_MT7510_FE
#ifdef TCSUPPORT_HWNAT_LED
int is_hwnat_led_enable = 1;
#else
int is_hwnat_led_enable = 0;
#endif // end of TCSUPPORT_HWNAT_LED
EXPORT_SYMBOL(is_hwnat_led_enable);
#endif // end of TCSUPPORT_MT7510_FE

//#if defined(TCSUPPORT_XPON_IGMP) || defined(TCSUPPORT_MULTICAST_SPEED)
int (*hwnat_is_alive_pkt_hook)(struct sk_buff* skb) = NULL;
int (*hwnat_skb_to_foe_hook)(struct sk_buff* skb) = NULL;
int (*hwnat_set_special_tag_hook)(int index, int tag) = NULL;
int (*hwnat_multicast_set_hwnat_info_hook)(int index, int mask) = NULL;
int (*hwnat_delete_foe_entry_hook)(int index) = NULL; 
int (*hwnat_is_multicast_entry_hook)(int index ,unsigned char* grp_addr,unsigned char* src_addr,int type) = NULL;
int (*hwnat_is_drop_entry_hook)(int index ,unsigned char* grp_addr,unsigned char* src_addr,int type) = NULL;
int  (*multicast_speed_find_entry_hook)(int index) = NULL;
int  (*multicast_speed_learn_flow_hook)(struct sk_buff* skb) = NULL;
int  (*hwnat_set_rule_according_to_state_hook)(int index, int state,int mask) = NULL;
int  (*hwnat_set_recover_info_hook)(struct sk_buff* skb,struct sock *sk,int flag) = NULL;
int  (*xpon_igmp_learn_flow_hook)(struct sk_buff* skb) = NULL;
int  (*xpon_igmp_exist)(void) = NULL;
int  (*xpon_igmp_clear_flows)(void) = NULL;
int (*hwnat_set_wlan_multicast_hook)(int index ,int flag) = NULL;
int (*wan_multicast_drop_hook)(struct sk_buff* skb) = NULL;
int (*wan_multicast_undrop_hook)(void) = NULL;
int (*wan_mvlan_change_hook)(void) = NULL;
int  (*multicast_flood_find_entry_hook)(int index) = NULL;
int  (*hwnat_set_multicast_speed_enable_hook)(int enable) = NULL;
int  (*multicast_flood_is_bind_hook)(int index) = NULL;

#if defined(TCSUPPORT_CPU_MT7520)
int (*MT7530LanPortMap2Switch_hook) (int port) = NULL;
EXPORT_SYMBOL(MT7530LanPortMap2Switch_hook);
#endif

#ifdef TCSUPPORT_MT7510_FE
void (*restore_offload_info_hook)(struct sk_buff *skb, struct port_info *pinfo, int magic) = NULL;
EXPORT_SYMBOL(restore_offload_info_hook);
#endif

EXPORT_SYMBOL(multicast_flood_is_bind_hook);
EXPORT_SYMBOL(multicast_flood_find_entry_hook);
EXPORT_SYMBOL(hwnat_set_multicast_speed_enable_hook);
EXPORT_SYMBOL(hwnat_is_alive_pkt_hook);
EXPORT_SYMBOL(hwnat_skb_to_foe_hook);
EXPORT_SYMBOL(hwnat_set_special_tag_hook);
EXPORT_SYMBOL(hwnat_multicast_set_hwnat_info_hook);
EXPORT_SYMBOL(hwnat_delete_foe_entry_hook);
EXPORT_SYMBOL(hwnat_is_multicast_entry_hook);
EXPORT_SYMBOL(hwnat_is_drop_entry_hook);
EXPORT_SYMBOL(multicast_speed_find_entry_hook);
EXPORT_SYMBOL(multicast_speed_learn_flow_hook);
EXPORT_SYMBOL(hwnat_set_rule_according_to_state_hook);
EXPORT_SYMBOL(hwnat_set_recover_info_hook);
EXPORT_SYMBOL(xpon_igmp_learn_flow_hook);
EXPORT_SYMBOL(xpon_igmp_exist);
EXPORT_SYMBOL(xpon_igmp_clear_flows);
EXPORT_SYMBOL(hwnat_set_wlan_multicast_hook);
EXPORT_SYMBOL(wan_multicast_drop_hook);
EXPORT_SYMBOL(wan_multicast_undrop_hook);
EXPORT_SYMBOL(wan_mvlan_change_hook);
//#endif

EXPORT_SYMBOL(ra_sw_nat_hook_rx);
EXPORT_SYMBOL(ra_sw_nat_ds_offload);
EXPORT_SYMBOL(ra_sw_nat_hook_update_dp);
EXPORT_SYMBOL(ra_sw_nat_hook_update_vlan);

EXPORT_SYMBOL(ra_sw_nat_hook_save_rxinfo);
EXPORT_SYMBOL(ra_sw_nat_hook_restore_rxinfo);
EXPORT_SYMBOL(ra_sw_nat_hook_save_txinfo);
EXPORT_SYMBOL(ra_sw_nat_hook_restore_txinfo);
EXPORT_SYMBOL(ra_sw_nat_hook_is_hwnat_pkt);
EXPORT_SYMBOL(ra_sw_nat_hook_sendto_ppe);
EXPORT_SYMBOL(ra_sw_nat_hook_set_l2tp_dev);
EXPORT_SYMBOL(ra_sw_nat_hook_read_l2tp_dev);
EXPORT_SYMBOL(ra_sw_nat_rtsp_offload_restore);
EXPORT_SYMBOL(ra_sw_nat_rtsp_data_handle);

EXPORT_SYMBOL(ra_sw_nat_hook_tx);
EXPORT_SYMBOL(ra_sw_nat_hook_free);
EXPORT_SYMBOL(ra_sw_nat_hook_rxinfo);
EXPORT_SYMBOL(ra_sw_nat_hook_txq);
EXPORT_SYMBOL(ra_sw_nat_hook_magic);
EXPORT_SYMBOL(ra_sw_nat_hook_set_magic);
EXPORT_SYMBOL(ra_sw_nat_hook_xfer);
EXPORT_SYMBOL(ra_sw_nat_hook_is_alive_pkt);
#ifdef TCSUPPORT_RA_HWNAT_ENHANCE_HOOK
EXPORT_SYMBOL(ra_sw_nat_hook_drop_packet);
EXPORT_SYMBOL(ra_sw_nat_hook_clean_table);
#endif
EXPORT_SYMBOL(ra_sw_nat_hook_foeentry);
EXPORT_SYMBOL(ra_sw_nat_hook_cpu_meter);

int (*hwnat_clean_lan_hook)(unsigned int) = NULL;
int (*hwnat_clean_wan_hook)(unsigned int) = NULL;
EXPORT_SYMBOL(hwnat_clean_lan_hook);
EXPORT_SYMBOL(hwnat_clean_wan_hook);

int hwnat_clean_lan(unsigned int port)
{
	if (hwnat_clean_lan_hook)
		return hwnat_clean_lan_hook(port);
	return 0;
}

int hwnat_clean_wan(unsigned int vid)
{
	if (hwnat_clean_wan_hook)
		return hwnat_clean_wan_hook(vid);
	return 0;
}
EXPORT_SYMBOL(hwnat_clean_lan);
EXPORT_SYMBOL(hwnat_clean_wan);


#ifndef TCSUPPORT_IPSEC_PASSTHROUGH
#include <net/mtk_esp.h>
//when not open vpn passthrough function,below function do nothing
void
ipsec_esp_ouput_finish_pt(ipsec_finishpara_t* inputParams)
{
	return;
}

void ipsec_esp_input_finish_pt(ipsec_finishpara_t* inputParams)
{
	return;
}
int ipsec_esp_output_pt(ipsec_para_t* ipsecparams)
{
	return -1;
}

int ipsec_esp_input_pt(ipsec_para_t* ipsecparams)
{
	return -1;
}

EXPORT_SYMBOL(ipsec_esp_input_pt);
EXPORT_SYMBOL(ipsec_esp_input_finish_pt);
EXPORT_SYMBOL(ipsec_esp_output_pt);
EXPORT_SYMBOL(ipsec_esp_ouput_finish_pt);
#endif


