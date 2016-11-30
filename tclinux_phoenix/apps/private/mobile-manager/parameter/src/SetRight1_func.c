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
#include "SetRight1_func.h"

#if defined(TCSUPPORT_CT_JOYME)

json_t* SetRight1(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	json_t *req_listnodes = NULL;	
	json_t *listnodes = NULL;
	json_t *parent = NULL;
	char *pPlugin_ID = NULL;
	char *pCPU = NULL;
	char *pRAM = NULL;
	char *pFLASH = NULL;
	char *pSOCKET = NULL;
	char *pPORT = NULL;
	int ret = 0;
	char failReason[64] = {0};
	char fileName[128] = {0};
	FILE *fpr=NULL;
	json_t *parar = NULL;
	FILE *fpw=NULL;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	pPlugin_ID = ParameterSearchValue(req_para, "Plugin_ID");
	pCPU = ParameterSearchValue(req_para, "CPU");
	pRAM = ParameterSearchValue(req_para, "RAM");
	pFLASH = ParameterSearchValue(req_para, "FLASH");
	pSOCKET = ParameterSearchValue(req_para, "SOCKET");
	pPORT = ParameterSearchValue(req_para, "PORT");
	if(pPlugin_ID==NULL || pCPU==NULL || pRAM==NULL || pFLASH==NULL || pSOCKET==NULL || pPORT==NULL){
		ret = -1;
		goto REPLY_ERR_MSG;
	}
	mobile_printf(MOBILE_INFO,"%s:Plugin_ID=%s,CPU=%s,RAM=%s,FLASH=%s,SOCKET=%s,PORT=%s\n", __FUNCTION__,pPlugin_ID,pCPU,pRAM,pFLASH,pSOCKET,pPORT);

	sprintf(fileName, "%s/%s_right1", MOBILE_RIGHT1PATH, pPlugin_ID);
	fpr=fopen(fileName, "r");
	if(fpr!=NULL){
		if(json_stream_parse(fpr,&parar)!=JSON_OK){
			mobile_printf(MOBILE_INFO,"%s:json_stream_parse:%s fail\n",__FUNCTION__,fileName);
			ret = -2;
			goto REPLY_ERR_MSG;			
		}
		else{
			mobile_printf(MOBILE_INFO,"%s:json_stream_parse:%s success\n",__FUNCTION__,fileName);
			
			/* find array node */
			listnodes = ParameterSearchFirstLabel(parar, "APIRightList");
			if ( !listnodes || !listnodes->child
				|| !listnodes->child->child )
			{
				ret = -2;
				goto REPLY_ERR_MSG;
			}			
			req_listnodes = ParameterSearchFirstLabel(req_para, "APIRightList");
			if ( !req_listnodes || !req_listnodes->child
				|| !req_listnodes->child->child )
			{
				ret = -3;
				goto REPLY_ERR_MSG;
			}
			if(MergeRightList(req_listnodes,listnodes,"API",req_listnodes->child)<0){
				ret = -4;
				goto REPLY_ERR_MSG;
			}
			mobile_printf(MOBILE_INFO,"%s:json_stream_parse:merge api success\n",__FUNCTION__);

			/* find array node */
			listnodes = ParameterSearchFirstLabel(parar, "TR069RightList");
			if ( !listnodes || !listnodes->child
				|| !listnodes->child->child )
			{
				ret = -2;
				goto REPLY_ERR_MSG;
			}			
			req_listnodes = ParameterSearchFirstLabel(req_para, "TR069RightList");
			if ( !req_listnodes || !req_listnodes->child
				|| !req_listnodes->child->child )
			{
				ret = -3;
				goto REPLY_ERR_MSG;
			}
			if(MergeRightList(req_listnodes,listnodes,"Name",req_listnodes->child)<0){
				ret = -5;
				goto REPLY_ERR_MSG;
			}

			mobile_printf(MOBILE_INFO,"%s:Merge file:%s success\n",__FUNCTION__,fileName);		
		}
	}
	fpw=fopen(fileName, "w");
	if(fpw!=NULL && json_stream_output(fpw,req_para)==JSON_OK){
		mobile_printf(MOBILE_INFO,"%s:Create file:%s success\n",__FUNCTION__,fileName);
		goto REPLY_ERR_MSG;
	}
	else{
		mobile_printf(MOBILE_INFO,"%s:Create file:%s fail\n",__FUNCTION__,fileName);
		ret = -6;
		goto REPLY_ERR_MSG;
	}

REPLY_ERR_MSG:
	//build parameter
	if(ret < 0){/* Fail */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		switch (ret) {
			case -1:
				strcpy(failReason, "Parameter error");
				break;
			case -2:
				strcpy(failReason, "Parse file fail");
				break;
			case -3:
				strcpy(failReason, "Parse request fail");
				break;
			case -4:
				strcpy(failReason, "Set API Right fail");
				break;
			case -5:
				strcpy(failReason, "Set TR069 Right fail");
				break;
			case -6:
				strcpy(failReason, "Output file fail");
				break;
			default:
				strcpy(failReason, "Parameter error");
				break;
		}		
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, failReason)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{/* Success */
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	//build head
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
	if(reply_para)
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}
	if(fpr) fclose(fpr);
	if(fpw) fclose(fpw);
	if(parar) ParameterFreeJSON(&parar);
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	if(fpr) fclose(fpr);
	if(fpw) fclose(fpw);
	if(parar) ParameterFreeJSON(&parar);
	return NULL;
}

int MergeRightList(json_t *req_list,json_t *list,char *type,json_t *parent){
	json_t *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	json_t *curnode2 = NULL, *curnodeval2 = NULL;
	int ret=0;
	json_t *array_obj = NULL;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	if(req_list==NULL || list==NULL || type==NULL || parent==NULL || list->child == NULL){
		return -1;
	}

	/*
	listnodes->child means:  "APIRightList":[				type->JSON_ARRAY
	listnodes->child->child means:	"APIRightList":[{		type->JSON_OBJECT
	*/
	for (cursor = list->child->child;
		cursor != NULL; cursor = cursor->next)
	{
		/*
		cursor->child means:				"APIRightList":[{ "API"
		cursor->child->child  means:		"APIRightList":[{ "API":"Val"
		cursor->child->next means:			"APIRightList":[{ "API":"Val", "Right"
		cursor->child->next->child means:	"APIRightList":[{ "API":"Val", "Right":"Val"
		*/
		if ( cursor->child
			&& JSON_STRING == cursor->child->type
			&& cursor->child->child
			&& JSON_STRING == cursor->child->child->type
			&& cursor->child->next
			&& JSON_STRING == cursor->child->next->type
			&& cursor->child->next->child
			&& JSON_STRING == cursor->child->next->child->type)
		{
			curnode = cursor->child;
			curnodeval = cursor->child->child;
			curnode2 = curnode->next;
			curnodeval2 = curnode->next->child;

			if ( 0 == strcmp (curnode->text, type)
				&& curnodeval->text
				&& 0 == strcmp (curnode2->text, "Right")
				&& curnodeval2->text)
			{
				mobile_printf(MOBILE_INFO,"%s:%s = %s, Right = %s\n", __FUNCTION__, type, curnodeval->text, curnodeval2->text);
				if(SearchRightName(req_list,type,curnodeval->text) < 0){/* not find */
					array_obj = ParameterNewJSONObject();
					if (array_obj==NULL){
						mobile_printf(MOBILE_INFO,"%s:malloc JSON array obj Err!...\n",__FUNCTION__);
						goto err;
					}
					if( JSON_OK != ParameterAddNewJSONChild(array_obj, type, curnodeval->text) )
					{
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
					if( JSON_OK != ParameterAddNewJSONChild(array_obj, "Right", curnodeval2->text) )
					{
						mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
						goto err;
					}
					if ( JSON_OK != json_insert_child(parent, array_obj) )
					{
						mobile_printf(MOBILE_INFO,"%s:Add JSON array obj Err!...\n",__FUNCTION__);
						goto err;
					}
				}
			}
			else{				
				mobile_printf(MOBILE_INFO,"%s:Error[%s = %s, Right = %s]\n", __FUNCTION__, type, curnodeval->text, curnodeval2->text);
				ret = -1;
				break;
			}
		}
		else{
			ret = -1;
			break;
		}
	}

	return ret;

err:
	if(array_obj)
		ParameterFreeJSON(&array_obj);
	return -1;
}

int SearchRightName(json_t *list,char *type,char *name){
	json_t *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	json_t *curnode2 = NULL, *curnodeval2 = NULL;
	int ret=-1;

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
	
	if(list==NULL || type==NULL || name==NULL || list->child == NULL){
		return -1;
	}

	/*
	listnodes->child means:  "APIRightList":[				type->JSON_ARRAY
	listnodes->child->child means:	"APIRightList":[{		type->JSON_OBJECT
	*/
	for (cursor = list->child->child;
		cursor != NULL; cursor = cursor->next)
	{
		/*
		cursor->child means:				"APIRightList":[{ "API"
		cursor->child->child  means:		"APIRightList":[{ "API":"Val"
		cursor->child->next means:			"APIRightList":[{ "API":"Val", "Right"
		cursor->child->next->child means:	"APIRightList":[{ "API":"Val", "Right":"Val"
		*/
		if ( cursor->child
			&& JSON_STRING == cursor->child->type
			&& cursor->child->child
			&& JSON_STRING == cursor->child->child->type
			&& cursor->child->next
			&& JSON_STRING == cursor->child->next->type
			&& cursor->child->next->child
			&& JSON_STRING == cursor->child->next->child->type)
		{
			curnode = cursor->child;
			curnodeval = cursor->child->child;
			curnode2 = curnode->next;
			curnodeval2 = curnode->next->child;
			mobile_printf(MOBILE_INFO,"%s:%s = %s, Right = %s, SearchName=%s\n", __FUNCTION__, type, curnodeval->text, curnodeval2->text, name);
			if ( 0 == strcmp (curnode->text, type)
				&& curnodeval->text
				&& 0 == strcmp (curnode2->text, "Right")
				&& curnodeval2->text)
			{
				if(0 == strcmp (curnodeval->text, name)){/* find */
					ret=0;
					break;
				}
			}
		}
	}

	return ret;
}

#endif

