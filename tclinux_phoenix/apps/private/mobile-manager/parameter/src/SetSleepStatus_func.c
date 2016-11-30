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
#include "SetSleepStatus_func.h"
#include "client.h"

#if defined(TCSUPPORT_CT_JOYME)
json_t* SetSleepStatus(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	json_t *listnodes = NULL, *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	json_t *curnode2 = NULL, *curnodeval2 = NULL;
	json_t *curnode3 = NULL, *curnodeval3 = NULL;
	json_t *curnode4 = NULL, *curnodeval4 = NULL;
	char *pAction = NULL;
	int ret = 0;
	int needSaveFlag = 0;
	char failReason[64] = {0};

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);

	pAction = ParameterSearchValue(req_para, "Action");
	if(pAction==NULL){
		ret = -1;
		goto REPLY_ERR_MSG;
	}
	mobile_printf(MOBILE_INFO,"%s:Action = %s\n", __FUNCTION__, pAction);

	/* find array node */
	listnodes = ParameterSearchFirstLabel(req_para, "SLEEPTimer");
	if ( !listnodes || !listnodes->child
		|| !listnodes->child->child )
	{
		ret = -2;
		goto REPLY_ERR_MSG;
	}	
		
	/*
	listnodes->child means:  "SLEEPTimer":[   				type->JSON_ARRAY
	listnodes->child->child means:  "SLEEPTimer":[{   		type->JSON_OBJECT
	*/
	for (cursor = listnodes->child->child;
		cursor != NULL; cursor = cursor->next)
	{
		/*
		cursor->child means:	 			"SLEEPTimer":[{ "Weekday"
		cursor->child->child  means:  		"SLEEPTimer":[{ "Weekday":"Val"
		cursor->child->next means:  		"SLEEPTimer":[{ "Weekday":"Val", "Time"
		cursor->child->next->child means:	"SLEEPTimer":[{ "Weekday":"Val", "Time":"Val"
		*/
		if ( cursor->child
			&& JSON_STRING == cursor->child->type
			&& cursor->child->child->child
			&& JSON_STRING == cursor->child->child->child->type
			&& cursor->child->next
			&& JSON_STRING == cursor->child->next->type
			&& cursor->child->next->child
			&& JSON_STRING == cursor->child->next->child->type
			&& cursor->child->next->next
			&& JSON_STRING == cursor->child->next->next->type
			&& cursor->child->next->next->child
			&& JSON_STRING == cursor->child->next->next->child->type
			&& cursor->child->next->next->next
			&& JSON_STRING == cursor->child->next->next->next->type
			&& cursor->child->next->next->next->child
			&& JSON_STRING == cursor->child->next->next->next->child->type
			)
		{
			curnode = cursor->child;
			curnodeval = cursor->child->child->child;
			curnode2 = curnode->next;
			curnodeval2 = curnode->next->child;
			curnode3 = curnode2->next;
			curnodeval3 = curnode2->next->child;
			curnode4 = curnode3->next;
			curnodeval4 = curnode3->next->child;

			if ( 0 == strcmp (curnode->text, "Weekday")
				&& curnodeval->text
				&& 0 == strcmp (curnode2->text, "Time")
				&& curnodeval2->text
				&& 0 == strcmp (curnode3->text, "Active")
				&& curnodeval3->text
				&& 0 == strcmp (curnode4->text, "Enable")
				&& curnodeval4->text )
			{

				needSaveFlag = 1;
				ret = setSleepSetting(pAction, curnodeval->text, curnodeval2->text, curnodeval3->text, curnodeval4->text);
				mobile_printf(MOBILE_INFO,"%s:Weekday = %s, Time = %s, Active = %s, Enable = %s\n", __FUNCTION__, curnodeval->text, curnodeval2->text, curnodeval3->text, curnodeval4->text);
				if(ret < 0)
					goto REPLY_ERR_MSG;
			}
		}
		else{
			ret = -3;
			goto REPLY_ERR_MSG;
		}
	}

	if(ret==0&&needSaveFlag==1){
		tcapi_save();
		Sendclientpipe();		
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
				strcpy(failReason, "Parameter SLEEPTimer error");
				break;
			case -3:
				strcpy(failReason, "Parameter SLEEPTimer detail error");
				break;
			case -4:
				strcpy(failReason, "Add fail:record is max");
				break;
			case -5:
				strcpy(failReason, "Delete fail:can not find match record");
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
	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

int setSleepSetting(char *pAction,char *pWeekday,char *pTime,char *pActive,char *pEnable){
	int i = 0, j = 0;
	int count = 0;
	char attrName[32] = {0};
	char attrValue[64] = {0};

	mobile_printf(MOBILE_INFO,"%s:start!...\n",__FUNCTION__);
		
	if(strcmp(pWeekday,"0")==0){
		tcapi_set(MOBILE_SLEEPENTRY, "Immediately", "1");
		tcapi_set(MOBILE_SLEEPENTRY, "ImmediatelyAct", pEnable);	
		return 0;
	}
	
	memset(attrValue, 0, sizeof(attrValue));
	if(tcapi_get(MOBILE_SLEEPENTRY, "Count", attrValue)==0)
		count = atoi(attrValue);

	if(strcmp(pAction,"0")==0){/* add */
		if(count>=MAX_ARRAY_NUM)/* max is 100 */
			return -4;
		count += 1;
		sprintf(attrValue, "%d", count);
		tcapi_set(MOBILE_SLEEPENTRY, "Count", attrValue);		
		sprintf(attrName, "Weekday%d", count);
		tcapi_set(MOBILE_SLEEPENTRY, attrName, pWeekday);
		sprintf(attrName, "Time%d", count);
		tcapi_set(MOBILE_SLEEPENTRY, attrName, pTime);
		sprintf(attrName, "Active%d", count);
		tcapi_set(MOBILE_SLEEPENTRY, attrName, pActive);
		sprintf(attrName, "Enable%d", count);
		tcapi_set(MOBILE_SLEEPENTRY, attrName, pEnable);
	}
	else if(strcmp(pAction,"1")==0){/* delete */
		for(i = 1; i <= count; i++){
			sprintf(attrName, "Weekday%d", i);
			tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);
			if(strcmp(attrValue,pWeekday)!=0)
				continue;
			sprintf(attrName, "Time%d", i);
			tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);
			if(strcmp(attrValue,pTime)!=0)
				continue;
			sprintf(attrName, "Active%d", i);
			tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);
			if(strcmp(attrValue,pActive)==0)
				break;
		}
		if(i<=count){/* found */
			if(count<=1)
				tcapi_unset(MOBILE_SLEEPENTRY);
			else{
				if(i<count){
					sprintf(attrName, "Weekday%d", count);
					tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);	
					sprintf(attrName, "Weekday%d", i);
					tcapi_set(MOBILE_SLEEPENTRY, attrName, attrValue);
					sprintf(attrName, "Time%d", count);
					tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);	
					sprintf(attrName, "Time%d", i);
					tcapi_set(MOBILE_SLEEPENTRY, attrName, attrValue);
					sprintf(attrName, "Active%d", count);
					tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);	
					sprintf(attrName, "Active%d", i);
					tcapi_set(MOBILE_SLEEPENTRY, attrName, attrValue);
					sprintf(attrName, "Enable%d", count);
					tcapi_get(MOBILE_SLEEPENTRY, attrName, attrValue);	
					sprintf(attrName, "Enable%d", i);
					tcapi_set(MOBILE_SLEEPENTRY, attrName, attrValue);	
				}
				sprintf(attrName, "Weekday%d", count);
				tcapi_set(MOBILE_SLEEPENTRY, attrName, "");		
				sprintf(attrName, "Time%d", count);
				tcapi_set(MOBILE_SLEEPENTRY, attrName, "");
				sprintf(attrName, "Active%d", count);
				tcapi_set(MOBILE_SLEEPENTRY, attrName, "");
				sprintf(attrName, "Enable%d", count);
				tcapi_set(MOBILE_SLEEPENTRY, attrName, "");
			}
			count -= 1;
			sprintf(attrValue, "%d", count);
			tcapi_set(MOBILE_SLEEPENTRY, "Count", attrValue);
		}
		else{
			/*return -5;*/
		}
	}

	return 0;
}

int delaySetSleepAct(int done, void *arg, void *arg1){
	char attrValue[8]={0};
	
	memset(attrValue,0,sizeof(attrValue));
	if(tcapi_get(MOBILE_SLEEPENTRY, "Immediately", attrValue)==0 && atoi(attrValue) == 1){
		tcapi_set(MOBILE_SLEEPENTRY, "Immediately", "0");
		memset(attrValue,0,sizeof(attrValue));
		if(tcapi_get(MOBILE_SLEEPENTRY, "ImmediatelyAct", attrValue)==0)
			setSleepAct(atoi(attrValue));
	}

	return 0;
}
#endif
