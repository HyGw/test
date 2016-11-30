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

#include "Cwmp_RPC_func.h"
#include "libcompileoption.h"
#include <sys/socket.h>
#include <sys/un.h>

#if defined(TCSUPPORT_CT_JOYME)
#define SOCK_PHONE_RPC_PATH "/tmp/cwmp_phone_rpc_sock"

/*
init and connect to cwmp
*/
int initSock(int *sock)
{
	int sockfd=0, len=0;
	struct sockaddr_un remote;

	if ( !sock )
		return -1;

	if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
	{
		mobile_printf(MOBILE_INFO
				, "%s:create socket error\n", __FUNCTION__);
		return -1;
	}

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PHONE_RPC_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	if ( connect(sockfd, (struct sockaddr *)&remote, len) == -1 )
	{
		mobile_printf(MOBILE_INFO
				, "%s:socket connect error\n", __FUNCTION__);
		close(sockfd);
		return -1;
	}

	*sock = sockfd;
	return 0;
}

/*
ADDOBJECT process
*/
json_t* RPC_AddObject_func
(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL, *reply_err_msg = NULL;
	tcphone_rpc_msg msg;
	int sock_fd = 0;
	char errmsg[1024] = {0};
	char *object_value = NULL, *rpcmethod = NULL;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	/*
	find RPC method name
	*/
	rpcmethod = ParameterSearchFirstValue(req_para, "CmdType");
	if ( !rpcmethod )
	{
		strcpy(errmsg, "Get CmdType failed");
		goto REPLY_ERR_MSG;
	}

	/*
	find object
	*/
	object_value = ParameterSearchFirstValue(req_para, "Object");
	if ( !object_value )
	{
		strcpy(errmsg, "Not found Object attribute");
		goto REPLY_ERR_MSG;
	}	

	if ( 0 != initSock(&sock_fd) )
	{
		strcpy(errmsg, "Init socket error");
		goto REPLY_ERR_MSG;
	}

	ZERO_RPC_MSG(msg);
	snprintf(msg.rpcmethod, sizeof(msg.rpcmethod) - 1
		, "%s", rpcmethod);
	msg.opcmd = RPC_CMD_REQ;
	snprintf(msg.nodename, sizeof(msg.nodename) - 1
		, "%s", object_value);

	/*tcdbg_printf("\n==>(%s), req=[%s] \n", __FUNCTION__, msg.nodename);*/
	if ( send(sock_fd, &msg, sizeof(msg), 0) < 0 )
	{
		mobile_printf(MOBILE_INFO
				, "%s:send msg fail \n", __FUNCTION__);
		strcpy(errmsg, "Send socket msg error");
		goto REPLY_ERR_MSG;

	}

	bzero(&msg, sizeof(msg));
	if( recv(sock_fd, &msg, sizeof(msg), 0) <= 0 )
	{
		strcpy(errmsg, "Receive socket msg error");
		goto REPLY_ERR_MSG;
	}

	if ( RPC_MSG_MAGIC != msg.magic )
	{
		snprintf(errmsg, sizeof(errmsg) - 1
			,"Invalid socket msg tag [%x]"
			, msg.magic);
		goto REPLY_ERR_MSG;
	}

	if ( RPC_CMD_REPLY_ERROR == msg.opcmd )
	{
		snprintf(errmsg, sizeof(errmsg) - 1,
				"%s", msg.nodename);
		goto REPLY_ERR_MSG;
	}

	/* set return parameter status as instance number */
	if(!(reply_para = ParameterBuildJSONReplyParaString(req_para
		, msg.nodename)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	
	if( !(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if ( ParameterBuildJSONAddPara2Head(reply_root, reply_para) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	CLOSE_X(sock_fd);
	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	return reply_root;

REPLY_ERR_MSG:
	CLOSE_X(sock_fd);

	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	/* status:0 -->  error add object failed. */
	if(!(reply_err_msg = ParameterBuildJSONReplyPara(req_para, 0)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	
	if(JSON_OK != ParameterAddNewJSONChild(reply_err_msg
				, PARA_FAIL_REASON, errmsg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if(!(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_err_msg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_err_msg ) ParameterFreeJSON(&reply_err_msg);

	return reply_root;

err:
	CLOSE_X(sock_fd);

	if ( reply_root ) ParameterFreeJSON(&reply_root);
	if ( reply_para ) ParameterFreeJSON(&reply_para);
	return NULL;
}

/*
DELETEOBJECT process
*/
json_t* RPC_DeleteObject_func
(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL, *reply_err_msg = NULL;
	tcphone_rpc_msg msg;
	int sock_fd = 0;
	char errmsg[1024] = {0};
	char *object_value = NULL, *rpcmethod = NULL;

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	/*
	find RPC method name
	*/
	rpcmethod = ParameterSearchFirstValue(req_para, "CmdType");
	if ( !rpcmethod )
	{
		strcpy(errmsg, "Get CmdType failed");
		goto REPLY_ERR_MSG;
	}

	/*
	find object
	*/
	object_value = ParameterSearchFirstValue(req_para, "Object");
	if ( !object_value )
	{
		strcpy(errmsg, "Not found Object attribute");
		goto REPLY_ERR_MSG;
	}	

	if ( 0 != initSock(&sock_fd) )
	{
		strcpy(errmsg, "Init socket error");
		goto REPLY_ERR_MSG;
	}

	ZERO_RPC_MSG(msg);
	snprintf(msg.rpcmethod, sizeof(msg.rpcmethod) - 1
		, "%s", rpcmethod);
	msg.opcmd = RPC_CMD_REQ;
	snprintf(msg.nodename, sizeof(msg.nodename) - 1
		, "%s", object_value);

	/*tcdbg_printf("\n==>(%s), req=[%s] \n", __FUNCTION__, msg.nodename);*/
	if ( send(sock_fd, &msg, sizeof(msg), 0) < 0 )
	{
		mobile_printf(MOBILE_INFO
				, "%s:send msg fail \n", __FUNCTION__);
		strcpy(errmsg, "Send socket msg error");
		goto REPLY_ERR_MSG;

	}

	bzero(&msg, sizeof(msg));
	if( recv(sock_fd, &msg, sizeof(msg), 0) <= 0 )
	{
		strcpy(errmsg, "Receive socket msg error");
		goto REPLY_ERR_MSG;
	}

	if ( RPC_MSG_MAGIC != msg.magic )
	{
		snprintf(errmsg, sizeof(errmsg) - 1
			,"Invalid socket msg tag [%x]"
			, msg.magic);
		goto REPLY_ERR_MSG;
	}

	if ( RPC_CMD_REPLY_ERROR == msg.opcmd )
	{
		snprintf(errmsg, sizeof(errmsg) - 1,
				"%s", msg.nodename);
		goto REPLY_ERR_MSG;
	}

	/* set return parameter status */
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para
			, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	
	if( !(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if ( ParameterBuildJSONAddPara2Head(reply_root, reply_para) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	CLOSE_X(sock_fd);
	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	return reply_root;

REPLY_ERR_MSG:
	CLOSE_X(sock_fd);

	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	if(!(reply_err_msg = ParameterBuildJSONReplyPara(req_para, 1)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	
	if(JSON_OK != ParameterAddNewJSONChild(reply_err_msg
				, PARA_FAIL_REASON, errmsg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if(!(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_err_msg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_err_msg ) ParameterFreeJSON(&reply_err_msg);

	return reply_root;

err:
	CLOSE_X(sock_fd);

	if ( reply_root ) ParameterFreeJSON(&reply_root);
	if ( reply_para ) ParameterFreeJSON(&reply_para);
	return NULL;
}

/*
GETPARAMETERVALUES process
*/
json_t* RPC_GetParameterValues_func
(json_t* req_root, json_t* req_para)
{
	json_t *reply_root = NULL, *reply_para = NULL, *reply_err_msg = NULL;
	json_t *listnodes = NULL, *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	json_t *nodes_array = NULL, *array_obj;
	tcphone_rpc_msg msg;
	int sock_fd = 0;
	char errmsg[1024] = {0};
	uint32_t recv_buf_len = 0;
	char *node_value = NULL, *rpcmethod = NULL;
	char *pPlugin_ID = NULL;
	int ret = 0;
	int rightValue = 0;	

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	pPlugin_ID = ParameterSearchFirstValue(req_root, "Plugin_Name");
	if(pPlugin_ID==NULL){
		strcpy(errmsg, "Get Plugin_Name failed");
		goto REPLY_ERR_MSG;
	}

	/*
	find RPC method name
	*/
	rpcmethod = ParameterSearchFirstValue(req_para, "CmdType");
	if ( !rpcmethod )
	{
		strcpy(errmsg, "Get CmdType failed");
		goto REPLY_ERR_MSG;
	}

	/*
	find node list
	*/
	listnodes = ParameterSearchFirstLabel(req_para, "List");
	if ( !listnodes || !listnodes->child
		|| !listnodes->child->child )
	{
		strcpy(errmsg, "Not found List objects");
		goto REPLY_ERR_MSG;
	}	

	/* set return parameter status to ZERO default ok */
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}

	if ( 0 != initSock(&sock_fd) )
	{
		strcpy(errmsg, "Init socket error");
		goto REPLY_ERR_MSG;
	}

	nodes_array = ParameterNewJSONArray();
	if ( !nodes_array )
	{
		mobile_printf(MOBILE_INFO,
			"%s:malloc JSON array Err!...\n",__FUNCTION__);
		goto err;
	}

	/*
	add array to parameter list, then only need free reply_para
	*/
	if ( JSON_OK != ParameterInsertJSONPair(reply_para
		, "List"
		, nodes_array) )
	{
		mobile_printf(MOBILE_INFO,
			"%s:Add JSON array to para Err!...\n",__FUNCTION__);
		goto err;
	}

	/*
		listnodes->child meas:  "List":[   type->JSON_ARRAY
		listnodes->child->child meas:  "List":[{   type->JSON_OBJECT
	*/
	for (cursor = listnodes->child->child;
		cursor != NULL; cursor = cursor->next)
	{
		/*
			cursor->child meas:	 "List":[{ "Name",
			cursor->child->child  meas:  "List":[{ "Name":"Val"
		*/
		if ( cursor->child /* Name string */
			&& JSON_STRING == cursor->child->type
			&& cursor->child->child /* Name value string */
			&& JSON_STRING == cursor->child->child->type )
		{
			curnode = cursor->child;
			curnodeval = cursor->child->child;

			if ( 0 == strcmp (curnode->text, "Name")
				&& 0 != curnodeval->text[0] )
			{
				/* judge access */
				rightValue=GetRight1(pPlugin_ID,"TR069RightList","Name",curnodeval->text);
				if(rightValue<0){
					strcpy(errmsg, "Get Right failed");
					mobile_printf(MOBILE_INFO, "%s:Get Right failed!\n", __FUNCTION__);
					goto REPLY_ERR_MSG;
				}
				if(rightValue!=0 && rightValue!=2){
					strcpy(errmsg, "Do not have get right");
					mobile_printf(MOBILE_INFO, "%s:Plugin[%s]TR069[%s]Check Permit fail:permit=%d!\n", __FUNCTION__, pPlugin_ID, curnodeval->text, rightValue);
					goto REPLY_ERR_MSG;
				}				
		
				ZERO_RPC_MSG(msg);
				snprintf(msg.rpcmethod, sizeof(msg.rpcmethod) - 1
					, "%s", rpcmethod);
				msg.opcmd = RPC_CMD_REQ;
				snprintf(msg.nodename, sizeof(msg.nodename) - 1
					, "%s", curnodeval->text);

				/*tcdbg_printf("\n==>(%s), req=[%s] \n"
					, __FUNCTION__, msg.nodename);*/
				if ( send(sock_fd, &msg, sizeof(msg), 0) < 0 )
				{
					mobile_printf(MOBILE_INFO
							, "%s:send msg fail \n", __FUNCTION__);
					strcpy(errmsg, "Send socket msg error");
					goto REPLY_ERR_MSG;

				}

				while (1)
				{
					bzero(&msg, sizeof(msg));
					if( recv(sock_fd, &msg, sizeof(msg), 0) <= 0 )
					{
						strcpy(errmsg, "Receive socket msg error");
						goto REPLY_ERR_MSG;
					}
					if ( RPC_MSG_MAGIC != msg.magic )
					{
						snprintf(errmsg, sizeof(errmsg) - 1
							,"Invalid socket msg tag [%x]"
							, msg.magic);
						goto REPLY_ERR_MSG;
					}

					if ( RPC_CMD_REPLY_ERROR == msg.opcmd )
					{
						snprintf(errmsg, sizeof(errmsg) - 1,
								"%s", msg.nodename);
						goto REPLY_ERR_MSG;
					}

					if ( RPC_CMD_REPLY_END == msg.opcmd )
					{
						mobile_printf(MOBILE_INFO
							, "%s:recv cwmp end msg \n"
							, __FUNCTION__);
						break;
					}

					/*
					tcdbg_printf("\n==>(%s), recv name=[%s] \n"
						, __FUNCTION__, msg.nodename);*/

					recv_buf_len = msg.valuelen;

					/* receive nodevalue if need */
					if ( recv_buf_len )
					{
						if ( !node_value )
							node_value = (char *)
									calloc(recv_buf_len + 1, sizeof(char));

						if( recv(sock_fd, node_value, recv_buf_len
								, 0) <= 0 )
						{
							strcpy(errmsg, "Receive msg node value error");
							goto REPLY_ERR_MSG;
						}
					}

					array_obj = ParameterNewJSONObject();
					if ( !array_obj )
					{
						mobile_printf(MOBILE_INFO,
							"%s:malloc JSON array obj Err!...\n",__FUNCTION__);
						goto err;
					}
					/* set object name */
					if( JSON_OK != ParameterAddNewJSONChild(array_obj
						, PARA_NAME, msg.nodename) )
					{
						mobile_printf(MOBILE_INFO
							,"%s:ParameterAddNewJSONChild Err!...\n"
							,__FUNCTION__);
						goto err;
					}
					/* set object value */
					if( JSON_OK != ParameterAddNewJSONChild(array_obj
						, PARA_VALUE, node_value ? node_value : "") )
					{
						mobile_printf(MOBILE_INFO
							,"%s:ParameterAddNewJSONChild Err!...\n"
							,__FUNCTION__);
						goto err;
					}

					/*
					add to array then it will free array only no need to free every object
					*/
					if ( JSON_OK != json_insert_child(nodes_array, array_obj) )
					{
						mobile_printf(MOBILE_INFO,
							"%s:Add JSON array obj Err!...\n",__FUNCTION__);
						goto err;
					}


					FREE_X(node_value);
				}
			}/* if ( 0 == strcmp (cursor->text ..*/
		}/* if ( cursor->child && JSON_STRING ...*/
	}/* for (cursor = listnodes-> ...*/

	if( !(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if ( ParameterBuildJSONAddPara2Head(reply_root, reply_para) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	CLOSE_X(sock_fd);
	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	return reply_root;

REPLY_ERR_MSG:
	FREE_X(node_value);
	CLOSE_X(sock_fd);

	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	if(!(reply_err_msg = ParameterBuildJSONReplyPara(req_para, 1)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	
	if(JSON_OK != ParameterAddNewJSONChild(reply_err_msg
				, PARA_FAIL_REASON, errmsg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if(!(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_err_msg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_err_msg ) ParameterFreeJSON(&reply_err_msg);

	return reply_root;

err:
	FREE_X(node_value);
	CLOSE_X(sock_fd);

	if ( reply_root ) ParameterFreeJSON(&reply_root);
	if ( reply_para ) ParameterFreeJSON(&reply_para);
	return NULL;
}

/*
SETPARAMETERVALUES process
*/
json_t* RPC_SetParameterValues_func
(json_t* req_root, json_t* req_para, void *arg)
{
	json_t *reply_root = NULL, *reply_para = NULL, *reply_err_msg = NULL;
	json_t *listnodes = NULL, *cursor = NULL;
	json_t *curnode = NULL, *curnodeval = NULL;
	json_t *curValnode = NULL, *curValnodeval = NULL;
	tcphone_rpc_msg msg;
	int sock_fd = 0;
	char errmsg[1024] = {0};
	uint32_t recv_buf_len = 0;
	char /**node_value = NULL, */*rpcmethod = NULL;
	char *pPlugin_ID = NULL;
	int ret = 0;
	int rightValue = 0;	

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	pPlugin_ID = ParameterSearchFirstValue(req_root, "Plugin_Name");
	if(pPlugin_ID==NULL){
		strcpy(errmsg, "Get Plugin_Name failed");
		goto REPLY_ERR_MSG;
	}

	/*
	find RPC method name
	*/
	rpcmethod = ParameterSearchFirstValue(req_para, "CmdType");
	if ( !rpcmethod )
	{
		strcpy(errmsg, "Get CmdType failed");
		goto REPLY_ERR_MSG;
	}

	/*
	find node list
	*/
	listnodes = ParameterSearchFirstLabel(req_para, "List");
	if ( !listnodes || !listnodes->child
		|| !listnodes->child->child )
	{
		strcpy(errmsg, "Not found List objects");
		goto REPLY_ERR_MSG;
	}	

	/* set return parameter status to ZERO default ok */
	if(!(reply_para = ParameterBuildJSONReplyPara(req_para
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}

	if ( 0 != initSock(&sock_fd) )
	{
		strcpy(errmsg, "Init socket error");
		goto REPLY_ERR_MSG;
	}
	/*
		listnodes->child meas:  "List":[   type->JSON_ARRAY
		listnodes->child->child meas:  "List":[{   type->JSON_OBJECT
	*/
	for (cursor = listnodes->child->child;
		cursor != NULL; cursor = cursor->next)
	{
		/*
			cursor->child meas:	 "List":[{ "Name",
			cursor->child->child  meas:  "List":[{ "Name":"Val"
			cursor->child->next meas:  "List":[{ "Name":"Val", "Value"
			cursor->child->next->child meas:  "List":[{ "Name":"Val", "Value":"Val"
		*/
		if ( cursor->child /* Name string */
			&& JSON_STRING == cursor->child->type
			&& cursor->child->child /* Name value string */
			&& JSON_STRING == cursor->child->child->type
			&& cursor->child->next /* Value string */
			&& JSON_STRING == cursor->child->next->type
			&& cursor->child->next->child  /* Value value string */
			&& JSON_STRING == cursor->child->next->child->type)
		{
			curnode = cursor->child;
			curnodeval = cursor->child->child;
			curValnode = cursor->child->next;
			curValnodeval = cursor->child->next->child;

			if ( 0 == strcmp (curnode->text, "Name")
				&& 0 != curnodeval->text[0]
				&& 0 == strcmp (curValnode->text, "Value")
				&& curValnodeval->text )
			{
				/* judge access */
				rightValue=GetRight1(pPlugin_ID,"TR069RightList","Name",curnodeval->text);
				if(rightValue<0){
					strcpy(errmsg, "Get Right failed");
					mobile_printf(MOBILE_INFO, "%s:Get Right failed!\n", __FUNCTION__);
					goto REPLY_ERR_MSG;
				}
				if(rightValue!=1 && rightValue!=2){
					strcpy(errmsg, "Do not have set right");
					mobile_printf(MOBILE_INFO, "%s:Plugin[%s]TR069[%s]Check Permit fail:permit=%d!\n", __FUNCTION__, pPlugin_ID, curnodeval->text, rightValue);
					goto REPLY_ERR_MSG;
				}

				ZERO_RPC_MSG(msg);
				snprintf(msg.rpcmethod, sizeof(msg.rpcmethod) - 1
					, "%s", rpcmethod);
				msg.opcmd = RPC_CMD_REQ;
				snprintf(msg.nodename, sizeof(msg.nodename) - 1
					, "%s", curnodeval->text);
				recv_buf_len = strlen(curValnodeval->text);
				msg.valuelen = recv_buf_len;

				/*
				tcdbg_printf("\n==>(%s), req name=[%s] \n"
					, __FUNCTION__, msg.nodename);
				tcdbg_printf("\n==>(%s), req val=[%s] \n"
					, __FUNCTION__, curValnodeval->text);*/
				if ( send(sock_fd, &msg, sizeof(msg), 0) < 0 )
				{
					mobile_printf(MOBILE_INFO,
						"%s:send msg fail \n", __FUNCTION__);
					strcpy(errmsg, "Send socket msg error");
					goto REPLY_ERR_MSG;

				}

				if ( recv_buf_len )
				{
					if ( send(sock_fd, curValnodeval->text
						, recv_buf_len, 0) < 0 )
					{
						mobile_printf(MOBILE_INFO,
							"%s:send msg value fail \n"
							, __FUNCTION__);
						strcpy(errmsg, "Send socket msg value error");
						goto REPLY_ERR_MSG;
					}
				}

				bzero(&msg, sizeof(msg));
				if( recv(sock_fd, &msg, sizeof(msg), 0) <= 0 )
				{
					strcpy(errmsg, "Receive socket msg error");
					goto REPLY_ERR_MSG;
				}
				if ( RPC_MSG_MAGIC != msg.magic )
				{
					snprintf(errmsg, sizeof(errmsg) - 1
						,"Invalid socket msg tag [%x]"
						, msg.magic);
					goto REPLY_ERR_MSG;
				}

				if ( RPC_CMD_REPLY_ERROR == msg.opcmd )
				{
					snprintf(errmsg, sizeof(errmsg) - 1,
							"%s", msg.nodename);
					goto REPLY_ERR_MSG;
				}

				if ( RPC_CMD_REPLY_END == msg.opcmd )
				{
					mobile_printf(MOBILE_INFO,
						"%s:recv cwmp end msg \n"
						, __FUNCTION__);
					continue;
				}
			}/* if ( 0 == strcmp (cursor->text ..*/
		}/* if ( cursor->child && JSON_STRING ...*/
	}/* for (cursor = listnodes-> ...*/

	if( !(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if ( ParameterBuildJSONAddPara2Head(reply_root, reply_para) )
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	CLOSE_X(sock_fd);
	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	return reply_root;

REPLY_ERR_MSG:
	CLOSE_X(sock_fd);

	if ( reply_para )
	{
		ParameterFreeJSON(&reply_para);
		reply_para = NULL;
	}

	if(!(reply_err_msg = ParameterBuildJSONReplyPara(req_para, 1)))
	{
		mobile_printf(MOBILE_INFO,
			"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
		goto err;
	}
	
	if(JSON_OK != ParameterAddNewJSONChild(reply_err_msg
				, PARA_FAIL_REASON, errmsg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if(!(reply_root = ParameterBuildJSONReplyHead(req_root
		, PARA_RESULT_SUCCESS)))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n"
			,__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_err_msg))
	{
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n"
			,__FUNCTION__);
		goto err;
	}

	if ( reply_err_msg ) ParameterFreeJSON(&reply_err_msg);

	return reply_root;

err:
	CLOSE_X(sock_fd);

	if ( reply_root ) ParameterFreeJSON(&reply_root);
	if ( reply_para ) ParameterFreeJSON(&reply_para);
	return NULL;
}

int HandleCommit(int done, void *arg, void *arg1)
{
	tcphone_rpc_msg msg;
	int sock_fd = 0;

	if ( 0 != initSock(&sock_fd) )
	{
		mobile_printf(MOBILE_INFO,
			"%s:Init socket error \n", __FUNCTION__);
		goto COMMIT_ERR;
	}

	ZERO_RPC_MSG(msg);
	msg.opcmd = RPC_CMD_REQ;
	strcpy(msg.nodename, PARA_SETCOMMIT_NAME);

	if ( send(sock_fd, &msg, sizeof(msg), 0) < 0 )
	{
		mobile_printf(MOBILE_INFO,
			"%s:send msg fail \n", __FUNCTION__);
		goto COMMIT_ERR;
	}

	CLOSE_X(sock_fd);
	return 0;
COMMIT_ERR:
	CLOSE_X(sock_fd);
	return -1;
}
#endif

