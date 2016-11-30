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

#ifndef _LIBAPI_GPONMGR_H
#define _LIBAPI_GPONMGR_h

#include <xpon_global/public/xpon_if.h>

#define ECONET_E_SUCCESS	(0)
#define ECONET_E_FAIL	    (-1)

#define INVALID_TCONT       (0XFFFF)
#define MAX_IPC_PATH_LEN 64

typedef enum{
    BLACK_LIST_FILTER,
    WHITE_LIST_FILTER,
}Event_Filter_Type;

typedef enum{
    WAN_TX_CNT = 1,
    WAN_RX_CNT,
}WAN_TX_RX_SELECT_T,WAN_TX_RX_SELECT_t;

typedef enum{
    CONFIG_IPC_PATH = 1,
    /*add other config data mask here*/    
}gpon_lib_config_mask_t;
typedef struct{
    char ipc_path[MAX_IPC_PATH_LEN];
    /*add other config data here*/
}gpon_lib_config_t;
/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/
int gponmgr_lib_init(void);
int gponmgr_lib_advanced_init(gpon_lib_config_t * data,int mask,unsigned int size);
int gponmgr_lib_deinit(void);
int gponmgr_lib_init_omci_socket(int * socket_f);
int gponmgr_lib_omci_receive_msg_block(int socket, unsigned char * msg,
    unsigned int *msg_len);
int gponmgr_lib_omci_receive_block(unsigned char * msg, unsigned int *msg_len);
int gponmgr_lib_omci_send_msg_block(int socket, unsigned char * msg,
    unsigned int msg_len);
int gponmgr_lib_omci_send_block(unsigned char * msg, unsigned int msg_len);
int gponmgr_lib_bind_tcont_to_allocid(unsigned short tcont, unsigned short allocid);
int gponmgr_lib_unbind_tcont_to_allocid(unsigned short tcont);
int gponmgr_lib_delete_tcont(unsigned short tcont);
int gponmgr_lib_config_gemport(unsigned short portid, GPON_GemType_t type, unsigned short tcont);
int gponmgr_lib_delete_gemport(unsigned short portid);
int gponmgr_lib_get_gemport_statistic(GEM_STATISTIC_t* statistic);
int gponmgr_lib_get_wan_statistic(WAN_STATISTIC_t * pWanCntStats);
int gponmgr_lib_set_driver_event_filter(Event_Filter_Type filter_type,unsigned short * filter_list,unsigned short  list_len);
int gponmgr_lib_get_driver_event_block(XPON_EVENT_t  * event);
int gponmgr_lib_set_emergency_state(void);
int gponmgr_lib_reset_driver_data_service(void);
int gponmgr_lib_set_pon_link_state(XPON_Mode_t state);
int gponmgr_lib_set_sn(char *psn);
int gponmgr_lib_set_passwd(char *ppasswd);
int gponmgr_lib_get_info(GPON_ONU_INFO_t*gponOnuInfo);
int gponmgr_lib_get_phy_trans_params(PHY_PARAMS_t*pPhyTransParams);
int gponmgr_lib_set_clear_gem_cnt(unsigned short gemportID);
int gponmgr_lib_set_clear_wan_cnt(WAN_TX_RX_SELECT_t mask);
int gponmgr_lib_set_InitRepO1_Flag(int value);
int gponmgr_lib_get_phy_fec_status(PHY_FECSTATUS_t * fecStatus);


#endif
