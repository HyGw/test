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
#ifndef _QDMA_WAN_LAN_FUNC_H_
#define _QDMA_WAN_LAN_FUNC_H_

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/

/**********************************************************
    function
**********************************************************/
#ifdef QDMA_LAN     // qdma1
/* qos */
#define qdmamgr_lib_set_qos_config qdmamgr_lan_lib_set_qos_config
#define qdmamgr_lib_set_qos_value qdmamgr_lan_lib_set_qos_value
#define qdmamgr_lib_get_qos qdmamgr_lan_lib_get_qos
/* tx ratelimit */
#define qdmamgr_lib_set_tx_ratemeter qdmamgr_lan_lib_set_tx_ratemeter
#define qdmamgr_lib_get_tx_ratemeter qdmamgr_lan_lib_get_tx_ratemeter
#define qdmamgr_lib_set_tx_ratelimit_config qdmamgr_lan_lib_set_tx_ratelimit_config
#define qdmamgr_lib_set_tx_ratelimit_value qdmamgr_lan_lib_set_tx_ratelimit_value
#define qdmamgr_lib_get_tx_ratelimit_config qdmamgr_lan_lib_get_tx_ratelimit_config
#define qdmamgr_lib_get_tx_ratelimit qdmamgr_lan_lib_get_tx_ratelimit
/* rx ratelimit */
#define qdmamgr_lib_set_rx_ratelimit_en qdmamgr_lan_lib_set_rx_ratelimit_en
#define qdmamgr_lib_set_rx_ratelimit_pkt_mode qdmamgr_lan_lib_set_rx_ratelimit_pkt_mode
#define qdmamgr_lib_set_rx_ratelimit_value qdmamgr_lan_lib_set_rx_ratelimit_value
#define qdmamgr_lib_get_rx_ratelimit qdmamgr_lan_lib_get_rx_ratelimit
/* virtual channel */
#define qdmamgr_lib_set_virtual_channel_config qdmamgr_lan_lib_set_virtual_channel_config
#define qdmamgr_lib_set_virtual_channel_value qdmamgr_lan_lib_set_virtual_channel_value
#define qdmamgr_lib_get_virtual_channel qdmamgr_lan_lib_get_virtual_channel
/* other */
#define qdmamgr_lib_set_rx_low_threshold qdmamgr_lan_lib_set_rx_low_threshold
#define qdmamgr_lib_get_rx_low_threshold qdmamgr_lan_lib_get_rx_low_threshold
#define qdmamgr_lib_set_dei_drop_en qdmamgr_lan_lib_set_dei_drop_en
#define qdmamgr_lib_get_dei_drop_en qdmamgr_lan_lib_get_dei_drop_en
#define qdmamgr_lib_get_lmgr_status qdmamgr_lan_lib_get_lmgr_status
#define qdmamgr_lib_set_lmgr_low_threshold qdmamgr_lan_lib_set_lmgr_low_threshold
#define qdmamgr_lib_get_lmgr_low_threshold qdmamgr_lan_lib_get_lmgr_low_threshold
#define qdmamgr_lib_set_overhead_en qdmamgr_lan_lib_set_overhead_en
#define qdmamgr_lib_get_overhead_en qdmamgr_lan_lib_get_overhead_en
#define qdmamgr_lib_set_overhead_value qdmamgr_lan_lib_set_overhead_value
#define qdmamgr_lib_get_overhead_value qdmamgr_lan_lib_get_overhead_value
#define qdmamgr_lib_set_mac_resource_limit qdmamgr_lan_lib_set_mac_resource_limit
#define qdmamgr_lib_get_mac_resource_limit qdmamgr_lan_lib_get_mac_resource_limit
#define qdmamgr_lib_set_txbuf_threshold qdmamgr_lan_lib_set_txbuf_threshold
#define qdmamgr_lib_get_txbuf_threshold qdmamgr_lan_lib_get_txbuf_threshold
#define qdmamgr_lib_set_prefetch qdmamgr_lan_lib_set_prefetch
#define qdmamgr_lib_set_dbareport qdmamgr_lan_lib_set_dbareport
#define qdmamgr_lib_dump_dbareport qdmamgr_lan_lib_dump_dbareport
/* test */
#define qdmamgr_lib_set_test_dbglevel qdmamgr_lan_lib_set_test_dbglevel
#define qdmamgr_lib_set_test_dma_dusy qdmamgr_lan_lib_set_test_dma_dusy
#define qdmamgr_lib_set_test_reg_polling qdmamgr_lan_lib_set_test_reg_polling
#define qdmamgr_lib_set_test_force_rx1 qdmamgr_lan_lib_set_test_force_rx1
/* txq cngst*/
#define qdmamgr_lib_set_txq_cngst_mode qdmamgr_lan_lib_set_txq_cngst_mode
#define qdmamgr_lib_set_txq_cngst_dei_scale qdmamgr_lan_lib_set_txq_cngst_dei_scale
#define qdmamgr_lib_set_txq_cngst_autoconfig qdmamgr_lan_lib_set_txq_cngst_autoconfig
#define qdmamgr_lib_set_txq_cngst_total_threshold qdmamgr_lan_lib_set_txq_cngst_total_threshold
#define qdmamgr_lib_set_txq_cngst_channel_threshold qdmamgr_lan_lib_set_txq_cngst_channel_threshold
#define qdmamgr_lib_set_txq_cngst_queue_threshold qdmamgr_lan_lib_set_txq_cngst_queue_threshold
#define qdmamgr_lib_set_txq_cngst_peekrate qdmamgr_lan_lib_set_txq_cngst_peekrate
#define qdmamgr_lib_set_static_cngst_normal qdmamgr_lan_lib_set_static_cngst_normal
#define qdmamgr_lib_set_static_cngst_dei qdmamgr_lan_lib_set_static_cngst_dei
#define qdmamgr_lib_get_txq_cngst qdmamgr_lan_lib_get_txq_cngst
/* dbg cntr */
#define qdmamgr_lib_set_dbgcntr_channel qdmamgr_lan_lib_set_dbgcntr_channel
#define qdmamgr_lib_set_dbgcntr_queue qdmamgr_lan_lib_set_dbgcntr_queue
#define qdmamgr_lib_clear_dbgcntr qdmamgr_lan_lib_clear_dbgcntr
#define qdmamgr_lib_dump_dbgcntr qdmamgr_lan_lib_dump_dbgcntr
/* dump */
#define qdmamgr_lib_clear_cpu_counter qdmamgr_lan_lib_clear_cpu_counter
#define qdmamgr_lib_dump_cpu_counter qdmamgr_lan_lib_dump_cpu_counter
#define qdmamgr_lib_dump_register qdmamgr_lan_lib_dump_register
#define qdmamgr_lib_dump_descriptor qdmamgr_lan_lib_dump_descriptor
#define qdmamgr_lib_dump_irq qdmamgr_lan_lib_dump_irq
#define qdmamgr_lib_dump_hwfwd qdmamgr_lan_lib_dump_hwfwd
#define qdmamgr_lib_dump_all qdmamgr_lan_lib_dump_all
#else               // qdma2
/* qos */
#define qdmamgr_lib_set_qos_config qdmamgr_wan_lib_set_qos_config
#define qdmamgr_lib_set_qos_value qdmamgr_wan_lib_set_qos_value
#define qdmamgr_lib_get_qos qdmamgr_wan_lib_get_qos
/* tx ratelimit */
#define qdmamgr_lib_set_tx_ratemeter qdmamgr_wan_lib_set_tx_ratemeter
#define qdmamgr_lib_get_tx_ratemeter qdmamgr_wan_lib_get_tx_ratemeter
#define qdmamgr_lib_set_tx_ratelimit_config qdmamgr_wan_lib_set_tx_ratelimit_config
#define qdmamgr_lib_set_tx_ratelimit_value qdmamgr_wan_lib_set_tx_ratelimit_value
#define qdmamgr_lib_get_tx_ratelimit_config qdmamgr_wan_lib_get_tx_ratelimit_config
#define qdmamgr_lib_get_tx_ratelimit qdmamgr_wan_lib_get_tx_ratelimit
/* rx ratelimit */
#define qdmamgr_lib_set_rx_ratelimit_en qdmamgr_wan_lib_set_rx_ratelimit_en
#define qdmamgr_lib_set_rx_ratelimit_pkt_mode qdmamgr_wan_lib_set_rx_ratelimit_pkt_mode
#define qdmamgr_lib_set_rx_ratelimit_value qdmamgr_wan_lib_set_rx_ratelimit_value
#define qdmamgr_lib_get_rx_ratelimit qdmamgr_wan_lib_get_rx_ratelimit
/* virtual channel */
#define qdmamgr_lib_set_virtual_channel_config qdmamgr_wan_lib_set_virtual_channel_config
#define qdmamgr_lib_set_virtual_channel_value qdmamgr_wan_lib_set_virtual_channel_value
#define qdmamgr_lib_get_virtual_channel qdmamgr_wan_lib_get_virtual_channel
/* other */
#define qdmamgr_lib_set_rx_low_threshold qdmamgr_wan_lib_set_rx_low_threshold
#define qdmamgr_lib_get_rx_low_threshold qdmamgr_wan_lib_get_rx_low_threshold
#define qdmamgr_lib_set_dei_drop_en qdmamgr_wan_lib_set_dei_drop_en
#define qdmamgr_lib_get_dei_drop_en qdmamgr_wan_lib_get_dei_drop_en
#define qdmamgr_lib_get_lmgr_status qdmamgr_wan_lib_get_lmgr_status
#define qdmamgr_lib_set_lmgr_low_threshold qdmamgr_wan_lib_set_lmgr_low_threshold
#define qdmamgr_lib_get_lmgr_low_threshold qdmamgr_wan_lib_get_lmgr_low_threshold
#define qdmamgr_lib_set_overhead_en qdmamgr_wan_lib_set_overhead_en
#define qdmamgr_lib_get_overhead_en qdmamgr_wan_lib_get_overhead_en
#define qdmamgr_lib_set_overhead_value qdmamgr_wan_lib_set_overhead_value
#define qdmamgr_lib_get_overhead_value qdmamgr_wan_lib_get_overhead_value
#define qdmamgr_lib_set_mac_resource_limit qdmamgr_wan_lib_set_mac_resource_limit
#define qdmamgr_lib_get_mac_resource_limit qdmamgr_wan_lib_get_mac_resource_limit
#define qdmamgr_lib_set_txbuf_threshold qdmamgr_wan_lib_set_txbuf_threshold
#define qdmamgr_lib_get_txbuf_threshold qdmamgr_wan_lib_get_txbuf_threshold
#define qdmamgr_lib_set_prefetch qdmamgr_wan_lib_set_prefetch
#define qdmamgr_lib_set_dbareport qdmamgr_wan_lib_set_dbareport
#define qdmamgr_lib_dump_dbareport qdmamgr_wan_lib_dump_dbareport
/* test */
#define qdmamgr_lib_set_test_dbglevel qdmamgr_wan_lib_set_test_dbglevel
#define qdmamgr_lib_set_test_dma_dusy qdmamgr_wan_lib_set_test_dma_dusy
#define qdmamgr_lib_set_test_reg_polling qdmamgr_wan_lib_set_test_reg_polling
#define qdmamgr_lib_set_test_force_rx1 qdmamgr_wan_lib_set_test_force_rx1
/* txq cngst*/
#define qdmamgr_lib_set_txq_cngst_mode qdmamgr_wan_lib_set_txq_cngst_mode
#define qdmamgr_lib_set_txq_cngst_dei_scale qdmamgr_wan_lib_set_txq_cngst_dei_scale
#define qdmamgr_lib_set_txq_cngst_autoconfig qdmamgr_wan_lib_set_txq_cngst_autoconfig
#define qdmamgr_lib_set_txq_cngst_total_threshold qdmamgr_wan_lib_set_txq_cngst_total_threshold
#define qdmamgr_lib_set_txq_cngst_channel_threshold qdmamgr_wan_lib_set_txq_cngst_channel_threshold
#define qdmamgr_lib_set_txq_cngst_queue_threshold qdmamgr_wan_lib_set_txq_cngst_queue_threshold
#define qdmamgr_lib_set_txq_cngst_peekrate qdmamgr_wan_lib_set_txq_cngst_peekrate
#define qdmamgr_lib_set_static_cngst_normal qdmamgr_wan_lib_set_static_cngst_normal
#define qdmamgr_lib_set_static_cngst_dei qdmamgr_wan_lib_set_static_cngst_dei
#define qdmamgr_lib_get_txq_cngst qdmamgr_wan_lib_get_txq_cngst
/* dbg cntr */
#define qdmamgr_lib_set_dbgcntr_channel qdmamgr_wan_lib_set_dbgcntr_channel
#define qdmamgr_lib_set_dbgcntr_queue qdmamgr_wan_lib_set_dbgcntr_queue
#define qdmamgr_lib_clear_dbgcntr qdmamgr_wan_lib_clear_dbgcntr
#define qdmamgr_lib_dump_dbgcntr qdmamgr_wan_lib_dump_dbgcntr
/* dump */
#define qdmamgr_lib_clear_cpu_counter qdmamgr_wan_lib_clear_cpu_counter
#define qdmamgr_lib_dump_cpu_counter qdmamgr_wan_lib_dump_cpu_counter
#define qdmamgr_lib_dump_register qdmamgr_wan_lib_dump_register
#define qdmamgr_lib_dump_descriptor qdmamgr_wan_lib_dump_descriptor
#define qdmamgr_lib_dump_irq qdmamgr_wan_lib_dump_irq
#define qdmamgr_lib_dump_hwfwd qdmamgr_wan_lib_dump_hwfwd
#define qdmamgr_lib_dump_all qdmamgr_wan_lib_dump_all
#endif


/**********************************************************
    export symbol
**********************************************************/

/************************************************************************
*                  M A C R O S
*************************************************************************
*/

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N    D E C L A R A T I O N S
#                  I N L I N E    F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#endif /* _QDMA_WAN_LAN_FUNC_H_ */
