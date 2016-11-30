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

#include <stdio.h>
#include <stdlib.h>
#include <linux/mii.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/types.h>
#include <linux/sockios.h>
#include "libapi_lib_switchmgr.h"

#include <modules/eth_global_def.h>

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
/* define  */
#if defined(TCSUPPORT_FWC_ENV)
#define LAN_IF	"eth"
#else
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
struct swconfig{
	int skfd;
	struct ifreq ifr;
};

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
static struct swconfig swcfg;
static u8 swcfg_init=0;
/************************************************************************
*        F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/


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
#if 0
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

#endif

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
int  getSwcfg(struct  swconfig *conf)
{
		memset(conf, 0, sizeof(struct swconfig));

	/* Open a basic socket.*/
	if ((conf->skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		perror("socket");
		exit(-1);
	}

	strncpy(conf->ifr.ifr_name, LAN_IF, IFNAMSIZ);
	if (ioctl(conf->skfd, SIOCGMIIPHY, &conf->ifr) < 0) {
		if (errno != ENODEV)
			fprintf(stderr, "SIOCGMIIPHY on '%s' failed: %s\n", LAN_IF, strerror(errno));
		close(conf->skfd);
		return -1;
	}

	return 0;
}

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
	struct  swconfig swcfg_tmp;
	struct swconfig *swcfg = &swcfg_tmp;
	mt7530_switch_api_params api_param;

	if(getSwcfg(&swcfg_tmp) < 0)
	{
		fprintf(stderr, "RAETH_GSW_CTLAPI on %s failed: %s\n", (swcfg->ifr).ifr_name, strerror(errno));	
		return -1;
	}

	(swcfg->ifr).ifr_data = (void *)(&api_param);
	api_param.cmdid = cmdid;
	api_param.PortQueueId = port;
	api_param.paramext_ptr = paramext_ptr;

	if (ioctl(swcfg->skfd, RAETH_GSW_CTLAPI, &(swcfg->ifr)) < 0)
	{
		fprintf(stderr, "RAETH_GSW_CTLAPI on %s failed: %s\n", (swcfg->ifr).ifr_name, strerror(errno));
		close(swcfg->skfd);
		swcfg_init = 0;
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
	close(swcfg->skfd);
	swcfg_init = 0;
	return (api_param.ret_value);
}


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
int switchmgr_lib_get_port_ratelimit(u8 port, u32 dir, u32 *rate)
{
	mt7530_switch_api_paramext2 paramext;
	int val=0;

	paramext.p1 = (u32)rate;
	paramext.p2 = (u32)dir;
	val = macMT7530gswAPIEntrance(CMD_GET_PORT_RATE_LIMIT, port, (void *)(&paramext));
	
	return val;
}

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
int switchmgr_lib_set_port_ratelimit(u8 port, u32 rate, u8 dir)
{
    mt7530_switch_api_paramext2 paramext;
    paramext.p1 = (u32)rate;
    paramext.p2 = (u32)dir;

    return macMT7530gswAPIEntrance(CMD_SET_PORT_RATE_LIMIT, port, (void *)(&paramext));
}

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
int switchmgr_lib_set_port_qos(u8 port, u32 enable)
{
    return macMT7530gswAPIEntrance(CMD_SET_PORT_QOS_ENABLE, port, (void *)(u32)(enable));
}

int switchmgr_lib_set_loopback(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_LOOPBACK_REMOTE, port, (void *)(u32)(enable));
}

int switchmgr_lib_get_loopback(u8 port, u8 *outptr_enable)
{
	return macMT7530gswAPIEntrance(CMD_GET_LOOPBACK_REMOTE, port, (void *)(outptr_enable));
}

int switchmgr_lib_set_port_enable(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_ADMIN, port, (void *)(u32)(!enable));
}

int switchmgr_lib_get_port_enable(u8 port, u8 *outptr_enable)
{
	int ret = 0;
	u8 disable;
	ret = macMT7530gswAPIEntrance(CMD_GET_PORT_ADMIN, port, (void *)(&disable));
	*outptr_enable = !disable;
	return ret;
}

int switchmgr_lib_set_port_speed_mode(u8 port, u8 mode)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_SPEEDMODE, port, (void *)(u32)mode);
}

int switchmgr_lib_get_port_speed_mode(u8 port, u8 *outptr_speedmode)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_SPEEDMODE, port, (void *)outptr_speedmode);
}

int switchmgr_lib_get_link_state(u8 port, u8 *outptr_linkst, u8 *outptr_speed)
{
	mt7530_switch_api_paramext2 paramext;
	int val=0;

	paramext.p1 = (u32)outptr_linkst;
	paramext.p2 = (u32)outptr_speed;

	return macMT7530gswAPIEntrance(CMD_GET_PORT_LINKSTATE, port, (void *)(&paramext));
}

int switchmgr_lib_set_port_follow_control_global(u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_FOLLOWCONTROL_GLOBAL, 0, (void *)(u32)(enable));
}

int switchmgr_lib_get_port_follow_control_global(u8 *outptr_enable)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_FOLLOWCONTROL_GLOBAL, 0, (void *)outptr_enable);
}

int switchmgr_lib_set_port_follow_control(u8 port, u8 enable)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_FOLLOWCONTROL, port, (void *)(u32)(enable));
}

int switchmgr_lib_get_port_follow_control(u8 port, u8 *outptr_enable)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_FOLLOWCONTROL, port, (void *)outptr_enable);
}

int switchmgr_lib_set_lan2lan_clear(void)
{
	return macMT7530gswAPIEntrance(CMD_SET_LAN2LAN_CLEAR, 0, NULL);
}

int switchmgr_lib_set_lan2lan(switch_lan2lan lan2lan)
{
	return macMT7530gswAPIEntrance(CMD_SET_LAN2LAN, 0, (void *)(&lan2lan));
}

int switchmgr_lib_get_lan2lan(switch_lan2lan_info *outptr_lan2lan)
{
	int ret = 0;

	ret = macMT7530gswAPIEntrance(CMD_GET_LAN2LAN, 0, (void *)(outptr_lan2lan));

	return ret;
}

int switchmgr_lib_get_all_port_statistics(u8 port, switch_api_cnt *portcnt)
{
	return macMT7530gswAPIEntrance(CMD_GET_PORT_STATISTICS, port, (void *)(portcnt));
}

int switchmgr_lib_set_all_port_statistics_clear(void)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_STATISTICS_CLEAR, 0, NULL);
}

int switchmgr_lib_set_port_isolate(u8 enable)
{
    return macMT7530gswAPIEntrance(CMD_SET_PORT_ISOLATE, 0, (void *)enable);
}

int switchmgr_lib_set_port_mac_limit(u8 port, u32 cnt)
{
	return macMT7530gswAPIEntrance(CMD_SET_PORT_MACLMT, port, (void *)cnt);
}

int switchmgr_lib_set_mac_ageout_time(u32 MaxAgeTime)
{
	return macMT7530gswAPIEntrance(CMD_SET_AGETIME, 0, (void *)(MaxAgeTime));
}

int switchmgr_lib_clr_mac_table(void)
{
    return macMT7530gswAPIEntrance(CMD_CLR_MAC_TABLE, 0, NULL);
}

int switchmgr_lib_get_brtable(switch_brtable *outptr_brtable, u8 port)
{
    return macMT7530gswAPIEntrance(CMD_GET_BR_TABLE, port, (void *)(outptr_brtable));
}

