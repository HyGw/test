
/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7505_IC_20131122/tclinux_phoenix/apps/private/TR69_64/cwmp/cwmpParameter/Includes/cwmpParameter_table.h#1 $
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

#ifndef _SYSINFO_FUNC_C_
#define _SYSINFO_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_VERSION "Version"
#define PARA_VERSION1 "Version1"
#define PARA_VERSION2 "Version2"

#define PARA_LOID	"LOID"
#define PARA_CARD	"CARD"
#define PARA_SN		"SN"
#define MOBILE_ENTRY 	"mobile_Entry"

json_t* GetCommProtocolVer(json_t* req_root, json_t* req_para);
json_t* Cmd_CheckPassword(json_t* req_root, json_t* req_para);

#endif

