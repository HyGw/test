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

#ifndef _GET_SYSINFO_FUNC_C_
#define _GET_SYSINFO_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_SWVERSION	"SWVersion"
#define PARA_HDVERSION	"HDVersion"
#define PARA_PRODUCTCLASS	"ProductCLass"
#define PARA_CPUCLASS	"CPUClass"
#define PARA_DEVTYPE	"DEVType"
#define PARA_DEVTYPE1	"DEVType1"
#define PARA_CAPABILITY	"Capability"
#define PARA_CARD	"Card"
#define PARA_UPLINK	"UPLink"
#define PARA_CARDNO	"Cardno"
#define PARA_IPV6	"IPV6"
#define PARA_VLANBIND	"VLANbind"

#if defined(TCSUPPORT_CT_JOYME)
#define PARA_LED	"LED"
#define PARA_FLASHSIZE	"FlashSize"
#define PARA_RAMSIZE	"RamSize"
#define PARA_DevName	"DevName"
#define PARA_WIFIMode	"WiFiMode"
#define PARA_NFC	"NFC"
#define PARA_WANIPADDR	"WanIPAddr"
#define PARA_LANIPADDR	"LanIPAddr"
#define GATEWAY_PATH "/usr/osgi/config/gateway.conf"
#endif

json_t* GetSysInfo(json_t* req_root, json_t* req_para);

#endif
