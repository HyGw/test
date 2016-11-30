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

#ifndef _GET_PINGRESULT_FUNC_C_
#define _GET_PINGRESULT_FUNC_C_

#include "parameter_comm_func.h"

#define PARA_PINGSTATUS	"PingStatus"
#define PARA_PINGRESULT	"PingResult"

#define IPPINGDIAGNOSTICFILE "/tmp/cwmp/ippingdiagnostic_stat"

json_t* Get_PingResult(json_t* req_root, json_t* req_para);
#endif

