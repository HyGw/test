#include "Global_res.h"
#include "cp_IPPingDiagnostics.h"
#include <netdb.h>

static char  IPPingDiagnosticsStateType[32] = "None";
static char ipPingDomainName[MAX_DOMAIN_LENGTH];
static char ipPingconvertIP[64]={0};
static int ipPingIPver = -1;
static uint32 NumberOfRepetitions = 4;//add the default value for ip ping diagnostic,brian
static uint32 DSCPValue = 0;
char  gIPPingInterface[128] = {0};
#if 1 //IPPING_DIAGNOSTICS
char  gIPPingDev[15] =  "NoDevice";
#else
char  gIPPingDev[15] = {0};
#endif
#ifndef THREAD_REDUCE
sem_t CWMP_PING;
#endif
pthread_t p_ping = 0;
ping_t cwmp_ping_p;
#if defined(TCSUPPORT_CT_PON_GDV20)
static char ipPingconvertIPv6[64]={0};
static char  IPPingDiagnosticsStateTypeV20[32] = "None";
#endif

extern int connection_req_flag;

extern int tc_getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result, int timeout);

int8 getDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, IPPingDiagnosticsStateType);
}

int8 setDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	char vtype[32];
	int result = -1 ;
	char targetIPAddr[128];

	if(strlen((char*)value) > 31)
		return -1;
	
	strcpy(vtype,(char*)value);		
	
	if ( strcmp(vtype,"Requested") ==0 ){
		//need to do in the future!
#ifdef THREAD_REDUCE		
		resetCwmpPing();//If the ping test is in progress, terminate it first.
#endif		
		rpc.flags.theIPPingFlag |= CWMP_IPPING_ACTIVE;
		strcpy(IPPingDiagnosticsStateType,"Requested");
#if defined(TCSUPPORT_CT_PON_GDV20)
		strcpy(IPPingDiagnosticsStateTypeV20,"Requested");
#endif
	}
	else {
		//dbg_pline_1("\r\nIPPingDiagnosticsStateType parameter is not Requested!");
		return -1;
	}

	return 0;
}

int8 getDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingDiagnosticsState;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingDiagnosticsState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getIPPingInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, gIPPingInterface);
}
#if defined(TCSUPPORT_CT_FJ)
int GetWanDevName(char *psetvalue)
{
	int tolallen=0, WanIndex=0;	
	char wannodeName[34] = {'\0'};
	char str_valid_if[128] ={'\0'};
	char isp[4] = {0},active[4] = {0};
	uint8 ispvalue = 0;
	char temp[64] ={'\0'};
	char setvalue[10] = {'\0'};
	char *p = NULL, *saveptr = NULL;

	if(psetvalue == NULL) goto fail;
	strcpy(gIPPingDev,"NoDevice");
	if(tcapi_get("WanInfo_Common","ValidIFIndex",str_valid_if)){
		goto fail;
	}

	if(!strcmp(psetvalue,"VOIP")){
		strncpy(setvalue, "VOICE", sizeof(setvalue));
	}
	else {
		strncpy(setvalue, psetvalue, sizeof(setvalue));
	}
	saveptr = str_valid_if;	
	while((p = strtok_r(NULL,",",&saveptr)) != NULL){
		WanIndex = atoi(p);
		sprintf(wannodeName, "Wan_PVC%d_Entry%d", WanIndex/8, WanIndex%8);
		if(tcapi_get(wannodeName, "ServiceList", temp)){
			tc_printf("getWanName:get ServiceList error!\n");
			goto fail;
		}
		
		if(strstr(temp,setvalue)){	
			tcapi_get(wannodeName,"Active",active);
			if(!strcmp(active,"Yes"))
			{	
				tcapi_get(wannodeName,"ISP",isp);
				ispvalue = atoi(isp);
				if(ispvalue == 2){//pppoe dial up
					sprintf(gIPPingDev,"ppp%d",WanIndex);
					return 0;
				}
				else if(ispvalue == 0 || ispvalue == 1){
					sprintf(gIPPingDev,"nas%d_%d", WanIndex/8, WanIndex%8);
					return 0;
				}
			}
			
		}
	}
	
fail:
	tc_printf("getWanName:return error!\n");
	return -1;	
}
#endif

int8 setIPPingInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	char pvcgateway[32];
	char DefaultIface_WAN[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char DefaultIface_LAN[]="InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1";
	char tempvalue[2];
	int status; 
	int len;
	char *setvalue;
	uint32 pvcnode;	
	DeviceNodePtr Node = NULL;
	char temp[32];
	char nodeName[32] = {0};
	char isp[4] = {0},active[4] = {0};
	uint8 ispvalue = 0;
#if 1 //IPPING_DIAGNOSTICS
	uint32 entrynode;
	int pppIndex;
	char *tmp = NULL;
#endif
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;
#endif
	
#ifdef THREAD_REDUCE	
	resetCwmpPing();//If the ping test is in progress, terminate it first.
#endif
	
	setvalue=(char*)value;
#if defined(TCSUPPORT_CT_FJ)
	if(!GetWanDevName(setvalue)){
		strcpy(gIPPingInterface, setvalue);//set the interface.
		return 0;
	}
#endif
	len = strlen(DefaultIface_WAN);

	if(strncmp(DefaultIface_WAN, setvalue, len) == 0){//WAN interface, get the gateway.
		/*check the value is valid or not.*/
		cwmpSearchDeviceNodeByName(setvalue, rpc.IGDDeviceRootPtr, &Node);
		if(Node == NULL){
			tc_printf("\r\nparameter.c---[setIPPingInterfaceValue]---Invalid Value!");
			return -1;
		}
		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
		
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		pvcnode = atoi(tempvalue);
		pvctmp = pvcnode;
		if(FindPVCMapping(&pvcnode) != 0){
			tc_printf("\r\nparameter.c---[setIPPingInterfaceValue]---Invalid pvc number!");
			return -1;
		}
#else
		if(setvalue[len]<'1'||setvalue[len]>'8'){
			tc_printf("\r\nparameter.c---[setIPPingInterfaceValue]---Invalid pvc number!");
			return -1;
		}
		pvcnode = atoi(tempvalue) - 1;
#endif

#if 1 //IPPING_DIAGNOSTICS
		strcpy(gIPPingDev,"NoDevice");

		if((tmp = strstr(setvalue, "WANPPPConnection")) != NULL){
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANPPPConnection") + 1);
			tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
			entrynode = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entrynode);
#else	
			entrynode = atoi(tempvalue) - 1;
#endif
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entrynode);
			tcapi_get(nodeName,"Active",active);
			if(!strcmp(active,"Yes"))
			{	
				tcapi_get(nodeName,"ISP",isp);
				ispvalue = atoi(isp);
				if(ispvalue == 2){//pppoe dial up
					pppIndex = pvcnode * 8 + entrynode;
					sprintf(gIPPingDev,"ppp%d",pppIndex);
				}
			}
		}
		else if((tmp = strstr(setvalue, "WANIPConnection")) != NULL){
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANIPConnection") + 1);
			tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
			entrynode = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entrynode);
#else	
			entrynode = atoi(tempvalue) - 1;
#endif
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entrynode);
			tcapi_get(nodeName,"Active",active);
			if(!strcmp(active,"Yes"))
			{	
				tcapi_get(nodeName,"ISP",isp);
				ispvalue = atoi(isp);
				if(ispvalue == 0 || ispvalue == 1){
					sprintf(gIPPingDev,"nas%d_%d", pvcnode, entrynode);
				}
			}
		}
#else
		sprintf(nodeName,"Wan_PVC%d",pvcnode);
		tcapi_get(nodeName,"ISP",isp);
		tcapi_get(nodeName,"Active",active);
		ispvalue = atoi(isp);
		if(!strcmp(active,"Yes"))
		{
			if(ispvalue == 2)//pppoe dial up
				sprintf(gIPPingDev,"ppp%d",pvcnode);
			else if(ispvalue == 0 || ispvalue == 1)
				sprintf(gIPPingDev,"nas%d",pvcnode);
			else
				strcpy(gIPPingDev,"NoDevice");
		}
		else
		{
			strcpy(gIPPingDev,"NoDevice");
		}		
#endif
	}
	else if(strcmp(setvalue,DefaultIface_LAN) == 0){//LAN interface, get the gateway.
		strcpy(gIPPingDev,"br0");		
	}else if(strcmp(setvalue, "") == 0)
	{//empty string
		strcpy(gIPPingInterface, "");		
		//get default interface(move to ping task)
		strcpy(gIPPingDev,"");
		return 0;
		
	}else{//error value
		return -1;
	}

	strcpy(gIPPingInterface, setvalue);//set the interface.

	return 0;
}

int8 getIPPingInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingInterface;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPPingInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getIPPingHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	//char* tempValue;
	//char	IPPingHost[32];

	//sprintf(IPPingHost,"%s",inet_ntoa(cwmp_ping_p.target));
	tc_printf("\r\nthe IPPingHost is %s",ipPingDomainName);
	return getTheValue(value, type, deviceNode, ipPingDomainName);
}
int8 setIPPingHostValue(void* value, struct DeviceNode*  deviceNode)
{
#if 0
	char vtype[256];
	ip4a dnsQueryResult;
	
	if(strlen((char*)value) > 255)
		return -1;
	
	if(strcmp((char*)value, "255.255.255.255") == 0){
		return -1;
	}
	
	//resetCwmpPing();//If the ping test is in progress, terminate it first.
	
	strcpy(vtype,(char*)value);	
	dnsQueryResult = resolveName(vtype);
	if(!dnsQueryResult)
	{
		tc_printf("setIPPingHostValue unknown!\n");
		return -1;
	}
	if ( resolve( inet_ntoa(dnsQueryResult), &cwmp_ping_p.target, NULL) != 0 )
	{
		tc_printf("\r\nparameter.c---[setIPPingHostValue]--- setIPPingHostValue unknown!\n");
		return -1;
	}

#endif
	int valueLen = 0;
	valueLen = strlen((char*)value);
	
	if((valueLen > 255) || (valueLen == 0))
		return -1;
		
#ifdef THREAD_REDUCE	
	resetCwmpPing();
#endif
#if 0
	if(ipPingDomainName == NULL)
	{
		ipPingDomainName = (char*)os_malloc(sizeof(char)*MAX_DOMAIN_LENGTH);
		if(ipPingDomainName == NULL)
		{
			tc_printf("\r\nos_malloc failure");
			return -1;
		}
	}
#endif
	memset(ipPingDomainName,0,sizeof(char)*MAX_DOMAIN_LENGTH);
	strcpy(ipPingDomainName,(char*)value);
	
	return 0;
}
int8 getIPPingHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingHost;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPPingHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingHost;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getNumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  vNumberOfRepetitions[4];
	
	sprintf(vNumberOfRepetitions,"%lu",NumberOfRepetitions);	
	return getTheValue(value, type, deviceNode, vNumberOfRepetitions);
}

int8 setNumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 num = 0;

	num = *((uint32*)value);
	if(num == 0){//Can not set NumberOfRepetitions to 0. 
		return -1;
	}

	NumberOfRepetitions = num;	
#ifdef THREAD_REDUCE	
	resetCwmpPing();//If the ping test is in progress, terminate it first.
#endif	
	return 0;
}

int8 getNumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingNumberOfRepetitions;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setNumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingNumberOfRepetitions;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getIPPingTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  vIPPingTimeout[10];

	if(cwmp_ping_p.interval < 1) 
		strcpy(vIPPingTimeout, "10");//now default timeout is 10
	else
		sprintf(vIPPingTimeout, "%lu",cwmp_ping_p.interval);

	return getTheValue(value, type, deviceNode, vIPPingTimeout);
}

int8 setIPPingTimeoutValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 num = 0;

	num = *((uint32*)value);
	if(num == 0){//Can not set Timeout to 0. 
		tc_printf("setIPPingTimeoutValue set value is wrong!\r\n");
		return -1;
	}
	cwmp_ping_p.interval = num;

#ifdef THREAD_REDUCE	
	resetCwmpPing();//If the ping test is in progress, terminate it first.
#endif
	return 0;
}

int8 getIPPingTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingTimeout;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPPingTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingTimeout;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getDataBlockSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  vDataBlockSize[10];

	if(cwmp_ping_p.len < 1) 
		strcpy(vDataBlockSize, "56");//now dedault len is 56
	else
		sprintf(vDataBlockSize, "%u", cwmp_ping_p.len);
	return getTheValue(value, type, deviceNode, vDataBlockSize);
}

int8 setDataBlockSizeValue(void* value, struct DeviceNode*  deviceNode)
{
	if((*((uint32*)value)) < 1 || (*((uint32*)value)) > 65535)
		return -1;
		
	cwmp_ping_p.len= *((uint32*)value);
#ifdef THREAD_REDUCE	
	resetCwmpPing();//If the ping test is in progress, terminate it first.
#endif
	return 0;
}

int8 getDataBlockSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingDataBlockSize;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDataBlockSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingDataBlockSize;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  vDSCPValue[10];
	
	sprintf(vDSCPValue, "%lu",DSCPValue);
	
	return getTheValue(value, type, deviceNode, vDSCPValue);
}

int8 setDSCPValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue = 0;
	
	tempValue = *(uint32*)value;
	if(tempValue > 63)
		return -1;

	DSCPValue = tempValue;	
	cwmp_ping_p.dscp= DSCPValue;
	
#ifdef THREAD_REDUCE	
	resetCwmpPing();//If the ping test is in progress, terminate it first.
#endif	
	return 0;
}

int8 getDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingDSCP;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingDSCP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getSuccessCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* retValue;
	char tempValue[4];
	unsigned int SuccessCount;

	SuccessCount = cwmp_ping_p.responses; 
	
	sprintf(tempValue,"%u",SuccessCount);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getSuccessCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingSuccessCount;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSuccessCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingSuccessCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getFailureCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* retValue;
	char tempValue[4];
	unsigned int FailureCount;

	FailureCount = cwmp_ping_p.sent - cwmp_ping_p.responses; 
	
	sprintf(tempValue,"%u",FailureCount);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getFailureCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingFilureCount;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFailureCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingFilureCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getAverageResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* retValue;
	char tempValue[10];
	unsigned int AverageResponseTime;

	AverageResponseTime = cwmp_ping_p.avrrtt; 
	
	sprintf(tempValue,"%u",AverageResponseTime);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getAverageResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingAverageResponseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setAverageResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingAverageResponseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getMinimumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* retValue;
	char tempValue[10];
	unsigned int MinimumResponseTime;

	MinimumResponseTime = cwmp_ping_p.minrtt; 
	
	sprintf(tempValue,"%u",MinimumResponseTime);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getMinimumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingMinimumResponseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMinimumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingMinimumResponseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getMaximumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* retValue;
	char tempValue[10];
	unsigned int MaximumResponseTime;

	MaximumResponseTime = cwmp_ping_p.maxrtt; 
	
	sprintf(tempValue,"%u",MaximumResponseTime);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getMaximumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpPingDiagnosticAttrAttr.aIpPingMaxumumResponseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMaximumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpPingDiagnosticAttrAttr.aIpPingMaxumumResponseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}
#ifdef THREAD_REDUCE
#define CWMP_PING_PID_PATH "/tmp/cwmp/tr69diag_ping.pid"

static void cwmpStopPing(void)
{
	FILE *fp = NULL;
	char cmdbuf[128] = {0};
	char ping_pid_tmp[100] = {0};
	int ping_pid = 0;
	fp = fopen(CWMP_PING_PID_PATH, "r");
#if 0  //IPPING_DIAGNOSTICS
	if(fp == NULL)
	{
		/*get ping process pid*/
		tc_printf("\r\ncwmpStopPing: can not get pid from file, we need use pidof to find ping pid\n");
		sprintf(cmdbuf, "pidof %s > %s", "ping", CWMP_PING_PID_PATH);
		system(cmdbuf);  /*get the PID of old diagnostic process if exist*/
		fp = fopen(CWMP_PING_PID_PATH, "r");
	}
#endif	
	
	if(fp == NULL){
		return;
	}
	
	fgets(ping_pid_tmp, sizeof(ping_pid_tmp), fp);
	fclose(fp);
	sprintf(cmdbuf, "rm -f %s", CWMP_PING_PID_PATH);
	system(cmdbuf);
	ping_pid = atoi(ping_pid_tmp);
	if(ping_pid != 0){  /*if pid != 0, that's mean old ping diagnostic process already exist*/
		sprintf(cmdbuf,"kill -9 %d", ping_pid);  /*remove old ping diagnostic process*/
		tc_printf("CWMP Ping Diagnostic process down, pid is [%d]\n", ping_pid);
	    system(cmdbuf);
	}
}

static void  resetCwmpPing(void)
{
	strcpy(IPPingDiagnosticsStateType,"None");
#if defined(TCSUPPORT_CT_PON_GDV20)
	strcpy(IPPingDiagnosticsStateTypeV20,"None");
#endif
	
	if(rpc.flags.theIPPingFlag & CWMP_IPPING_INPROGRESS){
		cwmpStopPing();
		tc_printf("\r\n resetCwmpPing-----------stop ping progress!!!");		
		//cwmpIPPingRTDrop();		
		rpc.flags.theIPPingFlag &= ~CWMP_IPPING_INPROGRESS;
		rpc.flags.theIPPingFlag |= CWMP_IPPING_TERMINATED; //The ping test is terminated.
	}
	return;
}
#endif

//add by brian 
/**************************ip ping diagnostic begin***********************************/
static void resetCWMPPingResult(void)
{
	cwmp_ping_p.sent = 0;
	cwmp_ping_p.responses = 0;
	cwmp_ping_p.minrtt = 0;
	cwmp_ping_p.maxrtt = 0;
	cwmp_ping_p.avrrtt = 0;
	return;
}

static void doCwmpIPPingError(void)
{
	cwmp_ping_p.sent = NumberOfRepetitions;
	cwmp_ping_p.responses = 0;
	cwmp_ping_p.minrtt = 0;
	cwmp_ping_p.maxrtt = 0;
	cwmp_ping_p.avrrtt = 0;
}

void getCWMPPingResult(void)
{
	FILE *fp = NULL;
	if((rpc.flags.theIPPingFlag & CWMP_IPPING_INPROGRESS) == 0){//this is not cwmp ip ping, do not get the result.
		return;
	}
	
	fp = fopen(IPPINGDIAGNOSTICFILE,"r");
	if(fp != NULL)
	{
		fscanf(fp,"%ld,%ld,%lu,%lu,%lu",&cwmp_ping_p.sent,&cwmp_ping_p.responses,&cwmp_ping_p.minrtt,&cwmp_ping_p.avrrtt,&cwmp_ping_p.maxrtt);
		fclose(fp);
	}
	unlink(IPPINGDIAGNOSTICFILE);
	
	tc_printf("\r\nthe send is %ld",cwmp_ping_p.sent);
	tc_printf("\r\nthe responses is %ld",cwmp_ping_p.responses);
	tc_printf("\r\nthe minrtt is %ld",cwmp_ping_p.minrtt);
	tc_printf("\r\nthe avrrtt is %ld",cwmp_ping_p.avrrtt);
	tc_printf("\r\nthe maxrtt is %ld",cwmp_ping_p.maxrtt);
	
	return;
}

int wan_ipVersion = 1;

#ifndef THREAD_REDUCE
void *cwmpPingTask(void)
{

	int resolveHostError = 0;
	int rtAddStatus = 0;
	int result = -1;
	int mallocError = 0;//malloc for read ping diagnostic
	int isRTFlag = 0;//0:IPPing rule is not in route table; 1:in route table; -1:IPPing rule error.
	char cmdbuf[200] = {'\0'};
	char sub_mask[20] = "255.255.255.255";
	char ipaddr[20] = {0};
	char *tmpbuf = NULL;
	char path[] ="/tmp/cwmp/ippingdiagnostic_stat";
	while(1)
	{
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\n Execute cwmpPingTask... ");
		}
		
		//judge if resolve host name error!
		if(cwmp_ping_p.target == 0)//resolve host name error
		{
			resolveHostError = -1;
		}
		else//judge if in route table
		{
			if(strcmp(gIPPingDev, "NoDevice") == 0)
			{		
				isRTFlag = -1;
				
			}else if(strcmp(gIPPingDev, "") == 0)
			{// default gateway
				isRTFlag = 1;// in route table
			}
			else
			{
				//isRTFlag = BeInRTTable(cwmp_ping_p.target, gIPPingDev);
				isRTFlag = 1;//need to implement in the future
			}
		}
		
		//if not in route table ,add it------use in the future!!
		strcpy(ipaddr,(char*)inet_ntoa(cwmp_ping_p.target));
		if((isRTFlag == 0) && (resolveHostError != -1))// not in route table then add it.
		{
			sprintf(cmdbuf,"route add -net %s netmask %s dev %s",ipaddr,sub_mask,gIPPingDev);
			rtAddStatus = system(cmdbuf);
		}
		//malloc buf for read file
		tmpbuf = malloc(MAX_PINGBUF_LEN);
		if(tmpbuf == NULL)
		{
			tc_printf("\r\n[cwmp ping task]mallco failed!");
			mallocError == -1;
		}
	
		resetCWMPPingResult();//reset the Ping Result before ping.
		rpc.flags.theIPPingFlag &= ~CWMP_IPPING_TERMINATED; 
		rpc.flags.theIPPingFlag |= CWMP_IPPING_INPROGRESS;//The cwmp IP Ping is in progress.
			
		//adding route fail or illegal route for adding or resolve host name error
		memset(cmdbuf,0,sizeof(cmdbuf));
		if((rtAddStatus < 0) || (isRTFlag == -1) || (resolveHostError == -1) || (mallocError == -1))
		{
			tc_printf("\r\nenter ip ping error!");
			doCwmpIPPingError();
		}else
		{
			//do ping 
		#if 1
			if((cwmp_ping_p.interval == 0) && (cwmp_ping_p.len == 0))
				sprintf(cmdbuf,"ping -q -M 1 -c %lu -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.dscp,ipaddr,path);
			else if((cwmp_ping_p.interval == 0) && (cwmp_ping_p.len != 0))	
				sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.len,cwmp_ping_p.dscp,ipaddr,path);
			else if((cwmp_ping_p.interval != 0) && (cwmp_ping_p.len == 0))
				sprintf(cmdbuf,"ping -q -M 1 -c %d -W %lu -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.interval,cwmp_ping_p.dscp,ipaddr,path);
			else
				sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -W %lu -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.len,cwmp_ping_p.interval,cwmp_ping_p.dscp,ipaddr,path);
			//exec ping
		#else
			strcpy(cmdbuf,"ping -q -M 1 -c 3 -s 32 -W 5000 192.168.1.12 > /tmp/cwmp/ippingdiagnostic_stat");//for test
		#endif
			system(cmdbuf);
			//read the ping result
			fileRead(IPPINGDIAGNOSTICFILE,tmpbuf,MAX_PINGBUF_LEN);
			
			if(strstr(tmpbuf,IMCOMPLETEFLAG))
			{
				result = -1;
			}
			else
			{
				result = 0;
				getCWMPPingResult();
			}
        }
		
		rpc.flags.theIPPingFlag &= ~CWMP_IPPING_INPROGRESS;
		
		if(mallocError != -1)
			free(tmpbuf);
		
		if(dbg_cwmp >= DBG_CWMP_L2 )
		{
			tc_printf("\r\ncwmpParameter.c-----[cwmpPingTask]----  result =", result);
		}
		if (!result)
		{
			strcpy(IPPingDiagnosticsStateType,"Complete");
		}else
		{
			if((rtAddStatus != 0) || (isRTFlag == -1) ||(mallocError == -1))
			{
				strcpy(IPPingDiagnosticsStateType,"Error_Internal");
			}
			else if(resolveHostError == -1)
			{
			strcpy(IPPingDiagnosticsStateType,"Error_CannotResolveHostName");
			}
			else
			{
				strcpy(IPPingDiagnosticsStateType,"Error_Other");
			}
			cwmp_ping_p.sent = NumberOfRepetitions;
		}
		
		//reset all flag.
		rtAddStatus = 0;
		result = -1;
		isRTFlag = 0;
		resolveHostError = 0;
	 	mallocError = 0;
		
		if(rpc.flags.theIPPingFlag & CWMP_IPPING_TERMINATED){//If the ping test is terminated unusally, does not send inform.
			rpc.flags.theIPPingFlag &= ~CWMP_IPPING_TERMINATED; 
		}else{
			/*Send the "8 DIAGNOSTICS COMPLETE" Event code*/
			//delete the route-----modify in the future
			//sprintf(cmdbuf,"route delete -net %s netmask %s dev %s",ipaddr,sub_mask,gIPPingDev);
			//rtAddStatus = system(cmdbuf);
			setIPPingCompleteFlag(1);	
#if defined(TCSUPPORT_CT_MIDWARE)
			setOperationFlag(1);
#endif

			sem_post(&SEM_IMFORM);
		}
		
		sem_wait(&CWMP_PING);
	}
	
}
#else
void *cwmpPingTask(void)
{

	int resolveHostError = 0;
	int rtAddStatus = 0;
	int result = -1;
	int mallocError = 0;//malloc for read ping diagnostic
	int isRTFlag = 0;//0:IPPing rule is not in route table; 1:in route table; -1:IPPing rule error.
	char cmdbuf[200] = {'\0'};
	char sub_mask[20] = "255.255.255.255";
	char ipaddr[65] = {0};
	char *tmpbuf = NULL;
	char path[] ="/tmp/cwmp/ippingdiagnostic_stat";
#if 1   //IPPING_DIAGNOSTICS
	int if_index;
	int findIndexError = 0;//malloc for read ping diagnostic
	char nodeName[33] = {'\0'};
	char cmd_rmRoute[200] = {'\0'};
	char gateWay[64];
#endif
	char devName[64] = {0};
	char ping_param[100] = {0};
#define ROUTE6_ADD_CMD  "ip -6 route add %s via %s dev %s"
#define ROUTE6_DEL_CMD  "ip -6 route del %s via %s dev %s"
#define ROUTE6BR_ADD_CMD  "ip -6 route add %s dev %s"
#define ROUTE6BR_DEL_CMD  "ip -6 route del %s dev %s"

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n Execute cwmpPingTask... ");
	}
	
#if defined(TCSUPPORT_CT_PON_GDV20)
	while(1)
	{
		if(strlen(ipPingconvertIP))
		{
			strcpy(ipaddr, ipPingconvertIP);
			memset(ipPingconvertIP, 0, sizeof(ipPingconvertIP));
		}
#ifdef IPV6
		else if(strlen(ipPingconvertIPv6))
		{
			strcpy(ipaddr, ipPingconvertIPv6);
			ipPingIPver = 1;
			memset(ipPingconvertIPv6, 0, sizeof(ipPingconvertIPv6));
		}
#endif
		else if((ipPingIPver==-1) && (resolveHostError==0))
		{
			strcpy(ipaddr, ipPingconvertIP);
		}
		else
			break;
#else
#if 0   // IPPING_DIAGNOSTICS
	strcpy(ipaddr,(char*)inet_ntoa(cwmp_ping_p.target));
#else
	strcpy(ipaddr, ipPingconvertIP);
#endif
#endif

	//judge if resolve host name error!
	if( -1 == ipPingIPver )//resolve host name error
	{
		resolveHostError = -1;
	}
	else//judge if in route table
	{
		if(strcmp(gIPPingDev, "NoDevice") == 0)
		{		
			isRTFlag = -1;
			
		}else if(strcmp(gIPPingDev, "") == 0)
		{// default gateway
			isRTFlag = 1;// in route table
		}
#if 0 //no need
		else
		{
#if 1   //IPPING_DIAGNOSTICS	
			isRTFlag = isInRouteTable(ipaddr, gIPPingDev) ;
#else
			//isRTFlag = BeInRTTable(cwmp_ping_p.target, gIPPingDev);
			isRTFlag = 1;//need to implement in the future
#endif
		}
#endif
	}
	
	//if not in route table ,add it------use in the future!!
#if 0   //IPPING_DIAGNOSTICS
	strcpy(ipaddr,(char*)inet_ntoa(cwmp_ping_p.target));
#endif
	if((isRTFlag == 0) && (resolveHostError != -1))// not in route table then add it.
	{
    	if( strcmp(gIPPingDev, "br0") == 0 )
		{   //add route for br0
    		if ( 0 == ipPingIPver )
    		{
                        sprintf(cmdbuf,"route add -net %s netmask %s dev %s",ipaddr,sub_mask,gIPPingDev);
			sprintf(cmd_rmRoute,"route del -net %s netmask %s dev %s",ipaddr,sub_mask,gIPPingDev);
    		}
			else
			{
				sprintf(cmdbuf, ROUTE6BR_ADD_CMD, ipaddr, gIPPingDev);
				sprintf(cmd_rmRoute, ROUTE6BR_DEL_CMD, ipaddr, gIPPingDev);
			}
			rtAddStatus = system(cmdbuf);
            	}
		else
		{
  			if( (if_index = get_wanindex_by_name(gIPPingDev)) < 0 )
			{ 
	      			findIndexError = -1;
      			}

			if( findIndexError != -1 )
			{
			//add new route
				sprintf(nodeName, "WanInfo_Entry%d", if_index);
				memset(gateWay, 0, sizeof(gateWay));
				if ( 0 == ipPingIPver )
				{
				tcapi_get(nodeName, "GateWay", gateWay);
				snprintf(devName, sizeof(devName) - 1, "%s%s", 
					( (4 == wan_ipVersion) ? "ds." : "" ), gIPPingDev);
				sprintf(cmdbuf,"route add -net %s netmask %s dev %s",ipaddr,sub_mask,devName);
				sprintf(cmd_rmRoute,"route del -net %s netmask %s dev %s",ipaddr,sub_mask,devName);
				}
				else
				{
					tcapi_get(nodeName, "GateWay6", gateWay);
					sprintf(cmdbuf, ROUTE6_ADD_CMD, ipaddr, gateWay, gIPPingDev);
					sprintf(cmd_rmRoute, ROUTE6_DEL_CMD, ipaddr, gateWay, gIPPingDev);
				}
				rtAddStatus = system(cmdbuf);			
        		}
		}
	}
	//malloc buf for read file
	tmpbuf = malloc(MAX_PINGBUF_LEN);
	if(tmpbuf == NULL)
	{
		tc_printf("\r\n[cwmp ping task]mallco failed!");
		mallocError == -1;
	}

	resetCWMPPingResult();//reset the Ping Result before ping.
	rpc.flags.theIPPingFlag &= ~CWMP_IPPING_TERMINATED; 
	rpc.flags.theIPPingFlag |= CWMP_IPPING_INPROGRESS;//The cwmp IP Ping is in progress.
		
	//adding route fail or illegal route for adding or resolve host name error
	memset(cmdbuf,0,sizeof(cmdbuf));
#if 1   //IPPING_DIAGNOSTICS
	if((rtAddStatus < 0) || (isRTFlag == -1) || (resolveHostError == -1) || (mallocError == -1) || (findIndexError == -1))
#else
	if((rtAddStatus < 0) || (isRTFlag == -1) || (resolveHostError == -1) || (mallocError == -1))
#endif
	{
		tc_printf("\r\nenter ip ping error!");
		doCwmpIPPingError();
	}
	else
	{
		if ( 0 == ipPingIPver )
		{
		//do ping 
		if((cwmp_ping_p.interval == 0) && (cwmp_ping_p.len == 0))
			sprintf(cmdbuf,"ping -q -M 1 -c %lu -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.dscp,ipaddr,path);
		else if((cwmp_ping_p.interval == 0) && (cwmp_ping_p.len != 0))	
				sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.len,cwmp_ping_p.dscp,ipaddr,path);
		else if((cwmp_ping_p.interval != 0) && (cwmp_ping_p.len == 0))
		{
			sprintf(cmdbuf,"ping -q -M 1 -c %d -W %lu -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.interval/1000,cwmp_ping_p.dscp,ipaddr,path);
		}
		else
		{
			sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -W %lu -Q %lu %s > %s",NumberOfRepetitions,cwmp_ping_p.len,cwmp_ping_p.interval/1000,cwmp_ping_p.dscp,ipaddr,path);
		}
		}
		else
		{
			memset(ping_param, 0, sizeof(ping_param));
			if ( 0 != NumberOfRepetitions )
			{
				memset(cmdbuf, 0, sizeof(cmdbuf));
				snprintf(cmdbuf, sizeof(cmdbuf) - 1, " -c %d ", NumberOfRepetitions);
				strcat(ping_param, cmdbuf);
			}
			if ( 0 != cwmp_ping_p.interval )
			{
				memset(cmdbuf, 0, sizeof(cmdbuf));
				snprintf(cmdbuf, sizeof(cmdbuf) - 1, " -W %d ", cwmp_ping_p.interval/1000);
				strcat(ping_param, cmdbuf);
			}
			if ( 0 != cwmp_ping_p.len )
			{
				memset(cmdbuf, 0, sizeof(cmdbuf));
				snprintf(cmdbuf, sizeof(cmdbuf) - 1, " -s %d ", cwmp_ping_p.len);
				strcat(ping_param, cmdbuf);
			}
			memset(cmdbuf, 0, sizeof(cmdbuf));
			snprintf(cmdbuf, sizeof(cmdbuf) - 1, 
					"ping6 -q -M 1 -Q %lu %s %s > %s",
					cwmp_ping_p.dscp, ping_param, ipaddr, path);
		}
		system(cmdbuf);
		/*remove temp pid file of IP ping*/
		sprintf(cmdbuf, "rm -f %s", CWMP_PING_PID_PATH);
		system(cmdbuf);
		if(rpc.flags.theIPPingFlag & CWMP_IPPING_TERMINATED)
		{
			tc_printf("\r\n[cwmpPingTask] TERMINATED   manually! we do not read ping result from file\n");
		}
		else
		{
			//read the ping result
			fileRead(IPPINGDIAGNOSTICFILE,tmpbuf,MAX_PINGBUF_LEN);		
			result = 0;
			getCWMPPingResult();
			/*Receive 0 Packet means Error occured*/
			if(cwmp_ping_p.responses == 0)
			{
				tc_printf("\r\n[cwmpPingTask] getCWMPPingResult cwmp_ping_p.responses == 0!\n");
				result = -1;
			}
		}
    }
	
	rpc.flags.theIPPingFlag &= ~CWMP_IPPING_INPROGRESS;
	
	if(mallocError != -1)
		free(tmpbuf);
	
	if(rpc.flags.theIPPingFlag & CWMP_IPPING_TERMINATED)
	{
		tc_printf("\r\n[cwmpPingTask] TERMINATED   manually!\n");
	}
	else
	{
	
		if (!result)
		{
#if defined(TCSUPPORT_CT_PON_GDV20)
			strcpy(IPPingDiagnosticsStateTypeV20,"Complete");
#else
			strcpy(IPPingDiagnosticsStateType,"Complete");
#endif
		}else
		{
#if defined(TCSUPPORT_CT_PON_GD)
			if(resolveHostError == -1)
			{
#if defined(TCSUPPORT_CT_PON_GDV20)
				snprintf(IPPingDiagnosticsStateTypeV20,sizeof(IPPingDiagnosticsStateTypeV20),"%s","Error_CannotResolveHostName");
#else
				snprintf(IPPingDiagnosticsStateType,sizeof(IPPingDiagnosticsStateType),"%s","Error_CannotResolveHostName");
#endif
			}
			else
			{
#if defined(TCSUPPORT_CT_PON_GDV20)
				snprintf(IPPingDiagnosticsStateTypeV20,sizeof(IPPingDiagnosticsStateTypeV20),"%s","Complete");
#else
				snprintf(IPPingDiagnosticsStateType,sizeof(IPPingDiagnosticsStateType),"%s","Complete");
#endif
			}
#else
#if 1     //IPPING_DIAGNOSTICS
			if((rtAddStatus != 0) || (isRTFlag == -1) ||(mallocError == -1) || (findIndexError == -1))
#else
			if((rtAddStatus != 0) || (isRTFlag == -1) ||(mallocError == -1))
#endif
			{
				strcpy(IPPingDiagnosticsStateType,"Error_Internal");
			}
			else if(resolveHostError == -1)
			{
			strcpy(IPPingDiagnosticsStateType,"Error_CannotResolveHostName");
			}
			else
			{
				strcpy(IPPingDiagnosticsStateType,"Error_Other");
			}
#endif

			cwmp_ping_p.sent = NumberOfRepetitions;
		}
	}
#if defined(TCSUPPORT_CT_PON_GDV20)
		if(strlen(cmd_rmRoute) != 0){
			system(cmd_rmRoute);
			memset(cmd_rmRoute,0,sizeof(cmd_rmRoute));
		}
		if ((!result) && cwmp_ping_p.responses)
			break;
	}
	strncpy(IPPingDiagnosticsStateType, IPPingDiagnosticsStateTypeV20, sizeof(IPPingDiagnosticsStateType));
#endif
	
	//reset all flag.
	rtAddStatus = 0;
	result = -1;
	isRTFlag = 0;
	resolveHostError = 0;
 	mallocError = 0;
	
	if(rpc.flags.theIPPingFlag & CWMP_IPPING_TERMINATED){//If the ping test is terminated unusally, does not send inform.
		tc_printf("\r\n[cwmpPingTask] terminated unusally, does not send inform.!\n");
		rpc.flags.theIPPingFlag &= ~CWMP_IPPING_TERMINATED; 
	}else{
		/*Send the "8 DIAGNOSTICS COMPLETE" Event code*/
		//delete the route-----modify in the future
		//sprintf(cmdbuf,"route delete -net %s netmask %s dev %s",ipaddr,sub_mask,gIPPingDev);
		//rtAddStatus = system(cmdbuf);
#if defined (TCSUPPORT_CT_PON_SC)
		connection_req_flag = 1;
#endif

		setIPPingCompleteFlag(1);	
#if defined(TCSUPPORT_CT_MIDWARE)
		setOperationFlag(1);
#endif
		sem_post(&SEM_IMFORM);
	}

#if 1   //IPPING_DIAGNOSTICS
	//delete route added before
	if(strlen(cmd_rmRoute) != 0){
		system(cmd_rmRoute);
	}
#endif

	/*Set p_ping varible to 0 means thread exit*/
	p_ping = 0;
	return NULL;
	
}
#endif

int cwmpPingTaskStart(void)
{
	int ret;
	//pthread_t p_ping;
	pthread_attr_t p_ping_attr;

	/*create thread*/
	ret = pthread_attr_init(&p_ping_attr);
	if(ret != 0)
	{
		printf("\r\nThread attribute creation fail!");
		return -1;
	}
	//need to set deached way
	ret = pthread_attr_setdetachstate(&p_ping_attr,PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		printf("\r\nSet attribute fail!");
		return -1;
	}

	ret = pthread_create(&p_ping,&p_ping_attr,(void *)cwmpPingTask,NULL);
	if(ret != 0)
	{
		printf("\r\nCreate thread fail!");
		return -1;
	}

	pthread_attr_destroy(&p_ping_attr);
}

void doCwmpPing(void)
{
	ip4a dnsQueryResult;
	char cmdbuf[200]  = {0};
	char buf[64]={0};
	struct sockaddr_in cvtaddr;
	struct sockaddr_in *cvtaddr2  = NULL;
	struct addrinfo hints;
	struct addrinfo *res = NULL;
#if defined(TCSUPPORT_CT_PON_GDV20)
#ifdef IPV6
	struct addrinfo hintsv6;
	struct addrinfo *resv6 = NULL;			
#endif
#endif
#ifdef IPV6
	struct sockaddr_in6 ipv6addr;
	struct sockaddr_in6 *ipv6addr2 = NULL;
#endif
#define IPPING_DNSURL "/tmp/ippingdiag.conf"

#if 1
	memset(cmdbuf, 0, sizeof(cmdbuf));
	sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", ipPingDomainName, gIPPingDev, IPPING_DNSURL);
	system(cmdbuf);

	cwmp_checkWan_by_name(gIPPingDev, &wan_ipVersion);

#if defined(TCSUPPORT_CT_PON_GDV20)
	memset(ipPingconvertIP, 0, sizeof(ipPingconvertIP));
	memset(ipPingconvertIPv6, 0, sizeof(ipPingconvertIPv6));
#endif

	ipPingIPver = -1;
	if( inet_aton(ipPingDomainName,&cvtaddr.sin_addr) )
	{
		//ping diagnostic address is ipv4
		strcpy(ipPingconvertIP, ipPingDomainName);
		ipPingIPver = 0;
	}
#ifdef IPV6
	else if( inet_pton(AF_INET6, ipPingDomainName,(struct sockaddr *) &ipv6addr.sin6_addr) >0 )
	{	
		//ping diagnostic address is ipv6
		strcpy(ipPingconvertIP, ipPingDomainName);
		ipPingIPver = 1;
	}
#endif
	else
	{
		//ping diagnostic address is a domain name
		memset( &hints, 0 , sizeof(hints));
		hints.ai_family = ( 2 == wan_ipVersion ) ? AF_INET6 : AF_INET;
		hints.ai_socktype = SOCK_DGRAM;  		//DNS is UDP packet
		hints.ai_protocol = IPPROTO_UDP;
						
#if defined(TCSUPPORT_CT_PON_GDV20)
#ifdef IPV6
		memset( &hintsv6, 0 , sizeof(hintsv6));
		hintsv6.ai_family = AF_INET6;
		hintsv6.ai_socktype = SOCK_DGRAM; 		//DNS is UDP packet
		hintsv6.ai_protocol = IPPROTO_UDP;				
#endif
#endif
		if ( 1 == wan_ipVersion
			|| 2 == wan_ipVersion ) // ipv4 only or ipv6 only
		{
			if ( tc_getaddrinfo(ipPingDomainName, NULL, &hints, &res, 5) )
			{
				goto DNS_RESOLVE_END;
			}
		}
		else // ipv4/ipv6 or dslite+ipv6
		{
			if ( tc_getaddrinfo(ipPingDomainName, NULL, &hints, &res, 5) )
			{
#ifdef IPV6
				hints.ai_family = AF_INET6;
					if( tc_getaddrinfo(ipPingDomainName, NULL, &hints, &res, 5) )
				{
					goto DNS_RESOLVE_END;
				}
#else
				goto DNS_RESOLVE_END;
#endif
			}
#if defined(TCSUPPORT_CT_PON_GDV20)
#ifdef IPV6
			else
			{
				if( tc_getaddrinfo(ipPingDomainName, NULL, &hintsv6, &resv6, 5) == 0 )
				{
					ipv6addr2 = (struct sockaddr_in6 *)resv6->ai_addr;
					memset(buf, 0, sizeof(buf));
					inet_ntop(AF_INET6,&ipv6addr2->sin6_addr, buf, INET6_ADDRSTRLEN);
					strcpy(ipPingconvertIPv6, buf);
				}
			}
#endif
#endif
		}

		switch ( res->ai_family )
		{
			case AF_INET:			//ipv4
				cvtaddr2 = (struct sockaddr_in *) res->ai_addr;
				strcpy(ipPingconvertIP, inet_ntoa(cvtaddr2->sin_addr));
				ipPingIPver = 0;
				break;
#ifdef IPV6
			case AF_INET6:			//ipv6
				ipv6addr2 = (struct sockaddr_in6 *)res->ai_addr;
				memset(buf, 0, sizeof(buf));
				inet_ntop(AF_INET6,&ipv6addr2->sin6_addr, buf, INET6_ADDRSTRLEN);
				strcpy(ipPingconvertIP, buf);
				ipPingIPver = 1;
				break;
#endif
			default:
				break;
		}
	}
#else
	if(ipPingDomainName != NULL)
	{
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", ipPingDomainName, gIPPingDev, IPPING_DNSURL);
		system(cmdbuf);

		dnsQueryResult = resolveName(ipPingDomainName);
		unlink(IPPING_DNSURL);
		if(!dnsQueryResult)
		{
			cwmp_ping_p.target = 0L;
		}
		else
		{
			if(resolve(inet_ntoa(dnsQueryResult),&cwmp_ping_p.target,NULL) != 0)
			{
				cwmp_ping_p.target = 0L;
			}
			else
			{
				//cwmp_ping_p.target is right
			}
		}
		os_free(ipPingDomainName);
		ipPingDomainName = NULL;
	}
	else
	{
		cwmp_ping_p.target = 0L;
	}
#endif

DNS_RESOLVE_END:
	unlink(IPPING_DNSURL);
#ifndef THREAD_REDUCE
	if(p_ping == 0)
	{
		tc_printf("\r\ncreate thread to do ip ping diagnostic!");
		cwmpPingTaskStart();
	}
	else
		sem_post(&CWMP_PING);
#else
	if(p_ping == 0)
	{
		tc_printf("\r\ncreate thread to do ip ping diagnostic!");
		cwmpPingTaskStart();
	}
	else
	{
		tc_printf("\r\n----------------------------p_ping is not 0, error occured!-------------------------------");
		//cwmpPingTaskStart();
	}
#endif
		
}
/**************************ip ping diagnostic end***********************************/
