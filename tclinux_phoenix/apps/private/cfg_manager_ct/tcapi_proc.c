/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/

#include "cfg_manager.h"
#include "utility.h"
#include "libcompileoption.h"
extern mxml_node_t * save_webtype_node(mxml_node_t * top);
extern void restore_webtype_node(mxml_node_t * tmptree, mxml_node_t * top);


#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int isNeedWait2SaveFlash(mxml_node_t* tree){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char waitSave[8] = {0};
	char cpeFlag[8] = {0};
	char simAuthRet[8] = {0};
	char cardExist[8] = {0};
	
	memset(nodeName, 0, sizeof(nodeName) );
	sprintf(nodeName[0], "SimCard");
	sprintf(nodeName[1], "Entry");	
	
	if( getAttrValue(tree,nodeName, "cpeFlag",cpeFlag) == TCAPI_PROCESS_OK && strcmp(cpeFlag,"1") == 0){	//cpeFlag is card
		if( getAttrValue(tree,nodeName, "cardExist",cardExist) == TCAPI_PROCESS_OK && strcmp(cardExist,"1") == 0
			&& getAttrValue(tree,nodeName, "simAuthRet",simAuthRet) == TCAPI_PROCESS_OK && strcmp(simAuthRet,"1") == 0
			&& getAttrValue(tree,nodeName, "itmsActionStart",waitSave) == TCAPI_PROCESS_OK && strcmp(waitSave,"1") == 0){
				tcdbg_printf("[%s:%d]need wait to save flash!!!======>\n",__FUNCTION__,__LINE__);
				return 1;
		}
	}
	
	return 0;
}
#endif


int
runNodeVerify(mxml_node_t *curNode, char name[][MAX_NODE_NAME]);
/*_____________________________________________________________________________
**      function name: isVaildNode
**      descriptions:
**             Check the element of name is valid or not.
**
**      parameters:
**             name: 	Specify the cfg node name that you want to check it.
**             attr: 		Specify the attribution of name that you want to check it.
**
**      global:
**             cfg_node
**             sub_node_num
**
**      return:
**             Vaild: 		0
**             Not Vaild: 	-1
**
**      call:
**      	None
**
**      revision:
**      1. Here 2008/5/16
**____________________________________________________________________________
*/
int
isVaildNode(char name[][MAX_NODE_NAME]){
	int i=0;
	int valid = FAIL;
	int max=0;
	char end_sub_name[MAX_NODE_NAME]={0};
	char sub_node_name[MAX_NODE_NAME]={0};
	cfg_node_t* pAllCfgNode=NULL;

	/*Find out the node index*/
	for(pAllCfgNode=getAllCfgTable(); pAllCfgNode->name != NULL; pAllCfgNode++){
		if(strcmp(name[0], pAllCfgNode->name) == 0){
			valid=0;
			break;
		}
	}

 	#if DEBUG
	fprintf(stderr,"isVaildNode func: type:%x name:%s %s %s\r\n",pAllCfgNode->type,name[0],name[1],name[2]);
	#endif

	if(valid==FAIL){
		return FAIL;
	}

	for(i=0; i < MAX_ARG_NUM; i++){
		if(strlen(name[i])!=0){
			strncpy(end_sub_name, name[i], MAX_NODE_NAME);
		}
		else{
			break;
		}
	}

	/*
	    15   14  13 12  11  10   9   8   7    6   5    4   3   2    1   0       (bit order)
	+----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	| ADv |                Type          |              MAX			|
	+----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/
	if(pAllCfgNode->type & SINGLE_TYPE){
		/*Reset vaild value, we need to check sub node name is validly or not.*/
		valid=FAIL;
		if(strcmp(name[1], SUB_NODE_NAME) == 0){
			valid=0;
		}
	}

	if(pAllCfgNode->type & PVC_TYPE){
		/*Reset vaild value, we need to check sub node name is validly or not.*/
		valid=FAIL;
		for(i=0; i < PVC_NUM; i++){
			sprintf(sub_node_name, SUB_PVC_NAME, i);
			if(strcmp(name[1],sub_node_name)==0){
				valid=0;
				break;
			}
		}
	}

	if(pAllCfgNode->type & ENTRY_TYPE){
		/*Enable set attribute of PVC node when type is PVC_TYPE|ENTRY_TYPE,example:Wan_PVC0 */
		if(!((pAllCfgNode->type & PVC_TYPE) && (strcmp(end_sub_name, name[1])==0))){
			/*Reset vaild value, we need to check sub node name is validly or not.*/
			valid=FAIL;
			/*Get the max value of configure node size*/
			max= pAllCfgNode->type & 0x00ff;
			for(i=0; i < max; i++ ){
				sprintf(sub_node_name, "%s%d", SUB_NODE_NAME, i);
				if(strcmp(end_sub_name, sub_node_name)==0){
					valid=0;
					break;
				}
			}
		}
	}

	/*
		If the node name is vaild, we need to check adv_flag is actived or not.
		If this node have adv_flag, it's mean we need to check the adv_table
	*/
	if( (valid  == FAIL) && (pAllCfgNode->type & ADV_FLAG)){
		for(i=0; strlen(pAllCfgNode->adv_table[i])!=0; i++){
			if(strcmp(end_sub_name, pAllCfgNode->adv_table[i]) ==0){
				valid=0;
				break;
			}
		}
	}

	return valid;
}/* end isVaildNode*/


int isSpecNode(char nodeName[][MAX_NODE_NAME],char* attr){
	if(strcmp(attr,"ConnectionError") == 0 
		&& strcmp(nodeName[0],"Wan") == 0 
		&& strstr(nodeName[1],"PVC") != NULL
		&& strstr(nodeName[2],"Entry") != NULL){
		//set Wan_PVC*_Entry* ConnectionError
		return 1;
	}
	
	return 0;
}


/*_____________________________________________________________________________
**      function name: tcapi_set_req
**      descriptions:
**            Handle the tcapi set request message from tcapi library.
**
**      parameters:
**            tree:   Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name: Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 isVaildNode
**   	 mxmlFindElement
**   	 mxmlNewElement
**   	 mxmlElementSetAttr
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
tcapi_set_req(mxml_node_t* tree, tcapi_msg_t *msg){
	mxml_node_t *curNode=tree;
	mxml_node_t *parantNode=NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];/*0:parent Node, 1: child node, 2:sub child node */
	int i=0;

	memset(nodeName, 0, sizeof(nodeName));
	splitName(msg->node, nodeName, "_");
	mergeNodeName(tree, nodeName);
	/*Check the node name is validly or not*/
	if(isVaildNode(nodeName)== FAIL){
		msg->retval = TCAPI_NONE_NODE;
		#if DEBUG
		printf("TCAPI set failure : None node information.\r\n");
		#endif
		return;
	}

	for(i=0; i < MAX_ARG_NUM; i++){
		if(strlen(nodeName[i])==0){
			break;
		}
		parantNode=curNode;
		curNode=mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			if(isSpecNode(nodeName, msg->attr)){
				msg->retval = TCAPI_NONE_NODE;
				//printf("TCAPI set failure : None node information.msg->node=%s\r\n",msg->node);
				return;
			}
			
			/*There is no cfg node information, so we add a sub node into cfg node tree*/
			curNode = mxmlNewElement(parantNode, nodeName[i]);
		}
	}

	if(curNode==NULL){
		msg->retval = TCAPI_NONE_NODE;
	}
	else if(runNodeVerify(curNode,nodeName) != SUCCESS){
		fprintf(stderr,"node verify fail\n");
		msg->retval = TCAPI_VERIFY_FAIL;
	}
	else{
		mxmlElementSetAttr(curNode, msg->attr, msg->value);
	}

	#ifdef DEBUG
	if(msg->retval == TCAPI_NONE_NODE){
		printf("TCAPI set failure : None node information.\r\n");
		return;
	}
	printf("TCAPI set attr:%s value:%s\r\n",msg->attr, msg->value);
	#endif

}/*end tcapi_set_req*/

/*_____________________________________________________________________________
**      function name: tcapi_unset_req
**      descriptions:
**            Delete a child cfg node information.
**
**      parameters:
**            tree:   Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name: Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlDelete
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
tcapi_unset_req(mxml_node_t* tree,tcapi_msg_t *msg){

	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode=tree;
	int i=0;
	cfg_node_t* pAllCfgNode=NULL;

	memset(nodeName, 0, sizeof(nodeName));
	splitName(msg->node, nodeName, "_");
	mergeNodeName(tree, nodeName);

#if 0
#ifndef PURE_BRIDGE
        /*if unset route, record route information. shnwind add*/
        if(!strcmp(nodeName[0],"Route") && strstr(nodeName[1], SUB_NODE_NAME)){
           create_route_exe_sh(tree, ROUTE_DEL);
        }

#endif
#endif
	for(i=0; i < MAX_ARG_NUM; i++){
		if(strlen(nodeName[i])==0){
			break;
		}
		curNode= mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
	}

	if(curNode){
		if(i == 1){
			/*Parent Cfg node can't be deleted from cfg node tree. Ext: Wan, Lan ....*/
			msg->retval = TCAPI_NONE_NODE;
		}else{
			/*Add pre_unset to offer a chance stop service before deleting node*/
			for(pAllCfgNode=getAllCfgTable(); pAllCfgNode->name != NULL; pAllCfgNode++){
				/*Find out the cfg node*/
				if(strcmp(nodeName[0],  pAllCfgNode->name)==0){
					if(pAllCfgNode->cfg_pre_unset != NULL){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
						if(isSimCardExist(tree) && isSimAuthSuccess(tree)){
							processUpdateSimFileModifyFlagAction(tree,nodeName[0]);
						}
#endif

						
						/*create a node configuration file*/
						pAllCfgNode->cfg_pre_unset(tree, nodeName);
					}
				}
			}
			mxmlDelete(curNode);
#if defined(TCSUPPORT_CT_VLAN_BIND)
			if (strcmp(nodeName[0], "Wan")==0)
				tcapi_commit_internal(tree, VLAN_BIND);
#endif
			
		}
	}
	else{
		msg->retval = TCAPI_NONE_NODE;
	}

	#if DEBUG
	 if(msg->retval==TCAPI_NONE_NODE){
		printf("TCAPI unset failure : None node information.\r\n");
	 }
	 else{
	 	printf("TCAPI unset OK.\r\n");
	 }
	#endif
}/*end tcapi_unset_req*/

/*_____________________________________________________________________________
**      function name: tcapi_get_req
**      descriptions:
**            Get a cfg node attribution-value.
**
**      parameters:
**            tree:   Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name: Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             cfg_node
**
**      return:
**             None
**
**      call:
**   	 getAttrValue
**   	 mxmlElementSetAttr
**   	 mxmlNewElement mxmlDelete
**   	 mxmlDelete
**   	 isVaildNode
**   	 create_romfile
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
tcapi_get_req(mxml_node_t* tree, tcapi_msg_t* msg){
	cfg_node_t* pAllCfgNode=NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName, 0, sizeof(nodeName));
	splitName(msg->node, nodeName, "_");
	mergeNodeName(tree, nodeName);
	memset(msg->value, 0, MAXLEN_TCAPI_MSG);

	/*
	   Check the node name is used for web interface.
	   Those node information must be updated right now.
	*/
	for(pAllCfgNode=getAllCfgTable(); pAllCfgNode->name !=NULL;  pAllCfgNode++){
		if((pAllCfgNode->type & UPDATE_TYPE)
			&& (strcmp(nodeName[0], pAllCfgNode->name) == 0)){
			pAllCfgNode->cfg_read(tree, nodeName, msg->attr);
			break;
		}
	}
	msg->retval=getAttrValue(tree, nodeName, msg->attr, msg->value);

	#if DEBUG
	printf("TCAPI get attr:%s\t value:%s retval:%d\r\n", msg->attr,  msg->value, msg->retval);
	#endif

}/*end tcapi_get_req*/

/*_____________________________________________________________________________
**      function name: tcapi_show_req
**      descriptions:
**            Get all attribution-values of a cfg node.
**
**      parameters:
**            tree:   Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name: Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
tcapi_show_req(mxml_node_t* top,tcapi_msg_t *msg){

	mxml_node_t *curNode=top;
	mxml_node_t *parantNode=NULL;
	mxml_node_t *childNode=NULL;
	mxml_attr_t   *attr=NULL;
	int i=0;
	char nodeName[3][MAX_NODE_NAME];
#if 0//modify by xiyang	
        char buf[160]={0};
#else
        char buf[MAX_NODE_NAME + MAXLEN_TCAPI_MSG + 1] = {0};
#endif
	char *nodeInfo=NULL;
	int fd=0;

	nodeInfo=malloc(MAX_SHOW_BUF);
	if(nodeInfo==NULL){
		msg->retval = TCAPI_NONE_NODE;
		return;
	}

	memset(nodeInfo, 0, MAX_SHOW_BUF);
	memset(nodeName, 0, sizeof(nodeName));
	splitName(msg->node, nodeName, "_");

	/*Get the configure node*/
	for(i=0;i < MAX_ARG_NUM;i++){
		parantNode=curNode;
		if(strlen(nodeName[i])==0){
			break;
		}
		curNode=mxmlFindElement(curNode, curNode, nodeName[i],
				NULL, NULL, MXML_DESCEND);
	}

	if(curNode){	
		/* show the attributes of curent node */
		attr = curNode->value.element.attrs;
		for(i = 0; i < curNode->value.element.num_attrs; i++){
			sprintf(buf,"%s=%s\n",attr->name, attr->value);
			strcat(nodeInfo, buf);
			attr ++;
		}
		
		childNode= mxmlWalkNext(curNode, curNode/*parantNode*/, MXML_DESCEND);
	#if 0
		if(childNode==NULL){
			/*final configure node*/
			attr = curNode->value.element.attrs;
			for(i = 0; i < curNode->value.element.num_attrs; i++ ){
				sprintf(buf,"%s=%s\n",attr->name, attr->value);
				strcat(nodeInfo, buf);
				attr ++;
			}
		}
		else
	#endif
		if(childNode){
			/*Get all child configure node system parameters*/
			while(childNode){
				if( (childNode->type == MXML_ELEMENT)
					&& (childNode->value.element.name)
				){
					sprintf(nodeInfo,"%sNode:%s\r\n", nodeInfo, childNode ->value.element.name);
					attr = childNode->value.element.attrs;
					for(i = 0; i < childNode->value.element.num_attrs; i++ ){
						sprintf(buf,"%s=%s\n",attr->name, attr->value);
						strcat(nodeInfo, buf);
						attr ++;
					}
				}
				childNode  = mxmlWalkNext(childNode , parantNode, MXML_DESCEND);
			}
		}
		/*
		If user query node information is over MAXLEN_TCAPI_MSG length,
		we dump those informaton into /tmp/show_info file.
		*/
		if(strlen(nodeInfo) >= MAXLEN_TCAPI_MSG){
			msg->retval=TCAPI_SHOW_FILE;
			fd = open(SHOW_FILE_PATH,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			if(fd != -1){
				write(fd,nodeInfo,strlen(nodeInfo));
				close(fd);
			}
		}
		else{
			strcpy(msg->value, nodeInfo);
		}
	}
	else{
		msg->retval = TCAPI_NONE_NODE;
	}

	free(nodeInfo);
	#ifdef DEBUG
	if(msg->retval == TCAPI_NONE_NODE){
		printf("TCAPI show failure : None node information.\r\n");
	}
	else{
		printf("TCAPI show ok.\r\n");
	}
	#endif
}/*end tcapi_show_req*/

/*_____________________________________________________________________________
**      function name: tcapi_commit_req
**      descriptions:
**            Create a node configuration and restart/start node service
**
**      parameters:
**            tree:   Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name: Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**             None
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
tcapi_commit_req(mxml_node_t* top,tcapi_msg_t *msg){
	cfg_node_t* pAllCfgNode=NULL;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t* curNode=NULL;

#if 0
	//add by brian for sending message to tr69
	cwmp_msg_t message;
	long type = 1;//tr69 must be 1
	int flag = 0;
	int mq = 2; //value change type 
	int ret;
#endif
	memset(nodeName, 0, sizeof(nodeName));
	splitName(msg->node, nodeName, "_");
	mergeNodeName(top, nodeName);

	/*Add for Wan info active notify mode*/
	/*Allowed user to commit node like this:tcapi commit WanInfo_Message_nas1_2*/
	if(!strcmp(nodeName[0], "WanInfo") && !strcmp(nodeName[1], "Message")){
		/*Copy string after WanInfo_Message_*/
		memset(nodeName[2], 0, sizeof(nodeName[2]));
		strcpy(nodeName[2], msg->node + 16);
	}
	
	curNode=mxmlFindElement(top, top, nodeName[0],
				NULL, NULL, MXML_DESCEND);

	if(curNode){
		/*Find out node index*/
		for(pAllCfgNode=getAllCfgTable(); pAllCfgNode->name != NULL; pAllCfgNode++){
			/*Find out the cfg node*/
			if(strcmp(nodeName[0],  pAllCfgNode->name)==0){
				if(pAllCfgNode->cfg_write != NULL){
#ifdef TCSUPPORT_TCAPI_ENHANCE
					pthread_mutex_lock(pAllCfgNode->nodeLock);
#endif
					/*create a node configuration file*/
					pAllCfgNode->cfg_write(top, curNode);
#ifdef TCSUPPORT_TCAPI_ENHANCE
					pthread_mutex_unlock(pAllCfgNode->nodeLock);
#endif

				}

				/*
				add to support tr069 value changed.
				*/
#if 0
				memset(&message,0,sizeof(cwmp_msg_t));
				message.cwmptype = mq;
				ret = sendmegq(type,&message,flag);

				if(ret < 0)
				{
					tcdbg_printf("\r\nsend message error!!");
				}
#endif
#if 0				
				else
				{
					tcdbg_printf("\r\nsend message ok!");
				}
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
				if(isSimCardExist(top) && isSimAuthSuccess(top)){
					processUpdateSimFileModifyFlagAction(top,nodeName[0]);
				}
#endif
				if(pAllCfgNode->cfg_execute != NULL){
#ifdef TCSUPPORT_TCAPI_ENHANCE
					pthread_mutex_lock(pAllCfgNode->nodeLock);
#endif
					/*Restart or start node service*/
					if(pAllCfgNode->cfg_execute(top, nodeName) != TCAPI_PROCESS_OK){
						msg->retval =TCAPI_UNKNOW_ERR;
					}
#ifdef TCSUPPORT_TCAPI_ENHANCE
					pthread_mutex_unlock(pAllCfgNode->nodeLock);
#endif

				}
				break;
			}
		}
	}
	else{
		msg->retval = TCAPI_NONE_NODE;
	}/*end if(node!=NULL)*/

	#if DEBUG
	if(msg->retval == TCAPI_NONE_NODE){
		printf("TCAPI commit failure : None node information.\r\n");
	}
	else{
		printf("TCAPI commit ok.\r\n");
	}
	#endif
}/*end tcapi_commit_req*/

#ifdef TCSUPPORT_EPON_OAM_CTC
#include "../xpon/lib/epon/libepon.h"

int sendEponOamCmdMsg(unsigned int msgType, unsigned char *buf, unsigned int len , unsigned int *cmdSeqOut)
{	
	key_t mqkey;
	int oflag,mqid;
	eponOamIpcCmdMsg_t ipcMsg;
	
	static unsigned int cmdSeq = 0;
	int ret;


	oflag = 0666 ;
	mqkey = ftok(EPONOAM_MQ_FLAG_PATH, EPONOAM_PROJID);
	mqid = msgget(mqkey,oflag);   
	if(mqid < 0)
	{
		printf("sendEponOamCmdMsg open message queue fail!\n");
		return -1;
	}
	
	memset(&ipcMsg,0,sizeof(eponOamIpcCmdMsg_t));
	ipcMsg.msgType = 1;//type;
	ipcMsg.msg.cmdType = msgType;
	ipcMsg.msg.cmdSeq = cmdSeq++;

	if (buf)
		memcpy((void*)&(ipcMsg.msg.cmdInfo),(void*)buf,len);
	ret = msgsnd(mqid,&ipcMsg,sizeof(eponOamCmdMsg_t),IPC_NOWAIT);
	if(ret < 0)
	{
		perror("sendEponOamCmdMsg write message fail!\n");
		return -2;
	}
	
	*cmdSeqOut = cmdSeq-1;

	return 0;
}

int sendAlarm2EponOam(OamAlarmMsg_Ptr pAlarm){
	unsigned int cmdSeq;
	return sendEponOamCmdMsg(EPONOAM_CMD_TYPE_OAM_ALARM, (unsigned char *)pAlarm, sizeof(OamAlarmMsg_t), &cmdSeq);
}

int sendOamUpdateConf(void){
	unsigned int cmdSeq;

	tcdbg_printf("\nSend OAM Update config!\n!");
	return sendEponOamCmdMsg(EPONOAM_CMD_TYPE_UPDATACFG, NULL, 0, &cmdSeq);
}
#endif


/*_____________________________________________________________________________
**      function name: tcapi_save_req
**      descriptions:
**            Create a current romfile at /var/romfile.cfg and write romfile.cfg
**            into romfile flash block.
**
**      parameters:
**            tree:   Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name: Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlDelete
**   	 create_romfile
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
tcapi_save_req(mxml_node_t* tree,tcapi_msg_t *msg){
#if defined(TCSUPPORT_BACKUPROMFILE)
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	int flag = -1;
#endif
	char bootCmd[128]={0};
#endif
	int ret;
#if defined(TCSUPPORT_CT)
	int copyRomfileForUpgrade = 0;
#endif
#if defined(TCSUPPORT_CT_PON_SC)
	int isITMSOK = 0;
	char sITMS_ST[8] = {0};
#endif
#if defined(TCSUPPORT_CT_PON_GDV20)
	int isTr069Commit = 1;
#endif
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
	char nodeNameBuf[MAX_ARG_NUM][MAX_NODE_NAME];
	char flagAttrValue[8] = {0};
	int i;
	mxml_node_t *curNode=tree;
	const char *pValue=NULL;
#endif

#ifdef CWMP
	cwmp_msg_t message;
	long type = 1;//tr69 must be 1
	int msgFlag = IPC_NOWAIT;//0;
	int mq = 2; //value change type 
	int  retFlag;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char attrValue[8] = {0};
#endif

#ifdef  TCSUPPORT_EPON_OAM_CTC
	sendOamUpdateConf();
#endif

#ifdef CWMP
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	if( isNeedWait2SaveFlash(tree)){
		return;
	}
#endif
#if defined(TCSUPPORT_CT_PON_SC)
	memset(nodeName, 0, sizeof(nodeName) );
	sprintf(nodeName[0], "WebCurSet");
	sprintf(nodeName[1], "Entry");

	if ( TCAPI_PROCESS_OK != 
		getAttrValue(tree, nodeName, "ITMS_ST", sITMS_ST) )
		strcpy(sITMS_ST, "0");
	isITMSOK = atoi(sITMS_ST);
	if ( 2 == isITMSOK )
	{
		tcdbg_printf("\n Warning: restoring defautromfile to current,"
					"can not be saving!\n");
		return;		
	}
	else
		setAttrValue(tree, nodeName, "ITMS_ST", "0");
#endif

	/*
		add to support tr069 value changed.
	*/
	memset(nodeName, 0, sizeof(nodeName) );
	sprintf(nodeName[0], "Cwmp");
	sprintf(nodeName[1], "Entry");
	
	retFlag = getAttrValue(tree, nodeName, "tr069Commit", attrValue);

	if( atoi(attrValue) == 0  ||  retFlag != TCAPI_PROCESS_OK )
	{
		memset(&message,0,sizeof(cwmp_msg_t));
		message.cwmptype = mq;
		ret = sendmegq(type,&message,msgFlag);

		if(ret < 0)
		{
			tcdbg_printf("\r\nsend message error!!");
		}
#if defined(TCSUPPORT_CT_PON_GDV20)
		isTr069Commit = 0;
#endif
	}	
	
	  setAttrValue(tree, nodeName, "tr069Commit", "0");
	
#if defined(TCSUPPORT_CT)
	retFlag = getAttrValue(tree, nodeName, "copyRomfileForUpgrade", attrValue);

	if(retFlag == TCAPI_PROCESS_OK )
	{
		copyRomfileForUpgrade = atoi(attrValue);
		//tcdbg_printf("\r\ntcapi save: copyRomfileForUpgrade is [%d]\r\n", copyRomfileForUpgrade);
	}	

  	setAttrValue(tree, nodeName, "copyRomfileForUpgrade", "0");	
#endif
	
#endif

#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
#if !defined(TCSUPPORT_RESERVEAREA_EXTEND) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	memset(nodeNameBuf, 0, sizeof(nodeNameBuf) );
	sprintf(nodeNameBuf[0], DEFVALUEFLAG_NODENAME);
	sprintf(nodeNameBuf[1], DEFVALUEFLAG_ENTRYNAME);

	for(i=0; strlen(nodeNameBuf[i])!=0; i++){
		curNode= mxmlFindElement(curNode, curNode, nodeNameBuf[i],
			NULL, NULL, MXML_DESCEND);
	}
	
	if(curNode){
		pValue=mxmlElementGetAttr(curNode, DEFVALUEFLAG_ATTRNAME);
		if(pValue){
			strcpy(flagAttrValue, pValue);
			tcdbg_printf("\n==>find attribute %s, value is %s\n", DEFVALUEFLAG_ATTRNAME, flagAttrValue);
		}
		mxmlDelete(curNode);
	}

	/*Disable auto ssid*/
	if(strcmp(flagAttrValue, DEFVALUEFLAG_ACTIVE) == 0) {
		memset(nodeNameBuf, 0, sizeof(nodeNameBuf) );
		sprintf(nodeNameBuf[0], WLAN_NODENAME);
		sprintf(nodeNameBuf[1], WLAN_ENTRYNAME);

	  	setAttrValue(tree, nodeNameBuf, WLAN_DEFCFG_ATTRNAME, WLAN_DEFCFG_DISABLE);
	}
#endif
#endif

#if defined(TCSUPPORT_CT_DSL_EX)
	update_wan_node();
	copy_from_VirServer_node();
	copy_from_route_node();
	copy_from_route6_node();
	copy_from_dmz_node();
	copy_from_igmpproxy_node();
	copy_from_mldproxy_node();
	copy_from_ddns_node();
#endif

	if(create_romfile(tree) == FAIL){
		fprintf(stderr, CREATE_ROMFILE_ERR_MSG);
		msg->retval =TCAPI_UNKNOW_ERR;
	}
	else{
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
		flag = compute_checksum(ROMFILE_PATH,STORE_ROMFILE_FILE_NAME);
		if(flag < 0)
		{
			fprintf(stderr, "tcapi_save_req:Fail to compute checksum!\n");
			msg->retval =TCAPI_UNKNOW_ERR;	
			return;
		}
#if defined(TCSUPPORT_CT)
		if(copyRomfileForUpgrade)
		{
			tcdbg_printf("\r\ntcapi save: Not save to flash, only copy to tmp\r\n");
			memset(bootCmd, 0, sizeof(bootCmd));	
			sprintf(bootCmd, "cp -f %s %s", STORE_ROMFILE_FILE_NAME, CWMP_UPGRADE_ROMFILE_FILE_NAME);
			tcdbg_printf("\r\ntcapi save: exe cmd [%s]\r\n", bootCmd);
			system(bootCmd);
			unlink(STORE_ROMFILE_FILE_NAME);
			return;		
		}

#endif
		sprintf(bootCmd,UPDATE_ROMFILE_CMD,STORE_ROMFILE_FILE_NAME);	
		system(bootCmd);
		usleep(100000);

		sprintf(bootCmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
		system(bootCmd);
		
		memset(bootCmd,0,sizeof(bootCmd));
		sprintf(bootCmd, TC_FLASH_WRITE_CMD,STORE_ROMFILE_FILE_NAME,(unsigned long)BACKUPROMFILE_RA_SIZE,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
		system(bootCmd);
		usleep(100000);
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
#if defined(TCSUPPORT_CT_PON_SC)
		/* 
		* save current romfile to defaut romfile
		* when ITMS+ provide service succeed.
		*/
		if ( 1 == isITMSOK )
		{
			/*
			* TC_FLASH_ERASE_SECTOR_CMD --> "/userfs/bin/mtd erasesector %lu %s"
			*/
			memset(bootCmd,0,sizeof(bootCmd));
			sprintf(bootCmd, TC_FLASH_ERASE_SECTOR_CMD,
							getDefaultRomfileRaSize(),
							RESERVEAREA_NAME);
			system(bootCmd);

			memset(bootCmd,0,sizeof(bootCmd));
			sprintf(bootCmd, TC_FLASH_WRITE_CMD,
							STORE_ROMFILE_FILE_NAME,
							getDefaultRomfileRaSize(),
							getDefaultRomfileRaOffset(),
							RESERVEAREA_NAME);
			system(bootCmd);
			usleep(100000);
			tcdbg_printf("\n saving settings to default romfile ok!\n");
		}
#else
                #if !defined(TCSUPPORT_RESERVEAREA_EXTEND) && !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
		if(strcmp(flagAttrValue, DEFVALUEFLAG_ACTIVE) == 0) {
			sprintf(bootCmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)DEFAULTROMFILE_RA_OFFSET, RESERVEAREA_NAME);
			system(bootCmd);
			
			memset(bootCmd,0,sizeof(bootCmd));
			sprintf(bootCmd, TC_FLASH_WRITE_CMD,STORE_ROMFILE_FILE_NAME,(unsigned long)DEFAULTROMFILE_RA_SIZE,(unsigned long)DEFAULTROMFILE_RA_OFFSET, RESERVEAREA_NAME);
			system(bootCmd);
			usleep(100000);
		}
                #endif
#endif
#endif
		unlink(STORE_ROMFILE_FILE_NAME);
#else
		sprintf(bootCmd,UPGRADE_ROMFILE_CMD,"");
		system(bootCmd);
#if defined(TCSUPPORT_BACKUPROMFILE)
		/*if user save the setting,store the new setting into backup area*/
		usleep(100000);
		sprintf(bootCmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
		system(bootCmd);
		
		memset(bootCmd,0,sizeof(bootCmd));
		sprintf(bootCmd, TC_FLASH_WRITE_CMD,ROMFILE_PATH,(unsigned long)BACKUPROMFILE_RA_SIZE,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
		system(bootCmd);
#if defined(TCSUPPORT_CT_DEFAULTPARA_STORE)
		if(strcmp(flagAttrValue, DEFVALUEFLAG_ACTIVE) == 0) {
			usleep(100000);
			sprintf(bootCmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)DEFAULTROMFILE_RA_OFFSET, RESERVEAREA_NAME);
			system(bootCmd);
		
			memset(bootCmd,0,sizeof(bootCmd));
			sprintf(bootCmd, TC_FLASH_WRITE_CMD,ROMFILE_PATH,(unsigned long)DEFAULTROMFILE_RA_SIZE,(unsigned long)DEFAULTROMFILE_RA_OFFSET, RESERVEAREA_NAME);
			system(bootCmd);
		}
#endif
#endif
#endif
	}


#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_NAMECHGNOTIFY)
	if(msg->retval != TCAPI_UNKNOW_ERR){
		checkNameChange(tree);
	}

#endif
	
#if defined(TCSUPPORT_CT_PON_GDV20)
	if(msg->retval != TCAPI_UNKNOW_ERR /*&& isTr069Commit == 0*/){
		checkInformValueChange(tree);
	}	
#endif
	
	#if DEBUG
	if(msg->retval == TCAPI_UNKNOW_ERR){
		printf("TCAPI save failure : TCAPI_UNKNOW_ERR.\r\n");
	}
	else{
		printf("TCAPI save ok.\r\n");
	}
	#endif
}/*end tcapi_save_req*/

/*_____________________________________________________________________________
**      function name: tcapi_read_req
**      descriptions:
**   				Update specify node value from flash.
**
**      parameters:
**            tree:  Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name:  Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlDelete
**   	 create_romfile
**
**      revision:
**      1. xyzhu 2009/11/23
**		2. xyzhu 2010/8/30
**____________________________________________________________________________
*/
void
tcapi_read_req(mxml_node_t* tree,tcapi_msg_t *msg){
#if 1
	char node_name[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *new_tree = NULL, *parent_node = NULL, *new_curNode = NULL;
	mxml_node_t *curNode = tree;
	mxml_node_t * tmpTreeNode = NULL;
	int i;

	memset(node_name, 0, sizeof(node_name));
	splitName(msg->node, node_name, "_");
	mergeNodeName(tree, node_name);

	/* create new tree from romfile */
	
#if defined(TCSUPPORT_BACKUPROMFILE)
	/*
		note:
		if execute function tc_parse_romfile() succeed,the function tc_parse_romfile() must return the true tree struct
	*/
	new_tree = tc_parse_romfile();
#endif
	

	new_curNode = new_tree;
	/* find this node in new tree. */
	for(i = 0; i < MAX_ARG_NUM; i++) {
		if (strlen(node_name[i]) == 0) {
			break;
		}
		new_curNode = mxmlFindElement(new_curNode, new_curNode, node_name[i],
			NULL, NULL, MXML_DESCEND);
	}

	if (new_curNode)
		tcdbg_printf("new_curNode name is %s\n", new_curNode->value.element.name);

	/* find this node in old tree */
	for (i = 0; i < MAX_ARG_NUM; i++) {
		if (strlen(node_name[i]) == 0) {
			break;
		}
		parent_node = curNode;
		curNode = mxmlFindElement(curNode, curNode, node_name[i],
			NULL, NULL, MXML_DESCEND);
	}

	/* update node in sdram with that in flash */
	mxmlDelete(curNode);
	mxmlRemove(new_curNode);
	tmpTreeNode = save_webtype_node(parent_node);
	if(tmpTreeNode == NULL)
	{
		tcdbg_printf("create_romfile: save_webtype_node Failed!\n");
	}
	mxmlAdd(parent_node, MXML_ADD_AFTER, MXML_ADD_TO_PARENT, new_curNode);

	restore_webtype_node(tmpTreeNode, parent_node);
	mxmlDelete(new_tree);
	return;	
#else
	mxml_node_t *newTree  = NULL;
	mxml_node_t *parentNode = NULL;
	mxml_node_t *childNode = NULL;
	mxml_node_t *newChildNode = NULL;
	mxml_node_t *curNode=tree;
	mxml_node_t *newCurNode;
	mxml_node_t *l2Node = NULL;
	mxml_node_t *l3Node = NULL;
	mxml_attr_t *attr=NULL;
	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i,j,ii;

/*if cfg node name is romfile,call tcapi_readAll_req() to update the whole tree from flash*/
	if (!strcmp(msg->node, "ROMFILE")) {
		tcapi_readAll_req(msg);
		return;
	}
	
	memset(nodeName, 0, sizeof(nodeName));
	splitName(msg->node, nodeName, "_");
	mergeNodeName(tree, nodeName);
	
	//0.create a new tree from flash.
#if defined(TCSUPPORT_BACKUPROMFILE)
	/*
		note:
		if execute function tc_parse_romfile() succeed,the function tc_parse_romfile() must return the true tree struct
	*/
	newTree = tc_parse_romfile();
#endif
	newCurNode = newTree;
	
	//1.find this node in new tree.
	for(j = 0;j < MAX_ARG_NUM; j++){
		if(strlen(nodeName[j])==0){
			break;
		}
		newCurNode = mxmlFindElement(newCurNode, newCurNode, nodeName[j],
			NULL, NULL, MXML_DESCEND);
	}
	
	//2.find this in old tree.
	for(i = 0; i < MAX_ARG_NUM; i++){
		if(strlen(nodeName[i]) == 0){
			break;
		}
		parentNode = curNode;
		curNode = mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
	}
	//3.if this node is NULL,goto error.if curNode is NULL, but newCurNode is not NULL,use newCurNode to update curNode.
	if( curNode == NULL && newCurNode == NULL ){  // there is no this node,so goto Error_Handle.
		msg->retval = TCAPI_NONE_NODE;
		goto Error_Handle;
	}
	
	//4.start to set attr/value from to specify node.
	if(i == 1){ 	//it means j == 1 too. it means this node is parent node.ex "Lan" "Wan"...	
		childNode= mxmlWalkNext(curNode, curNode, MXML_DESCEND);
		
		//if this node is parent node, delete all of its child node,but parent node can not be deleted.
		while(childNode){	
			mxmlDelete(childNode);
			childNode  = mxmlWalkNext(curNode, curNode, MXML_DESCEND);
		}
		//set attr/value from flash to spcify node.
		newChildNode = mxmlWalkNext(newCurNode, newCurNode, MXML_DESCEND);
		
		while(newChildNode){
			if( (newChildNode->type == MXML_ELEMENT)
				&& (newChildNode->value.element.name) ){
				
				if(newChildNode->value.element.num_attrs == 0){	//it means this parent node may be 3 layer node.
					l2Node = mxmlNewElement(curNode, newChildNode->value.element.name);
					if(l2Node == NULL)
						goto Error_Handle;					
				}
				else{  //2 layer node
					if(l2Node == NULL)
						l2Node = curNode;
					
					l3Node = mxmlNewElement(l2Node, newChildNode->value.element.name);
					if(l3Node == NULL)
						goto Error_Handle;
										
					//set attribute/value
					attr = newChildNode->value.element.attrs;
					for(ii = 0; ii < newChildNode->value.element.num_attrs; ii++ ){
						mxmlElementSetAttr(l3Node, attr->name, attr->value);
						attr ++;
					}
				}	
			}
			newChildNode  = mxmlWalkNext(newChildNode, newCurNode, MXML_DESCEND);
		}
	}
	else { 
		if(curNode){ // this node is not parent node,so delete it.
			mxmlDelete(curNode);
		}
		if(newCurNode){ //if this node is in flash,create it.
			if( (newCurNode->type == MXML_ELEMENT)
				&& (newCurNode->value.element.name) ){			
				if(newCurNode->value.element.num_attrs == 0){//may be 3 layer node
					//create this node. layer 2 node.
					l2Node = mxmlNewElement(parentNode, newCurNode->value.element.name);
					
					if(l2Node == NULL)
						goto Error_Handle;
					
					newChildNode = mxmlWalkNext(newCurNode, newCurNode, MXML_DESCEND);
					while(newChildNode){
						if( (newChildNode->type == MXML_ELEMENT)
							&& (newChildNode->value.element.name) ){
							
							//create this node. the last layer node.
							l3Node = mxmlNewElement(l2Node, newChildNode->value.element.name);
							if(l3Node == NULL)
								goto Error_Handle;

							attr = newChildNode->value.element.attrs;
							for(ii = 0; ii < newChildNode->value.element.num_attrs; ii++ ){
								mxmlElementSetAttr(l3Node, attr->name, attr->value);
								attr ++;
							}
						}
						newChildNode  = mxmlWalkNext(newChildNode, newCurNode, MXML_DESCEND);
					}
				} 
				else{//2 layer node
					
					l3Node = mxmlNewElement(parentNode, newCurNode->value.element.name);
					if(l3Node == NULL)
						goto Error_Handle;
					
					//set attribute/value
					attr = newCurNode->value.element.attrs;
					for(ii = 0; ii < newCurNode->value.element.num_attrs; ii++ ){
						mxmlElementSetAttr(l3Node, attr->name, attr->value);
						attr ++;
					}
				}
			}
		}
	}
	
Error_Handle:
	mxmlDelete(newTree);
	return;
#endif
}

/*______________________________________________________________________________
**	runNodeVerify
**
**	descriptions:
**		do the verify function of the curNode.
**
**	parameters:
**		curNode: 		Specify the current node.
**		name: the name of current node.
**
**	return:
**		Success: 		0
**		FAIL:     -1
**____________________________________________________________________________
*/
int
runNodeVerify(mxml_node_t *curNode, char name[][MAX_NODE_NAME]){
	int ret = FAIL;
	cfg_node_t* pAllCfgNode=NULL;
	/*Find out the node index*/
	for(pAllCfgNode=getAllCfgTable(); pAllCfgNode->name != NULL; pAllCfgNode++){
		if(strcmp(name[0], pAllCfgNode->name) == 0){
			if(pAllCfgNode->cfg_verify == NULL){
				ret=SUCCESS;
			}
			else if(pAllCfgNode->cfg_verify(curNode) == SUCCESS){
				ret=SUCCESS;
			}
			break;
		}
	}
	return ret;
}
