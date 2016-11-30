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

#ifndef _DNSREDIRECT_C_
#define _DNSREDIRECT_C_

#include "parameter_comm_func.h"

#define PARA_ERRREDIRECT	"ErrRedirect"
#define AF_INET		2	/* Internet IP Protocol 	*/
#define AF_INET6	10	/* IP version 6 	*/


void findAssignedValue(char *name, char *tempvalue);
void initRedirect_node();
json_t* DNSRedirect(json_t* req_root, json_t* req_para);


#endif
