#include "Global_res.h"
#include "ct_IPPingDiagnostics.h"
#if !defined(TCSUPPORT_C1_CUC)
int ctcomPingNumberOfEntries = 0;

int getpingNumberEntries()
{
	return ctcomPingNumberOfEntries;
}

int	cwmpInitIpPingDiagnosticTable( DeviceNodePtr deviceNode )
{
	int i = 0;
	char index[4] = {0};
	int rtnCode = 0;
	char nodeName[32] = {0};
	char tmpBuf[32] = {0};
	int ret = 0;
	
	if(deviceNode == NULL){
		return -1;
	}
	
	ctcomPingNumberOfEntries = 0;

	for(i = 0; i < MAX_IPPINGDIAGNOSTIC_NUM; i++)
	{
		memset(nodeName,0,sizeof(nodeName));
		memset(tmpBuf,0,sizeof(tmpBuf));
		memset(index,0,sizeof(index));

		sprintf(nodeName,IPPINGDIAGNOSTIC_ENTRY,i);

		/*check if this entry is exist or not*/
		ret = tcapi_get(nodeName, "DiagnosticsState",tmpBuf);
		if ( ret < 0 ) { //it means this entry is not exist.
			continue;
		}

		ctcomPingNumberOfEntries++;
		sprintf(index, "%d", i+1);

		PingConfigIndex[0].name = index;
		
		if ( RegisterNodeFromTable( deviceNode, PingConfigIndex) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}
	
	return rtnCode;
}

int8 checkFreePingEntryRule(void)
{
	int i = 0;
	char nodeName[32] = {0};
	char tmpBuf[32] = {0};
	int ret = 0;
	
	for(i=0; i < MAX_IPPINGDIAGNOSTIC_NUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, i);
		ret = tcapi_get(nodeName, "DiagnosticsState", tmpBuf);
		if( ret < 0 )//it means this entry is not exist.
			return i;
	}
	
	return -1;
}

int8 addIpPingDiagnosticEntryByIndex(int number)
{	
	char nodeName[32] = {0};
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, number);
	tcapi_set(IPPINGDIAGNOSTIC_COMMON, "Active", "No");
	tcapi_set(nodeName, "DiagnosticsState", "None");
	tcapi_set(nodeName, "Interface", "");
	tcapi_set(nodeName, "Host", "");
	tcapi_set(nodeName, "NumberOfRepetitions", "0");
	tcapi_set(nodeName, "Timeout", "3");
	tcapi_set(nodeName, "DataBlockSize", "56");
	tcapi_set(nodeName, "DSCP", "2");
	tcapi_set(nodeName, "Interval", "2");
	tcapi_set(nodeName, "Stop", "0");
	tcapi_set(nodeName, "PingNum", "0");
	tcapi_set(nodeName,"StopPingFlag","0");
	return 0;
}
	
int8 DeviceNodeAddPingInedxObject(uint32* num, DeviceNode*  deviceNode)
{
	uint32 number;

	DeviceNodePtr ippingdiagnosticNode = NULL;
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.";
	int rtnCode = 0;
		tc_printf("\r\nDeviceNodeAddPingInedxObject");
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ippingdiagnosticNode);
	if(ippingdiagnosticNode == NULL){
		tc_printf("\r\nDeviceNodeAddPingInedxObject:not find node");
		return -1;
	}
	
	if ((number = checkFreePingEntryRule()) < 0) {
		tc_printf("\r\ncan't find free entry for use!");
		return -1;
	}
	else
	{
		addIpPingDiagnosticEntryByIndex(number);
		cwmpFreeDeviceTable(ippingdiagnosticNode);
		rtnCode = cwmpInitIpPingDiagnosticTable(ippingdiagnosticNode);

		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\nct_IPPingDiagnostic_func.c---[addPingInedxObject]---*num = ", *num);
		}
	}
	
	return rtnCode;
}

int8 deleteIpPingDiagnosticEntryByIndex(uint32 num)
{
	char nodeName[32] = {0};
	
	if(num < 0 || num >= MAX_IPPINGDIAGNOSTIC_NUM)
		return -1;

	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, num);
	tcapi_unset(nodeName);
	tcapi_commit(IPPINGDIAGNOSTIC);
	return 0;
}

int8 DeviceNodeDeletePingInedxObject(uint32 num, DeviceNode*  deviceNode)
{
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\nct_IPPingDiagnostic_func.c---[deletePingInedxObject]---num = ", num);
	}
	
	deleteIpPingDiagnosticEntryByIndex(num - 1);//InstanceNumber - 1, disable this rule and reset its members

	cwmpFreeDeviceTable(ruleNode);
	rtnCode = cwmpInitIpPingDiagnosticTable(ruleNode);

	return rtnCode;
}

int8 getCTCOMDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[32] = {0};
	int entryindex =atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	tcapi_get(nodeName,"DiagnosticsState",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	
	if(strlen((char*)value) > 31)
	{
		tc_printf("\r\nIPPingDiagnosticsStateType parameter len is over max limit!");
		return -1;
	}
	
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	if ( strcmp((char*)value,"Requested") ==0)
	{	
		stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.
		tcapi_set(nodeName,"DiagnosticsState",(char*)value);
	}
	else 
	{
		tc_printf("\r\nIPPingDiagnosticsStateType parameter is not Requested!");
		return -1;
	}

	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);

	return 0;
}

int8 getCTCOMDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingDiagnosticsState[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingDiagnosticsState[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMIPPingInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[150] = {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	tcapi_get(nodeName,"cwmpInterface",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);

}

int8 setCTCOMIPPingInterfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	char DefaultIface_WAN[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char DefaultIface_LAN[]="InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1";
	char tempvalue[2];
	int len;
	char *setvalue = NULL;
	char *tmp = NULL;
	uint32 pvcnode = 0,entrynode = 0,index = 0;	
	DeviceNodePtr Node = NULL;
	char nodeName[32] = {0};
	char interfaceName[32] = {0};
	int entryindex = -1;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;;
#endif
	
	entryindex = atoi(deviceNode->parentNode->name) -1;
	stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.

	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	setvalue=(char*)value;
	len = strlen(DefaultIface_WAN);
	
	if(strncmp(DefaultIface_WAN, setvalue, len) == 0)//WAN interface
	{
		#if 0
		/*check the value is valid or not.*/
		cwmpSearchDeviceNodeByName(setvalue, rpc.IGDDeviceRootPtr, &Node);
		if(Node == NULL)
		{
			tc_printf("\r\n---[setCTCOMIPPingInterfaceValue]---Invalid Value(1)!");
			return -1;
		}
		#endif
		
		len = strlen(DefaultIface_WAN);
		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		pvcnode = atoi(tempvalue);
		pvctmp = pvcnode;
		if(FindPVCMapping(&pvcnode) != 0){
			tc_printf("\r\n---[setCTCOMIPPingInterfaceValue]---Invalid pvc number!");
			return -1;
		}
#else
		if(setvalue[len]<'1'||setvalue[len]>'8')
		{
			tc_printf("\r\n---[setCTCOMIPPingInterfaceValue]---Invalid pvc number!");
			return -1;
		}
		pvcnode = atoi(tempvalue) - 1;
#endif

		tmp = strstr(setvalue,"WANPPPConnection");
		if(!tmp)
		{
			tmp =  strstr(setvalue,"WANIPConnection");
			if(!tmp)
			{
				tc_printf("\r\n---[setCTCOMIPPingInterfaceValue]---Invalid Value(2)!");
				return -1;
			}
			else
			{
				tempvalue[0] = *(tmp+strlen("WANIPConnection."));
			}
		}
		else
		{
				tempvalue[0] = *(tmp+strlen("WANPPPConnection."));
		}
		
		tempvalue[1] = '\0';
		tc_printf("\r\ntempvalue is %s",tempvalue);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		entrynode = atoi(tempvalue);
		if(FindWanNodeMapping(&pvctmp, &entrynode) != 0)
		{
			tc_printf("\r\n---[setCTCOMIPPingInterfaceValue]---Invalid entry number!");
			return -1;
		}
#else	
		entrynode = atoi(tempvalue) - 1;
		if(entrynode<0||entrynode>7)
		{
			tc_printf("\r\n---[setCTCOMIPPingInterfaceValue]---Invalid entry number!");
			return -1;
		}
#endif
		index = pvcnode*8 + entrynode;
		//use "smux" as prfixname when interface is wan connection
		sprintf(interfaceName,"smux%d",index);
		tcapi_set(nodeName,"Interface",interfaceName);
	}
	else if(strcmp(setvalue,DefaultIface_LAN) == 0)//LAN interface
	{
		tcapi_set(nodeName,"Interface","br0");		
	}else if(strcmp(setvalue, "") == 0)
	{
		//empty string mean use default route
		tcapi_set(nodeName,"Interface","");		
		return 0;
		
	}else{
		tc_printf("\r\n---[setCTCOMIPPingInterfaceValue]---Invalid Value(3)!");
		return -1;
	}

	//attribute:cwmpInterface is only used by tr069 show
	tcapi_set(nodeName,"cwmpInterface",setvalue);
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
		
	return 0;
}

int8 getCTCOMIPPingInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingInterface[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMIPPingInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingInterface[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMIPPingHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[256]= {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_get(nodeName,"Host",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMIPPingHostValue(void* value, struct DeviceNode*  deviceNode)
{
	int valueLen = 0;	
	char nodeName[32] = {0};
	int entryindex = -1;
	
	valueLen = strlen((char*)value);

	if((valueLen > 255) || (valueLen == 0))
	{
		tc_printf("\r\n---[setCTCOMIPPingHostValue]---len is 0 or over 255!");
		return -1;
	}
	
	entryindex = atoi(deviceNode->parentNode->name) -1;
	stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.
	
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_set(nodeName,"Host",(char*)value);
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
	
	return 0;
}

int8 getCTCOMIPPingHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingHost[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMIPPingHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingHost[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMNumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[15]= {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_get(nodeName,"NumberOfRepetitions",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMNumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entryindex = -1;
	int numberOfRepetitions = 0;
	char ctnumberOfRepetitions[16] = {0};
	
	entryindex = atoi(deviceNode->parentNode->name) -1;
	stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.

	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	numberOfRepetitions = *((uint32*)value);
	sprintf(ctnumberOfRepetitions,"%lu",numberOfRepetitions);
	tcapi_set(nodeName,"NumberOfRepetitions",ctnumberOfRepetitions);
	tcapi_set(nodeName,"PingNum","0");//must be set!
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
		
	return 0;
}

int8 getCTCOMNumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingNumberOfRepetitions[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMNumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingNumberOfRepetitions[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMIPPingTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[15]= {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_get(nodeName,"Timeout",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMIPPingTimeoutValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entryindex = -1;

	int timeout = 0;
	char cttimeout[16] ={0};
	entryindex = atoi(deviceNode->parentNode->name) -1;
	stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.
	
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	timeout = *((uint32*)value);
	sprintf(cttimeout,"%lu",timeout);
	tcapi_set(nodeName,"Timeout",cttimeout);
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
	
	return 0;
}

int8 getCTCOMIPPingTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingTimeout[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMIPPingTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingTimeout[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMDataBlockSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[15] = {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_get(nodeName,"DataBlockSize",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMDataBlockSizeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entryindex = -1;

	int dataBlockSize = 0;
	char ctdataBlockSize[16] ={0};
	
	if( (*((uint32*)value)) > 65535)
	{
		tc_printf("\r\n---[setCTCOMIPPingHostValue]---vlue should be 0~65535(0 mean use default value)!");
		return -1;
	}
	
	entryindex = atoi(deviceNode->parentNode->name) -1;
	stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.
	
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	dataBlockSize = *((uint32*)value);
	sprintf(ctdataBlockSize,"%lu",dataBlockSize);
	tcapi_set(nodeName,"DataBlockSize",ctdataBlockSize);
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
	
	return 0;
}

int8 getCTCOMDataBlockSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingDataBlockSize[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMDataBlockSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingDataBlockSize[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[15] = {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_get(nodeName,"DSCP",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMDSCPValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entryindex = -1;

	int DSCP = 0;
	char ctDSCP[16] ={0};
	
	if( (*((uint32*)value)) > 63)
	{
		tc_printf("\r\n---[setCTCOMIPPingHostValue]---vlue should be 0~63(0 mean use default value)!");
		return -1;
	}
	
	entryindex = atoi(deviceNode->parentNode->name) -1;
	stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.
	
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	DSCP = *((uint32*)value);
	sprintf(ctDSCP,"%lu",DSCP);
	tcapi_set(nodeName,"DSCP",ctDSCP);
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
	
	return 0;	
}

int8 getCTCOMDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingDSCP[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingDSCP[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[15] = {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_get(nodeName,"Interval",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entryindex = -1;

	int interval = 0;
	char ctinterval[16] ={0};
	
	entryindex = atoi(deviceNode->parentNode->name) -1;
	stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.
	
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	interval = *((uint32*)value);
	sprintf(ctinterval,"%lu",interval);
	tcapi_set(nodeName,"Interval",ctinterval);
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
	
	return 0;
}

int8 getCTCOMIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingInterval[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingInterval[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}

int8 getCTCOMStopValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char tmpbuf[15] = {0};
	int entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	tcapi_get(nodeName,"Stop",tmpbuf);
	return getTheValue(value, type, deviceNode, tmpbuf);
}

int8 setCTCOMStopValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entryindex = -1;
	uint8 flag;

	flag = *((uint8*)value);
	
	//stopCwmpPing(entryindex);//If the ping test is in progress, terminate it first.

	entryindex = atoi(deviceNode->parentNode->name) -1;
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	
	if(flag == 1){
		tcapi_set(nodeName,"Stop","1");
	} 
	else if (flag == 0){
		tcapi_set(nodeName,"Stop","0");
	}
	else{
		tc_printf("==>setCTCOMEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	SetCfgNodeFlag( IPPINGDIAGNOSTIC,entryindex);
	
	return 0;
}

int8 getCTCOMStopAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = ctIpPingDiagnosticAttr.actcomIpPingStop[atoi(deviceNode->parentNode->name) -1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTCOMStopAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &ctIpPingDiagnosticAttr.actcomIpPingStop[atoi(deviceNode->parentNode->name) -1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag1 |= CTCOM_IPPINGDIAGNOSTIC_ATTR_FLAG;
	return result;
}


//function 
void stopCwmpPing(int entryindex)
{
	FILE *fp = NULL;
	char cmdbuf[128] = {0};
	char ping_pid_tmp[50] = {0};
	int ping_pid = 0;
	char nodeName[32] = {0};
	
	sprintf(nodeName, IPPINGDIAGNOSTIC_ENTRY, entryindex);
	tcapi_set(nodeName,"StopPingFlag","1");
	tcapi_commit(nodeName);
	
#if 0 //handle move to cfg_manager	
	sprintf(cmdbuf,CTCOM_PING_PID_PATH,entryindex);
	fp = fopen(CTCOM_PING_PID_PATH, "r");
	if(fp)
	{
		tcdbg_printf("\r\npid file is not null");
		//kill the ping process
		fgets(ping_pid_tmp, sizeof(ping_pid_tmp), fp);
		ping_pid = atoi(ping_pid_tmp);
		if(ping_pid != 0)
		{ 
			/*if pid != 0, that's mean old ping diagnostic process already exist*/
			memset(cmdbuf,0,sizeof(cmdbuf));
			sprintf(cmdbuf,"kill -9 %d", ping_pid);  /*remove old ping diagnostic process*/
			tcdbg_printf("ct com Ping Diagnostic process down, pid is [%d]\n", ping_pid);
		   	system(cmdbuf);
		}
		
		//delete the file/tmp/cwmp/ct_ping_xxx.pid
		fclose(fp);
		sprintf(cmdbuf,CTCOM_PING_PID_PATH,entryindex);
		unlink(cmdbuf);
	}

	//operation route information
	memset(cmdbuf,0,sizeof(cmdbuf));
	sprintf(cmdbuf,"%s%d",CTCOM_PING_ROUTE_PATH,entryindex);
	tcdbg_printf("\r\nroute file is %s",cmdbuf);
	fp = fopen(cmdbuf, "r");
	if(fp)
	{
		tcdbg_printf("\r\npid file is not null");
		//delete the route which add at the beginning of the ping 
		memset(cmdbuf,0,sizeof(cmdbuf));
		fgets(cmdbuf, sizeof(cmdbuf), fp);
		system(cmdbuf);

		//delete the file /tmp/cwmp/ct_ping_route
		fclose(fp);
		sprintf(cmdbuf,"%s%d",CTCOM_PING_ROUTE_PATH,entryindex);
		unlink(cmdbuf);
	}
#endif
	return;
}
#endif
