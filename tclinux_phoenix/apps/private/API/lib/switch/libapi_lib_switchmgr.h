/*
** $Id: $
*/
/************************************************************************
 *
 *	Copyright (C) 2012 Mediatek Inc.
 *	All Rights Reserved.
 *
 * Mediatek Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Mediatek Inc.
 *
 *************************************************************************/
/*
** $Log$
**
 */
#ifndef _API_LIB_SWITCHMGR_H
#define _API_LIB_SWITCHMGR_H

//#ifdef TCSUPPORT_MT7530_SWITCH_API

#ifndef u32
typedef unsigned int u32;
#endif

#ifndef u16
typedef unsigned short u16;
#endif

#ifndef u8
typedef unsigned char u8;
#endif

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/



/************************************************************************
*                            M A C R O S
*************************************************************************
*/


/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/
typedef struct
{
	u32 cmdid;
	u32 PortQueueId;	// port id, or queue id, or ...
	void *paramext_ptr; // pointer to any paramext struct, or just 32-bit variables
	int ret_value;
} switch_api_params;

typedef struct
{
	u32 p1;
	u32 p2;
} switch_api_paramext2;

typedef struct
{
	u32 p1;
	u32 p2;
	u32 p3;
	u32 p4;
} switch_api_paramext4;

typedef struct
{
    //u64 TxPktsByte;
    u32 TxPktsCnt;
    u32 TxUniPktsCnt;
    u32 TxBroadPktsCnt;
    u32 TxMultiPktsCnt;	
    u32 TxNonUniPktsCnt;
	
    u32 TxUnderSizePktsCnt;
    u32 Tx64BytePktsCnt;
    u32 Tx65_127BytePktsCnt;
    u32 Tx128_255BytePktsCnt;
    u32 Tx256_511BytePktsCnt;
    u32 Tx512_1023BytePktsCnt;
    u32 Tx1024_1518BytePktsCnt;
    u32 TxOverSizePktsCnt;
	
    u32 TxDropFramesCnt;
    u32 TxPauseFramesCnt;
    u32 TxCRCFramesCnt;
    u32 TxSingleCollsionEvent;
    u32 TxMultiCollsionEvent;
    
    //u64 RTxPktsByte;
    u32 RxPktsCnt;
    u32 RxUniPktsCnt;
    u32 RxBroadPktsCnt;
    u32 RxMultiPktsCnt;
    u32 RxNonUniPktsCnt;
	
    u32 RxUnderSizePktsCnt;
    u32 Rx64BytePktsCnt;
    u32 Rx65_127BytePktsCnt;
    u32 Rx128_255BytePktsCnt;
    u32 Rx256_511BytePktsCnt;
    u32 Rx512_1023BytePktsCnt;
    u32 Rx1024_1518BytePktsCnt;
    u32 RxOverSizePktsCnt;
	
    u32 RxDropFramesCnt;
    u32 RxPauseFramesCnt;
    u32 RxCRCFramesCnt;

}switch_api_cnt;

typedef struct
{
	u8 port;
	u8 tx_enable;
	u8 rx_enable;
}
switch_srcports;

typedef struct
{
	u8 enable;
	switch_srcports srcport[7];
	u8 srcportnum;
	u8 dstport;
}
switch_lan2lan_info;

typedef struct
{
	u8 srcport;
	u8 tx_enable;
	u8 rx_enable;
	u8 dstport;
}switch_lan2lan;

typedef struct
{
	u8 port;
	u8 timer;
	u8 mac[6];
	u16 vid;
}switch_macinfo;

typedef struct
{
	u32 count;
	switch_macinfo macinfo[2048];
}switch_brtable;

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

// user API start from ID 0x0010
/*___________________________________________________________________
**      function name: switchmgr_lib_get_port_ratelimit
**      descriptions:
**         set switch port rate limit
**
**      parameters:
**		port: switch port index
**		dir:	0:tx
**			1:rx
**		rate: return limit rate
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_port_ratelimit(u8 port, u32 dir, u32 *rate);
/*___________________________________________________________________
**      function name: switchmgr_lib_set_port_ratelimit
**      descriptions:
**         set switch port rate limit
**
**      parameters:
**		port: switch port index
**		rate: limit rate kbps
**		dir:	0:tx
**			1:rx
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_ratelimit(u8 port, u32 rate, u8 dir);
/*___________________________________________________________________
**      function name: switchmgr_lib_set_port_qos
**      descriptions:
**         set switch port qos function enable
**
**      parameters:
**         0 : disable internal switch port qos feature
**         1 : enable internal switch port qos feature, default sp mode
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_qos(u8 port, u32 enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_loopback
**      descriptions:
**         set switch port loopback enable
**
**      parameters:
**         port: switch port index
**         enable:  0 : disable switch port loopback feature
**         		     1 : enable switch port loopback feature
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_loopback(u8 port, u8 enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_loopback
**      descriptions:
**         get switch port loopback enable
**
**      parameters:
**         port: switch port index, port0~port4
**         enable:  0 :  switch port loopback feature is disable
**         		     1 :  switch port loopback feature is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_loopback(u8 port, u8 *outptr_enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_port_enable
**      descriptions:
**         set switch port phy enable
**
**      parameters:
**         port: switch port index, port0~port4
**         enable:  0 :  set switch port phy disable
**         		     1 :  set switch port phy enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_enable(u8 port, u8 enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_port_enable
**      descriptions:
**         get switch port phy enable
**
**      parameters:
**         port: switch port index
**         enable:  0 :  switch port phy is disable
**         		     1 :  switch port phy is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_port_enable(u8 port, u8 *outptr_enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_port_speed_mode
**      descriptions:
**         set switch port work mode
**
**      parameters:
**         port: switch port index
**         mode:  1 :  Auto negotiation
**         		   2 :  100M Full
**         		   3 :  100M Half
**         		   4 :  10M Full
**         		   5 :  10M Half
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_speed_mode(u8 port, u8 mode);
/*___________________________________________________________________
**      function name: switchmgr_lib_get_port_speed_mode
**      descriptions:
**         get switch port work mode
**
**      parameters:
**         port: switch port index
**         mode:  1 :  Auto negotiation
**         		   2 :  100M Full
**         		   3 :  100M Half
**         		   4 :  10M Full
**         		   5 :  10M Half
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_port_speed_mode(u8 port, u8 *outptr_speedmode);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_link_state
**      descriptions:
**         get lan port link state
**
**      parameters:
**         port: port index
**         outptr_linkst:   0 :  Down
**         		   	       1 :  Up
**         		  
**         outptr_speed:  meaningful when link state is up
** 					 1:  1000M FULL
**         		  		 2 : 100M FULL
**         		  		 3 : 100M HALF
**         		  		 4 : 10M FULL
**         		  		 5 : 10M HALF
**         		  		 6 : 1000M HALF
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_link_state(u8 port, u8 *outptr_linkst, u8 *outptr_speed);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_port_follow_control_global
**      descriptions:
**         set switch port follow control global switch, for port0~port4
**
**      parameters:
**         enable:  0 :  set switch port follow control global disable
**         		     1 :  set switch port follow control global enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_follow_control_global(u8 enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_port_follow_control_global
**      descriptions:
**         get switch port follow control global switch, for port0~port4
**
**      parameters:
**         enable:  0 :  switch port follow control global is disable
**         		     1 :  switch port follow control global is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_port_follow_control_global(u8 *outptr_enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_port_follow_control
**      descriptions:
**         set switch port follow control
**
**      parameters:
**         port: switch port index, for port0~4,need to set follow control global first
**         enable:  0 :  switch port follow control is disable
**         		     1 :  switch port follow control is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_follow_control(u8 port, u8 enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_port_follow_control
**      descriptions:
**         get switch port follow control
**
**      parameters:
**         port: switch port index
**         enable:  0 :  switch port follow control is disable
**         		     1 :  switch port follow control is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_port_follow_control(u8 port, u8 *outptr_enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_lan2lan_clear
**      descriptions:
**         clear lan to lan rules
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_lan2lan_clear(void);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_lan2lan
**      descriptions:
**         set lan to lan rules
**
**      parameters:switch_lan2lan:1:srcport:source port to mirror
**					              2:tx_enable: source port tx enable to mirror
**					              3:rx_enable: source port rx enable to mirror
**					              4:dstport: mirror to destination port
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_lan2lan(switch_lan2lan lan2lan);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_lan2lan
**      descriptions:
**         get lan to lan rules
**
**      parameters:switch_lan2lan:1:enable:  0: lan2lan disable  1: lan2lan enable
**							 2:srcport:1:mirror source port idx
**							 		 2:mirror source port tx_enable
**							 		 3:mirror source port rx_enable
**					              3:srcportnum: total source port number
**					              4:dstport: mirror to destination port
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_lan2lan(switch_lan2lan_info *outptr_lan2lan);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_all_port_statistics
**      descriptions:
**         get port statistics
**
**      parameters:switch_lan2lan:1: RxPktsCnt
**							 2: TxPktsCnt
**							 3: TxDropFramesCnt
**							 4: RxDropFramesCnt
**					              5: RxLackDropPktCnt
**					              6: RxUniPktsCnt
**							 7: TxUniPktsCnt
**							 8: RxBroadPktsCnt
**							 9: TxBroadPktsCnt
**					              10: RxMultiPktsCnt
**					              11: TxMultiPktsCnt

**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_all_port_statistics(u8 port, switch_api_cnt *portcnt);

/*___________________________________________________________________
**      function name: switchmgr_lib_set_all_port_statistics_clear
**      descriptions:
**         clear all port statistics
**
**      parameters:
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_all_port_statistics_clear(void);

/*
**      function name: switchmgr_lib_get_loopback
**      descriptions:
**         get switch port loopback enable
**
**      parameters:
**         port: switch port index, port0~port4
**         enable:  0 :  switch port loopback feature is disable
**         		     1 :  switch port loopback feature is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_isolate(u8 enable);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_loopback
**      descriptions:
**         get switch port loopback enable
**
**      parameters:
**         port: switch port index, port0~port4
**         enable:  0 :  switch port loopback feature is disable
**         		     1 :  switch port loopback feature is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_port_mac_limit(u8 port, u32 cnt);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_loopback
**      descriptions:
**         get switch port loopback enable
**
**      parameters:
**         port: switch port index, port0~port4
**         enable:  0 :  switch port loopback feature is disable
**         		     1 :  switch port loopback feature is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_set_mac_ageout_time(u32 MaxAgeTime);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_loopback
**      descriptions:
**         get switch port loopback enable
**
**      parameters:
**         port: switch port index, port0~port4
**         enable:  0 :  switch port loopback feature is disable
**         		     1 :  switch port loopback feature is enable
**
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_clr_mac_table(void);

/*___________________________________________________________________
**      function name: switchmgr_lib_get_lan2lan
**      descriptions:
**         get switch bridge table
**
**      parameters:switch_brtable:1:port:  LAN port index
**							2:timer: ageing timer
**					              3:mac: MAC
** 				port: LAN port index, port1~port4
**      global:
**
**      return:
**         -1:fail
**         0:succuces
**
**      call:
**
**      revision:
**_______________________________________________________________
*/
int switchmgr_lib_get_brtable(switch_brtable *outptr_brtable, u8 port);

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/



// ==============redefine API for FH =================
#define ecnt_switchmgr_lib_get_port_ratelimit switchmgr_lib_get_port_ratelimit
#define ecnt_switchmgr_lib_set_port_ratelimit switchmgr_lib_set_port_ratelimit
#define ecnt_switchmgr_lib_set_port_qos switchmgr_lib_set_port_qos
#define ecnt_switchmgr_lib_set_loopback switchmgr_lib_set_loopback
#define ecnt_switchmgr_lib_get_loopback switchmgr_lib_get_loopback
#define ecnt_switchmgr_lib_set_port_enable switchmgr_lib_set_port_enable
#define ecnt_switchmgr_lib_get_port_enable switchmgr_lib_get_port_enable
#define ecnt_switchmgr_lib_set_port_speed_mode switchmgr_lib_set_port_speed_mode
#define ecnt_switchmgr_lib_get_port_speed_mode switchmgr_lib_get_port_speed_mode
#define ecnt_switchmgr_lib_get_link_state switchmgr_lib_get_link_state
#define ecnt_switchmgr_lib_set_port_follow_control_global switchmgr_lib_set_port_follow_control_global
#define ecnt_switchmgr_lib_get_port_follow_control_global switchmgr_lib_get_port_follow_control_global
#define ecnt_switchmgr_lib_set_port_follow_control switchmgr_lib_set_port_follow_control
#define ecnt_switchmgr_lib_get_port_follow_control switchmgr_lib_get_port_follow_control
#define ecnt_switchmgr_lib_set_lan2lan_clear switchmgr_lib_set_lan2lan_clear
#define ecnt_switchmgr_lib_set_lan2lan switchmgr_lib_set_lan2lan
#define ecnt_switchmgr_lib_get_lan2lan switchmgr_lib_get_lan2lan
#define ecnt_switchmgr_lib_get_all_port_statistics switchmgr_lib_get_all_port_statistics
#define ecnt_switchmgr_lib_set_all_port_statistics_clear switchmgr_lib_set_all_port_statistics_clear
#define ecnt_switchmgr_lib_switchmgr_lib_get_brtable switchmgr_lib_get_brtable
//#endif // #ifdef TCSUPPORT_MT7530_SWITCH_API

#endif // #ifndef _MTK_ITF_SWITCH_H_

