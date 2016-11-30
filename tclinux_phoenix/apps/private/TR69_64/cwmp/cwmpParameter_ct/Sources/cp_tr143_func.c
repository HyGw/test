#include "Global_res.h"
#include "cp_tr143.h"

/*******************  InternetGatewayDevice.Capabilities. start ************************/
int8 getDiagDldTsportsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char portValue[8] = "HTTP";
	return getTheValue(value, type, deviceNode, portValue);
}
int8 setDiagDldTsportsValue(void* value, struct DeviceNode*  deviceNode)
{
	char *portValue = (char*)(value);

	/*we only support HTTP protocol now, FTP support may added in future*/
	if ( strcmp(portValue, "HTTP") != 0 ) {
		return -1;
	}
	
	return 0;
}
int8 getDiagDldTsportsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDiagDldTsports;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setDiagDldTsportsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDiagDldTsports;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDiagUpTsportsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char portValue[8] = "HTTP";
	return getTheValue(value, type, deviceNode, portValue);
}
int8 setDiagUpTsportsValue(void* value, struct DeviceNode*  deviceNode)
{
	char *portValue = (char*)(value);

	/*we only support HTTP protocol now, FTP support may added in future*/
	if ( strcmp(portValue, "HTTP") != 0 ) {
		return -1;
	}
	
	return 0;
}
int8 getDiagUpTsportsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDiagUpTsports;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDiagUpTsportsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDiagUpTsports;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}


/*******************  InternetGatewayDevice.DownloadDiagnostics. start **********************/
static uint32 DSCPValue = 0;
uint16 EthernetPriorityValue = 0;

void resetDownloadInfo()
{
	memset( &rpc.loadDiagInfo.sysTimeTCPOpenRequest, 0, sizeof(rpc.loadDiagInfo.sysTimeTCPOpenRequest) );
	memset( &rpc.loadDiagInfo.sysTimeTCPOpenResponse, 0, sizeof(rpc.loadDiagInfo.sysTimeTCPOpenResponse) );
	memset( &rpc.loadDiagInfo.sysTimeROM, 0, sizeof(rpc.loadDiagInfo.sysTimeROM) );
	memset( &rpc.loadDiagInfo.sysTimeBOM, 0, sizeof(rpc.loadDiagInfo.sysTimeBOM) );
	memset( &rpc.loadDiagInfo.sysTimeEOM, 0, sizeof(rpc.loadDiagInfo.sysTimeEOM) );

	rpc.loadDiagInfo.testBytesReceived = 0;
	rpc.loadDiagInfo.BOMInOctet = 0;
	rpc.loadDiagInfo.EOMInOctet = 0;
}

int8 resetCwmpDownloadDiagnostics(void)
{
	if ( rpc.flags.theDownloadDiagFlag & CWMP_DIAGNOSTICS_INPROGRESS ) {
		rpc.flags.theDownloadDiagFlag |= CWMP_DIAGNOSTICS_TERMINATED;
	}
	
	setDiagnosticsCompleteFlag(0);
	
	return 0;
}

#if defined(TCSUPPORT_CT_PON_SC)
char DldSampledValuesBuff[512]={0};
int8 calcDldSampledValues(void){
	char nodeName[32] = {0};
	char nodeVal[10] = {0};
	char tmpvalue[64] = {0};
	char *ptmp = NULL;
	int i = 0;
	int startVal = 0;
	double dValue = 0;

	snprintf(tmpvalue, sizeof(tmpvalue) - 1, "%s"
					, rpc.loadDiagInfo.interfaceName);
	if ( strstr(tmpvalue, "nas")
		&& (ptmp = strstr(tmpvalue, "_")) )
		ptmp[0] = '.';					
	snprintf(nodeName, sizeof(nodeName) - 1,
			"Info_%s",
			tmpvalue);
	memset( DldSampledValuesBuff, 0, sizeof(DldSampledValuesBuff) );
	for(i=0;i<21;i++){
		memset( nodeVal, 0, sizeof(nodeVal) );
		tcapi_get( nodeName, "rxbytes", nodeVal );
		if(i>0){
			if(strlen(DldSampledValuesBuff)>0)
				strcat(DldSampledValuesBuff,"|");
			dValue = atoi(nodeVal)-startVal;
			sprintf(tmpvalue,"%.2f",dValue/i/1024);
			strcat(DldSampledValuesBuff,tmpvalue);
		}
		else
			startVal = atoi(nodeVal);		
		sleep(1);
	}
	return 0;
}
#endif

int8 getDldDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, getDldUldDiagStateType() );
}
int8 setDldDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	char vtype[32];
	
	if ( strlen((char*)value) > 31 ) {
		return -1;
	}
	
	strcpy(vtype,(char*)value);		
	
	if ( strcmp(vtype,"Requested") ==0 ) {
		
		/*If the diagnostics is in progress, terminate it first.*/
		resetCwmpDownloadDiagnostics();
		resetDownloadInfo();
#if defined(TCSUPPORT_CT_TR143_WGET_DOWNLOAD)
		rpc.flags.theDownloadDiagFlag |= CWMP_DIAGNOSTICS_WGET_DOWNLOAD_ACTIVE;
#else
		rpc.flags.theDownloadDiagFlag |= CWMP_DIAGNOSTICS_ACTIVE;
#endif
		setDldUldDiagStateType("Requested");
		rpc.fileTransferStruct->transferType = Download;
#if defined(TCSUPPORT_CT_PON_SC)
#if defined(TCSUPPORT_CT_TR143_WGET_DOWNLOAD)
#else
		timerStartS(1000,(TimerCallback)calcDldSampledValues,NULL);
#endif
#endif
	}
	else {
		return -1;
	}

	return 0;	
}
int8 getDldDiagnosticsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldDiagnostics;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldDiagnosticsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldDiagnostics;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

static char DldUldInterface[128];
int8 getDldInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, DldUldInterface);
}
int8 setDldInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34];
	char gateway[32];
	char defLanInterface[] = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1";
	char defWanInterface[] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char *setInterface = NULL;
	int lanIfLen = 0, wanIfLen = 0;
 	uint32 pvcNum = 0,entryNum = 0;
	DeviceNodePtr Node = NULL;
	char *entryPtr = NULL;
	int nasFlag = 1;

	memset( nodeName, 0, sizeof(nodeName) );
	memset( gateway, 0, sizeof(gateway) );
	
	setInterface = (char*)value;
	lanIfLen = strlen( defLanInterface );
	wanIfLen = strlen( defWanInterface );
	entryPtr = strstr(setInterface,"WANPPPConnection") ;
	if( entryPtr != NULL )
	{
		nasFlag = 0;
		tcdbg_printf("WAN PPP Connection\n");
		tcdbg_printf("entryPtr:%s\n",entryPtr);
		entryPtr += sizeof("WANPPPConnection");
		tcdbg_printf("entryPtr:%s\n",entryPtr);
	}
	else
	{
		entryPtr = strstr(setInterface,"WANIPConnection") ;
		if( entryPtr != NULL )
		{
			nasFlag = 1;
			tcdbg_printf("WAN IP Connection\n");
			tcdbg_printf("entryPtr:%s\n",entryPtr);
			entryPtr += sizeof("WANIPConnection");
			tcdbg_printf("entryPrt:%s\n",entryPtr);
		}
		else
		{
			tcdbg_printf(" Error Connection\n");
			return -1;
		}
	}
	
	/*remove check for test*/
#if 0
	/*check if the interface is valid, may it do not need.*/
	cwmpSearchDeviceNodeByName(setInterface, rpc.IGDDeviceRootPtr, &Node);
	if ( Node == NULL ) {
		/*interface in not exist*/
		tc_printf( "The setting interface is not exist.\n");
		return -1;
	}
#endif

	if ( strncmp(defWanInterface, setInterface, wanIfLen) == 0 ) {
		/*wan interface, get the interface's gateway*/

		/*we have find out the interface.*/
		pvcNum = atoi( setInterface + wanIfLen );
		entryNum = atoi(entryPtr);

		tcdbg_printf("pvc num is %d, entry num is %d\n", pvcNum, entryNum);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		if(FindWanNodeMapping(&pvcNum, &entryNum) != 0){
			tc_printf( "The pvc is not exist.\n" );
			return -1;
		}
		pvcNum++;
		entryNum++;
#else
		/*check if the pvcNum is valid.*/
		if ( pvcNum < 1 || pvcNum > 8 ||entryNum < 1 ||entryNum > 8 ) {
			tc_printf( "The pvc is not exist.\n" );
			return -1;
		}
#endif

		/*check if the pvcNum is valid.*/
		if ( pvcNum < 1 || pvcNum > 8 ||entryNum < 1 ||entryNum > 8 ) {
			tc_printf( "The pvc is not exist.\n" );
			return -1;
		}

		/*fill the interface name to struct.*/
		if(nasFlag == 1)
			sprintf( rpc.loadDiagInfo.interfaceName, "nas%d_%d", pvcNum - 1,entryNum -1);
		else
			sprintf( rpc.loadDiagInfo.interfaceName, "ppp%d", (pvcNum - 1) <<3 | (entryNum -1)  );

		/*get the gateway of this pvc.*/
		sprintf( nodeName, "WanInfo_Entry%d",  (pvcNum - 1) <<3 | (entryNum -1) );
		tcapi_get( nodeName, "GateWay", gateway );
	}
	else if ( strncmp(defLanInterface, setInterface, lanIfLen) == 0 ) {
		/*lan interface, get the interface's gateway*/

		/*fill the interface name to struct.*/
		strcpy( rpc.loadDiagInfo.interfaceName, "br0" );

		/*the lan ip of CPE is lan side gateway*/
		tcapi_get( "Lan_Entry0", "IP", gateway );
	}
	else if ( strcmp(setInterface, "") == 0 ) {
		/*empty string*/
		strcpy( rpc.loadDiagInfo.interfaceName, "" );
	}
	else {
		return -1;
	}

	/*save set value*/
	strcpy( DldUldInterface, setInterface );

	/*save gateway*/
	strcpy( rpc.loadDiagInfo.diagGateway, gateway );

	tcdbg_printf("DldUldInterface = %s,  rpc.loadDiagInfo.diagGateway = %s \n", setInterface,gateway);
	return 0;
		
}
int8 getDldInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldInterface;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDldInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldDownloadURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, rpc.loadDiagInfo.transferURL);
}

int8 setDldDownloadURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char *HTTPURL = NULL;

	HTTPURL = (char*)(value); 
	
	if ( strlen(HTTPURL) > 255 || strlen(HTTPURL) < 7 ) {
		return -1;
	}

	if ( strncmp(HTTPURL,"http://", 7) != 0 ) {
		return -1;
	}
	
	strcpy(rpc.loadDiagInfo.transferURL, HTTPURL);
	
	return 0;
}

int8 getDldDownloadURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldDownloadURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDldDownloadURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldDownloadURL;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vDSCPValue[10];

	sprintf(vDSCPValue, "%lu",DSCPValue);

	return getTheValue(value, type, deviceNode, vDSCPValue);
}
int8 setDldDSCPValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue = 0;

	tempValue = *(uint32*)value;
	if ( tempValue > 63 ) {
		return -1;
	}
	
	DSCPValue = tempValue;
	
	return 0;
}
int8 getDldDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldDSCP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDldDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldDSCP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldEthernetPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vPriorityValue[10];

	sprintf(vPriorityValue, "%d",EthernetPriorityValue);
	
	return getTheValue(value, type, deviceNode, vPriorityValue);
}
int8 setDldEthernetPriorityValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue = 0;

	tempValue = *(uint32*)value;
	if ( tempValue > 7 ) {
		return -1;
	}
	
	EthernetPriorityValue = tempValue;
	
	return 0;
}
int8 getDldEthernetPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldEthernetPriority;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDldEthernetPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldEthernetPriority;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldROMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldROMTime[60];

	memset( DldROMTime, 0, sizeof(DldROMTime) );

	/*convert time to format that tr143 spec required*/
#if defined(TCSUPPORT_CT_DIAG_MICROSECOND)
	convertSysTimeMicrosecondToString( rpc.loadDiagInfo.sysTimeROM, DldROMTime, sizeof(DldROMTime) );
#else
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeROM, DldROMTime, sizeof(DldROMTime) );
#endif

	return getTheValue(value, type, deviceNode, DldROMTime);
}
int8 getDldROMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldROMTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldROMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldROMTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldBOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldBOMTime[60];

	memset( DldBOMTime, 0, sizeof(DldBOMTime) );

	/*convert time to format that tr143 spec required*/
#if defined(TCSUPPORT_CT_DIAG_MICROSECOND)
	convertSysTimeMicrosecondToString( rpc.loadDiagInfo.sysTimeBOM, DldBOMTime, sizeof(DldBOMTime) );
#else
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeBOM, DldBOMTime, sizeof(DldBOMTime) );
#endif

	return getTheValue(value, type, deviceNode, DldBOMTime);
}
int8 getDldBOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldBOMTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldBOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldBOMTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldEOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldEOMTime[60];

	memset( DldEOMTime, 0, sizeof(DldEOMTime) );

	/*convert time to format that tr143 spec required*/
#if defined(TCSUPPORT_CT_DIAG_MICROSECOND)
	convertSysTimeMicrosecondToString( rpc.loadDiagInfo.sysTimeEOM, DldEOMTime, 60 );
#else
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeEOM, DldEOMTime, 60 );
#endif

	return getTheValue(value, type, deviceNode, DldEOMTime);
}
int8 getDldEOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldEOMTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldEOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldEOMTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldTestBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempValue[10] = "";

	sprintf(tempValue, "%lu",rpc.loadDiagInfo.testBytesReceived);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getDldTestBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldTestBytesReceived;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldTestBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldTestBytesReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldTotalBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempValue[10];
	uint32  InOctet = 0;

	if ( rpc.loadDiagInfo.EOMInOctet > rpc.loadDiagInfo.BOMInOctet ) {
		InOctet = rpc.loadDiagInfo.EOMInOctet - rpc.loadDiagInfo.BOMInOctet;
	}
	
	sprintf(tempValue, "%lu",InOctet);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getDldTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldTotalBytesReceived;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldTotalBytesReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldTCPOpenRequestTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldTCPOpenRequestTime[60];

	memset( DldTCPOpenRequestTime, 0, sizeof(DldTCPOpenRequestTime) );

	/*convert time to format that tr143 spec required*/
#if defined(TCSUPPORT_CT_DIAG_MICROSECOND)
	convertSysTimeMicrosecondToString( rpc.loadDiagInfo.sysTimeTCPOpenRequest, DldTCPOpenRequestTime, 60 );
#else
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeTCPOpenRequest, DldTCPOpenRequestTime, 60 );
#endif

	return getTheValue(value, type, deviceNode, DldTCPOpenRequestTime);
}
int8 getDldTCPOpenRequestTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldTCPOpenRequestTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldTCPOpenRequestTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldTCPOpenRequestTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getDldTCPOpenResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldTCPOpenResponseTime[60];

	memset( DldTCPOpenResponseTime, 0, sizeof(DldTCPOpenResponseTime) );

	/*convert time to format that tr143 spec required*/
#if defined(TCSUPPORT_CT_DIAG_MICROSECOND)
	convertSysTimeMicrosecondToString( rpc.loadDiagInfo.sysTimeTCPOpenResponse, DldTCPOpenResponseTime, 60 );
#else
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeTCPOpenResponse, DldTCPOpenResponseTime, 60 );
#endif

	return getTheValue(value, type, deviceNode, DldTCPOpenResponseTime);
}
int8 getDldTCPOpenResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldTCPOpenResponseTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setDldTCPOpenResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldTCPOpenResponseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_PON_SC)
int8 getDldSampledValuesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CT_TR143_WGET_DOWNLOAD)
	FILE *fp = NULL;

	fp = fopen("/tmp/tmp_tr143samplevalue", "r");
	if ( fp )
	{
		fread(DldSampledValuesBuff, sizeof(DldSampledValuesBuff), 1, fp);
		fclose(fp);	
	}
#endif
	return getTheValue(value, type, deviceNode, DldSampledValuesBuff);
}

int8 getDldSampledValuesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aDldSampledValues;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDldSampledValuesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aDldSampledValues;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}
#endif

/*******************  InternetGatewayDevice.UploadDiagnostics. start **********************/
int8 resetCwmpUploadDiagnostics(void)
{
	if(rpc.flags.theUploadDiagFlag & CWMP_DIAGNOSTICS_INPROGRESS)
		rpc.flags.theUploadDiagFlag |= CWMP_DIAGNOSTICS_TERMINATED;
	return 0;
}

int8 getUldDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue( value, type, deviceNode, getDldUldDiagStateType() );
}
int8 setUldDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	char vtype[32];
	
	if ( strlen((char*)value) > 31 ) {
		return -1;
	}
	
	strcpy(vtype,(char*)value);		
	
	if ( strcmp(vtype,"Requested") ==0 ) {
		/*If the diagnostics is in progress, terminate it first.*/
		resetCwmpUploadDiagnostics();
#if !defined(TCSUPPORT_TR143_CURL_UPLOAD) 
		rpc.flags.theUploadDiagFlag |= CWMP_DIAGNOSTICS_ACTIVE;
#endif
		setDldUldDiagStateType("Requested");
		rpc.fileTransferStruct->transferType = Upload;
	}
	else {
		return -1;
	}

	return 0;
}
int8 getUldDiagnosticsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldDiagnostics;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUldDiagnosticsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldDiagnostics;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue(value, type, deviceNode, DldUldInterface);
}
int8 setUldInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	return setDldInterfaceValue( value, deviceNode );
}
int8 getUldInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldInterface;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUldInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldUploadURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	return getTheValue( value, type, deviceNode, rpc.loadDiagInfo.transferURL );
}
int8 setUldUploadURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char *HTTPURL = NULL;

	HTTPURL = (char*)(value); 
	
	if ( strlen(HTTPURL) > 255 ||strlen(HTTPURL) < 7 ) {
		return -1;
	}

	if ( strncmp(HTTPURL,"http://", 7) != 0 ) {
		return -1;
	}
	
	strcpy(rpc.loadDiagInfo.transferURL, HTTPURL);
	return 0;
}
int8 getUldUploadURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldUploadURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUldUploadURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldUploadURL;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vDSCPValue[10];

	sprintf(vDSCPValue, "%lu",DSCPValue);

	return getTheValue(value, type, deviceNode, vDSCPValue);
}
int8 setUldDSCPValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue = 0;

	tempValue = *(uint32*)value;
	
	if ( tempValue > 63 ) {
		return -1;
	}
	
	DSCPValue = tempValue;

	return 0;
}
int8 getUldDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldDSCP;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUldDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldDSCP;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldEthernetPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  vPriorityValue[10];

	sprintf(vPriorityValue, "%d",EthernetPriorityValue);

	return getTheValue(value, type, deviceNode, vPriorityValue);
}
int8 setUldEthernetPriorityValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempValue = 0;

	tempValue = *(uint32*)value;
	
	if ( tempValue > 7 ) {
		return -1;
	}
	
	EthernetPriorityValue = tempValue;

	return 0;
}
int8 getUldEthernetPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldEthernetPriority;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUldEthernetPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldEthernetPriority;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldTestFileLengthValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char fileleng[10];
	
	sprintf(fileleng, "%lu", rpc.loadDiagInfo.lengToSend);
	
	return getTheValue(value, type, deviceNode, fileleng);
}
int8 setUldTestFileLengthValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 testLeng = *((uint32*)value); 

	rpc.loadDiagInfo.lengToSend = testLeng;
	
	return 0;
}
int8 getUldTestFileLengthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldTestFileLength;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUldTestFileLengthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldTestFileLength;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldROMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldROMTime[60];

	memset( DldROMTime, 0, sizeof(DldROMTime) );

	/*convert time to format that tr143 spec required*/
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeROM, DldROMTime, 60 );

	return getTheValue(value, type, deviceNode, DldROMTime);
}
int8 getUldROMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldROMTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUldROMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldROMTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldBOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldBOMTime[60];

	memset( DldBOMTime, 0, sizeof(DldBOMTime) );

	/*convert time to format that tr143 spec required*/
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeBOM, DldBOMTime, 60 );

	return getTheValue(value, type, deviceNode, DldBOMTime);
}
int8 getUldBOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldBOMTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUldBOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldBOMTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldEOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char DldEOMTime[60];

	memset( DldEOMTime, 0, sizeof(DldEOMTime) );

	/*convert time to format that tr143 spec required*/
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeEOM, DldEOMTime, 60 );

	return getTheValue(value, type, deviceNode, DldEOMTime);
}
int8 getUldEOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldEOMTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUldEOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldEOMTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldTotalBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempValue[10];
	uint32  outOctet = 0;

	if ( rpc.loadDiagInfo.EOMOutOctet > rpc.loadDiagInfo.BOMOutOctet ) {
		outOctet = rpc.loadDiagInfo.EOMOutOctet - rpc.loadDiagInfo.BOMOutOctet;
	}

	sprintf(tempValue, "%lu",outOctet);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getUldTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldTotalBytesSent;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUldTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldTotalBytesSent;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldTCPOpenRequestTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char UldTCPOpenRequestTime[60];

	memset( UldTCPOpenRequestTime, 0, sizeof(UldTCPOpenRequestTime) );

	/*convert time to format that tr143 spec required*/
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeTCPOpenRequest, UldTCPOpenRequestTime, 60 );

	return getTheValue(value, type, deviceNode, UldTCPOpenRequestTime);
}
int8 getUldTCPOpenRequestTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldTCPOpenRequestTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUldTCPOpenRequestTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldTCPOpenRequestTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUldTCPOpenResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char UldTCPOpenResponseTime[60];

	memset( UldTCPOpenResponseTime, 0, sizeof(UldTCPOpenResponseTime) );

	/*convert time to format that tr143 spec required*/
	convertSysTimeToString( rpc.loadDiagInfo.sysTimeTCPOpenResponse, UldTCPOpenResponseTime, 60 );

	return getTheValue(value, type, deviceNode, UldTCPOpenResponseTime);
}
int8 getUldTCPOpenResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUldTCPOpenResponseTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUldTCPOpenResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUldTCPOpenResponseTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

/*******************  InternetGatewayDevice.UDPEchoConfig. start **********************/
int8 getUDPEchoEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char udpEchoEnable[4];

	memset( udpEchoEnable, 0, sizeof(udpEchoEnable) );

	tcapi_get( TR143UDPEcho, "Enable", udpEchoEnable );

	if ( !strcmp( udpEchoEnable, "Yes" ) ) {
		strcpy( udpEchoEnable, "1" );
	}
	else {
		strcpy( udpEchoEnable, "0" );
	}

	return getTheValue(value, type, deviceNode, udpEchoEnable);
}
int8 setUDPEchoEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean tempEnable = 0;
	char oldEnable[10];
	boolean oldVal = 0;

	memset( oldEnable, 0, sizeof(oldEnable) );
	
	tempEnable = *(boolean *)value;

	tcapi_get( TR143UDPEcho, "Enable", oldEnable );
	oldVal = atoi( oldEnable );

	if ( tempEnable == 0 ) {
		
		closeUDPSocket();
		tcapi_set( TR143UDPEcho, "Enable", "No" );
	}
	else if ( tempEnable == 1 ) {
		/*only last udpEcho is disabled we clear the stats*/
		if ( oldVal == 0 ) {
			clearUDPEchoStats();
		}
		tcapi_set( TR143UDPEcho, "Enable", "Yes" );
	}
	else {
		return -1;
	}
	SetCfgNodeFlag ( TR143UDPEcho, -1 );

	return 0;
}
int8 getUDPEchoEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aUDPEchoEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUDPEchoEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUDPEchoEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char getInterface[64];

	memset( getInterface, 0, sizeof(getInterface) );

	tcapi_get( TR143UDPEcho, "Interface", getInterface );

	return getTheValue(value, type, deviceNode, getInterface);
}
int8 setUDPEchoInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{	
	char *setInterface = NULL;
	char defLanInterface[] = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1";
	char defWanInterface[] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";

	int lanIfLen = 0, wanIfLen = 0;

	setInterface = (char*)value;
	lanIfLen = strlen( defLanInterface );
	wanIfLen = strlen( defWanInterface );

	if ( strncmp(defWanInterface, setInterface, wanIfLen) == 0 || strncmp(defLanInterface, setInterface, lanIfLen) == 0 || strcmp( setInterface, "" ) == 0 ) {
		tcapi_set( TR143UDPEcho, "Interface", setInterface );
		closeUDPSocket();
	}
	else {
		return -1;
	}


	SetCfgNodeFlag( TR143UDPEcho, -1 );

	return 0;
}
int8 getUDPEchoInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aUDPEchoInterface;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUDPEchoInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUDPEchoInterface;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoSourceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempSourceIPAddress[32];

	memset( tempSourceIPAddress, 0, sizeof(tempSourceIPAddress) );

	tcapi_get( TR143UDPEcho, "SourceIPAddress", tempSourceIPAddress );

	return getTheValue(value, type, deviceNode, tempSourceIPAddress);
}
int8 setUDPEchoSourceIPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char *tempValue = NULL;

	tempValue = (char *)value;
	

	if ( InternalClientIpCheck(tempValue) != 1 ) {
		return -1;
	}
	
	tcapi_set( TR143UDPEcho, "SourceIPAddress", tempValue );
	SetCfgNodeFlag( TR143UDPEcho, -1 );

	return 0;
}
int8 getUDPEchoSourceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aEchoSourceIPAddress;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUDPEchoSourceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aEchoSourceIPAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoUDPPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUDPEchoUDPPort[32];

	memset( tempUDPEchoUDPPort, 0, sizeof(tempUDPEchoUDPPort) );

	tcapi_get( TR143UDPEcho, "UDPPort", tempUDPEchoUDPPort );
	
	return getTheValue(value, type, deviceNode, tempUDPEchoUDPPort);
}
int8 setUDPEchoUDPPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 tempPort = 0;
	char oldPort[10], newPort[10];
	
	memset( oldPort, 0, sizeof(oldPort) );
	memset( newPort, 0, sizeof(newPort) );

	tempPort = *((uint32*)value);
	sprintf( newPort, "%lu", tempPort );
	
	if ( checkPortValid( tempPort ) == 0 ) {
		tc_printf( "The Port is not valid for it has been used.\n" );
		return -1;
	}

	tcapi_get( TR143UDPEcho, "UDPPort", oldPort );

	tcdbg_printf( "oldPort is %s, and newPort is %s\n", oldPort, newPort );

	/*if port is changed, we should close the old socket*/
	if ( strcmp( oldPort, newPort ) != 0 ) {
		
		tcapi_set( TR143UDPEcho, "UDPPort", newPort );

		closeUDPSocket();
	}

	SetCfgNodeFlag( TR143UDPEcho, -1 );

	return 0;
}
int8 getUDPEchoUDPPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUDPEchoUDPPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUDPEchoUDPPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTr143Attr.aUDPEchoUDPPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoEchoPlusEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUDPEchoPlusEnabled[4] = "";

	tcapi_get( TR143UDPEcho, "EchoPlusEnabled", tempUDPEchoPlusEnabled );

	if ( !strcmp( tempUDPEchoPlusEnabled, "Yes" ) ) {
		strcpy( tempUDPEchoPlusEnabled, "1" );
	}
	else {
		strcpy( tempUDPEchoPlusEnabled, "0" );
	}
	
	return getTheValue(value, type, deviceNode, tempUDPEchoPlusEnabled);
}
int8 setUDPEchoEchoPlusEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	boolean tempEnable = 0;

	tempEnable = *(boolean *)value;
	

    if ( tempEnable == 0 ) {
		tcapi_set( TR143UDPEcho, "EchoPlusEnabled", "No" );
	}
	else if ( tempEnable == 1 ) {
		tcapi_set( TR143UDPEcho, "EchoPlusEnabled", "Yes" );
	}
	else {
		return -1;
	}
	SetCfgNodeFlag( TR143UDPEcho, -1 );
	
	return 0;	
}
int8 getUDPEchoEchoPlusEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUDPEchoPlusEnabled;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUDPEchoEchoPlusEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aUDPEchoPlusEnabled;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoPlusSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char UDPEchoPlusSupported[4] = "1";

	return getTheValue(value, type, deviceNode, UDPEchoPlusSupported);
}
int8 getUDPEchoPlusSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aUDPEchoPlusSupported;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setUDPEchoPlusSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aUDPEchoPlusSupported;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUDPEchoPacketsReceived[8] = "";
	
	sprintf( tempUDPEchoPacketsReceived, "%lu", rpc.UDPEchoInfo.packetReceived );
	
	return getTheValue(value, type, deviceNode, tempUDPEchoPacketsReceived);
}
int8 getUDPEchoPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aUDPEchoPacketsReceived;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUDPEchoPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aUDPEchoPacketsReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoPacketsRespondedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUDPEchoPacketsResponded[8] = "";
	
	sprintf(tempUDPEchoPacketsResponded, "%lu", rpc.UDPEchoInfo.packetResponsed );
	
	return getTheValue(value, type, deviceNode, tempUDPEchoPacketsResponded);
}
int8 getUDPEchoPacketsRespondedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aUDPEchoPacketsResponded;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUDPEchoPacketsRespondedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aUDPEchoPacketsResponded;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUDPEchoBytesReceived[8] = "";
	
	sprintf(tempUDPEchoBytesReceived, "%lu", rpc.UDPEchoInfo.bytesReceived );

	return getTheValue(value, type, deviceNode, tempUDPEchoBytesReceived);
}
int8 getUDPEchoBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcTr143Attr.aUDPEchoBytesReceived;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUDPEchoBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aUDPEchoBytesReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getUDPEchoBytesRespondedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  tempUDPEchoBytesResponded[8] ="";
	
	sprintf( tempUDPEchoBytesResponded, "%lu", rpc.UDPEchoInfo.bytesResponsed );
	
	return getTheValue(value, type, deviceNode, tempUDPEchoBytesResponded);
}
int8 getUDPEchoBytesRespondedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aUDPEchoBytesResponded;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setUDPEchoBytesRespondedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aUDPEchoBytesResponded;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getTimeFirstPacketReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char timeString[60];

	memset( timeString, 0, sizeof(timeString) );

	/*convert time to format that tr143 spec required*/
	convertSysTimeToString( rpc.UDPEchoInfo.sysTimeFirstRcv, timeString, 60 );

	return getTheValue(value, type, deviceNode, timeString);
}
int8 getTimeFirstPacketReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aTimeFirstPacketReceived;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setTimeFirstPacketReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aTimeFirstPacketReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	return result;
}

int8 getTimeLastPacketReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char timeString[60];

	memset( timeString, 0, sizeof(timeString) );

	/*convert time to format that tr143 spec required*/
	convertSysTimeToString( rpc.UDPEchoInfo.sysTimeRecentRcv, timeString, 60 );

	return getTheValue(value, type, deviceNode, timeString);
}
int8 getTimeLastPacketReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcTr143Attr.aTimeLastPacketReceived;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)  
		*notification = None;
	
	return result;
}
int8 setTimeLastPacketReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcTr143Attr.aTimeLastPacketReceived;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= TR143_ATTR_FLAG;
	
	return result;
}

char * convertSysTimeToString(struct timeval tv, char *timeStr, int len)
{
	time_t t_sec;
	suseconds_t t_usec;
	struct tm st;
	char buf[60];

	/*check rev buf if is NULL.*/
	if( NULL == timeStr ) {
		return NULL;
	}
	
	memset( buf, 0, sizeof(buf) );
	
	t_sec = tv.tv_sec;
	t_usec = tv.tv_usec;

	/*check if tv is 0*/
	if ( 0 == t_sec && 0 == t_usec ) {
		return NULL;
	}
	
	/*convert t_set to struct tm format*/
	memcpy( &st, localtime(&t_sec), sizeof(struct tm) );

	sprintf( buf, "%04d-%02d-%02dT%02d:%02d:%02d:%03d", 
		st.tm_year + 1900,  (st.tm_mon + 1), st.tm_mday, st.tm_hour, st.tm_min,  st.tm_sec, t_usec / 1000 );
	
	/*check if rev buf is large enough*/
	if ( strlen(buf) > len - 1 ) {
		return NULL;
	}

	strcpy( timeStr, buf );

	return timeStr;
}

#if defined(TCSUPPORT_CT_DIAG_MICROSECOND)
char * convertSysTimeMicrosecondToString(struct timeval tv, char *timeStr, int len)
{
	time_t t_sec;
	suseconds_t t_usec;
	struct tm st;
	char buf[60];

	/*check rev buf if is NULL.*/
	if( NULL == timeStr ) {
		return NULL;
	}
	
	memset( buf, 0, sizeof(buf) );
	
	t_sec = tv.tv_sec;
	t_usec = tv.tv_usec;

	/*check if tv is 0*/
	if ( 0 == t_sec && 0 == t_usec ) {
		return NULL;
	}
	
	/*convert t_set to struct tm format*/
	memcpy( &st, localtime(&t_sec), sizeof(struct tm) );
#if defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_CY_JX)
	sprintf( buf, "%04d-%02d-%02dT%02d:%02d:%02d.%06d", 
		st.tm_year + 1900,  (st.tm_mon + 1), st.tm_mday, st.tm_hour, st.tm_min,  st.tm_sec, t_usec );
#else
	sprintf( buf, "%04d-%02d-%02dT%02d:%02d:%02d:%06d", 
		st.tm_year + 1900,  (st.tm_mon + 1), st.tm_mday, st.tm_hour, st.tm_min,  st.tm_sec, t_usec );
#endif
	
	/*check if rev buf is large enough*/
	if ( strlen(buf) > len - 1 ) {
		return NULL;
	}

	strcpy( timeStr, buf );

	return timeStr;
}
#endif

