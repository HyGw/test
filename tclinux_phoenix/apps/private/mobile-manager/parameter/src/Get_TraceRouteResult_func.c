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
#include "Get_TraceRouteResult_func.h"
#include "TraceRouteDiagnosticsRequest_func.h"


extern traceroute_t cwmp_traceroute_p;

/*______________________________________________________________________________
** get_profile_str
**
** descriptions:
**     Get the value of specify keyword on the configuration file. The max length of a line in the profile
**     should less than 128!
**
** parameters:
**  keyname:   keyword.
**  str_return:  Put the value into det.
**  type:   NO_QMARKS: The value is not included  QuotationMarks character
**      		QMARKS:	 The value is included  QuotationMarks character
**  size:		the size of str_return
**  fd:    file descript.
**
** return:
**  Success:  length of value(may be zero!!).
**  Otherwise:  FAIL(-1).
**
**	revision:
**      1. krammer 2008/11/14
**____________________________________________________________________________
*/
int get_profile_str(char *keyname,char *str_return, int size, int type, char *path)
{
//#if 0
/*

profile fomat

example

keyworld length
<--------->
DefaultRoute="YES"\n
<--------------->
      stream length

so if we want to return YES, we should get the pointer which point to the "Y",
and catch the length of "YES" characters.

to get the pointer point to "Y": Y should locate at the (stream length) - (keyworld length)
and should also consider if there the type is qmark, we should skip one character(skipQmark)
because we don't want to get the left qmark.

to get the length of "YES", we can use (stream length) - (keyworld length).
Note 1:
	if the type is qmark, we should substract 2(qmarkLength) because we don't want to get qmark.
Note 2:
	if there is a '\n', we should substract 1(enterOffSet) because we don't want to get '\n'

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Care!!!!!!!!!!!!!!!!!!!!!!!!!!!!
the size of this example can't be less or eaual to 3!!!!!
because we will return a stream "Yes\0", so the minimum size should be 4.

*/
	FILE *fp;
	char *str_key= NULL;//,*str_enter= NULL;
	//int buffersize=100;
	char stream[MAXGET_PROFILE_SIZE]={0};
	int enterOffSet=1;
	int qmarkLength=0;
	int skipQmark=0;
	int totalLength=0;
	//int pos=0;

	fp=fopen(path,"r");
	if(fp==NULL){
		fprintf(stderr,"Can't open %s\n",path);
		return -2;
	}

	memset(str_return, 0, size);
	fseek(fp, 0, SEEK_SET);
	if(type==QMARKS){
		qmarkLength=2;
		skipQmark=1;
	}
	else if(type == NO_QMARKS){
		qmarkLength=0;
		skipQmark=0;
	}
	else{
		fprintf(stderr, "The input qmark type of get_profile_str is wrong \n");
		fclose(fp);
		return -2;
	}

	while(fgets(stream, MAXGET_PROFILE_SIZE, fp) != NULL){

		mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

		//str_enter = strrchr(stream,'\n');
		if(strrchr(stream,'\n') == NULL){
			enterOffSet=0;
		}
		else{
			enterOffSet=1;
		}

		str_key = strstr(stream,keyname);
		if(str_key == NULL || str_key != stream){
			continue;
		}

		totalLength=strlen(stream)-strlen(keyname)-enterOffSet-qmarkLength;
		if(size<totalLength+1){/*total length + '\0' should not less than buffer*/
			fprintf(stderr, "Too small buffer to catch the %s frome get_profile_str\n", keyname);
			fclose(fp);
			return -1;
		}
		else if(totalLength<0) {/*can't get a negative length string*/
			fprintf(stderr, "No profile string can get\n");
			fclose(fp);
			return -1;
		}
		else{
			strncpy(str_return, stream+strlen(keyname)+skipQmark, totalLength);
			str_return[totalLength]= '\0';
			//fprintf(stderr,"str_return:%s,str_length%d\n",str_return,strlen(str_return));
			fclose(fp);
			return strlen(str_return);
		}

		memset(stream, 0, MAXGET_PROFILE_SIZE);
	}
	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	fclose(fp);
	fprintf(stderr,"File %s content %s is worng\n",path,keyname);
	return -1;
}/* end get_profile_str */

json_t* Get_TraceRouteResult(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL;
	char TraceResult[MAX_BUF_SIZE] = {0}, TraceStatus[16]={0};
	char convertIP[30] = {0};
	int ret = -1;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	if(theTraceRouteFlag == 1)
	{
		strcpy(TraceStatus,"Request");
		strcpy(TraceResult,"");
	}
	else
	{
		fileRead(TMP_PHONEAPP_TRACE_PATH, TraceResult, MAX_BUF_SIZE);	

		strcpy(TraceStatus,"Unreach");
		if(strlen(TraceResult) > 0){
			strcpy(convertIP,(char*)inet_ntoa(cwmp_traceroute_p.target));
			if(strstr(TraceResult,convertIP)){
				strcpy(TraceStatus,"Complete");
				mobile_printf(MOBILE_INFO,"%s, ip = %s\n",__FUNCTION__,convertIP);
			}
		}

#if 0
		ret = get_profile_str("MaxHopCountExceeded=", TraceStatus, sizeof(TraceStatus),NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);

		mobile_printf(MOBILE_DEBUG,"%s, line = %d,	TraceStatus = %s\n",__FUNCTION__, __LINE__, TraceStatus);

		if((strcmp(TraceStatus, "1")==0) || (strcmp(TraceStatus, "")==0))
			strcpy(TraceStatus,"Unreach");
		else if(strcmp(TraceStatus, "0")==0)
			strcpy(TraceStatus,"Complete");
#endif		
	}

	//build parameter
	if(ret == -2){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "Get Traceroute Result Failure")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_TRACESTATUS, TraceStatus)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_TRACERESULT, TraceResult)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
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

	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

