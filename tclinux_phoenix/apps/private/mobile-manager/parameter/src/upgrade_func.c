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

#include "Upgrade_func.h"
#include "threadpool.h"


void *UpgradeTask(worker_data_t *argv)
{
	char ug_type[16] = {0};

	mobile_printf(MOBILE_INFO,"%s, start!\n",__FUNCTION__);
	if(argv->ptr != NULL)
	{
		system(argv->ptr);		
		mobile_printf(MOBILE_INFO,"%s, cmd = %s!\n",__FUNCTION__, argv->ptr);
		free(argv->ptr);
		
		sprintf(ug_type,"%d",PHONE_NO_HEADER);
		if(tcapi_set("System_Entry","upgrade_fw", ug_type) == 0)
		{
			if(!tcapi_commit("System_Entry"))
			{
				mobile_printf(MOBILE_INFO,"\r\nWARNING:It will take some minutes writing flash.\nThe modem will reboot itself after finishing writing flash.\nDon't reboot modem within 5 minutes.\n");
			}
		}				
			
	}
	
	return NULL;
}

#if defined(TCSUPPORT_CT_JOYME)
void *ForceUpgradeTask(worker_data_t *argv)
{
	char ug_type[16] = {0};

	mobile_printf(MOBILE_INFO,"%s, start!\n", __FUNCTION__);
	if ( argv->ptr )
	{
		/* get firmware/osgi/jvm via WGET */
		mobile_printf(MOBILE_INFO, "%s, cmd = %s!\n", __FUNCTION__, argv->ptr);
		system(argv->ptr);
		free(argv->ptr);

		if ( 0 == argv->data ) /* firmware upgrade */
		{
			sprintf(ug_type, "%d", PHONE_NO_HEADER);
			tcapi_set("System_Entry","upgrade_fw", ug_type);

			if ( 0 == tcapi_commit("System_Entry") )
			{
				mobile_printf(MOBILE_INFO,
			"\r\nWARNING:It will take some minutes writing flash.\n"
			"The modem will reboot itself after finishing writing flash.\n"
			"Don't reboot modem within 5 minutes.\n");
			}
		}
		else if ( 1 == argv->data /* OSGI framework */
				|| 2 == argv->data  /* JAVA machine */
				|| 5 == argv->data /* Mobile manager */
				|| 6 == argv->data /* Gateway Libs */
				)
		{
			sprintf(ug_type, "%d", argv->data);
			tcapi_set("OSGIUpgrade_Entry", "mode", ug_type);
			tcapi_set("OSGIUpgrade_Entry", "pushview", "No");
			tcapi_set("OSGIUpgrade_Entry", "pushselected", "");
			tcapi_save();
			if ( 0 == tcapi_commit("OSGIUpgrade_Entry") )
			{
				mobile_printf(MOBILE_INFO,
			"\r\nWARNING:It will take some minutes when upgrade OSGI.\n");
			}
		}
	}
	
	return NULL;
}
#endif

json_t* Upgrade(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char *url = NULL, *temp = NULL;
	worker_data_t dataptr;
	int ret = 0, buflen = 0;
	char *mode = NULL, *method = NULL, *upgid = NULL;
	char *pvstart = NULL, *pvend = NULL;
	char version[64] = {0};
	
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__,__LINE__);

	url = ParameterSearchValue(req_para, "downurl");
#if defined(TCSUPPORT_CT_JOYME)
	mode = ParameterSearchValue(req_para, "mode");
	method = ParameterSearchValue(req_para, "method");
	upgid = ParameterSearchValue(req_para, "upgrade_ID");
#endif

#if defined(TCSUPPORT_CT_JOYME)
	if( url && mode && method )
	{
		mobile_printf(MOBILE_DEBUG,"%s, mode = %s\n",__FUNCTION__,mode);
		if ( upgid )
			tcapi_set("OSGIUpgrade_Entry", "upgrade_ID", upgid);
		/* parse the version for firmware upgrade */
		if ( 0 == strcmp(mode, "0") )
		{
			pvstart = strstr(url, "_v");
			if ( pvstart )
			{
				pvstart ++; /* point to v */
				pvend = strstr(pvstart, "_");
				if ( pvend 
					&& ((pvend - pvstart) < sizeof(version)) )
				{
					strncpy(version, pvstart, (pvend - pvstart));
					tcapi_set("OSGIUpgrade_Entry", "fwver", version);
				}
			}
		}
		
		if ( 0 == strcmp(method, "1") ) /* force upgrade */
		{
			if ( 0 == strcmp(mode, "0") /* firmware upgrade */
				|| 0 == strcmp(mode, "1") /* OSGI framework */
				|| 0 == strcmp(mode, "2") /* JAVA machine */
				|| 0 == strcmp(mode, "5") /* Mobile manager */
				|| 0 == strcmp(mode, "6") /* Gateway Libs */
				)
			{
				buflen = strlen(url) + 128;
				temp = malloc(buflen);
				bzero(temp, buflen);
				snprintf(temp, buflen - 1
					, "/userfs/bin/wget --writestate /tmp/upgst_phone"
					" -O /tmp/phone-temp %s"
					, url);
				dataptr.ptr = temp;
				dataptr.data = atoi(mode);
				ret = ThreadPoolAddWork(&gThreadPool
									, ForceUpgradeTask
									, &dataptr);
			}
			else
				ret = -3;
		}
		else if ( 0 == strcmp(method, "0") ) /* recommend upgrade */
		{
			if ( 0 == strcmp(mode, "0") /* firmware upgrade */
				|| 0 == strcmp(mode, "1") /* OSGI framework */
				|| 0 == strcmp(mode, "2") /* JAVA machine */
				|| 0 == strcmp(mode, "5") /* Mobile manager */
				|| 0 == strcmp(mode, "6") /* Gateway Libs */
				)
			{
				buflen = strlen(url) + 128;
				temp = malloc(buflen);
				if ( temp )
				{
					bzero(temp, buflen);
					snprintf(temp, buflen - 1
						, "/userfs/bin/wget --writestate /tmp/upgst_phone"
						" -O /tmp/phone-temp %s"
						, url);
					tcapi_set("OSGIUpgrade_Entry", "cmd", temp);
					free(temp);
					temp = NULL;
					tcapi_set("OSGIUpgrade_Entry", "mode", mode);
					tcapi_set("OSGIUpgrade_Entry", "pushview", "Yes");
					tcapi_set("OSGIUpgrade_Entry",
								"pushselected", "unset");
					tcapi_commit("OSGIUpgrade");
					tcapi_save();
				}
			}
			else
				ret = -3;
		}
		else
			ret = -2;
	}
	else
		ret = -1;

	/* build parameter */
	if ( ret < 0 )
	{
		if( !(reply_para = ParameterBuildJSONReplyPara(req_para, 1)) )
		{
			mobile_printf(MOBILE_INFO
				,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}

		if ( -2 == ret )
		{
			if( JSON_OK != ParameterAddNewJSONChild(reply_para
				, PARA_FAIL_REASON, "Method not support"))
			{
				mobile_printf(MOBILE_INFO
					,"%s:ParameterAddNewJSONChild Err!...\n"
					,__FUNCTION__);
				goto err;
			}
		}
		else if ( -3 == ret )
		{
			if( JSON_OK != ParameterAddNewJSONChild(reply_para
				, PARA_FAIL_REASON, "Mode not support"))
			{
				mobile_printf(MOBILE_INFO
					,"%s:ParameterAddNewJSONChild Err!...\n"
					,__FUNCTION__);
				goto err;
			}
		}
		else
		{
			if( JSON_OK != ParameterAddNewJSONChild(reply_para
				, PARA_FAIL_REASON, "Invalid Parameter Names"))
			{
				mobile_printf(MOBILE_INFO
					,"%s:ParameterAddNewJSONChild Err!...\n"
					,__FUNCTION__);
				goto err;
			}
		}
	}
	else
	{
		/* OK */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0)))
		{
			mobile_printf(MOBILE_INFO
				,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
/*
		if( JSON_OK != ParameterAddNewJSONChild(reply_para
			, "upgrade_ID", upgid))
		{
			mobile_printf(MOBILE_INFO
				,"%s:ParameterAddNewJSONChild Err!...\n"
				,__FUNCTION__);
			goto err;
		}
*/
	}
#else
	if(url != NULL)
	{
		temp = malloc(strlen(url) + 40);
		if(temp){
			dataptr.ptr=temp;
			strcpy(temp, "/userfs/bin/wget -O /tmp/phone-temp ");
			strcat(temp, url);
			ret = ThreadPoolAddWork(&gThreadPool, UpgradeTask, &dataptr);
		}	
	}
	else{
		ret = -1;
	}

	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
	}
#endif

	/* build head */
	if(!(reply_root = ParameterBuildJSONReplyHead(req_root,PARA_RESULT_SUCCESS))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n",__FUNCTION__);
		goto err;
	}

	if(ParameterBuildJSONAddPara2Head(reply_root, reply_para)){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n",__FUNCTION__);
		goto err;
	}	
			
#if PARAMETER_REPLAY_DBG
	char* value_str = NULL; 
	json_tree_to_string(reply_root,&value_str);
	mobile_printf(MOBILE_INFO, "%s: reply json pkt=%s!...\n", __FUNCTION__,value_str);
	free(value_str);	
#endif	
	if(reply_para) ParameterFreeJSON(&reply_para);
	
		return reply_root;
		
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

