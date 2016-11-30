/*
** $Id: //BBN_Linux/DEV/main/tclinux_phoenix/apps/private/phone_app/parameter/inc/Get_PhyInterConStatus_func.h#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/

#ifndef _GET_PHYINTERCONSTATUS_FUNC_C_
#define _GET_PHYINTERCONSTATUS_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_LAN1STATUS	"LAN1Status"
#define PARA_LAN2STATUS	"LAN2Status"
#define PARA_LAN3STATUS	"LAN3Status"
#define PARA_LAN4STATUS	"LAN4Status"
#define PARA_WANSTATUS	"WANStatus"
#define PARA_WIFIMODULESTATUS	"WIFIModuleStatus"

json_t* GetPhyInterConStatus(json_t* req_root, json_t* req_para);
int get_LanPhyInterConStatus(char LanPortConStatus[4][8]);
void get_WanPhyInterConStatus(char WanPortConStatus[8]);
void get_WLanPhyInterConStatus(char WIFIModuleStatus[8]);


#endif

