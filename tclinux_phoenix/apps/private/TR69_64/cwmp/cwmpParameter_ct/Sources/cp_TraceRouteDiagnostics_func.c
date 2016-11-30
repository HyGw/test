#include "Global_res.h"
#include "cp_TraceRouteDiagnostics.h"
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
#include <netdb.h>
#endif

static char  TraceRouteDiagnosticsStateType[32] = "None";
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
static char traceRouteDomainName[MAX_DOMAIN_LENGTH] = {0};
static char iptraceRouteconvertIP[64]={0};
#if defined(TCSUPPORT_CT_PON_GDV20)
static char iptraceRouteconvertIPv6[64]={0};
static char  TraceRouteDiagnosticsStateTypeV20[32] = "None";
#endif
static int iptraceRouteIPver = -1;
extern int tc_getaddrinfo(const char *hostname, const char *service
							, const struct addrinfo *hints
							, struct addrinfo **result, int timeout);
static int tr_wan_ipVersion = 1;
#else
static char *traceRouteDomainName = NULL;
#endif
//static uint32 NumberOfRepetitions = 4;//add the default value for traceroute diagnostic,brian
static uint32 DSCPValue = 0;
char  gTraceRouteInterface[128] = {0};
#if 0//xyyou_gw
char  gTraceRouteDev[15] = "NoDevice";
#else
char  gTraceRouteDev[15] = {0};
#endif
#ifndef THREAD_REDUCE
sem_t CWMP_TRACEROUTE;
#endif
pthread_t p_traceroute = 0;
traceroute_t cwmp_traceroute_p;
tracerouteHophost_t *cwmp_tracerouteHophost_p[64];


int8 getTraceRouteDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, TraceRouteDiagnosticsStateType);
}

int8 setTraceRouteDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
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
		resetCwmpTraceRoute();//If the trace route test is in progress, terminate it first.
#endif		
		rpc.flags.theTraceRouteFlag |= CWMP_TRACEROUTE_ACTIVE;
		strcpy(TraceRouteDiagnosticsStateType,"Requested");
#if defined(TCSUPPORT_CT_PON_GDV20) && defined(TCSUPPORT_CT_TRACEROUTEIPV6)
		strcpy(TraceRouteDiagnosticsStateTypeV20,"Requested");
#endif
	}
	else {
		//dbg_pline_1("\r\nTraceRouteDiagnosticsStateType parameter is not Requested!");
		return -1;
	}

	return 0;
}

int8 getTraceRouteDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteDiagnosticsState;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTraceRouteDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteDiagnosticsState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getTraceRouteInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, gTraceRouteInterface);
}

int8 setTraceRouteInterfaceValue(void* value, struct DeviceNode*  deviceNode)
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
	uint32 entrynode;
	int pppIndex;
	char *tmp = NULL;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;
#endif
#ifdef THREAD_REDUCE	
	resetCwmpTraceRoute();//If the trace route test is in progress, terminate it first.
#endif
	
	setvalue=(char*)value;
	len = strlen(DefaultIface_WAN);

	if(strncmp(DefaultIface_WAN, setvalue, len) == 0){//WAN interface, get the gateway.
		/*check the value is valid or not.*/
		cwmpSearchDeviceNodeByName(setvalue, rpc.IGDDeviceRootPtr, &Node);
		if(Node == NULL){
			tc_printf("\r\nparameter.c---[setTraceRouteInterfaceValue]---Invalid Value!");
			return -1;
		}
		
		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		pvcnode = atoi(tempvalue);
		pvctmp = pvcnode;
		if(FindPVCMapping(&pvcnode) != 0){
			tc_printf("\r\nparameter.c---[setTraceRouteInterfaceValue]---Invalid pvc number!");
			return -1;
		}
#else
		if(setvalue[len]<'1'||setvalue[len]>'8'){
			tc_printf("\r\nparameter.c---[setTraceRouteInterfaceValue]---Invalid pvc number!");
			return -1;
		}
		pvcnode = atoi(tempvalue) - 1;
#endif
		
		strcpy(gTraceRouteDev,"NoDevice");

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
					sprintf(gTraceRouteDev,"ppp%d",pppIndex);
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
					sprintf(gTraceRouteDev,"nas%d_%d", pvcnode, entrynode);
				}
			}
		}
		else{//error value
			return -1;
		}
	}
	else if(strcmp(setvalue,DefaultIface_LAN) == 0){//LAN interface, get the gateway.
		strcpy(gTraceRouteDev,"br0");		
	}else if(strcmp(setvalue, "") == 0)
	{//empty string
		strcpy(gTraceRouteInterface, "");		
		//get default interface(move to trace route task)
		strcpy(gTraceRouteDev,"");
		return 0;
		
	}else{//error value
		return -1;
	}

	strcpy(gTraceRouteInterface, setvalue);//set the interface.

	return 0;
}

int8 getTraceRouteInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteInterface;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTraceRouteInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getTraceRouteHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
	tc_printf("\r\nthe TraceRouteHost is %s", traceRouteDomainName);
	return getTheValue(value, type, deviceNode, traceRouteDomainName);

#else
	char* tempValue;
	char	TraceRouteHost[32];

	sprintf(TraceRouteHost,"%s",inet_ntoa(cwmp_traceroute_p.target));
	tc_printf("\r\nthe TraceRouteHost is %s",TraceRouteHost);
	return getTheValue(value, type, deviceNode, TraceRouteHost);
#endif
}
int8 setTraceRouteHostValue(void* value, struct DeviceNode*  deviceNode)
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
	resetCwmpTraceRoute();
#endif
	
#if !defined(TCSUPPORT_CT_TRACEROUTEIPV6)
	if(traceRouteDomainName == NULL)
	{
		traceRouteDomainName = (char*)os_malloc(sizeof(char)*MAX_DOMAIN_LENGTH);
		if(traceRouteDomainName == NULL)
		{
			tc_printf("\r\nos_malloc failure");
			return -1;
		}
	}
#endif
	
	memset(traceRouteDomainName,0,sizeof(char)*MAX_DOMAIN_LENGTH);
	strcpy(traceRouteDomainName,(char*)value);
	
	return 0;
}
int8 getTraceRouteHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteHost;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTraceRouteHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteHost;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getNumberOfTriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  vNumberOfTries[4];

	if(cwmp_traceroute_p.NumberofTries < 1) 
		sprintf(vNumberOfTries,"3");	
	else
		sprintf(vNumberOfTries,"%lu",cwmp_traceroute_p.NumberofTries);	
	return getTheValue(value, type, deviceNode, vNumberOfTries);
}

int8 setNumberOfTriesValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 num = 0;

	num = *((uint32*)value);
	if((num <1)||(num>3)){
		return -1;
	}
	cwmp_traceroute_p.NumberofTries = num;	
#ifdef THREAD_REDUCE	
	resetCwmpTraceRoute();//If the trace route test is in progress, terminate it first.
#endif	
	return 0;
}

int8 getNumberOfTriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteNumberOfTries;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setNumberOfTriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteNumberOfTries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getTraceRouteTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  vTraceRouteTimeout[10];

	if(cwmp_traceroute_p.Timeout < 1) 
#if defined(TCSUPPORT_CT_PON_CZ_GD)
		strcpy(vTraceRouteTimeout, "5000");//now default timeout is 5000
#else
		strcpy(vTraceRouteTimeout, "4999");//now default timeout is 4999
#endif
		
	else
		sprintf(vTraceRouteTimeout, "%lu",cwmp_traceroute_p.Timeout);

	return getTheValue(value, type, deviceNode, vTraceRouteTimeout);
}

int8 setTraceRouteTimeoutValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 num = 0;

	num = *((uint32*)value);
	if(num <1 || num > 65535){//Can not set Timeout to 0. 
		tc_printf("setTraceRouteTimeoutValue set value is wrong!\r\n");
		return -1;
	}
	cwmp_traceroute_p.Timeout = num;

#ifdef THREAD_REDUCE	
	resetCwmpTraceRoute();//If the traceroute test is in progress, terminate it first.
#endif
	return 0;
}

int8 getTraceRouteTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteTimeout;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTraceRouteTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteTimeout;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getTraceRouteDataBlockSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char* tempValue;
	char  vDataBlockSize[10];

	if(cwmp_traceroute_p.DataBlockSize < 1) 
		strcpy(vDataBlockSize, "38");
	else
		sprintf(vDataBlockSize, "%u", cwmp_traceroute_p.DataBlockSize);
	return getTheValue(value, type, deviceNode, vDataBlockSize);
}

int8 setTraceRouteDataBlockSizeValue(void* value, struct DeviceNode*  deviceNode)
{
	if((*((uint32*)value)) < 1 || (*((uint32*)value)) > 65535)
		return -1;
		
	cwmp_traceroute_p.DataBlockSize= *((uint32*)value);
#ifdef THREAD_REDUCE	
	resetCwmpTraceRoute();//If the trace route test is in progress, terminate it first.
#endif
	return 0;
}

int8 getTraceRouteDataBlockSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteDataBlockSize;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTraceRouteDataBlockSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteDataBlockSize;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getTraceRouteDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	//char* tempValue;
	char  vDSCPValue[10];
	
	sprintf(vDSCPValue, "%lu",DSCPValue);

	return getTheValue(value, type, deviceNode, vDSCPValue);
}

int8 setTraceRouteDSCPValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue = 0;
	
	tempValue = *(uint32*)value;
	if(tempValue > 63)
		return -1;

	DSCPValue = tempValue;	
#ifdef THREAD_REDUCE	
	resetCwmpTraceRoute();//If the trace route test is in progress, terminate it first.
#endif	
	return 0;
}

int8 getTraceRouteDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteDSCP;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setTraceRouteDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteDSCP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getMaxHopCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{

//	char* tempValue;
	char  vMaxHopCount[10];

	if(cwmp_traceroute_p.MaxHopCount < 1) 
		strcpy(vMaxHopCount, "29");//now dedault len is 29
	else
		sprintf(vMaxHopCount, "%u", cwmp_traceroute_p.MaxHopCount);
	return getTheValue(value, type, deviceNode, vMaxHopCount);

}

int8 setMaxHopCountValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue = 0;
	
	tempValue = *(uint32*)value;
	if(tempValue > 255||tempValue < 1)
		return -1;

	cwmp_traceroute_p.MaxHopCount = tempValue;	
#ifdef THREAD_REDUCE	
	resetCwmpTraceRoute();//If the trace route test is in progress, terminate it first.
#endif	
	return 0;
}


int8 getMaxHopCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteMaxHopCount;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMaxHopCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteMaxHopCount;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
//	char* retValue;
	char tempValue[10];
	
	sprintf(tempValue,"%u",cwmp_traceroute_p.ResponseTime);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteResponseTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteResponseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getRouteHopsNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
//	char* retValue;
	char tempValue[10];
	uint16 RouteHopsNumberOfEntries;

	RouteHopsNumberOfEntries = cwmp_traceroute_p.RouteHopsNumberOfEntries; 
	
	sprintf(tempValue,"%u",RouteHopsNumberOfEntries);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getRouteHopsNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteRouteHopsNumberOfEntries;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRouteHopsNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteRouteHopsNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getHopHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char hopHost[MAX_DOMAIN_LENGTH+1]="";
	int index = 0;
	int len = 0;
	index= atoi(deviceNode->parentNode->name);
	if((index <1)||(index >64))
		strcpy(hopHost, "");

	if(cwmp_tracerouteHophost_p[index-1] == NULL)
		strcpy(hopHost, "");
	else{
		len = strlen(cwmp_tracerouteHophost_p[index-1]->HopHost);
		if(len > MAX_DOMAIN_LENGTH){
			strncpy(hopHost, cwmp_tracerouteHophost_p[index-1]->HopHost, MAX_DOMAIN_LENGTH);
			hopHost[MAX_DOMAIN_LENGTH] = '\0';
		}
		else{
			strncpy(hopHost, cwmp_tracerouteHophost_p[index-1]->HopHost, len);
			hopHost[len] = '\0';
		}
	}
	return getTheValue(value, type, deviceNode, hopHost);
}

int8 getHopHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopHost;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHopHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopHost;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getHopHostAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char hopHostAddr[64]="";
	int index = 0;
	
	index= atoi(deviceNode->parentNode->name);
	if((index <1)||(index >64))
		strcpy(hopHostAddr, "");
	
	if(cwmp_tracerouteHophost_p[index-1] == NULL)
		strcpy(hopHostAddr,"");
	else
		strcpy(hopHostAddr, cwmp_tracerouteHophost_p[index-1]->HopHostAddress);
	return getTheValue(value, type, deviceNode, hopHostAddr);
}

int8 getHopHostAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopHostAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHopHostAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopHostAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getHopErrorCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ErrorCode[10]="";
	int index = 0;
	index= atoi(deviceNode->parentNode->name);
	if((index <1)||(index >64))
		strcpy(ErrorCode, "");
	
	if(cwmp_tracerouteHophost_p[index-1] == NULL)
		strcpy(ErrorCode, "");
	else
		sprintf(ErrorCode, "%lu", cwmp_tracerouteHophost_p[index-1]->HopErrorCode);
	return getTheValue(value, type, deviceNode, ErrorCode);
}

int8 getHopErrorCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopErrorCode;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHopErrorCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopErrorCode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getHopRTTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char RTTimes[33]="";
	int index = 0;
	int len = 0;
	index= atoi(deviceNode->parentNode->name);
	if((index <1)||(index >64))
		strcpy(RTTimes, "");
	
	if(cwmp_tracerouteHophost_p[index-1] == NULL)
		strcpy(RTTimes,"");
	else{
		len = strlen(cwmp_tracerouteHophost_p[index-1]->HopRTTime);
		if(len >32){
			strncpy(RTTimes, cwmp_tracerouteHophost_p[index-1]->HopRTTime, 32);
			RTTimes[32] = '\0';
		}else{
			strncpy(RTTimes, cwmp_tracerouteHophost_p[index-1]->HopRTTime, len);
			RTTimes[len] = '\0';
		}
		
	}
	return getTheValue(value, type, deviceNode, RTTimes);
}

int8 getHopRTTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopRTTimes;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setHopRTTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcTraceRouteDiagnosticAttrAttr.aTraceRouteHopRTTimes;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= TRACEROUTEDIAGNOSTIC_ATTR_FLAG;
	return result;
}

//add by brian 
/**************************trace route diagnostic begin***********************************/
static void resetCWMPTraceRouteResult(void)
{
	int i = 0; 
	cwmp_traceroute_p.ResponseTime = 0;
	cwmp_traceroute_p.RouteHopsNumberOfEntries = 0; 
	for(i = 0; i <64; i++){
		if(cwmp_tracerouteHophost_p[i] !=NULL){
			os_free(cwmp_tracerouteHophost_p[i]);
			cwmp_tracerouteHophost_p[i] = NULL;
		}
	}
	return;

}


#ifdef THREAD_REDUCE
#define CWMP_TRACEROUTE_PID_PATH "/tmp/cwmp/tr69diag_traceroute.pid"

static void cwmpStopTraceRoute(void)
{
	FILE *fp = NULL;
	char cmdbuf[128] = {0};
	char traceroute_pid_tmp[100] = {0};
	int traceroute_pid = 0;
	fp = fopen(CWMP_TRACEROUTE_PID_PATH, "r");
	if(fp == NULL)
	{
		/*get trace route process pid*/
		tc_printf("\r\ncwmpStopTraceRoute: can not get pid from file, we need use pidof to find traceroute pid\n");
		sprintf(cmdbuf, "pidof %s > %s", "traceroute", CWMP_TRACEROUTE_PID_PATH);
		system(cmdbuf);  /*get the PID of old diagnostic process if exist*/
		fp = fopen(CWMP_TRACEROUTE_PID_PATH, "r");
	}
	
	if(fp == NULL){
		return;
	}
	
	fgets(traceroute_pid_tmp, sizeof(traceroute_pid_tmp), fp);
	fclose(fp);
	unlink(CWMP_TRACEROUTE_PID_PATH);
	traceroute_pid = atoi(traceroute_pid_tmp);
	if(traceroute_pid != 0){  /*if pid != 0, that's mean old traceroute diagnostic process already exist*/
		sprintf(cmdbuf,"kill -9 %d", traceroute_pid);  /*remove old traceroute diagnostic process*/
		tc_printf("CWMP trace route Diagnostic process down, pid is [%d]\n", traceroute_pid);
	    system(cmdbuf);
	}
}

static void  resetCwmpTraceRoute(void)
{
	strcpy(TraceRouteDiagnosticsStateType,"None");
#if defined(TCSUPPORT_CT_PON_GDV20) && defined(TCSUPPORT_CT_TRACEROUTEIPV6)
	strcpy(TraceRouteDiagnosticsStateTypeV20,"None");
#endif
	
	if(rpc.flags.theTraceRouteFlag & CWMP_TRACEROUTE_INPROGRESS){
		cwmpStopTraceRoute();
		tc_printf("\r\n resetCwmpTraceRoute-----------stop traceroute progress!!!");		
		resetCWMPTraceRouteResult();
		RebuildRouteHopsObject();
		rpc.flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_INPROGRESS;
		rpc.flags.theTraceRouteFlag |= CWMP_TRACEROUTE_TERMINATED; //The trace route test is terminated.
	}
	return;
}
#endif


#if 0
static void doCwmpIPPingError(void)
{
	cwmp_traceroute_p.sent = NumberOfRepetitions;
	cwmp_traceroute_p.responses = 0;
	cwmp_traceroute_p.minrtt = 0;
	cwmp_traceroute_p.maxrtt = 0;
	cwmp_traceroute_p.avrrtt = 0;
}
#endif

extern int cwmp_checkWan_by_name(char *wan_if_name, int *ipMode);

int getCWMPTraceRouteResult(void)
{
	//FILE *fp = NULL;
	int i = 0;
	char tmpstr[50]={0};
	char result[10]={0};
	char tmpresult[50]={0};
	int rt=-2;
	if((rpc.flags.theTraceRouteFlag & CWMP_TRACEROUTE_INPROGRESS) == 0){//this is not cwmp trace route, do not get the result.
		return 0;
	}
	
		rt=get_profile_str("ResponseTime=", tmpresult, sizeof(tmpresult), NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);
		cwmp_traceroute_p.ResponseTime = atoi(tmpresult);
		get_profile_str("RouteHopsNumberOfEntries=", tmpresult, sizeof(tmpresult),  NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);//xyyou==
		cwmp_traceroute_p.RouteHopsNumberOfEntries = atoi(tmpresult);
#if 0
		get_profile_str("MaxHopCountExceeded=", result, sizeof(result);
		if(strcmp(result, "1")==0)
			return -2;
#endif
	for(i=0; i<cwmp_traceroute_p.RouteHopsNumberOfEntries; i++){
		cwmp_tracerouteHophost_p[i] = (tracerouteHophost_t *)os_malloc(sizeof(tracerouteHophost_t));
		if(cwmp_tracerouteHophost_p[i] == NULL){
			tcdbg_printf("\r\n error allocat cwmp_tracerouteHophost_p%d", i);
			resetCWMPTraceRouteResult();//xyyou== error other?
			return -1;
		}
		memset(cwmp_tracerouteHophost_p[i], 0, sizeof(tracerouteHophost_t));
		sprintf(tmpstr, "HopHostAddress%d=", i+1);
		get_profile_str(tmpstr, cwmp_tracerouteHophost_p[i]->HopHostAddress, sizeof(cwmp_tracerouteHophost_p[i]->HopHostAddress), NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);
		sprintf(tmpstr, "HopHost%d=", i+1);
		get_profile_str(tmpstr, cwmp_tracerouteHophost_p[i]->HopHost, sizeof(cwmp_tracerouteHophost_p[i]->HopHost), NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);
		sprintf(tmpstr, "HopErrorCode%d=", i+1);
		get_profile_str(tmpstr, tmpresult, sizeof(tmpresult), NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);
		cwmp_tracerouteHophost_p[i]->HopErrorCode = atoi(tmpresult);
		sprintf(tmpstr, "HopRTTimes%d=", i+1);
		get_profile_str(tmpstr, cwmp_tracerouteHophost_p[i]->HopRTTime, sizeof(cwmp_tracerouteHophost_p[i]->HopRTTime), NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);

	}
#if 1
		get_profile_str("MaxHopCountExceeded=", result, sizeof(result),NO_QMARKS, TRACEROUTEDIAGNOSTICFILE);
		if(strcmp(result, "1")==0)
			return -2;
#endif

	
	//unlink(TRACEROUTEDIAGNOSTICFILE);
	
	return 0;
}


#ifndef THREAD_REDUCE
void *cwmpTraceRouteTask(void)
{

	int resolveHostError = 0;
	int rtAddStatus = 0;
	int result = -1;
	int mallocError = 0;//malloc for read trace route diagnostic
	int isRTFlag = 0;//0:trace route rule is not in route table; 1:in route table; -1:trace route rule error.
	char cmdbuf[200] = {'\0'};
	char sub_mask[20] = "255.255.255.255";
	char ipaddr[20] = {0};
//	char *tmpbuf = NULL;
//	char path[] ="/tmp/cwmp/ippingdiagnostic_stat";
	unsigned long tmpdatablockSize = 38;
	unsigned long tmptimeout = 4999;
	unsigned long tmpnumberoftries = 3;
	unsigned long tmpmaxhopcount = 29;

	while(1)
	{
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\n Execute cwmpTraceRouteTask... ");
		}
		
		//judge if resolve host name error!
		if(cwmp_traceroute_p.target == 0)//resolve host name error
		{
			resolveHostError = -1;
		}
		else//judge if in route table
		{
			if(strcmp(gTraceRouteDev, "NoDevice") == 0)
			{		
				isRTFlag = -1;
				
			}else if(strcmp(gTraceRouteDev, "") == 0)
			{// default gateway
				isRTFlag = 1;// in route table
			}
			else
			{
				//isRTFlag = BeInRTTable(cwmp_traceroute_p.target, gIPPingDev);
				isRTFlag = 1;//need to implement in the future
			}
		}
		
		//if not in route table ,add it------use in the future!!
		strcpy(ipaddr,(char*)inet_ntoa(cwmp_traceroute_p.target));
		if((isRTFlag == 0) && (resolveHostError != -1))// not in route table then add it.
		{
			sprintf(cmdbuf,"route add -net %s netmask %s dev %s",ipaddr,sub_mask,gTraceRouteDev);
			rtAddStatus = system(cmdbuf);
		}
		//malloc buf for read file
		//tmpbuf = malloc(MAX_TRACEROUTBUF_LEN);
		//if(tmpbuf == NULL)
		//{
		//	tc_printf("\r\n[cwmp traceroute task]mallco failed!");
		//	mallocError == -1;
		//}
	
		resetCWMPTraceRouteResult();//reset the trace route Result before trace.
		rpc.flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_TERMINATED; 
		rpc.flags.theTraceRouteFlag |= CWMP_TRACEROUTE_INPROGRESS;//The cwmp traceroute is in progress.
			
		//adding route fail or illegal route for adding or resolve host name error
		memset(cmdbuf,0,sizeof(cmdbuf));
		if((rtAddStatus < 0) || (isRTFlag == -1) || (resolveHostError == -1) || (mallocError == -1))
		{
			tc_printf("\r\nenter trace route error!");
			resetCWMPTraceRouteResult();
		}else
		{
		//do trace route
		//default value for trace route
		if(cwmp_traceroute_p.DataBlockSize)
			tmpdatablockSize = cwmp_traceroute_p.DataBlockSize;
		if(cwmp_traceroute_p.Timeout)
			tmptimeout = cwmp_traceroute_p.Timeout/1000;
		if(cwmp_traceroute_p.NumberofTries)
			tmpnumberoftries = cwmp_traceroute_p.NumberofTries;	
		if(cwmp_traceroute_p.MaxHopCount)
			tmpmaxhopcount = cwmp_traceroute_p.MaxHopCount;
		sprintf(cmdbuf,"traceroute -x 1 -m %lu -q %lu  -w %lu %s %lu", tmpmaxhopcount, tmpnumberoftries, tmptimeout/1000, ipaddr, tmpdatablockSize);
#if 0
		#if 1
			if((cwmp_traceroute_p.interval == 0) && (cwmp_traceroute_p.len == 0))
				sprintf(cmdbuf,"ping -q -M 1 -c %lu %s > %s",NumberOfRepetitions,ipaddr,path);
			else if((cwmp_traceroute_p.interval == 0) && (cwmp_traceroute_p.len != 0))	
				sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u %s > %s",NumberOfRepetitions,cwmp_traceroute_p.len,ipaddr,path);
			else if((cwmp_traceroute_p.interval != 0) && (cwmp_traceroute_p.len == 0))
				sprintf(cmdbuf,"ping -q -M 1 -c %d -W %lu %s > %s",NumberOfRepetitions,cwmp_traceroute_p.interval,ipaddr,path);
			else
				sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -W %lu %s > %s",NumberOfRepetitions,cwmp_traceroute_p.len,cwmp_traceroute_p.interval,ipaddr,path);
			//exec ping
		#else
			strcpy(cmdbuf,"ping -q -M 1 -c 3 -s 32 -W 5000 192.168.1.12 > /tmp/cwmp/ippingdiagnostic_stat");//for test
		#endif
#endif
			system(cmdbuf);
			//read the traceroute result
			//fileRead(TRACEROUTEDIAGNOSTICFILE,tmpbuf,MAX_TRACEROUTBUF_LEN);
			//result = 0;
			result = getCWMPTraceRouteResult();
			unlink(TRACEROUTEDIAGNOSTICFILE);
        }
		
		rpc.flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_INPROGRESS;
		
		//if(mallocError != -1)
			//free(tmpbuf);
		
		if(dbg_cwmp >= DBG_CWMP_L2 )
		{
			tc_printf("\r\ncwmpParameter.c-----[cwmpTraceRouteTask]----  result =", result);
		}
		if (!result)
		{
			strcpy(TraceRouteDiagnosticsStateType,"Complete");
		}else if(result==-2){
				strcpy(TraceRouteDiagnosticsStateType,"Error_MaxHopCountExceeded");
				tcdbg_printf("\r\n[cwmpTraceRouteTask] getCWMPTraceRouteResult MaxHopCountExceeded!\n");
		}else
		{
			if((rtAddStatus != 0) || (isRTFlag == -1) ||(mallocError == -1))
			{
				strcpy(TraceRouteDiagnosticsStateType,"Error_Internal");
			}
			else if(resolveHostError == -1)
			{
			strcpy(TraceRouteDiagnosticsStateType,"Error_CannotResolveHostName");
			}
			else
			{
				strcpy(TraceRouteDiagnosticsStateType,"Error_Other");
			}
			//cwmp_traceroute_p.sent = NumberOfRepetitions;
		}
		
		//reset all flag.
		rtAddStatus = 0;
		result = -1;
		isRTFlag = 0;
		resolveHostError = 0;
	 	mallocError = 0;
		
		if(rpc.flags.theTraceRouteFlag & CWMP_TRACEROUTE_TERMINATED){//If the trace route test is terminated unusally, does not send inform.
			rpc.flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_TERMINATED; 
		}else{
			/*Send the "8 DIAGNOSTICS COMPLETE" Event code*/
			//delete the route-----modify in the future
			//sprintf(cmdbuf,"route delete -net %s netmask %s dev %s",ipaddr,sub_mask,gTraceRouteDev);
			//rtAddStatus = system(cmdbuf);
			setIPPingCompleteFlag(1);	
			sem_post(&SEM_IMFORM);
		}
		
		sem_wait(&CWMP_TRACEROUTE);
	}
	
}
#else
void *cwmpTraceRouteTask(void)
{

	int resolveHostError = 0;
	int rtAddStatus = 0;
	int result = -1;
	int mallocError = 0;//malloc for read trace route diagnostic
	int isRTFlag = 0;//0:trace route rule is not in route table; 1:in route table; -1:trace route rule error.
	char cmdbuf[200] = {'\0'};
	char sub_mask[20] = "255.255.255.255";
	char ipaddr[64] = {0};
//	char *tmpbuf = NULL;
//	char path[] ="/tmp/cwmp/ippingdiagnostic_stat";
	unsigned long tmpdatablockSize = 38;
#if defined(TCSUPPORT_CT_PON_CZ_GD)
	unsigned long  tmptimeout = 5000;
#else
	unsigned long  tmptimeout = 4999;
#endif
	unsigned long tmpnumberoftries = 3;
	unsigned long tmpmaxhopcount = 29;
	int ipVersion = 1;
	char devName[64] = {0};
#if 1//xyyou_gw
	char nodeName[33] = {0};
	char cmd_rmRoute[200] = {0};
	char gateWay[64] = {0};
	char pvcindex[10]={0};
#endif
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
	int findIndexError = 0, if_index = 0;;
#define ROUTE6_ADD_CMD  "ip -6 route add %s via %s dev %s"
#define ROUTE6_DEL_CMD  "ip -6 route del %s via %s dev %s"
#define ROUTE6BR_ADD_CMD  "ip -6 route add %s dev %s"
#define ROUTE6BR_DEL_CMD  "ip -6 route del %s dev %s"
#endif

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n Execute cwmpTraceRouteTask... ");
	}
	
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
#if defined(TCSUPPORT_CT_PON_GDV20)
	while(1)
	{
		if(strlen(iptraceRouteconvertIP))
		{
	strcpy(ipaddr, iptraceRouteconvertIP);
			memset(iptraceRouteconvertIP, 0, sizeof(iptraceRouteconvertIP));
		}
#ifdef IPV6
		else if(strlen(iptraceRouteconvertIPv6))
		{
			strcpy(ipaddr, iptraceRouteconvertIPv6);
			iptraceRouteIPver = 1;
			memset(iptraceRouteconvertIPv6, 0, sizeof(iptraceRouteconvertIPv6));
		}
#endif
		else if((iptraceRouteIPver==-1) && (resolveHostError==0))
		{
			strcpy(ipaddr, iptraceRouteconvertIP);
		}
		else
			break;
#else
	strcpy(ipaddr, iptraceRouteconvertIP);
#endif
#else
#if 1//xyyou_gw
	strcpy(ipaddr,(char*)inet_ntoa(cwmp_traceroute_p.target));
#endif
#endif

	/* judge if resolve host name error! */
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
	if( -1 == iptraceRouteIPver ) /* resolve host name error */
#else
	if(cwmp_traceroute_p.target == 0) /* resolve host name error */
#endif
	{
		resolveHostError = -1;
	}
	else//judge if in route table
	{
		if(strcmp(gTraceRouteDev, "NoDevice") == 0)
		{		
			isRTFlag = -1;
			
		}else if(strcmp(gTraceRouteDev, "") == 0)
		{// default gateway
			isRTFlag = 1;// in route table
		}
		else
		{
#if !defined(TCSUPPORT_CT_TRACEROUTEIPV6)
			#if 1//xyyou_gw
			isRTFlag = isInRouteTable(ipaddr, gTraceRouteDev);	
			#else
			//isRTFlag = BeInRTTable(cwmp_traceroute_p.target, gTraceRouteDev);
			isRTFlag = 1;//need to implement in the future
			#endif
#endif
		}
	}
	
	//if not in route table ,add it------use in the future!!
//	strcpy(ipaddr,(char*)inet_ntoa(cwmp_traceroute_p.target));//xyyou_gw
	if((isRTFlag == 0) && (resolveHostError != -1))// not in route table then add it.
	{
	#if 1//xyyou_gw
		if(strcmp(gTraceRouteDev, "br0")==0){
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
			if ( 0 == iptraceRouteIPver )
    		{
#endif
			sprintf(cmdbuf, "route add -net %s netmask %s dev %s", ipaddr, sub_mask,gTraceRouteDev);
			sprintf(cmd_rmRoute, "route del -net %s netmask %s dev %s", ipaddr, sub_mask,gTraceRouteDev);
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
			}
			else
			{
				sprintf(cmdbuf, ROUTE6BR_ADD_CMD, ipaddr, gTraceRouteDev);
				sprintf(cmd_rmRoute, ROUTE6BR_DEL_CMD, ipaddr, gTraceRouteDev);
			}
#endif
			rtAddStatus = system(cmdbuf);
			}else{
				cwmp_checkWan_by_name(gTraceRouteDev, &ipVersion);
				//sprintf(pvcindex, "%c", gTraceRouteDev[3]);
				//if((atoi(pvcindex)<0)||(atoi(pvcindex)>=(MAX_WAN_NODE*MAX_WAN_NODEL2))){
				//	rtAddStatus = -1;
				//}
				//if(rtAddStatus !=-1){
					//sprintf(nodeName, "WanInfo_Entry%d",atoi(pvcindex));
					//memset(gateWay, 0 ,sizeof(gateWay));
					//tcapi_get(nodeName, "GateWay", gateWay);
					//if(_checkIp(gateWay)){
					//	sprintf(cmdbuf,"route add -net %s netmask %s dev %s",ipaddr,sub_mask,gTraceRouteDev);
					//	sprintf(cmd_rmRoute, "route del -net %s netmask %s dev %s", ipaddr, sub_mask,gTraceRouteDev);
					//}else{
					//	sprintf(cmdbuf, "route add -net %s netmask %s gw %s dev %s", ipaddr, sub_mask,gateWay,gTraceRouteDev);
					//	sprintf(cmd_rmRoute, "route del -net %s netmask %s gw %s dev %s", ipaddr, sub_mask,gateWay,gTraceRouteDev);
					//}
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
					if( (if_index = get_wanindex_by_name(gTraceRouteDev)) < 0 )
							findIndexError = -1;
				
					if ( 0 == iptraceRouteIPver )
					{
#endif
					snprintf(devName, sizeof(devName) - 1, "%s%s", 
						( (4 == ipVersion) ? "ds." : "" ), gTraceRouteDev);

					sprintf(cmdbuf, "route add -host %s dev %s",ipaddr, devName);
					sprintf(cmd_rmRoute, "route del %s", ipaddr);
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
					}
					else
					{
						if( findIndexError != -1 )
						{
							sprintf(nodeName, "WanInfo_Entry%d", if_index);
							bzero(gateWay, sizeof(gateWay));
							tcapi_get(nodeName, "GateWay6", gateWay);
							sprintf(cmdbuf, ROUTE6_ADD_CMD, ipaddr
								, gateWay, gTraceRouteDev);
							sprintf(cmd_rmRoute, ROUTE6_DEL_CMD, ipaddr
								, gateWay, gTraceRouteDev);
						}
					}
#endif
					rtAddStatus = system(cmdbuf);
				//}
		}
	#else
		sprintf(cmdbuf,"route add -net %s netmask %s dev %s",ipaddr,sub_mask,gTraceRouteDev);
		rtAddStatus = system(cmdbuf);
	#endif
	}
	//malloc buf for read file
//	tmpbuf = malloc(MAX_TRACEROUTBUF_LEN);
//	if(tmpbuf == NULL)
//	{
//		tc_printf("\r\n[cwmp trace route task]mallco failed!");
//		mallocError == -1;
//	}

	resetCWMPTraceRouteResult();//reset the trace route Result before trace.
	rpc.flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_TERMINATED; 
	rpc.flags.theTraceRouteFlag |= CWMP_TRACEROUTE_INPROGRESS;//The cwmp trace route is in progress.
		
	//adding route fail or illegal route for adding or resolve host name error
	memset(cmdbuf,0,sizeof(cmdbuf));
	if((rtAddStatus < 0) || (isRTFlag == -1) || (resolveHostError == -1) || (mallocError == -1))
	{
		tc_printf("\r\nenter trace route error!");
		resetCWMPTraceRouteResult();
	}
	else
	{
		//do trace route
		//default value for trace route
		if(cwmp_traceroute_p.DataBlockSize)
			tmpdatablockSize = cwmp_traceroute_p.DataBlockSize;
		if(cwmp_traceroute_p.Timeout)
			tmptimeout = cwmp_traceroute_p.Timeout;
		if(cwmp_traceroute_p.NumberofTries)
			tmpnumberoftries = cwmp_traceroute_p.NumberofTries;	
		if(cwmp_traceroute_p.MaxHopCount)
			tmpmaxhopcount = cwmp_traceroute_p.MaxHopCount;

#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
		if ( 0 == iptraceRouteIPver )
		{
#endif
		sprintf(cmdbuf,"/usr/bin/traceroute -n -x 1"
					" -t %lu -m %lu -q %lu -w %lu -i %s %lu"
					, (DSCPValue << 2)
					, tmpmaxhopcount, tmpnumberoftries, tmptimeout/1000, ipaddr, tmpdatablockSize);
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
		}
		else
		{
			snprintf(cmdbuf, sizeof(cmdbuf) - 1
					, "/userfs/bin/traceroute6 -n -x 1 -I "
					"-t %lu -m %lu -q %lu -w %lu -i %s %s %lu"
					, (DSCPValue << 2)
					, tmpmaxhopcount
					, tmpnumberoftries
					, tmptimeout/1000
					, gTraceRouteDev
					, ipaddr
					, tmpdatablockSize);
		}
#endif

	#if 0	
	#if 1
		if((cwmp_traceroute_p.interval == 0) && (cwmp_traceroute_p.len == 0))
			sprintf(cmdbuf,"ping -q -M 1 -c %lu %s > %s",NumberOfRepetitions,ipaddr,path);
		else if((cwmp_traceroute_p.interval == 0) && (cwmp_traceroute_p.len != 0))	
			sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u %s > %s",NumberOfRepetitions,cwmp_traceroute_p.len,ipaddr,path);
		else if((cwmp_traceroute_p.interval != 0) && (cwmp_traceroute_p.len == 0))
			sprintf(cmdbuf,"ping -q -M 1 -c %d -W %lu %s > %s",NumberOfRepetitions,cwmp_traceroute_p.interval,ipaddr,path);
		else
			sprintf(cmdbuf,"ping -q -M 1 -c %d -s %u -W %lu %s > %s",NumberOfRepetitions,cwmp_traceroute_p.len,cwmp_traceroute_p.interval,ipaddr,path);
		//exec trace route
	#else
		strcpy(cmdbuf,"ping -q -M 1 -c 3 -s 32 -W 5000 192.168.1.12 > /tmp/cwmp/ippingdiagnostic_stat");//for test
	#endif
	#endif
		system(cmdbuf);
		/*remove temp pid file of traceroute*/
		sprintf(cmdbuf, "rm -f %s", CWMP_TRACEROUTE_PID_PATH);
		system(cmdbuf);
		if(rpc.flags.theTraceRouteFlag & CWMP_TRACEROUTE_TERMINATED)
		{
			tc_printf("\r\n [cwmpTraceRouteTask] TERMINATED   manually! we do not read traceroute result from file\n");
		}
		else
		{
			//read the traceroute result
			//fileRead(TRACEROUTEDIAGNOSTICFILE,tmpbuf,MAX_TRACEROUTBUF_LEN);
			//result = 0;
			result = getCWMPTraceRouteResult();
			unlink(TRACEROUTEDIAGNOSTICFILE);
	#if 0			
			if(cwmp_traceroute_p.RouteHopsNumberOfEntries >  tmpmaxhopcount)
			{
				tc_printf("\r\n[cwmpTraceRouteTask] getCWMPTraceRouteResult cwmp_traceroute_p.responses == 0!\n");
				result = -1;
			}
	#endif
		}
    }
	
	rpc.flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_INPROGRESS;
	
//	if(mallocError != -1)
//		free(tmpbuf);
	
	if(rpc.flags.theTraceRouteFlag & CWMP_TRACEROUTE_TERMINATED)
	{
		tc_printf("\r\n[cwmpTraceRouteTask] TERMINATED   manually!\n");
	}
	else
	{
	
		if (!result)
		{
#if defined(TCSUPPORT_CT_PON_GDV20) && defined(TCSUPPORT_CT_TRACEROUTEIPV6)
			strcpy(TraceRouteDiagnosticsStateTypeV20,"Complete");
#else
			strcpy(TraceRouteDiagnosticsStateType,"Complete");
#endif
		}else if(result==-2){
#if defined(TCSUPPORT_CT_PON_GDV20) && defined(TCSUPPORT_CT_TRACEROUTEIPV6)
				strcpy(TraceRouteDiagnosticsStateTypeV20,"Error_MaxHopCountExceeded");
#else
				strcpy(TraceRouteDiagnosticsStateType,"Error_MaxHopCountExceeded");
#endif
#if defined(TCSUPPORT_CT_PON_GD)
				cwmp_traceroute_p.ResponseTime = 0;
#endif
				tcdbg_printf("\r\n[cwmpTraceRouteTask] getCWMPTraceRouteResult MaxHopCountExceeded!\n");
		}else
		{
#if defined(TCSUPPORT_CT_PON_GD)
#if defined(TCSUPPORT_CT_PON_GDV20) && defined(TCSUPPORT_CT_TRACEROUTEIPV6)
			snprintf(TraceRouteDiagnosticsStateTypeV20,sizeof(TraceRouteDiagnosticsStateTypeV20), "%s", "Error_CannotResolveHostName");
#else
			snprintf(TraceRouteDiagnosticsStateType,sizeof(TraceRouteDiagnosticsStateType), "%s", "Error_CannotResolveHostName");
#endif
			cwmp_traceroute_p.ResponseTime = 0;
#else
			if((rtAddStatus != 0) || (isRTFlag == -1) ||(mallocError == -1))
			{
				strcpy(TraceRouteDiagnosticsStateType,"Error_Internal");
			}
			else if(resolveHostError == -1)
			{
			strcpy(TraceRouteDiagnosticsStateType,"Error_CannotResolveHostName");
			}
			else
			{
				strcpy(TraceRouteDiagnosticsStateType,"Error_Other");
			}
#endif
			//cwmp_traceroute_p.sent = NumberOfRepetitions;
		}
	}
#if defined(TCSUPPORT_CT_PON_GDV20) && defined(TCSUPPORT_CT_TRACEROUTEIPV6)
		if(strlen(cmd_rmRoute) != 0){
			system(cmd_rmRoute);
			memset(cmd_rmRoute,0,sizeof(cmd_rmRoute));
		}
		if ((!result) && (cwmp_traceroute_p.RouteHopsNumberOfEntries > 0))
			break;
	}
	strncpy(TraceRouteDiagnosticsStateType, TraceRouteDiagnosticsStateTypeV20, sizeof(TraceRouteDiagnosticsStateType));
#endif
	
	
	//reset all flag.
	rtAddStatus = 0;
	result = -1;
	isRTFlag = 0;
	resolveHostError = 0;
 	mallocError = 0;
	
	if(rpc.flags.theTraceRouteFlag & CWMP_TRACEROUTE_TERMINATED){//If the trace route test is terminated unusally, does not send inform.
		tc_printf("\r\n[cwmpTraceRouteTask] terminated unusally, does not send inform.!\n");
		rpc.flags.theTraceRouteFlag &= ~CWMP_TRACEROUTE_TERMINATED; 
	}else{
		/*Send the "8 DIAGNOSTICS COMPLETE" Event code*/
		//delete the route-----modify in the future
		//sprintf(cmdbuf,"route delete -net %s netmask %s dev %s",ipaddr,sub_mask,gTraceRouteDev);
		//rtAddStatus = system(cmdbuf);
		RebuildRouteHopsObject();
		setIPPingCompleteFlag(1);	
		sem_post(&SEM_IMFORM);
	}
#if 1//xyyou_gw
	//delete route 
	if((strlen(cmd_rmRoute)!=0)&&(rtAddStatus==0)){
		system(cmd_rmRoute);
	}
#endif

	/*Set p_traceroute varible to 0 means thread exit*/
	p_traceroute = 0;
	return NULL;
	
}
#endif

int cwmpTraceRouteTaskStart(void)
{
	int ret;
	//pthread_t p_traceroute;
	pthread_attr_t p_traceroute_attr;

	/*create thread*/
	ret = pthread_attr_init(&p_traceroute_attr);
	if(ret != 0)
	{
		printf("\r\nThread attribute creation fail!");
		return -1;
	}
	//need to set deached way
	ret = pthread_attr_setdetachstate(&p_traceroute_attr,PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		printf("\r\nSet attribute fail!");
		return -1;
	}

	ret = pthread_create(&p_traceroute,&p_traceroute_attr,(void *)cwmpTraceRouteTask,NULL);
	if(ret != 0)
	{
		printf("\r\nCreate thread fail!");
		return -1;
	}

	pthread_attr_destroy(&p_traceroute_attr);
}

void doCwmpTraceRoute(void)
{
	ip4a dnsQueryResult;

#ifdef TCSUPPORT_TRACEROUTE
	char cmdbuf[200]  = {0};
	#define TraceRoute_DNSURL "/tmp/ippingdiag.conf"
#endif
#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
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
#endif

#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
	memset(cmdbuf, 0, sizeof(cmdbuf));
	sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", traceRouteDomainName, gTraceRouteDev, IPPING_DNSURL);
	system(cmdbuf);

	cwmp_checkWan_by_name(gTraceRouteDev, &tr_wan_ipVersion);

#if defined(TCSUPPORT_CT_PON_GDV20)
	memset(iptraceRouteconvertIP, 0, sizeof(iptraceRouteconvertIP));
	memset(iptraceRouteconvertIPv6, 0, sizeof(iptraceRouteconvertIPv6));
#endif

	iptraceRouteIPver = -1;
	if( inet_aton(traceRouteDomainName, &cvtaddr.sin_addr) )
	{
		//ping diagnostic address is ipv4
		strcpy(iptraceRouteconvertIP, traceRouteDomainName);
		iptraceRouteIPver = 0;
	}
#ifdef IPV6
	else if( inet_pton(AF_INET6, traceRouteDomainName, (struct sockaddr *) &ipv6addr.sin6_addr) >0 )
	{	
		/* ping diagnostic address is ipv6 */
		strcpy(iptraceRouteconvertIP, traceRouteDomainName);
		iptraceRouteIPver = 1;
	}
#endif
	else
	{
		/* ping diagnostic address is a domain name */
		memset( &hints, 0 , sizeof(hints));
		hints.ai_family = ( 2 == tr_wan_ipVersion ) ? AF_INET6 : AF_INET;
		hints.ai_socktype = SOCK_DGRAM;  		//DNS is UDP packet
		hints.ai_protocol = IPPROTO_UDP;

#if defined(TCSUPPORT_CT_PON_GDV20)
#ifdef IPV6
		memset( &hintsv6, 0 , sizeof(hintsv6));
		hintsv6.ai_family = AF_INET6;
		hintsv6.ai_socktype = SOCK_DGRAM;		//DNS is UDP packet
		hintsv6.ai_protocol = IPPROTO_UDP;				
#endif
#endif

		if ( 1 == tr_wan_ipVersion
			|| 2 == tr_wan_ipVersion ) /* ipv4 only or ipv6 only */
		{
			if ( tc_getaddrinfo(traceRouteDomainName, NULL, &hints, &res, 5) )
			{
				goto DNS_RESOLVE_END;
			}
		}
		else /* ipv4/ipv6 or dslite+ipv6 */
		{
			if ( tc_getaddrinfo(traceRouteDomainName, NULL, &hints, &res, 5) )
			{
#ifdef IPV6
				hints.ai_family = AF_INET6;
					if( tc_getaddrinfo(traceRouteDomainName, NULL, &hints, &res, 5) )
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
				if( tc_getaddrinfo(traceRouteDomainName, NULL, &hintsv6, &resv6, 5) == 0 )
				{
					ipv6addr2 = (struct sockaddr_in6 *)resv6->ai_addr;
					memset(buf, 0, sizeof(buf));
					inet_ntop(AF_INET6,&ipv6addr2->sin6_addr, buf, INET6_ADDRSTRLEN);
					strcpy(iptraceRouteconvertIPv6, buf);
				}
			}
#endif
#endif
		}

		switch ( res->ai_family )
		{
			case AF_INET:			/* ipv4 */
				cvtaddr2 = (struct sockaddr_in *) res->ai_addr;
				strcpy(iptraceRouteconvertIP, inet_ntoa(cvtaddr2->sin_addr));
				iptraceRouteIPver = 0;
				break;
#ifdef IPV6
			case AF_INET6:			/* ipv6 */
				ipv6addr2 = (struct sockaddr_in6 *)res->ai_addr;
				memset(buf, 0, sizeof(buf));
				inet_ntop(AF_INET6,&ipv6addr2->sin6_addr, buf, INET6_ADDRSTRLEN);
				strcpy(iptraceRouteconvertIP, buf);
				iptraceRouteIPver = 1;
				break;
#endif
			default:
				break;
		}
	}

#else
	if(traceRouteDomainName != NULL)
	{
#ifdef TCSUPPORT_TRACEROUTE
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "/bin/echo \"%s %s\" > %s", traceRouteDomainName, gTraceRouteDev, TraceRoute_DNSURL);
		system(cmdbuf);
#endif

		dnsQueryResult = resolveName(traceRouteDomainName);
#ifdef TCSUPPORT_TRACEROUTE
		unlink(TraceRoute_DNSURL);
#endif

		if(!dnsQueryResult)
		{
			cwmp_traceroute_p.target = 0L;
		}
		else
		{
			if(resolve(inet_ntoa(dnsQueryResult),&cwmp_traceroute_p.target,NULL) != 0)
			{
				cwmp_traceroute_p.target = 0L;
			}
			else
			{
				//cwmp_traceroute_p.target is right
			}
		}
		os_free(traceRouteDomainName);
		traceRouteDomainName = NULL;
	}
	else
	{
		cwmp_traceroute_p.target = 0L;
	}
#endif

#if defined(TCSUPPORT_CT_TRACEROUTEIPV6)
DNS_RESOLVE_END:
		unlink(IPPING_DNSURL);
#endif
	
#ifndef THREAD_REDUCE
	if(p_traceroute == 0)
	{
		tc_printf("\r\ncreate thread to do ip traceroute diagnostic!");
		cwmpTraceRouteTaskStart();
	}
	else
		sem_post(&CWMP_TRACEROUTE);
#else
	if(p_traceroute == 0)
	{
		tc_printf("\r\ncreate thread to do ip traceroute diagnostic!");
		cwmpTraceRouteTaskStart();
	}
	else
	{
		tc_printf("\r\n----------------------------p_traceroute is not 0, error occured!-------------------------------");
		//cwmpTraceRouteTask();
	}
#endif
		
}
/**************************trace route diagnostic end***********************************/
