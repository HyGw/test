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

#ifndef _QUERY_CURRENT_BUSINESS_FUNC_C_
#define _QUERY_CURRENT_BUSINESS_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_WIFISTARTTIME	"wifiStartTime"
#define PARA_WIFIENDTIME	"wifiEndTime"
#define PARA_WIFICONTROLCYCLE	"wifiControlCycle"
#define PARA_WIFIENABLE	        "wifiEnable"
#define MOBILE_ENTRY 	"mobile_Entry"
#define PARA_LEDSTATUS	"LEDStatus"
#define PARA_LEDSTARTTIME	"LEDStartTime"
#define PARA_LEDENDTIME	"LEDEndTime"
#define PARA_LEDCONTROLCYCLE	        "LEDControlCycle"
#define PARA_LEDENABLE	        "LEDEnable"
#define PARA_NFC	        "NFC"
#define PARA_HGWSLEEP	        "HGWSLEEP"

json_t* Query_Current_Business_func(json_t* req_root, json_t* req_para);

#endif
