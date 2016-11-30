#include "Global_res.h"
#include "cp_wandev_wanconndev_ip_ppp_conn.h"

#define MAXLEN_NODE_NAME	32
#define MAXLEN_ATTR_NAME	32
#define MAXLEN_TCAPI_MSG		1024


/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.*/
int8 getWANCnt1_1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char enable[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "Active", enable);

	if(!strcmp(enable, "Yes")){
		strcpy(enable, "1");
	}
	else{
		strcpy(enable, "0");
	}
	
	return getTheValue(value, type, deviceNode, enable);
	
}
int8 setWANCnt1_1EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint8 ena;
	char enable[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", enable);
	if(ret){//this pvc is not exist.
		return -1;
	}

	//tcdbg_printf("nodename:%s\n Wan Commit Entry set to %d", nodeName, (wanNodeIndex<<3|pvcNodeEntry) +100 );
	//tcdbg_printf("wanNodeIndex:%d, pvcNodeIndex:%d, wan<<3:%d, wan<<3|pvc:%d\n",wanNodeIndex,pvcNodeEntry, wanNodeIndex<<3,wanNodeIndex<<3|pvcNodeEntry);
	ena = *((uint8*)value);
	if(ena == 1){
		tcapi_set(nodeName, "Active", "Yes");
	}
	else if(ena == 0){
		tcapi_set(nodeName, "Active", "No");
	}
	else{
		
		return -1;
	}
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}


int8 getWANIPCnt1_1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1Enable[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1Enable[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
/*
check brige ppp status
return code:  1 ppp up
*/
int checkBrigePPPState(uint32_t if_idx)
{
	FILE *fp = NULL;
	char tmpbuf[256] = {0};
	uint32_t if_index_sub = 0;
	uint32_t bridge_state[2] = {0};

	fp = fopen("/proc/tc3162/bridge_ppp_sate", "r");
	if ( fp )
	{
		bzero(tmpbuf, sizeof(tmpbuf));
		fgets(tmpbuf, sizeof(tmpbuf), fp);
		fclose(fp);

		if ( 0 != tmpbuf[0] )
		{
			sscanf(tmpbuf, "%x %x", &bridge_state[1], &bridge_state[0]);
			if ( if_idx < 32 )
			{
				if ( bridge_state[0] & (1 << if_idx) )
					return 1;
			}
			else if ( ( if_index_sub = (if_idx - 32) ) < 32 )
			{
				if ( bridge_state[1] & (1 << if_index_sub) )
					return 1;
			}
		}
	}

	return 0;
}
#endif

int8 getWANCnt1_1StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCnt1_1Status[64] = {'\0'};
	char nodeName[34] = {'\0'};
	char status[10] = {'\0'};
	char isp[10] = {'\0'};
	uint8 sts;
	uint32 wanNodeIndex,pvcNodeEntry;
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE) || defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
	char ipver[16] = {0}, wanmode[20] = {0};
	int wan_bridge = 0;
#endif
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE) || defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	tcapi_get(nodeName, "IPVERSION", ipver);

	if ( 0 == tcapi_get(nodeName, "WanMode", wanmode)
		&& 0 == strcmp(wanmode, "Bridge") )
		wan_bridge = 1;
#endif

	//sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);	
	//tcapi_get(nodeName, "ISP", isp);
	sprintf(nodeName, "WanInfo_Entry%d", wanNodeIndex<<3|pvcNodeEntry);
	tcapi_get(nodeName, "Status", status);

	sts = atoi(status);
	if(!strcmp(status,"up")){
		strcpy(WANCnt1_1Status, "Connected");
	}
	else  {
		strcpy(WANCnt1_1Status, "Disconnected");
	}
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE) || defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
	if ( 0 == strcmp(ipver, "IPv6") )
		strcpy(WANCnt1_1Status, "Unconfigured");
	else
	{
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
		if ( 1 == wan_bridge )
		{
			if ( checkBrigePPPState((wanNodeIndex<<3|pvcNodeEntry)) )
				strcpy(WANCnt1_1Status, "Connected");
			else
				strcpy(WANCnt1_1Status, "Disconnected");
		}
#endif
	}
#endif
	
	return getTheValue(value, type, deviceNode, WANCnt1_1Status);
}
int8 getWANIPCnt1_1StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1CntStatus[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1CntStatus[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1PsbTypesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char  WANCnt1_1PsbTypes[256] = {'\0'};
	uint32 wannodeindex,pvcNodeEntry;
	char wanDSLLinkType[4] = {'\0'};
	char bridgedType[10] = {'\0'};
	char bridgeMode[20] = {'\0'};
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wannodeindex, &pvcNodeEntry);
#else
	wannodeindex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wannodeindex,pvcNodeEntry);

#if 0//tony
	tcapi_get(nodeName, "BRIDGEDTYPE", bridgedType);
	
	if(spSysParam.tcCwmpAttr->vWANLinkTypeFlag[wannodeindex] == 0){ //ip connection
		if(!strcmp(bridgedType, "IP"))
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed , IP_Bridged");
		else
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed");
	}
	else{  // ppp connection
		if(!strcmp(bridgedType, "PPP"))
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed , PPPoE_Bridged");
		else
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed");
	}
#endif
	tcapi_get(nodeName, "BridgeMode", bridgeMode);
	if(spSysParam.tcCwmpAttr->vWANLinkTypeFlag[wannodeindex] == 0){ //ip connection
		if(!strcmp(bridgeMode, "IP_Bridged"))
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed , IP_Bridged");
		else
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed");
	}
	else{  // ppp connection
		if(!strcmp(bridgeMode, "PPPoE_Bridged"))
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed , PPPoE_Bridged");
		else
			strcpy(WANCnt1_1PsbTypes,"Unconfigured , IP_Routed");
	}
	
	return getTheValue(value, type, deviceNode, WANCnt1_1PsbTypes);
}
int8 getWANIPCnt1_1PsbTypesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1PsbCntTypes[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1PsbTypesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1PsbCntTypes[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char WANIPCnt1_1Type[64] = {'\0'};
	char bridgeType[10] = {'\0'};
	char isp[64] = {'\0'};
	uint32 wannodeindex = 0,pvcNodeEntry = 0;
#if defined(TCSUPPORT_CT_PON_GDV20)
	char attrValue[64] = {0};
#endif
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wannodeindex, &pvcNodeEntry);
#else
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wannodeindex,pvcNodeEntry);

	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP)){
		strcpy(WANIPCnt1_1Type, "IP_Routed");
#if defined(TCSUPPORT_CT_PON_GDV20)
		if(!strcmp(isp, WAN_ENCAP_PPP)){
			tcapi_get(nodeName, "BridgeInterface", attrValue);
			if ( 0 == strcmp("Yes", attrValue) )
				strcpy(WANIPCnt1_1Type, "PPPoE_Hybrid");		
		}
#endif
	}
	else if(!strcmp(isp, WAN_ENCAP_BRIDGE)){
#if 0//tony
		tcapi_get(nodeName, "BRIDGEDTYPE", bridgeType);
		if(!strcmp(bridgeType, "IP")){ //ip bridged
			strcpy(WANIPCnt1_1Type, "IP_Bridged");
		}
		else if(!strcmp(bridgeType, "PPP")){  //ppp bridged
			strcpy(WANIPCnt1_1Type, "PPPoE_Bridged");
		}
#endif
		tcapi_get(nodeName, "BridgeMode", bridgeType);
		if(!strcmp(bridgeType, "IP_Bridged")) {
			strcpy(WANIPCnt1_1Type, "IP_Bridged");
		}
		else if(!strcmp(bridgeType, "PPPoE_Bridged")) {
			strcpy(WANIPCnt1_1Type, "PPPoE_Bridged");
		}
		else{
			strcpy(WANIPCnt1_1Type, "Unconfigured");
		}
	}
	else{
		strcpy(WANIPCnt1_1Type, "Unconfigured");
	}

	return getTheValue(value, type, deviceNode, WANIPCnt1_1Type);

	return 0;
}
int8 setWANCnt1_1TypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char vtype[64] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	char serviceList[SERVICE_LIST_LEN] = {'\0'};
	char bridgeMode[20] = {'\0'};
	char tmpVar[10] = {0}, tmpVar2[10] = {0};
	int ret;
	//char bridgedType[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->name) - 1;
#endif

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcdbg_printf("nodeName:%s!\n",nodeName);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret != 0){ //this pvc is not exist.
		tcdbg_printf("get node fails!\n");
		return -1;
	}	
	tcapi_get(nodeName, "ServiceList", serviceList);
	tcapi_get(nodeName, "BridgeMode", bridgeMode);
	
	if(strlen((char*)value) > 63){
		tcdbg_printf("value len  exceeds max value!\n");
		return -1;
	}

	strcpy(vtype,(char*)value);		
	//tcapi_get(nodeName, "BRIDGEDTYPE", bridgedType);
	tcapi_get(nodeName, "ISP", isp);


	if(spSysParam.tcCwmpAttr->vWANLinkTypeFlag[wanNodeIndex] & (1<<pvcNodeEntry) ){/* ACS is adding this PVC and its linkType is EoA */
		
		if( !strcmp(isp,WAN_ENCAP_DYN) ) { //ip
			if( strcmp(vtype, "IP_Routed") == 0 ){//set it to dynamic mode.
				tcapi_set(nodeName, "ISP", WAN_ENCAP_DYN);
				tcapi_set(nodeName, "WanMode", "Route");

				tcapi_set(nodeName, "ENCAP", "1483 Bridged IP LLC");
				tcapi_set(nodeName, "DEFAULTROUTE", "Yes");
				tcapi_set(nodeName, "NATENABLE","Enable");
#if !defined(TCSUPPORT_CT_PON)
				tcapi_set(nodeName, "MTU", "0");
#endif
				if(0!=tcapi_get(nodeName, "MTU", tmpVar) || '\0'==tmpVar[0])
					tcapi_set(nodeName, "MTU", "1500");
				//tcapi_set(nodeName, "RIPVERSION", "RIP1");
				//tcapi_set(nodeName, "DIRECTION", "None");
				//tcapi_set(nodeName, "IGMP", "IGMP v1");
				tcdbg_printf("flag:IP_Routed!\n");
			}
			else if( strcmp(vtype, "IP_Bridged") == 0 /*&& strcmp(bridgedType, "IP")*/ ){//set it to bridge mode.
				tcapi_set(nodeName, "BridgeMode", vtype);
				tcapi_set(nodeName, "ISP", WAN_ENCAP_BRIDGE);
				tcapi_set(nodeName, "WanMode", "Bridge");
				
				if( strcmp(serviceList, "OTHER") == 0) {
					tcapi_set(nodeName, "DHCPRealy", "Yes");
				}
				
				tcapi_set(nodeName, "ENCAP", "1483 Bridged IP LLC");
				tcdbg_printf("flag:IP_Bridged!\n");
			}
			else{
				/*we should not delete this pvc object here.*/
				//DeviceNodeDelPVCObject(wanNodeIndex+1, NULL);
				tcdbg_printf("flag:fail!\n");
				return -1;
			}
			/*here we can determine IPConnection or PPPConnection,so we clear this flag.*/
			//ACSaddingPVCFlag &=  ~(1<<wanNodeIndex);
			tcdbg_printf("before clear flag!\n");
			spSysParam.tcCwmpAttr->vWANLinkTypeFlag[wanNodeIndex] &=  ~(1<<pvcNodeEntry);
			tcdbg_printf("after clear flag!\n");
			
		}
		else if( !strcmp(isp,WAN_ENCAP_PPP) ) { //ppp
#if defined(TCSUPPORT_CT_PON_GDV20)
			if(strcmp(vtype,"IP_Routed")==0 || strcmp(vtype,"PPPoE_Hybrid")==0){
				if(strcmp(vtype,"PPPoE_Hybrid")==0)
					tcapi_set(nodeName, "BridgeInterface", "Yes");
				else
					tcapi_set(nodeName, "BridgeInterface", "No");
#else
			if ( strcmp(vtype,"IP_Routed") == 0 ){ //pppoe
#endif
				tcapi_set(nodeName, "ISP", WAN_ENCAP_PPP);
				tcapi_set(nodeName, "WanMode", "Route");
#if !defined(TCSUPPORT_CT_PON)
				tcapi_set(nodeName, "USERNAME", "");
				tcapi_set(nodeName, "PASSWORD", "");
#endif
				tcapi_set(nodeName, "ENCAP", "PPPoE LLC");
#if defined(TCSUPPORT_CT_PON_CZ_QH)
				tcapi_set(nodeName, "CONNECTION", "Connect_on_Demand");
#else
				tcapi_set(nodeName, "CONNECTION", "Connect_Keep_Alive");
#endif
				tcapi_set(nodeName, "MSS", "0");
				tcapi_set(nodeName, "PPPGETIP", "Dynamic");
				tcapi_set(nodeName, "NATENABLE","Enable");
#if !defined(TCSUPPORT_CT_PON)
				tcapi_set(nodeName, "MTU", "0");
#endif
				if(0!=tcapi_get(nodeName, "MTU", tmpVar) || '\0'==tmpVar[0])
					tcapi_set(nodeName, "MTU", "1492");
				//tcapi_set(nodeName, "RIPVERSION", "RIP1");
				//tcapi_set(nodeName, "DIRECTION", "None");
				//tcapi_set(nodeName, "IGMP", "IGMP v1");
				tcdbg_printf("flag PPPoE:IP_Routed!\n");
			}
			else if( strcmp(vtype, "PPPoE_Bridged") == 0 /* && strcmp(bridgedType, "PPP")*/ ){
				tcapi_set(nodeName, "BridgeMode", vtype);
				tcapi_set(nodeName, "WanMode", "Bridge");
				tcapi_set(nodeName, "ISP", WAN_ENCAP_BRIDGE);

				if( strcmp(serviceList, "OTHER") == 0) {
					tcapi_set(nodeName, "DHCPRealy", "Yes");
				}

				tcapi_set(nodeName, "ENCAP", "1483 Bridged IP LLC");
				tcdbg_printf("flag PPPoE:PPPoE_Bridged!\n");
			}
			else{
				/*we should not delete this pvc object here.*/
				//DeviceNodeDelPVCObject(wanNodeIndex+1, NULL);
				tcdbg_printf("flag PPPoE:fail!\n");
				return -1;
			}
			
			/*here we can determine IPConnection or PPPConnection,so we clear this flag.*/
			//ACSaddingPVCFlag &=  ~(1<<wanNodeIndex);
			tcdbg_printf("before clear flag PPPoE!\n");
			spSysParam.tcCwmpAttr->vWANLinkTypeFlag[wanNodeIndex] &=  ~(1<<pvcNodeEntry);
			tcdbg_printf("after clear flag PPPoE!\n");
		}
		else{
			return -1;
		}
	}
	else{
#if defined(TCSUPPORT_CT_PON_GDV20)
		if(!strcmp(vtype, "IP_Routed") || !strcmp(vtype, "PPPoE_Hybrid")){
			if(strcmp(vtype,"PPPoE_Hybrid")==0)
				tcapi_set(nodeName, "BridgeInterface", "Yes");
			else
				tcapi_set(nodeName, "BridgeInterface", "No");			
#else
		if(!strcmp(vtype, "IP_Routed")){
#endif
			if(!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP)){
				;//ip route mode, it not need to change connectionType
				tcdbg_printf("NO flag:IP_Routed 1!\n");
			}
			else if(!strcmp(isp, WAN_ENCAP_BRIDGE)){//if isp is bridge,we set it to dynamic mode.
				tcapi_set(nodeName, "WanMode", "Route");

				tcapi_set(nodeName, "ENCAP", "1483 Bridged IP LLC");
				tcapi_set(nodeName, "DEFAULTROUTE", "Yes");
				tcapi_set(nodeName, "NATENABLE","Enable");
#if !defined(TCSUPPORT_CT_PON)
				tcapi_set(nodeName, "MTU", "0");
#endif
#if defined(TCSUPPORT_CT_PON_CZ_QH)
				tcapi_set(nodeName, "CONNECTION", "Connect_on_Demand");
#endif
				/* set isp by link mode */
				tcapi_get(nodeName, "LinkMode", tmpVar);
				if(0==strcmp(tmpVar, "linkIP"))	{
					tcapi_set(nodeName, "ISP", WAN_ENCAP_DYN);
				}
				else {
					tcapi_set(nodeName, "ISP", WAN_ENCAP_PPP);
				}

				/* set mtu by link mode */
				if (tcapi_get(nodeName, "MTU", tmpVar2) || '\0'==tmpVar2[0]) {
					if(0==strcmp(tmpVar, "linkIP"))	{
					tcapi_set(nodeName, "MTU", "1500");
					}
					else {
						tcapi_set(nodeName, "MTU", "1492");
					}
				}
				
				//tcapi_set(nodeName, "RIPVERSION", "RIP1");
				//tcapi_set(nodeName, "DIRECTION", "None");
				//tcapi_set(nodeName, "IGMP", "IGMP v1");
				tcdbg_printf("NO flag:IP_Routed!\n");
				//tcdbg_printf("NO flag:IP_Routed 2!\n");
			}
			else{
				tcdbg_printf("NO flag:IP_Routed fail!\n");
				return -1;
			}
		}
		else if( !strcmp(vtype, "PPPoE_Bridged") )
		{
			if(!strcmp(bridgeMode, "PPPoE_Bridged") || !strcmp(isp, WAN_ENCAP_PPP)) {
				tcapi_set(nodeName, "WanMode", "Bridge");
				tcapi_set(nodeName, "BridgeMode", vtype);
				tcapi_set(nodeName, "ISP", WAN_ENCAP_BRIDGE);
				if( strcmp(serviceList, "OTHER") == 0) {
					tcapi_set(nodeName, "DHCPRealy", "Yes");
				}
				
				tcapi_set(nodeName, "ENCAP", "1483 Bridged IP LLC");
				tcdbg_printf("NO flag:PPPoE_Bridged!\n");
			}
			else{
				tcdbg_printf("NO flag:PPPoE_Bridged fail!\n");
				return -1;
			}
		}
		else if( !strcmp(vtype, "IP_Bridged") )
		{
			if(!strcmp(bridgeMode, "IP_Bridged") || !strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) ) {
				tcapi_set(nodeName, "WanMode", "Bridge");
				tcapi_set(nodeName, "BridgeMode", vtype);
				tcapi_set(nodeName, "ISP", WAN_ENCAP_BRIDGE);
				if( strcmp(serviceList, "OTHER") == 0) {
					tcapi_set(nodeName, "DHCPRealy", "Yes");
				}
				
				tcapi_set(nodeName, "ENCAP", "1483 Bridged IP LLC");
				tcdbg_printf("NO flag: IP_Bridged!\n");
			}
			else{
				tcdbg_printf("NO flag:IP_Bridged fail!\n");
				return -1;
			}
		}
		else{
			tcdbg_printf("NO flag: fail!\n");
			return -1;
		}
	}
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) +COMMIT_THIRD_ALIAS );

	return 0;
}

int8 getWANIPCnt1_1TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1Type[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1Type[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1NameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char conname[257] = {'\0'}; //The max length of service name is 256.
	uint32 wanNodeIndex,pvcNodeEntry,ret = 0;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	char ifIdx[5] = {0};
	char serviceList[30] = {0};
	char vpiValue[10] = {0};
	char vciValue[10] = {0};
	char vidMode[10] = {0};
	char vidValue[10] = {0};
	char wanMode[20] = {0};
	char atmValue[10] = {0};
#endif

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	ret = tcapi_get(nodeName, "CONNAME", conname);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	if(ret)
	{
		memset(conname, 0, sizeof(conname));
		tcapi_get(nodeName, "IFIdx", ifIdx);
		tcapi_get(nodeName, "ServiceList", serviceList);
		tcapi_get(nodeName, "WanMode", wanMode);

		sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
		tcapi_get(nodeName, "ATMEnable", atmValue);

#if defined(TCSUPPORT_CT_E8B_ADSL)
		strcpy(atmValue, "Yes");
#endif

		sprintf(conname, "%s_%s", ifIdx, serviceList);

		if(strcmp(wanMode, "Route") == 0)
			strcat(conname, "_R_");
		else
			strcat(conname, "_B_");

		if(strcmp(atmValue, "Yes") == 0)	//atm
		{	
			tcapi_get(nodeName, "VPI", vpiValue);
			tcapi_get(nodeName, "VCI", vciValue);
			strcat(conname, vpiValue);
			strcat(conname, "_");
			strcat(conname, vciValue);
		}
		else
		{
			strcat(conname, "VID_");
			sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
			tcapi_get(nodeName, "VLANMode", vidMode);
			if(strcmp(vidMode, "TAG") == 0)
			{
				tcapi_get(nodeName, "VLANID", vidValue);
				strcat(conname, vidValue);
			}
		}			
	}
#endif

	return getTheValue(value, type, deviceNode, conname);	
}
int8 setWANCnt1_1NameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);


	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	if(strlen((char*)value) > 256){
		return -1;
	}
	
	tcapi_set(nodeName, "CONNAME", (char*)value);
	return 0;
}
int8 getWANIPCnt1_1NameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1Name[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1NameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1Name[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1UptimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if 1
	char nodeName[34] = {'\0'};
	char isp[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char upTime[32] = {"0"};//set upTime default value to 0.
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	
	tcapi_get(nodeName, "ISP", isp);
	//it does not support dynamic/static mode, may need to add in future.
	if(!strcmp(isp, WAN_ENCAP_PPP)){
		sprintf(nodeName, "WanInfo_Entry%d", wanNodeIndex<<3|pvcNodeEntry);
		tcapi_get("nodeName", "pppConnTime", upTime);
	}

	return getTheValue(value, type, deviceNode, upTime);
#else
	int wanNodeIndex;
	int connect_time;
	char upTime[30] = {"0"};
	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	if(wanNodeIndex > 7 || wanNodeIndex < 0)
		return getTheValue(value, type, deviceNode, upTime);

	if(*(pStartTime+wanNodeIndex) == -1 || *(pNowTime+wanNodeIndex) == -1 || *(pNowTime+wanNodeIndex) < *(pStartTime+wanNodeIndex))
		return getTheValue(value, type, deviceNode, upTime);

	connect_time = *(pNowTime+wanNodeIndex) - *(pStartTime+wanNodeIndex);
	
	sprintf(upTime,"%d",connect_time);
	return getTheValue(value, type, deviceNode, upTime);
#endif
}
int8 getWANIPCnt1_1UptimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1Uptime[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
}
int8 setWANIPCnt1_1UptimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1Uptime[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1LErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CT_PPPCONN_ERROR)
	char WANCnt1_1LError[128] = {'\0'},wan_name[16] = {'\0'}, wan_info_name[16] = {'\0'}, wan_ip[16] = {'\0'};
	uint32 entryIndex,pvcIndex;
	char ipver[16] = {0}, ip6addr[64] = {0}, ip6gw[64] = {0};
	char connSTV4[12] = {0}, connSTV6[12] = {0}, wanmode[20] = {0};
	int wan_bridge = 0, isPPP = 0;
	char ipv6cp[12] = {0};
	char ispMode[12] = {0};
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	entryIndex = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcIndex, &entryIndex);
#else
	pvcIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	entryIndex = atoi(deviceNode->parentNode->name)-1;
#endif
	
	sprintf(wan_name, "Wan_PVC%d_Entry%d", pvcIndex, entryIndex);
	tcapi_get(wan_name, "ConnectionError", WANCnt1_1LError);
	tcapi_get(wan_name, "IPVERSION", ipver);

	if ( 0 == tcapi_get(wan_name, "WanMode", wanmode)
		&& 0 == strcmp(wanmode, "Bridge") )
		wan_bridge = 1;
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	if ( 0 == tcapi_get(wan_name, "ISP", ispMode)
		&& 0 == strcmp(ispMode, PPP_MODE) )
		isPPP = 1;
#endif
	
#if !defined(TCSUPPORT_CT_PPPOE_EMULATOR)
	sprintf(wan_info_name, "WanInfo_Entry%d", pvcIndex*PVC_NUM+ entryIndex);
	tcapi_get(wan_info_name, "IP", wan_ip);
	if(_checkIp(wan_ip)){
		if(!strcmp(WANCnt1_1LError,"ERROR_NONE"))
				strcpy(WANCnt1_1LError, "ERROR_UNKNOWN");
	}
#endif
#if defined(TCSUPPORT_CT_PON_CZ_GD)
	sprintf(wan_info_name, "WanInfo_Entry%d", pvcIndex*PVC_NUM+ entryIndex);
	tcapi_get(wan_info_name, "Status", connSTV4);
	tcapi_get(wan_info_name, "Status6", connSTV6);
	if(0 == wan_bridge 
		&& (!strcmp(connSTV6, "up") || !strcmp(connSTV4, "up")))
	{
		strcpy(WANCnt1_1LError, "ERROR_NONE");
	}
#endif
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
	if ( (0 == strcmp(ipver, "IPv4/IPv6")
		|| 0 == strcmp(ipver, "IPv6"))
		&& 0 == wan_bridge /* only for route mode */
		&& 0 != strcmp(WANCnt1_1LError, "ERROR_AUTHENTICATION_FAILURE")
		&&  0 != strcmp(WANCnt1_1LError, "ERROR_ISP_TIME_OUT") )
	{
		sprintf(wan_name, "Wan_PVC%d_Entry%d", pvcIndex, entryIndex);
		if ( 1 == get_dslite_state(wan_name) )
		{
			sprintf(wan_name, "WanInfo_Entry%d"
				, pvcIndex*MAX_SMUX_NUM + entryIndex);
			if ( (1 == isPPP)
				&& 1 != is_ipv6cp_up(pvcIndex*MAX_SMUX_NUM + entryIndex) )
			{
				strcpy(WANCnt1_1LError
									, "ERROR_ISP_DISCONNECT_IPv6");
			}
			else if ( 1 != is_dslite_up(wan_name) )
				strcpy(WANCnt1_1LError
					, "ERROR_SERVER_OUT_OF_RESOURCES_IPv6");
		}
		else
		{
			sprintf(wan_name, "WanInfo_Entry%d",
						pvcIndex*MAX_SMUX_NUM + entryIndex);
			tcapi_get(wan_name, "Status", connSTV4);
			tcapi_get(wan_name, "Status6", connSTV6);
			if ( 0 != tcapi_get(wan_name, "IP6", ip6addr) )
				ip6addr[0] = 0;
			if ( 0 != tcapi_get(wan_name, "GateWay6", ip6gw) )
				ip6gw[0] = 0;

			if ( 0 == strcmp(ipver, "IPv4/IPv6")
				&& 0 != strcmp(connSTV4, "up") )
				strcpy(WANCnt1_1LError, "ERROR_ISP_DISCONNECT_IPv4");
			else
			{
				if ( (1 == isPPP)
					&& 1 != is_ipv6cp_up(pvcIndex*MAX_SMUX_NUM + entryIndex) )
				{
					strcpy(WANCnt1_1LError
										, "ERROR_ISP_DISCONNECT_IPv6");
				}
				else if ( 0 != strcmp(connSTV6, "up") )
					strcpy(WANCnt1_1LError, "ERROR_SERVER_OUT_OF_RESOURCES_IPv6");
				else
				{
					if ( 0 == ip6addr[0]
						|| 0 == ip6gw[0] )
						strcpy(WANCnt1_1LError
							, "ERROR_SERVER_OUT_OF_RESOURCES_IPv6");
				}
			}
		}
	}
#endif
	
	return getTheValue(value, type, deviceNode, WANCnt1_1LError);
#else
	char nodeName[34] = {'\0'};
	char WANCnt1_1LError[64] = {'\0'};
	char lastError[10] = {'\0'};

	
	tcapi_get("Info_Adsl", "lineState", lastError);
	if(!strcmp(lastError, "up")){
		strcpy(WANCnt1_1LError, "ERROR_NONE");
	}
	else{
		strcpy(WANCnt1_1LError, "ERROR_UNKNOWN");
	}

	return getTheValue(value, type, deviceNode, WANCnt1_1LError);
#endif
}
int8 getWANIPCnt1_1LErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1LastCntError[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
}
int8 setWANIPCnt1_1LErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1LastCntError[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANIPCnt1_1RSIPAvailableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  RSIPAvailable[4];
	int vRSIPAvailable = 0;

	sprintf(RSIPAvailable,"%d",vRSIPAvailable);	
	return getTheValue(value, type, deviceNode, RSIPAvailable);
}
int8 getWANIPCnt1_1RSIPAvailableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1RSIPAvailable[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);	
}
int8 setWANIPCnt1_1RSIPAvailableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1RSIPAvailable[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1NATEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char isp[10] = {'\0'};
	char WANIPCnt1_1NATEnabled[12] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcdbg_printf("nodeName: %s\n", nodeName);

	tcapi_get(nodeName, "ISP", isp);
#if !defined(TCSUPPORT_CT_PON_C9_HUN)
	if (!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP)) { // only dynamic/static/pppoe/pppoa support nat.
#endif
		tcapi_get(nodeName, "NATENABLE", WANIPCnt1_1NATEnabled);
		tcdbg_printf("NATENABLE for %s is %s\n", nodeName, WANIPCnt1_1NATEnabled);
		
		if(!strcmp(WANIPCnt1_1NATEnabled, "Enable")){
			strcpy(WANIPCnt1_1NATEnabled, "1");
		}
		else{
			strcpy(WANIPCnt1_1NATEnabled, "0");
		}
#if !defined(TCSUPPORT_CT_PON_C9_HUN)
	}
	else{
		strcpy(WANIPCnt1_1NATEnabled, "0");
	}
#endif
	return getTheValue(value, type, deviceNode, WANIPCnt1_1NATEnabled);
}
int8 setWANCnt1_1NATEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 vWANCnt1_1NATEnabled;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	vWANCnt1_1NATEnabled = *((uint8*)value);
	tcapi_get(nodeName, "ISP", isp);
#if !defined(TCSUPPORT_CT_PON_C9_HUN) && !defined(TCSUPPORT_CT_PON_QH)
	if( !strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP) ){ // only dynamic/static/pppoe/pppoa support nat.
#endif

		if(vWANCnt1_1NATEnabled == 1){
			tcapi_set(nodeName, "NATENABLE", "Enable");
		}
		else if(vWANCnt1_1NATEnabled == 0){
			tcapi_set(nodeName, "NATENABLE", "Disabled");
		}
#if !defined(TCSUPPORT_CT_PON_C9_HUN) && !defined(TCSUPPORT_CT_PON_QH)
		else{
			tc_printf("==>setWANCnt1_1NATEnabledValue: value should be 0 or 1.\n");
			return -1;
		}
	}
	else{
		tc_printf("==>setWANCnt1_1NATEnabledValue: bridge mode do not support nat.\n");
#if !defined(TCSUPPORT_CT_FJ) && !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CT_PON_SK)
		return -1;
#endif
	}
#endif

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) +COMMIT_THIRD_ALIAS );

	return 0;
}

int8 getWANIPCnt1_1NATEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1NATEnable[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1NATEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1NATEnable[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANIPCnt1_1AddrTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char isp[10] = {'\0'};
	char  WANIPCnt1_1AddrType[8];
	uint32 wannodeindex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wannodeindex, &pvcNodeEntry);
#else
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wannodeindex,pvcNodeEntry);

	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_DYN)){
		strcpy(WANIPCnt1_1AddrType, "DHCP");
	}
	else if(!strcmp(isp, WAN_ENCAP_STATIC)){
		strcpy(WANIPCnt1_1AddrType, "Static");
	}
	else{
		strcpy(WANIPCnt1_1AddrType, " ");
	}
	
	return getTheValue(value, type, deviceNode, WANIPCnt1_1AddrType);
}
int8 setWANIPCnt1_1AddrTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	char viptype[16];
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", active);
	if(ret){	//this pvc is not exist.
		return -1;
	}
	
	if(strlen((char*)value) > 15){	
		return -1;
	}
	strcpy(viptype,(char*)value);
	
	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_DYN)){	//dynamic mode
		if(!strcmp(viptype, "DHCP")){
			;
		}
		else if(!strcmp(viptype, "Static")){
			tcapi_set(nodeName, "ISP", WAN_ENCAP_STATIC);
		}
		else{
			return -1;
		}
	}
	else if(!strcmp(isp, WAN_ENCAP_STATIC)){ //static mode
		if(!strcmp(viptype, "DHCP")){
			tcapi_set(nodeName, "ISP", WAN_ENCAP_DYN);
		}
		else if(!strcmp(viptype, "Static")){
			;
		}
		else{
			return -1;
		}
	}
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) +COMMIT_THIRD_ALIAS );
	return 0;
}

int8 getWANIPCnt1_1AddrTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1AddrType[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1AddrTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1AddrType[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1ExtIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char isp[10] = {'\0'};
	char  WANCnt1_1ExtIPAddr[32] = {"0.0.0.0"};
	uint32 wanNodeIndex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "ISP", isp);
	
	if (!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP)){
		memset(nodeName, 0, sizeof(nodeName) );
		sprintf(nodeName, "WanInfo_Entry%d", wanNodeIndex<<3|pvcNodeEntry);
		tcapi_get(nodeName, "IP", WANCnt1_1ExtIPAddr);
	}
	
	return getTheValue(value, type, deviceNode, WANCnt1_1ExtIPAddr);
}
int8 setWANCnt1_1ExtIPAddrValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	char ipType[20] = {'\0'};
	char vExtIPAddr[32] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	if(strlen((char*)value) > 31){
		return -1;
	}
	
	strcpy(vExtIPAddr,(char*)value);	
	if(InternalClientIpCheck(vExtIPAddr) != 1 ){
		tc_printf("==>setWANCnt1_1ExtIPAddrValue: Invalid ip address.\n");
		return -1;
	}

#if 1
	tcapi_get(nodeName, "ISP", isp);
	tcapi_set(nodeName, "IPADDR", vExtIPAddr);	
	if(!strcmp(isp, WAN_ENCAP_PPP)){ //pppoe/ppoa ip
	   tcapi_get(nodeName, "PPPGETIP", ipType);
	   if(!strcmp(ipType, "Static")){ // only static suport
		   tcapi_set(nodeName, "NETMASK", "255.255.255.255");
	   }
	}
#else
	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_STATIC)){ //static ip
		tcapi_set(nodeName, "IPADDR", vExtIPAddr);
	}
	else if(!strcmp(isp, WAN_ENCAP_PPP)){ //pppoe/ppoa ip
		tcapi_get(nodeName, "PPPGETIP", ipType);
		if(!strcmp(ipType, "Static")){ // only static suport
			tcapi_set(nodeName, "IPADDR", vExtIPAddr);
			//need set submask here ..
			tcapi_set(nodeName, "NETMASK", "255.255.255.255");
		}
		else{
			return -1;
		}
	}
	else{
		return -1;
	}
#endif

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry)+COMMIT_THIRD_ALIAS);
	return 0;
}

int8 getWANIPCnt1_1ExtIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1ExtIPAddr[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1ExtIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1ExtIPAddr[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANIPCnt1_1SubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char WANIPCnt1_1SubMask[32] = {"0.0.0.0"};
	char isp[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	tcapi_get(nodeName, "ISP", isp);
	if( !strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) /* || !strcmp(isp, WAN_ENCAP_PPP)*/ ){ //only dynamic/static support submask.
		memset(nodeName, 0, sizeof(nodeName) );
		sprintf(nodeName, "WanInfo_Entry%d", wanNodeIndex<<3|pvcNodeEntry);
		tcapi_get(nodeName, "NetMask", WANIPCnt1_1SubMask);
	}
	
	return getTheValue(value, type, deviceNode, WANIPCnt1_1SubMask);
}
int8 setWANIPCnt1_1SubMaskValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	char ipType[20] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char vSubMask[32];
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	if(strlen((char*)value) > 31){
		return -1;
	}
	strcpy(vSubMask,(char*)value);
#if 1
	tcapi_set(nodeName, "NETMASK", vSubMask);
#else
	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_STATIC)){ //static ip
		tcapi_set(nodeName, "NETMASK", vSubMask);
	}
	else{
		return -1;
	}
#endif
	
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	return 0;
}

int8 getWANIPCnt1_1SubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1SubMask[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1SubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1SubMask[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANIPCnt1_1DefGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char  WANIPCnt1_1DefGateway[32] = {"0.0.0.0"};
	char isp[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "ISP", isp);
	
	if( !strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) ){
		memset(nodeName, 0, sizeof(nodeName) );
		sprintf(nodeName, "WanInfo_Entry%d", wanNodeIndex<<3|pvcNodeEntry);
		tcapi_get(nodeName, "GateWay", WANIPCnt1_1DefGateway);
		
	}
	
	return getTheValue(value, type, deviceNode, WANIPCnt1_1DefGateway);
}

int8 setWANIPCnt1_1DefGatewayValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	char ipType[20] = {'\0'};
	char vDefGateway[32];
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	if(strlen((char*)value) > 31){
		return -1;

	}
	
	strcpy(vDefGateway,(char*)value);
	tcapi_get(nodeName, "ISP", isp);

#if 1
	tcapi_set(nodeName, "GATEWAY", vDefGateway);
#else
	if(!strcmp(isp, WAN_ENCAP_STATIC)){ //static ip
		tcapi_set(nodeName, "GATEWAY", vDefGateway);
	}
	else{
		return -1;
	}
#endif

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );	
	return 0;
}

int8 getWANIPCnt1_1DefGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1DefGateway[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANIPCnt1_1DefGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1DefGateway[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1DNSEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	char  WANIPCnt1_1DNSEnabled[4] = {"0"};//default value is '0'
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
#if 0
	ret = tcapi_get(nodeName, "Active", active);
	if( strcmp(active,"Yes") != 0 ){
		strcpy(WANIPCnt1_1DNSEnabled, "0");	//return -1;
	}else{
		if(ret == 0){//this pvc is not exist.
			tcapi_get(nodeName, "ISP", isp);
			if(!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP) ){
				strcpy(WANIPCnt1_1DNSEnabled, "1");
			}
		}
	}
#else
	ret = tcapi_get(nodeName, "DNSENABLE", active);
	if( !strcmp(active,"1")){
		strcpy(WANIPCnt1_1DNSEnabled, "1");
	}
	else{
		strcpy(WANIPCnt1_1DNSEnabled, "0");
	}
#endif
	return getTheValue(value, type, deviceNode, WANIPCnt1_1DNSEnabled);
}
int8 setWANCnt1_1DNSEnabledValue(void* value, struct DeviceNode*  deviceNode)
{	
	//may need further handle. xzwang_060926
	char nodeName[34] = {'\0'};
	uint8 vDNSenable;
	uint32 wanNodeIndex,pvcNodeEntry;

	vDNSenable = *((uint8*)value);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	if(vDNSenable == 1){
		tcapi_set(nodeName, "DNSENABLE", "1");
	}
	else if(vDNSenable == 0){
		tcapi_set(nodeName, "DNSENABLE", "0");
	}
	else{
		tc_printf("==>setWANCnt1_1DNSEnabledValue: value should be 0 or 1.\n");
		return -1;
	}

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) +COMMIT_THIRD_ALIAS );

	return 0;
}
int8 getWANIPCnt1_1DNSEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1DNSEnabled[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1DNSEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1DNSEnabled[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1DNSOvAllowedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	//merge from linos by xyzhu_091123
	static uint8 vWANCnt1_1DNSOvAllowed;
	char  WANIPCnt1_1DNSOvAllowed[4];
	
	vWANCnt1_1DNSOvAllowed=1;
	
	sprintf(WANIPCnt1_1DNSOvAllowed,"%d",vWANCnt1_1DNSOvAllowed);	

	return getTheValue(value, type, deviceNode, WANIPCnt1_1DNSOvAllowed);
#else
	char nodeName[34] = {'\0'};
	char DNSOvAllowed[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "DNSOvAllowed", DNSOvAllowed);
	if(strcmp(DNSOvAllowed,"1") != 0){
		strcpy(DNSOvAllowed,"0");
	} 
	
	return getTheValue(value, type, deviceNode, DNSOvAllowed);
#endif
}
int8 setWANCnt1_1DNSOvAllowedValue(void* value, struct DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	char nodeName[34] = {'\0'};
	char DNSOvAllowed[10] = {'\0'};	
	uint8 vDNSOvAllowed;
	uint32 wanNodeIndex,pvcNodeEntry;

	vDNSOvAllowed = *((uint8*)value);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	if(vDNSOvAllowed == 1){
		tcapi_set(nodeName, "DNSOvAllowed", "1");
	}
	else if(vDNSOvAllowed == 0){
		tcapi_set(nodeName, "DNSOvAllowed", "0");
	}
	else{
		tc_printf("==>setWANCnt1_1DNSOvAllowedValue: value should be 0 or 1.\n");
		return -1;
	}

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) +COMMIT_THIRD_ALIAS );
#endif

	return 0;
}
int8 getWANIPCnt1_1DNSOvAllowedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1DNSOverAllow[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1DNSOvAllowedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{

	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1DNSOverAllow[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1DNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34];
	char  WANCnt1_1DNSServers[65];//here we supprt two dns at most.
	char dnsBuf[20];
	uint32 wannodeindex,pvcNodeEntry;

	memset(nodeName, 0, sizeof(nodeName));
	memset(WANCnt1_1DNSServers, 0, sizeof(WANCnt1_1DNSServers));
	memset(dnsBuf, 0, sizeof(dnsBuf));
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wannodeindex, &pvcNodeEntry);
#else
	wannodeindex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "WanInfo_Entry%d", wannodeindex<<3|pvcNodeEntry);

	tcapi_get(nodeName, "DNS", WANCnt1_1DNSServers); 
	if (strlen(WANCnt1_1DNSServers) > 0) { //here we get the first dns server.
		tcapi_get(nodeName, "SecDNS", dnsBuf);
		if(strlen(dnsBuf) > 0){			//there is seconde dns also.
			strcat(WANCnt1_1DNSServers, ",");
			strcat(WANCnt1_1DNSServers, dnsBuf);
		}
	}

	return getTheValue(value, type, deviceNode, WANCnt1_1DNSServers);
}
int8 setWANCnt1_1DNSServersValue(void* value, struct DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CT_PON_C9_HUN)
	char DNSServer[32] = {0};
#else
	char *DNSServer;
#endif
	char *temp;
	FILE *fp;
	char tmpBuf[128]={'\0'};
	char dhcpType[10] = {'\0'};
	char dnsType[10] = {'\0'};
	char ipAddress1[16] = {'\0'};
	char ipAddress2[16] = {'\0'};
	int dnsNum = 0;
	char nodeName[34] = {0}, isp[8] = {0};
	uint32 wannodeindex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wannodeindex, &pvcNodeEntry);
#else
	wannodeindex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
#if defined(TCSUPPORT_CT_PON_C9_HUN)
	strncpy(DNSServer,value,sizeof(DNSServer));
#else
	DNSServer = (char*)value;
#endif
#if defined(TCSUPPORT_CT_PON_C9_HUN)
	if(strlen(DNSServer) == 0)
	{
		memset(DNSServer, 0, sizeof(DNSServer));
		strcpy(DNSServer,"0.0.0.0,0.0.0.0");
	}
#endif

	temp = strstr(DNSServer, ",");

	if(temp == NULL){
		dnsNum = 1;
		if( InternalClientIpCheck(DNSServer) != 1 ){
			return -1;
		}
		strcpy(ipAddress1, DNSServer);
	}
	else{ 
		dnsNum = 2;
		*temp = '\0';
		if( InternalClientIpCheck(DNSServer) != 1 ){
			tc_printf("Illegal ip address\n");
			return -1;
		}			
		if( InternalClientIpCheck(temp+1) != 1 ){
			tc_printf("Illegal ip address\n");
			return -1;
		}
		strcpy(ipAddress1, DNSServer);
		strcpy(ipAddress2, temp+1);
	}
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wannodeindex, pvcNodeEntry );
	tcapi_get(nodeName,"ISP",isp);

#if 0
	if(atoi(isp) != 1)
	{
		tcdbg_printf("13");
		return -1;
	}
#endif

	if(dnsNum == 1){
			
			tcdbg_printf("14");
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
			tcapi_set(nodeName, "DNS", ipAddress1);
#else
			tcapi_set(nodeName, "Primary_DNS", ipAddress1);
#endif
	}
	else if(dnsNum == 2){
			tcdbg_printf("15");
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
			tcapi_set(nodeName, "DNS", ipAddress1);
			tcapi_set(nodeName, "SecDNS", ipAddress2);		
#else
			tcapi_set(nodeName, "Primary_DNS", ipAddress1);
			tcapi_set(nodeName, "Secondary_DNS", ipAddress2);
#endif
	}
	else{
			return -1;
	}

	
	SetCfgNodeFlag( "Wan", COMMIT_THIRD_ALIAS + (wannodeindex <<3 |pvcNodeEntry) );
	return 0;

#if 0

	if( !strcmp(dhcpType, "1") && !strcmp(dnsType, "1") ){//dhcp is enable and dns is manual
		if(dnsNum == 1){
			tcapi_set("Dproxy_Entry", "Primary_DNS", ipAddress1);
		}
		else if(dnsNum == 2){
			tcapi_set("Dproxy_Entry", "Primary_DNS", ipAddress1);
			tcapi_set("Dproxy_Entry", "Secondary_DNS", ipAddress2);
		}
		else{
			return -1;
		}
		
		return 0;
	}

	fp = fopen(DNS_INFO_PATH, "w");
	if(fp != 0){
		if(dnsNum == 1){
			sprintf(tmpBuf,"nameserver %s\n",ipAddress1);
		}
		else if(dnsNum == 2){
			sprintf(tmpBuf,"nameserver %s\n",ipAddress1);
			sprintf(tmpBuf + strlen(tmpBuf),"nameserver %s\n",ipAddress2);
		}
		else{
			return -1;
		}
		fputs(tmpBuf, fp);
	}
	else{
		return -1;
	}
	fclose(fp);

	SetCfgNodeFlag( "Dproxy", -1 );
	return 0;
#endif
}
int8 getWANIPCnt1_1DNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1DNSServers[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1DNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1DNSServers[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1MACAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34];
	char WANCnt1_1MACAddr[64];
	uint32 wanNodeIndex,pvcNodeEntry;

	memset(nodeName, 0, sizeof(nodeName));
	memset(WANCnt1_1MACAddr, 0, sizeof(WANCnt1_1MACAddr));
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
		
	sprintf(nodeName, "Info_nas%d.%d", wanNodeIndex,pvcNodeEntry);

	tcapi_get(nodeName, "hwaddr", WANCnt1_1MACAddr);

	return getTheValue(value, type, deviceNode, WANCnt1_1MACAddr);
}
int8 setWANCnt1_1MACAddrValue(void* value, struct DeviceNode*  deviceNode)
{
	//we do not support to set mac address. may need future.
	return -1;
}
int8 getWANIPCnt1_1MACAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1MACAddress[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1MACAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1MACAddress[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1CntTriggerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	uint32 wannodeindex,pvcNodeEntry;
	char  WANCnt1_1CntTrigger[32] = {'\0'};

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wannodeindex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wannodeindex, &pvcNodeEntry);
#else
	wannodeindex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wannodeindex,pvcNodeEntry);

	tcapi_get(nodeName, "CONNECTION", WANCnt1_1CntTrigger);

	if(!strcmp(WANCnt1_1CntTrigger, "Connect_Keep_Alive")){
		strcpy(WANCnt1_1CntTrigger, "AlwaysOn");
	}
	else if(!strcmp(WANCnt1_1CntTrigger, "Connect_on_Demand")){
		strcpy(WANCnt1_1CntTrigger, "OnDemand");
	}
	else if(!strcmp(WANCnt1_1CntTrigger, "Connect_Manually")){
		strcpy(WANCnt1_1CntTrigger, "Manual");
	}
	else{
		strcpy(WANCnt1_1CntTrigger, "");
	}
	
	return getTheValue(value, type, deviceNode, WANCnt1_1CntTrigger);
}
int8 setWANCnt1_1CntTriggerValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	char *WANCnt1_1CntTrigger;
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	tcapi_get(nodeName, "ISP", isp);

	if(strcmp(isp, WAN_ENCAP_PPP) != 0){

		//only pppoe/pppoa support this parameter.
		return -1;
	}
	
	WANCnt1_1CntTrigger = (char*)(value); 
	if(!strcmp(WANCnt1_1CntTrigger, "AlwaysOn")){
		tcapi_set(nodeName, "CONNECTION", "Connect_Keep_Alive");
	}
	else if(!strcmp(WANCnt1_1CntTrigger, "OnDemand")){
		tcapi_set(nodeName, "CONNECTION", "Connect_on_Demand");
#if !defined(TCSUPPORT_CT_PPP_ONDEMAND)
		/*set idle time to 30min,merge from linos*/
		tcapi_set(nodeName, "CLOSEIFIDLE", "1800");
#endif
	}
#if !defined(TCSUPPORT_CT_PPP_ONDEMAND)
	else if(!strcmp(WANCnt1_1CntTrigger, "Manual")){
		tcapi_set(nodeName, "CONNECTION", "Connect_Manually");
	}
#endif
	else{

		return -1;
	}
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	return 0;
}

int8 getWANIPCnt1_1CntTriggerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1CntTrigger[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1CntTriggerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1CntTrigger[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
int8 getWANCnt1_IdleDisconnectTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char value_buf[32] = {0};
	uint32 pvcIndex = 0, entryIndex = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	entryIndex = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcIndex, &entryIndex);
#else
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	entryIndex = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if ( pvcIndex >= PVC_NUM || entryIndex >= MAX_SMUX_NUM )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcIndex, entryIndex);
	if ( 0 != tcapi_get(nodeName, "CLOSEIFIDLE", value_buf) )
		strcpy(value_buf, "1800");
	
	return getTheValue(value, type, deviceNode, value_buf);
}
int8 setWANCnt1_IdleDisconnectTimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char value_buf[64] = {0};
	uint32 pvcIndex = 0, entryIndex = 0;
	uint32 timeValue = *((uint32*)value);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	entryIndex = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcIndex, &entryIndex);
#else
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	entryIndex = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if ( pvcIndex >= PVC_NUM || entryIndex >= MAX_SMUX_NUM )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcIndex, entryIndex);
	snprintf(value_buf, sizeof(value_buf), "%lu", timeValue);
	tcapi_set(nodeName, "CLOSEIFIDLE", value_buf);

	SetCfgNodeFlag ( "Wan", (pvcIndex<<3|entryIndex) + COMMIT_THIRD_ALIAS );
	return 0;
}
int8 getWANCnt1_IdleDisconnectTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint32 pvcIndex = 0, entryIndex = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	entryIndex = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcIndex, &entryIndex);
#else
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	entryIndex = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if ( pvcIndex >= PVC_NUM || entryIndex >= MAX_SMUX_NUM )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_IdleDisconnectTime[pvcIndex<<3|entryIndex];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANCnt1_IdleDisconnectTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8 *attribute = NULL;
	int8 result = 0;
	uint32 pvcIndex = 0, entryIndex = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	entryIndex = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcIndex, &entryIndex);
#else
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	entryIndex = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if ( pvcIndex >= PVC_NUM || entryIndex >= MAX_SMUX_NUM )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_IdleDisconnectTime[pvcIndex<<3 | entryIndex];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_PPPOEPROXY)
int8 getWANCnt1_1ProxyEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	uint32 wanNodeIndex, pvcNodeEntry;
	char proxyenable[4]={0};

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "ProxyEnable", proxyenable);
	
	return getTheValue(value, type, deviceNode, proxyenable);
}

	
int8 setWANCnt1_1ProxyEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	char isp[10] = {'\0'};
	uint32 wanNodeIndex, pvcNodeEntry;
	uint8 proxyEnabled;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	proxyEnabled = *((uint8*)value);
	
	tcapi_get(nodeName, "ISP", isp);
	
	if(strcmp(isp, WAN_ENCAP_PPP) != 0){

		//only pppoe/pppoa support this parameter.
		return -1;
	}
	if(proxyEnabled==1){
		tcapi_set(nodeName, "ProxyEnable", "1");
	}else if(proxyEnabled==0){
		tcapi_set(nodeName, "ProxyEnable", "0");
	}else{
		return -1;
	}

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );	
	
	return 0;
}
int8 getWANIPCnt1_1ProxyEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1proxyenable[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1ProxyEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1proxyenable[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1ProxyMaxUserValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	uint32 wanNodeIndex, pvcNodeEntry;
	char maxuser[4]={0};

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	
	tcapi_get(nodeName, "ProxyMaxUser", maxuser);
	
	return getTheValue(value, type, deviceNode, maxuser);
}
int8 setWANCnt1_1ProxyMaxUserValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	char isp[10] = {'\0'};
	char maxusrstring[10]={'\0'};
	uint32 wanNodeIndex, pvcNodeEntry;
	uint32 maxuser;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	

	tcapi_get(nodeName, "ISP", isp);
	
	if(strcmp(isp, WAN_ENCAP_PPP) != 0){

		//only pppoe/pppoa support this parameter.
		return -1;
	}
	maxuser = *((uint32*)value);
	sprintf(maxusrstring, "%d",maxuser);

	if(maxuser<=4 && maxuser>=0)
		tcapi_set(nodeName, "ProxyMaxUser", maxusrstring);
	else
		return -1;
	
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );	
	return 0;
}
int8 getWANIPCnt1_1ProxyMaxUserAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1maxuser[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1ProxyMaxUserAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1maxuser[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif
#endif
int8 getWANCnt1_1RProRxValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char WANIPCnt1_1RProRx[16] = {"Off"};
	uint32 wanNodeIndex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "RIPVERSION", WANIPCnt1_1RProRx);
	
	if(!strcmp(WANIPCnt1_1RProRx, "RIP1")){
		strcpy(WANIPCnt1_1RProRx, "RIPv1");
	}
	else if(!strcmp(WANIPCnt1_1RProRx, "RIP2")){
		strcpy(WANIPCnt1_1RProRx, "RIPv2");
	}
	
	return getTheValue(value, type, deviceNode, WANIPCnt1_1RProRx);
}
int8 setWANCnt1_1RProRxValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char isp[10] = {'\0'};
	char *WANIPCnt1_1RProRx;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
		
	WANIPCnt1_1RProRx = (char*)(value); 
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}

	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_BRIDGE)){ // if bridge mode,just return.
		return -1;
	}

	if(!strcmp(WANIPCnt1_1RProRx, "RIPv1")){
		tcapi_set(nodeName, "RIPVERSION", "RIP1");
	}
	else if(!strcmp(WANIPCnt1_1RProRx, "RIPv2")){
		tcapi_set(nodeName, "RIPVERSION", "RIP2");
	}
	else{
		tc_printf("==>setWANCnt1_1RProRxValue:invalid value.");
		return -1;
	}
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry)+COMMIT_THIRD_ALIAS );
	return 0;
}

int8 getWANIPCnt1_1RProRxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1RProRx[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1RProRxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1RProRx[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANCnt1_1PMapNumOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[49] = {'\0'};
	char locIP[32] = {'\0'};
	char  WANCnt1_1PMapNumOfEntries[4];
	uint32 wanNodeIndex, pvcNodeEntry,i;
	uint8 ipMapNum = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	
	for(i = 0; i < MAX_VIRSERV_RULE; i++){
		sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, i);
		if( 0 ==  tcapi_get(nodeName, "LOCALIP", locIP))
			ipMapNum++;
		memset(locIP, 0, sizeof(locIP));
	}
	
	sprintf(WANCnt1_1PMapNumOfEntries,"%d",ipMapNum);	

	return getTheValue(value, type, deviceNode, WANCnt1_1PMapNumOfEntries);
}
int8 getWANIPCnt1_1PMapNumOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aPortMappingEntryNum[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_1PMapNumOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aPortMappingEntryNum[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

#if !defined(TCSUPPORT_C1_CUC)
int8 getWANCnt1_CTComServiceListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[49] = {0};
	char  serviceList[SERVICE_LIST_LEN] = {0};
	uint32 wanNodeIndex, pvcNodeEntry;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	uint8 bufLen = 0;
	uint8 idx = 0;
	char *p = NULL;
	char  tmp_svrList[SERVICE_LIST_LEN] = {0};
#endif

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	if( 0 !=  tcapi_get(nodeName, "ServiceList", serviceList))
	{
		memset(serviceList, 0, sizeof(serviceList));
		strcpy(serviceList, "OTHER");
	}
	else
	{
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
		if ( 0 != strcmp("OTHER", serviceList) )
		{
			bufLen = strlen(serviceList);
			for ( idx = 0; idx < bufLen; idx ++ )
			{
				if ( '_' == serviceList[idx] )
					serviceList[idx] = ',';
			}

#if !defined(TCSUPPORT_CUC)
			p = strstr(serviceList, "VOICE");
			if(p)
			{		
				strncpy(tmp_svrList,serviceList, (p-serviceList));
				strcat(tmp_svrList, "VOIP");
				strcat(tmp_svrList, (p+strlen("VOICE")));
				//memset(serviceList,0, sizeof(serviceList));
				strcpy(serviceList,tmp_svrList);
			}				
#endif
		}
#else
		if(strstr(serviceList, "TR069_INTERNET"))
		{
			memset(serviceList, 0, sizeof(serviceList));
			strcpy(serviceList,"TR069,INTERNET");
		}
#endif
	}
		

	return getTheValue(value, type, deviceNode, serviceList);
}
int8 setWANCnt1_CTComServiceListValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char serviceList[SERVICE_LIST_LEN] = {'\0'};
	char active[8] = {0};
	char isp[8] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	int tr069 = 0, internet = 0, other = 0;
#ifdef TCSUPPORT_CUC
	int iptv = 0;
#endif
	char setValue[34] = {0};
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SERVICELIST_E8C)
	uint8 voip = 0;
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	char old_serviceList[SERVICE_LIST_LEN] = {'\0'};
	int old_internet = 0;
	int wanActionType = WAN_TYPE_NOCARE;
	char bridgeMode[32] = {'\0'};
#endif

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	tcapi_get(nodeName, "ServiceList", old_serviceList);
	if( strstr( old_serviceList, "INTERNET" ) != NULL ){
		old_internet = 1;
	}
#endif
	

	
	if( strstr( (char*)value, "TR069" ) != NULL )
		tr069 = 1;
	if( strstr( (char*)value, "INTERNET" ) != NULL )
		internet = 1;
	if( strstr( (char*)value, "OTHER" ) != NULL )
		other = 1;
#ifdef TCSUPPORT_CUC
	if( strstr( (char*)value, "IPTV" ) != NULL )
		iptv = 1;
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SERVICELIST_E8C)
	if( strstr( (char*)value, "VOICE" ) != NULL  || strstr( (char*)value, "VOIP" ) != NULL)
		voip = 1;
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SERVICELIST_E8C)
	tcapi_get(nodeName, "ISP", isp);
	if ( 0 == strcmp(isp, WAN_ENCAP_BRIDGE)
		&& (1 == tr069 || 1 == voip) )
		return -1; // only internet or other for bridge mode.

	if ( 1 == tr069 && 1 == voip && 1 == internet )
		strcpy(serviceList, "TR069_VOICE_INTERNET");
	else if( 1 == tr069 && 1 == internet )
		strcpy(serviceList, "TR069_INTERNET");
	else if ( 1 == tr069 && 1 == voip )
		strcpy(serviceList, "TR069_VOICE");
	else if ( 1 == internet && 1 == voip )
		strcpy(serviceList, "VOICE_INTERNET");
	else if ( 1 == tr069 )
		strcpy(serviceList, "TR069");
	else if ( 1 == internet )
		 strcpy(serviceList, "INTERNET");
	else if ( 1 == voip )
		strcpy(serviceList, "VOICE");
#ifdef TCSUPPORT_CUC
	else if( 1 == iptv )
		strcpy(serviceList, "IPTV");
#endif
	else if ( 1 == other )
		strcpy(serviceList, "OTHER");
	else
		return -1;
#else
	if( tr069 == 1 && internet == 1)
		strcpy(serviceList, "TR069_INTERNET");
	else if(tr069 == 1)
		strcpy(serviceList, "TR069");
	else if(internet == 1)
                strcpy(serviceList, "INTERNET");
	else if(other == 1)
		strcpy(serviceList, "OTHER");
	else
		return -1;
#endif

#if 0
	
	if( strcmp((char*)value,"TR069")!=0 && strcmp((char*)value,"INTERNET")!=0  && strcmp((char*)value,"TR069_INTERNET")!=0 && strcmp((char*)value,"OTHER")!=0 )
		return -1;

	strcpy(serviceList, (char *)value);

#endif
	tcapi_set(nodeName, "ServiceList", serviceList);
	tcapi_get(nodeName, "ISP", isp);
	/* enable nat if service list is not tr069 */
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SERVICELIST_E8C)
	if ( 1 == tr069 || 1 == voip )
#else
	if (!strcmp((char*)value, "TR069"))
#endif
	{
		tcapi_set(nodeName, "NATENABLE", "Disabled");
	}
	else {
		tcapi_set(nodeName, "NATENABLE", "Enable");
	}

	
	if (1 == internet)	
	{
		tcapi_set(nodeName, "NATENABLE", "Enable");
	}		

	if ( 1 == other )
		tcapi_set(nodeName, "DHCPEnable", "0");
	
	/* Only OTHER-Bridge mode, DCHPRealy  Enable */
	if(!strcmp((char*)value, "OTHER") && !strcmp(isp, WAN_ENCAP_BRIDGE)) {
		tcapi_set(nodeName, "DHCPRealy", "Yes");
	}
	else if (!strcmp((char*)value, "INTERNET") && !strcmp(isp, WAN_ENCAP_BRIDGE)) {
		tcapi_set(nodeName, "DHCPRealy", "No");
	}
	
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	if(1 == old_internet && 1 != internet){
			/*serviceList modify as not internet, need delete*/
			setMonitorCollectorParaList(WAN_TYPE_NOCARE,wanNodeIndex,pvcNodeEntry,WAN_DEV_DEL);
	}else if(1 != old_internet && 1 == internet){
		/*servicelist modfiy to internet, need add*/
		if ( 0 == strcmp(isp, WAN_ENCAP_PPP)){
			wanActionType = PPP_WAN_TYPE;
		}else if( 0 == strcmp(isp, WAN_ENCAP_BRIDGE)){
			tcapi_get(nodeName, "BridgeMode", bridgeMode);
			if( 0 == strcmp(bridgeMode, "PPPoE_Bridged")){
				wanActionType = PPP_WAN_TYPE;
			}else{
				wanActionType = IP_WAN_TYPE;
			}
		}else{
			wanActionType = IP_WAN_TYPE;
		}

		setMonitorCollectorParaList(wanActionType,wanNodeIndex,pvcNodeEntry,WAN_DEV_ADD);
	}
		
#endif


	
	
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry)+COMMIT_THIRD_ALIAS );
	return 0;
}

int8 getWANIPCnt1_CTComServiceListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANCnt1_CTComServiceList[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_CTComServiceListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANCnt1_CTComServiceList[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_CTComInterfaceBondValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[49] = {0};
	char attr[8] = {0}, attrValue[8] = {0};
	int i,len;
	uint32 wanNodeIndex, pvcNodeEntry;
	char buf[512] = {0};
	int flagComma = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);

	for(i = 0; i < 4; i++)
	{
		sprintf(attr,"LAN%d", i+1);

		memset(attrValue, 0, sizeof(attrValue) );
		tcapi_get(nodeName, attr, attrValue);
		
		if( !strcmp(attrValue,"Yes"))
		{
			if( flagComma )
			{
				len = strlen(buf);
				sprintf(buf+len, ",");
			}
			len = strlen(buf);
			sprintf(buf+len, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.%d", i+1);
			flagComma = 1;
		}
	}

	for(i = 0; i < 4; i++)
	{
		sprintf(attr,"SSID%d", i+1);

		memset(attrValue, 0, sizeof(attrValue) );
		tcapi_get(nodeName, attr, attrValue);
		
		if( !strcmp(attrValue,"Yes"))
		{
			if( flagComma)
			{
				len = strlen(buf);
				sprintf(buf+len, ",");
			}
			len = strlen(buf);
			sprintf(buf+len, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d", i+1);
			flagComma = 1;
		}
	}
	
			

	return getTheValue(value, type, deviceNode, buf);
	
}
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 isPortDuplicated(char *portbuf, int ifaceidx, char *ipver)
{
#define PORTCNT	4
#define LAN_PORTMAP(x)	(1 << x)
#define WLAN_PORTMAP(x) (1 << (x+PORTCNT))
	char ifa_buf[128] = {0};
	char nodeName[32] = {0};
	char active[8] = {0};
	char ipverother[20] = {0};
	char portAttr[16] ={0};
	char portAttrValue[8] ={0};
	uint8 idx = 0;
	uint16 portmap = 0;
	uint8 pvc_idx = 0;
	uint8 entry_idx = 0;
	int ifindex = 0;
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
	int retcode = 0;
#endif

	if ( NULL == portbuf || NULL == ipver )
		return -1;

	// 1. check new state: is port bind
	for ( idx = 0; idx < PORTCNT; idx ++ )
	{
		sprintf(ifa_buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.%d", idx + 1);
		if( NULL != strstr(portbuf, ifa_buf) )
			portmap |= LAN_PORTMAP(idx);
	}
	for ( idx = 0; idx < PORTCNT; idx ++ )
	{
		sprintf(ifa_buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d", idx + 1);
		if( NULL != strstr(portbuf, ifa_buf) )
			portmap |= WLAN_PORTMAP(idx);
	}

	// no need to check if no port bind
	if ( 0 == portmap )
		return 0;

	// 2. check port duplicate from every pvc entry
	for ( pvc_idx = 0; pvc_idx < PVC_NUM; pvc_idx ++ )
	{
		for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx ++ )
		{
			ifindex = (pvc_idx<<3|entry_idx);
			if ( ifindex == ifaceidx ) // self
				continue;

			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_idx, entry_idx);
			if ( tcapi_get(nodeName, "Active", active) < 0 ) // pvc is not exist
				continue;

			memset(ipverother, 0, sizeof(ipverother));
			if ( tcapi_get(nodeName, "IPVERSION", ipverother) < 0
				|| ( NULL == strstr(ipverother, ipver) && NULL == strstr(ipver, ipverother)  ) )
				continue;

			// check LAN
			for ( idx = 0; idx < PORTCNT; idx ++ )
			{
				sprintf(portAttr, "LAN%d", idx + 1);
				memset(portAttrValue, 0, sizeof(portAttrValue));
				if ( tcapi_get(nodeName, portAttr, portAttrValue) < 0
					|| 0 != strcmp(portAttrValue, "Yes") )
					continue;

				if ( portmap & LAN_PORTMAP(idx))
				{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
					tcapi_set(nodeName, portAttr, "No");
					retcode = 1;
					SetCfgNodeFlag("Wan",
						(pvc_idx<<3|entry_idx) + COMMIT_THIRD_ALIAS);
#else
					return 1;
#endif
				}
			}

			// check WLAN
			for ( idx = 0; idx < PORTCNT; idx ++ )
			{
				sprintf(portAttr, "SSID%d", idx + 1);
				memset(portAttrValue, 0, sizeof(portAttrValue));
				if ( tcapi_get(nodeName, portAttr, portAttrValue) < 0
					|| 0 != strcmp(portAttrValue, "Yes") )
					continue;

				if ( portmap & WLAN_PORTMAP(idx))
				{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
					tcapi_set(nodeName, portAttr, "No");
					retcode = 1;
					SetCfgNodeFlag("Wan",
						(pvc_idx<<3|entry_idx) + COMMIT_THIRD_ALIAS);
#else
					return 1;
#endif
				}

			}
		}
	}
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
	return retcode;
#else
	return 0;
#endif
}
#endif

#if defined(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE)
#if defined(TCSUPPORT_CT_2PORTS)
#define LANPORTCNT	2 
#define WLANPORTCNT 0 
#else 
#if defined(TCSUPPORT_CT_1PORT)
#define LANPORTCNT	1 
#define WLANPORTCNT 0 
#else
#define LANPORTCNT	4
#define WLANPORTCNT 4
#endif
#endif
#endif

int8 setWANCnt1_CTComInterfaceBondValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0}, active[8] = {0};
	char attr[8] = {0}, attrValue[8] = {0};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret,i;
	char *ptrOri =(char *)value;
	char *ptrTmp =NULL;
	char buf[128] = {0};
	//int flagWlan = 0,
	//int flagWan = 0;
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	char ipver[20] = {0};
#endif
#if defined(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE)
	int compatible = 0;
#endif
		
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	tcapi_get(nodeName, "IPVERSION", ipver);
	if ( 1 == isPortDuplicated(ptrOri, (wanNodeIndex<<3|pvcNodeEntry), ipver ) )
	{
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_GD)
		tcdbg_printf("\nWarning: duplicated lan ports bind info"
						"and updated with new bind info\n");
#else
		return -1;
#endif
	}
#endif

#if defined(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE)
	ret = tcapi_get("Cwmp_Entry", "workCompatible", attrValue);
	if(ret){
		compatible = 0;
	}
	else
	{
		compatible = atoi(attrValue);
	}
#endif	

	for(i=0;i<4;i++)
	{
		sprintf(attr,"LAN%d",i+1);
		sprintf(buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.%d",i+1);
		if( NULL != strstr(ptrOri, buf))
		{
#if defined(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE)
			if(compatible)
			{
				if(i < LANPORTCNT)
				{
					tcapi_set(nodeName, attr, "Yes");
				}
				else
				{
					return -1;	
				}			
			}
			else
#endif				
			tcapi_set(nodeName, attr, "Yes");
			//flagWan = 1;
		}
		else
			tcapi_set(nodeName, attr, "No");
	}

	for(i=0;i<4;i++)
	{
		sprintf(attr,"SSID%d",i+1);
		sprintf(buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d",i+1);
		if( NULL != strstr(ptrOri, buf))
		{
#if defined(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE)		
			if(compatible)
			{
				if(i < WLANPORTCNT)
				{
					tcapi_set(nodeName, attr, "Yes");
				}
				else
				{
					return -1;	
				}			
			}
			else
#endif				
			tcapi_set(nodeName, attr, "Yes");
		}
		else
			tcapi_set(nodeName, attr, "No");
	}

		SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry)+COMMIT_THIRD_ALIAS );


	return 0;
}

int8 getWANIPCnt1_CTComInterfaceBondAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANCnt1_CTComInterfaceBond[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_CTComInterfaceBondAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANCnt1_CTComInterfaceBond[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_CTComDHCPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char dhcpEn[8] = {0};

	char serviceList[SERVICE_LIST_LEN] = {0};

	char nodeName[34] = {0}, active[8] = {0};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}

	sprintf(nodeName, "Wan_PVC%d_Entry%d",wanNodeIndex,pvcNodeEntry );
	ret = tcapi_get(nodeName, "DHCPEnable", dhcpEn);
	if(ret)
	{
		tcapi_get(nodeName, "ServiceList", serviceList);
		memset(dhcpEn, 0, sizeof(dhcpEn) );
		if(0 == strcmp(serviceList,"OTHER"))
		{
			
			strcpy(dhcpEn, "0");
		}
		else
		{
			strcpy(dhcpEn, "1");
		}
	}

	
	return getTheValue(value, type, deviceNode, dhcpEn);
	
}

int8 setWANCnt1_CTComDHCPEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char dhcpEn[34] = {'\0'}, active[8] = {0};
	int var;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
		
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}

	var = *((uint8*)value);
	strcpy(dhcpEn, (char *)value);

	if(var == 1) {
		tcapi_set(nodeName, "DHCPEnable", "1");
		tcapi_set(nodeName, "DHCPRealy","No");
	}
	else {
		tcapi_set(nodeName, "DHCPEnable", "0");
		tcapi_set(nodeName, "DHCPRealy","Yes");
	}
	
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry)+100 );
	return 0;
}

int8 getWANIPCnt1_CTComDHCPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANCnt1_CTComDHCPEnable[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_CTComDHCPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANCnt1_CTComDHCPEnable[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif
int	cwmpInitWANGetPortMappingFromFlash( DeviceNodePtr deviceNode )
{
	char nodeName[34];
	char natEnable[32];
	char nodeIndex[10];
	char localIP[32];
	uint32 wanNodeIndex,pvcNodeEntry;
	int i, ret, rtnCode = 0;

	if(deviceNode == NULL){
		return -1;
	}

	/*clear all char array to 0*/
	memset( nodeName, 0, sizeof(nodeName) );
	memset( natEnable, 0, sizeof(natEnable) );
	memset( nodeIndex, 0, sizeof(nodeIndex) );
	memset( localIP, 0, sizeof(localIP) );
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "NATENABLE", natEnable);

	/*if nat of this pvc is disabled, just return.*/
	if ( strcmp(natEnable, "Enable") != 0 || strlen(natEnable) == 0 ) {
		return 0;
	}

	for ( i = 0; i < MAX_VIRSERV_RULE; i++ ) {
		sprintf( nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, i );

		/*check if this virserver entry is exist*/
		ret = tcapi_get( nodeName, "LOCALIP", localIP );
		if ( ret != 0 ) {
			continue;
		}

		sprintf( nodeIndex, "%d", i + 1 );
		PortMappingIndex[0].name = nodeIndex;

		/*register this node*/
		if ( RegisterNodeFromTable( deviceNode, PortMappingIndex ) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}

	return rtnCode;
}

int8 DeviceNodeAddPMappingObject(uint32* num, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char localIP[32] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	DeviceNodePtr WANCntDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	int rtnCode = 0;
	int i, ret;

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &WANCntDeviceNode);
	if(WANCntDeviceNode == NULL){
		return -1;
	}

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	for(i = 0; i < MAX_VIRSERV_RULE; i++){
		sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3 | pvcNodeEntry, i);
		ret = tcapi_get(nodeName, "LOCALIP", localIP);
		//if(!strlen(localIP)) //it means this entry is not configured, we can use it for add new object.
		//	break;
		if (ret < 0) {
		   break;
		}
	}

	if(i == MAX_VIRSERV_RULE){ //there is no free Entry
		return -1;
	}

	*num = i + 1;

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3 | pvcNodeEntry, i);

	tcapi_set(nodeName, "Active", "No");
	tcapi_set(nodeName, "SrcIP", "");
	tcapi_set(nodeName, "STARTPORT", "0");
	tcapi_set(nodeName, "ENDPORT", "0");
	tcapi_set(nodeName, "Protocal", "");
	tcapi_set(nodeName, "LOCALIP", "");
	tcapi_set(nodeName, "MapName", "");

	//we should set other attributes here.
	
	//it should do tcapi_commit somewhere.
	//tcapi_commit(nodeName);
	
	cwmpFreeDeviceTable(WANCntDeviceNode);
	rtnCode = cwmpInitWANConnectionDeviceTable(WANCntDeviceNode);

	/*we have deleted node: "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}", here we should init it */
	if ( InitDynamicNode( rpc.IGDDeviceRootPtr, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice." ) != 0 ) {
		tc_printf("==>DeviceNodeAddPMappingObject(): RegisterNode Dynamic ERROR\n");
	}
	
	return rtnCode;
}

int8 DeviceNodeDelPMappingObject(uint32 num, DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	uint8 pMappingIndex = 0;
	uint32 wanNodeIndex,pvcNodeEntry;
	DeviceNodePtr WANCntDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	int rtnCode = 0;

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &WANCntDeviceNode);
	if(WANCntDeviceNode == NULL){
		return -1;
	}
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name) - 1;

	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif	
	if( (num > 0) && (num < MAX_VIRSERV_RULE) ){
		pMappingIndex = num - 1;
	}

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, pMappingIndex);
	tc_printf("nodeName is %s\n", nodeName);
	
	tcapi_unset(nodeName);
	//it should do tcapi_commit somewhere.
	tcapi_commit(nodeName);

	cwmpFreeDeviceTable(WANCntDeviceNode);
	rtnCode = cwmpInitWANConnectionDeviceTable(WANCntDeviceNode);

	/*we have deleted node: "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}", here we should init it */
	if ( InitDynamicNode( rpc.IGDDeviceRootPtr, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice." ) != 0 ) {
		tc_printf("==>DeviceNodeDelPMappingObject(): RegisterNode Dynamic ERROR\n");
	}
	
	return rtnCode;
}

int8 getIPPMapping1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char  IPPMapping1Enable[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 instance; 
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, instance);

	tcapi_get(nodeName, "Active", IPPMapping1Enable);

	if(!strcmp(IPPMapping1Enable, "Yes")){
		strcpy(IPPMapping1Enable, "1");
	}
	else /* if(!strcmp(IPPMapping1Enable, "No"))*/{
		strcpy(IPPMapping1Enable, "0");
	}
	
	return getTheValue(value, type, deviceNode, IPPMapping1Enable);
}
int8 setIPPMapping1EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	//char wanNode[34] = {'\0'};
	char nodeName[51] = {'\0'};
	uint8 IPPMapping1Enable;
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 instance; 

	IPPMapping1Enable = *(uint8*)value;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;
	
	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, instance);
	//sprintf( wanNode, "VirServer_Entry%d", wanNodeIndex );

	if( IPPMapping1Enable == 1 ){
		tcapi_set(nodeName, "Active", "Yes");
	}
	else if( IPPMapping1Enable == 0 ){
		tcapi_set(nodeName, "Active", "No");
	}
	else{
		return -1;
	}
	SetCfgNodeFlag ( "VirServer", instance+( wanNodeIndex<<3|pvcNodeEntry)*10 +COMMIT_THIRD_ALIAS);	
	return 0;
}

int8 getIPPMapping1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIPPMapping1Enable[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPPMapping1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aIPPMapping1Enable[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getIPPMapping1LeaseDurValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char leaseTime[10] = {"0"}; 	//set default value to 0.
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 instance; 
	int ret = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3 |pvcNodeEntry, instance);

	ret = tcapi_get(nodeName, "LEASEDUR", leaseTime);	
	if (ret < 0 || ((ret == 0) && !strcmp(leaseTime, "N/A")) || strlen(leaseTime) == 0) {
		tcdbg_printf("getIPPMapping1LeaseDurValue : value not exist!\n");
		strcpy(leaseTime,"0");
	}
	return getTheValue(value, type, deviceNode, leaseTime);
}
int8 setIPPMapping1LeaseDurValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char leaseDur[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 instance; 
	uint32 leaseTime;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, instance);
	
	leaseTime = *((uint32*)value);
	if(leaseTime != 0){
		tc_printf("==>setIPPMapping1LeaseDurValue:leaseTime should be 0.");
		return -1;
	}

	sprintf(leaseDur, "%d", leaseTime);
	tcapi_set(nodeName, "LEASEDUR", leaseDur);
	
	/*it should not set flag for do  commit here. for we do not need update vserver.sh.*/
	return 0;
}
int8 getIPPMapping1LeaseDurAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIPPMapping1LeaseDur[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPPMapping1LeaseDurAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	attribute = &tcIpConAttr.aIPPMapping1LeaseDur[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getIP1RemoteHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char  IP1RemoteHost[32] = {"0.0.0.0"};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 instance; 

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, instance);

	tcapi_get(nodeName, "SrcIP", IP1RemoteHost);

	return getTheValue(value, type, deviceNode, IP1RemoteHost);
}
int8 setIP1RemoteHostValue(void* value, struct DeviceNode*  deviceNode)
{
	//char wanNode[34] = {0};
	char nodeName[51] = {'\0'};
	char  IP1RemoteHost[32] = {"0.0.0.0"};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 instance; 

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;

	sprintf( nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, instance );
	//sprintf( wanNode, "VirServer_PVC%d", wanNodeIndex );

	
	
	if( InternalClientIpCheck((char*)value) != 1 ){
		return -1;
	}

	strcpy(IP1RemoteHost,(char*)value);

	tcapi_set(nodeName, "SrcIP", IP1RemoteHost);

	SetCfgNodeFlag ( "VirServer", instance+( wanNodeIndex<<3|pvcNodeEntry)*10 +COMMIT_THIRD_ALIAS );
	return 0;
}
int8 getIP1RemoteHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIP1RemoteHost[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIP1RemoteHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aIP1RemoteHost[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getIP1ExternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char  IP1ExternalPort[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 portmapindex;
	int ret = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	ret = tcapi_get(nodeName, "STARTPORT", IP1ExternalPort);
	if (ret < 0 || ((ret == 0) && !strcmp(IP1ExternalPort, "N/A")) ||  strlen(IP1ExternalPort) == 0) {
		tcdbg_printf("getIP1ExternalPortValue : value not exist!\n");
		strcpy(IP1ExternalPort,"0");
	}
	return getTheValue(value, type, deviceNode, IP1ExternalPort);
}
int8 setIP1ExternalPortValue(void* value, struct DeviceNode*  deviceNode)
{	
	//char wanNode[34] = {0};
	char nodeName[51] = {'\0'};
	uint32 wanNodeIndex, pvcNodeEntry;
	uint8 portmapindex;
	char IP1ExternalPort[10] = {'\0'};

	uint32 port = *((uint32*)value);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1; 
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 

	//rpc.cfgFlags.theSettingFlag0 |= CFG_NODE_VIRSERVER;
	if( (port < 1) || (port > 65535) ) {
		return -1;
	}

	sprintf(IP1ExternalPort, "%lu", port);

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	tcapi_set(nodeName, "STARTPORT", IP1ExternalPort);
	tcapi_set(nodeName, "ENDPORT", IP1ExternalPort);

	//sprintf( wanNode, "VirServer_PVC%d", wanNodeIndex );
	SetCfgNodeFlag ( "VirServer", portmapindex +(wanNodeIndex<<3|pvcNodeEntry) *10 + COMMIT_THIRD_ALIAS);
	return 0;
}
int8 getIP1ExternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIP1ExternalPort[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIP1ExternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aIP1ExternalPort[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getIPExternalPortEndRangeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	
	char  ExternalPortEndRange[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 portmapindex;
	int ret = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	ret = tcapi_get(nodeName, "ENDPORT", ExternalPortEndRange);
	if (ret < 0 || ((ret == 0) && !strcmp(ExternalPortEndRange, "N/A")) || strlen(ExternalPortEndRange) == 0) {
		tcdbg_printf("getIPExternalPortEndRangeValue : value not exist!\n");
		strcpy(ExternalPortEndRange,"0");
	}
	return getTheValue(value, type, deviceNode, ExternalPortEndRange);
}
int8 setIPExternalPortEndRangeValue(void* value, struct DeviceNode*  deviceNode)
{
	char wanNode[34] = {0};
	char nodeName[51] = {'\0'};
	char stPort[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 portmapindex;
	
	uint32 startPort = 0,endPort = 0;
	char  vIPExternalPortEndRange[10] = {'\0'};

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1; 
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);
	
	endPort =  *((uint32*)value);
	
	if(endPort < 1 || endPort > 65535){
		tcdbg_printf("error 1, end port %d\n", endPort);
		return -1;
	}
	
	tcapi_get(nodeName, "STARTPORT", stPort);
	startPort = atoi(stPort);

	if(startPort > endPort){
		tcdbg_printf("error 2, start port:%d end port:%d\n", startPort, endPort);
		return -1;
	}
	
	sprintf(vIPExternalPortEndRange, "%lu", endPort);

	tcapi_set(nodeName, "ENDPORT", vIPExternalPortEndRange);

	
	return 0;
}
int8 getIPExternalPortEndRangeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	
	if (notification==NULL || acl==NULL ||deviceNode==NULL)
	{
		tc_printf("\r\n getIPExternalPortEndRangeAttribute(), Pointer == NULL!");
		return -1;		
	}

	attribute = tcIpConAttr.aExternalPortEndRange[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPExternalPortEndRangeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	if (acl==NULL || deviceNode==NULL)
	{
		tc_printf("\r\n setIPExternalPortEndRangeAttribute(), Pointer == NULL!");
		return -1;		
	}
	
	attribute = &tcIpConAttr.aExternalPortEndRange[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getIP1InternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char  IP1InternalPort[32];
	uint32 wanNodeIndex, pvcNodeEntry;
	uint8 portmapindex;
	int ret = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	memset(IP1InternalPort, 0, sizeof(IP1InternalPort));
	ret = tcapi_get(nodeName, "INTPORT", IP1InternalPort);
	if (ret < 0 || ((ret == 0) && !strcmp(IP1InternalPort, "N/A")) || strlen(IP1InternalPort) == 0) {
		tcdbg_printf("getIP1InternalPortValue : value not exist!\n");
		strcpy(IP1InternalPort,"0");
	}

	return getTheValue(value, type, deviceNode, IP1InternalPort);
}
int8 setIP1InternalPortValue(void* value, struct DeviceNode*  deviceNode)
{
	//char wanNode[34] = {0};
	char nodeName[51] = {'\0'};
	uint32 intPort;
	char  IP1InternalPort[32];
	uint32 wanNodeIndex, pvcNodeEntry;
	uint8 portmapindex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	intPort = *((uint32*)value);
	if (intPort < 1 || intPort > 65535) {
		return -1;
	}
	
	sprintf(IP1InternalPort, "%lu", intPort);
	tcapi_set(nodeName, "INTPORT", IP1InternalPort);
	
	//sprintf( wanNode, "VirServer_PVC%d", wanNodeIndex );
	SetCfgNodeFlag ( "VirServer", portmapindex+(wanNodeIndex<<3|pvcNodeEntry)*10 + COMMIT_THIRD_ALIAS);
	return 0;
}
int8 getIP1InternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIP1InternalPort[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIP1InternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aIP1InternalPort[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getIPPMapping1ProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char  IPPMapping1Protocol[8];
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 portmapindex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	tcapi_get(nodeName, "Protocal", IPPMapping1Protocol);
	
	return getTheValue(value, type, deviceNode, IPPMapping1Protocol);
}
int8 setIPPMapping1ProtocolValue(void* value, struct DeviceNode*  deviceNode)
{
	//char wanNode[34] = {0};
	char nodeName[51] = {'\0'};
	char  IPPMapping1Protocol[8];
	uint32 wanNodeIndex, pvcNodeEntry;
	uint8 portmapindex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1; 
	
	if(strlen((char*)value) > 7)
		return -1;

	strcpy(IPPMapping1Protocol,(char*)value);
	
	/*here we support TCP/UDP and ALL three protocol*/
	if ( (strcmp(IPPMapping1Protocol, "TCP") != 0) && (strcmp(IPPMapping1Protocol, "UDP") != 0) && (strcmp(IPPMapping1Protocol,"TCP/UDP") != 0) ) {	
		return -1;
	}
	
	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	tcapi_set(nodeName, "Protocal", IPPMapping1Protocol);

	//sprintf( wanNode, "VirServer_PVC%d", wanNodeIndex );
	//SetCfgNodeFlag ( wanNode, portmapindex );
	SetCfgNodeFlag ( "VirServer", portmapindex+(wanNodeIndex<<3|pvcNodeEntry)*10 + COMMIT_THIRD_ALIAS);
	return 0;
}
int8 getIPPMapping1ProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIPPMapping1Protocol[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPPMapping1ProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aIPPMapping1Protocol[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getIP1InternalClientValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char  IP1InternalClient[32] = {'\0'};
	
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 portmapindex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1;

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);
	
	tcapi_get(nodeName, "LOCALIP", IP1InternalClient);
	
	return getTheValue(value, type, deviceNode, IP1InternalClient);
}
int8 setIP1InternalClientValue(void* value, struct DeviceNode*  deviceNode)
{
	//char wanNode[34] = {0};
	char nodeName[51] = {'\0'};
	char  IP1InternalClient[32] = {'\0'};
	
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 portmapindex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	portmapindex =  atoi(deviceNode->parentNode->name) - 1;
	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, portmapindex);

	if(strcmp((char*)value, "0.0.0.0") == 0){
		return -1;
	}
	if( InternalClientIpCheck((char*)value) != 1 && strcmp((char*)value, "255.255.255.255") != 0){
		return -1;
	}
	strcpy(IP1InternalClient,(char*)value);
	tcapi_set(nodeName, "LOCALIP", IP1InternalClient);
	//sprintf( wanNode, "VirServer_PVC%d", wanNodeIndex );
	SetCfgNodeFlag ( "VirServer", portmapindex +(wanNodeIndex<<3|pvcNodeEntry)*10 +COMMIT_THIRD_ALIAS);
	return 0;
}
int8 getIP1InternalClientAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIP1InternalClient[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIP1InternalClientAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aIP1InternalClient[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPPMapping1DescripValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[51] = {'\0'};
	char active[10] = {'\0'};
	char vIPPMapping1Descrip[64] = {'\0'};
	
	uint32 wanNodeIndex,pvcNodeEntry;
	uint8 instance;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;

	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, instance);
	tcapi_get(nodeName, "MapName", vIPPMapping1Descrip);
	
	return getTheValue(value, type, deviceNode, vIPPMapping1Descrip);
}
int8 setIPPMapping1DescripValue(void* value, struct DeviceNode*  deviceNode)
{
	//char wanNode[34] = {0};
	char nodeName[51] = {'\0'};
	char active[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry, instance;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	instance = atoi(deviceNode->parentNode->name) - 1;
	
	if(strlen((char*)value) > 63){
		return -1;
	}
	
	sprintf(nodeName, "VirServer_Entry%d_Entry%d", wanNodeIndex<<3|pvcNodeEntry, instance);
	tcapi_set(nodeName, "MapName", (char*)value);

	/*it need not do commit here.*/
	//sprintf( wanNode, "VirServer_PVC%d", wanNodeIndex );
	SetCfgNodeFlag ( "VirServer", instance+(wanNodeIndex<<3|pvcNodeEntry)*10 + COMMIT_THIRD_ALIAS );
	return 0;
}
int8 getIPPMapping1DescripAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aIPPMapping1Descrip[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPPMapping1DescripAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aIPPMapping1Descrip[getWANNodeIndexForPortMapping(deviceNode)][getPortMappingIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getEthernetBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char tempValue[32] = {"0"}; //set default value to 0.

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Info_nas%d.%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "txbytes", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getEthernetBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANIPCnt1_1EthBytesSent[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthernetBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 ||pvcNodeEntry < 0 || pvcNodeEntry > 7){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANIPCnt1_1EthBytesSent[wanNodeIndex<<3|pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getEthernetBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char tempValue[32] = {"0"}; //set dafault value to 0.

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Info_nas%d.%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "rxbytes", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getEthernetBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 ||pvcNodeEntry < 0 ||pvcNodeEntry > 7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANIPCnt1_1EthBytesRcv[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthernetBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 ||pvcNodeEntry < 0 ||pvcNodeEntry > 7){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANIPCnt1_1EthBytesRcv[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getEthernetPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char tempValue[32] = {"0"};	//set default value to 0.

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Info_nas%d.%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "txpackets", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getEthernetPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 || pvcNodeEntry > 7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANIPCnt1_1EthPktsSent[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthernetPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0, pvcNodeEntry= 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANIPCnt1_1EthPktsSent[wanNodeIndex<<3 |pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getEthernetPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char tempValue[32] = {"0"}; //set default value to 0.

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Info_nas%d.%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "rxpackets", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getEthernetPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANIPCnt1_1EthPktRcv[wanNodeIndex<<3 |pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthernetPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANIPCnt1_1EthPktRcv[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#if defined(TCSUPPORT_CT_FJ)
int8 getEthernetErrorsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char tempValue[32] = {"0"}; //set default value to 0.

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Info_nas%d.%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "rxerrors", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getEthernetErrorsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANIPCnt1_1EthErrRcv[wanNodeIndex<<3 |pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthernetErrorsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANIPCnt1_1EthErrRcv[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getEthernetDiscardPktsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char tempValue[32] = {"0"}; //set default value to 0.

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Info_nas%d.%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName, "rxdropped", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 getEthernetDiscardPktsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7){
		return -1;
	}

	attribute = tcIpConAttr.aWANIPCnt1_1EthDisPktRcv[wanNodeIndex<<3 |pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEthernetDiscardPktsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->parentNode->name)-1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANIPCnt1_1EthDisPktRcv[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif
int8 getWANPPPCnt1_1UsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	uint32 wannodeindex,pvcNodeEntry;
	char isp[10] = {'\0'};
	char WANPPPCnt1_1Username[64] = {'\0'};
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wannodeindex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wannodeindex, &pvcNodeEntry);
#else
	wannodeindex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wannodeindex,pvcNodeEntry);

	tcapi_get(nodeName, "ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_PPP)){
		tcapi_get(nodeName, "USERNAME", WANPPPCnt1_1Username);
	}
	
	return getTheValue(value, type, deviceNode, WANPPPCnt1_1Username);
}
int8 setWANPPPCnt1_1UsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	char isp[10] = {'\0'};
	char WANPPPCnt1_1Username[65];
	char active[10] = {'\0'};
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", active);
	if(ret){	//this pvc is not exist.
		return -1;
	}

	if(strlen((char*)value) > 64){
		tc_printf("==>setWANPPPCnt1_1UsernameValue:The length of useraname should not be more than 64.\n");
		return -1;

	}
	
	strcpy(WANPPPCnt1_1Username,(char*)value);

		tcapi_set(nodeName, "USERNAME", WANPPPCnt1_1Username);

	
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3 | pvcNodeEntry) + COMMIT_THIRD_ALIAS);
	return 0;
}

int8 getWANPPPCnt1_1UsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANPPPCnt1_1Username[getWANConnectionIndex(deviceNode)];

#if defined(TCSUPPORT_CT)
	if (attribute & 0x01)
		attribute &= 0xfe;
	else
		attribute |= 0x01;
#endif

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPPPCnt1_1UsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANPPPCnt1_1Username[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

#if defined(TCSUPPORT_CT)
	if (*attribute & 0x01)
		*attribute &= 0xfe;
	else
		*attribute |= 0x01;
#endif

	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANPPPCnt1_1PasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  WANPPPCnt1_1Password[65];

	WANPPPCnt1_1Password[0] = 0;		

	return getTheValue(value, type, deviceNode, WANPPPCnt1_1Password);
}
int8 setWANPPPCnt1_1PasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	char WANPPPCnt1_1Password[65];
	char isp[10] = {'\0'};
	char active[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;


#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex=atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){	//this pvc is not exist.
		return -1;
	}
	
	if(strlen((char*)value) > 64){
		return -1;
	}
	
	strcpy(WANPPPCnt1_1Password,(char*)value);
	
		tcapi_set(nodeName, "PASSWORD", WANPPPCnt1_1Password);
		
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3 |pvcNodeEntry) + COMMIT_THIRD_ALIAS );	
	return 0;
}

int8 getWANPPPCnt1_1PasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANPPPCnt1_1Password[getWANConnectionIndex(deviceNode)];
	
#if defined(TCSUPPORT_CT)
	if (attribute & 0x01)
		attribute &= 0xfe;
	else
		attribute |= 0x01;
#endif

	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setWANPPPCnt1_1PasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANPPPCnt1_1Password[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

#if defined(TCSUPPORT_CT)
	if (*attribute & 0x01)
		*attribute &= 0xfe;
	else
		*attribute |= 0x01;
#endif


	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANPPPCnt1_1TransTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	char WANPPPCnt1_1TransType[8] = {'\0'};
	char encap[32] = {'\0'};
	char isp[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);
	tcapi_get(nodeName,"ISP", isp);
	if(!strcmp(isp, WAN_ENCAP_PPP)){ //pppoe/pppoa
		strcpy(WANPPPCnt1_1TransType,"PPPoE");
	}
	else{
		strcpy(WANPPPCnt1_1TransType," ");
	}
	
	return getTheValue(value, type, deviceNode, WANPPPCnt1_1TransType);
}
int8 getWANPPPCnt1_1TransTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANIPCnt1_1TransportType[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPPPCnt1_1TransTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANIPCnt1_1TransportType[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
//#ifdef TC_TR069
int8 getWANPPPCnt1_1AuthenTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char AuthenType[16] = {'\0'};
	char authType[16] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	tcapi_get(nodeName, "AUTHEN", AuthenType);
	if(!strcmp(AuthenType, "CHAP")){
		strcpy(authType, "CHAP");
	}
	else if(!strcmp(AuthenType, "PAP")){
		strcpy(authType, "PAP");
	}
	else if(!strcmp(AuthenType, "BOTH")){
		strcpy(authType, "BOTH");
	}

	return getTheValue(value, type, deviceNode, authType);
}
int8 setWANPPPCnt1_1AuthenTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34];
	char active[10];
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

	memset(nodeName, 0, sizeof(nodeName));
	memset(active, 0, sizeof(active));
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry", wanNodeIndex,pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", active);
	if(ret){ //this pvc is not exist.
		return -1;
	}

	if(!strcmp((char*)value, "CHAP")){
		tcapi_set(nodeName, "AUTHEN", "CHAP");
	}
	else if(!strcmp((char*)value, "PAP")){
		tcapi_set(nodeName, "AUTHEN", "PAP");
	}
	else if(!strcmp((char*)value, "BOTH")){
		tcapi_set(nodeName, "AUTHEN", "BOTH");
	}
	else{
		return -1;
	}

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	return 0;
}
int8 getWANPPPCnt1_1AuthenTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.AuthenType[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setWANPPPCnt1_1AuthenTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8  *attribute = NULL;
	int8  result;

	attribute = &tcIpConAttr.AuthenType[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANPPPCnt1_1PPPoEACNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char pppoeCAname[257] = {'\0'}; //The max length of service name is 256.
	uint32 wanNodeIndex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	tcapi_get(nodeName, "CANAME", pppoeCAname);
	
	return getTheValue(value, type, deviceNode, pppoeCAname);	
}
int8 setWANPPPCnt1_1PPPoEACNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->name) - 1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	if(strlen((char*)value) > 256){
		return -1;
	}
	
	tcapi_set(nodeName, "CANAME", (char*)value);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex <<3 |pvcNodeEntry) +COMMIT_THIRD_ALIAS);
	return 0;
}
int8 getWANPPPCnt1_1PPPoEACNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANPPPCnt1_1PPPoEACName[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	
	return result;
}
int8 setWANPPPCnt1_1PPPoEACNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8  *attribute = NULL;
	int8  result;

	attribute = &tcIpConAttr.aWANPPPCnt1_1PPPoEACName[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getWANPPPCnt1_1PPPoESvcNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char srvname[256] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry =  atoi(deviceNode->parentNode->name) - 1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "SRVNAME", srvname);
	
	return getTheValue(value, type, deviceNode, srvname);
}
int8 setWANPPPCnt1_1PPPoESvcNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex,pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}


	
	if(strlen((char*)value) > 256){	
		return -1;
	}
	
	tcapi_set(nodeName, "SRVNAME", (char*)value);
	
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	return 0;
}
int8 getWANPPPCnt1_1PPPoESvcNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcIpConAttr.aWANPPPCnt1_1PPPoESvcName[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPPPCnt1_1PPPoESvcNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcIpConAttr.aWANPPPCnt1_1PPPoESvcName[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_E8DDNS)
int8 getDDNSCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;
	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);
	
	tcapi_get(nodeName, "Active", active);
	
	if(!strcmp(active, "Yes")){
		strcpy(active, "1");
	}
	else{
		strcpy(active, "0");
	}
	
	return getTheValue(value, type, deviceNode, active);
}
int8 setDDNSCfgEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	uint8 act;
	char wan_node[40] = {0}, ifacename[32] = {0}, wanmode[16] = {0}, tmpval[64] = {0};

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;
	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);

	act = *((uint8*)value);
	if(act == 1){
		tcapi_set(nodeName, "Active", "Yes");
	}
	else if(act == 0){
		tcapi_set(nodeName, "Active", "No");
	}
	else{
		
		return -1;
	}

	if ( 0 != tcapi_get(nodeName, "DDNS_Name", tmpval) )
		tcapi_set(nodeName, "DDNS_Name", "China Telecom");

	if ( 0 != tcapi_get(nodeName, "DDNS_Protocal", tmpval) )
		tcapi_set(nodeName, "DDNS_Protocal", "GNUDip.http");

	if ( 0 != tcapi_get(nodeName, "SERVERNAME", tmpval) )
		tcapi_set(nodeName, "SERVERNAME", "ns.eagleeyes.com.cn");

	if ( 0 != tcapi_get(nodeName, "DDNS_Interface", tmpval) )
	{
		sprintf(wan_node, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
		memset(tmpval, 0, sizeof(tmpval));
		memset(ifacename, 0, sizeof(ifacename));
		if ( 0 == tcapi_get(wan_node, "IFName", ifacename)
			&& 0 == tcapi_get(wan_node, "ServiceList", tmpval)
			&& 0 == tcapi_get(wan_node, "WanMode", wanmode)
			&& (NULL != strstr(tmpval, "INTERNET") || NULL != strstr(tmpval, "OTHER"))
			&& 0 == strcmp(wanmode, "Route") )
			tcapi_set(nodeName, "DDNS_Interface", ifacename);
	}
	
	SetCfgNodeFlag ( "Ddns",  ddnsNodeIndex);
	
	return 0;
}
int8 getDDNSCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDDNSCfgEnable[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDDNSCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDDNSCfgEnable[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int8 getDDNSProviderValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char provder[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;

	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);
	
	tcapi_get(nodeName, "DDNS_Name", provder);
	return getTheValue(value, type, deviceNode, provder);

}
int8 setDDNSProviderValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char provder[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	uint8 act;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;
	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);

	if(strlen((char*)value) > 256){	
		return -1;
	}
	
	tcapi_set(nodeName, "DDNS_Name", (char*)value);
	
	SetCfgNodeFlag ( "Ddns", ddnsNodeIndex);
	return 0;
}
int8 getDDNSProviderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDDNSProvider[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDDNSProviderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDDNSProvider[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDDNSUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char username[256] = {'\0'};	
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;

	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);
	
	tcapi_get(nodeName, "USERNAME", username);
	return getTheValue(value, type, deviceNode, username);
}
int8 setDDNSUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char username[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	uint8 act;
		
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;

	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);

	if(strlen((char*)value) > 256){	
		return -1;
	}
	
	tcapi_set(nodeName, "USERNAME", (char*)value);
	
	SetCfgNodeFlag ( "Ddns", ddnsNodeIndex);
	return 0;
}
int8 getDDNSUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDDNSUsername[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDDNSUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDDNSUsername[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDDNSPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char password[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;

	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);
	
	tcapi_get(nodeName, "PASSWORD", password);
	return getTheValue(value, type, deviceNode, password);
}
int8 setDDNSPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char password[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	uint8 act;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;

	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);

	if(strlen((char*)value) > 256){	
		return -1;
	}
	
	tcapi_set(nodeName, "PASSWORD", (char*)value);
	
	SetCfgNodeFlag ( "Ddns", ddnsNodeIndex);
	return 0;
}
int8 getDDNSPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDDNSPassword[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDDNSPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDDNSPassword[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getServicePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char port[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;
	
	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);
	
	tcapi_get(nodeName, "SERVICEPORT", port);
	return getTheValue(value, type, deviceNode, port);
}
int8 setServicePortValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char port[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	uint8 act;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;

	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);

	if(strlen((char*)value) > 256){	
		return -1;
	}
	
	tcapi_set(nodeName, "SERVICEPORT", (char*)value);
	
	SetCfgNodeFlag ( "Ddns", ddnsNodeIndex);
	return 0;
}
int8 getServicePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComServicePort[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
}
int8 setServicePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComServicePort[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDDNSDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char domainName[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;
	
	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);
	
	tcapi_get(nodeName, "DDNS_Domain", domainName);
	return getTheValue(value, type, deviceNode, domainName);
}
int8 setDDNSDomainNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char domainName[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	uint8 act;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;
	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);

	if(strlen((char*)value) > 256){	
		return -1;
	}
	
	tcapi_set(nodeName, "DDNS_Domain", (char*)value);
	
	SetCfgNodeFlag ( "Ddns", ddnsNodeIndex);
	return 0;
}
int8 getDDNSDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDDNSDomainName[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
}
int8 setDDNSDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDDNSDomainName[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDDNSHostNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char host[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;

	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);
	
	tcapi_get(nodeName, "MYHOST", host);
	return getTheValue(value, type, deviceNode, host);
}
int8 setDDNSHostNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char host[256] = {'\0'};
	uint8 ddnsNodeIndex;
	uint32 wanNodeIndex,pvcNodeEntry;
	int ret;
	uint8 act;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;	
#endif	
	ddnsNodeIndex = wanNodeIndex*PVC_ENTRY_NUM+pvcNodeEntry;
	snprintf(nodeName, sizeof(nodeName), "Ddns_Entry%d", ddnsNodeIndex);

	if(strlen((char*)value) > 256){	
		return -1;
	}
	
	tcapi_set(nodeName, "MYHOST", (char*)value);
	
	SetCfgNodeFlag ( "Ddns", ddnsNodeIndex);
	return 0;
}
int8 getDDNSHostNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDDNSHostName[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
}
int8 setDDNSHostNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDDNSHostName[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
int cwmpInitDDNSConfigurationSubTree(DeviceNodePtr deviceNode)
{
	char node_index[4];
	int ret = 0;
			
	/* register this node */
	snprintf(node_index, sizeof(node_index), "%d",  1);
	CTComDDNSConfigurationIndex[0].name = node_index;
	if (RegisterNodeFromTable(deviceNode, CTComDDNSConfigurationIndex) == NULL) {
		tcdbg_printf("%s:error!.\n", __FUNCTION__);
		ret = -1;
	}

	return ret;
}
#endif
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getWANCnt1_IPModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	char ipver[16]={0};

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	
	tcapi_get(nodeName, "IPVERSION", ipver);
	if ( 0 == strcmp(ipver, "IPv4") )
		strcpy(ipver, "1");
	else if ( 0 == strcmp(ipver, "IPv6") )
		strcpy(ipver, "2");
	else if ( 0 == strcmp(ipver, "IPv4/IPv6") )
		strcpy(ipver, "3");
	else
		strcpy(ipver, "1");
	
	return getTheValue(value, type, deviceNode, ipver);
}
int8 setWANCnt1_IPModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	char ipverStr[16]={0};
    char ipverStr_old[16]={0};
    
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint32 ipver = atoi((char*)value);
#else
    uint32 ipver = *((uint32*)value);
#endif

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode )
		return -1;


#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;
	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);


	switch ( ipver )
	{
		case 1:
			strcpy(ipverStr, "IPv4");
			break;
		case 2:
			strcpy(ipverStr, "IPv6");
			break;
		case 3:
			strcpy(ipverStr, "IPv4/IPv6");
 #ifdef TCSUPPORT_CT_PON_CN_JS
			tcapi_get(nodeName, "IPVERSION", ipverStr_old);
			if(0 == strcmp(ipverStr_old, "IPv6") )
			tcapi_set(nodeName, "DsliteEnable", "No");
#endif
			break;
		default:
			return -1;
	}

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	tcapi_set(nodeName, "IPVERSION", ipverStr);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );

	return 0;
}
int8 getWANIPCnt1_IPModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_CTComIPMode[((wanNodeIndex<<3) | pvcNodeEntry)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_IPModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_CTComIPMode[((wanNodeIndex<<3) | pvcNodeEntry)];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
/*
 get wan dslite state: 1->on , 0->off
*/
int get_dslite_state(char *nodeName)
{
	char dslite_enable[8] = {0};
	char serviceList[64] = {0};
	char isp[5] = {0};

	if ( NULL == nodeName )
		return 0;

	if( 0 != tcapi_get(nodeName, "DsliteEnable", dslite_enable) )
		return 0;
	if( 0 != tcapi_get(nodeName, "ServiceList", serviceList) )
		return 0;
	if( 0 != tcapi_get(nodeName, "ISP", isp) )
		return 0;

	if( NULL != strstr(serviceList, "INTERNET")
		&& 0 != strcmp(isp, "3")  /* not BRIDGE_MODE */
		&& 0 == strcmp("Yes", dslite_enable))
		return 1;
	else
		return 0;
}

/*
 check dslite state: 1->up , 0->down
*/
int is_dslite_up(char *nodeName)
{
	char dslite_up[8] = {0};

	if ( NULL == nodeName )
		return 0;

	if( 0 != tcapi_get(nodeName, "DsliteUP", dslite_up) )
		return 0;

	if( 0 == strcmp("Yes", dslite_up))
		return 1;
	else
		return 0;
}

/*
 check ipv6cp state: 1->up , 0->down
*/
int is_ipv6cp_up(int idx)
{
	char ipv6cp_up[12] = {0};
	char state_path[128] = {0};
	FILE *fp = NULL;

	snprintf(state_path, sizeof(state_path) - 1,
		"/var/run/ppp%d/ipv6cp", idx);
	fp = fopen(state_path, "r");
	if ( fp )
	{
		bzero(ipv6cp_up, sizeof(ipv6cp_up));
		fgets(ipv6cp_up, sizeof(ipv6cp_up), fp);
		fclose(fp);
		if ( strstr(ipv6cp_up, "up") )
			return 1;
	}

	return 0;
}
#endif

int8 getWANCnt1_IPv6ConStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	char ipver[16] = {0}, ip6addr[64] = {0}, ip6gw[64] = {0};
	char connST[12] = {0}, connSTV4[12] = {0}, wanmode[20] = {0};
	char ip6state[20] = "Unconfigured";
	int wan_bridge = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif		
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	tcapi_get(nodeName, "IPVERSION", ipver);

	if ( 0 == tcapi_get(nodeName, "WanMode", wanmode)
		&& 0 == strcmp(wanmode, "Bridge") )
		wan_bridge = 1;

	if ( 0 == strcmp(ipver, "IPv6")
		|| 0 == strcmp(ipver, "IPv4/IPv6") )
	{
		sprintf(nodeName, "WanInfo_Entry%d", ((wanNodeIndex<<3)|pvcNodeEntry));
		tcapi_get(nodeName, "Status6", connST);
		tcapi_get(nodeName, "Status", connSTV4);
		if ( 0 == strcmp(connST, "up")
			|| (1 == wan_bridge && 0 == strcmp(connSTV4, "up")) )
		{
			strcpy(ip6state, "Connected");
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
			if ( 0 == wan_bridge )
			{
				sprintf(nodeName, "WanInfo_Entry%d"
					, ((wanNodeIndex<<3)|pvcNodeEntry));
				if ( 0 != tcapi_get(nodeName, "IP6", ip6addr) )
					ip6addr[0] = 0;
				if ( 0 != tcapi_get(nodeName, "GateWay6", ip6gw) )
					ip6gw[0] = 0;
				if ( 0 == ip6addr[0]
					|| 0 == ip6gw[0] )
					strcpy(ip6state, "Connecting");

				if ( 1 == get_dslite_state(nodeName) )
				{
					if ( 1 != is_dslite_up(nodeName) )
						strcpy(ip6state, "Connecting");
				}
			}
#endif
		}
		else
			strcpy(ip6state, "Disconnected");
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
		if ( 1 == wan_bridge )
		{
			if ( checkBrigePPPState(((wanNodeIndex<<3)|pvcNodeEntry)) )
			strcpy(ip6state, "Connected");
		else
			strcpy(ip6state, "Disconnected");
	}
#endif
	}
	
	return getTheValue(value, type, deviceNode, ip6state);
}
int8 getWANIPCnt1_IPv6ConStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif		
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_CTIPv6ConStatus[((wanNodeIndex<<3) | pvcNodeEntry)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_IPv6ConStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif		
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_CTIPv6ConStatus[((wanNodeIndex<<3) | pvcNodeEntry)];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_MulticastVlanValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	char vid[16]={0};

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	
	tcapi_get(nodeName, "MulticastVID", vid);
	if ( '\0' == vid[0] )
		strcpy(vid, "-1");
	
	return getTheValue(value, type, deviceNode, vid);
}
int8 setWANCnt1_MulticastVlanValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	char vidStr[16]={0};
	int32 vid = *((int32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode )
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	if ( -1 == vid )
		strcpy(vidStr, "");
	else if ( vid > 0 && vid < 4095 )
		sprintf(vidStr, "%d", vid);
	else
		return -1;

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	tcapi_set(nodeName, "MulticastVID", vidStr);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );

	return 0;
}
int8 getWANIPCnt1_MulticastVlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_CTMulticastVlan[((wanNodeIndex<<3) | pvcNodeEntry)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCnt1_MulticastVlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if ( wanNodeIndex >= MAX_WAN_NODE
		|| pvcNodeEntry >= MAX_WAN_NODEL2 )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_CTMulticastVlan[((wanNodeIndex<<3) | pvcNodeEntry)];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption60EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char enableValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name);

	if(option60Index < 1 || option60Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option60Enable%d", option60Index);
	
	tcapi_get(nodeName, attrName, enableValue);

	if(strcmp(enableValue, "Yes") == 0){
		sprintf(enableValue, "%s", "1");
	}
	else{
		sprintf(enableValue, "%s", "0");
	}
	
	return getTheValue(value, type, deviceNode, enableValue);
}
int8 setDhcpOption60EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char tempValue[10] = {0};
	uint8 enableValue = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name) ;

	if(option60Index < 1 || option60Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option60Enable%d", option60Index);

	ret = tcapi_get(nodeName, "Active", tempValue);
	if(ret){
		return -1;
	}

	enableValue = *((uint8*)value);
	if(enableValue == 1){
		tcapi_set(nodeName, attrName, "Yes");
	}
	else if(enableValue == 0){
		tcapi_set(nodeName, attrName, "No");
	}
	else{
		
		return -1;
	}
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}
int8 getDhcpOption60EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption60Enable[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption60EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption60Enable[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption60TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char typeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name);

	if(option60Index < 1 || option60Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option60Type%d", option60Index);
	
	tcapi_get(nodeName, attrName, typeValue);

	if(typeValue[0] == '\0')
		sprintf(typeValue, "%d", 34);
	
	return getTheValue(value, type, deviceNode, typeValue);


}
int8 setDhcpOption60TypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char typeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name);

	if(option60Index < 1 || option60Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option60Type%d", option60Index);

	ret = tcapi_get(nodeName, "Active", typeValue);
	if(ret){
		return -1;
	}

	sprintf(typeValue, "%d", *((uint32*)value));

	tcapi_set(nodeName, attrName, typeValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}
int8 getDhcpOption60TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption60Type[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption60TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption60Type[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption60ValueModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char modeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0;
	char sipProtocol[16] = {0};
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name);

	if(option60Index < 1 || option60Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option60ValueMode%d", option60Index);
	
	if ( tcapi_get(nodeName, attrName, modeValue) < 0 ){
#if defined(TCSUPPORT_CT_PON_CN_JS)
		tcapi_get("VoIPBasic_Common", "SIPProtocol", sipProtocol);
		if(!strcmp(sipProtocol, "IMSSIP"))
			strcpy(modeValue, "2");
		else
			strcpy(modeValue, "3");
	}
#else
		strcpy(modeValue, "0");
	}
#endif
	
	return getTheValue(value, type, deviceNode, modeValue);

}
int8 setDhcpOption60ValueModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	uint32 modeValue = 0;
	char tempValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name);

	if(option60Index < 1 || option60Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option60ValueMode%d", option60Index);

	ret = tcapi_get(nodeName, "Active", tempValue);
	if(ret){
		return -1;
	}

	modeValue = *((uint32*)value);
#if defined(TCSUPPORT_CT_PON_CN_JS)
	if(modeValue < 0 || modeValue > 3)
#else
	if(modeValue < 0 || modeValue > 2)
#endif
		return -1;

	sprintf(tempValue, "%d", modeValue);
	
	tcapi_set(nodeName, attrName, tempValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpOption60ValueModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption60ValueMode[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption60ValueModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption60ValueMode[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption60ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char valueValue[254] = {0};
	char typeValue[10] = {0};
	char modeValue[10] = {0};
	char sipProtocol[16] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0;
	int valueLen = 0, i;
	char valueValue2[254] = {0};
	uint8 *pValue = NULL;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name);

	if(option60Index < 1 || option60Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	
	sprintf(attrName, "Option60Type%d", option60Index);
		
	tcapi_get(nodeName, attrName, typeValue);

	if (!strcmp(typeValue,"34") || typeValue[0] == '\0') {
#if defined(TCSUPPORT_CT_PON_CN_JS)
		memset(attrName, 0, sizeof(attrName));
		sprintf(attrName, "Option60ValueMode%d", option60Index);
		if ( tcapi_get(nodeName, attrName, modeValue) < 0 )
		{
			tcapi_get("VoIPBasic_Common", "SIPProtocol", sipProtocol);
			if(!strcmp(sipProtocol, "IMSSIP"))
				strcpy(modeValue, "2");
			else
				strcpy(modeValue, "3");
		}
		valueLen = make_option60_voip_msg((unsigned char*)valueValue, sizeof(valueValue), atoi(modeValue));
#else
		valueLen = make_option60_voip_msg((unsigned char*)valueValue, sizeof(valueValue));
#endif
		pValue = (unsigned char*)valueValue;
		for (i = 0; i < valueLen; i++) {
			sprintf(valueValue2, "%s%02X", valueValue2, *(pValue+i));
		}
		tcdbg_printf("\r\nvalueValue2 = %s", valueValue2);
		return getTheValue(value, type, deviceNode, valueValue2);
	}
	else{
		sprintf(attrName, "Option60Value%d", option60Index);
		tcapi_get(nodeName, attrName, valueValue);
	}
	
	return getTheValue(value, type, deviceNode, valueValue);


}
int8 setDhcpOption60ValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char valueValue[254] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option60Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option60Index = atoi(deviceNode->parentNode->name);

	if(option60Index < 1 || option60Index > 4)
		return -1;

	if(strlen((char*)(value)) > 253)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option60Value%d", option60Index);

	ret = tcapi_get(nodeName, "Active", valueValue);
	if(ret){
		return -1;
	}

	sprintf(valueValue, "%s", (char*)(value));
	
	tcapi_set(nodeName, attrName, valueValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpOption60ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0; 

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption60Value[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption60ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption60Value[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int cwmpInitDhcpOption60SubTree(DeviceNodePtr deviceNode)
{
	char node_index[4];
	int ret = 0, i = 0;

	/* register this node */
	for(i = 0; i < 4; i++)
	{
		snprintf(node_index, sizeof(node_index), "%d", i + 1);
		cwmpInitDhcpOption60Index[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, cwmpInitDhcpOption60Index) == NULL) {
			tcdbg_printf("%s:error!.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}

	return ret;
}

int8 getDhcpOption125EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char enableValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option125Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option125Index = atoi(deviceNode->parentNode->name);
	
	if(option125Index < 1 || option125Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option125Enable%d", option125Index);
	
	tcapi_get(nodeName, attrName, enableValue);

	if(strcmp(enableValue, "Yes") == 0){
		sprintf(enableValue, "%s", "1");
	}
	else{
		sprintf(enableValue, "%s", "0");
	}
	
	return getTheValue(value, type, deviceNode, enableValue);

}
int8 setDhcpOption125EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char tempValue[10] = {0};
	uint8 enableValue = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option125Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option125Index = atoi(deviceNode->parentNode->name);

	if(option125Index < 1 || option125Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option125Enable%d", option125Index);

	ret = tcapi_get(nodeName, "Active", tempValue);
	if(ret){
		return -1;
	}

	enableValue = *((uint8*)value);
	if(enableValue == 1){
		tcapi_set(nodeName, attrName, "Yes");
	}
	else if(enableValue == 0){
		tcapi_set(nodeName, attrName, "No");
	}
	else{
		
		return -1;
	}

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpOption125EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption125Enable[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption125EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption125Enable[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption125SubCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char subCodeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option125Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option125Index = atoi(deviceNode->parentNode->name);

	if(option125Index < 1 || option125Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option125SubCode%d", option125Index);
	
	if ( tcapi_get(nodeName, attrName, subCodeValue) < 0 )
		strcpy(subCodeValue, "1");
	
	return getTheValue(value, type, deviceNode, subCodeValue);


}
int8 setDhcpOption125SubCodeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char subCodeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option125Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option125Index = atoi(deviceNode->parentNode->name);

	if(option125Index < 1 || option125Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option125SubCode%d", option125Index);

	ret = tcapi_get(nodeName, "Active", subCodeValue);
	if(ret){
		return -1;
	}

	sprintf(subCodeValue, "%d", *((uint32*)value));
	
	tcapi_set(nodeName, attrName, subCodeValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpOption125SubCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption125SubCode[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption125SubCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption125SubCode[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption125SubDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char subDataValue[251] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option125Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option125Index = atoi(deviceNode->parentNode->name);
	
	if(option125Index < 1 || option125Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option125SubData%d", option125Index);
	
	tcapi_get(nodeName, attrName, subDataValue);
	
	return getTheValue(value, type, deviceNode, subDataValue);


}
int8 setDhcpOption125SubDataValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char subDataValue[251] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option125Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option125Index = atoi(deviceNode->parentNode->name);

	if(option125Index < 1 || option125Index > 4)
		return -1;

	if(strlen((char*)(value)) > 250)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option125SubData%d", option125Index);

	ret = tcapi_get(nodeName, "Active", subDataValue);
	if(ret){
		return -1;
	}

	sprintf(subDataValue, "%s", (char*)(value));
	
	tcapi_set(nodeName, attrName, subDataValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpOption125SubDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption125SubData[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption125SubDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption125SubData[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption125ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char valueValue[255] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option125Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option125Index = atoi(deviceNode->parentNode->name);

	if(option125Index < 1 || option125Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option125Value%d", option125Index);
	
	tcapi_get(nodeName, attrName, valueValue);
	
	#ifdef TCSUPPORT_CT_PON_CN_JS
		if (tcapi_get(nodeName, attrName, valueValue) < 0)
			strcpy(valueValue, "TELCOMITMSDHCPAA");
	#endif
	
	return getTheValue(value, type, deviceNode, valueValue);


}
int8 setDhcpOption125ValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char *p_Value = NULL;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option125Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option125Index) )
		return -1;

	p_Value = (char *)value;
	if ( strlen(p_Value) > 250 )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option125Value%d", option125Index);

	tcapi_set(nodeName, attrName, p_Value);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );
	return 0;

}
int8 getDhcpOption125ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpOption125Value[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption125ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpOption125Value[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 ip_ppp_dhcpoption_node_check(DeviceNode*  deviceNode, uint32 *pvcIndex, uint32* entryIndex, uint8 *optionIndex)
{
	uint32 pvcidx = 0, entryidx = 0;
	uint8 optionidx = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode
		|| NULL == pvcIndex
		|| NULL == entryIndex
		|| NULL == optionIndex )
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcidx = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	entryidx = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcidx, &entryidx);
#else
	pvcidx = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	entryidx = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	optionidx = atoi(deviceNode->parentNode->name) ;

	if ( pvcidx >= MAX_WAN_NODE
		|| entryidx >= MAX_WAN_NODEL2
		|| optionidx > 4 )
		return -1;

	*pvcIndex = pvcidx;
	*entryIndex = entryidx;
	*optionIndex = optionidx;

	return 0;
}

int8 getDhcpOption125TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_typeValue[10] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option125Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option125Index) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option125Type%d", option125Index);

	if ( tcapi_get(nodeName, attrName, s_typeValue) < 0 )
		strcpy(s_typeValue, "2");

	return getTheValue(value, type, deviceNode, s_typeValue);
}
int8 setDhcpOption125TypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_typeValue[10] = {0};
	int i_typeValue = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option125Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option125Index) )
		return -1;

	i_typeValue = *((int*)value);
	if ( !(i_typeValue >= 1 && i_typeValue <= 2) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option125Type%d", option125Index);
	snprintf(s_typeValue, sizeof(s_typeValue), "%d", i_typeValue);

	tcapi_set(nodeName, attrName, s_typeValue);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );
	return 0;

}
int8 getDhcpOption125TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option125Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option125Index) )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_DhcpOption125Type[wanNodeIndex<<3|wanEntryIndex][option125Index - 1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption125TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option125Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option125Index) )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_DhcpOption125Type[wanNodeIndex<<3 | wanEntryIndex][option125Index - 1];
	result= SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int cwmpInitDhcpOption125SubTree(DeviceNodePtr deviceNode)
{
	char node_index[4] = {0};
	int ret = 0, i = 0;

	/* register this node */
	for(i = 0; i < 4; i++)
	{
		/* register this node */
		snprintf(node_index, sizeof(node_index), "%d",  i + 1);
		cwmpInitDhcpOption125Index[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, cwmpInitDhcpOption125Index) == NULL) {
			tcdbg_printf("%s:error!.\n", __FUNCTION__);
			ret = -1;
		}
	}

	return ret;

}

int8 getDhcpv6Option17EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char enableValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option17Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option17Index = atoi(deviceNode->parentNode->name);

	if(option17Index < 1 || option17Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option17Enable%d", option17Index);
	
	tcapi_get(nodeName, attrName, enableValue);

	if(strcmp(enableValue, "Yes") == 0){
		sprintf(enableValue, "%s", "1");
	}
	else{
		sprintf(enableValue, "%s", "0");
	}
	
	return getTheValue(value, type, deviceNode, enableValue);

}
int8 setDhcpv6Option17EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char tempValue[10] = {0};
	uint8 enableValue = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option17Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option17Index = atoi(deviceNode->parentNode->name);

	if(option17Index < 1 || option17Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option17Enable%d", option17Index);

	ret = tcapi_get(nodeName, "Active", tempValue);
	if(ret){
		return -1;
	}

	enableValue = *((uint8*)value);
	if(enableValue == 1){
		tcapi_set(nodeName, attrName, "Yes");
	}
	else if(enableValue == 0){
		tcapi_set(nodeName, attrName, "No");
	}
	else{
		
		return -1;
	}


	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpv6Option17EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpv6Option17Enable[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpv6Option17EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpv6Option17Enable[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpv6Option17TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_typeValue[10] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;
	
	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option17Type%d", option17Index);
	
	if ( tcapi_get(nodeName, attrName, s_typeValue) < 0 )
		strcpy(s_typeValue, "2");
	
	return getTheValue(value, type, deviceNode, s_typeValue);
}
int8 setDhcpv6Option17TypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_typeValue[10] = {0};
	int i_typeValue = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;
	
	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	
	i_typeValue = *((int*)value);
	if ( !(i_typeValue >= 1 && i_typeValue <= 2) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option17Type%d", option17Index);
	snprintf(s_typeValue, sizeof(s_typeValue), "%d", i_typeValue);

	tcapi_set(nodeName, attrName, s_typeValue);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );
	return 0;

}
int8 getDhcpv6Option17TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpv6Option17Type[wanNodeIndex<<3|wanEntryIndex][option17Index - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpv6Option17TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpv6Option17Type[wanNodeIndex<<3 | wanEntryIndex][option17Index - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpv6Option17ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char valueValue[254] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option17Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option17Index = atoi(deviceNode->parentNode->name);

	if(option17Index < 1 || option17Index > 4)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option17Value%d", option17Index);
	
	tcapi_get(nodeName, attrName, valueValue);
	
	return getTheValue(value, type, deviceNode, valueValue);


}
int8 setDhcpv6Option17ValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char valueValue[254] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option17Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option17Index = atoi(deviceNode->parentNode->name) ;

	if(option17Index < 1 || option17Index > 4)
		return -1;
	
	if(strlen((char*)(value)) > 253)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option17Value%d", option17Index);

	ret = tcapi_get(nodeName, "Active", valueValue);
	if(ret){
		return -1;
	}

	sprintf(valueValue, "%s", (char*)(value));
	
	tcapi_set(nodeName, attrName, valueValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpv6Option17ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpv6Option17Value[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpv6Option17ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpv6Option17Value[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption17SubCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_subCodeValue[12] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option17SubCode%d", option17Index);
	
	if ( tcapi_get(nodeName, attrName, s_subCodeValue) < 0 )
		strcpy(s_subCodeValue, "1");
	
	return getTheValue(value, type, deviceNode, s_subCodeValue);
}
int8 setDhcpOption17SubCodeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_subCodeValue[12] = {0};
	int i_subCodeValue = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;
	
	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;

	i_subCodeValue = *((int*)value);
	if ( i_subCodeValue <= 0 )
		return -1;
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option17SubCode%d", option17Index);
	sprintf(s_subCodeValue, "%d", i_subCodeValue);
	
	tcapi_set(nodeName, attrName, s_subCodeValue);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );
	return 0;
	
}
int8 getDhcpOption17SubCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;
	
	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpv6Option17SubCode[wanNodeIndex<<3|wanEntryIndex][option17Index - 1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption17SubCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpv6Option17SubCode[wanNodeIndex<<3 | wanEntryIndex][option17Index - 1];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption17SubDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_subDataValue[251] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;
	
	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option17SubData%d", option17Index);
	
	if ( tcapi_get(nodeName, attrName, s_subDataValue) < 0 )
		s_subDataValue[0] = '\0';

	return getTheValue(value, type, deviceNode, s_subDataValue);
	}
int8 setDhcpOption17SubDataValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char *p_subDataValue = NULL;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	
	p_subDataValue = (char *)value;
	if( strlen(p_subDataValue) > 250 )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option17SubData%d", option17Index);
	
	tcapi_set(nodeName, attrName, p_subDataValue);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );
	return 0;

}
int8 getDhcpOption17SubDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	
	attribute = tcIpConAttr.aWANCnt1_Dhcpv6Option17SubData[wanNodeIndex<<3|wanEntryIndex][option17Index - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption17SubDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option17Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option17Index) )
		return -1;
	
	attribute = &tcIpConAttr.aWANCnt1_Dhcpv6Option17SubData[wanNodeIndex<<3 | wanEntryIndex][option17Index - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}


int cwmpInitDhcpv6Option17SubTree(DeviceNodePtr deviceNode)
{
	char node_index[4] = {0};
	int ret = 0, i = 0;

	for(i = 0; i < 4; i++)
	{
		/* register this node */
		snprintf(node_index, sizeof(node_index), "%d",  i + 1);
		cwmpInitDhcpv6Option17Index[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, cwmpInitDhcpv6Option17Index) == NULL) {
			tcdbg_printf("%s:error!.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}
	
	return ret;

}

int8 getDhcpv6Option16EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char enableValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option16Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option16Index = atoi(deviceNode->parentNode->name);

	if(option16Index < 1 || option16Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option16Enable%d", option16Index);
	
	tcapi_get(nodeName, attrName, enableValue);

	if(strcmp(enableValue, "Yes") == 0){
		sprintf(enableValue, "%s", "1");
	}
	else{
		sprintf(enableValue, "%s", "0");
	}
	
	return getTheValue(value, type, deviceNode, enableValue);

}
int8 setDhcpv6Option16EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char tempValue[10] = {0};
	uint8 enableValue = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option16Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option16Index = atoi(deviceNode->parentNode->name);

	if(option16Index < 1 || option16Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option16Enable%d", option16Index);

	ret = tcapi_get(nodeName, "Active", tempValue);
	if(ret){
		return -1;
	}

	enableValue = *((uint8*)value);
	if(enableValue == 1){
		tcapi_set(nodeName, attrName, "Yes");
	}
	else if(enableValue == 0){
		tcapi_set(nodeName, attrName, "No");
	}
	else{
		
		return -1;
	}

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpv6Option16EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpv6Option16Enable[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpv6Option16EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpv6Option16Enable[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpv6Option16ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char valueValue[251] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option16Index = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option16Index = atoi(deviceNode->parentNode->name);

	if(option16Index < 1 || option16Index > 4)
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option16Value%d", option16Index);
	
	tcapi_get(nodeName, attrName, valueValue);
	
	return getTheValue(value, type, deviceNode, valueValue);


}
int8 setDhcpv6Option16ValueValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char valueValue[251] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 option16Index = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	option16Index = atoi(deviceNode->parentNode->name);

	if(option16Index < 1 || option16Index > 4)
		return -1;

	if(strlen((char*)(value)) > 250)
		return -1;
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	sprintf(attrName, "Option16Value%d", option16Index);

	ret = tcapi_get(nodeName, "Active", valueValue);
	if(ret){
		return -1;
	}

	sprintf(valueValue, "%s", (char*)(value));
	
	tcapi_set(nodeName, attrName, valueValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getDhcpv6Option16ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDhcpv6Option16Value[wanNodeIndex<<3|pvcNodeEntry][instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpv6Option16ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	int instance = 0;

	instance = atoi(deviceNode->parentNode->name);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDhcpv6Option16Value[wanNodeIndex<<3 | pvcNodeEntry][instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption16TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_typeValue[10] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option16Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option16Index) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option16Type%d", option16Index);

	if ( tcapi_get(nodeName, attrName, s_typeValue) < 0 )
		strcpy(s_typeValue, "34");

	return getTheValue(value, type, deviceNode, s_typeValue);
}
int8 setDhcpOption16TypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_typeValue[10] = {0};
	int i_typeValue = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option125Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option125Index) )
		return -1;

	i_typeValue = *((int*)value);
	if ( !(i_typeValue >= 1 && i_typeValue <= 255) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option16Type%d", option125Index);
	snprintf(s_typeValue, sizeof(s_typeValue), "%d", i_typeValue);

	tcapi_set(nodeName, attrName, s_typeValue);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );
	return 0;

}
int8 getDhcpOption16TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option16Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option16Index) )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_Dhcpv6Option16Type[wanNodeIndex<<3 | wanEntryIndex][option16Index - 1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption16TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option16Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option16Index) )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_Dhcpv6Option16Type[wanNodeIndex<<3 | wanEntryIndex][option16Index - 1];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getDhcpOption16ValueModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_valueMode[10] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option16Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option16Index) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option16ValueMode%d", option16Index);

	if ( tcapi_get(nodeName, attrName, s_valueMode) < 0 )
		strcpy(s_valueMode, "0");

	return getTheValue(value, type, deviceNode, s_valueMode);
}
int8 setDhcpOption16ValueModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char attrName[34] = {0};
	char s_valueMode[10] = {0};
	int i_valueMode = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option125Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option125Index) )
		return -1;

	i_valueMode = *((int*)value);
	if ( !(i_valueMode >= 0 && i_valueMode <= 2) )
		return -1;

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	snprintf(attrName, sizeof(attrName), "Option16ValueMode%d", option125Index);
	snprintf(s_valueMode, sizeof(s_valueMode), "%d", i_valueMode);

	tcapi_set(nodeName, attrName, s_valueMode);
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );
	return 0;

}
int8 getDhcpOption16ValueModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option16Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option16Index) )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_Dhcpv6Option16ValueMode[wanNodeIndex<<3 | wanEntryIndex][option16Index - 1];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDhcpOption16ValueModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	uint8 option16Index = 0;

	if ( 0 != ip_ppp_dhcpoption_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex, &option16Index) )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_Dhcpv6Option16ValueMode[wanNodeIndex<<3 | wanEntryIndex][option16Index - 1];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}


int cwmpInitDhcpv6Option16SubTree(DeviceNodePtr deviceNode)
{
	char node_index[4] = {0};
	int ret = 0, i = 0;

	/* register this node */
	for(i = 0; i < 4; i++)
	{
		/* register this node */
		snprintf(node_index, sizeof(node_index), "%d", i + 1);
		cwmpInitDhcpv6Option16Index[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, cwmpInitDhcpv6Option16Index) == NULL) {
			tcdbg_printf("%s:error!.\n", __FUNCTION__);
			ret = -1;
		}
	}
	return ret;

}

int8 getIPv6AliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char aliasValue[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "IPv6AddrAlias", aliasValue);
	
	return getTheValue(value, type, deviceNode, aliasValue);

}

int8 getIPv6AliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6Alias[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6AliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6Alias[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6IPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char ipAddrValue[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "WanInfo_Entry%d", (pvcNodeIndex<<3 | pvcNodeEntry));

	tcapi_get(nodeName, "IP6", ipAddrValue);
	
	return getTheValue(value, type, deviceNode, ipAddrValue);
}



int8 setIPv6IPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char originValue[33] = {0};
	char ipAddrValue[65] = {0};
	unsigned char tmp[100] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 64)
		return -1;

	if(inet_pton6((const char*)value, tmp)!= 1)
	{
		tcdbg_printf("IPv6 Address Error.\n");
		return -1;	
	}

	ret = tcapi_get(nodeName, "DHCPv6", originValue);
	if(ret){
		return -1;
	}

	if(strcmp(originValue, "N/A") != 0)	//static only
		return -1;

	sprintf(ipAddrValue, "%s", (char*)value);

	tcapi_set(nodeName, "IPADDR6", ipAddrValue);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;


}
int8 getIPv6IPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6IPAddress[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6IPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6IPAddress[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6DNSIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char dnsIpAddrValue[129] = {0};
	char dns1[65] = {0};
	char dns2[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "WanInfo_Entry%d", (pvcNodeIndex<<3 | pvcNodeEntry));

	tcapi_get(nodeName, "DNS6", dns1);
	tcapi_get(nodeName, "SecDNS6", dns2);
	
	sprintf(dnsIpAddrValue, "%s,%s", dns1, dns2);
	
	return getTheValue(value, type, deviceNode, dnsIpAddrValue);

}

int8 setIPv6DNSIPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char originValue[32] = {0};
	char dnsIpAddrValue[129] = {0};
	char dns1[129] = {0};
	char dns2[129] = {0};
	unsigned char tmp[100] = {0};
	char *tp = NULL;
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 128)
		return -1;

	ret = tcapi_get(nodeName, "DHCPv6", originValue);
	if(ret){
		return -1;
	}

	if(strcmp(originValue, "N/A") != 0)	//static only
		return -1;

	sprintf(dnsIpAddrValue, "%s", (char*)value);

	tp = strstr(dnsIpAddrValue, ",");
	if(tp == NULL)
	{
		if(strlen(dnsIpAddrValue) > 64)
			return -1;
		
		if(inet_pton6((const char*)dnsIpAddrValue, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}
			
		tcapi_set(nodeName, "DNS6", dnsIpAddrValue);
	}
	else
	{
		if(strstr((tp + 1), ",") != NULL)
			return -1;
		
		strncpy(dns1, dnsIpAddrValue, (tp - dnsIpAddrValue));
		sprintf(dns2, "%s", (tp + 1));

		if(strlen(dns1) > 64 || strlen(dns2) > 64)
			return -1;
		
		if(inet_pton6((const char*)dns1, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}	

		if(inet_pton6((const char*)dns2, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}	

		tcapi_set(nodeName, "DNS6", dns1);
		tcapi_set(nodeName, "SecDNS6", dns2);
	}
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getIPv6DNSIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6DNSIPAddress[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6DNSIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6DNSIPAddress[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6OriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char dhcpv6Value[10] = {0};
	char originValue[33] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DHCPv6", dhcpv6Value);

	if(strcmp(dhcpv6Value, "Yes") == 0)
		sprintf(originValue, "%s", "DHCPv6");
	else if(strcmp(dhcpv6Value, "No") == 0)
		sprintf(originValue, "%s", "AutoConfigured");
	else
		sprintf(originValue, "%s", "Static");
	
	return getTheValue(value, type, deviceNode, originValue);

}

int8 setIPv6OriginValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char modeValue[20] = {0};
	char ispValue[10] = {0};
	char originValue[33] = {0};
	char linkModeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 ret = 0, ipv4Mode = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 32)
		return -1;
	
	ret = tcapi_get(nodeName, "IPVERSION", modeValue);
	if(ret){
		return -1;
	}

	if(strcmp(modeValue, "IPv4") == 0 || strcmp(modeValue, "IPv4/IPv6") == 0)
	{
		ipv4Mode = 1;
	}

	if(ipv4Mode == 1)
	{
		ret = tcapi_get(nodeName, "ISP", ispValue);
		if(ret){
			return -1;
		}
	}

	ret = tcapi_get(nodeName, "LinkMode", linkModeValue);
	if(ret){
		return -1;
	}

	sprintf(originValue, "%s", (char*)(value));

	if(strcmp(originValue, "AutoConfigured") == 0 && (ipv4Mode != 1 || strcmp(ispValue, "1") != 0))
	{
		tcapi_set(nodeName, "DHCPv6", "No");
		if(strcmp(linkModeValue, "linkIP") == 0)
			tcapi_set(nodeName, "ISP", "0");
		else
			tcapi_set(nodeName, "ISP", "2");
	}
	else if(strcmp(originValue, "DHCPv6") == 0 && (ipv4Mode != 1 || strcmp(ispValue, "1") != 0))
	{
		tcapi_set(nodeName, "DHCPv6", "Yes");
		if(strcmp(linkModeValue, "linkIP") == 0)
			tcapi_set(nodeName, "ISP", "0");
		else
			tcapi_set(nodeName, "ISP", "2");

	}
	else if(strcmp(originValue, "Static") == 0 && strcmp(linkModeValue, "linkIP") == 0 && (ipv4Mode != 1 || strcmp(ispValue, "1") == 0))
	{
		tcapi_set(nodeName, "DHCPv6", "N/A");
		tcapi_set(nodeName, "ISP", "1");
	}
	else
		return -1;

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}
int8 getIPv6OriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;
 
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6Origin[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6OriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6Origin[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6ChildPrefixBitsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char childPrefixBits[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "ChildPrefixBits", childPrefixBits);

	if(childPrefixBits[0] == '\0')
		sprintf(childPrefixBits, "%s", "::/64"); 

	return getTheValue(value, type, deviceNode, childPrefixBits);

}

int8 setIPv6ChildPrefixBitsValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char childPrefixBits[65] = {0};
	char *tp = NULL;
	char tmp1[100] = {0};
	unsigned char tmp2[100] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 64)
		return -1;

	ret = tcapi_get(nodeName, "Active", childPrefixBits);
	if(ret){
		return -1;
	}

	sprintf(childPrefixBits, "%s", (char*)(value));

	if(strlen(childPrefixBits) > 64)
		return -1;
	
	tp = strstr(childPrefixBits, "/");
	if( tp == NULL)
		return -1;
	else
	{
			sprintf(tmp1, "%s", tp + 1);
			if(atoi(tmp1) > 128 || atoi(tmp1) < 1)
				return -1;
			
			memset(	tmp1, 0, sizeof(tmp1));
			
			strncpy(tmp1, childPrefixBits, tp - childPrefixBits);
			if(inet_pton6((const char*)tmp1, tmp2)!= 1)
				return -1;
	}
	
	tcapi_set(nodeName, "ChildPrefixBits", childPrefixBits);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getIPv6ChildPrefixBitsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6ChildPrefixBits[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6ChildPrefixBitsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6ChildPrefixBits[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6PrefixAliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char aliasValue[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "PDAlias", aliasValue);
	
	return getTheValue(value, type, deviceNode, aliasValue);

}

int8 getIPv6PrefixAliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6PrefixAlias[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6PrefixAliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6PrefixAlias[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixValue[65] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "PDPrefix", prefixValue);

	return getTheValue(value, type, deviceNode, prefixValue);

}

int8 setIPv6PrefixValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixValue[65] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	uint8 ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", prefixValue);
	if(ret){
		return -1;
	}

	sprintf(prefixValue, "%s", (char*)(value));

	if(strlen(prefixValue) > 64)
		return -1;
	
	tcapi_set(nodeName, "PDPrefix", prefixValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6Prefix[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6Prefix[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6PrefixOriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixOriginValue[32] = {0};
	char prefixModeValue[20] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DHCPv6PD", prefixOriginValue);
	tcapi_get(nodeName, "PDOrigin", prefixModeValue);

	if(strcmp(prefixOriginValue, "Yes") == 0)
		sprintf(prefixOriginValue, "%s", "PrefixDelegation");
	else
	{
		if(strcmp(prefixModeValue, "RouterAdv") == 0)
			sprintf(prefixOriginValue, "%s", "RouterAdvertisement");
		else if(strcmp(prefixModeValue, "Static") == 0)
			sprintf(prefixOriginValue, "%s", "Static");
		else
			sprintf(prefixOriginValue, "%s", "None");
	}
	
	return getTheValue(value, type, deviceNode, prefixOriginValue);

}

int8 setIPv6PrefixOriginValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixOriginValue[32] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 32)
		return -1;	

	ret = tcapi_get(nodeName, "Active", prefixOriginValue);
	if(ret){
		return -1;
	}

	sprintf(prefixOriginValue, "%s", (char*)(value));

	if(strcmp(prefixOriginValue, "PrefixDelegation") == 0)
		tcapi_set(nodeName, "DHCPv6PD", "Yes");
	else if(strcmp(prefixOriginValue, "RouterAdvertisement") == 0)
	{
		tcapi_set(nodeName, "DHCPv6PD", "No");
		tcapi_set(nodeName, "PDOrigin", "RouterAdv");
	}
	else if(strcmp(prefixOriginValue, "Static") == 0)
	{
		tcapi_set(nodeName, "DHCPv6PD", "No");
		tcapi_set(nodeName, "PDOrigin", "Static");
	}
	else if(strcmp(prefixOriginValue, "None") == 0)
	{
		tcapi_set(nodeName, "DHCPv6PD", "No");
		tcapi_set(nodeName, "PDOrigin", "None");
	}
	else
		return -1;

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getIPv6PrefixOriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6PrefixOrigin[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6PrefixOriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6PrefixOrigin[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6PrefixChildPrefixBitsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char childPrefixBits[65] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "PDChildPrefixBits", childPrefixBits);

	return getTheValue(value, type, deviceNode, childPrefixBits);

}

int8 setIPv6PrefixChildPrefixBitsValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char childPrefixBits[65] = {0};
	char *tp = NULL;
	char tmp1[100] = {0};
	unsigned char tmp2[100] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry, ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", childPrefixBits);
	if(ret){
		return -1;
	}

	sprintf(childPrefixBits, "%s", (char*)(value));

	if(strlen(childPrefixBits) > 64)
		return -1;
		
	tp = strstr(childPrefixBits, "/");
	if( tp == NULL)
		return -1;
	else
	{
			sprintf(tmp1, "%s", tp + 1);
			if(atoi(tmp1) > 128 || atoi(tmp1) < 1)
				return -1;
			
			memset(	tmp1, 0, sizeof(tmp1));
			
			strncpy(tmp1, childPrefixBits, tp - childPrefixBits);
			if(inet_pton6((const char*)tmp1, tmp2)!= 1)
				return -1;
	}	
	
	tcapi_set(nodeName, "PDChildPrefixBits", childPrefixBits);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getIPv6PrefixChildPrefixBitsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComIPv6PrefixChildPrefixBits[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPv6PrefixChildPrefixBitsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef uint32//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComIPv6PrefixChildPrefixBits[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

#ifdef TCSUPPORT_DSLITE
int8 getCTComDsliteEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char tmpValue[10] = {0};
	char enableValue[10] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DsliteEnable", tmpValue);

	if(strcmp(tmpValue, "Yes") == 0)
		sprintf(enableValue, "%s", "1");
	else
		sprintf(enableValue, "%s", "0");
	
	return getTheValue(value, type, deviceNode, enableValue);

}

int8 setCTComDsliteEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char tmpValue[10] = {0};
	uint8 enableValue = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	int ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", tmpValue);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	enableValue = *((uint8*)value);
	if(enableValue == 1){
		tcapi_set(nodeName, "DsliteEnable", "Yes");
	}
	else if(enableValue == 0){
		tcapi_set(nodeName, "DsliteEnable", "No");
	}
	else{

		return -1;
	}
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getCTComDsliteEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComDsliteEnable[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTComDsliteEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComDsliteEnable[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getCTComAftrModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char modeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DsliteMode", modeValue);

	if(modeValue[0] == '\0')
		sprintf(modeValue, "%d", 0);

	return getTheValue(value, type, deviceNode, modeValue);

}

int8 setCTComAftrModeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char modeValue[10] = {0};
	int tmpValue;
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", modeValue);
	if(ret){
		return -1;
	}

	tmpValue = *(int*)(value);

	if(tmpValue != 0 && tmpValue != 1)
	{
		return -1;

	}

	sprintf(modeValue, "%d", tmpValue);
	
	tcapi_set(nodeName, "DsliteMode", modeValue);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getCTComAftrModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComAftrMode[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTComAftrModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComAftrMode[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getCTComAftrValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char aftrValue[120] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DsliteAddr", aftrValue);

	return getTheValue(value, type, deviceNode, aftrValue);

}

int8 setCTComAftrValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char aftrValue[120] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	
#ifdef uint32//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 100)
		return -1;

	ret = tcapi_get(nodeName, "Active", aftrValue);
	if(ret){
		return -1;
	}

	sprintf(aftrValue, "%s", (char*)value);
	
	tcapi_set(nodeName, "DsliteAddr", aftrValue);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getCTComAftrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_CTComAftr[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTComAftrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_CTComAftr[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif

int8 ip_ppp_node_check(DeviceNode*  deviceNode, uint32 *pvcIndex, uint32* entryIndex)
{
	uint32 pvcidx = 0, entryidx = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode->parentNode
		|| NULL == pvcIndex
		|| NULL == entryIndex)
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcidx = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	entryidx = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcidx, &entryidx);
#else
	pvcidx = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	entryidx = atoi(deviceNode->parentNode->name) - 1;
#endif
	if ( pvcidx >= MAX_WAN_NODE
		|| entryidx >= MAX_WAN_NODEL2 )
		return -1;

	*pvcIndex = pvcidx;
	*entryIndex = entryidx;

	return 0;
}

int8 ip_ppp_IPForwardV4addr_check(char *v4addr)
{
	char start_addr[80] = {0};
	char end_addr[80] = {0};
	struct in_addr in_s_v4addr = {0};
	struct in_addr in_e_v4addr = {0};

	if ( NULL == v4addr
		|| '\0' == v4addr[0])
		return -1;

	if ( NULL == strstr(v4addr, "-") ) // single ip
	{
		if ( strlen(v4addr) > 15 )
			return -1;

		if ( 1 != inet_pton(AF_INET, v4addr, &in_s_v4addr) )
			return -1;
	}
	else // ip range
	{
		sscanf(v4addr, "%[^-]-%s", start_addr, end_addr);

		if ( 1 != inet_pton(AF_INET, start_addr, &in_s_v4addr) )
			return -1;

		if ( 1 != inet_pton(AF_INET, end_addr, &in_e_v4addr) )
			return -1;

		if ( in_s_v4addr.s_addr > in_e_v4addr.s_addr )
			return -1;
	}

	return 0;
}

int8 ip_ppp_IPForwardV4_check(char *v4addr, int maxlen)
{
	char *ipforwardVal = NULL;
	char *pos = NULL;
	int addr_len = 0;
	int idx = 0;
	int v4frt_maxlen = 31; // xxx.xxx.xxx.xxx-xxx.xxx.xxx.xxx

	if ( NULL == v4addr
		|| '\0' == v4addr[0])
		return -1;

	addr_len = strlen(v4addr);
	if ( addr_len > maxlen
		|| ',' == v4addr[addr_len - 1])
		return -1;

	ipforwardVal = (char*)os_malloc(addr_len+1);
	if(ipforwardVal == NULL)
	{
		tcdbg_printf("ip_ppp_IPForwardV4_check  os_malloc  fail\n");
		return -1;
	}
	memset(ipforwardVal, 0, addr_len+1);
	strncpy(ipforwardVal, v4addr, addr_len);

	pos = strtok(ipforwardVal, ",");
	while ( pos )
	{
		addr_len = strlen(pos);
		if ( addr_len > v4frt_maxlen ){
			os_free(ipforwardVal);
			return -1;
		}
		if ( 0 != ip_ppp_IPForwardV4addr_check(pos) ){
			os_free(ipforwardVal);
			return -1;
		}
		pos = strtok(NULL, ",");
	}
	os_free(ipforwardVal);
	return 0;
}

int8 ip_ppp_IPForwardV6addr_check(char *v6addr)
{
	char start_addr[80] = {0};
	char end_addr[80] = {0};
	struct in6_addr in_s_v6addr = {0};
	struct in6_addr in_e_v6addr = {0};

	if ( NULL == v6addr
		|| '\0' == v6addr[0])
		return -1;

	if ( NULL == strstr(v6addr, "-") ) // single ip
	{
		if ( strlen(v6addr) > 39 )
			return -1;

		if ( 1 != inet_pton(AF_INET6, v6addr, &in_s_v6addr) )
			return -1;
	}
	else // ip range
	{
		sscanf(v6addr, "%[^-]-%s", start_addr, end_addr);
		if ( 1 != inet_pton(AF_INET6, start_addr, &in_s_v6addr) )
			return -1;

		if ( 1 != inet_pton(AF_INET6, end_addr, &in_e_v6addr) )
			return -1;

		if ( memcmp(&in_s_v6addr, &in_e_v6addr, sizeof(struct in6_addr)) > 0 )
			return -1;
	}

	return 0;

}

int8 ip_ppp_IPForwardV6_check(char *v6addr, int maxlen)
{
	char *ipforwardVal = NULL;
	char *pos = NULL;
	int addr_len = 0;
	int idx = 0;
	int v6frt_maxlen = 79;

	if ( NULL == v6addr
		|| '\0' == v6addr[0])
		return -1;

	addr_len = strlen(v6addr);
	if ( addr_len > maxlen
		|| ',' == v6addr[addr_len - 1])
		return -1;

	ipforwardVal = (char*)os_malloc(addr_len+1);
	if(ipforwardVal == NULL)
	{
		tcdbg_printf("ip_ppp_IPForwardV6_check	os_malloc  fail\n");
		return -1;
	}
	memset(ipforwardVal, 0, addr_len+1);
	strncpy(ipforwardVal, v6addr, addr_len);

	pos = strtok(ipforwardVal, ",");
	while ( pos )
	{
		addr_len = strlen(pos);
		if ( addr_len > v6frt_maxlen ){
			os_free(ipforwardVal);
			return -1;
		}
		if ( 0 != ip_ppp_IPForwardV6addr_check(pos) ){
			os_free(ipforwardVal);
			return -1;
		}
		pos = strtok(NULL, ",");
	}
	os_free(ipforwardVal);

	return 0;
}

int8 ip_ppp_IPForwardV4V6_check(char *v4v6addr, int maxlen)
{
	char *ipforwardVal = NULL;
	char *pos = NULL;
	int addr_len = 0;
	int idx = 0;
	int v6frt_maxlen = 79;

	if ( NULL == v4v6addr
		|| '\0' == v4v6addr[0])
		return -1;

	addr_len = strlen(v4v6addr);
	if ( addr_len > maxlen
		|| ',' == v4v6addr[addr_len - 1])
		return -1;

	ipforwardVal = (char*)os_malloc(addr_len+1);
	if(ipforwardVal == NULL)
	{
		tcdbg_printf("ip_ppp_IPForwardV4V6_check	os_malloc  fail\n");
		return -1;
	}
	memset(ipforwardVal, 0, addr_len+1);
	strncpy(ipforwardVal, v4v6addr, addr_len);

	pos = strtok(ipforwardVal, ",");
	while ( pos )
	{
		addr_len = strlen(pos);
		if ( addr_len > v6frt_maxlen ){
			os_free(ipforwardVal);
			return -1;
		}
		if(NULL != strstr(pos, "."))
		{
			if ( 0 != ip_ppp_IPForwardV4addr_check(pos) ){
				os_free(ipforwardVal);
				return -1;
		}
		}
		else{	
			if ( 0 != ip_ppp_IPForwardV6addr_check(pos) ){
				os_free(ipforwardVal);
			return -1;
			}
		}
		pos = strtok(NULL, ",");
	}
	os_free(ipforwardVal);

	return 0;
}


int8 getIPForwardListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	char ipforwardVal[MAXLEN_TCAPI_MSG]={0};
	char IPForwardListTemp[16] = {0};
	int i = 0;
	int total = 80*1024/MAXLEN_TCAPI_MSG + 1;
	int len = 0;

	if ( 0 != ip_ppp_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex) )
		return -1;

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	
	if ( tcapi_get(nodeName, "IPForwardList0", ipforwardVal) < 0 ){
		ipforwardVal[0] = '\0';
	}

	if(strlen(ipforwardVal) < MAXLEN_TCAPI_MSG - 1){
	return getTheValue(value, type, deviceNode, ipforwardVal);
	}
	else{
		*value = (char*)os_malloc(strlen(ipforwardVal)+1);
		if(*value == NULL)
			return -1;
		strncpy(*value, ipforwardVal, MAXLEN_TCAPI_MSG - 1);
		*type = deviceNode->type;
	}

	for(i=1; i<total; i++){
		sprintf(IPForwardListTemp, "IPForwardList%d", i);

		if ( tcapi_get(nodeName, IPForwardListTemp, ipforwardVal) < 0 ){
			ipforwardVal[0] = '\0';
		}
		len = strlen(ipforwardVal);
		if(len == 0){
			break;
		}
		*value = os_str_realloc(*value, strlen(*value)+len, strlen(*value));
		if(*value == NULL)
			return -1;
		strcat(*value,  ipforwardVal);
		if(len < MAXLEN_TCAPI_MSG - 1){
			break;	
		}
	}

	len = strlen(*value);
	(*value)[len] = '\0';
	
	return 0;
}
int8 setIPForwardListValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;
	char ipver[16]={0};
	int maxlen = 81*MAXLEN_TCAPI_MSG;
	int i = 0;
	int len = 0;
	int lastLen = 0;
	char IPForwardListTemp[16] = {0};
	char ipforwardValTemp[MAXLEN_TCAPI_MSG] = {0};
	char *ipforwardVal = (char *)value;

	if ( NULL == ipforwardVal
		|| 0 != ip_ppp_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex) )
		return -1;

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, wanEntryIndex);
	if ( '\0' != ipforwardVal[0] )
	{
		if ( tcapi_get(nodeName, "IPVERSION", ipver) < 0 )
			strcpy(ipver, "IPv4");

		if ( 0 == strcmp(ipver, "IPv4") )
		{
			if ( 0 != ip_ppp_IPForwardV4_check(ipforwardVal, maxlen) )
				return -1;
		}
		else if ( 0 == strcmp(ipver, "IPv6") )
		{
			if ( 0 != ip_ppp_IPForwardV6_check(ipforwardVal, maxlen) )
				return -1;
		}
		else if ( 0 == strcmp(ipver, "IPv4/IPv6") )
		{
 			if ( 0 != ip_ppp_IPForwardV4V6_check(ipforwardVal, maxlen) )
				return -1;
		}
		else
			return -1;
	}
	
	len = strlen(ipforwardVal);
	lastLen = len;
	while(lastLen > 0){
		memset(ipforwardValTemp, 0 , MAXLEN_TCAPI_MSG);
		if(lastLen > MAXLEN_TCAPI_MSG-1)
		{
			strncpy(ipforwardValTemp, ipforwardVal+(len-lastLen), MAXLEN_TCAPI_MSG-1);
			lastLen -= MAXLEN_TCAPI_MSG-1;
		}
		else
		{
			strncpy(ipforwardValTemp, ipforwardVal+(len-lastLen), lastLen);
			lastLen = 0;
		}
		
		sprintf(IPForwardListTemp, "IPForwardList%d", i++);
		if(0 != tcapi_set(nodeName, IPForwardListTemp, ipforwardValTemp))
			return -1;
	}
	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|wanEntryIndex) + COMMIT_THIRD_ALIAS );

	return 0;
}
int8 getIPForwardListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;

	if ( 0 != ip_ppp_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex) )
		return -1;

	attribute = tcIpConAttr.aWANCnt1_IPForwardList[((wanNodeIndex<<3) | wanEntryIndex)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPForwardListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0, wanEntryIndex = 0;

	if ( 0 != ip_ppp_node_check(deviceNode, &wanNodeIndex, &wanEntryIndex) )
		return -1;

	attribute = &tcIpConAttr.aWANCnt1_IPForwardList[((wanNodeIndex<<3) | wanEntryIndex)];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}


int8 getXIPv6IPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char value_buf[64] = {'\0'};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	
	snprintf(nodeName, sizeof(nodeName), "WanInfo_Entry%d", (pvcNodeIndex<<3 | pvcNodeEntry));

	tcapi_get(nodeName, "IP6", value_buf);
	
	return getTheValue(value, type, deviceNode, value_buf);
}

int8 setXIPv6IPAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	unsigned char tmp[100] = {0};
	char value_buf[64] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 40)
		return -1;

	if(inet_pton6((const char*)value, tmp)!= 1)
	{
		tcdbg_printf("IPv6 Address Error.\n");
		return -1;	
	}

	ret = tcapi_get(nodeName, "DHCPv6", value_buf);
	if(ret){
		return -1;
	}

	if(strcmp(value_buf, "N/A") != 0)	//static only
		return -1;

	sprintf(value_buf, "%s", (char*)value);

	tcapi_set(nodeName, "IPADDR6", value_buf);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getXIPv6IPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6IPAddress[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setXIPv6IPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6IPAddress[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6IPAddressAliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char aliasValue[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "IPv6AddrAlias", aliasValue);
	
	return getTheValue(value, type, deviceNode, aliasValue);

}

int8 setIPv6IPAddressAliasValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	unsigned char tmp[100] = {0};
	char value_buf[64] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	
	if(strlen((char*)value) > 40)
		return -1;

	sprintf(value_buf, "%s", (char*)value);

	tcapi_set(nodeName, "IPv6AddrAlias", value_buf);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getIPv6IPAddressAliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6IPAddressAlias[pvcNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6IPAddressAliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 pvcNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6IPAddressAlias[pvcNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPAddressOriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char dhcpv6Value[10] = {0};
	char originValue[33] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DHCPv6", dhcpv6Value);

	if(strcmp(dhcpv6Value, "Yes") == 0)
		sprintf(originValue, "%s", "DHCPv6");
	else if(strcmp(dhcpv6Value, "No") == 0)
		sprintf(originValue, "%s", "AutoConfigured");
	else
		sprintf(originValue, "%s", "Static");
	
	return getTheValue(value, type, deviceNode, originValue);

}

int8 setIPAddressOriginValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char modeValue[20] = {0};
	char wanMode[20] = {0};
	char ispValue[10] = {0};
	char originValue[33] = {0};
	char linkModeValue[10] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 ret = 0, ipv4Mode = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 32)
		return -1;
	
	ret = tcapi_get(nodeName, "IPVERSION", modeValue);
	if(ret){
		return -1;
	}

	if(strcmp(modeValue, "IPv4") == 0 || strcmp(modeValue, "IPv4/IPv6") == 0)
	{
		ipv4Mode = 1;
	}

	if(ipv4Mode == 1)
	{
		ret = tcapi_get(nodeName, "ISP", ispValue);
		if(ret){
			return -1;
		}
	}

	ret = tcapi_get(nodeName, "LinkMode", linkModeValue);
	if(ret){
		return -1;
	}

	if ( tcapi_get(nodeName, "WanMode", wanMode) < 0 )
		strcpy(wanMode, "");

	sprintf(originValue, "%s", (char*)(value));

	if(strcmp(originValue, "AutoConfigured") == 0 && (ipv4Mode != 1 || strcmp(ispValue, "1") != 0))
	{
		tcapi_set(nodeName, "DHCPv6", "No");
		if ( 0 != strcmp(wanMode, "Bridge") )
		{
			if(strcmp(linkModeValue, "linkIP") == 0)
				tcapi_set(nodeName, "ISP", "0");
			else
				tcapi_set(nodeName, "ISP", "2");
		}
	}
	else if(strcmp(originValue, "DHCPv6") == 0 && (ipv4Mode != 1 || strcmp(ispValue, "1") != 0))
	{
		tcapi_set(nodeName, "DHCPv6", "Yes");
		if ( 0 != strcmp(wanMode, "Bridge") )
		{
			if(strcmp(linkModeValue, "linkIP") == 0)
				tcapi_set(nodeName, "ISP", "0");
			else
				tcapi_set(nodeName, "ISP", "2");
		}

	}
	else if(strcmp(originValue, "Static") == 0 && strcmp(linkModeValue, "linkIP") == 0 && (ipv4Mode != 1 || strcmp(ispValue, "1") == 0))
	{
		tcapi_set(nodeName, "DHCPv6", "N/A");
		if ( 0 != strcmp(wanMode, "Bridge") )
			tcapi_set(nodeName, "ISP", "1");
	}
	else
		return -1;

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getIPAddressOriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry =0;
 
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	 pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	 pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPAddressOrigin[pvcNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPAddressOriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 pvcNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPAddressOrigin[pvcNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6DNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char dnsIpAddrValue[129] = {0};
	char dns1[65] = {0};
	char dns2[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "WanInfo_Entry%d", (pvcNodeIndex<<3 | pvcNodeEntry));

	tcapi_get(nodeName, "DNS6", dns1);
	tcapi_get(nodeName, "SecDNS6", dns2);

	if ( '\0' != dns1[0] )
	{
		if ( '\0' != dns2[0] )
			sprintf(dnsIpAddrValue, "%s,%s", dns1, dns2);
		else
			strcpy(dnsIpAddrValue, dns1);
	}

	return getTheValue(value, type, deviceNode, dnsIpAddrValue);

}

int8 setIPv6DNSServersValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char originValue[32] = {0};
	char dnsIpAddrValue[129] = {0};
	char dns1[129] = {0};
	char dns2[129] = {0};
	unsigned char tmp[100] = {0};
	char *tp = NULL;
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 128)
		return -1;

	ret = tcapi_get(nodeName, "DHCPv6", originValue);
	if(ret){
		return -1;
	}

	if(strcmp(originValue, "N/A") != 0)	//static only
		return -1;

	sprintf(dnsIpAddrValue, "%s", (char*)value);

	tp = strstr(dnsIpAddrValue, ",");
	if(tp == NULL)
	{
		if(strlen(dnsIpAddrValue) > 64)
			return -1;
		
		if(inet_pton6((const char*)dnsIpAddrValue, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}
			
		tcapi_set(nodeName, "DNS6", dnsIpAddrValue);
	}
	else
	{
		if(strstr((tp + 1), ",") != NULL)
			return -1;
		
		strncpy(dns1, dnsIpAddrValue, (tp - dnsIpAddrValue));
		sprintf(dns2, "%s", (tp + 1));

		if(strlen(dns1) > 64 || strlen(dns2) > 64)
			return -1;
		
		if(inet_pton6((const char*)dns1, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}	

		if(inet_pton6((const char*)dns2, tmp)!= 1)
		{
			tcdbg_printf("IPv6 Address Error.\n");
			return -1;	
		}	

		tcapi_set(nodeName, "DNS6", dns1);
		tcapi_set(nodeName, "SecDNS6", dns2);
	}
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}
int8 getIPv6DNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 pvcNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6DNSServers[pvcNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6DNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6DNSServers[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6PrefixDelegationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char enable[10] = {'\0'};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DHCPv6PD", enable);

	if(!strcmp(enable, "Yes")){
		strcpy(enable, "1");
	}
	else{
		strcpy(enable, "0");
	}
	
	return getTheValue(value, type, deviceNode, enable);
}

int8 setIPv6PrefixDelegationValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char node_val[24] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	uint8 enableValue = 0;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "ServiceList", node_val);
	if(ret){
		return -1;
	}
	
	enableValue = *((uint8*)value);

	if((enableValue == 1) && (!strcmp(node_val, "TR069") || !strcmp(node_val, "VOICE")))
		return -1;

	tcapi_get(nodeName, "DHCPv6PD", node_val);
	
	if(enableValue == 1 ){
		tcapi_set(nodeName, "DHCPv6PD", "Yes");
		tcapi_set(nodeName, "PDEnable", "Yes");
	}
	else if(enableValue == 0){
		tcapi_set(nodeName, "DHCPv6PD", "No");
		tcapi_set(nodeName, "PDEnable", "No");
	}
	else{
		return -1;
	}	
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getIPv6PrefixDelegationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6PrefixDelegation[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6PrefixDelegationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6PrefixDelegation[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getCTComIPv6PrefixAliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char aliasValue[65] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "PDAlias", aliasValue);
	
	return getTheValue(value, type, deviceNode, aliasValue);

}

int8 setCTComIPv6PrefixAliasValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	unsigned char tmp[100] = {0};
	char value_buf[64] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	
	if(strlen((char*)value) > 40)
		return -1;

	sprintf(value_buf, "%s", (char*)value);

	tcapi_set(nodeName, "PDAlias", value_buf);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getCTComIPv6PrefixAliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 pvcNodeIndex = 0, pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6PrefixAlias[pvcNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTComIPv6PrefixAliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 pvcNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6PrefixAlias[pvcNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getCTComIPv6PrefixOriginValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixOriginValue[32] = {0};
	char prefixModeValue[20] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "DHCPv6PD", prefixOriginValue);
	tcapi_get(nodeName, "PDOrigin", prefixModeValue);

	if(strcmp(prefixOriginValue, "Yes") == 0)
		sprintf(prefixOriginValue, "%s", "PrefixDelegation");
	else
	{
		if(strcmp(prefixModeValue, "RouterAdv") == 0)
			sprintf(prefixOriginValue, "%s", "RouterAdvertisement");
		else if(strcmp(prefixModeValue, "Static") == 0)
			sprintf(prefixOriginValue, "%s", "Static");
		else
			sprintf(prefixOriginValue, "%s", "None");
	}
	
	return getTheValue(value, type, deviceNode, prefixOriginValue);

}

int8 setCTComIPv6PrefixOriginValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixOriginValue[32] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0, ret = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 32)
		return -1;	

	ret = tcapi_get(nodeName, "Active", prefixOriginValue);
	if(ret){
		return -1;
	}

	sprintf(prefixOriginValue, "%s", (char*)(value));

	if(strcmp(prefixOriginValue, "PrefixDelegation") == 0)
	{
		tcapi_set(nodeName, "DHCPv6PD", "Yes");
		tcapi_set(nodeName, "PDOrigin", "PrefixDelegation");
	}
	else if(strcmp(prefixOriginValue, "RouterAdvertisement") == 0)
	{
		tcapi_set(nodeName, "DHCPv6PD", "No");
		tcapi_set(nodeName, "PDOrigin", "RouterAdv");
	}
	else if(strcmp(prefixOriginValue, "Static") == 0)
	{
		tcapi_set(nodeName, "DHCPv6PD", "No");
		tcapi_set(nodeName, "PDOrigin", "Static");
	}
	else if(strcmp(prefixOriginValue, "None") == 0)
	{
		tcapi_set(nodeName, "DHCPv6PD", "No");
		tcapi_set(nodeName, "PDOrigin", "None");
	}
	else
		return -1;

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getCTComIPv6PrefixOriginAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 pvcNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6PrefixOrigin[pvcNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCTComIPv6PrefixOriginAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 pvcNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6PrefixOrigin[pvcNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getCTComIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixValue[65] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "PDPrefix", prefixValue);

	return getTheValue(value, type, deviceNode, prefixValue);

}

int8 setCTComIPv6PrefixValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char prefixValue[65] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry, ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "Active", prefixValue);
	if(ret){
		return -1;
	}

	sprintf(prefixValue, "%s", (char*)(value));

	if(strlen(prefixValue) > 40)
		return -1;
	
	tcapi_set(nodeName, "PDPrefix", prefixValue);

	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;

}

int8 getCTComIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 pvcNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6Prefix[pvcNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCTComIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 pvcNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	if(pvcNodeIndex < 0 || pvcNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6Prefix[pvcNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6PrefixPltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char value_buf[32] = {'\0'};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if ( tcapi_get(nodeName, "PrefixPltime", value_buf) < 0 )
		strcpy(value_buf, "0");
	
	return getTheValue(value, type, deviceNode, value_buf);
}

int8 setIPv6PrefixPltimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char value_buf[64] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	uint32 timeValue = *((uint32*)value);
#else
	if(strlen((char*)value) > 40)
		return -1;
	strcpy(value_buf, (char*)value);
#endif

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	sprintf(value_buf, "%lu", timeValue);
#endif
	tcapi_set(nodeName, "PrefixPltime", value_buf);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getIPv6PrefixPltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6PrefixPltime[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6PrefixPltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6PrefixPltime[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6PrefixVltimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char value_buf[32] = {'\0'};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if ( tcapi_get(nodeName, "PrefixVltime", value_buf) < 0 )
		strcpy(value_buf, "0");
	
	return getTheValue(value, type, deviceNode, value_buf);
}

int8 setIPv6PrefixVltimeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char value_buf[64] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	uint32 timeValue = *((uint32*)value);
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	sprintf(value_buf, "%lu", timeValue);
#endif
	tcapi_set(nodeName, "PrefixVltime", value_buf);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getIPv6PrefixVltimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6PrefixVltime[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6PrefixVltimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6PrefixVltime[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6DefGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char GateWay6[64] = {'\0'};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "WanInfo_Entry%d", (pvcNodeIndex<<3 | pvcNodeEntry));

	tcapi_get(nodeName, "GateWay6", GateWay6);
	
	return getTheValue(value, type, deviceNode, GateWay6);
}

int8 setIPv6DefGatewayValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	unsigned char tmp[100] = {0};
	char value_buf[64] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	ret = tcapi_get(nodeName, "IPVERSION", value_buf);
	if(ret){
		return -1;
	}

	if(!strcmp(value_buf, "IPv4"))	//IPv4
		return -1;

	ret = tcapi_get(nodeName, "DHCPv6", value_buf);
	if(ret){
		return -1;
	}

	if(!strcmp(value_buf, "No"))	//SLAAC
		return -1;	

	if(strlen((char*)value) > 40)
		return -1;

	if(inet_pton6((const char*)value, tmp)!= 1)
	{
		tcdbg_printf("IPv6 Address Error.\n");
		return -1;	
	}

	sprintf(value_buf, "%s", (char*)value);

	tcapi_set(nodeName, "GATEWAY6", value_buf);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getIPv6DefGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6DefaultGateway[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6DefGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6DefaultGateway[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getIPv6DomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char value_buf[32] = {'\0'};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "IPv6DomainName", value_buf);
	
	return getTheValue(value, type, deviceNode, value_buf);
}

int8 setIPv6DomainNameValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char value_buf[72] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	if(strlen((char*)value) > 64)
		return -1;

	sprintf(value_buf, "%s", (char*)value);

	tcapi_set(nodeName, "IPv6DomainName", value_buf);
	
	SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	
	return 0;
}

int8 getIPv6DomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IPv6DomainName[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPv6DomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IPv6DomainName[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANCnt1_ResetValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char reset[4] = "0";
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "Reset", reset);
	if ( strcmp(reset, "1") )
	{
		strcpy(reset, "0");
	}
	
	return getTheValue(value, type, deviceNode, reset);
}
int8 setWANCnt1_ResetValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	uint8 reset = *((uint8*)value);	
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
	if(reset == 1)
		tcapi_set(nodeName, "Reset", "1");
	else if(reset == 1)
		tcapi_set(nodeName, "Reset", "0");
	else
		return -1;
	
	//SetCfgNodeFlag ( "Wan", (pvcNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );
	return 0;

}
int8 getWANCnt1_ResetAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_Reset[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANCnt1_ResetAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*	attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_Reset[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_ForwardModeEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};

	if ( tcapi_get("Wan_Common", "IPForwardModeEnable", s_enable) < 0 )
		strcpy(s_enable, "No");

	strcpy(s_enable, (0 == strcmp(s_enable, "Yes")) ? "1" : "0");
	return getTheValue(value, type, deviceNode, s_enable);
}
int8 setWANCnt1_ForwardModeEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};
	uint8 i_enable = *((uint8*)value);

	switch ( i_enable )
	{
		case 0:
			strcpy(s_enable, "No");
			break;
		case 1:
			if ( 0 != checkAllRouteMode() )
				return -1;
			strcpy(s_enable, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set("Wan_Common", "IPForwardModeEnable", s_enable);
	SetCfgNodeFlag("Wan", -1);
	return 0;
}
int8 getWANCnt1_ForwardModeEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aIPForwardModeEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANCnt1_ForwardModeEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aIPForwardModeEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_VlanEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char vlanenable[4] = "0";
	char mode[20] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "VLANMode", mode);
	if ( 0 == strcmp(mode, "TAG") )
	{
		strcpy(vlanenable, "1");
	}
	
	return getTheValue(value, type, deviceNode, vlanenable);
}
int8 setWANCnt1_VlanEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char modeStr[20] = {0};
	char oldmodeStr[20] = {0};
	char vlanid[12] = {0};
	char pbit[12] = {0};
	uint32 instant = 0;
	char *vid_untag = "4096";
	char *pbit_u = "0";
	uint8 mode = *((uint8*)value);
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindPVCMapping(&instant);
#else
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "VLANMode", oldmodeStr);
	if ( 0 == strcmp(oldmodeStr, "TAG") )
	{
		tcapi_get(nodeName, "VLANID", vlanid);
		tcapi_get(nodeName, "DOT1P", pbit);
	}

	switch ( mode )
	{
		case 0:
			strcpy(modeStr, "UNTAG");

			if ( 1 == isExistDuplicateChannel(modeStr, vlanid, pbit, instant) )
				return -1;
			updateVLANID_PBIT(modeStr, vid_untag, pbit_u, instant);
			break;
		case 1:
			strcpy(modeStr, "TAG");
			if ( 0 == strcmp("UNTAG", oldmodeStr)
				|| 0 == strcmp("TRANSPARENT", oldmodeStr) )
				updateVLANID_PBIT(modeStr, "", "", instant);
			break;
		default:
			return -1;			
	}

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_set(nodeName, "VLANMode", modeStr);
	SetCfgNodeFlag ( "Wan", instant );
	return 0;

}
int8 getWANCnt1_VlanEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_VlanEnable[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANCnt1_VlanEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*	attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_VlanEnable[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_VlanIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[36] = {0};
	char vlanid[20] = "0";
	char mode[20] = {0};
	uint32 pvcNodeIndex = 0, pvcNodeEntry = 0;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "VLANMode", mode);
	if ( 0 == strcmp(mode, "TAG") )
	{
		tcapi_get(nodeName, "VLANID", vlanid);
	}
	
	return getTheValue(value, type, deviceNode, vlanid);		

}
int8 setWANCnt1_VlanIDValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char vidStr[20] = {0};
	char pbitStr[20] = "0";
	char mode[20] = {0};
	uint32 instant = 0;
	uint32 vlanid = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindPVCMapping(&instant);
#else
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if ( instant >= MAX_WAN_NODE )
		return -1;

	if ( vlanid > 4094 )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "VLANMode", mode);
	if ( 0 != strcmp(mode, "TAG") )
		return -1;

	tcapi_get(nodeName, "DOT1P", pbitStr);
	sprintf(vidStr, "%u", vlanid);
	if ( 1 == isExistDuplicateChannel(mode, vidStr, pbitStr, instant) )
		return -1;

	tcapi_set(nodeName, "VLANID", vidStr);
	updateVLANID_PBIT(mode, vidStr, pbitStr, instant);
	SetCfgNodeFlag ( "Wan", instant );
	return 0;
}
int8 getWANCnt1_VlanIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_VlanID[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANCnt1_VlanIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*	attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_VlanID[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_PriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char priority[20] = "-1";
	char mode[20] = {0};
	uint32 instant = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindPVCMapping(&instant);
#else
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if ( instant >= MAX_WAN_NODE )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "VLANMode", mode);
	if ( 0 == strcmp(mode, "TAG") )
	{
		tcapi_get(nodeName, "DOT1P", priority);
	}

	return getTheValue(value, type, deviceNode, priority);
}
int8 setWANCnt1_PriorityValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char mode[20] = {0};
	char priorityStr[20] = {0};
	char vlanidStr[20] = {0};
	uint32 instant = 0;
	uint32 priValue = *((uint32*)value);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	FindPVCMapping(&instant);
#else
	instant = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
#endif	
	if ( instant >= MAX_WAN_NODE )
		return -1;

	if ( priValue > 7)
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "VLANMode", mode);
	if ( 0 != strcmp(mode, "TAG") )
		return -1;

	tcapi_get(nodeName, "VLANID", vlanidStr);
	sprintf(priorityStr, "%u", priValue);
	if ( 1 == isExistDuplicateChannel(mode, vlanidStr, priorityStr, instant) )
		return -1;

	tcapi_set(nodeName, "DOT1P", priorityStr);
	updateVLANID_PBIT(mode, vlanidStr, priorityStr, instant);
	SetCfgNodeFlag ( "Wan", instant );
	return 0;
}
int8 getWANCnt1_PriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_Priority[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANCnt1_PriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*	attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_Priority[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANCnt1_IgmpProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char IgmpProxy[20] = "0";
	char igmpIface[20] = {0};
	char wanIface[20] = {0};
	char igmpActive[8] = {0};
	
	uint32 pvcNodeIndex, pvcNodeEntry;

	tcapi_get("IGMPproxy_Entry", "Active", igmpActive);
	if(!strcmp(igmpActive, "Yes"))
	{
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
		pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
		pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;		
#endif	

		snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
		tcapi_get(nodeName, "IFName", wanIface);
		tcapi_get("IGMPproxy_Entry", "UpstreamIF", igmpIface);

		if(!strcmp(wanIface, igmpIface))
			strcpy(IgmpProxy,"1");		
	}
	
	return getTheValue(value, type, deviceNode, IgmpProxy);
}
int8 setWANCnt1_IgmpProxyValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	char value_buf[20] = {0};
	char svrlist[40] = {0};
	char wanMode[20] = {0};
	char wanIface[20] = {0};
	uint32 pvcNodeIndex, pvcNodeEntry;
	int ret;
	uint8 enl = *((uint8*)value);
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcNodeIndex, &pvcNodeEntry);
#else
	pvcNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	
	
	snprintf(nodeName, sizeof(nodeName), "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);

	tcapi_get(nodeName, "WanMode", wanMode);	
	if ( 0 == strcmp(wanMode, "Bridge") )
		return -1;
	
	tcapi_get(nodeName, "ServiceList", svrlist);
	if( (NULL==strstr(svrlist,"OTHER")) && (NULL==strstr(svrlist,"INTERNET")) )	
		return -1;
	
	if(enl==1)
		strcpy(value_buf,"Yes");
	else if(enl==0)
		strcpy(value_buf,"No");
	else
		return -1;	
	
	tcapi_set("IGMPproxy_Entry", "Active", value_buf);
	tcapi_set("MLDproxy_Entry", "Active", value_buf);
	
	if(enl==1)
	{
		tcapi_get(nodeName, "IFName", wanIface);
		tcapi_set("IGMPproxy_Entry", "UpstreamIF", wanIface);
		tcapi_set("MLDproxy_Entry", "UpstreamIF", wanIface);
	}
	
	SetCfgNodeFlag ( "IGMPproxy", -1 );
	SetCfgNodeFlag ( "MLDproxy", -1 );
	
	return 0;
}
int8 getWANCnt1_IgmpProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry =0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7||pvcNodeEntry <0 ||pvcNodeEntry >7){
		return -1;
	}
	
	attribute = tcIpConAttr.aWANCnt1_IgmpProxy[wanNodeIndex<<3|pvcNodeEntry];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANCnt1_IgmpProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*	attribute = NULL;
	int8  result = 0;
	uint32 wanNodeIndex = 0,pvcNodeEntry = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name) - 1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name) - 1;
#endif	

	if(wanNodeIndex < 0 || wanNodeIndex > 7 || pvcNodeEntry < 0 ||pvcNodeEntry > 7 ){
		return -1;
	}
	
	attribute = &tcIpConAttr.aWANCnt1_IgmpProxy[wanNodeIndex<<3 | pvcNodeEntry];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) && !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANPPPCnt1_1PPPoEBIValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	char pppbienable[8] = {0}, tmpv[8] = {0};

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);

	if ( 0 != tcapi_get(nodeName, "BridgeInterface", tmpv)
		|| 0 == tmpv[0] )
		strcpy(tmpv, "No");

	if ( 0 == strcmp("Yes", tmpv) )
		strcpy(pppbienable, "1");
	else
		strcpy(pppbienable, "0");

	return getTheValue(value, type, deviceNode, pppbienable);
}

	
int8 setWANPPPCnt1_1PPPoEBIValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char isp[10] = {0}, tmpv[16] = {0};
	uint32 wanNodeIndex = 0, pvcNodeEntry = 0;
	uint8 pppbiEnabled = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	pvcNodeEntry = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&wanNodeIndex, &pvcNodeEntry);
#else
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	pvcNodeEntry = atoi(deviceNode->parentNode->name)-1;
#endif	

	sprintf(nodeName, "Wan_PVC%d_Entry%d", wanNodeIndex, pvcNodeEntry);
	pppbiEnabled = *((uint8*)value);
	
	tcapi_get(nodeName, "ISP", isp);
	
	if ( 0 != strcmp(isp, WAN_ENCAP_PPP) )
	{
		//only pppoe/pppoa support this parameter.
		return -1;
	}

	if ( 1 == pppbiEnabled )
	{
#if defined(TCSUPPORT_CT_PPPOEPROXY)
		tcapi_set(nodeName, "pppProxyBiMode", "2");
		tcapi_set(nodeName, "ProxyEnable", "0");
#endif
		tcapi_set(nodeName, "BridgeInterface", "Yes");
	}
	else if( 0 == pppbiEnabled )
	{
#if defined(TCSUPPORT_CT_PPPOEPROXY)
		if ( 0 == tcapi_get(nodeName, "pppProxyBiMode", tmpv)
			&& 0 == strcmp(tmpv, "2") )
			tcapi_set(nodeName, "pppProxyBiMode", "0");
#endif
		tcapi_set(nodeName, "BridgeInterface", "No");
	}
	else
		return -1;

	SetCfgNodeFlag ( "Wan", (wanNodeIndex<<3|pvcNodeEntry) + COMMIT_THIRD_ALIAS );	
	return 0;
}
int8 getWANPPPCnt1_1PPPoEBIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpConAttr.aWANCnt1_PPPBIEnable[getWANConnectionIndex(deviceNode)];
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPPPCnt1_1PPPoEBIAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpConAttr.aWANCnt1_PPPBIEnable[getWANConnectionIndex(deviceNode)];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
int8 getWANPPPCnt1_1LErrorCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANCnt1_1LErrorCode[256] = {0}, wan_name[32] = {0};
	uint32 entryIndex = 0, pvcIndex = 0;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name);
	entryIndex = atoi(deviceNode->parentNode->name);
	FindWanNodeMapping(&pvcIndex, &entryIndex);
#else
	pvcIndex = atoi(deviceNode->parentNode->parentNode->parentNode->name)-1;
	entryIndex = atoi(deviceNode->parentNode->name)-1;
#endif

	sprintf(wan_name, "Wan_PVC%d_Entry%d", pvcIndex, entryIndex);
	tcapi_get(wan_name, "ConnectionErrorCode", WANCnt1_1LErrorCode);

	return getTheValue(value, type, deviceNode, WANCnt1_1LErrorCode);
}
int8 getWANPPPCnt1_1LErrorCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcIpConAttr.aWANPPPCnt1_1LastCntErrorCode[getWANConnectionIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
}
int8 setWANPPPCnt1_1LErrorCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcIpConAttr.aWANPPPCnt1_1LastCntErrorCode[getWANConnectionIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}
#endif

