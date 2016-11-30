/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7505_IC_20131122/tclinux_phoenix/apps/private/TR69_64/cwmp/cwmpParameter/Sources/cwmpParameter_table.c#1 $
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
#include "if_ipdiag_func.h"

int doPingDiag(void);


json_t* RequestIPPingDiag(json_t* request)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	mobile_printf(MOBILE_DEBUG,"RequestIPPingDiag.....\n");
			
	/*Check whether WAN interface exist*/

	if(!doPingDiag()){
		//build parameter
		reply_para = ParameterBuildJSONReplyPara(request, 0);			

		//build head
		reply_root = ParameterBuildJSONReplyHead(request,PARA_RESULT_SUCCESS);
		ParameterBuildJSONAddPara2Head(reply_root, reply_para);	
	}

	
	return reply_root;
}

json_t* GetIPPingDiagResult(json_t* request)
{
	json_t* reply_root = NULL, *reply_para = NULL;
	mobile_printf(MOBILE_DEBUG,"GetIPPingDiagResult.....\n");

	//build parameter
	reply_para = ParameterBuildJSONReplyPara(request, 1);
	ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "WAN Is Not Exist"); 

	//build head
	reply_root = ParameterBuildJSONReplyHead(request,PARA_RESULT_SUCCESS);
	ParameterBuildJSONAddPara2Head(reply_root, reply_para);	
	
	return reply_root;
}

int doPingDiag(void)
{
	int ret = 0;

	//Start Ping Task Here
	return ret;
}


