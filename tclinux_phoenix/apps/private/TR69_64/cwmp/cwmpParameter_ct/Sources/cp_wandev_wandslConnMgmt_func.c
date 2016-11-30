#include "Global_res.h"
#include "cp_wandev_wandslConnMgmt.h"

#if !defined(TCSUPPORT_CT_PON)

/*----------------------dsl connection management function begin--------------------------------------------*/
int8 getConnSerNumbeOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char active[20] = {0};
	int iNum = 0;
	char result[20] = {0};
	char nodeName[20] = {0};
	int i,j;
#define ENTRY_NUM 8

	for(i = 0; i < MAX_WAN_IF_INDEX; i++)
	{
		for(j = 0;j < MAX_SMUX_NUM; j++)
		{
			memset(active, 0, sizeof(active) );
			sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);	
			if(tcapi_get(nodeName, "Active", active) != 0)
			{
			//	tcdbg_printf("\r\nget value error(normal attr),the attrName is Active");
				continue;
			}
			if(strcmp(active,"Yes"))
			{	
				iNum++;
				break;
			}			
		}		
	}
	sprintf(result, "%d", iNum);

	return getTheValue(value, type, deviceNode, result);
}

int8 getConnSerNumbeOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslConnMgmtAttr.aConnSerNumbeOfEntries;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setConnSerNumbeOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslConnMgmtAttr.aConnSerNumbeOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCONNMGMT_ATTR_FLAG;
	return result;
}

/*--------------------dsl connection management function end-----------------------------------------------*/

int	cwmpInitConnectionServiceSubTree( DeviceNodePtr deviceNode )
{
	char active[20] = {0};
	char nodeName[20] = {0};
	int i = 0, j = 0;
	int rtnCode = 0;
	char nodeIndex[10] = {0};
	
	if ( deviceNode == NULL ) {
		return -1;
	}

	for(i = 0; i < MAX_WAN_IF_INDEX; i++)
	{
		for(j = 0; j < MAX_SMUX_NUM; j++)
		{
			memset(active, 0, sizeof(active));
			sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);	
			if(tcapi_get(nodeName, "Active", active) != 0)
			{
				//tcdbg_printf("\r\nget value error(normal attr),the attrName is ISP");
				continue;
			}
			
			
			//if(!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP))
			if( !strcmp(active,"Yes") ) 
			{
				sprintf( nodeIndex, "%d", i + 1 );
				ConnectionServiceIndex[0].name = nodeIndex;
				if ( RegisterNodeFromTable( deviceNode, ConnectionServiceIndex ) == NULL ) {
				
					/*register fail*/
					tc_printf( " ==>cwmpInitConnectionServiceSubTree(): call RegisterNodeFromTable fail.\n" );
					return rtnCode;
				}
				break;
			}
		}
	}

	return rtnCode;
}

/*--------------------dsl connection management connection service function begin-----------------------------*/
int8 getWANConnectionDeviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  WANConnectDevice[256] = {0};

	sprintf(WANConnectDevice,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%s", deviceNode->parentNode->name);
	return getTheValue(value, type, deviceNode, WANConnectDevice);	
}

int8 getWANConnectionDeviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslConnMgmtSvrAttr.aWANConnectionDevice;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANConnectionDeviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslConnMgmtSvrAttr.aWANConnectionDevice;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCONNMGMTSVR_ATTR_FLAG;
	return result;
}

int8 getWANConnectionServiceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[64] = {0};
#if 0
	char nodeName[34] = {0};
	
	char isp[64] = {0};
	uint8 wannodeindex = 0;
	
	wannodeindex=atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "Wan_PVC%d", wannodeindex);

	tcapi_get(nodeName, "ISP", isp);
	if(0 == strcmp(isp, WAN_ENCAP_DYN) || 0 == strcmp(isp, WAN_ENCAP_STATIC))
		sprintf(result, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANIPConnection.1", wannodeindex+1);
	else if(0 == strcmp(isp, WAN_ENCAP_PPP))
		sprintf(result, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.1", wannodeindex+1);
	else
	{
		tcdbg_printf("\r\nget value error(normal attr),the attrName is ISP");
		return -1;
	}
#endif
	return getTheValue(value, type, deviceNode, result);	
}

int8 getWANConnectionServiceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslConnMgmtSvrAttr.aWANConnectionService;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANConnectionServiceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslConnMgmtSvrAttr.aWANConnectionService;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCONNMGMTSVR_ATTR_FLAG;
	return result;
}

int8 getDestinationAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char result[64] = {0};
	char vpi[10] = {0};
	char vci[10] = {0};
	uint8 wannodeindex = 0;
	
	wannodeindex=atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "Wan_PVC%d", wannodeindex);

	tcapi_get(nodeName, "VPI", vpi);
	tcapi_get(nodeName, "VCI", vci);

	sprintf(result,"PVC:%s/%s",vpi,vci);

	return getTheValue(value, type, deviceNode, result);	
}

int8 getDestinationAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslConnMgmtSvrAttr.aDestinationAddress;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDestinationAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslConnMgmtSvrAttr.aDestinationAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCONNMGMTSVR_ATTR_FLAG;
	return result;
}

int8 getLinkTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[32] = {"Unconfigured"};
#if 0
	char nodeName[34] = {0};
	char encap[64] = {0};
	int wannodeindex;
	
	wannodeindex=atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "Wan_PVC%d", wannodeindex);
	tcapi_get(nodeName, "ENCAP", encap);

	if(strcmp(encap, "PPPoA LLC") == 0 //ppoa mode
	|| strcmp(encap, "PPPoA VC-Mux") == 0){
		strcpy(result,"PPPoA");
	}
	else if(strcmp(encap, "1483 Routed IP LLC(IPoA)") == 0 //dynamic/static mode
		 || strcmp(encap, "1483 Routed IP VC-Mux") == 0){
		strcpy(result,"IPoA");
	}
	else if(strcmp(encap, "1483 Bridged IP LLC") == 0	//dynamic/static mode
		 || strcmp(encap, "1483 Bridged IP VC-Mux") == 0
		 || strcmp(encap, "PPPoE LLC") == 0          	//pppoe mode
		 || strcmp(encap, "PPPoE VC-Mux") == 0){
		strcpy(result,"EoA");
	}
#endif	
	return getTheValue(value, type, deviceNode, result);	
}

int8 getLinkTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslConnMgmtSvrAttr.aLinkType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setLinkTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslConnMgmtSvrAttr.aLinkType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCONNMGMTSVR_ATTR_FLAG;
	return result;
}

int8 getConnectionTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char result[64] ="Unconfigured";
#if 0
	char nodeName[34] = {'\0'};
	char result[64] = {'\0'};
	char isp[64] = {'\0'};
	uint8 wannodeindex = 0;
	
	wannodeindex=atoi(deviceNode->parentNode->name)-1;
	sprintf(nodeName, "Wan_PVC%d", wannodeindex);

	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP)){
		strcpy(result, "IP_Routed");
	}
	else
		strcpy(result, "Unconfigured");
#endif	
	return getTheValue(value, type, deviceNode, result);
}

int8 getConnectionTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcdslConnMgmtSvrAttr.aConnectionType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setConnectionTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcdslConnMgmtSvrAttr.aConnectionType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	
	rpc.attrFlags.theSettingFlag0 |= DSLCONNMGMTSVR_ATTR_FLAG;
	return result;
}

/*--------------------dsl connection management connection service function end-----------------------------*/

#endif

