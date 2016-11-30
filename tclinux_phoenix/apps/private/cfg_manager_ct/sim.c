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
#include "sim.h"
#include <signal.h>

static int writeBeforeReboot = 0;

int setSimCardModifyAttrFlag(mxml_node_t *top,int fileIndex){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	char attrName[32] = {0};
	char buf[32] = {0};
	int modifyFlag = {0};
	int i = fileIndex/MAX_MODIFY_FLAG_NUM;
	int j = fileIndex%MAX_MODIFY_FLAG_NUM;
		
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	sprintf(attrName,"modifyFlag%d",i);
	getAttrValue(top,nodeName,attrName, buf);

	modifyFlag = atoi(buf) | (1 << j);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d",modifyFlag);
	setAttrValue(top,nodeName,attrName, buf);
	return 0;
}

int processUpdateSimFileModifyFlagAction(mxml_node_t *top,char* nodeName){
	ptrCfgNodeSimMapping pCfgNodeSimMapping = NULL;	
	int filesArray[MAX_SIM_FILE_NUM] = {-1};
	int idx = 0;
	int i = 0;

	pCfgNodeSimMapping = getCfgNodeSimMappingTable();
	for(;(pCfgNodeSimMapping != NULL) && (pCfgNodeSimMapping->cfgNode != NULL); pCfgNodeSimMapping++){
		if(strcmp(nodeName,	pCfgNodeSimMapping->cfgNode)==0){	//find the node
			idx = setFilesIndex(pCfgNodeSimMapping->files,filesArray);
			
			//tcdbg_printf("[%s:%d]nodeName=%s, idx = %d !!!===>\n",__FUNCTION__,__LINE__,nodeName,idx);
			for(i = 0; i < idx; i++){			
				
				//tcdbg_printf("[%s:%d]filesArray[%d] = %d !!!===>\n",__FUNCTION__,__LINE__,i,filesArray[i]);
				setSimCardModifyAttrFlag(top,filesArray[i]);
			}
			break;
		}
	}
	
	return 0;
}


int isSimCardExist(mxml_node_t *top){	
	char cardExist[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	if(getAttrValue(top, nodeName, "cardExist", cardExist) != TCAPI_PROCESS_OK ){
		//not find, new term
		return 0;
	}

	return atoi(cardExist);
}

int getCpeFlag(mxml_node_t *top){	
	char cpeFlag[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	if(getAttrValue(top, nodeName, "cpeFlag", cpeFlag) != TCAPI_PROCESS_OK ){
		//not find, new term
		return 0;
	}

	return atoi(cpeFlag);
}


int isSimAuthSuccess(mxml_node_t *top){	
	char simAuthRet[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	if(getAttrValue(top, nodeName, "simAuthRet", simAuthRet) != TCAPI_PROCESS_OK ){
		//not find, new term
		return 0;
	}

	return atoi(simAuthRet);
}



int alertUser(){
	tcdbg_printf("[%s:%d]cpeFlag is \"Card\", need add a correct SIM Card!!!\n",__FUNCTION__,__LINE__);		
	return 0;
}



int initFirstBootInfo(mxml_node_t *top){		//X CT-COM BIND
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], LOGIC_ID_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);
	setAttrValue(top, nodeName, "registerStatus", "99");
	setAttrValue(top, nodeName, "registerResult", "99");
	return 0;
}


int setDevRegStatus(mxml_node_t *top,char* value){		//X CT-COM BIND
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], LOGIC_ID_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);
	setAttrValue(top, nodeName, "registerStatus", value);
	return 0;
}

int setDevregInform(mxml_node_t *top,char* value){		//X CT-COM BIND
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], CWMP_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);
	setAttrValue(top, nodeName, "devregInform", value);
	return 0;
}


int clearSIMMac(){
	char filePath[64] = {0};
	char* buf = "<P00=\"000000000000\">";
	FILE *fp = NULL;
	int ret = -1;
	int macFileIndex = 3;
	

	tcdbg_printf("[%s:%d]========>\n",__FUNCTION__,__LINE__);
#if 0
	ret = simcard_fileread(macFileIndex,&len);
	if(ret < 0){
		tcdbg_printf("[%s:%d]simcard_fileread failed!fileIndex=%d!!!\n",__FUNCTION__,__LINE__,macFileIndex);
		return -1;
	}

#endif
	sprintf(filePath,SIM_FILE_DIR,macFileIndex);
	fp = fopen(filePath, "w");
	if(fp == NULL){
		tcdbg_printf("[%s:%d]fopen fail!!! filePath=%s\n",__FUNCTION__,__LINE__,filePath);
		return -1;
	}

	fwrite(buf, sizeof(char),strlen(buf), fp);
	fclose(fp);

	ret = simcard_filewrite(macFileIndex,strlen(buf));
	if(ret != SIM_OP_SUCCESS){
		tcdbg_printf("[%s:%d]simcard_filewrite failed!fileIndex=%d!!!\n",__FUNCTION__,__LINE__,macFileIndex);
		return -1;
	}

	return 0;
}

int getMacAddr(char* macAddr)
{
   	FILE *fptmp;  
	unsigned char flash_mac_addr[6]={0};
	
	/* Read bootloader from flash */
	system("cat /dev/mtd0 > /tmp/boot.bin");

	/*Read MAC*/
	fptmp = fopen("/tmp/boot.bin", "r+b");
	if( fptmp == NULL ){
   		tcdbg_printf("open boot.bin file error!!\n");
    	system("rm /tmp/boot.bin");        
    	return FAIL;
	}

	fseek(fptmp, 0x0000ff48L, SEEK_SET);
	fread(flash_mac_addr, 6, 1, fptmp);
	sprintf(macAddr, "%02x%02x%02x%02x%02x%02x",flash_mac_addr[0],flash_mac_addr[1],flash_mac_addr[2],flash_mac_addr[3],flash_mac_addr[4],flash_mac_addr[5]);
	
    fclose(fptmp);
	system("rm /tmp/boot.bin");        
}	


int isMacSNSame(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	char retValue[MAXLEN_TCAPI_MSG] = {0};
	char attrValue[MAXLEN_TCAPI_MSG] = {0};
	char retMac[32] = {0};
	char macAddr[32] = {0};
	
	getValueByParameterFromSimFile(retMac,3,"P00");		//MAC
	getMacAddr(macAddr);
	if(strcasecmp(retMac,macAddr) != 0){		//not the same		
		tcdbg_printf("[%s:%d] retValue=%s,macAddr=%s, mac is different!!!\n",__FUNCTION__,__LINE__,retMac,macAddr);
		return 0;
	}

	
	getValueByParameterFromSimFile(retValue,0,"P00");		//CardSN
	memset(attrValue,0,sizeof(attrValue));
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);
	getAttrValue(top, nodeName, "CardSN", attrValue);
	if(strcmp(retValue,attrValue)){		//not the same
		tcdbg_printf("[%s:%d] retValue=%s,attrValue=%s, CardSN is different!!!\n",__FUNCTION__,__LINE__,retValue,attrValue);
		return 0;
	}
	
	return 1;
}

int simFirstDirAuthentication(mxml_node_t *top){	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	char cardKey[MAXLEN_TCAPI_MSG] = {0};
	int ret = -1;

	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);
	
	getAttrValue(top, nodeName, "CardKey", cardKey);	
	ret = simcard_verify(cardKey,strlen(cardKey));
	if(ret != 0){
		if(ret == SIM_VERIFYCARD_ERR){ /* Device auth card fail */
			signalCwmp(SIM_CARD, SIM_CARD_AUTH_CARD);/* tony add Alarm Inform */
		}
		else if(ret == SIM_VERIFYCPE_ERR){/* Card auth Device fail */
			signalCwmp(SIM_CARD, SIM_CARD_AUTH_DEVICE);/* tony add Alarm Inform */
		}
		else{	
		signalCwmp(SIM_CARD, SIM_CARD_AUTH_DEVICE);/* tony add Alarm Inform */
		}
		setAttrValue(top, nodeName, "simAuthRet", "0");
	}else{			
		setAttrValue(top, nodeName, "simAuthRet", "1");
	}

	return ret;
}

int getInfoFromSimCard(mxml_node_t *top){	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	char retValue[MAXLEN_TCAPI_MSG] = {0};
	
	strcpy(nodeName[0], LOGICID_NODE);
	strcpy(nodeName[1], LOGICID_NODE_ENTRY);
	
	getValueByParameterFromSimFile(retValue,0,"P01");		//Username
	setAttrValue(top, nodeName, "Username", retValue);

	memset(retValue,0,sizeof(retValue));
	getValueByParameterFromSimFile(retValue,0,"P02");		//Password
	setAttrValue(top, nodeName, "Password", retValue);

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	memset(retValue,0,sizeof(retValue));
	getValueByParameterFromSimFile(retValue,0,"P00");		//Password
	setAttrValue(top, nodeName, "CardSN", retValue);

	memset(retValue,0,sizeof(retValue));
	getValueByParameterFromSimFile(retValue,0,"P03");		//Password
	setAttrValue(top, nodeName, "Ver", retValue);
	
	return 0;
}


//init flag
void clearSimCardFlag(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	char attrName[32] = {0};
	int i = 0;		

	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	setAttrValue(top, nodeName, "cardExist", "0");
	setAttrValue(top, nodeName, "needReadSim", "0");
	setAttrValue(top, nodeName, "needWriteSim", "0");
	setAttrValue(top, nodeName, "firstBoot", "0");
	setAttrValue(top, nodeName, "Status", "0");
	setAttrValue(top, nodeName, "simAuthRet", "0");
	setAttrValue(top, nodeName, "itmsActionStart", "0");
	setAttrValue(top, nodeName, "needNotifyWrite", "0");
	setAttrValue(top, nodeName, "needReadSpecialFile", "0");
	

	for(i = 0; i < MAX_MODIFY_FLAG_NUM; i++){		
		memset(attrName,0,sizeof(attrName));		
		sprintf(attrName,"modifyFlag%d",i);		
		setAttrValue(top, nodeName,attrName, "0");
	}

	return;
}

int readAllSIMCardFiles(){
	int fileIndex = 0;
	int fileSize = 0;	
	int i, j;
	update_data_para updatePara;
	int needUpdate = 0;

	//get sim file update flag
	simcard_update(&updatePara);
		
	for( fileIndex = 4; fileIndex < MAX_SIM_FILE_NUM; fileIndex++){		
			needUpdate = 0;
			if(simcard_status() != 0){		
					tcdbg_printf("[%s:%d]simCard not exist!fileIndex=%d!!!\n",__FUNCTION__,__LINE__,fileIndex);
				return -2;
			}
			//only for Wan_IPv4&Wan_IPv6, if wan is update,  Wan_IPv4&Wan_IPv6 consider as update too.
			if((fileIndex >= 7 && fileIndex <= 38) || (fileIndex >= 150 && fileIndex <= 165)){
				if((updatePara.data[0] & 0x1F) || (updatePara.data[1] & 0xFF) || (updatePara.data[2] & 0xE0)){		
					needUpdate = 1;
				}
			}	
		
			i = (fileIndex-4)/8;
			j = (fileIndex-4)%8;
			
			if(updatePara.data[i] & (1 << (7-j))){
				needUpdate = 1;
			}

			if(needUpdate == 1)
				simcard_fileread(fileIndex,&fileSize);
	}

	return 0;
}

mxml_node_t* simCardInit(mxml_node_t *top){
	int cpeFlag = 0;	//read from romfile.[ 0:new term; 1:card; 2: not card]
	int isExist = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
	char buf[32] = {0}; 
	int resetNum = 3;
	int i = 0;
	char romfilepath[64]={0};

	strcpy(nodeName[0], SIM_CARD_NODE_NAME);
	strcpy(nodeName[1], ENTRY_NODE_NAME);

	//init flag
	clearSimCardFlag(top);

	//check cpeFlag
	if(getAttrValue(top, nodeName, "cpeFlag", buf) != TCAPI_PROCESS_OK ){
		//not find, new term
		cpeFlag = 0;
	}else{
		cpeFlag = atoi(buf);
	}

	//is sim card exsit
	if(simcard_status() == 0){
		isExist = 1;			
		setAttrValue(top, nodeName, "cardExist", "1");
	}else{
			while(i < resetNum){
				tcdbg_printf("[%s:%d]i=%d!!!\n",__FUNCTION__,__LINE__,i);
		
				if(simcard_reset() == 0){	
					tcdbg_printf("[%s:%d]simcard_reset OK!!!\n",__FUNCTION__,__LINE__);
					break;
				}
				i++;
			}

			if(i < resetNum && simcard_status() == 0){
				isExist = 1;			
				setAttrValue(top, nodeName, "cardExist", "1");
			}
	}
	
	tcdbg_printf("simCardInit cpeFlag=%d,isExist=%d !!!======>\n",cpeFlag,isExist);
	if(isExist){		
		if(cpeFlag == 0){		//0:new term	
			setAttrValue(top, nodeName, "firstBoot", "1");			
			setDevregInform(top, "1");			
			getInfoFromSimCard(top);
		}
		else{ 
			if(cpeFlag == 2 || !isMacSNSame(top)){	//1:card &check mac fail ; 2: not card
				if(top != NULL){
					mxmlDelete(top);
				}
				getDefaultRomfile(romfilepath);
				top = load_checksum_def_romfile(romfilepath);
				if(top == NULL){
					tcdbg_printf("[%s:%d]load_def_romfile fail!!! ===>\n",__FUNCTION__,__LINE__);
				}else{
					//tree = parser_romfile(DEF_ROMFILE_PATH);
					clearSIMMac();
					setDevregInform(top,"1");
					setAttrValue(top, nodeName, "firstBoot", "1");
					setAttrValue(top, nodeName, "cardExist", "1");					
				}	
			}else{
				//auth
				if(simFirstDirAuthentication(top) == 0){
					tcdbg_printf("[%s:%d]simFirstDirAuthentication success!!! ===>\n",__FUNCTION__,__LINE__);
					setAttrValue(top, nodeName, "needReadFileFirst", "1");					
				}else{
					tcdbg_printf("[%s:%d]simFirstDirAuthentication fail!!! ===>\n",__FUNCTION__,__LINE__);
				}
			}		
		}
		
		setDevRegStatus(top,"0");
		setAttrValue(top, nodeName, "needReadSpecialFile", "1");
		getInfoFromSimCard(top);						
	}else{	//sim Card not Exist
		if(cpeFlag == 0){	////0:new term
			//initFirstBootInfo(tree);			
			setAttrValue(top, nodeName, "cpeFlag", "2"); 
		}else if(cpeFlag == 1){
			alertUser();
		}
	}

	return top;
}

#if defined(TCSUPPORT_CT_BLOCK_PROCESS)
#define CFG_MANAGER_STATE_FILE_PATH "var/tmp/cfg_manager_up"
extern int isCfgManagerUp;
extern int isNeedReadSpecFile;

void checkCfgManagerUP(){
	while(1){
		if(isNeedReadSpecFile)
			break;

		sleep(1);
	}
	isNeedReadSpecFile = 0;
}

#endif

/* tony add to restore no keypara from sim card */
static int NeedReadSimCard = 0;
int getNeedReadSimCard(void)
{
	return NeedReadSimCard;
}

void setNeedReadSimCard(int value)
{

	NeedReadSimCard = value;
	return;
}

int isNodeInArray(char cfgSpecNode[][MAX_NODE_NAME], char* nodeName){
	int i = 0;

	if(nodeName == NULL){
		tcdbg_printf("[%s:%d]nodeName=NULL\n",__FUNCTION__,__LINE__);		
		return 0;
	}

	
	for(i = 0; strlen(cfgSpecNode[i]) != 0; i++){		
		if(strcmp(cfgSpecNode[i],nodeName) == 0){
			return 1;
		}
	}
	
	return 0;
}


extern mxml_node_t **gTreePtr;
extern cfg_node_t *all_cfg_node;

void callCfgWriteFunc(){	
	mxml_node_t *tree = get_node_top();
	mxml_node_t *node=NULL;
	cfg_node_t *ptr=NULL;
	int i = 0;
	
	char cfgSpecNode[][MAX_NODE_NAME] = {
		"deviceAccount",
		"Wan",
		"Cwmp"
	};
	
	/*save the tree address to gTreePtr*/
	if(tree == NULL){
		return -1;
	}
		
	/*Create all node configuration file*/
	for(ptr = all_cfg_node; ptr!=NULL && ptr->name!= NULL; ptr++){
		node = mxmlFindElement(tree, tree, all_cfg_node[i].name,
	  		NULL, NULL,MXML_DESCEND);
		if((node !=NULL) && isNodeInArray(&cfgSpecNode,ptr->name) &&(ptr->cfg_write!=NULL) ){
			ptr->cfg_write(tree,node);
			/*fprintf(stdout,"run %s write\n",all_cfg_node[i].name);*/
		}
		i++;
	}

	return 0;
}

int stopWanExceptTr069(){
	char nodeName[256] = {0};
	char serviceList[64] = {0};	
	char ifname[16] = {0};
	int pvc_index = 0;
	int entry_index = 0;
	int ret = 0;

	for(pvc_index = 0; pvc_index < MAX_PVC_NUM; pvc_index++){
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			memset(nodeName,0,sizeof(nodeName));
			sprintf(nodeName,WAN_PVC_ENTRY_NODE,pvc_index,entry_index);
			ret = tcapi_get(nodeName,"ServiceList",serviceList);
			if(ret != TCAPI_PROCESS_OK){
				continue;
			}
			
			if(strstr(serviceList,"TR069") != NULL){		//do not commt tr069 wan connection
				continue;
			}
			
			if(tcapi_get(nodeName, "IFName", ifname) == TCAPI_PROCESS_OK ){
				/*Stop Wan interface*/
				stop_interface(pvc_index, entry_index, ifname);
			}			
		}
	}	
	return 0;	

}

#define SIMSIGFILE  "/var/tmp/simsignal"
void writeSingalFile(int num){
	FILE *fp = NULL;
	char buf[10] = "\0";
	fp = fopen(SIMSIGFILE, "w");
	if(fp == NULL){
		tcdbg_printf("\n writeSingalFile: open failed");
		return;
	}
	snprintf(buf, sizeof(buf),"%d",num);
	fwrite(buf, sizeof(char), strlen(buf), fp);
	fclose(fp);
	//tcdbg_printf("writeSingalFile: buf=%s!!\n",buf);
}

void sim_signal_handler(int signo, siginfo_t *info, void *myact){
	if (signo == SIGUSR2){
		tcdbg_printf("sim: test SIGUSR2 info=%d!!\n",info->si_int);
		writeBeforeReboot = 1;
		tcapi_set(SIMCARD_NODE,"needWriteSim","1");
	}
}


void
simThread(void* unused)
{
	char needReadSim[8] = {0};
	char needWriteSim[8] = {0};
	char needReadSpecialFile[8] = {0};
	char needReadFileFirst[8] = {0};
	int retryTime = SIM_CARD_EXIST_CHECK_TIME;
	int ret = -1;
	int pidfd = -1;

	
	char simLock[32] = {0};
	struct sigaction sim_action;

#if defined(TCSUPPORT_CT_BLOCK_PROCESS)
	checkCfgManagerUP();
	tcapi_get(SIMCARD_NODE,"needReadSpecialFile",needReadSpecialFile);
	if(strcmp(needReadSpecialFile,"1") != 0){
		signalBlockProcess();
		
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
			InitWanNodeIndex();
#endif
	}
#endif
		
	sigemptyset(&sim_action.sa_mask);
	sim_action.sa_flags = SA_SIGINFO;

	/*  Set up SIGUSR2 handlers  */
	sim_action.sa_handler = sim_signal_handler;
	if (sigaction (SIGUSR2, &sim_action, NULL) != 0 ){
		tcdbg_printf("[%s:%d] sigaction err!\n",__FUNCTION__,__LINE__);	
	}
	else{
		writeSingalFile(getpid());
	}
		
	pidfd =open(SIMCARD_LOCK_FILE,O_RDWR | O_CREAT);
	while(1){
		if(pidfd<0){
			ret = simcard_status();			
			tcdbg_printf("[%s:%d]pidfd=%d\n",__FUNCTION__,__LINE__,pidfd);		
		}else{
			writew_lock(pidfd,0,SEEK_SET,0);
			ret = simcard_status();
			un_lock(pidfd,0,SEEK_SET,0);
		}

		
		if(ret == 0){
			retryTime = SIM_CARD_EXIST_CHECK_TIME;
			/* tony add to restore no keypara from sim card */
			if(getNeedReadSimCard())
			{
				ret  = system("/userfs/bin/simCard restoreSim");
				if(ret == 0){
					tcdbg_printf("[%s:%d]restoresimCard OK, Will Reboot!!\n",__FUNCTION__,__LINE__);
					system("reboot");
				}
				else
					tcdbg_printf("[%s:%d]restoresimCard error!!ret = %d\n",__FUNCTION__,__LINE__, ret);		
			}
			
			tcapi_set(SIMCARD_NODE,"cardExist","1");
			if(TCAPI_PROCESS_OK == tcapi_get(SIMCARD_NODE,"needReadSim",needReadSim) && strcmp(needReadSim,"1") == 0){
				tcdbg_printf("[%s:%d]needReadSim=1\n",__FUNCTION__,__LINE__);		
				system("/userfs/bin/simCard updateRomfile");				
				tcapi_set(SIMCARD_NODE, "needReadSim", "0");
			}

			if(TCAPI_PROCESS_OK == tcapi_get(SIMCARD_NODE,"needWriteSim",needWriteSim) && strcmp(needWriteSim,"1") == 0){				
				tcdbg_printf("[%s:%d]needWriteSim=1\n",__FUNCTION__,__LINE__);		
				system("/userfs/bin/simCard updateSim");	
			}	
			
			if(TCAPI_PROCESS_OK == tcapi_get(SIMCARD_NODE,"needReadSpecialFile",needReadSpecialFile) && strcmp(needReadSpecialFile,"1") == 0){
				system("/userfs/bin/simCard updateSpecialRomfile 0,4,7,23,39,40,54,150,174");					
				callCfgWriteFunc();
				tcapi_set(SIMCARD_NODE, "needReadSpecialFile", "0");
#if defined(TCSUPPORT_CT_BLOCK_PROCESS)
				signalBlockProcess();
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
	InitWanNodeIndex();
#endif

#endif
			}

			if(TCAPI_PROCESS_OK == tcapi_get(SIMCARD_NODE,"needReadFileFirst",needReadFileFirst) && strcmp(needReadFileFirst,"1") == 0){
				tcdbg_printf("[%s:%d]needReadFileFirst=1\n",__FUNCTION__,__LINE__);		
				writew_lock(pidfd,0,SEEK_SET,0);
				readAllSIMCardFiles();
				un_lock(pidfd,0,SEEK_SET,0);
				tcapi_set(SIMCARD_NODE, "needReadFileFirst", "0");
			}
			
		}else{
			tcapi_set(SIMCARD_NODE,"cardExist","0");
			tcdbg_printf("[%s:%d]isCardExist=0\n",__FUNCTION__,__LINE__);		
			if(retryTime == 0){		//delay 1s check again, then signal CWMP
				signalCwmp(SIM_CARD, SIM_CARD_OFF_LINE);
				stopWanExceptTr069();
				retryTime = SIM_CARD_EXIST_CHECK_TIME;
				writeSingalFile(0);
				break;
			}
			else{
				retryTime--;		
			}
		}
		if(1 == writeBeforeReboot){
			tcdbg_printf("simthread:needwrite!!\n");
			tcapi_set(SIMCARD_NODE,"needWriteSim","1");
			writeBeforeReboot = 2;
			continue;
		}
		else if(2 == writeBeforeReboot){
			writeBeforeReboot = 0;
			writeSingalFile(0);
			tcdbg_printf("simthread:write OK!!\n");
		}
		
		sleep(1);
	}
	pthread_exit(0);
}

