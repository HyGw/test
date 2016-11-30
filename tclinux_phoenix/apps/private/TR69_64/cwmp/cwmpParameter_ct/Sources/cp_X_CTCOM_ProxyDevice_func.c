#include "Global_res.h"
#include "cp_X_CTCOM_ProxyDevice.h"
#include "upnp_parse.h"

pDeviceList_Header pDeviceList = NULL; 
pUPnPAction_Header pUPnPAct_Header = NULL;
extern int iUPnPDMModifyFlag;
int iUPnPDownloadInstance  = 0;
int upnpDMErrCode = 0;
extern int upnpDMsockfd;

enum templateType{
	AP_Internet=0,
	AP_IPTV,
	AP_VoIP,
	STB_Internet,
	STB_IPTV,
	STB_VoIP,
	Screen4_Internet,
	Screen4_IPTV,
	Screen4_VoIP,
	HomeCenter_Internet,
	HomeCenter_NAS,
	HomeCenter_IPTV,
	HomeCenter_VoIP,
	Plc
};


int isNodenameExist(int num, char* node_name, char* nodeModify){
	int i;
	int ret;
	char node_attr[MAXLEN_ATTR_NAME];	
	char tmp[MAXLEN_TCAPI_MSG];
	char* p;
	char* delim = ",";

	for(i = 0; i < num; i++){
		sprintf(node_attr, "NodeModify%d", i+1);	
		ret =  tcapi_get(node_name, node_attr, tmp);
		if(ret < 0){
			return 0;
		}
		p = strtok(tmp,delim);
		while(p){								
			if(strcmp(p,nodeModify) == 0){
				tcdbg_printf("%s:find nodeModify=%s\n",__FUNCTION__,nodeModify);
				return 1;
			}
			p = strtok(NULL,delim);
		}
		
	}
	
	return 0;
}

//Add the node which is modify, eg. NodeModify{i}="Enable,SSID,BeaconType"
int nodeModifyAddFunc(char* node_name, char* nodeModify){	
	char node_attr[MAXLEN_ATTR_NAME];
	char node_value[MAXLEN_TCAPI_MSG];
	char tmp[MAXLEN_TCAPI_MSG];
	int num = 0;
	int len,ret;

	ret =  tcapi_get(node_name, "num", tmp);
	if(ret < 0){
		//attribute num is not exist
		tcapi_set(node_name, "NodeModify1", nodeModify);
		tcapi_set(node_name, "num", "1");
	}
	else{
		num = atoi(tmp);
		if(isNodenameExist(num,node_name,nodeModify) == 0){		//node is not exist in NodeModify, need add		
			//get the NodeModify{num}
			memset(node_attr, 0, MAXLEN_ATTR_NAME);
			memset(tmp, 0, MAXLEN_TCAPI_MSG);
			sprintf(node_attr, "NodeModify%d", num);	
			tcapi_get(node_name, node_attr, tmp);

			//check the max len				
			memset(node_value, 0, MAXLEN_TCAPI_MSG);
			len = strlen(tmp) + strlen(nodeModify) + 1;
			if(len >= 32/*MAXLEN_TCAPI_MSG*/){
				//more than MAXLEN_TCAPI_MSG, need  the nodeName in new NodeModify{i}
				memset(node_attr, 0, MAXLEN_ATTR_NAME);
				sprintf(node_attr, "NodeModify%d", num+1); 
				tcapi_set(node_name, node_attr, nodeModify);
				sprintf(node_value,"%d",num+1);				
				tcapi_set(node_name, "num", node_value);
			}else{
				if(strlen(tmp) == 0){
					strcpy(node_value,nodeModify);
				}
				else{
					sprintf(node_value,"%s,%s",tmp,nodeModify);
				}
				tcapi_set(node_name, node_attr, node_value);
			}
		}
	}

	return 0;
}


//Add the node which is modify, eg. NodeModify{i}="Enable,SSID,BeaconType"
int serviceProfileNodeModifyAdd(int templateType, int entryIdx, char* nodeModify){	
	char node_name[MAXLEN_NODE_NAME];

	tcdbg_printf("%s:templateType=%d,entryIdx=%d,nodeName=%s\n",__FUNCTION__,templateType,entryIdx,nodeModify);
	switch(templateType){
		case AP_Internet:
			//get the current number of NodeModify
			memset(node_name, 0, MAXLEN_NODE_NAME);
			sprintf(node_name, "UPnPDMAPInt_Entry%d", entryIdx); 	
			nodeModifyAddFunc(node_name,nodeModify);
			break;
		case AP_IPTV:
			break;
		default:
			break;
	}

	return 0;
}

int doUPnPRoutineAction(pUPnPAction pUPnPAct, DeviceNodePtr DeviceListNode){
	int ret = 0;
	int index = 0;
	if(pUPnPAct == NULL || DeviceListNode ==NULL){
		tcdbg_printf("%s:pUPnPAct == NULL || DeviceListNode ==NULL\n",__FUNCTION__);
		return -1;
	}
	
	if(pUPnPAct->actType == OP_ADD_DEVICE){
		ret = AddUPnPDMDeviceListObject(pUPnPAct->devNum,DeviceListNode);
	}else if(pUPnPAct->actType == OP_DEL_DEVICE){
		index = getIndexByUPnPInstance(pUPnPAct->devNum);
		ret = DelUPnPDMDeviceListObject(index,DeviceListNode);
	}else{
		ret = -1;
	}

	if(ret == 0){
		tcdbg_printf("%s:success!!!pUPnPAct->actType =%d, devnum=%d\n",__FUNCTION__,pUPnPAct->actType,pUPnPAct->devNum);
	}else{
		tcdbg_printf("%s:error!!!pUPnPAct->actType =%d, devnum=%d\n",__FUNCTION__,pUPnPAct->actType,pUPnPAct->devNum);
	}
	return ret;
}

int doUPnPRoutineActionList(DeviceNodePtr DeviceListNode){	
	pUPnPAction pUPnPAct = NULL;
	int ret = 0;

	if(pUPnPAct_Header == NULL){
		tcdbg_printf("%s:pUPnPAct_Header == NULL\n",__FUNCTION__);
		return -1;
	}

	pthread_mutex_lock(&pUPnPAct_Header->upnpActLock);
	
	tcdbg_printf("%s:pUPnPAct_Header->num=%d\n",__FUNCTION__,pUPnPAct_Header->num);
	pUPnPAct = pUPnPAct_Header->pUPnPAction;
	while(pUPnPAct){			//the fisrt upnp action node
		doUPnPRoutineAction(pUPnPAct,DeviceListNode);
		pUPnPAct_Header->pUPnPAction = pUPnPAct->next;
		os_free(pUPnPAct);
		pUPnPAct_Header->num -=1;
		pUPnPAct = pUPnPAct_Header->pUPnPAction;
		
	}
	pthread_mutex_unlock(&pUPnPAct_Header->upnpActLock);
	return 0;
}

int freeUPnPRoutineActionList(){	
	pUPnPAction pUPnPAct = NULL;
	int ret = 0;

	if(pUPnPAct_Header == NULL){
		tcdbg_printf("%s:pUPnPAct_Header == NULL\n",__FUNCTION__);
		return 0;
	}

	pthread_mutex_lock(&pUPnPAct_Header->upnpActLock);
	tcdbg_printf("%s:pUPnPAct_Header->num=%d\n",__FUNCTION__,pUPnPAct_Header->num);
	pUPnPAct = pUPnPAct_Header->pUPnPAction;
	while(pUPnPAct){			//the fisrt upnp action node
		pUPnPAct_Header->pUPnPAction = pUPnPAct->next;
		os_free(pUPnPAct);
		pUPnPAct_Header->num -=1;
		pUPnPAct = pUPnPAct_Header->pUPnPAction;
		
	}
	pthread_mutex_unlock(&pUPnPAct_Header->upnpActLock);
	return 0;
}


int getTR69ErrCodeByUPnPDM(){
	int tr69ErrCode = 0;
	switch(upnpDMErrCode){
		case 602:		//9003	Optional Action Not Implemented
		case 604:		//Human Intervention Required
		case 606:		//Action not authorized
		case 607:		//Signature failure
		case 608:		//Signature missing
		case 609:		//Not encrypted
		case 610:		//Invalid sequence
		case 611:		//Invalid control URL
		case 612:		//No such session
			tr69ErrCode = 9001;
			break;
		case 501:		//Action Failed
		case 603:		//Out of Memory
		case 803:		//Unsupported protocol
		case 804:		//Download failure
			tr69ErrCode = 9002;
			break;
		case 401:		//Invalid Action
		case 402:		//Invalid Args
		case 707:		//Mutiple Set
			tr69ErrCode = 9003;
			break;	
		case 605:		//String Argument Too Long
		case 708:		//Resource Temporarily Unavailable
		case 709:		
			tr69ErrCode = 9004;
			break;
		case 701:		//Invalid Argument Syntax
		case 702:
			tr69ErrCode = 9005;
			break;
		case 704:
			tr69ErrCode = 9006;
			break;
		case 600:		//Argument Value Invalid
		case 601:		//Arguement Value Out of Range
		case 801:		//Invalid URI
		case 705:		//Invalid Value
			tr69ErrCode = 9007;
			break;
		case 706:		//Read Only Violation
			tr69ErrCode = 9008;
			break;
		default:
			break;
	}
	
	tcdbg_printf("%s :tr69ErrCode = %d,upnpDMErrCode = %d\n",__FUNCTION__,tr69ErrCode,upnpDMErrCode);	
	upnpDMErrCode = 0;
	return tr69ErrCode;
}

int getDevInstance(char*buf,const char* delim, int* num, int instanceList[]){
	char *p=NULL;
	char tmpValue[32]={0};	
	char* tmp = NULL;
	int i=0;	
	char* name = "instance";

	tmp = strstr(buf,name);
	tmp = tmp + strlen(name) + 1;
	strcpy(tmpValue, tmp);	
	/*Get parent node name*/
	p = strtok(tmpValue,delim);
	
	while(p){		
		instanceList[i++] = atoi(p);		
		p = strtok(NULL,delim);
	}

	*num = i;
	return 0;
}

char g_operationStatus[256] = {'\0'};
char* getOperationStatus(int index)
{
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_STATUS_INDEX,BMS_DOWNLOADSTATUS_OPERATIONSTATUS,g_operationStatus);
	
	return g_operationStatus;
}

int setUPnPDownloadInstance(int index)
{
	iUPnPDownloadInstance = index;
	return 0;
}

int getUPnPDownloadInstance(void)
{
	return iUPnPDownloadInstance;
}

int getDeviceListIndexByDevNode(DeviceNodePtr pDevNode){
	DeviceNodePtr pNode = NULL;
	int index  = -1;
	
	if(pDevNode == NULL){
		tcdbg_printf("%s :pDevNode == NULL,error\n",__FUNCTION__);  
		return -1;
	}

	pNode = pDevNode;
	while(pNode){		
		if(pNode->parentNode && strcmp(pNode->parentNode->name,UPNPDM_DEVICELIST) == 0){
			break;
		}
		pNode = pNode->parentNode;		
	}
	if(pNode)
		index = atoi(pNode->name);

	return index;
}


int getUPnPDMNodeByDevNode(DeviceNodePtr pDevNode, char* upnpNodeName){
	DeviceNodePtr pNode = NULL;
	char* upnpName = "/UPnP/DM";
	char nodeName[256] = {0};
	char buf[256] = {0};

	int len = 0;;

	if(pDevNode == NULL){
		tcdbg_printf("%s :pDevNode == NULL,error\n",__FUNCTION__);  
		return -1;
	}

	pNode = pDevNode->parentNode;
	while(pNode){
		if(pNode->parentNode && strcmp(pNode->parentNode->name,UPNPDM_DEVICELIST) == 0){
			break;
		}
		strcat(buf,pNode->name);
		strcat(buf,".");
		pNode = pNode->parentNode;		
	}
	len = strlen(buf)-1;
	buf[len--] = '\0';
	
	strcat(nodeName,"/UPnP/DM/");
	while(len){
		if(buf[len] == '.'){
			buf[len] = '\0';
			strcat(nodeName,buf+len+1);
			strcat(nodeName,"/");
		}
		len--;
	}
	
	strcat(nodeName,buf);
	strcat(nodeName,"/");
	strcat(nodeName,pDevNode->name);	
	strcpy(upnpNodeName,nodeName);
	return 0;
}


void freeBMSNode(pBMS_Node pBMSNode){
	if(pBMSNode == NULL){
		tcdbg_printf("%s :pNode == NULL, no need free!\n",__FUNCTION__);  
		return;
	}

	if(pBMSNode->CTCOM_Download.FileType){
		os_free(pBMSNode->CTCOM_Download.FileType);
		pBMSNode->CTCOM_Download.FileType = NULL;
	}
	
	if(pBMSNode->CTCOM_Download.URL){
		os_free(pBMSNode->CTCOM_Download.URL);
		pBMSNode->CTCOM_Download.URL = NULL;
	}

	
	if(pBMSNode->CTCOM_Download.SoftwareVersion){
		os_free(pBMSNode->CTCOM_Download.SoftwareVersion);
		pBMSNode->CTCOM_Download.SoftwareVersion = NULL;
	}

	
	if(pBMSNode->CTCOM_Download.HardwareVersion){
		os_free(pBMSNode->CTCOM_Download.HardwareVersion);
		pBMSNode->CTCOM_Download.HardwareVersion = NULL;
	}

	
	if(pBMSNode->CTCOM_Download.TimeWindow){
		os_free(pBMSNode->CTCOM_Download.TimeWindow);
		pBMSNode->CTCOM_Download.TimeWindow = NULL;
	}
	
	if(pBMSNode->CTCOM_Download.Username){
		os_free(pBMSNode->CTCOM_Download.Username);
		pBMSNode->CTCOM_Download.Username = NULL;
	}

	
	if(pBMSNode->CTCOM_Download.Password){
		os_free(pBMSNode->CTCOM_Download.Password);
		pBMSNode->CTCOM_Download.Password = NULL;
	}
	os_free(pBMSNode);
	return;
}


void freeDevNode(pDev_Node pNode){
	if(pNode == NULL){
		tcdbg_printf("%s :pNode == NULL, no need free!\n",__FUNCTION__);  
		return;
	}

	freeBMSNode(pNode->pBMS_Node);
	pNode->pBMS_Node = NULL;
	os_free(pNode);
	return;
}

pDev_Node getDevNodeByIndex(int index){
	pDev_Node pNode = NULL;
	if(pDeviceList == NULL){	
		tcdbg_printf("%s: no dev node exist\n", __FUNCTION__);
		return NULL;
	}

	pNode = pDeviceList->pDevNode;
	while(pNode){
		if(pNode->index == index){		//find the node
			break;
		}
		pNode = pNode->next;
	}

	return pNode;
}

pBMS_Node getBMSNodeByIndex(int index){	
	pDev_Node pNode = NULL;
	pBMS_Node pBMSNode = NULL;

	pNode =getDevNodeByIndex(index);
	if(pNode)
		pBMSNode = pNode->pBMS_Node;

	return pBMSNode;
}

int selectCurUnusedIndex(){
	int index = 1;
	pDev_Node pNode = NULL;
	if(pDeviceList == NULL){	
		tcdbg_printf("%s: the first dev node\n", __FUNCTION__);
		return index;
	}

	pNode = pDeviceList->pDevNode;
	while(pNode){
		if(pNode->index > index){	//index is exist		
			break;
		}else if(pNode->index == index){	//index is exist		
			 index++;
		}

		pNode = pNode->next;
	}

	return index;
}

/*need call os_free paravalue  to release memory after call getValueByName.
	eg.	buf = "140030dev=01&/UPnP/DM/DeviceInfo/FriendlyName=Hello"
		buf = "140030dev=01&/UPnP/DM/SoftwareDescription/=software for wireless AP"
*/
int getValueByName(char*buf, char* name, char** paravalue,const char* delim){
	char* tmpValue = NULL;
	char* tmpValue2 = NULL;	
	char* delimTmp = NULL;
	char* tmpBuf = buf;
	int len = 0;
	int len2 = 0; 

begin:	
	if(tmpValue = strstr(tmpBuf,name)){	
		tmpValue = tmpValue+strlen(name);
		if(*tmpValue == '/'){
			tmpValue +=1;
		}
		
		if(*tmpValue != '='){		
			tmpBuf = tmpValue;
			goto begin;
		}

		tmpValue +=1;
		if(tmpValue2 = strstr(tmpValue,"&")){					
			len = tmpValue2 - tmpValue;
		}else{
			len = strlen(tmpValue);
		}

		if(delim != NULL){
			delimTmp = strstr(tmpValue,delim);
			if(delimTmp != NULL){
				len2 = delimTmp - tmpValue;
				if(len2 < len){
					len = len2;
				}
			}
		}

		if(*paravalue = os_malloc(len+1)){
			memset(*paravalue,0,len+1);
			strncpy(*paravalue,tmpValue,len);
		}
		else{
			*paravalue = NULL;
			tcdbg_printf("%s : os_malloc fail!!!\n",__FUNCTION__,*paravalue );
			return -1;
		}
	}else{
		//tcdbg_printf("%s : get %s fail\n",__FUNCTION__,name);
		return -1;
	}
	
	return 0;
}

int getUPnPInstanceByTr69Index(int index){
	pDev_Node pNode = NULL;
	
	if(pDeviceList == NULL){
		tcdbg_printf("%s: No node exist\n",__FUNCTION__);
		return 0;
	}

	pNode = pDeviceList->pDevNode;
	while(pNode){
		if(pNode->index == index){	//find the UPnP devnum	
			return pNode->upnpInstance;
		}

		pNode = pNode->next;
	}

	return 0;

}


int getIndexByUPnPInstance(int devnum){
	pDev_Node pNode = NULL;
	
	if(pDeviceList == NULL){
		tcdbg_printf("%s: No node exist\n",__FUNCTION__);
		return 0;
	}

	pNode = pDeviceList->pDevNode;
	while(pNode){
		if(pNode->upnpInstance == devnum){	//find the UPnP devnum	
			return pNode->index;
		}

		pNode = pNode->next;
	}

	return 0;

}

int getBMSValueByIndex(int index, int bmsNameIndex1, int bmsNameIndex2, char* value){
	pBMS_Node pBMSNode = NULL;	
	u_buf* upbuffer = NULL;
	char* retBuf = NULL;

	if(pDeviceList == NULL){
		tcdbg_printf("%s:pDeviceList == NULL\n",__FUNCTION__);
		return NULL;
	}

	//pthread_mutex_lock(&pDeviceList->upnpLock);
	pBMSNode = getBMSNodeByIndex(index);
	if(pBMSNode == NULL){
		tcdbg_printf("%s: pBMSNode == NULL",__FUNCTION__);
		goto error;
	}

	switch(bmsNameIndex1){
		case BMS_REBOOT_INDEX:
			if(bmsNameIndex2 ==BMS_INVOKE){
				if(pBMSNode->Reboot.InvokeAction)
					strcpy(value,"1");
				else				
					strcpy(value,"0");
			}else if(bmsNameIndex2 ==BMS_REBOOT_STATUS){				
				strcpy(value,pBMSNode->Reboot.RebootStatus);
			}
			else{
				goto error;
			}
			break;
		case BMS_BASELINE_RESET_INDEX:
			if(bmsNameIndex2 ==BMS_INVOKE){
				if(pBMSNode->BaselineReset.InvokeAction)
					strcpy(value,"1");
				else				
					strcpy(value,"0");
			}else{
				goto error;
			}
			break;
		case BMS_DEVICE_STATUS_INDEX:
			if(bmsNameIndex2 ==BMS_INVOKE){
				if(pBMSNode->GetDeviceStatus.InvokeAction)
					strcpy(value,"1");
				else				
					strcpy(value,"0");
			}else if(bmsNameIndex2 ==BMS_DEVICE_STATUS){	
				strcpy(value,pBMSNode->GetDeviceStatus.DeviceStaus);				
			}else{
				goto error;
			}
			break;
		case BMS_CTCOM_DOWNLOAD_INDEX:
			switch(bmsNameIndex2){
				case BMS_INVOKE:
					if(pBMSNode->CTCOM_Download.InvokeAction)
						strcpy(value,"1");
					else				
						strcpy(value,"0");
					break;
				case BMS_DOWNLOAD_CHANNEL:
					sprintf(value,"%d",pBMSNode->CTCOM_Download.Channel);
					break;
				case BMS_DOWNLOAD_FILETYPE:				
					if(pBMSNode->CTCOM_Download.FileType)
						strcpy(value,pBMSNode->CTCOM_Download.FileType);
					else
						strcpy(value,"");
					break;
				case BMS_DOWNLOAD_URL:
					if(pBMSNode->CTCOM_Download.URL)
						strcpy(value,pBMSNode->CTCOM_Download.URL);
					else
						strcpy(value,"");
					break;
				case BMS_DOWNLOAD_SOFTWAREVERSION:				
					if(pBMSNode->CTCOM_Download.SoftwareVersion)
						strcpy(value,pBMSNode->CTCOM_Download.SoftwareVersion);
					else
						strcpy(value,"");
					break;
				case BMS_DOWNLOAD_HARDWAREVERSION:
					if(pBMSNode->CTCOM_Download.HardwareVersion)
						strcpy(value,pBMSNode->CTCOM_Download.HardwareVersion);
					else
						strcpy(value,"");
					break;
				case BMS_DOWNLOAD_TIMEWINDOW:
					if(pBMSNode->CTCOM_Download.TimeWindow)
						strcpy(value,pBMSNode->CTCOM_Download.TimeWindow);
					else
						strcpy(value,"");
					break;
				case BMS_DOWNLOAD_USERNAME:					
					if(pBMSNode->CTCOM_Download.Username)
						strcpy(value,pBMSNode->CTCOM_Download.Username);
					else
						strcpy(value,"");
					break;
				case BMS_DOWNLOAD_PASSWORD:
					if(pBMSNode->CTCOM_Download.Password)
						strcpy(value,pBMSNode->CTCOM_Download.Password);
					else
						strcpy(value,"");
					break;
				case BMS_DOWNLOAD_STATUS:
					sprintf(value,"%lu",pBMSNode->CTCOM_Download.DownloadStatus);
					break;
				case BMS_DOWNLOAD_OPERETIONID:
					sprintf(value,"%lu",pBMSNode->CTCOM_Download.OperationID);
					break;
				default:
					goto error;
			}
			break;
		case BMS_CTCOM_DOWNLOAD_STATUS_INDEX:
			if(bmsNameIndex2 == BMS_INVOKE){
				if(pBMSNode->CTCOM_GetDownloadStatus.InvokeAction)
						strcpy(value,"1");
					else				
						strcpy(value,"0");
					break;
			}else if(bmsNameIndex2 == BMS_DOWNLOADSTATUS_OPERATIONSTATUS){
				strcpy(value,pBMSNode->CTCOM_GetDownloadStatus.OperationStatus);
			}else{
				goto error;
			}
			break;
		default:
			break;
	}

	//pthread_mutex_unlock(&pDeviceList->upnpLock);
	return 0;
error:
	tcdbg_printf("%s:bmsNameIndex1=%d, bmsNameIndex2=%d\n",__FUNCTION__,bmsNameIndex1, bmsNameIndex2);
	//pthread_mutex_unlock(&pDeviceList->upnpLock);
	return -1;
}


int setBMSValueByIndex(int index, int bmsNameIndex1, int bmsNameIndex2, void* value){
	pBMS_Node pBMSNode = NULL;
	int len = 0;

	if(pDeviceList == NULL){
		tcdbg_printf("%s :pDeviceList == NULL\n",__FUNCTION__);
		return NULL;
	}

	pthread_mutex_lock(&pDeviceList->upnpLock);
	pBMSNode = getBMSNodeByIndex(index);
	if(pBMSNode == NULL){
		tcdbg_printf("%s: pBMSNode == NULL",__FUNCTION__);
		goto error;
	}

	switch(bmsNameIndex1){
		case BMS_REBOOT_INDEX:
			if(bmsNameIndex2 ==BMS_INVOKE){
				pBMSNode->Reboot.InvokeAction = *(boolean *)value;
			}else{
				goto error;
			}
			break;
		case BMS_BASELINE_RESET_INDEX:
			if(bmsNameIndex2 ==BMS_INVOKE){
				pBMSNode->BaselineReset.InvokeAction = *(boolean *)value;
			}else{
				goto error;
			}
			break;
		case BMS_DEVICE_STATUS_INDEX:
			if(bmsNameIndex2 ==BMS_INVOKE){
				pBMSNode->GetDeviceStatus.InvokeAction = *(boolean *)value;
			}else{
				goto error;
			}
			break;
		case BMS_CTCOM_DOWNLOAD_INDEX:
			switch(bmsNameIndex2){
				case BMS_INVOKE:
					pBMSNode->CTCOM_Download.InvokeAction = *(boolean *)value;
					break;
				case BMS_DOWNLOAD_CHANNEL:
					pBMSNode->CTCOM_Download.Channel = *(int *)value;
					break;
				case BMS_DOWNLOAD_FILETYPE:				
					len = strlen((char *)value);
					if(len > 64){
						tcdbg_printf("%s:len is more than 64\n",__FUNCTION__);
						goto error;
					}
					pBMSNode->CTCOM_Download.FileType = malloc(len);
					memset(pBMSNode->CTCOM_Download.FileType,0,len);
					strcpy(pBMSNode->CTCOM_Download.FileType,(char *)value);
					break;
				case BMS_DOWNLOAD_URL:				
					len = strlen((char *)value);
					if(len > 256){
						tcdbg_printf("%s:len is more than 256\n",__FUNCTION__);
						goto error;
					}
					pBMSNode->CTCOM_Download.URL = malloc(len);
					memset(pBMSNode->CTCOM_Download.URL,0,len);
					strcpy(pBMSNode->CTCOM_Download.URL,(char *)value);
					break;
				case BMS_DOWNLOAD_SOFTWAREVERSION:
					len = strlen((char *)value);
					if(len > 64){
						tcdbg_printf("%s:len is more than 64\n",__FUNCTION__);
						goto error;
					}
					pBMSNode->CTCOM_Download.SoftwareVersion = malloc(len);
					memset(pBMSNode->CTCOM_Download.SoftwareVersion,0,len);
					strcpy(pBMSNode->CTCOM_Download.SoftwareVersion,(char *)value);
					break;
				case BMS_DOWNLOAD_HARDWAREVERSION:
					len = strlen((char *)value);
					if(len > 64){
						tcdbg_printf("%s:len is more than 64\n",__FUNCTION__);
						goto error;
					}
					pBMSNode->CTCOM_Download.HardwareVersion = malloc(len);
					memset(pBMSNode->CTCOM_Download.HardwareVersion,0,len);
					strcpy(pBMSNode->CTCOM_Download.HardwareVersion,(char *)value);
					break;
				case BMS_DOWNLOAD_TIMEWINDOW:
					len = strlen((char *)value);
					if(len > 1024){
						tcdbg_printf("%s:len is more than 1024\n",__FUNCTION__);
						goto error;
					}
					pBMSNode->CTCOM_Download.TimeWindow = malloc(len);
					memset(pBMSNode->CTCOM_Download.TimeWindow,0,len);
					strcpy(pBMSNode->CTCOM_Download.TimeWindow,(char *)value);
					break;
				case BMS_DOWNLOAD_USERNAME:
					len = strlen((char *)value);
					if(len > 256){
						tcdbg_printf("%s:len is more than 256\n",__FUNCTION__);
						goto error;
					}
					pBMSNode->CTCOM_Download.Username = malloc(len);
					strcpy(pBMSNode->CTCOM_Download.Username,(char *)value);
					break;
				case BMS_DOWNLOAD_PASSWORD:
					len = strlen((char *)value);
					if(len > 256){
						tcdbg_printf("%s:len is more than 256\n",__FUNCTION__);
						goto error;
					}
					pBMSNode->CTCOM_Download.Password = malloc(len);
					memset(pBMSNode->CTCOM_Download.Password,0,len);
					strcpy(pBMSNode->CTCOM_Download.Password,(char *)value);
					break;
				default:
					tcdbg_printf("%s:default case!\n",__FUNCTION__);
					goto error;
			}
			break;
		case BMS_CTCOM_DOWNLOAD_STATUS_INDEX:
			if(bmsNameIndex2 ==BMS_INVOKE){
				pBMSNode->CTCOM_GetDownloadStatus.InvokeAction = *(boolean *)value;
			}else{
				goto error;
			}
			break;
		default:
			break;
	}
	
	pthread_mutex_unlock(&pDeviceList->upnpLock);
	return 0;
error:
	
	tcdbg_printf("%s:set fail\n",__FUNCTION__);
	pthread_mutex_unlock(&pDeviceList->upnpLock);
	return -1;
}

int getValueOfXMLAttr(mxml_node_t* node,char* attrName, char* attrValue){
	int i;
	if(node == NULL){
		tcdbg_printf("%s: node not exist\n",__FUNCTION__);
		return -1;
	}

	for(i = 0; i < node->value.element.num_attrs; i++){
		if(node->value.element.attrs[i].name && strcmp(node->value.element.attrs[i].name,attrName) == 0){
			if(node->value.element.attrs[i].value)
				strcpy(attrValue,node->value.element.attrs[i].value);
			return 0;
		}
	}

	return -1;	
}

DeviceNodePtr findChildNodeByName(DeviceNodePtr pParentNode,char* name){
	DeviceNodePtr node;
	if(pParentNode == NULL){
		tcdbg_printf("%s: pParentNode == NULL\n",__FUNCTION__); 
		return NULL;
	}

	node = pParentNode->firstChild;
	while(node && node->name){
		if(strcmp(node->name,name) == 0)			//node is already exist
		{		
			return node;
		}
		node = node->nextSibling;
	}

	return NULL;
}
DeviceNodePtr RegisterDynmicNodeByXMLNode(DeviceNodePtr pDevNode, mxml_node_t *xmlNode, char* name, int haveChild){
	mxml_node_t *parameterNode = NULL;
	mxml_node_t *pNode = NULL;
	char attrName[64] = {0};
	char attrValue[64] = {0};
	char elementName[64] = {0};
	char syntax[64] = {0};
	int attrNum = 0;
	int i;
	int access = 0;	
	int type = 0;
	DeviceNodePtr pParentNode = NULL;
	DeviceNodePtr pChildNode = NULL;
	DeviceNodePtr ptr69Node = NULL;
	char nodeValue[256] = {0};
	char dynamicNodeName[256] = {0};
	
	if(xmlNode == NULL || pDevNode == NULL){
		tcdbg_printf("%s: node is NULL\n",__FUNCTION__); 
		return NULL;
	}

	attrNum = xmlNode->value.element.num_attrs;
	for(i = 0; i < attrNum; i++){
		strcpy(attrName,xmlNode->value.element.attrs[i].name);
		strcpy(attrValue,xmlNode->value.element.attrs[i].value);
		if(strcmp(attrName,"access") == 0){
			if(strcmp(attrValue,"readOnly") == 0){
				access = 0;
			}else if(strcmp(attrValue,"readWrite") == 0){
				access = 1;
			}else{
				tcdbg_printf("%s:name=%s, attrValue = %s; acces is wrong\n",__FUNCTION__,name,attrValue); 
				access -1;
			}
			break;
		}		
	}

	strcpy(elementName,xmlNode->value.element.name);
	if(strcmp(elementName,"object") == 0){	
		strcpy(dynamicNodeName,name);
		UPnPDMDynamicNode[0].name = dynamicNodeName;	
		if(access == 1){
			UPnPDMDynamicNode[0].type = ObjectW;
			if(haveChild)
				UPnPDMDynamicNode[0].func->addObject = DeviceNodeAddUPnPDMDynamicObject;
			else
				UPnPDMDynamicNode[0].func->deleteObject = DeviceNodeDelUPnPDMDynamicObject;
		}
		else{	
			UPnPDMDynamicNode[0].type = ObjectR;
		}
		UPnPDMDynamicNode[0].attrFlag= NoLimite;
		ptr69Node = RegisterNodeFromTable(pDevNode, UPnPDMDynamicNode);
		if(ptr69Node == NULL) {
			tcdbg_printf("%s: RegisterNodeFromTable error!name=%s\n", __FUNCTION__,name);
			return NULL;
		}

		if(!haveChild){		//only object node have no object child need register paremeter node
			pParentNode = findChildNodeByName(ptr69Node,name);
			if(pParentNode == NULL){
				tcdbg_printf("%s: findChildNodeByName error!name=%s\n", __FUNCTION__,name);
				return NULL;
			}
			for(parameterNode = mxmlFindElement(xmlNode, xmlNode, "parameter",
					NULL, NULL, MXML_DESCEND);
					parameterNode !=NULL;
					parameterNode = mxmlFindElement(parameterNode, xmlNode, "parameter",
						NULL, NULL, MXML_NO_DESCEND)){
						
				getValueOfXMLAttr(parameterNode,"name",nodeValue); 	
				
				if(pChildNode = findChildNodeByName(pParentNode,nodeValue)){				
					continue;
				}else{
					pParentNode = RegisterDynmicNodeByXMLNode(pParentNode,parameterNode,nodeValue,0);
					if( pParentNode == NULL) {
						tcdbg_printf("%s: RegisterDynmicNodeByXMLNode error!nodeValue=%s\n", __FUNCTION__,nodeValue);
						return NULL;
					}
				}
			}		
		}
	}else if(strcmp(elementName,"parameter") == 0){
		pNode = mxmlFindElement(xmlNode, xmlNode, "syntax",
							NULL, NULL, MXML_DESCEND);
		if(pNode && pNode->child){
			strcpy(syntax,pNode->child->value.text.string);
			
			if(strncmp(syntax,"unsignedInt",strlen("unsignedInt")) == 0){
				if(access == 1)
					type  = UnsignIntW;
				else
					type  = UnsignIntR;
			}else if(strncmp(syntax,"Int",strlen("Int")) == 0){
				if(access == 1)
					type  = IntW;
				else
					type = IntR;
			}else  if(strncmp(syntax,"boolean",strlen("boolean")) == 0){
				if(access == 1)
					type = BooleanW;
				else
					type = BooleanR;
			}else if(strncmp(syntax,"dateTime",strlen("dateTime")) == 0){
				if(access == 1)
					type = DateTimeTypeW;
				else
					type = DateTimeTypeR;
			}else if(strncmp(syntax,"string",strlen("string")) == 0){
				if(access == 1)
					type = StrW;
				else
					type = StrR;
			}else{
				tcdbg_printf("%s: syntax=%s  error!name=%s\n", __FUNCTION__,syntax,name);			
			}
		}
		else{
			tcdbg_printf("%s: get syntax node  error!name=%s\n", __FUNCTION__,name);			
			return NULL;
		}

		strcpy(dynamicNodeName,name);
		UPnPDMDynamicNode[0].name = dynamicNodeName;	
		UPnPDMDynamicNode[0].type = type;
		UPnPDMDynamicNode[0].attrFlag= NoLimite;		
		UPnPDMDynamicNode[0].func->getValueInfo = getDynamicNodeValue;
		if(access == 1)
			UPnPDMDynamicNode[0].func->setValueInfo = setDynamicNodeValue;

		UPnPDMDynamicNode[0].func->getAttribute = getUPnPDMNodeValueAttribute;
		UPnPDMDynamicNode[0].func->setAttribute = setUPnPDMNodeValueAttribute;

		ptr69Node = RegisterNodeFromTable(pDevNode, UPnPDMDynamicNode);
		if(ptr69Node == NULL) {
			tcdbg_printf("%s: RegisterNodeFromTable error!name=%s\n", __FUNCTION__,name);
			return NULL;
		}
	}else{
		tcdbg_printf("%s: error element!!!!\n", __FUNCTION__);
		return NULL;
	}

	return ptr69Node;
}

int RegisterObjectNodeByName(DeviceNodePtr pDevNode,mxml_node_t *objectNode, char* name){
	char nodeValue[256] = {0};
	DeviceNodePtr pChildNode = NULL;
	DeviceNodePtr pParentNode = NULL;
	char* p = NULL;
	char* tmp = NULL;
	char subName[256] = {0};		
	int instanceList[8] = {0};
	int i = 0;
	int index = -1;
	int devNum = -1;
	u_buf* upbuffer = NULL;
	char *retBuf = NULL;
	char upnpNodeName[256] = {0};
	int num = 0;

	if(pDevNode == NULL){
		tcdbg_printf("%s: pDevNode is NULL\n",__FUNCTION__); 
		return -1;
	}

	strcpy(subName,name);
	tmp = name;
	p = strtok(subName,"/");
	pParentNode = pDevNode;
	while(p){		
		strcpy(nodeValue,p);
		tmp = strstr(tmp,nodeValue);
		p = strtok(NULL,"/");
		if(strcmp(nodeValue,"#")){			// not "#" node
			if(pChildNode = findChildNodeByName(pParentNode,nodeValue)){				
				pParentNode = pChildNode;				
			}else{
				if(p){
					//pParentNode  = RegisterDynmicObjectNodeByName(pParentNode,nodeValue);
					pParentNode = RegisterDynmicNodeByXMLNode(pParentNode,objectNode,nodeValue,1);		
					pParentNode = findChildNodeByName(pParentNode,nodeValue);
				}
				else{
					pParentNode = RegisterDynmicNodeByXMLNode(pParentNode,objectNode,nodeValue,0);		
					if(pParentNode == NULL){
						tcdbg_printf("%s:name=%s register fail!!!\n",__FUNCTION__,name); 
						return -1;
					}else{
						//tcdbg_printf("%s:name=%s register success!!!\n",__FUNCTION__,name); 
						return 0;
					}
				}				
			}
		}else{
#if 0
			if(!strcmp(pParentNode->name,"AssociatedDevice")){
				tcdbg_printf("%s:name=%s return 0!!!\n",__FUNCTION__,pParentNode->name); 
				return 0;
			}
#endif

			index = getDeviceListIndexByDevNode(pParentNode);
			devNum = getUPnPInstanceByTr69Index(index);
			getUPnPDMNodeByDevNode(pParentNode,upnpNodeName);
			strcat(upnpNodeName,"/");			
			add_to_ubuf(&upbuffer, OP_CMS_GETINSTANCES, devNum, upnpNodeName, "0");

			if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
				//tcdbg_printf("%s :upbuffer->buf=%s \n",__FUNCTION__,upbuffer->buf); 
				//need parse the retBuf
				if(!parseActionResponse(retBuf,OP_CMS_GETINSTANCES_RES,devNum)){
					getDevInstance(retBuf,",",&num,instanceList);
				}
				free(retBuf);
			}else{
				tcdbg_printf("%s :send_u_bufwithRes error!!!\n",__FUNCTION__);  
				//return -1;
			}
			free_ubuf(&upbuffer);
			i = 0;
			while(instanceList[i]){
				sprintf(subName,"%d%s",instanceList[i],tmp+1);
				if(RegisterObjectNodeByName(pParentNode,objectNode, subName)){
					tcdbg_printf("%s:subName=%s register fail!!!\n",__FUNCTION__,subName); 
					return -1;
				}
				i++;
			}
			return 0;
		}
	}

	return 0;
}

int InitDynamicUPnPDMNodeByPath(DeviceNodePtr pDevNode, char* filePath){
	FILE *fp = NULL;
	mxml_node_t *tree = NULL;
	mxml_node_t	*objectNode = NULL;
	char attrValue[NODE_NAME_LEN] = {0};

	if(pDevNode == NULL){
		tcdbg_printf("%s: pDevNode is NULL\n",__FUNCTION__); 
		return -1;
	}

	fp = fopen(filePath, "r");
	if(fp == NULL){
		tcdbg_printf("%s: open file fail\n",__FUNCTION__);
		return -1;
	}
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);	
	fclose(fp);

	//tcdbg_printf("%s: pDevNode->name=%s\n",__FUNCTION__,pDevNode->name); 
	for(objectNode = mxmlFindElement(tree, tree, "object",
				NULL, NULL, MXML_DESCEND); 
		objectNode != NULL; 
		objectNode = mxmlFindElement(objectNode, tree, "object",
					NULL, NULL, MXML_NO_DESCEND)){			//object node
		memset(attrValue,0,NODE_NAME_LEN);
		getValueOfXMLAttr(objectNode,"name",attrValue);	
		RegisterObjectNodeByName(pDevNode,objectNode,attrValue);
	}
		
	if(tree != NULL){
		mxmlDelete(tree);
		tree = NULL;
	}
	return 0;
}

int InitDynamicUPnPDMNode(int index, DeviceNodePtr  deviceNode){
	char filePath[256];
	char tr69NodeName[256] = {0};
	DeviceNodePtr DeviceListNode = NULL;
	int	devnum = 0;
	int i= 0;
	FILE *fp = NULL;
	char buf[32] = {0};
	int num = 0;

	if(index <= 0){
		tcdbg_printf("%s : index = %d,error!\n",__FUNCTION__,index);  
		return -1;
	}

	sprintf(tr69NodeName,"%d",index);
	DeviceListNode = findChildNodeByName(deviceNode,tr69NodeName);
	if(DeviceListNode == NULL){
		tcdbg_printf("%s:search node fail\n",__FUNCTION__);
		return -1;
	}

	devnum = getUPnPInstanceByTr69Index(index);
	if(devnum <= 0){
		tcdbg_printf("%s : devnum = %d error!\n",__FUNCTION__, devnum);  
		return -1;
	}

#if 0
	//Init DeviceInfo
	sprintf(filePath,"/userfs/UPnPDM/%d/DeviceInfo_Template.xml",devnum);
	if(InitDynamicUPnPDMNodeByPath(DeviceListNode,filePath)){
		tcdbg_printf("%s : Init DeviceInfo fail!devnum=%d\n",__FUNCTION__,devnum);  
		return -1;
	}
	
	//Init ServerObject
	sprintf(filePath,"/userfs/UPnPDM/%d/ServerObject_Template.xml",devnum);
	if(InitDynamicUPnPDMNodeByPath(DeviceListNode,filePath)){		
		tcdbg_printf("%s : Init ServerObject fail!devnum=%d\n",__FUNCTION__,devnum);  
		return -1;
	}
#else
	memset(filePath,0,256);
	sprintf(filePath,"/tmp/dev%02x/f0.xml",devnum);
	fp = fopen(filePath, "r");
	if(fp == NULL){
		tcdbg_printf("%s :fopen fail!!! filePath = %s!\n",__FUNCTION__, filePath);  
		return -1;
	}else{
		fgets(buf, sizeof(buf), fp);
		num = atoi(buf);
	}
	fclose(fp);
	
	i = 0;
	while(i<num){
		memset(filePath,0,256);
		sprintf(filePath,"/tmp/dev%02x/%02x.xml",devnum,i+0x10);
		InitDynamicUPnPDMNodeByPath(DeviceListNode,filePath);
		i++;
	}
#endif

	return 0;
}

int appendDevNode(int index,int devnum){
	pDeviceList_Header pDeviceList_Header= NULL;
	pBMS_Node pBMSNode = NULL;	
	pDev_Node pDevNode = NULL;
	pDev_Node pNode= NULL;
	pDev_Node pNode_pre= NULL;

	if(index <= 0){
		tcdbg_printf("%s : index begin from 1!\n",__FUNCTION__);  
		return -1;
	}

	if(pDeviceList == NULL){	//the first node, need alloc DeviceList
		if ((pDeviceList_Header = os_malloc(sizeof(DeviceList_Header))) == NULL){
		   tcdbg_printf("%s:malloc DeviceList_Header fail!\n",__FUNCTION__);  
		   return -1; 	   
		}  
		memset(pDeviceList_Header, 0, sizeof(DeviceList_Header));	
		pDeviceList_Header->num = 0;
		pDeviceList_Header->pDevNode = NULL;
		pDeviceList = pDeviceList_Header;
		if(pthread_mutex_init(&pDeviceList->upnpLock,NULL) != 0){		//init mutex lock
			tcdbg_printf("%s:malloc DeviceList_Header pthread_mutex_int fail!\n",__FUNCTION__);	
			os_free(pDeviceList_Header);
			pDeviceList = NULL;
			return -1;			
		}
	}

	//alloc Dev node
	if ((pDevNode = os_malloc(sizeof(Dev_Node))) == NULL){			
	   tcdbg_printf("%s:malloc Dev_Node fail!\n",__FUNCTION__); 
	   return -1;
	}  
	memset(pDevNode, 0, sizeof(Dev_Node));
	pDevNode->index = index;
	pDevNode->next = NULL;	

	//alloc BMS node
	if ((pBMSNode = os_malloc(sizeof(BMS_Node))) == NULL){			
	   tcdbg_printf("%s:malloc BMS_Node fail!\n",__FUNCTION__); 
	   freeDevNode(pDevNode);  
	   pDevNode = NULL;
	   return -1;
	}  
	memset(pBMSNode, 0, sizeof(BMS_Node));
	pDevNode->pBMS_Node= pBMSNode;

	//insert node
	pthread_mutex_lock(&pDeviceList->upnpLock);
	if(pDeviceList->pDevNode == NULL || pDeviceList->pDevNode->index > index){			//insert the first node
		pDevNode->next = pDeviceList->pDevNode;
		pDeviceList->pDevNode = pDevNode;
		pDeviceList->num += 1;
		tcdbg_printf("%s:append the first Dev_Node success!\n",__FUNCTION__); 
		goto success;
	}
	
	pNode_pre = pDeviceList->pDevNode;
	pNode = pNode_pre->next;
	while(pNode){
		if(pNode->index > index){	//insert in the middle
			tcdbg_printf("%s : insert in the middle!\n",__FUNCTION__);  
			break;
		}
		
		pNode_pre = pNode;
		pNode=pNode->next;
	}
	
	pDevNode->next = pNode;	
	pNode_pre->next = pDevNode;	
	pDeviceList->num += 1;
	tcdbg_printf("%s : insert at the end!\n",__FUNCTION__); 

success:
	pDevNode->upnpInstance = devnum;
	pthread_mutex_unlock(&pDeviceList->upnpLock);
	return 0;
}

int AddUPnPDMDeviceListObject(uint32 devnum,  DeviceNodePtr  deviceNode){
	int index = -1;
	int rtnCode = 0;
	char node_index[4];
	DeviceNodePtr devNode = NULL;
	DeviceNodePtr pChildNode = NULL;
	pDev_Node pDevNode = NULL;

	if(deviceNode == NULL){
		tcdbg_printf("%s: deviceNode == NULL!!!\n", __FUNCTION__);
		return -1;
	}

	if(devnum < 0){
		tcdbg_printf("%s: error!!! devnum<0!\n", __FUNCTION__);
		return -1;
	}
	
	index = selectCurUnusedIndex();
	sprintf(node_index, "%d", index);

	UPnPDMDeviceListIndex[0].name = node_index;	
	if ((devNode = RegisterNodeFromTable( deviceNode, UPnPDMDeviceListIndex)) == NULL ) {
		tcdbg_printf("%s:RegisterNodeFromTable fail.\n", __FUNCTION__);
		return -1;
	}
	
	if(appendDevNode(index,devnum)){
		tcdbg_printf("%s: appendDevNode(%d) fail.\n", __FUNCTION__,index);
		return -1;
	}

	if(InitDynamicUPnPDMNode(index,deviceNode)){
		tcdbg_printf("%s: InitDynamicUPnPDMNode fail!index=%d\n", __FUNCTION__,index);
		pDevNode = getDevNodeByIndex(index);
		freeDevNode(pDevNode);
		pDevNode = NULL;
		return -1;
	}
	return 0;
}

int delDevNode(int index){	
	pDev_Node pNode= NULL;
	pDev_Node pNode_pre= NULL;
	
	if(index <= 0){
		tcdbg_printf("%s : index begin from 1!\n",__FUNCTION__);  
		return -1;
	}

	if(pDeviceList == NULL){
		tcdbg_printf("%s : pDeviceList == NULL\n",__FUNCTION__); 
		return -1;		
	}
	
	pthread_mutex_lock(&pDeviceList->upnpLock);
	if(pDeviceList->pDevNode == NULL){
		tcdbg_printf("%s :no Dev node exist!\n",__FUNCTION__); 
		goto fail;
	}
	
	pNode = pDeviceList->pDevNode;
	if(pNode->index == index){
		tcdbg_printf("%s : find the first node,del it!\n",__FUNCTION__); 
		pDeviceList->pDevNode = pNode->next;	
		goto success;
	}
	
	pNode_pre = pNode;
	pNode = pNode_pre->next;
	while(pNode){
		if(pNode->index == index){
			tcdbg_printf("%s : find the node,del it!index=%d\n",__FUNCTION__,index); 
			pNode_pre->next= pNode->next;		
			goto success;
		}		
		pNode_pre = pNode;
		pNode = pNode->next;
	}

fail:
	tcdbg_printf("%s : not find the node!index=%d\n",__FUNCTION__,index); 	
	pthread_mutex_unlock(&pDeviceList->upnpLock);
	return -1;

success:
	freeDevNode(pNode);
	pNode= NULL;
	pDeviceList->num -= 1;
	pthread_mutex_unlock(&pDeviceList->upnpLock);
	return 0;	
}

int DelUPnPDMDeviceListObject(uint32 index, DeviceNodePtr  deviceNode){
	int rtnCode = 0;
	DeviceNodePtr  devNode = NULL;	
	DeviceNodePtr  devChildNode = NULL;
	char strIndex[32] = {0};

	if(deviceNode == NULL){
		tcdbg_printf("%s: deviceNode == NULL!!!\n", __FUNCTION__);
		return -1;
	}
	
	//del dev node
	if(	delDevNode(index) < 0){
		tcdbg_printf("DelUPnPDMDeviceListObject:do delBMSNode fail,index=%d\n",index);
		return -1;
	}
	
	//free node before init
	sprintf(strIndex,"%d",index);
	devChildNode = findChildNodeByName(deviceNode,strIndex);
	cwmpFreeDeviceTable(devChildNode);
	cwmpNodeFree(devChildNode);
	devChildNode = NULL;
	return rtnCode;
}

int AddUPnPActionList(int actType,int devnum){	
	pUPnPAction pUPnPAct = NULL;
	
	//malloc header & upnpActLock
	if(pUPnPAct_Header == NULL){			
		if((pUPnPAct_Header = os_malloc(sizeof(UPnPAction_Header))) == NULL){
			tcdbg_printf("%s:malloc UPnPAction_Header fail!\n",__FUNCTION__);  
			return -1;		
		}	
		memset(pUPnPAct_Header, 0, sizeof(UPnPAction_Header));	
		if(pthread_mutex_init(&pUPnPAct_Header->upnpActLock,NULL) != 0){		//init mutex lock
			tcdbg_printf("%s:malloc pUPnPAct_Header pthread_mutex_int fail!\n",__FUNCTION__);	
			os_free(pUPnPAct_Header);
			pUPnPAct_Header = NULL;
			return -1;			
		}
	}
	
	if((pUPnPAct = os_malloc(sizeof(UPnPAction))) == NULL){
		tcdbg_printf("%s:malloc UPnPAction fail!\n",__FUNCTION__);	
		return -1;		
	}
	memset(pUPnPAct, 0, sizeof(UPnPAction));	
	pUPnPAct->actType = actType;
	pUPnPAct->devNum = devnum;
	
	pthread_mutex_lock(&pUPnPAct_Header->upnpActLock);
	pUPnPAct->next = pUPnPAct_Header->pUPnPAction;
	pUPnPAct_Header->pUPnPAction = pUPnPAct;
	pUPnPAct_Header->num++;
	pthread_mutex_unlock(&pUPnPAct_Header->upnpActLock);
	
	iUPnPDMModifyFlag = 1;

	return 0;
}

int ProcessUPnPAddDelDevAction(int popcode,int devnum){
	char *objectName = "InternetGatewayDevice.X_CT-COM_ProxyDevice.DeviceList.";
	DeviceNodePtr DeviceListNode = NULL;
	DeviceNodePtr rootptr = NULL;
	int ret = 0;
	int index = 0;

	pthread_mutex_lock(&value_change_lock);
	if(inform_task_start == 1){
		/* if inform task is running, quit and do nothing */
//			signal_valueChg_flag = 1; //spSave() will call this task again, no need set flag to re-do it.
		pthread_mutex_unlock(&value_change_lock);		
		tcdbg_printf("%s:inform is performing!\n",__FUNCTION__);	
		AddUPnPActionList(popcode,devnum);
		return -1;
	}else{
		rootptr = rpc.IGDDeviceRootPtr;
		rpc.IGDDeviceRootPtr = NULL;
		pthread_mutex_unlock(&value_change_lock);
	}
	
	cwmpSearchDeviceNodeByName(objectName, rootptr, &DeviceListNode);
	if(popcode == OP_ADD_DEVICE){
		ret = AddUPnPDMDeviceListObject(devnum,DeviceListNode);
	}else{
		index = getIndexByUPnPInstance(devnum);
		ret = DelUPnPDMDeviceListObject(index,DeviceListNode);
	}

	pthread_mutex_lock(&value_change_lock);
	rpc.IGDDeviceRootPtr = rootptr; 
	pthread_mutex_unlock(&value_change_lock);

	if(ret < 0){
		tcdbg_printf("%s:popcode=%d fail!devnum=%d\n",__FUNCTION__,popcode,devnum);
		AddUPnPActionList(popcode,devnum);
	}

	return ret;
}

int ProcessOperationDone(char* buf,int devnum){
	int index = 0;
	char* operationStatus = NULL;
	pBMS_Node pBMSNode = NULL;
	
	index = getIndexByUPnPInstance(devnum);
	if(index < 0){
		tcdbg_printf("%s: index=%d, error!!!\n",__FUNCTION__,index);
		return -1;
	}
	
	tcdbg_printf("%s:OP_OPRATION_DONE==============>!!!\n",__FUNCTION__);
	if(getValueByName(buf,"OperationStatus",&operationStatus,NULL) == 0) {
		if(strlen(operationStatus) == 0){
			tcdbg_printf("%s:OP_OPRATION_DONE, OperationStatus is NULL, no need send value chang inform\n",__FUNCTION__);
		}
		else{
			pBMSNode = getBMSNodeByIndex(index);
			if(pBMSNode == NULL){						
				tcdbg_printf("%s:OP_OPRATION_DONE pBMSNode == NULL!!!\n",__FUNCTION__);
				os_free(operationStatus);
				return -1;
			}
			memcpy(pBMSNode->CTCOM_GetDownloadStatus.OperationStatus,operationStatus,sizeof(operationStatus));
			valueModify_Nofity = 1; 	
			setUPnPDownloadInstance(index);
			setUPnPDMDownloadFlag(1);
			sem_post(&SEM_IMFORM);	
		}
		os_free(operationStatus);
		operationStatus = NULL;
	}else{
		tcdbg_printf("%s:getValueByName of	OperationStatus fail\n",__FUNCTION__);
		return -1;
	}

	return 0;
}

int ProcessUPnPDMMessage(char* buf){
	int index = 0;
	int devnum, popcode;
	unsigned int length;
	char* operationStatus = NULL;
	pBMS_Node pBMSNode = NULL;
	int ret = 0;
	pUPnPAction pUPnPAct = NULL;


	parse_u_buf(buf, &devnum, &popcode, &length);				
	tcdbg_printf("%s:buf=%s\n",__FUNCTION__,buf);
	tcdbg_printf("%s, devnum=%d, &popcode=%d, length=%d\n",__FUNCTION__, devnum, popcode, length);

	switch (popcode) {
		case OP_ADD_DEVICE:
		case OP_DEL_DEVICE:
			ret = ProcessUPnPAddDelDevAction(popcode,devnum);
			break;
		case OP_OPRATION_DONE:			
			ret = ProcessOperationDone(buf,devnum);
			break;
		case OP_BMS_DOWNLOAD_RES:			
			ret = parseActionResponse(buf,OP_BMS_GETDOWNLOADSTATUS_RES,devnum);
		default:
			tcdbg_printf("%s:popcode=%d,not support!!!\n",__FUNCTION__,popcode);
			break;
	}
	return ret;
}

int parseActionResponse( char* buffer, unsigned char opcode, int upnpInstance){
	char *buf = NULL;
	int len = 0;
	int devnum, popcode;
	unsigned int length;
	pBMS_Node pBMSNode = NULL;
	char* p;
	int index;
	char* paravalue = NULL;	
	char *tmpValue = NULL;
	
	buf = buffer;	
	parse_u_buf(buf, &devnum, &popcode, &length);

	
	tcdbg_printf("%s:buf=%s, \n",__FUNCTION__,buf);
	tcdbg_printf("%s:devnum=%d, &popcode=%d, length=%d\n",__FUNCTION__,devnum, popcode, length);
	if((popcode != opcode) || (devnum != upnpInstance)){
		tcdbg_printf("%s:opcode or devnum error!popcode=%d,devnum=%d\n",__FUNCTION__,popcode,devnum);
		return -1;
	}
	
	if(getValueByName(buf,"ErrCode",&paravalue,NULL) == 0){		//content error code
		//tcdbg_printf("%s:paravalue=%s\n",__FUNCTION__,paravalue);			
		upnpDMErrCode = atoi(paravalue);
		os_free(paravalue);
		paravalue = NULL;
		if(upnpDMErrCode){
			tcdbg_printf("%s:upnpDMErrCode=%d\n",__FUNCTION__,upnpDMErrCode);	
			return -1;
		}
	}

	index = getIndexByUPnPInstance(devnum);
	pBMSNode = getBMSNodeByIndex(index);
	switch(opcode){
		case OP_BMS_REBOOT_RES:		
			if(getValueByName(buf,"RebootStatus",&paravalue,NULL) == 0){
				strcpy(pBMSNode->Reboot.RebootStatus, paravalue);
				goto end;
			}			
			break;
		case OP_BMS_GETDEVICESTATUS_RES:	
			if(getValueByName(buf,"DeviceStatus",&paravalue,",") == 0){
				strcpy(pBMSNode->GetDeviceStatus.DeviceStaus, paravalue);				
				goto end;
			}	
			break;
		case OP_BMS_DOWNLOAD_RES:	
			if(getValueByName(buf,"DownloadStatus",&paravalue,NULL) == 0){
				pBMSNode->CTCOM_Download.DownloadStatus = atoi(paravalue);
				goto end;
			}	

			if(getValueByName(buf,"OperationID",&paravalue,NULL) == 0){
				pBMSNode->CTCOM_Download.OperationID = atoi(paravalue);
				goto end;
			}
			break;
		case OP_BMS_GETDOWNLOADSTATUS_RES:	
			if(getValueByName(buf,"OperationStatus",&paravalue,",") == 0){
				strcpy(pBMSNode->CTCOM_GetDownloadStatus.OperationStatus, paravalue);
				goto end;
			}	
			break;
		case OP_BMS_BASELINERESET_RES:	
		default:
			tcdbg_printf("%s:default case\n",__FUNCTION__,opcode);			
			break;
	}

end:
	if(paravalue){
		os_free(paravalue);
		paravalue = NULL;
	}
	return 0;
}

int	processBMSAction(){
	pDev_Node pNode = NULL;
	pBMS_Node pBMSNode = NULL;
	int upnpInstance;
	char temp[32] = {0};	
	u_buf* upbuffer = NULL;
	char *retBuf = NULL;

	if(pDeviceList == NULL){
		//tcdbg_printf("%s:pDeviceList == NULL\n",__FUNCTION__);
		return -1;
	}
	
	pthread_mutex_lock(&pDeviceList->upnpLock);
	if(pDeviceList->pDevNode == NULL){
		tcdbg_printf("%s:No Device node exist\n",__FUNCTION__);
		pthread_mutex_unlock(&pDeviceList->upnpLock);
		return -1;
	}

	pNode = pDeviceList->pDevNode;
	while(pNode){
		pBMSNode = pNode->pBMS_Node;
		if(pBMSNode == NULL){
			continue;
		}
		upnpInstance = pNode->upnpInstance;
		if(pBMSNode->Reboot.InvokeAction == 1){		
			add_to_ubuf(&upbuffer, OP_BMS_REBOOT, upnpInstance, NULL,NULL);
			tcdbg_printf("upbuffer=%s\n",upbuffer->buf);
			if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
				
				tcdbg_printf("processBMSAction : retBuf=%s\n",retBuf);
				parseActionResponse(retBuf,OP_BMS_REBOOT_RES,upnpInstance);
				pBMSNode->Reboot.InvokeAction = 0;
				free(retBuf);
			}else{
				tcdbg_printf("processBMSAction : OP_BMS_REBOOT_RES fail\n");
			}
			free_ubuf(&upbuffer);
		}

		if(pBMSNode->BaselineReset.InvokeAction == 1){					
			upbuffer = add_to_ubuf(&upbuffer, OP_BMS_BASELINERESET, upnpInstance, NULL,NULL);			
			if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
				parseActionResponse(retBuf,OP_BMS_BASELINERESET_RES,upnpInstance);
				pBMSNode->BaselineReset.InvokeAction = 0;
				free(retBuf);
			}else{
				tcdbg_printf("processBMSAction : OP_BMS_BASELINERESET_RES fail\n");
			}
			
			free_ubuf(&upbuffer);
		}

		if(pBMSNode->GetDeviceStatus.InvokeAction == 1){					
			upbuffer = add_to_ubuf(&upbuffer, OP_BMS_GETDEVICESTATUS, upnpInstance, NULL,NULL);			
			if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
				parseActionResponse(retBuf,OP_BMS_GETDEVICESTATUS_RES,upnpInstance);
				pBMSNode->GetDeviceStatus.InvokeAction = 0;
				free(retBuf);
			}else{
				tcdbg_printf("processBMSAction : OP_BMS_GETDEVICESTATUS_RES fail!upnpDMsockfd=%d\n",upnpDMsockfd);
			}
			free_ubuf(&upbuffer);
		}

		if(pBMSNode->CTCOM_Download.InvokeAction == 1){		
			memset(temp,0,32);
			sprintf(temp,"%d",pBMSNode->CTCOM_Download.Channel);
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "Channel",temp);
			
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "FileType",pBMSNode->CTCOM_Download.FileType);
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "URL",pBMSNode->CTCOM_Download.URL);
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "SoftwareVersion",pBMSNode->CTCOM_Download.SoftwareVersion);
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "HardwareVersion",pBMSNode->CTCOM_Download.HardwareVersion);
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "TimeWindow",pBMSNode->CTCOM_Download.TimeWindow);
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "Username",pBMSNode->CTCOM_Download.Username);
			add_to_ubuf(&upbuffer, OP_BMS_DOWNLOAD, upnpInstance, "Password",pBMSNode->CTCOM_Download.Password);
			
			tcdbg_printf("processBMSAction: upnpDMsockfd=%d\n",upnpDMsockfd);
			if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
				parseActionResponse(retBuf,OP_BMS_DOWNLOAD_RES,upnpInstance);
				pBMSNode->CTCOM_Download.InvokeAction = 0;
				free(retBuf);
			}else{
				tcdbg_printf("processBMSAction : OP_BMS_DOWNLOAD_RES fail\n");
			}
			free_ubuf(&upbuffer);
		}

		
		if(pBMSNode->CTCOM_GetDownloadStatus.InvokeAction == 1){
			add_to_ubuf(&upbuffer, OP_BMS_GETDOWNLOADSTATUS, upnpInstance, NULL,NULL);			
			if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
				parseActionResponse(retBuf,OP_BMS_GETDOWNLOADSTATUS_RES,upnpInstance);
				pBMSNode->CTCOM_GetDownloadStatus.InvokeAction = 0;
				free(retBuf);				
			}else{
				tcdbg_printf("processBMSAction : OP_BMS_GETDOWNLOADSTATUS_RES fail\n");
			}
			free_ubuf(&upbuffer);
		}
		pNode = pNode->next;
	}

	pthread_mutex_unlock(&pDeviceList->upnpLock);
	return 0;
}

/* init UPnP DM DeviceList node */
int cwmpInitUPnPDMDeviceListTable(DeviceNodePtr deviceNode)
{
	char node_index[8];
	pDev_Node pNode = NULL;
	DeviceNodePtr ptr69Node = NULL;

	if(pDeviceList == NULL){		
		tcdbg_printf("%s: pDeviceList == NULL\n", __FUNCTION__);
		return 0;
	}

	pNode = pDeviceList->pDevNode;
	while(pNode){
		/* register this node */
		sprintf(node_index, "%d", pNode->index);
		UPnPDMDeviceListIndex[0].name = node_index;
		ptr69Node = RegisterNodeFromTable(deviceNode, UPnPDMDeviceListIndex);
		if (ptr69Node == NULL) {
			tcdbg_printf("%s:RegisterNodeFromTable fail.\n", __FUNCTION__);
			return -1;
		}
		
		if(InitDynamicUPnPDMNode(pNode->index,deviceNode)){
			tcdbg_printf("%s:InitDynamicUPnPDMNode fail.pNode->index=%d\n", __FUNCTION__,pNode->index);
			return -1;
		}
		pNode = pNode->next;
	}

	return 0;
}

int8 getDynamicNodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* dynamicValue = NULL;
	uint8 index;
	u_buf* upbuffer = NULL;
	char *retBuf = NULL;
	char upnpNodeName[256] = {0};			//length????
	int devNum = 0;
	int ret = 0;

	getUPnPDMNodeByDevNode(deviceNode,upnpNodeName);
	index = getDeviceListIndexByDevNode(deviceNode);
	devNum = getUPnPInstanceByTr69Index(index);
	
	add_to_ubuf(&upbuffer, OP_CMS_GETVALULE, devNum, upnpNodeName,NULL);
	if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
		//check if error
		if(getValueByName(retBuf,"ErrCode",&dynamicValue,NULL) == 0){ 	//content error code
			upnpDMErrCode = atoi(dynamicValue);
			if(upnpDMErrCode){
				tcdbg_printf("%s:upnpDMErrCode=%d\n",__FUNCTION__,upnpDMErrCode);	
				free(retBuf);
				ret = -1;
				if(upnpDMErrCode == 703){
					ret = 0;
				}				
				goto end;	
			}
			if(dynamicValue){
				os_free(dynamicValue);
				dynamicValue = NULL;
			}
		}


		//get the value
		if(getValueByName(retBuf,deviceNode->name,&dynamicValue,NULL)){	//no value
		//if(getValueByName(retBuf,upnpNodeName,&dynamicValue,NULL)){	//no value
			ret = getTheValue(value, type, deviceNode, "");
			free(retBuf);
			goto end;	
		}		
		free(retBuf);
	}else{
		tcdbg_printf("%s :send_u_bufwithRes fail!!! upbuffer->buf=%s \n",__FUNCTION__,upbuffer->buf); 
		ret= -1;
		goto end;
	}

	ret =  getTheValue(value, type, deviceNode, dynamicValue);
end:	
	if(&upbuffer){
		free_ubuf(&upbuffer);
	}
	
	if(dynamicValue){
		os_free(dynamicValue);
		dynamicValue = NULL;
	}
	return ret;
}

int8 setDynamicNodeValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 index;
	u_buf* upbuffer = NULL;
	char *retBuf;
	char upnpNodeName[256] = {0};
	int devNum = 0;
	int ret = 0;
	char dynamicValue[256] = {0};
	char* retValue = NULL;

	getUPnPDMNodeByDevNode(deviceNode,upnpNodeName);
	index = getDeviceListIndexByDevNode(deviceNode);
	devNum = getUPnPInstanceByTr69Index(index);

	switch(deviceNode->type){
		case BooleanType:
			sprintf(dynamicValue,"%d",*(uint8 *)value);
			add_to_ubuf(&upbuffer, OP_CMS_SETVALULE, devNum, upnpNodeName,dynamicValue);
			break;
		case UnsignedIntType:
			sprintf(dynamicValue,"%d",*(uint32*)value);
			add_to_ubuf(&upbuffer, OP_CMS_SETVALULE, devNum, upnpNodeName,dynamicValue);
			break;
		case IntType:
			sprintf(dynamicValue,"%d",*(int*)value);
			tcdbg_printf("%s :value=%s\n",__FUNCTION__,dynamicValue);	
			add_to_ubuf(&upbuffer, OP_CMS_SETVALULE, devNum, upnpNodeName,dynamicValue);
			break;
		case Base64Type:
			strcpy(dynamicValue, (char*)(value));
			decodeBase64(dynamicValue);	
			add_to_ubuf(&upbuffer, OP_CMS_SETVALULE, devNum, upnpNodeName,dynamicValue);
			break;
		case StringType:
		case DateTimeType:			
			add_to_ubuf(&upbuffer, OP_CMS_SETVALULE, devNum, upnpNodeName,(char*)value);
			break;			
		default:
			break;

	}

	
	tcdbg_printf("upbuffer=%s\n",upbuffer->buf);
	#if 1
	if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
		tcdbg_printf("%s :retBuf=%s \n",__FUNCTION__,retBuf); 
		//check if error
		if(getValueByName(retBuf,"ErrCode",&retValue,NULL) == 0){	//content error code
			upnpDMErrCode = atoi(retValue);
			if(upnpDMErrCode){
				tcdbg_printf("%s:upnpDMErrCode=%d\n",__FUNCTION__,upnpDMErrCode);	
				free(retBuf);
				ret = -1;
				goto end;	
			}
			
			if(retValue){
				os_free(retValue);
				retValue = NULL;
			}
		}
		
		//get Status
		if(getValueByName(retBuf,"Status",&retValue,NULL)){ //no value
			if(strstr(retValue,"ChangesApplied") == NULL){	//error
				tcdbg_printf("%s :retValue=%s,set error!!!\n",__FUNCTION__,retValue);  
				ret = -1;
			}
		}	
		free(retBuf);
	}else{
		tcdbg_printf("%s :upbuffer->buf=%s,error!!!\n",__FUNCTION__,upbuffer->buf);  
		ret = -1;
	}
	#endif

end:
	free_ubuf(&upbuffer);
	return ret;
}


int8 getUPnPDMNodeValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setUPnPDMNodeValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}



int8 DeviceNodeAddUPnPDMDynamicObject(uint32* num, DeviceNodePtr  deviceNode)
{
	uint8 index;
	u_buf* upbuffer = NULL;
	char *retBuf;
	char upnpNodeName[256] = {0};
	int devNum = 0;
	int ret = 0;
	
	getUPnPDMNodeByDevNode(deviceNode,upnpNodeName);
	index = getDeviceListIndexByDevNode(deviceNode);
	devNum = getUPnPInstanceByTr69Index(index);
	
	add_to_ubuf(&upbuffer, OP_CMS_CREATEINSTANCES, devNum, upnpNodeName,NULL);
	if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
		//tcdbg_printf("%s :upbuffer->buf=%s \n",__FUNCTION__,upbuffer->buf); 
		//need parse the retBuf,get the instance number
		ret = parseActionResponse(retBuf,OP_CMS_CREATEINSTANCES_RES,devNum);
		free(retBuf);
	}else{
		tcdbg_printf("%s :upbuffer->buf=%s error!!!\n",__FUNCTION__,upbuffer->buf);  
		ret = -1;
	}
	free_ubuf(&upbuffer);

	if(ret < 0)
		return -1;
	
	return InitDynamicUPnPDMNode(index,deviceNode);
}

int8 DeviceNodeDelUPnPDMDynamicObject(uint32 num, DeviceNodePtr  deviceNode)
{
	int ret = 0;
	uint8 index;
	u_buf* upbuffer = NULL;
	char *retBuf;
	char upnpNodeName[256] = {0};
	int devNum = 0;

	getUPnPDMNodeByDevNode(deviceNode,upnpNodeName);
	index = getDeviceListIndexByDevNode(deviceNode);
	devNum = getUPnPInstanceByTr69Index(index);
	
	add_to_ubuf(&upbuffer, OP_CMS_DELETEINSTANCES, devNum, upnpNodeName,num);
	if(send_u_bufwithRes(upnpDMsockfd,upbuffer,&retBuf) == 0){
		//tcdbg_printf("%s :upbuffer->buf=%s \n",__FUNCTION__,upbuffer->buf); 
		//need parse the retBuf
		ret = parseActionResponse(retBuf,OP_CMS_DELETEINSTANCES_RES,devNum);
		free(retBuf);
	}else{
		tcdbg_printf("%s :upbuffer->buf=%s, error!!!\n",__FUNCTION__,upbuffer->buf);	
		ret = -1;
	}
	free_ubuf(&upbuffer);
	
	if(ret < 0)
		return -1;
	
	return InitDynamicUPnPDMNode(index,deviceNode);
}


int8 DeviceNodeDelConfigProfileListObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	DeviceNodePtr ConfigProfileDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigProfile.";
	int rtnCode = 0;
	char tmp[32] = {0};
	int configProfileNum = 0;	

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ConfigProfileDeviceNode);
	if(ConfigProfileDeviceNode == NULL){
		tcdbg_printf("%s: search node fail!!!\n", __FUNCTION__);
		return -1;
	}

	if( (num-1 < 0) || (num > SERVICE_CONFIG_PROFILE_MAXNUM) ){
		tcdbg_printf("%s: num= %d error!!! should be [0,%d]!!!\n", __FUNCTION__,num,SERVICE_CONFIG_PROFILE_MAXNUM);
		return -1;
	}

	tcapi_get("UPnPDMService_CfgProfile", "ConfigProfileNum", tmp);
	configProfileNum = atoi(tmp);
	sprintf(tmp, "%d", configProfileNum-1);	
	tcapi_set("UPnPDMService_CfgProfile", "ConfigProfileNum", tmp);

	sprintf(nodeName, "UPnPDMService_Entry%d", num-1);	
	tcapi_unset(nodeName);

	cwmpFreeDeviceTable(ConfigProfileDeviceNode);
	rtnCode = cwmpInitConfigProfileTable(ConfigProfileDeviceNode);
	return rtnCode;
}

int8 DeviceNodeAddConfigProfileListObject(uint32* num,  DeviceNode*  deviceNode)
{
	DeviceNodePtr ConfigProfileDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigProfile.";	
	char tmp[512] = {0};
	int rtnCode = 0;
	int configProfileNum = 0;	
	int i = 0;	
	char nodeName[32] = {0};
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ConfigProfileDeviceNode);
	if(ConfigProfileDeviceNode == NULL){
		tcdbg_printf("%s:node is not exist!\n",__FUNCTION__);
		return -1;
	}

	tcapi_get("UPnPDMService_CfgProfile", "ConfigProfileNum", tmp);
	configProfileNum = atoi(tmp);
	if(configProfileNum >= SERVICE_CONFIG_PROFILE_MAXNUM){
		tcdbg_printf("%s:num is %d, error!!! should be [0,%d]!\n",__FUNCTION__,configProfileNum,SERVICE_CONFIG_PROFILE_MAXNUM);
		return -1;
	}
	
	sprintf(tmp, "%d", configProfileNum+1);	
	tcapi_set("UPnPDMService_CfgProfile", "ConfigProfileNum", tmp);
	//find the first unused node
	for(i = 0; i < SERVICE_CONFIG_PROFILE_MAXNUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName,"UPnPDMService_Entry%d",i);
		if( 0 != tcapi_get(nodeName,"Rule",tmp) )
			break;
	}
	

	// set some default value for wifi parameters	
	tcapi_set(nodeName, "Rule", "");		
	tcdbg_printf("%s:nodeName=%s\n",__FUNCTION__,nodeName);

	*num = i + 1;
	cwmpFreeDeviceTable(ConfigProfileDeviceNode);
	rtnCode = cwmpInitConfigProfileTable(ConfigProfileDeviceNode);
	return rtnCode;
}

int cwmpInitConfigProfileTable(DeviceNodePtr deviceNode)
{
	pDev_Node pNode = NULL;
	char tmp[32] = {0};		
	char nodeName[32] = {0};
	char node_index[8] = {0};
	int configProfileNum = 0;	
	int i;

	
	tcapi_get("UPnPDMService_CfgProfile", "ConfigProfileNum", tmp);
	configProfileNum = atoi(tmp);	
	for(i = 0; i < SERVICE_CONFIG_PROFILE_MAXNUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName,"UPnPDMService_Entry%d",i);
		if( 0 != tcapi_get(nodeName,"Rule",tmp) )
			continue;

		sprintf(node_index, "%d", i+1);
		ConfigProfileListIndex[0].name = node_index;
		 if(RegisterNodeFromTable(deviceNode, ConfigProfileListIndex) == NULL){
			tcdbg_printf("%s:RegisterNodeFromTable fail. i = %d\n", __FUNCTION__,i);
			return -1;
	 	}
	
	}

	return 0;
}

int8 DeviceNodeDelAPWLanListObject(uint32 num, DeviceNode*  deviceNode)
{	
	DeviceNodePtr cfgTemplateSubNode = NULL;
	char objectName[256] = {0};
	char tr69NodeName[32] = {0};
	char nodeName[32] = {0};
	char attrAPNum[32] = {0};
	char tmp[16] = {0};
	int rtnCode = 0;	
	uint8 apNum;

	strcpy(tr69NodeName,deviceNode->parentNode->name);	
	sprintf(objectName,"InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.%s.",tr69NodeName);
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &cfgTemplateSubNode);
	if(cfgTemplateSubNode == NULL){
		tcdbg_printf("%s: search node fail!!!\n", __FUNCTION__);
		return -1;
	}

	if( (num-1 < 0) || (num > AP_MAX_WLANNUM) ){
		tcdbg_printf("%s: num= %d error!!! should be [0,%d]!!!\n", __FUNCTION__,num,AP_MAX_WLANNUM);
		return -1;
	}
	
	if(strcmp(tr69NodeName,X_CT_COM"AP_Internet") == 0){
		sprintf(nodeName,"UPnPDMAPInt_Entry%d",num-1);
		strcpy(attrAPNum,"AP_InternetNum");
	}else if(strcmp(tr69NodeName,X_CT_COM"AP_IPTV") == 0){
		sprintf(nodeName,"UPnPDMAPIptv_Entry%d",num-1);
		strcpy(attrAPNum,"AP_IPTVNum");
	}else{
		tcdbg_printf("%s: tr69NodeName=%s is error!!!\n", __FUNCTION__,tr69NodeName);
		return -1;
	}	

	if( 0 != tcapi_get(nodeName,"Enable",tmp) ){
		tcdbg_printf("%s:num = %d, node not exist\n",__FUNCTION__,num,AP_MAX_WLANNUM);
		return -1;
	}
	tcapi_unset(nodeName);

	
	tcapi_get("UPnPDMService_CfgTemplate", attrAPNum, tmp);
	apNum = atoi(tmp);
	sprintf(tmp,"%d",apNum-1);
	tcapi_set("UPnPDMService_CfgTemplate", attrAPNum, tmp);
	
	cwmpFreeDeviceTable(cfgTemplateSubNode);
	rtnCode = cwmpInitAPWLanTable(cfgTemplateSubNode);

	return rtnCode;
}

int8 DeviceNodeAddAPWLanListObject(uint32* num,  DeviceNode*  deviceNode)
{	
	DeviceNodePtr cfgTemplateSubNode = NULL;
	char objectName[256] = {0};
	char tr69NodeName[32] = {0};	
	char nodeName[32] = {0};
	char nodeNameTmp[32] = {0};
	char attrAPNum[32] = {0};
	char tmp[16] = {0};
	int rtnCode = 0;	
	uint8 apNum;
	int i = 0;
	
	strcpy(tr69NodeName,deviceNode->name);	
	sprintf(objectName,"InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.%s.",tr69NodeName);
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &cfgTemplateSubNode);
	if(cfgTemplateSubNode == NULL){
		tcdbg_printf("%s: node is not exist!\n",__FUNCTION__);
		return -1;
	}
	
	if(strcmp(tr69NodeName,X_CT_COM"AP_Internet") == 0){		
		strcpy(nodeNameTmp,"UPnPDMAPInt_Entry");
		strcpy(attrAPNum,"AP_InternetNum");
	}else if(strcmp(tr69NodeName,X_CT_COM"AP_IPTV") == 0){		
		strcpy(nodeNameTmp,"UPnPDMAPIptv_Entry");
		strcpy(attrAPNum,"AP_IPTVNum");
	}else{
		tcdbg_printf("%s: tr69NodeName=%s is error!!!\n", __FUNCTION__,tr69NodeName);
		return -1;
	}
	

	tcapi_get("UPnPDMService_CfgTemplate",attrAPNum, tmp);
	apNum = atoi(tmp);
	if(apNum >= AP_MAX_WLANNUM){
		tcdbg_printf("%s:the exist num is %d, can not add anymore!\n",__FUNCTION__,apNum);
		return -1;
	}
	
	sprintf(tmp,"%d",apNum+1);
	tcapi_set("UPnPDMService_CfgTemplate", attrAPNum, tmp);
	
	//find the first unused node
	for(i = 0; i < AP_MAX_WLANNUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName,"%s%d",nodeNameTmp,i);
		if( 0 != tcapi_get(nodeName,"Enable",tmp) )
			break;
	}

	// set some default value for wifi parameters	
	tcapi_set(nodeName, "Enable", "0");		

	*num = i + 1;
	cwmpFreeDeviceTable(cfgTemplateSubNode);
	rtnCode = cwmpInitAPWLanTable(cfgTemplateSubNode);
	return rtnCode;
}

int cwmpInitAPWLanTable(DeviceNodePtr deviceNode)
{
	char tr69NodeName[32] = {0};	
	char nodeName[32] = {0};
	char nodeNameTmp[32] = {0};
	char node_index[8] = {0};
	char tmp[16] = {0};
	int rtnCode = 0;	
	int i = 0;
	
	
	strcpy(tr69NodeName,deviceNode->name);	
	if(strcmp(tr69NodeName,X_CT_COM"AP_Internet") == 0){		
		strcpy(nodeNameTmp,"UPnPDMAPInt_Entry");
	}else if(strcmp(tr69NodeName,X_CT_COM"AP_IPTV") == 0){		
		strcpy(nodeNameTmp,"UPnPDMAPIptv_Entry");
	}else{
		tcdbg_printf("%s: tr69NodeName=%s is error!!!\n", __FUNCTION__,tr69NodeName);
		return -1;
	}

	for(i = 0; i < AP_MAX_WLANNUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName,"%s%d",nodeNameTmp,i);		
		if( 0 != tcapi_get(nodeName,"Enable",tmp) )
			continue;
		
		sprintf(node_index, "%d", i+1);
		APWLanIndex[0].name = node_index;
		 if(RegisterNodeFromTable(deviceNode, APWLanIndex) == NULL){
			tcdbg_printf("%s:RegisterNodeFromTable fail. i = %d\n", __FUNCTION__,i);
			return -1;
	 	}
	
	}
	
	return 0;
}

int  cwmpInitAPWEPKeyTable( DeviceNodePtr deviceNode )
{
	int num = APWLAN_MAX_WEPKEY, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, APWEPKeyeIndex, num );

	return rtnCode;
}

int  cwmpInitAPPreSharedKey( DeviceNodePtr deviceNode )
{
	int num = APWLAN_MAX_PRE_SHARED_KEY, rtnCode = 0;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, APPreSharedKeyIndex, num );

	return rtnCode;
}

int8 DeviceNodeDelFileProfileListObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	DeviceNodePtr FileProfileDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.";
	int rtnCode = 0;
	int fileProfileNum = 0;	
	char tmp[16] = {0};

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &FileProfileDeviceNode);
	if(FileProfileDeviceNode == NULL){
		tcdbg_printf("%s: search node fail!!!\n", __FUNCTION__);
		return -1;
	}
	
	if( (num-1 < 0) || (num > SOFTWARE_FILE_PROFILE_MAXNUM) ){
		tcdbg_printf("%s: num= %d error!!! should be [0,%d]!!!\n", __FUNCTION__,num,SOFTWARE_FILE_PROFILE_MAXNUM);
		return -1;
	}

	sprintf(nodeName,"UPnPDMSWProfile_Entry%d",num-1);
	if( 0 != tcapi_get(nodeName,"Rule",tmp) ){
		tcdbg_printf("%s:num = %d, node not exist\n",__FUNCTION__,num,AP_MAX_WLANNUM);
		return -1;
	}
	tcapi_unset(nodeName);
	
	tcapi_get("UPnPDMSWProfile_Common", "FileProfileNum", tmp);
	fileProfileNum = atoi(tmp);
	sprintf(tmp, "%d", fileProfileNum-1);	
	tcapi_set("UPnPDMSWProfile_Common", "FileProfileNum", tmp);

	cwmpFreeDeviceTable(FileProfileDeviceNode);
	rtnCode = cwmpInitFileProfileTable(FileProfileDeviceNode);
	return rtnCode;
}

int8 DeviceNodeAddFileProfileListObject(uint32* num,  DeviceNode*  deviceNode)
{
	DeviceNodePtr FileProfileDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.";
	char tmp[512] = {0};
	int rtnCode = 0;
	int fileProfileNum = 0;	
	int i = 0;
	char nodeName[32] = {0};
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &FileProfileDeviceNode);
	if(FileProfileDeviceNode == NULL){
		tcdbg_printf("%s:node is not exist!\n",__FUNCTION__);
		return -1;
	}

	tcapi_get("UPnPDMSWProfile_Common", "FileProfileNum", tmp);
	fileProfileNum = atoi(tmp);
	if(fileProfileNum >= SOFTWARE_FILE_PROFILE_MAXNUM){
		tcdbg_printf("%s:num is %d, error!!! should be [0,%d]!\n",__FUNCTION__,SOFTWARE_FILE_PROFILE_MAXNUM,SOFTWARE_FILE_PROFILE_MAXNUM);
		return -1;
	}

	
	sprintf(tmp, "%d", fileProfileNum+1);	
	tcapi_set("UPnPDMSWProfile_Common", "FileProfileNum", tmp);
	//find the first unused node
	for(i = 0; i < SOFTWARE_FILE_PROFILE_MAXNUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName,"UPnPDMSWProfile_Entry%d",i);
		if( 0 != tcapi_get(nodeName,"Rule",tmp) )
			break;
	}
	

	// set some default value for wifi parameters	
	tcapi_set(nodeName, "Rule", "");		

	*num = i + 1;
	cwmpFreeDeviceTable(FileProfileDeviceNode);
	rtnCode = cwmpInitFileProfileTable(FileProfileDeviceNode);
	return rtnCode;
}


int cwmpInitFileProfileTable(DeviceNodePtr deviceNode)
{
	pDev_Node pNode = NULL;
	char tmp[32] = {0};			
	char nodeName[32] = {0};
	char node_index[8] = {0};
	int fileProfileNum = 0;	
	int i;

	
	tcapi_get("UPnPDMSWProfile_Common", "FileProfileNum", tmp);
	fileProfileNum = atoi(tmp);	
	for(i = 0; i < SOFTWARE_FILE_PROFILE_MAXNUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName,"UPnPDMSWProfile_Entry%d",i);
		if( 0 != tcapi_get(nodeName,"Rule",tmp) )
			continue;
		
		sprintf(node_index, "%d", i+1);
		FileProfileListIndex[0].name = node_index;
		 if(RegisterNodeFromTable(deviceNode, FileProfileListIndex) == NULL){
			tcdbg_printf("%s:RegisterNodeFromTable fail. i = %d\n", __FUNCTION__,i);
			return -1;
	 	}
	
	}

	return 0;
}


int8 DeviceNodeDelTimeWindowListObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[32] = {'\0'};
	uint8 fileProfileIndex;
	uint8 timeWindowNum;
	DeviceNodePtr TimeWindowDeviceNode = NULL;
	char objectName[256] = {0}; 
	int rtnCode = 0;
	char tmp[16] = {0};

	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->name);
	sprintf(objectName,"InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.%d.TimeWindowList.",fileProfileIndex);
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &TimeWindowDeviceNode);
	if(TimeWindowDeviceNode == NULL){
		tcdbg_printf("%s: search node fail!!!\n", __FUNCTION__);
		return -1;
	}
	
	if( (num-1 < 0) || (num > SOFTWARE_FILE_PROFILE_MAXNUM) ){
		tcdbg_printf("%s: num= %d error!!! should be [0,%d]!!!\n", __FUNCTION__,num,SOFTWARE_FILE_PROFILE_MAXNUM);
		return -1;
	}

	sprintf(nodeName, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,num-1);	
	if( 0 != tcapi_get(nodeName,"WindowStart",tmp) ){
		tcdbg_printf("%s:num = %d, node not exist\n",__FUNCTION__,num,AP_MAX_WLANNUM);
		return -1;
	}
	tcapi_unset(nodeName);

	sprintf(nodeName, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);	
	tcapi_get(nodeName, "TimeWindowListNum", tmp);
	timeWindowNum = atoi(tmp);
	sprintf(tmp, "%d", timeWindowNum-1);	
	tcapi_set(nodeName, "TimeWindowListNum", tmp);

	cwmpFreeDeviceTable(TimeWindowDeviceNode);
	rtnCode = cwmpInitTimeWindowListTable(TimeWindowDeviceNode);
	return rtnCode;
}

int8 DeviceNodeAddTimeWindowListObject(uint32* num,  DeviceNode*  deviceNode)
{
	DeviceNodePtr TimeWindowDeviceNode = NULL;
	char objectName[256] = {0}; 
	char tmp[16] = {0};
	int rtnCode = 0;
	uint8 fileProfileIndex;
	int timeWindowNum = 0;	
	int i = 0;
	char nodeValue[MAXLEN_TCAPI_MSG] = {0};	
	char nodeName[32] = {0};

	
	fileProfileIndex = atoi(deviceNode->parentNode->name);
	sprintf(objectName,"InternetGatewayDevice.X_CT-COM_ProxyDevice.SoftwareProfile.FileProfile.%d.TimeWindowList.",fileProfileIndex);
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &TimeWindowDeviceNode);
	if(TimeWindowDeviceNode == NULL){
		tcdbg_printf("%s:node is not exist!\n",__FUNCTION__);
		return -1;
	}

	sprintf(nodeName, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1); 
	tcapi_get(nodeName, "TimeWindowListNum", tmp);
	timeWindowNum = atoi(tmp);
	if(timeWindowNum >= TIME_WINDOW_LIST_MAXNUM){
		tcdbg_printf("%s:num is %d, error!!! should be [0,%d]!\n",__FUNCTION__,timeWindowNum,TIME_WINDOW_LIST_MAXNUM);
		return -1;
	}

	sprintf(tmp, "%d", timeWindowNum + 1); 
	tcapi_set(nodeName, "TimeWindowListNum", tmp);
	//find the first unused node
	for(i = 0; i < TIME_WINDOW_LIST_MAXNUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,i); 
		if( 0 != tcapi_get(nodeName,"WindowStart",nodeValue) )
			break;
	}
	
	// set some default value for wifi parameters	
	tcapi_set(nodeName, "WindowStart", "");	

	*num = i + 1;
	cwmpFreeDeviceTable(TimeWindowDeviceNode);
	rtnCode = cwmpInitTimeWindowListTable(TimeWindowDeviceNode);
	return rtnCode;
}


int cwmpInitTimeWindowListTable(DeviceNodePtr deviceNode)
{
	char node_index[8] = {0};
	char nodeName[32] = {0};	
	char tmp[32] = {0};		
	pDev_Node pNode = NULL;
	int fileProfileIndex = 0;	
	int timeWindowListNum = 0;
	int i;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(nodeName, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(nodeName, "TimeWindowListNum", tmp);

	timeWindowListNum = atoi(tmp);
	for(i = 0; i < TIME_WINDOW_LIST_MAXNUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,i); 
		if( 0 != tcapi_get(nodeName,"WindowStart",tmp) )
			continue;
	
		sprintf(node_index, "%d", i+1);
		TimeWindowListIndex[0].name = node_index;
		 if(RegisterNodeFromTable(deviceNode, TimeWindowListIndex) == NULL){
			tcdbg_printf("%s:RegisterNodeFromTable fail. i = %d\n", __FUNCTION__,i);
			return -1;
	 	}
	}

	return 0;
}


int8 getDeviceListNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{		
	char strNum[32] = {0};
	
	if(pDeviceList == NULL){	//no deviceList exist
		strcpy(strNum,"0");
	}else{
		sprintf(strNum,"%d",pDeviceList->num);
	}

	return getTheValue(value, type, deviceNode, strNum);
}


int8 getDeviceListNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setDeviceListNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}


int8 getBMS_InvokeActionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char InvokeAction[8] = {'\0'};
	DeviceNode*  pDevNode = NULL;
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pDevNode = deviceNode->parentNode;
	if(strcmp(pDevNode->name,"Reboot") == 0){	
		getBMSValueByIndex(index,BMS_REBOOT_INDEX,BMS_INVOKE,InvokeAction);
	}else if(strcmp(pDevNode->name,"BaselineReset") == 0){
		getBMSValueByIndex(index,BMS_BASELINE_RESET_INDEX,BMS_INVOKE,InvokeAction);
	}else if(strcmp(pDevNode->name,"GetDeviceStatus") == 0){
		getBMSValueByIndex(index,BMS_DEVICE_STATUS_INDEX,BMS_INVOKE,InvokeAction);
	}else if(strcmp(pDevNode->name,X_CT_COM"Download") == 0){
		getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_INVOKE,InvokeAction);
	}else if(strcmp(pDevNode->name,X_CT_COM"GetDownloadStatus") == 0){
		getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_STATUS_INDEX,BMS_INVOKE,InvokeAction);
	}

	tcdbg_printf("getBMS_InvokeActionValue: InvokeAction=%s\n",InvokeAction);
	return getTheValue(value, type, deviceNode, InvokeAction);
}
int8 setBMS_InvokeActionValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 index;	
	DeviceNode*  pDevNode;
	int ret;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	tcdbg_printf("setBMS_InvokeActionValue\n");
	pDevNode = deviceNode->parentNode;
	if(strcmp(pDevNode->name,"Reboot") == 0){	
		ret = setBMSValueByIndex(index,BMS_REBOOT_INDEX,BMS_INVOKE,value);
	}else if(strcmp(pDevNode->name,"BaselineReset") == 0){
		ret = setBMSValueByIndex(index,BMS_BASELINE_RESET_INDEX,BMS_INVOKE,value);
	}else if(strcmp(pDevNode->name,"GetDeviceStatus") == 0){
		ret = setBMSValueByIndex(index,BMS_DEVICE_STATUS_INDEX,BMS_INVOKE,value);
	}else if(strcmp(pDevNode->name,X_CT_COM"Download") == 0){
		ret = setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_INVOKE,value);
	}else if(strcmp(pDevNode->name,X_CT_COM"GetDownloadStatus") == 0){
		ret = setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_STATUS_INDEX,BMS_INVOKE,value);
	}

	return ret;
}


int8 getBMS_RebootStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char rebootStatus[16] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_REBOOT_INDEX,BMS_REBOOT_STATUS,rebootStatus);
	return getTheValue(value, type, deviceNode, rebootStatus);
}

int8 getBMS_DeviceStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char deviceStatus[16] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_DEVICE_STATUS_INDEX,BMS_DEVICE_STATUS,deviceStatus);
	return getTheValue(value, type, deviceNode, deviceStatus);
}


int8 getBMS_FileType(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char fileType[64] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_FILETYPE,fileType);
	return getTheValue(value, type, deviceNode, fileType);
}

int8 setBMS_FileType(void* value, struct DeviceNode*  deviceNode)
{
	char fileType[64] = {'\0'};
	uint8 index;

	if(strlen((char*)value)>=64){
		tcdbg_printf("setBMS_FileType: length is more than 64\n");		
		return -1;
	}
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);	
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_FILETYPE,value);
}

int8 getBMS_URL(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char url[256] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_URL,url);
	return getTheValue(value, type, deviceNode, url);
}
int8 setBMS_URL(void* value, struct DeviceNode*  deviceNode)
{
	char url[256] = {'\0'};
	uint8 index;
	if(strlen((char*)value)>=256){
		tcdbg_printf("setBMS_URL: length is more than 256\n");		
		return -1;
	}
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_URL,value);
}

int8 getBMS_SoftwareVersion(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char softwareVersion[64] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_SOFTWAREVERSION,softwareVersion);
	return getTheValue(value, type, deviceNode, softwareVersion);
}
int8 setBMS_SoftwareVersion(void* value, struct DeviceNode*  deviceNode)
{
	char softwareVersion[64] = {'\0'};
	uint8 index;

	if(strlen((char*)value)>=64){
		tcdbg_printf("setBMS_SoftwareVersion: length is more than 64\n");		
		return -1;
	}
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_SOFTWAREVERSION,value);
}

int8 getBMS_HardwareVersion(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char hardwareVersion[64] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_HARDWAREVERSION,hardwareVersion);
	return getTheValue(value, type, deviceNode, hardwareVersion);
}
int8 setBMS_HardwareVersion(void* value, struct DeviceNode*  deviceNode)
{
	char hardwareVersion[64] = {'\0'};
	uint8 index;

	if(strlen((char*)value)>=64){
		tcdbg_printf("setBMS_HardwareVersion: length is more than 64\n");		
		return -1;
	}
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_HARDWAREVERSION,value);
}

int8 getBMS_Channel(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char channel[8] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_CHANNEL,channel);
	return getTheValue(value, type, deviceNode, channel);
}

int8 setBMS_Channel(void* value, struct DeviceNode*  deviceNode)
{
	char channel[64] = {'\0'};
	uint8 index;
	int iChannel;

	iChannel = *((int*)value);
	if(iChannel < -1 || iChannel > 4095){
		tcdbg_printf("setBMS_Channel: iChannel=%d,should be [-1,4095]\n",iChannel);		
		return -1;
	}
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_CHANNEL,value);
}

int8 getBMS_TimeWindow(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char timeWindow[1024] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_TIMEWINDOW,timeWindow);
	return getTheValue(value, type, deviceNode, timeWindow);
}
int8 setBMS_TimeWindow(void* value, struct DeviceNode*  deviceNode)
{
	char timeWindow[64] = {'\0'};
	uint8 index;	

	if(strlen((char*)value)>=1024){
		tcdbg_printf("setBMS_TimeWindow: length is more than 1024\n");		
		return -1;
	}
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_TIMEWINDOW,value);
}


int8 getBMS_Username(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char username[256] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_USERNAME,username);
	return getTheValue(value, type, deviceNode, username);
}
int8 setBMS_Username(void* value, struct DeviceNode*  deviceNode)
{
	char username[256] = {'\0'};
	uint8 index;

	
	if(strlen((char*)value)>=256){
		tcdbg_printf("setBMS_Username: length is more than 256\n");		
		return -1;
	}

	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_USERNAME,value);
}

int8 getBMS_Password(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char password[256] = {'\0'};
	uint8 index;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_PASSWORD,password);
	return getTheValue(value, type, deviceNode, password);
}
int8 setBMS_Password(void* value, struct DeviceNode*  deviceNode)
{
	char password[64] = {'\0'};
	uint8 index;

	if(strlen((char*)value)>=256){
		tcdbg_printf("setBMS_Password: length is more than 256\n");		
		return -1;
	}

	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	return setBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_PASSWORD,value);
}

int8 getBMS_DownloadStatus(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char downloadStatus[32] = {'\0'};
	uint8 index;
	uint32* status;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_STATUS,downloadStatus);
	return getTheValue(value, type, deviceNode, downloadStatus);
}


int8 getBMS_OperationID(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char operationID[32] = {'\0'};
	uint8 index;
	uint32* id;
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_INDEX,BMS_DOWNLOAD_OPERETIONID,operationID);
	return getTheValue(value, type, deviceNode, operationID);
}



int8 getBMS_OperationStatus(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char operationStatus[256] = {'\0'};
	uint8 index;
	
	
	index = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	getBMSValueByIndex(index,BMS_CTCOM_DOWNLOAD_STATUS_INDEX,BMS_DOWNLOADSTATUS_OPERATIONSTATUS,operationStatus);
	return getTheValue(value, type, deviceNode, operationStatus);
}

int8 getRMSConfigOver(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char RMSConfigOver[32] = {0};
	
	tcapi_get("UPnPDMSWProfile_Common", "RMSConfigOver", RMSConfigOver);
	
	return getTheValue(value, type, deviceNode, RMSConfigOver);
}

int8 setRMSConfigOver(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	uint32 RMSConfigOver = 0;

	RMSConfigOver = *((uint32*)value);
	sprintf(tmp,"%d",RMSConfigOver);

	tcapi_set("UPnPDMSWProfile_Common", "RMSConfigOver", tmp);

	if(RMSConfigOver == 1){			//config over, need clear the flag isApplied. if isApplied=1, template will not be applied any more.
		tcapi_set("UPnPDMSWProfile_Common", "isApplied", "0");
		//need notify UPnP DM to use the new template
	}
	
	return 0;
}


int8 getRMSConfigOverAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setRMSConfigOverAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char FileProfileNum[32] = {0};
	
	tcapi_get("UPnPDMSWProfile_Common", "FileProfileNum", FileProfileNum);
	
	return getTheValue(value, type, deviceNode, FileProfileNum);
}

int8 getFileProfileNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileRule(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[256] = {0};
	char node_name[256] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "Rule", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setFileProfileRule(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 256){
		tcdbg_printf("%s: length is more than 256, error!!!\n",__FUNCTION__);	
		return -1;
	}

	fileProfileIndex = atoi(deviceNode->parentNode->name);			
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);	
	tcapi_set(node_name, "Rule", tmp);
	tcapi_set(node_name, "isNew", "1");
	return 0;

}


int8 getFileProfileRuleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileRuleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileFileType(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "FileType", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);

}

int8 setFileProfileFileType(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);	
		return -1;
	}

	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "FileType", tmp);	
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getFileProfileFileTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileFileTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileURL(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[256] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "URL", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setFileProfileURL(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 256){
		tcdbg_printf("%s: length is more than 256, error!!!\n",__FUNCTION__);
		return -1;
	}

	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "URL", tmp);	
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getFileProfileURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileSWVersion(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "SoftwareVersion", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setFileProfileSWVersion(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);	
		return -1;
	}

	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "SoftwareVersion", tmp);
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getFileProfileSWVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileSWVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileHWVersion(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "HardwareVersion", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setFileProfileHWVersion(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);	
		return -1;
	}

	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "HardwareVersion", tmp);	
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getFileProfileHWVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileHWVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileChannel(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "Channel", tmp);

	return getTheValue(value, type, deviceNode, tmp);
}

int8 setFileProfileChannel(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	
	int channel = -1;

	channel = *(int*)value;
	if(channel < -1 || channel > 4095){
		tcdbg_printf("%s: channel should be [-1,4095]. error!!!\n",__FUNCTION__);	
		return -1;
	}

	sprintf(tmp,"%d",channel);
	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "Channel", tmp);	
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getFileProfileChannelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileChannelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfileUsername(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "Username", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setFileProfileUsername(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);	
		return -1;
	}

	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "Username", tmp);
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getFileProfileUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfileUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getFileProfilePassword(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "Password", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setFileProfilePassword(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);	
		return -1;
	}

	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "Password", tmp);
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getFileProfilePasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setFileProfilePasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}


int8 getTimeWindowListNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_get(node_name, "TimeWindowListNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setTimeWindowListNum(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;	
	int num = 0;

	num = *(int *)value;
	if(num > TIME_WINDOW_LIST_MAXNUM){
		tcdbg_printf("%s: num = %d is more than %d\n",__FUNCTION__,num, TIME_WINDOW_LIST_MAXNUM);
		return -1;
	}
	
	sprintf(tmp,"%d",num);
	fileProfileIndex = atoi(deviceNode->parentNode->name);		
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "TimeWindowListNum", tmp);
	return 0;
}


int8 getTimeWindowListNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setTimeWindowListNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getWindowStart(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_get(node_name, "WindowStart", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setWindowStart(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;
	uint32 start = 0;
	*((uint32*)value);

	start = *((uint32*)value);
	sprintf(tmp,"%d",start);
	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_set(node_name, "WindowStart", tmp);

	memset(node_name,0,sizeof(node_name));
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "isNew", "1");
	
	return 0;
}


int8 getWindowStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setWindowStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getWindowEnd(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_get(node_name, "WindowEnd", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setWindowEnd(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	
	uint32 end = 0;

	end = *((uint32*)value);
	sprintf(tmp,"%d",end);
	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_set(node_name, "WindowEnd", tmp);

	
	memset(node_name,0,sizeof(node_name));
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getWindowEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setWindowEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getWindowMode(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_get(node_name, "WindowMode", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setWindowMode(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);
		return -1;
	}
	
	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_set(node_name, "WindowMode", tmp);
	
	memset(node_name,0,sizeof(node_name));
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getWindowModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setWindowModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getUserMessage(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[512] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_get(node_name, "UserMessage", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setUserMessage(void* value, struct DeviceNode*  deviceNode)
{
	char* tmp = NULL;
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	

	tmp = (char*)value;
	if(strlen(tmp) >= 512){
		tcdbg_printf("%s: length is more than 512, error!!!\n",__FUNCTION__);	
		return -1;
	}
	
	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_set(node_name, "UserMessage", tmp);
	
	memset(node_name,0,sizeof(node_name));
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getUserMessageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setUserMessageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getMaxRetries(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	

	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_get(node_name, "MaxRetries", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setMaxRetries(void* value, struct DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int fileProfileIndex = 0;		
	int timeWindowListIndex = 0;	
	int maxRetries= 0;

	maxRetries = *(int *)value;
	sprintf(tmp,"%d",maxRetries);
	fileProfileIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	timeWindowListIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMSWProfile_Entry%d_Entry%d", fileProfileIndex-1,timeWindowListIndex-1);		
	tcapi_set(node_name, "MaxRetries", tmp);

	
	memset(node_name,0,sizeof(node_name));
	sprintf(node_name, "UPnPDMSWProfile_Entry%d", fileProfileIndex-1);		
	tcapi_set(node_name, "isNew", "1");
	return 0;
}


int8 getMaxRetriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setMaxRetriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getConfigProfileNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgProfile", "ConfigProfileNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getConfigProfileNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setConfigProfileNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getSerProfileRMSConfigOver(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgProfile", "RMSConfigOver", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setSerProfileRMSConfigOver(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	uint32 configOver= 0;

	configOver = *(uint32 *)value;
	sprintf(tmp,"%d",configOver);
	tcapi_set("UPnPDMService_CfgProfile", "RMSConfigOver", tmp);
	
	if(configOver == 1){			//config over, need clear the flag isApplied. if isApplied=1, template will not be applied any more.
		tcapi_set("UPnPDMService_CfgProfile", "isApplied", "0");
		//need notify UPnP DM to use the new template
	}
	return 0;
}

int8 getSerProfileRMSConfigOverAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setSerProfileRMSConfigOverAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getConfigurationMode(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgProfile", "ConfigurationMode", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setConfigurationMode(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	uint32 configOver= 0;

	configOver = *(uint32 *)value;
	sprintf(tmp,"%d",configOver);
	tcapi_set("UPnPDMService_CfgProfile", "ConfigurationMode", tmp);
	return 0;
}

int8 getConfigurationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setConfigurationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getConfigProfileRule(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int configProfileIndex = 0;		

	configProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMService_Entry%d", configProfileIndex-1);		
	tcapi_get(node_name, "Rule", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setConfigProfileRule(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int configProfileIndex = 0;		

	tmp = (char*)value;
	if(strlen(tmp) >= 512){
		tcdbg_printf("%s: length is more than 512, error!!!\n",__FUNCTION__);	
		return -1;
	}
	configProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMService_Entry%d", configProfileIndex-1);		
	tcapi_set(node_name, "Rule", tmp);	
	tcapi_set(node_name, "isNew", "1");
	return 0;
}

int8 getConfigProfileRuleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setConfigProfileRuleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getConfigurationPath(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int configProfileIndex = 0;		

	configProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMService_Entry%d", configProfileIndex-1);		
	tcapi_get(node_name, "ConfigurationPath", tmp);
	tcapi_set(node_name, "isNew", "1");
	return getTheValue(value, type, deviceNode, tmp);
}

//tr69Path="InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.1.;InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.X_CT-COM_AP_Internet.2.
int checkCfgPathAsUPnPDMType(char *tr69Path){
	char* cfgTemplatePath = "InternetGatewayDevice.X_CT-COM_ProxyDevice.ServiceProfile.ConfigTemplate.";	
	char* cfgTemplate = NULL;
	char* tmp = NULL;
	char* p = NULL;
	char* delim = ",";
	int len = 0;
	int index;
	int flag = 0;

	p = strtok(tr69Path,delim);
	while(p){
		len = strlen(p);		
		tcdbg_printf("%s: len=%d!\n",__FUNCTION__,len);	
		if((cfgTemplate = strstr(p,cfgTemplatePath)) == NULL || strlen(cfgTemplate) != len){
			tcdbg_printf("%s: p=%s,is not ConfigTemplate Path!!!\n",__FUNCTION__,p);	
			return -1;
		}

		cfgTemplate += strlen(cfgTemplatePath);
		if((tmp = strstr(cfgTemplate,X_CT_COM"AP_Internet.")) != NULL ){
			tmp += strlen(X_CT_COM"AP_Internet.");
		}else if((tmp = strstr(cfgTemplate,X_CT_COM"AP_IPTV.")) != NULL ){
			tmp += strlen(X_CT_COM"AP_IPTV.");
		}else{
			tcdbg_printf("%s: cfgTemplate=%s error\n",__FUNCTION__,cfgTemplate);	
			return -1;
		}

		index = atoi(tmp);
		if(index <= 0 || index > AP_MAX_WLANNUM){
			tcdbg_printf("%s: index=%d error\n",__FUNCTION__,index);	
			return -1;
		}
		
		p= strtok(NULL,delim);
	}

	return 0;
}

int8 setConfigurationPath(void* value, struct DeviceNode*  deviceNode)
{	 
	char* tmp = NULL;
	char path[256] = {0};
	char node_name[32] = {0};
	int configProfileIndex = 0;		

	tmp = (char*)value;
	if(strlen(tmp) >= 256){
		tcdbg_printf("%s: length is more than 256, error!!!\n",__FUNCTION__);	
		return -1;
	}

	configProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMService_Entry%d", configProfileIndex-1);		
	strcpy(path,tmp);
	if(checkCfgPathAsUPnPDMType(path) != 0){
		tcdbg_printf("%s: value is not right.\n",__FUNCTION__);	
		return -1;
	}
	
	tcapi_set(node_name, "ConfigurationPath", tmp);	
	tcapi_set(node_name, "isNew", "1");
	return 0;
}

int8 getConfigurationPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setConfigurationPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getConfigProvisionCode(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int configProfileIndex = 0;		

	configProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMService_Entry%d", configProfileIndex-1);		
	tcapi_get(node_name, "ProvisioningCode", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setConfigProvisionCode(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int configProfileIndex = 0;		

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);	
		return -1;
	}
	configProfileIndex = atoi(deviceNode->parentNode->name);	
	sprintf(node_name, "UPnPDMService_Entry%d", configProfileIndex-1);		
	tcapi_set(node_name, "ProvisioningCode", tmp);
	return 0;
}

int8 getConfigProvisionCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setConfigProvisionCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAP_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "AP_InternetNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getAP_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAP_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAP_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "AP_IPTVNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getAP_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAP_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAP_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "AP_VoIPNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getAP_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAP_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getSTB_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "STB_InternetNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getSTB_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setSTB_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getSTB_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "STB_IPTVNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getSTB_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setSTB_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getSTB_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "STB_VoIPNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getSTB_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setSTB_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getScreen4_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "Screen4_IPTVNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getScreen4_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setScreen4_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getScreen4_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "Screen4_InternetNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getScreen4_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setScreen4_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getScreen4_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "Screen4_VoIPNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getScreen4_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setScreen4_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getHomeCenter_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "HomeCenter_InternetNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getHomeCenter_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setHomeCenter_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getHomeCenter_NASNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "HomeCenter_NASNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getHomeCenter_NASNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setHomeCenter_NASNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getHomeCenter_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "HomeCenter_IPTVNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getHomeCenter_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setHomeCenter_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getHomeCenter_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "HomeCenter_VoIPNum", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getHomeCenter_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setHomeCenter_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getPlc_Num(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};

	tcapi_get("UPnPDMService_CfgTemplate", "Plc_Num", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 getPlc_NumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setPlc_NumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int getCfgNodenameByDeviceNode(DeviceNode*  deviceNode, int deep,  int index, char* node_name){
	char tr69NodeName[32] = {0};
	DeviceNode*  pNode = NULL;
	int i = 0;

	pNode = deviceNode;
	while( i < deep){
		pNode = pNode->parentNode;
		i ++;
	}
	
	strcpy(tr69NodeName,pNode->name);
	if(strcmp(tr69NodeName,X_CT_COM"AP_Internet") == 0){
		sprintf(node_name,"UPnPDMAPInt_Entry%d",index-1);
		return AP_Internet;
	}else if(strcmp(tr69NodeName,X_CT_COM"AP_IPTV") == 0){
		sprintf(node_name,"UPnPDMAPIptv_Entry%d",index-1);
		return AP_IPTV;
	}else{
		tcdbg_printf("%s: tr69NodeName=%s is error!!!\n", __FUNCTION__,tr69NodeName);
		return -1;
	}	

}

int8 getAPWLanEnable(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	char vEnable[4] = {0};	
	
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "Enable", tmp);
	if(strcmp(tmp, "Yes") == 0 || strcmp(tmp, "1") == 0 || strcmp(tmp, "true") == 0)
		strcpy(vEnable, "1"); 
	else
		strcpy(vEnable, "0"); 
		
	return getTheValue(value, type, deviceNode, vEnable);
}

int8 setAPWLanEnable(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[8];
	char node_name[32] = {0};
	char tr69NodeName[32] = {0};
	int apWLanIdx = 0;	
	boolean* enable;
	int templateType;

	enable = (boolean *)value;
	if( *enable == (boolean)0)
		sprintf(tmp, "%s", "0");
	else if( *enable == (boolean)1 ) 
		sprintf(tmp, "%s", "1");
	else 
		return -1;

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "Enable", tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"Enable");
	return 0;
}

int8 getAPWLanEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPWLanMaxBitRate(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[4] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "MaxBitRate", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPWLanMaxBitRate(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	int templateType;

	tmp = (char*)value;
	if(strlen(tmp) >= 4){
		tcdbg_printf("%s: length is more than 4, error!!!\n",__FUNCTION__);	
		return -1;
	}
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "MaxBitRate", tmp);

	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"MaxBitRate");
	return 0;

}

int8 getAPWLanMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}
int8 getAPWLanChannel(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "Channel", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPWLanChannel(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 channel = 0;
	int templateType;

	channel = *((uint32*)value);
	sprintf(tmp,"%d",channel);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name,"Channel", tmp);
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"Channel");
	return 0;
}

int8 getAPWLanChannelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanChannelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPWLanSSID(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "SSID", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPWLanSSID(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	int templateType;

	tmp = (char*)value;
	if(strlen(tmp) >= 32){
		tcdbg_printf("%s: length is more than 32, error!!!\n",__FUNCTION__);	
		return -1;
	}
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "SSID", tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"SSID");
	return 0;
}

int8 getAPWLanSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPWLanBeaconType(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "BeaconType", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPWLanBeaconType(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	int templateType;

	tmp = (char*)value;
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "BeaconType", tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"BeaconType");
	return 0;
}

int8 getAPWLanBeaconTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanBeaconTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getMACAddressControlEnabled(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	char vEnable[4] = {0};	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "MACAddressControlEnable", tmp);
	if(strcmp(tmp, "Yes") == 0 || strcmp(tmp, "1") == 0 || strcmp(tmp, "true") == 0)
		strcpy(vEnable, "1"); 
	else
		strcpy(vEnable, "0"); 
		
	return getTheValue(value, type, deviceNode, vEnable);
}

int8 setMACAddressControlEnabled(void* value, struct DeviceNode*  deviceNode)
{		
	char tmp[8];
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	boolean* enable;
	int templateType;

	enable = (boolean *)value;
	if( *enable == (boolean)0)
		sprintf(tmp, "%s", "0");
	else if( *enable == (boolean)1 ) 
		sprintf(tmp, "%s", "1");
	else 
		return -1;

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "MACAddressControlEnable", tmp);

	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"MACAddressControlEnable");
	return 0;
}

int8 getMACAddressControlEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setMACAddressControlEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}


int8 getAPWLanSSIDHide(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	char vEnable[4] = {0};	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"SSIDHide", tmp);
	if(strcmp(tmp, "Yes") == 0 || strcmp(tmp, "1") == 0 || strcmp(tmp, "true") == 0)
		strcpy(vEnable, "1"); 
	else
		strcpy(vEnable, "0"); 
		
	return getTheValue(value, type, deviceNode, vEnable);
}

int8 setAPWLanSSIDHide(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[8];
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	boolean* enable;
	int templateType;

	enable = (boolean *)value;
	if( *enable == (boolean)0)
		sprintf(tmp, "%s", "0");
	else if( *enable == (boolean)1 ) 
		sprintf(tmp, "%s", "1");
	else 
		return -1;

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"SSIDHide", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"SSIDHide");
	return 0;
}

int8 getAPWLanSSIDHideAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanSSIDHideAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}


int8 getAPWLanPowerlevel(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"Powerlevel", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPWLanPowerlevel(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 powerlevel= 0;
	int templateType;

	powerlevel = *((uint32*)value);
	sprintf(tmp,"%d",powerlevel);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"Powerlevel", tmp);

	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"Powerlevel");
	return 0;
}

int8 getAPWLanPowerlevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanPowerlevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPWLanPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"PowerValue", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPWLanPowerValue(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 powerlevel= 0;
	int templateType;

	powerlevel = *((uint32*)value);
	sprintf(tmp,"%d",powerlevel);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"PowerValue", tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"PowerValue");
	return 0;
}

int8 getAPWLanPowerValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanPowerValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}


int8 getAPModuleEnable(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	char vEnable[4] = {0};	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"APModuleEnable", tmp);
	if(strcmp(tmp, "Yes") == 0 || strcmp(tmp, "1") == 0 || strcmp(tmp, "true") == 0)
		strcpy(vEnable, "1"); 
	else
		strcpy(vEnable, "0"); 
		
	return getTheValue(value, type, deviceNode, vEnable);
}

int8 setAPModuleEnable(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[8];
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	boolean* enable;
	int templateType;

	enable = (boolean *)value;
	if( *enable == (boolean)0)
		sprintf(tmp, "%s", "0");
	else if( *enable == (boolean)1 ) 
		sprintf(tmp, "%s", "1");
	else 
		return -1;

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"APModuleEnable", tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"APModuleEnable");
	return 0;
}

int8 getAPModuleEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPModuleEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getWPSKeyWord(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"WPSKeyWord", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setWPSKeyWord(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 powerlevel= 0;
	int templateType;

	powerlevel = *((uint32*)value);
	sprintf(tmp,"%d",powerlevel);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"WPSKeyWord", tmp);

	
	serviceProfileNodeModifyAdd(templateType, apWLanIdx-1, X_CT_COM"WPSKeyWord");
	return 0;
}

int8 getWPSKeyWordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setWPSKeyWordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPWLanStandard(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "Standard", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPWLanStandard(void* value, struct DeviceNode*  deviceNode)
{		
	char* tmp = NULL;
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	int templateType;

	tmp = (char *)value;
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "Standard", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"Standard");
	return 0;
}

int8 getAPWLanStandardAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWLanStandardAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getWEPEncryptionLevel(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "WEPEncryptionLevel", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setWEPEncryptionLevel(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	int templateType;
	
	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 64, error!!!\n",__FUNCTION__);	
		return -1;
	}
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "WEPEncryptionLevel", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"WEPEncryptionLevel");
	return 0;
}

int8 getWEPEncryptionLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setWEPEncryptionLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getWEPKeyIndex(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "WEPKeyIndex", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setWEPKeyIndex(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 keyIndex= 0;
	int templateType;

	keyIndex = *((uint32*)value);
	sprintf(tmp,"%d",keyIndex);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "WEPKeyIndex", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"WEPKeyIndex");
	return 0;
}

int8 getWEPKeyIndexAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setWEPKeyIndexAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getBasicAuthenticationMode(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[64] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "BasicAuthenticationMode", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setBasicAuthenticationMode(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	int templateType;

	tmp = (char*)value;
	if(strlen(tmp) >= 31){
		tcdbg_printf("%s: length is more than 31, error!!!\n",__FUNCTION__);	
		return -1;
	}
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "BasicAuthenticationMode", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"BasicAuthenticationMode");
	return 0;
}

int8 getBasicAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setBasicAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getWPAEncryptionModes(char** value, ParameterType* type, DeviceNode*  deviceNode)
{	
	char tmp[64] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, "WPAEncryptionModes", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);	
}

int8 setWPAEncryptionModes(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	int templateType;

	tmp = (char*)value;
	if(strlen(tmp) >= 31){
		tcdbg_printf("%s: length is more than 31, error!!!\n",__FUNCTION__);	
		return -1;
	}
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, "WPAEncryptionModes", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,"WPAEncryptionModes");
	return 0;
}

int8 getWPAEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setWPAEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getCTComRFBand(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	char rfBand[4] = {0};	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"RFBand", tmp);
	return getTheValue(value, type, deviceNode, rfBand);
}

int8 setCTComRFBand(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[8];
	char node_name[32] = {0};
	int apWLanIdx = 0;	
	uint32 rfBand;
	int templateType;

	//0:2.4GHz; 1:5.8GHz; default is 0
	rfBand = *((uint32*)value);
	sprintf(tmp,"%d",rfBand);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"RFBand", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"RFBand");
	return 0;
}

int8 getCTComRFBandAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setCTComRFBandAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getCTComChannelWidth(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"ChannelWidth", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);		
}

int8 setCTComChannelWidth(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 channelWidth= 0;
	int templateType;

	//0:20MHz; 1:40MHz; 2:20/40Hz
	channelWidth =  *((uint32*)value);
	sprintf(tmp,"%d",channelWidth);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"ChannelWidth", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"ChannelWidth");
	return 0;
}

int8 getCTComChannelWidthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setCTComChannelWidthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getCTComGuardInterval(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"GuardInterval", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);		
}

int8 setCTComGuardInterval(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[8] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 guardInterval= 0;
	int templateType;

	//0:400ns; 1:800ns
	guardInterval =  *((uint32*)value);
	sprintf(tmp,"%d",guardInterval);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"GuardInterval", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"GuardInterval");
	return 0;
}

int8 getCTComGuardIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setCTComGuardIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getCTComRetryTimeout(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"RetryTimeout", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);		
}

int8 setCTComRetryTimeout(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	uint32 iRetryTimeout= 0;
	int templateType;

	iRetryTimeout = *((uint32*)value);
	sprintf(tmp,"%d",iRetryTimeout);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"RetryTimeout", tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"RetryTimeout");
	return 0;
}

int8 getCTComRetryTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setCTComRetryTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getCTComVLAN(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;	

	apWLanIdx = atoi(deviceNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);
	tcapi_get(node_name, X_CT_COM"VLAN", tmp);
		
	return getTheValue(value, type, deviceNode, tmp);		
}

int8 setCTComVLAN(void* value, struct DeviceNode*  deviceNode)
{	
	char tmp[32] = {0};
	char node_name[32] = {0};
	int apWLanIdx = 0;		
	int vlan = 0;
	int templateType;

	vlan = *(int *)value;
	sprintf(tmp,"%d",vlan);
	apWLanIdx = atoi(deviceNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 2, apWLanIdx, node_name);	
	tcapi_set(node_name, X_CT_COM"VLAN", tmp);

	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,X_CT_COM"VLAN");
	return 0;
}

int8 getCTComVLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setCTComVLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPWEPKey(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[128] = {0};
	char node_name[32] = {0};	
	char node_attr[32] = {0};
	int apWLanIdx = 0;	
	int wepKeyIdx = 0;	
	
	wepKeyIdx = atoi(deviceNode->parentNode->name);	
	apWLanIdx = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	getCfgNodenameByDeviceNode(deviceNode, 4, apWLanIdx, node_name);
	sprintf(node_attr, "WEPKey%d", wepKeyIdx);		
	tcapi_get(node_name, node_attr, tmp);		
	return getTheValue(value, type, deviceNode, tmp);		
}

int8 setAPWEPKey(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	char node_attr[32] = {0};
	int apWLanIdx = 0;			
	int wepKeyIdx = 0;	
	int templateType;

	tmp = (char*)value;
	if(strlen(tmp) >= 128){
		tcdbg_printf("%s: length is more than 128, error!!!\n",__FUNCTION__);	
		return -1;
	}
	wepKeyIdx = atoi(deviceNode->parentNode->name);	
	apWLanIdx = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 4, apWLanIdx, node_name);	
	sprintf(node_attr, "WEPKey%d", wepKeyIdx);		
	tcapi_set(node_name, node_attr, tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,node_attr);
	return 0;
}

int8 getAPWEPKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPWEPKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPAssociatedDeviceMACAddress(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return 0;
}


int8 getAPAssociatedDeviceMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPAssociatedDeviceMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPAssociatedDeviceIPAddress(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return 0;
}


int8 getAPAssociatedDeviceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPAssociatedDeviceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPAssociatedDeviceAuthenticationState(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return 0;
}


int8 getAPAssociatedDeviceAuthenticationStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPAssociatedDeviceAuthenticationStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPPreSharedKey(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if 0
	/*it should return empty according to spe TR098*/
	char tempPreSharedKey[64];

	memset(tempPreSharedKey, 0, sizeof(tempPreSharedKey));
	return getTheValue(value, type, deviceNode, tempPreSharedKey);
#else
	char tmp[128] = {0};
	char node_name[32] = {0};	
	char node_attr[32] = {0};
	int apWLanIdx = 0;	
	int preSharedKeyIdx = 0;	

	preSharedKeyIdx = atoi(deviceNode->parentNode->name); 
	apWLanIdx = atoi(deviceNode->parentNode->parentNode->parentNode->name); 
	getCfgNodenameByDeviceNode(deviceNode, 4, apWLanIdx, node_name);
	sprintf(node_attr, "WPAPSK%d", preSharedKeyIdx);		
	tcapi_get(node_name, node_attr, tmp);		
	return getTheValue(value, type, deviceNode, tmp);
#endif
}

int8 setAPPreSharedKey(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	char node_attr[32] = {0};
	int apWLanIdx = 0;			
	int preSharedKeyIdx = 0;		
	int templateType;

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 128, error!!!\n",__FUNCTION__);	
		return -1;
	}
	preSharedKeyIdx = atoi(deviceNode->parentNode->name);	
	apWLanIdx = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 4, apWLanIdx, node_name);	
	sprintf(node_attr, "WPAPSK%d", preSharedKeyIdx);		
	tcapi_set(node_name, node_attr, tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,node_attr);
	return 0;
}

int8 getAPPreSharedKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPPreSharedKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getAPPreKeyPassphrase(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmp[128] = {0};
	char node_name[32] = {0};	
	char node_attr[32] = {0};
	int apWLanIdx = 0;	
	int preSharedKeyIdx = 0;	

	preSharedKeyIdx = atoi(deviceNode->parentNode->name); 
	apWLanIdx = atoi(deviceNode->parentNode->parentNode->parentNode->name); 
	getCfgNodenameByDeviceNode(deviceNode, 4, apWLanIdx, node_name);
	sprintf(node_attr, "KeyPassphrase%d", preSharedKeyIdx);		
	tcapi_get(node_name, node_attr, tmp);	
	return getTheValue(value, type, deviceNode, tmp);
}

int8 setAPPreKeyPassphrase(void* value, struct DeviceNode*  deviceNode)
{	
	char* tmp = NULL;
	char node_name[32] = {0};
	char node_attr[32] = {0};
	int apWLanIdx = 0;			
	int preSharedKeyIdx = 0;		
	int templateType;

	tmp = (char*)value;
	if(strlen(tmp) >= 64){
		tcdbg_printf("%s: length is more than 128, error!!!\n",__FUNCTION__);	
		return -1;
	}
	preSharedKeyIdx = atoi(deviceNode->parentNode->name);	
	apWLanIdx = atoi(deviceNode->parentNode->parentNode->parentNode->name);	
	templateType = getCfgNodenameByDeviceNode(deviceNode, 4, apWLanIdx, node_name);	
	sprintf(node_attr, "KeyPassphrase%d", preSharedKeyIdx);		
	tcapi_set(node_name, node_attr, tmp);
	
	serviceProfileNodeModifyAdd(templateType,apWLanIdx-1,node_attr);
	return 0;
}

int8 getAPPreKeyPassphraseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	return 0;
}
int8 setAPPreKeyPassphraseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	return 0;
}
