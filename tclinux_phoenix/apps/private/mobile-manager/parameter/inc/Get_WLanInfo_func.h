/*
** $Id: //BBN_Linux/DEV/main/tclinux_phoenix/apps/private/phone_app/parameter/inc/Get_WLanInfo_func.h#1 $
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

#ifndef _GET_WLANINFO_FUNC_C_
#define _GET_WLANINFO_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_ENABLE	"Enable"
#define PARA_CHANNEL	"Channel"
#define PARA_SSID	"SSID"
#define PARA_POWERLEVEL	"PowerLevel"
#define PARA_ENCRYPT	"ENCRYPT"
#define PARA_RECV	"RECV"
#define PARA_SEND	"SEND"

json_t* Get_WLanInfo(json_t* req_root, json_t* req_para);

#endif

