/******************************************************************************
Filename:    voip_data_util.h
Description: send data to voip 
author:      Leibt
create date: 2013/8/24
modified date:
*******************************************************************************
                
Copyright (c) 2013 dare-tech
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "cfg_manager.h"


/* CODEC payload types */
typedef enum
{
   CODEC_PAYLOAD_PCMU        = 0,               /* G.711 mu-law 64 kbps   */
   CODEC_PAYLOAD_G726_32     = 2,               /* G.726-32               */
   CODEC_PAYLOAD_G723        = 4,               /* G.723                  */
   CODEC_PAYLOAD_PCMA        = 8,               /* G.711 A-law 64 kbps    */
   CODEC_PAYLOAD_G722        = 9,               /* G.722                  */
   CODEC_PAYLOAD_CNA         = 13,              /* Comfort noise          */
   CODEC_PAYLOAD_G728        = 15,              /* G.728                  */
   CODEC_PAYLOAD_G729        = 18,              /* G.729                  */
   CODEC_PAYLOAD_CN          = 19,              /* Comfort noise          */
   CODEC_PAYLOAD_G726_16     = 97,              /* G.726-16               */
   CODEC_PAYLOAD_G726_24     = 98,              /* G.726-24               */
   CODEC_PAYLOAD_ILBC        = 104,             /* iLBC                   */
   CODEC_PAYLOAD_AMRWB       = 105,             /* AMRWB                  */
   CODEC_PAYLOAD_T38,
   CODEC_PAYLOAD_T38_MUTE,
   CODEC_PAYLOAD_INVALID     = 0xff
} CODEC_PAYLOAD_TYPE;

typedef struct mtk_codec_map_ {
    char codec_string[16];
    CODEC_PAYLOAD_TYPE codec_payload;
} mtk_codec_map ;


void send_data_init_finished();

void init_communicate_2voip();
void init_msg_2vpcfg();

void voip_sip_configure_update(int class);

//forceSend : 1: send ip to voip even don't get wan ip,0:if no wan ip ,don't send
int send_wan_ip_2voip(unsigned char forceSend);
int send_wan_dns_2voip(unsigned char forceSend);

void send_lan_ip_2voip();
//VoIPBasic
void send_priary_outband_proxy_ip_2voip(void);
void send_priary_outband_proxy_port_2voip();
void send_auth_name_2voip(int line);   //line : start from 0 
void send_auth_passwd_2voip(int line);
void send_telnum_2voip(int line);
void send_line_enable(int line);
void send_standby_outband_proxy_ip_2voip(void);
void send_standby_outband_proxy_port_2voip();
void send_pack_enable_2voip();
void send_heart_beat_2voip();
void send_sip_transaction_type_2voip();
void send_pri_reg_serv_ip_2voip(void);
void send_pri_reg_serv_port_2voip();
void send_standby_reg_serv_ip_2voip(void);
void send_standby_reg_serv_port_2voip();

void send_session_update_2voip();

void send_rtp_port_range_max_2voip();
void send_rtp_port_range_min_2voip();



//VoIPCallCtrl
void send_call_forward_type_2voip(int line);
void send_hook_flash_min_time_2voip(int line);
void send_hook_flash_max_time_2voip(int line);

//VoIPMedia
void send_codec_priority_2voip(int line);

//VoIPAdvanced
void send_dtmf_mode_2voip();
void send_fax_mode_2voip();
void send_call_waiting_enable_2voip(int line);
void send_polarity_reverse_2voip() ;
void send_dns_serv_2voip();
void send_codec_pkg_time_2voip(int line);
void send_voice_volume_speak_2voip(int line);
void send_voice_volume_listen_2voip(int line);
void send_echo_cancel_enable_2voip(int line);
void send_reg_retry_timer_2voip();
void send_reg_expire_2voip();
void send_CNG_enable_2voip(int line);
void send_SLC_enable_2voip(int line); 

//VoIPDigitMap
void send_digitmap_2voip(int line);
void send_digit_long_timer_2voip();
void send_digit_short_timer_2voip();
void send_digit_start_timer_2voip();
void send_busytone_time_timer_2voip();
void send_hangingremindertone_timeout_timer_2voip();

void send_voice_sip_dscp_2voip();

int voip_sip_simultate_call_test();
void voip_sip_iad_test();