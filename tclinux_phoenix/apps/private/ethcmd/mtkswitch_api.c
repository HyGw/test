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

#ifdef TCSUPPORT_MT7530_SWITCH_API

#define BUIL_SHARED_LIB

#include <stdio.h>
#include <stdlib.h>
#include <linux/mii.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>

#ifdef ETHCMD_DBG
//#include <sys/stat.h>
//#include <unistd.h>
#endif

#include <unistd.h>

#ifdef BUIL_SHARED_LIB
#include <net/if.h>
#include <linux/types.h>
#include <linux/sockios.h>
#else
#include "libcmd.h"
#include "libmii.h"
#include "ethercmd.h"
#include "ether.h"
#endif

#ifdef BUIL_SHARED_LIB
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;
#endif

#include <modules/eth_global_def.h>



/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/

/* define  */
#ifdef BUIL_SHARED_LIB
#define LAN_IF	"eth0"
#endif

/************************************************************************
*                            M A C R O S
*************************************************************************
*/

/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/
#ifdef BUIL_SHARED_LIB

//typedef u_int32_t u32;
//typedef u_int16_t u16;
//typedef u_int8_t u8;

struct swconfig{
	int skfd;
	struct ifreq ifr;
//	u8 swic_id;
//	u8 flag;
};

#endif

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                      E X T E R N A L   D A T A
*************************************************************************
*/

/************************************************************************
*                       P R I V A T E   D A T A
*************************************************************************
*/
#ifdef BUIL_SHARED_LIB
//struct swconfig swcfg;
static struct swconfig swcfg;
static u8 swcfg_init=0;
#endif

/************************************************************************
*        F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

/*___________________________________________________________________
**      function name:
**      descriptions:
**         ...
**
**      parameters:
**         struct ifreq *ifr: The arguments number.
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
**      1. WPLin 2012/06/01
**_______________________________________________________________
*/

#ifdef BUIL_SHARED_LIB
/*___________________________________________________________________
**      function name: init_swconfig
**      descriptions:
**         Init the switch ic and register switch ic api function.
**
**      parameters:
**         None
**
**      global:
**           swcfg
**
**      return:
**          None
**
**      call:
**           searchSwic
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int init_swconfig(void)
{
//	char buf[32];
	/*init swcfg*/
	memset(&swcfg, 0, sizeof(struct swconfig));

	/* Open a basic socket.*/
	if ((swcfg.skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		perror("socket");
		exit(-1);
	}

	strncpy(swcfg.ifr.ifr_name, LAN_IF, IFNAMSIZ);
	if (ioctl(swcfg.skfd, SIOCGMIIPHY, &swcfg.ifr) < 0) {
		if (errno != ENODEV)
			fprintf(stderr, "SIOCGMIIPHY on '%s' failed: %s\n", LAN_IF, strerror(errno));
		close(swcfg.skfd);
		return -1;
	}

	return 0;
}

/*___________________________________________________________________
**      function name: getSwcfg
**      descriptions:
**         Get the global swcfg variable.
**
**      parameters:
**        None
**
**      global:
**           swcfg
**
**      return:
**          The address of global swcfg variable.
**
**      call:
**          None
**
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
struct swconfig* getSwcfg(void)
{
	if (swcfg_init == 0)
	{
		if(init_swconfig() == 0){
            swcfg_init = 1;
        }
	}
	return &swcfg;
}
#endif

/*___________________________________________________________________
**      function name: macMT7530gswAPIEntrance
**      descriptions:
**         Common part, called by all command
**
**      parameters:
**         mt7530_switch_api_cmdid cmdid
**         void *paramext_ptr
**
**      global:
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**
**      revision:
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530gswAPIEntrance(mt7530_switch_api_cmdid cmdid, u32 port, void *paramext_ptr)
{
	struct swconfig *swcfg = getSwcfg();
	mt7530_switch_api_params api_param;

	(swcfg->ifr).ifr_data = (void *)(&api_param);
	api_param.cmdid = cmdid;
	api_param.PortQueueId = port;
	api_param.paramext_ptr = paramext_ptr;

	if (ioctl(swcfg->skfd, RAETH_GSW_CTLAPI, &(swcfg->ifr)) < 0)
	{
		fprintf(stderr, "RAETH_GSW_CTLAPI on %s failed: %s\n", (swcfg->ifr).ifr_name, strerror(errno));
		return -1;
	}

	if (api_param.ret_value == -1)
	{
#if (1)
		usleep(10000);
#endif
//		fprintf(stderr, "^%s(%04X, %d, %08X) : kernel return %d !\n", __FUNCTION__, cmdid, port, paramext_ptr, api_param.ret_value);
		//fprintf(stderr, "^MT7530 switch kernel API (%04X, %d, %08X) return -1 !\n", cmdid, port, (u32)(paramext_ptr));
	}
	return (api_param.ret_value);
}

/*___________________________________________________________________
**      function name: macMT7530GetBrglearningInd
**      descriptions:
**         get port bridge learning status (disable/enable)
**
**      corresponding register:
**         0x2P0C[4] SA_DIS
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0:disable, 1:enable
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/22
**_______________________________________________________________
*/
int macMT7530GetBrglearningInd(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_BRGLEARNINGIND, port, NULL);
}

int macMT7530SetBrglearningInd(u8 port, u8 Ind)
{
	return macMT7530gswAPIEntrance(CMD_SET_BRGLEARNINGIND, port, (void *)(u32)(Ind));
}

/*___________________________________________________________________
**      function name: macMT7530GetPortBrgInd
**      descriptions:
**         get bridging between UNI ports status (disable/enable)
**
**      corresponding register:
**         0x3P00[14] MAC_TX_EN_Pn
**         0x3P00[13] MAC_RX_EN_Pn
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0:disable, 1:enable
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/22
**_______________________________________________________________
*/
int macMT7530GetPortBrgInd(u8 port, u8 *Ind)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORTBRGIND, port, (void *)(Ind));
}

int macMT7530SetPortBrgInd(u8 port, u8 Ind)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORTBRGIND, port, (void *)(u32)(Ind));
}

/*___________________________________________________________________
**      function name: macMT7530GetDiscardUnknownMacInd
**      descriptions:
**         get discard unknown Mac address status (disable/enable)
**
**      corresponding register:
**         0x0010[31:24] BC_FFP
**         0x0010[23:16] MC_FFP
**         0x0010[15:8]  UC_FFP
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0:disable, 1:enable
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/25
**_______________________________________________________________
*/
int macMT7530GetDiscardUnknownMacInd(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_DISCARDUNKNOWNMACIND, port, NULL);
}

int macMT7530SetDiscardUnknownMacInd(u8 port, u8 Ind)
{
	return macMT7530gswAPIEntrance(CMD_SET_DISCARDUNKNOWNMACIND, port, (void *)(u32)(Ind));
}

/*___________________________________________________________________
**      function name: macMT7530GetAgeTime
**      descriptions:
**         get address table age time (in seconds)
**
**      corresponding register:
**         0x00A0[19:12] AGE_CNT, default=0x95
**         0x00A0[11:0]  AGE_UNIT, default=1
**         --> (AGE_CNT+1)*(AGE_UNIT+1) seconds
**
**      parameters:
**         u32 *MaxAgeTime
**
**      global:
**
**      return:
**         0: ok
**         -1: failure
**
**         u32 *outptr_MaxAgeTime: MaxAgeTime (in seconds)
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/25
**_______________________________________________________________
*/
int macMT7530GetAgeTime(u32 *outptr_MaxAgeTime)
{
	return macMT7530gswAPIEntrance(CMD_GET_AGETIME, 0, (void *)(outptr_MaxAgeTime));
}

int macMT7530SetAgeTime(u32 MaxAgeTime)
{
	return macMT7530gswAPIEntrance(CMD_SET_AGETIME, 0, (void *)(MaxAgeTime));
}

/*___________________________________________________________________
**      function name: macMT7530GetPortMac
**      descriptions:
**         get port mac address
**
**      corresponding register:
**         0x0080, 0x0084, 0x0088, 0x008C
**
**      parameters:
**         u8 port
**         u8 *PortMac
**
**      global:
**
**      return:
**         0: ok
**         -1: failure
**
**         u8 *PortMac
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/26
**_______________________________________________________________
*/
int macMT7530GetPortMac(u8 port, u8 *outptr_PortMac)	// "u8 portMac[6];
{
	return macMT7530gswAPIEntrance(CMD_GET_PORTMAC, port, (void *)(outptr_PortMac));
}

/*___________________________________________________________________
**      function name: macMT7530GetPortStatus
**      descriptions:
**         get port status
**
**      corresponding register:
**         0x2P00[1:0] FID0_PST
**         0x3P08[0] MAC_LNK_STS_Pn
**
**      parameters:
**         u8 port
**
**      global:
**
**      return:
**         0: Disabled
**         1: Listening
**         2: Learning
**         3: Forwarding
**         4: Blocking (same as 1 for MT7530 h/w)
**         5: Linkdown
**         6: (R)Stp_off (N/A)
**         -1: failure
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/26
**_______________________________________________________________
*/
int macMT7530GetPortStatus(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORTSTATUS, port, NULL);
}

/*___________________________________________________________________
**      function name: macMT7530GetMIBCounter
**      descriptions:
**         ...
**
**      parameters:
**         mt7530_switch_api_MibCntType MibCntType
**         u8 port
**         u32 *Out_Cnt_ptr
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
**      1. WPLin 2012/06/01
**_______________________________________________________________
*/
int macMT7530GetMIBCounter(mt7530_switch_api_MibCntType MibCntType, u8 port, u32 *outptr_Cnt)
{
	mt7530_switch_GetMibCnt_param GetMibCnt_param;
	int ret_value;

	GetMibCnt_param.MibCntType = MibCntType;
	ret_value = macMT7530gswAPIEntrance(CMD_GET_MIBCNT, port, (void *)(&GetMibCnt_param));
	*outptr_Cnt = GetMibCnt_param.Out__Cnt;
//	printf("macMT7530GetMIBCounter() read counter = %08X\n", GetMibCnt_param.Out__Cnt);
	return ret_value;

/*
	struct swconfig *swcfg = getSwcfg();
	mt7530_switch_api_params api_param;
	mt7530_switch_GetMibCnt_param GetMibCnt_param;

	(swcfg->ifr).ifr_data = (void *)(&api_param);
	api_param.cmdid = CMD_GET_MIBCNT;
	api_param.port = port;
	api_param.paramext_ptr = (void *)(&GetMibCnt_param);
//	api_param.ret_value = 0;		//dummy
	GetMibCnt_param.MibCntType = MibCntType;
//	GetMibCnt_param.Out__Cnt = 0;	//dummy

	if (ioctl(swcfg->skfd, RAETH_GSW_CTLAPI, &(swcfg->ifr)) < 0)
	{
		fprintf(stderr, "RAETH_GSW_CTLAPI on %s failed: %s\n", (swcfg->ifr).ifr_name,
			strerror(errno));
		return -1;
	}

//	printf("api_param.ret_value = %d !\n", api_param.ret_value);
//	printf("macMT7530GetMIBCounter() read return value from kernel : read CNT = %08X\n", GetMibCnt_param.Out__Cnt);

	(*outptr_Cnt) = GetMibCnt_param.Out__Cnt;

	return (api_param.ret_value);
*/
}

int macMT7530GetPortTxSuccFramesCnt(u8 port, u32 *outptr_Cnt)
{
	u32 ucast_cnt, mcast_cnt, bcast_cnt;
	int err_no_ucast, err_no_mcast, err_no_bcast;

	err_no_ucast = macMT7530GetMIBCounter(MIB_ID_TX_UCAST_CNT, port, &ucast_cnt);
	err_no_mcast = macMT7530GetMIBCounter(MIB_ID_TX_MCAST_CNT, port, &mcast_cnt);
	err_no_bcast = macMT7530GetMIBCounter(MIB_ID_TX_BCAST_CNT, port, &bcast_cnt);
	*outptr_Cnt = ucast_cnt + mcast_cnt + bcast_cnt;

	return (((err_no_ucast == -1) || (err_no_mcast == -1) || (err_no_bcast == -1)) ? -1 : 0);
}

int macMT7530GetPortRxSuccFramesCnt(u8 port, u32 *outptr_Cnt)
{
	u32 ucast_cnt, mcast_cnt, bcast_cnt;
	int err_no_ucast, err_no_mcast, err_no_bcast;

	err_no_ucast = macMT7530GetMIBCounter(MIB_ID_RX_UCAST_CNT, port, &ucast_cnt);
	err_no_mcast = macMT7530GetMIBCounter(MIB_ID_RX_MCAST_CNT, port, &mcast_cnt);
	err_no_bcast = macMT7530GetMIBCounter(MIB_ID_RX_BCAST_CNT, port, &bcast_cnt);
	*outptr_Cnt = ucast_cnt + mcast_cnt + bcast_cnt;

	return (((err_no_ucast == -1) || (err_no_mcast == -1) || (err_no_bcast == -1)) ? -1 : 0);
}

int macMT7530GetPortRxDelayFramesCnt(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;
	return (0);
}

int macMT7530GetPortTxDelayFramesCnt(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;
	return (0);
}

int macMT7530GetPortRxDropFramesCnt(u8 port, u32 *outptr_Cnt)
{
	u32 align_err_cnt, fcs_err_cnt, under_cnt, over_cnt, frag_cnt, jabb_cnt, drop_cnt;
	int err_no_align_err, err_no_fcs_err, err_no_under, err_no_over, err_no_frag, err_no_jabb, err_no_drop;

	err_no_align_err = macMT7530GetMIBCounter(MIB_ID_RX_ALIGN_ERR_CNT, port, &align_err_cnt);
	err_no_fcs_err = macMT7530GetMIBCounter(MIB_ID_RX_FCS_ERR_CNT, port, &fcs_err_cnt);
	err_no_under = macMT7530GetMIBCounter(MIB_ID_RX_UNDERSIZE_CNT, port, &under_cnt);
	err_no_over = macMT7530GetMIBCounter(MIB_ID_RX_OVERSIZE_CNT, port, &over_cnt);
	err_no_frag = macMT7530GetMIBCounter(MIB_ID_RX_FRAG_ERR_CNT, port, &frag_cnt);
	err_no_jabb = macMT7530GetMIBCounter(MIB_ID_RX_JABB_ERR_CNT, port, &jabb_cnt);
	err_no_drop = macMT7530GetMIBCounter(MIB_ID_RX_DROP_CNT, port, &drop_cnt);
	*outptr_Cnt = align_err_cnt + fcs_err_cnt + under_cnt + over_cnt + frag_cnt + jabb_cnt + drop_cnt;

	return (((err_no_align_err == -1) || (err_no_fcs_err == -1) || (err_no_under == -1) || (err_no_over == -1) || (err_no_frag == -1) || (err_no_jabb == -1) || (err_no_drop == -1)) ? -1 : 0);
}

int macMT7530GetPortTxDropFramesCnt(u8 port, u32 *outptr_Cnt)
{
	u32 lcol_cnt, xcol_cnt;
	int err_no_lcol, err_no_xcol;

	err_no_lcol = macMT7530GetMIBCounter(MIB_ID_TX_LCOL_CNT, port, &lcol_cnt);
	err_no_xcol = macMT7530GetMIBCounter(MIB_ID_TX_XCOL_CNT, port, &xcol_cnt);
	*outptr_Cnt = lcol_cnt + xcol_cnt;

	return (((err_no_lcol == -1) || (err_no_xcol == -1)) ? -1 : 0);
}

int macMT7530GetPortRxLackDropPktCnt(u8 port, u32 *outptr_Cnt)
{
	u32 drop_cnt, ctrl_drop_cnt, ing_drop_cnt;
	int err_no_drop, err_no_ctrl_drop, err_no_ing_drop;

	err_no_drop = macMT7530GetMIBCounter(MIB_ID_RX_DROP_CNT, port, &drop_cnt);
	err_no_ctrl_drop = macMT7530GetMIBCounter(MIB_ID_RX_CTRL_DROP_CNT, port, &ctrl_drop_cnt);
	err_no_ing_drop = macMT7530GetMIBCounter(MIB_ID_RX_ING_DROP_CNT, port, &ing_drop_cnt);
	*outptr_Cnt = drop_cnt + ctrl_drop_cnt + ing_drop_cnt;

	return (((err_no_drop == -1) || (err_no_ctrl_drop == -1) || (err_no_ing_drop == -1)) ? -1 : 0);
}

int macMT7530GetPortTxLackDropPktCnt(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;
	return (0);
}

int macMT7530GetPortRxOctetsCnt(u8 port, u32 *outptr_Cnt)
{
	macMT7530SetRxOctMode(OCTMODE_BAD_GOOD_BOTH);
	return macMT7530GetMIBCounter(MIB_ID_RX_OCT_CNT_L, port, outptr_Cnt);
}

int macMT7530GetPortTxOctetsCnt(u8 port, u32 *outptr_Cnt)
{
	macMT7530SetTxOctMode(OCTMODE_BAD_GOOD_BOTH);
	return macMT7530GetMIBCounter(MIB_ID_TX_OCT_CNT_L, port, outptr_Cnt);
}

int macMT7530GetPortRxPktsCnt(u8 port, u32 *outptr_Cnt)
{
	u32 ucast_cnt, mcast_cnt, bcast_cnt;
	int err_no_ucast, err_no_mcast, err_no_bcast;

	err_no_ucast = macMT7530GetMIBCounter(MIB_ID_RX_UCAST_CNT, port, &ucast_cnt);
	err_no_mcast = macMT7530GetMIBCounter(MIB_ID_RX_MCAST_CNT, port, &mcast_cnt);
	err_no_bcast = macMT7530GetMIBCounter(MIB_ID_RX_BCAST_CNT, port, &bcast_cnt);
	*outptr_Cnt = ucast_cnt + mcast_cnt + bcast_cnt;

	return (((err_no_ucast == -1) || (err_no_mcast == -1) || (err_no_bcast == -1)) ? -1 : 0);
}

int macMT7530GetPortTxPktsCnt(u8 port, u32 *outptr_Cnt)
{
	u32 ucast_cnt, mcast_cnt, bcast_cnt;
	int err_no_ucast, err_no_mcast, err_no_bcast;

	err_no_ucast = macMT7530GetMIBCounter(MIB_ID_TX_UCAST_CNT, port, &ucast_cnt);
	err_no_mcast = macMT7530GetMIBCounter(MIB_ID_TX_MCAST_CNT, port, &mcast_cnt);
	err_no_bcast = macMT7530GetMIBCounter(MIB_ID_TX_BCAST_CNT, port, &bcast_cnt);
	*outptr_Cnt = ucast_cnt + mcast_cnt + bcast_cnt;

	return (((err_no_ucast == -1) || (err_no_mcast == -1) || (err_no_bcast == -1)) ? -1 : 0);
}

int macMT7530GetPortRxBroadPktsCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_BCAST_CNT, port, outptr_Cnt);
}

int macMT7530GetPortTxBroadPktsCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_TX_BCAST_CNT, port, outptr_Cnt);
}

int macMT7530GetPortRxMultiPktsCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_MCAST_CNT, port, outptr_Cnt);
}

int macMT7530GetPortTxMultiPktsCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_TX_MCAST_CNT, port, outptr_Cnt);
}

int macMT7530GetPortRxCRCPktsCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_FCS_ERR_CNT, port, outptr_Cnt);
}

int macMT7530GetPortTxCRCPktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	return (-1);	// designer add this feature in FPGA version 2 and later (not include 1st ASIC test chip)
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_TX_CRC_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortRxUnderSizePktsCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_UNDERSIZE_CNT, port, outptr_Cnt);
}

int macMT7530GetPortTxUnderSizePktsCnt(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;	// confirmed always return 0
	return (0);
}

int macMT7530GetPortRxOverSizePktsCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_OVERSIZE_CNT, port, outptr_Cnt);
}

int macMT7530GetPortTxOverSizePktsCnt(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;	// confirmed always return 0
	return (0);
}

int macMT7530GetPortRx64PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_RX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_64_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_RX_PKT_64_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortTx64PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_TX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_64_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_TX_PKT_64_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortRx127PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_RX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_65TO127_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_RX_PKT_65TO127_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortTx127PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_TX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_65TO127_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_TX_PKT_65TO127_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortRx255PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_RX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_128TO255_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_RX_PKT_128TO255_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortTx255PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_TX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_128TO255_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_TX_PKT_128TO255_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortRx511PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_RX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_256TO511_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_RX_PKT_256TO511_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortTx511PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_TX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_256TO511_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_TX_PKT_256TO511_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortRx1023PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_RX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_512TO1023_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_RX_PKT_512TO1023_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortTx1023PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_TX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_512TO1023_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_TX_PKT_512TO1023_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortRx1518PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_RX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_1024TOMAX_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_RX_PKT_1024TOMAX_CNT, port, outptr_Cnt);
#endif
}

int macMT7530GetPortTx1518PktsCnt(u8 port, u32 *outptr_Cnt)
{
#if (MT7530_MIBCNT_VER == 1)
	macMT7530SetBxPktMode(BXMODE_TX_ONLY);
	return macMT7530GetMIBCounter(MIB_ID_BX_PKT_1024TOMAX_CNT, port, outptr_Cnt);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return macMT7530GetMIBCounter(MIB_ID_TX_PKT_1024TOMAX_CNT, port, outptr_Cnt);
#endif
}

#if (0)		// deleted
int macMT7530ResetPMCountersAndTCAs(void)
{
	return (-1);
}

int macMT7530GetPortRxOctetsFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTxOctetsFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRxFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTxFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRxBroadFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTxBroadFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRxMultiFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTxMultiFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRxCRCFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTxCRCFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRxUnderSizeFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTxUnderSizeFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRxOverSizeFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTxOverSizeFramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRx64FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTx64FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRx127FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTx127FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRx255FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTx255FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRx511FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTx511FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRx1023FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTx1023FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortRx1518FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}

int macMT7530GetPortTx1518FramesCnt(u8 port, u8 flag, u16 tci, u32 *outptr_Cnt)
{
	return (-1);
}
#endif

#if (0)		// deleted
int macMT7530GetMLDQueryProcess(u8 port, u8 *upStatus, u8 *downStatus)
{
	return (-1);
}

int macMT7530SetMLDQueryProcess(u8 port, u8 upStatus, u8 downStatus)
{
	return (-1);
}
#endif

int macMT7530SetQueuePriority(u16 priority, u8 queueId)
{
	return macMT7530gswAPIEntrance(CMD_SET_QUEUE_PRIORITY, queueId, (void *)(u32)(priority));
}

int macMT7530GetMaximumQueueSize(u8 queueId)
{
	return macMT7530gswAPIEntrance(CMD_GET_QUEUE_MAX_SIZE, queueId, NULL);
}

int macMT7530SetAllocatedQueueSize(u16 AllocatedQueueSize, u8 queueId)
{
	return macMT7530gswAPIEntrance(CMD_SET_ALLOC_QUEUE_SIZE, queueId, (void *)(u32)(AllocatedQueueSize));
}

int macMT7530GetAllocatedQueueSize(u8 queueId)
{
	return macMT7530gswAPIEntrance(CMD_GET_ALLOC_QUEUE_SIZE, queueId, NULL);
}

int macMT7530GetDiscardBlockCounter(u8 queueId)
{
	return (-1);
}

int macMT7530SetWeight(u8 weight, u8 queueId, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_WEIGHT, ((port << 16) + queueId), (void *)(u32)(weight));
}

int macMT7530GetWeight(u8 queueId, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_WEIGHT, ((port << 16) + queueId), NULL);
}

int macMT7530SetBackPressure(mt7530_switch_api_backPressure_t *backPressure_p, u8 queueId, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_BACK_PRESSURE, ((port << 16) + queueId), backPressure_p);
}

int macMT7530GetBackPressure(mt7530_switch_api_backPressure_t *backPressure_p, u8 queueId, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_BACK_PRESSURE, ((port << 16) + queueId), backPressure_p);
}

int macMT7530SetDropPolicy(u8 port, u8 queueId, mt7530_switch_DropPolicy_t *DropPolicy)
{
	return macMT7530gswAPIEntrance(CMD_SET_DROP_POLICY, ((port << 16) + queueId), (void *)(DropPolicy));
}

int macMT7530GetDropPolicy(u8 port, u8 queueId, mt7530_switch_DropPolicy_t *outptr_DropPolicy)
{
	return macMT7530gswAPIEntrance(CMD_GET_DROP_POLICY, ((port << 16) + queueId), (void *)(outptr_DropPolicy));
}

int macMT7530SetTrafficDescriptor(u8 port, mt7530_switch_api_trafficDescriptor_t *trafficDescriptor_p)
{
	return macMT7530gswAPIEntrance(CMD_SET_TRAFFIC_DESCRIPTOR, port, (void *)(trafficDescriptor_p));
}

int macMT7530GetTrafficDescriptor(u8 port, mt7530_switch_api_trafficDescriptor_t *outptr_trafficDescriptor_p)
{
	return macMT7530gswAPIEntrance(CMD_GET_TRAFFIC_DESCRIPTOR, port, (void *)(outptr_trafficDescriptor_p));
}

int macMT7530CleanTrafficDescriptor(u8 port, u8 direction)
{
	return macMT7530gswAPIEntrance(CMD_CLR_TRAFFIC_DESCRIPTOR, port, (void *)(u32)(direction));
}

int macMT7530SetAutoDetection(u8 AutoDetction, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_AUTO_DETECT, port, (void *)(u32)(AutoDetction));
}

int macMT7530GetAutoDetection(u8 *outptr_AutoDetction, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_AUTO_DETECT, port, (void *)(outptr_AutoDetction));
}

int macMT7530SetLoopbackConf(u8 enable, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_LOOPBACK_CONF, port, (void *)(u32)(enable));
}

int macMT7530GetLoopbackConf(u8 *outptr_mode, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_LOOPBACK_CONF, port, (void *)(outptr_mode));
}

int macMT7530GetConfigurationStatus(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_CONFIG_STAT, port, NULL);
}

int macMT7530SetMaxFrameSize(u16 MaxFrameSize, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_MAX_FRAME_SIZE, port, (void *)(u32)(MaxFrameSize));
}

int macMT7530GetMaxFrameSize(u16 *outptr_MaxFrameSize, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_MAX_FRAME_SIZE, port, (void *)(outptr_MaxFrameSize));
}

int macMT7530SetDTEDCEInd(u8 ind, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_DTEDCE_Ind, port, (void *)(u32)(ind));
}

int macMT7530GetDTEDCEInd(u8 *outptr_ind, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_DTEDCE_Ind, port, (void *)(outptr_ind));
}

int macMT7530SetPauseTime(u16 pauseTime, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_PAUSE_TIME, port, (void *)(u32)(pauseTime));
}

int macMT7530GetPauseTime(u16 *outptr_pauseTime, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PAUSE_TIME, port, (void *)(outptr_pauseTime));
}

int macMT7530SetPortAdmin(u8 admin, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_ADMIN, port, (void *)(u32)(admin));
}

int macMT7530GetPortAdmin(u8 *outptr_admin, u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_ADMIN, port, (void *)(outptr_admin));
}

int macMT7530GetFCSErrors(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_FCS_ERR_CNT, port, outptr_Cnt);
}

int macMT7530GetExcessiveCollisionCounter(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_TX_XCOL_CNT, port, outptr_Cnt);
}

int macMT7530GetLateCollisionCounter(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_TX_LCOL_CNT, port, outptr_Cnt);
}

int macMT7530GetFramesTooLong(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_OVERSIZE_CNT, port, outptr_Cnt);
}

int macMT7530GetBufferOverflowsRx(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_DROP_CNT, port, outptr_Cnt);
}

int macMT7530GetBufferOverflowsTx(u8 port, u32 *outptr_Cnt)
{
	// Sanders : use RX_DROP_CNT	//(*outptr_Cnt) = 0;
	// Sanders : use RX_DROP_CNT	return (0);
	return macMT7530GetMIBCounter(MIB_ID_RX_DROP_CNT, port, outptr_Cnt);
}

int macMT7530GetSingleCollisionFrameCounter(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_TX_SCOL_CNT, port, outptr_Cnt);
}

int macMT7530GetMultipleCollisionsFrameCounter(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_TX_MCOL_CNT, port, outptr_Cnt);
}

int macMT7530GetSQECounter(u8 port, u32 *outptr_Cnt)
{
	return (-1);	// confirmed not supported, obsolete
}

int macMT7530GetDeferredTxCounter(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_TX_DEFER_CNT, port, outptr_Cnt);
}

int macMT7530GetInternalMACTxErrorCounter(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;	// confirmed always return 0
	return (0);
}

int macMT7530GetCarrierSenseErrorCounter(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;	// confirmed always return 0
	return (0);
}

int macMT7530GetAlignmentErrorCounter(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_ALIGN_ERR_CNT, port, outptr_Cnt);
}

int macMT7530GetInternalMACRxErrorCounter(u8 port, u32 *outptr_Cnt)
{
	(*outptr_Cnt) = 0;	// confirmed always return 0
	return (0);
}

int macMT7530GetFragments(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_FRAG_ERR_CNT, port, outptr_Cnt);
}

int macMT7530GetJabbers(u8 port, u32 *outptr_Cnt)
{
	return macMT7530GetMIBCounter(MIB_ID_RX_JABB_ERR_CNT, port, outptr_Cnt);
}


int macMT7530GetPortRxPacketsDiscardsCnt(u8 port, u32 *outptr_Cnt)
{
	u32 align_err_cnt, fcs_err_cnt, under_cnt, over_cnt, frag_cnt, jabb_cnt, drop_cnt, filter_cnt, ctrl_drop_cnt, ing_drop_cnt, arl_drop_cnt;
	int err_no_align_err, err_no_fcs_err, err_no_under, err_no_over, err_no_frag, err_no_jabb, err_no_drop, err_no_filter, err_no_ctrl_drop, err_no_ing_drop, err_no_arl_drop;

	err_no_align_err = macMT7530GetMIBCounter(MIB_ID_RX_ALIGN_ERR_CNT, port, &align_err_cnt);
	err_no_fcs_err = macMT7530GetMIBCounter(MIB_ID_RX_FCS_ERR_CNT, port, &fcs_err_cnt);
	err_no_under = macMT7530GetMIBCounter(MIB_ID_RX_UNDERSIZE_CNT, port, &under_cnt);
	err_no_over = macMT7530GetMIBCounter(MIB_ID_RX_OVERSIZE_CNT, port, &over_cnt);
	err_no_frag = macMT7530GetMIBCounter(MIB_ID_RX_FRAG_ERR_CNT, port, &frag_cnt);
	err_no_jabb = macMT7530GetMIBCounter(MIB_ID_RX_JABB_ERR_CNT, port, &jabb_cnt);
	err_no_drop = macMT7530GetMIBCounter(MIB_ID_RX_DROP_CNT, port, &drop_cnt);
	err_no_filter = macMT7530GetMIBCounter(MIB_ID_RX_FILTER_CNT, port, &filter_cnt);
	err_no_ctrl_drop = macMT7530GetMIBCounter(MIB_ID_RX_CTRL_DROP_CNT, port, &ctrl_drop_cnt);
	err_no_ing_drop = macMT7530GetMIBCounter(MIB_ID_RX_ING_DROP_CNT, port, &ing_drop_cnt);
	err_no_arl_drop = macMT7530GetMIBCounter(MIB_ID_RX_ARL_DROP_CNT, port, &arl_drop_cnt);
	*outptr_Cnt = align_err_cnt + fcs_err_cnt + under_cnt + over_cnt + frag_cnt + jabb_cnt + drop_cnt + filter_cnt + ctrl_drop_cnt + ing_drop_cnt + arl_drop_cnt;

	if ((err_no_align_err == -1) || (err_no_fcs_err == -1) || (err_no_under == -1) || (err_no_over == -1)
	  || (err_no_frag == -1) || (err_no_jabb == -1) || (err_no_drop == -1)
	  || (err_no_filter == -1) || (err_no_ctrl_drop == -1) || (err_no_ing_drop == -1) || (err_no_arl_drop == -1))
	{
		return (-1);
	}
	else
	{
		return (0);
	}
}

int macMT7530GetPortTxPacketsDiscardsCnt(u8 port, u32 *outptr_Cnt)
// identical as int macMT7530GetPortTxDropFramesCnt(u8 port, u32 *outptr_Cnt)
{
	u32 lcol_cnt, xcol_cnt;
	int err_no_lcol, err_no_xcol;

	err_no_lcol = macMT7530GetMIBCounter(MIB_ID_TX_LCOL_CNT, port, &lcol_cnt);
	err_no_xcol = macMT7530GetMIBCounter(MIB_ID_TX_XCOL_CNT, port, &xcol_cnt);
	*outptr_Cnt = lcol_cnt + xcol_cnt;

	return (((err_no_lcol == -1) || (err_no_xcol == -1)) ? -1 : 0);
}

int macMT7530GetPortChangedCnt(u8 port, u32 *outptr_Cnt)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_CHANGEDCNT, port, (void *)(outptr_Cnt));
}

int macMT7530GetPortMacLimit(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_MACLMT, port, NULL);
}

int macMT7530SetPortMacLimit(u8 port, u32 cnt)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_MACLMT, port, (void *)cnt);
}

int macMT7530SetPortSpeedMode(u8 port, u8 mode)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_SPDMD, port, (void *)(u32)mode);
}

int macMT7530GetPortPause(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_PAUSE, port, NULL);
}

int macMT7530SetPortPause(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_PAUSE, port, (void *)(u32)(enable));
}

int macMT7530GetPortPolicingEnable(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_POLEN, port, NULL);
}

int macMT7530SetPortPolicingEnable(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_POLEN, port, (void *)(u32)(enable));
}

int macMT7530GetPortPolicing(u8 port, u32 *outptr_cir, u32 *outptr_cbs, u32 *outptr_ebs)
{
	mt7530_switch_api_paramext4 paramext;

	paramext.p1 = (u32)outptr_cir;
	paramext.p2 = (u32)outptr_cbs;
	paramext.p3 = (u32)outptr_ebs;
	return macMT7530gswAPIEntrance(CMD_GET_PORT_POL, port, (void *)(&paramext));
}

int macMT7530SetPortPolicing(u8 port, u32 cir, u32 cbs, u32 ebs)
{
	mt7530_switch_api_paramext4 paramext;

	paramext.p1 = cir;
	paramext.p2 = cbs;
	paramext.p3 = ebs;
	return macMT7530gswAPIEntrance(CMD_SET_PORT_POL, port, (void *)(&paramext));
}

int macMT7530GetPortRatelimitingEnable(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_RATELMTEN, port, NULL);
}

int macMT7530SetPortRatelimitingEnable(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_RATELMTEN, port, (void *)(u32)(enable));
}

int macMT7530GetPortRatelimiting(u8 port, u32 *outptr_cir, u32 *outptr_pir)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = (u32)outptr_cir;
	paramext.p2 = (u32)outptr_pir;
	return macMT7530gswAPIEntrance(CMD_GET_PORT_RATELMT, port, (void *)(&paramext));
}

int macMT7530SetPortRatelimiting(u8 port, u32 cir, u32 pir)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = cir;
	paramext.p2 = pir;
	return macMT7530gswAPIEntrance(CMD_SET_PORT_RATELMT, port, (void *)(&paramext));
}

int macMT7530GetPortLoopDetect(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_LOOPDET, port, NULL);
}

int macMT7530SetPortLoopDetect(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_LOOPDET, port, (void *)(u32)(enable));
}

int macMT7530SetPortDisableLooped(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_DISLOOPED, port, (void *)(u32)(enable));
}

int macMT7530GetPortActive(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_ACT, port, NULL);
}

int macMT7530SetPortActive(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_ACT, port, (void *)(u32)(enable));
}

int macMT7530GetPortAutoNegState(u8 port){
	return macMT7530gswAPIEntrance(CMD_GET_PORT_AN, port, NULL);
}

int macMT7530PortAutoNegActive(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_AN, port, (void *)(u32)(enable));
}

int macMT7530PortAutoNegRestartAutoConfig(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_AN_RESTART, port, NULL);
}

int macMT7530GetEthPortAutoNegFailure(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_AN_FAIL, port, NULL);
}

int macMT7530GetEthPortLos(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_LINK_LOSS, port, NULL);
}

int macMT7530GetEthPortFailure(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_FAIL, port, NULL);
}

int macMT7530GetEthPortCongestion(u8 port)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_CONGESTION, port, NULL);
}

int macMT7530GetTxTimestamp(mt7530_switch_api_timestamp_fifo *outptr_tx_ts)
{
	return macMT7530gswAPIEntrance(CMD_GET_TX_TIMESTAMP, 0, (void *)(outptr_tx_ts));
}

int macMT7530GetRxTimestamp(mt7530_switch_api_timestamp_fifo *outptr_rx_ts)
{
	return macMT7530gswAPIEntrance(CMD_GET_RX_TIMESTAMP, 0, (void *)(outptr_rx_ts));
}

int macMT7530GetCurrTime(mt7530_switch_api_timestamp *outptr_ts)
{
	return macMT7530gswAPIEntrance(CMD_GET_CURRTIME, 0, (void *)(outptr_ts));
}

int macMT7530SetCurrTime(int type, mt7530_switch_api_timestamp *ts)
{
	return macMT7530gswAPIEntrance(CMD_SET_CURRTIME, (u32)(type), (void *)(ts));
}

int macMT7530SetOffset(int opt, mt7530_switch_api_timestamp *ts)
{
	return macMT7530gswAPIEntrance(CMD_SET_OFFSET, (u32)(opt), (void *)(ts));
}

int macMT7530GetPTPState(int *outptr_EnablePort)
{
	return macMT7530gswAPIEntrance(CMD_GET_PTPSTATE, 0, (void *)(outptr_EnablePort));
}

int macMT7530SetPTPState(int EnablePort)
{
	return macMT7530gswAPIEntrance(CMD_SET_PTPSTATE, 0, (void *)(EnablePort));
}

int macMT7530VLan_Entry_Enable(u16 vid, u8 sw)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = (u32)vid;
	paramext.p2 = (u32)sw;
	return macMT7530gswAPIEntrance(CMD_SET_VLAN_ENTRY_ENABLE, 0, (void *)(&paramext));
}

int macMT7530VLan_Entry_SetETag(u16 vid, u8 port, u8 etag)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = (u32)vid;
	paramext.p2 = (u32)etag;
	return macMT7530gswAPIEntrance(CMD_SET_VLAN_ENTRY_EGTAG, port, (void *)(&paramext));
}

int macMT7530VLan_Entry_GetETag(u16 vid, u8 port, u8* outptr_etag)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = (u32)vid;
	paramext.p2 = (u32)(u32 *)outptr_etag;
	return macMT7530gswAPIEntrance(CMD_GET_VLAN_ENTRY_EGTAG, port, (void *)(&paramext));
}

int macMT7530VLan_Entry_SetSTag(u16 vid, u8 port, u8 stag)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = (u32)vid;
	paramext.p2 = (u32)stag;
	return macMT7530gswAPIEntrance(CMD_SET_VLAN_ENTRY_STAG, port, (void *)(&paramext));
}

int macMT7530VLan_Entry_GetSTag(u16 vid, u8 port, u8* outptr_stag)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = (u32)vid;
	paramext.p2 = (u32)(u32 *)outptr_stag;
	return macMT7530gswAPIEntrance(CMD_GET_VLAN_ENTRY_STAG, port, (void *)(&paramext));
}

int macMT7530VLan_Port_AddSTag(u8 port, u8 stag, u16 vid)
{
	mt7530_switch_api_paramext2 paramext;

	paramext.p1 = (u32)stag;
	paramext.p2 = (u32)vid;
	return macMT7530gswAPIEntrance(CMD_ADD_VLAN_PORT_STAG, port, (void *)(&paramext));
}

int macMT7530VLan_Entry_SetETagMode(u16 vid, u8 etag_mode)
{
	return macMT7530gswAPIEntrance(CMD_SET_VLAN_ENTRY_ETAGMODE, (u32)vid, (void *)(u32)(etag_mode));
}

/*___________________________________________________________________
**      function name: macMT7530EnableMIBCounter
**      descriptions:
**         ...
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
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530EnableMIBCounter(u32 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_MIBCNT_EN, 0, (void *)enable);
}

/*___________________________________________________________________
**      function name: macMT7530ClearMIBCounter
**      descriptions:
**         ...
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
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530ClearMIBCounter(void)
{
	return macMT7530gswAPIEntrance(CMD_SET_MIBCNT_CLR, 0, NULL);
}

/*___________________________________________________________________
**      function name: macMT7530SetRxOctMode
**      descriptions:
**         select which mode will count into MIB_ID_RX_OCT_CNT_H/MIB_ID_RX_OCT_CNT_L
**
**      parameters:
**         mt7530_switch_api_RxTxOctMode mode = 1 (OCTMODE_BAD_ONLY)
**                                              2 (OCTMODE_GOOD_ONLY)
**                                              3 (OCTMODE_BAD_GOOD_BOTH)
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
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530SetRxOctMode(mt7530_switch_api_RxTxOctMode mode)
{
	return macMT7530gswAPIEntrance(CMD_SET_RXOCT_MODE, 0, (void *)mode);
}

/*___________________________________________________________________
**      function name: macMT7530SetTxOctMode
**      descriptions:
**         select which mode will count into MIB_ID_TX_OCT_CNT_H/MIB_ID_TX_OCT_CNT_L
**
**      parameters:
**         mt7530_switch_api_RxTxOctMode mode = 1 (OCTMODE_BAD_ONLY)
**                                              2 (OCTMODE_GOOD_ONLY)
**                                              3 (OCTMODE_BAD_GOOD_BOTH)
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
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530SetTxOctMode(mt7530_switch_api_RxTxOctMode mode)
{
	return macMT7530gswAPIEntrance(CMD_SET_TXOCT_MODE, 0, (void *)mode);
}

/*___________________________________________________________________
**      function name: macMT7530SetBxPktMode
**      descriptions:
**         select which mode will count into BX_PKT_xxx_CNT
**
**      parameters:
**         mt7530_switch_api_BxPktMode mode = 1 (BXMODE_TX_ONLY)
**                                            2 (BXMODE_RX_ONLY)
**                                            3 (BXMODE_TX_RX_BOTH)
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
**      1. WPLin 2012/07/09
**_______________________________________________________________
*/
int macMT7530SetBxPktMode(mt7530_switch_api_BxPktMode mode)
{
#if (MT7530_MIBCNT_VER == 1)
	return macMT7530gswAPIEntrance(CMD_SET_BXPKT_MODE, 0, (void *)mode);
#endif
#if (MT7530_MIBCNT_VER == 2)
	return (-1);
#endif
}


/*___________________________________________________________________
**      function name: macMT7530APIDbgDumpEn
**      descriptions:
**         ...
**
**      parameters:
**         u8 enable
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
**      1. WPLin 2012/06/21
**_______________________________________________________________
*/
int macMT7530APIDbgDumpEn(u32 DbgDump_enable)
{
	return macMT7530gswAPIEntrance(CMD_APIDBGDUMP_EN, 0, (void *)DbgDump_enable);
}


/*___________________________________________________________________
**      function name: gswAPItest
**      descriptions:
**
**      parameters:
**
**      global:
**
**      return:
**         0:succuces
**         -1:fail
**
**      call:
**
**      revision:
**      1. WPLin 2012/06/01
**_______________________________________________________________
*/
int gswAPItest(void)
{
	u32 MIBCnt_val;

	macMT7530GetMIBCounter(MIB_ID_TX_BCAST_CNT, 0, &MIBCnt_val);
	printf("macMT7530GetMIBCounter() finished : read CNT = %08X\n", MIBCnt_val);

	return 0;
}

//YM test
/*___________________________________________________________________
**      function name: doP6Cal
**      descriptions:
**         ...
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
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530doP6Cal(void)
{
	return macMT7530gswAPIEntrance(CMD_DO_P6Cal, 0, NULL);
}


/*___________________________________________________________________
**      function name: doPortMatrix
**      descriptions:
**         ...
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
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/
int macMT7530SetPortMatrix(u8 port_group[4])
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_MATRIX, 0, (void *)port_group);
}
/*___________________________________________________________________
**      function name: rgmii traffic setting
**      descriptions:
**         ...
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
**      1. WPLin 2012/07/05
**_______________________________________________________________
*/

int macMT7530RgmiiTrafficSetting(u8 type, u8 interface)
{
    rgmii_setting_s para;
    para.wan_type = type;
    para.interface = interface;
    return macMT7530gswAPIEntrance(CMD_TRAFFIC_SET, 0 ,(void *)&para);
}

int macMT7530RgmiiSetting(u8 mode)
{
    rgmii_setting_s para;
    para.interface = RGMII;
    para.mode = mode;
    return macMT7530gswAPIEntrance(CMD_TRAFFIC_RGMII_MODE, 0 ,(void *)&para);
}

int macMT7530RgmiiDisplay()
{
    return macMT7530gswAPIEntrance(CMD_TRAFFIC_DISPLAY, 0 ,NULL);
}

//Andy test

/*___________________________________________________________________
**      function name: macMT7530SetDsTrtcmEnable
**      descriptions:
**         ...
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
**      1. Andy 2014/03/25
**_______________________________________________________________
*/
int macMT7530SetDsTrtcmEnable(u8 trtcm_enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_DS_TRTCM_ENABLE, 0, (void *)(u32)trtcm_enable);
}

int macMT7530SetDsTrtcm(mt7530_switch_api_trtcm_t *trtcm_p)
{
	return macMT7530gswAPIEntrance(CMD_SET_DS_TRTCM, 0, (void *)trtcm_p);
}

int macMT7530GetDsTrtcm(mt7530_switch_api_trtcm_acl_t *trtcm_acl_p)
{
	return macMT7530gswAPIEntrance(CMD_GET_DS_TRTCM, 0, (void *)trtcm_acl_p);
}

/*___________________________________________________________________
**      function name: macMT7530doDsP5RateLimitEnable
**      descriptions:
**         ...
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
**      1. Andy 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetDsP5RateLimitEnable(u8 ratectl_enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_DSP5_RATELIMIT_ENABLE, 0, (void *)(u32)ratectl_enable);
}

/*___________________________________________________________________
**      function name: macMT7530doDsP5RateLimit
**      descriptions:
**         ...
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
**      1. Andy 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetDsP5RateLimit(u32 rate)
{
	return macMT7530gswAPIEntrance(CMD_SET_DSP5_RATELIMIT, 0, (void *)(u32)rate);
}

/*___________________________________________________________________
**      function name: macMT7530doDsP5Qos
**      descriptions:
**         ...
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
**      1. Andy 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetDsP5Qos(u8 qos_enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_DSP5_QOS, 0, (void *)(u32)qos_enable);
}



/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlPort
**      descriptions:
**         ...
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
**      1. kenny 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetStormCtrlPort(u16 port)
{
	return macMT7530gswAPIEntrance(CMD_SET_STORM_CTRL_PORT, 0, (void *)(u32)port);
}


/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlUnit
**      descriptions:
**         ...
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
**      1. kenny 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetStormCtrlUnit(u8 unit)
{
	return macMT7530gswAPIEntrance(CMD_SET_STORM_CTRL_UNIT, 0, (void *)(u32)unit);
}


/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlType
**      descriptions:
**         ...
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
**      1. kenny 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetStormCtrlType(u8 type)
{
	return macMT7530gswAPIEntrance(CMD_SET_STORM_CTRL_TYPE, 0, (void *)(u32)type);
}


/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlMode
**      descriptions:
**         ...
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
**      1. kenny 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetStormCtrlMode(u8 mode)
{
	return macMT7530gswAPIEntrance(CMD_SET_STORM_CTRL_MODE, 0, (void *)(u32)mode);
}


/*___________________________________________________________________
**      function name: macMT7530SetStormCtrlPeriod
**      descriptions:
**         ...
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
**      1. kenny 2014/03/11
**_______________________________________________________________
*/
int macMT7530SetStormCtrlPeriod(u8 period)
{
	return macMT7530gswAPIEntrance(CMD_SET_STORM_CTRL_PERIOD, 0, (void *)(u32)period);
}

#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
int macMT7530SetMacMatchACL(mt7530_switch_dev_speed_acl_t *dev_mac_acl_p)
{
	return macMT7530gswAPIEntrance(CMD_SET_DEV_TRTCM_RULE, 0, (void *)dev_mac_acl_p);
}
#endif

int macMT7530SetPortStatus(u8 port, u8 stp_Status)
{
    return macMT7530gswAPIEntrance(CMD_SET_PORTSTATUS, port, (void *)(u32)(stp_Status));
}

int macMT7530SetMaxBitRate(int port, char* speed)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_MAX_BIT_RATE, port, (void *)speed);
}

int macMT7530SetDuplexMode(int port, char* Mode)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_DUPLEX_MODE, port, (void *)Mode);
}

int macMT7530GetMaxBitRate(int port, char* speed)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_MAX_BIT_RATE, port, (void *)speed);
}

int macMT7530GetDuplexMode(int port, char* Mode)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_DUPLEX_MODE, port, (void *)Mode);
}

/*___________________________________________________________________
**      function name: XPON_HAL_API_EXT
**      descriptions:
**         ...
**
**      parameters:
**         u8 enable
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
**      1. WPLin 2012/06/21
**_______________________________________________________________
*/
int XPON_HAL_API_EXT(u32 enable)
{
    mt7530_switch_api_paramext2 paramext;
    paramext.p1 = (u32)HAL_API_EXT;
    paramext.p2 = (u32)enable;

	printf("XPON_HAL_API_EXT() %d\n", enable);
    return macMT7530gswAPIEntrance(CMD_ETH_GENERAL_API_CTL, 0, (void *)(&paramext));

}

#endif // #ifdef TCSUPPORT_MT7530_SWITCH_API

